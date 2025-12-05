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
/*!        \file  SoAd_Util.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component Util.
 *
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

/* PRQA S 1881 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA S 4304 EOF */ /* MD_MSR_AutosarBoolean */

#define SOAD_UTIL_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_Util.h"
#include "SoAd_Priv.h"
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
# include "Bmc.h"
#endif /* SOAD_MULTI_PARTITION == STD_ON */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (SOAD_LOCAL) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL static
#endif

#if !defined (SOAD_LOCAL_INLINE) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */ /*lint !e451 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_Util_CompareIpAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3206 2 */ /* MD_SoAd_UnusedParameter */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Util_CompareIpAddr(
  SoAd_DomainType Domain,
  SoAd_IpAddrConstPtrType IpAddrAPtr,
  SoAd_IpAddrConstPtrType IpAddrBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_IPV6 == STD_ON )
  /* #10 Compare IPv6 IP address (if enabled). */
  if ( Domain == SOAD_AF_INET6 )
  {
    /* #100 Compare both IP addresses. */
    if ( (IpAddrAPtr[0u] == IpAddrBPtr[0u]) &&
      (IpAddrAPtr[1u] == IpAddrBPtr[1u]) &&
      (IpAddrAPtr[2u] == IpAddrBPtr[2u]) &&
      (IpAddrAPtr[3u] == IpAddrBPtr[3u]) )
    {
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
#else
  SOAD_DUMMY_STATEMENT(Domain); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_IPV6 == STD_ON */
  /* #20 Compare IPv4 IP address otherwise. */
  {
    /* #200 Compare both IP addresses. */
    if ( IpAddrAPtr[0u] == IpAddrBPtr[0u] )
    {
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* SoAd_Util_CompareIpAddr() */

#if ( SOAD_TX_DYN_LEN == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Util_UpdatePduHdrLen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_Util_UpdatePduHdrLen(
  SoAd_PduHdrLenType PduHeaderLen,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update PDU header length by overwriting the buffer with offset of PDU header ID. */
  BufPtr[SOAD_PDU_HDR_ID_SIZE]      = (uint8)(PduHeaderLen >> 24u);                                                    /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
  BufPtr[SOAD_PDU_HDR_ID_SIZE + 1u] = (uint8)(PduHeaderLen >> 16u);                                                    /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
  BufPtr[SOAD_PDU_HDR_ID_SIZE + 2u] = (uint8)(PduHeaderLen >> 8u);                                                     /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
  BufPtr[SOAD_PDU_HDR_ID_SIZE + 3u] = (uint8)(PduHeaderLen);                                                           /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
} /* SoAd_Util_UpdatePduHdrLen() */
#endif /* SOAD_TX_DYN_LEN == STD_ON */

/**********************************************************************************************************************
 *  SoAd_Util_CopySockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_Util_CopySockAddr(
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) TgtSockAddrPtr,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) SrcSockAddrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy domain type. */
  TgtSockAddrPtr->domain = SrcSockAddrPtr->domain;                                                                     /* SBSW_SOAD_POINTER_WRITE */

  /* #20 Copy IP address depending on domain type (IPv4/6). */
#if ( SOAD_IPV6 == STD_ON )
  if ( SrcSockAddrPtr->domain == SOAD_AF_INET6 )
  {
    /* PRQA S 310, 3305 8 */ /* MD_SoAd_PointerCastOnStruct */
    (SOAD_A_P2VAR(SoAd_SockAddrInet6Type)TgtSockAddrPtr)->addr[0u] =                                                   /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_WRITE */
      (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcSockAddrPtr)->addr[0u];
    (SOAD_A_P2VAR(SoAd_SockAddrInet6Type)TgtSockAddrPtr)->addr[1u] =                                                   /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_WRITE */
      (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcSockAddrPtr)->addr[1u];
    (SOAD_A_P2VAR(SoAd_SockAddrInet6Type)TgtSockAddrPtr)->addr[2u] =                                                   /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_WRITE */
      (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcSockAddrPtr)->addr[2u];
    (SOAD_A_P2VAR(SoAd_SockAddrInet6Type)TgtSockAddrPtr)->addr[3u] =                                                   /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_WRITE */
      (SOAD_A_P2CONST(SoAd_SockAddrInet6Type)SrcSockAddrPtr)->addr[3u];
  }
  else
#endif /* SOAD_IPV6 == STD_ON */
  {
    /* PRQA S 310, 3305 2 */ /* MD_SoAd_PointerCastOnStruct */
    (SOAD_A_P2VAR(SoAd_SockAddrInetType)TgtSockAddrPtr)->addr[0u] =                                                    /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_WRITE */
      (SOAD_A_P2CONST(SoAd_SockAddrInetType)SrcSockAddrPtr)->addr[0u];
  }

  /* #30 Copy port. */
  /* PRQA S 310, 3305 2 */ /* MD_SoAd_PointerCastOnStruct */
  (SOAD_A_P2VAR(SoAd_SockAddrInetXType)TgtSockAddrPtr)->port =                                                         /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_WRITE */
    (SOAD_A_P2CONST(SoAd_SockAddrInetXType)SrcSockAddrPtr)->port;
} /* SoAd_Util_CopySockAddr() */

/**********************************************************************************************************************
 *  SoAd_Util_CopyPduHdr2Buf()
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
FUNC(void, SOAD_CODE) SoAd_Util_CopyPduHdr2Buf(
  SoAd_PduHdrIdType PduHeaderId,
  SoAd_PduHdrLenType PduHeaderLen,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  PduLengthType Offset,
  PduLengthType Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType pduHdrIdx;
  PduLengthType pduHdrEndIdx = Offset + Length;
  uint8         pduHdrBuf[SOAD_PDU_HDR_SIZE];

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if entire PDU header shall be copied. */
  if ( Length == SOAD_PDU_HDR_SIZE )
  {
    /* #100 Copy entire PDU header to provided buffer. */
    BufPtr[0u] = (uint8)(PduHeaderId >> 24u);                                                                          /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    BufPtr[1u] = (uint8)(PduHeaderId >> 16u);                                                                          /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    BufPtr[2u] = (uint8)(PduHeaderId >> 8u);                                                                           /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    BufPtr[3u] = (uint8)(PduHeaderId);                                                                                 /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    BufPtr[4u] = (uint8)(PduHeaderLen >> 24u);                                                                         /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    BufPtr[5u] = (uint8)(PduHeaderLen >> 16u);                                                                         /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    BufPtr[6u] = (uint8)(PduHeaderLen >> 8u);                                                                          /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    BufPtr[7u] = (uint8)(PduHeaderLen);                                                                                /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
  }
  /* #11 Handle PDU header segmentation otherwise. */
  else
  {
    /* #110 Fill local buffer with PDU header. */
    pduHdrBuf[0u] = (uint8)(PduHeaderId >> 24u);                                                                       /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    pduHdrBuf[1u] = (uint8)(PduHeaderId >> 16u);                                                                       /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    pduHdrBuf[2u] = (uint8)(PduHeaderId >> 8u);                                                                        /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    pduHdrBuf[3u] = (uint8)(PduHeaderId);                                                                              /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    pduHdrBuf[4u] = (uint8)(PduHeaderLen >> 24u);                                                                      /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    pduHdrBuf[5u] = (uint8)(PduHeaderLen >> 16u);                                                                      /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    pduHdrBuf[6u] = (uint8)(PduHeaderLen >> 8u);                                                                       /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    pduHdrBuf[7u] = (uint8)(PduHeaderLen);                                                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */

    /* #111 Copy PDU header segment from local buffer to provided buffer. */
    for ( pduHdrIdx = Offset; pduHdrIdx < pduHdrEndIdx; pduHdrIdx++ )
    {
      BufPtr[pduHdrIdx - Offset] = pduHdrBuf[pduHdrIdx];                                                               /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    }
  }
} /* SoAd_Util_CopyPduHdr2Buf() */

/**********************************************************************************************************************
 *  SoAd_Util_GetSupportedPduLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(PduLengthType, SOAD_CODE) SoAd_Util_GetSupportedPduLength(
  uint32 Length,
  boolean IsUdp)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType supportedPduLength;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get max supported length. */
  uint32 maxSupportedLen = IsUdp ? (uint32)SOAD_MAX_UDP_PDU_LEN : SOAD_MAX_PDU_LEN;

  /* #20 Return maximum supported PDU length if requested length exceeds maximum supported PDU length. */
  if ( Length > maxSupportedLen )
  {
    supportedPduLength = (PduLengthType)maxSupportedLen;
  }
  else
  /* #21 Return requested PDU length otherwise. */
  {
    supportedPduLength = (PduLengthType)Length;
  }

  return supportedPduLength;
} /* SoAd_Util_GetSupportedPduLength() */

/**********************************************************************************************************************
 *  SoAd_Util_LoadU8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, SOAD_CODE) SoAd_Util_LoadU8(
  SoAd_AtomicUint8ConstPtrType VariablePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* #10 Load the value in multi-partition use-case. */
  retVal = VariablePtr->Aligned.Value;
#else
  /* #20 Simply return the value otherwise. */
  retVal = *VariablePtr;
#endif /* SOAD_MULTI_PARTITION == STD_ON */

  return retVal;
} /* SoAd_Util_LoadU8() */

/**********************************************************************************************************************
 *  SoAd_Util_LoadU32()
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
FUNC(uint32, SOAD_CODE) SoAd_Util_LoadU32(
  SoAd_AtomicUint32ConstPtrType VariablePtr,
  boolean UseAtomicAccess)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 retVal;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* #10 Check if atomic access is needed to load the value for multi-partition use-case. */
  if ( UseAtomicAccess )
  {
# if ( SOAD_BMC_USAGE_ENABLED == STD_ON ) /* COV_SOAD_QM_FEATURE_BMC_USAGE */
    /* #20 Use Bmc API if enabled. */
    retVal = Bmc_Load_u32(&VariablePtr->Aligned.Value);                                                                /* SBSW_SOAD_POINTER_FORWARD */
# else
    /* #30 Use critical sections for protection otherwise. */
    SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION();
    retVal = VariablePtr->Aligned.Value;
    SOAD_END_CRITICAL_SECTION_MULTI_PARTITION();
# endif /* SOAD_BMC_USAGE_ENABLED == STD_ON */
  }
  /* #40 Load the value without atomic access otherwise for multi-partition use-case. */
  else
  {
    retVal = VariablePtr->Aligned.Value;
  }
#else
  /* #50 Simply return the value for single-partition use-case. */
  retVal = *VariablePtr;
  SOAD_DUMMY_STATEMENT(UseAtomicAccess); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_MULTI_PARTITION == STD_ON */

  return retVal;
} /* SoAd_Util_LoadU32() */

/**********************************************************************************************************************
 *  SoAd_Util_StoreU8()
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
FUNC(void, SOAD_CODE) SoAd_Util_StoreU8(
  SoAd_AtomicUint8PtrType VariablePtr,
  uint8 DesiredValue,
  boolean UseAtomicAccess)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* #10 Check if atomic access is needed to store the desired value for multi-partition use-case. */
  if ( UseAtomicAccess == TRUE )
  {
# if ( SOAD_BMC_USAGE_ENABLED == STD_ON ) /* COV_SOAD_QM_FEATURE_BMC_USAGE */
    /* #20 Use Bmc API if enabled. */
    Bmc_Store_u8(&VariablePtr->Aligned.Value, DesiredValue);                                                           /* SBSW_SOAD_POINTER_FORWARD */
# else
    /* #30 Use critical sections for protection otherwise. */
    SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION();
    VariablePtr->Aligned.Value = DesiredValue;                                                                         /* SBSW_SOAD_POINTER_WRITE */
    SOAD_END_CRITICAL_SECTION_MULTI_PARTITION();
# endif /* SOAD_BMC_USAGE_ENABLED == STD_ON */
  }
  /* #40 Store the desired value without atomic access otherwise for multi-partition use-case. */
  else
  {
    VariablePtr->Aligned.Value = DesiredValue;                                                                         /* SBSW_SOAD_POINTER_WRITE */
  }
#else
  /* #50 Simply store the desired value for single-partition use-case. */
  *VariablePtr = DesiredValue;                                                                                         /* SBSW_SOAD_POINTER_WRITE */

  SOAD_DUMMY_STATEMENT(UseAtomicAccess); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_MULTI_PARTITION == STD_ON */
} /* SoAd_Util_StoreU8() */

/**********************************************************************************************************************
 *  SoAd_Util_StoreU32()
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
FUNC(void, SOAD_CODE) SoAd_Util_StoreU32(
  SoAd_AtomicUint32PtrType VariablePtr,
  uint32 DesiredValue,
  boolean UseAtomicAccess)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* #10 Check if atomic access is needed to store the desired value for multi-partition use-case. */
  if ( UseAtomicAccess == TRUE )
  {
# if ( SOAD_BMC_USAGE_ENABLED == STD_ON ) /* COV_SOAD_QM_FEATURE_BMC_USAGE */
    /* #20 Use Bmc API if enabled. */
    Bmc_Store_u32(&VariablePtr->Aligned.Value, DesiredValue);                                                          /* SBSW_SOAD_POINTER_FORWARD */
# else
    /* #30 Use critical sections for protection otherwise. */
    SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION();
    VariablePtr->Aligned.Value = DesiredValue;                                                                         /* SBSW_SOAD_POINTER_WRITE */
    SOAD_END_CRITICAL_SECTION_MULTI_PARTITION();
# endif /* SOAD_BMC_USAGE_ENABLED == STD_ON */
  }
  /* #40 Store the desired value without atomic access otherwise for multi-partition use-case. */
  else
  {
    VariablePtr->Aligned.Value = DesiredValue;                                                                         /* SBSW_SOAD_POINTER_WRITE */
  }
#else
  /* #50 Simply store the desired value for single-partition use-case. */
  *VariablePtr = DesiredValue;                                                                                         /* SBSW_SOAD_POINTER_WRITE */

  SOAD_DUMMY_STATEMENT(UseAtomicAccess); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_MULTI_PARTITION == STD_ON */
} /* SoAd_Util_StoreU32() */

/**********************************************************************************************************************
 *  SoAd_Util_FetchAddU32()
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
FUNC(void, SOAD_CODE) SoAd_Util_FetchAddU32(
  SoAd_AtomicUint32PtrType VariablePtr,
  uint32 Count,
  boolean UseAtomicAccess)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* #10 Check if atomic access is needed to add the desired count to the current value for multi-partition
         use-case. */
  if ( UseAtomicAccess == TRUE )
  {
# if ( SOAD_BMC_USAGE_ENABLED == STD_ON ) /* COV_SOAD_QM_FEATURE_BMC_USAGE */
    /* #20 Use Bmc API if enabled. */
    (void)Bmc_FetchAdd_u32(&VariablePtr->Aligned.Value, Count);                                                        /* SBSW_SOAD_POINTER_FORWARD */
# else
    /* #30 Use critical sections for protection otherwise. */
    SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION();
    VariablePtr->Aligned.Value = VariablePtr->Aligned.Value + Count;                                                   /* SBSW_SOAD_POINTER_WRITE */
    SOAD_END_CRITICAL_SECTION_MULTI_PARTITION();
# endif /* SOAD_BMC_USAGE_ENABLED == STD_ON */
  }
  /* #40 Add the desired count to the current value without atomic access otherwise for multi-partition use-case. */
  else
  {
    VariablePtr->Aligned.Value = VariablePtr->Aligned.Value + Count;                                                   /* SBSW_SOAD_POINTER_WRITE */
  }
#else
  /* #50 Simply add the desired count to the current value for single-partition use-case. */
  *VariablePtr = *VariablePtr + Count;                                                                                 /* SBSW_SOAD_POINTER_WRITE */
  SOAD_DUMMY_STATEMENT(UseAtomicAccess); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_MULTI_PARTITION == STD_ON */
} /* SoAd_Util_FetchAddU32() */

/**********************************************************************************************************************
 *  SoAd_Util_FetchSubU32()
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
FUNC(void, SOAD_CODE) SoAd_Util_FetchSubU32(
  SoAd_AtomicUint32PtrType VariablePtr,
  uint32 Count)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
# if ( SOAD_BMC_USAGE_ENABLED == STD_ON ) /* COV_SOAD_QM_FEATURE_BMC_USAGE */
  /* #10 Subtract the desired count from the current value by usage of Bmc API if enabled and multi-partition is
         used. */
  (void)Bmc_FetchSub_u32(&VariablePtr->Aligned.Value, Count);                                                          /* SBSW_SOAD_POINTER_FORWARD */
# else
  /* #20 Subtract the desired count from the current value by usage of critical sections in multi-partition use-case
         otherwise. */
  SOAD_BEGIN_CRITICAL_SECTION_MULTI_PARTITION();
  VariablePtr->Aligned.Value = VariablePtr->Aligned.Value - Count;                                                     /* SBSW_SOAD_POINTER_WRITE */
  SOAD_END_CRITICAL_SECTION_MULTI_PARTITION();
# endif /* SOAD_BMC_USAGE_ENABLED == STD_ON */
#else
  /* #30 Simply subtract the desired count from the current value for single-partition use-case. */
  *VariablePtr = *VariablePtr - Count;                                                                                 /* SBSW_SOAD_POINTER_WRITE */
#endif /* SOAD_MULTI_PARTITION == STD_ON */
} /* SoAd_Util_FetchSubU32() */

#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
/**********************************************************************************************************************
 *  SoAd_Util_CheckApplicationIdValidity()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Util_CheckApplicationIdValidity(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ApplicationType osApplicationId = (ApplicationType)GetApplicationID();
  Std_ReturnType  retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the OS application identifier contains a valid value. */
  if ( osApplicationId >= INVALID_OSAPPLICATION )
  {
    retVal = E_NOT_OK;
  }
  /* #20 Check if the mapped OS application ID of the partition does not match to the OS application ID otherwise. */
  else if ( SoAd_GetOsApplicationIdOfPartition(0u, PartitionIdx) != osApplicationId )
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_Util_CheckApplicationIdValidity() */
# endif /* SOAD_MULTI_PARTITION == STD_ON */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

/**********************************************************************************************************************
 *  SoAd_Util_GetNextArrayElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint32, SOAD_CODE) SoAd_Util_GetNextArrayElement(
  uint32 CurrentFirstIdx,
  uint32 FillLevel,
  uint32 FirstValidIdx,
  uint32 FirstInvalidIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 nextIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate next array index in case of wrap around. */
  if ( (CurrentFirstIdx + FillLevel) >= FirstInvalidIdx )
  {
    nextIdx = CurrentFirstIdx + FillLevel + FirstValidIdx - FirstInvalidIdx;
  }
  /* #20 Calculate next array index in case of no wrap around otherwise. */
  else
  {
    nextIdx = CurrentFirstIdx + FillLevel;
  }

  return nextIdx;
} /* SoAd_Util_GetNextArrayElement() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */ /*lint !e451 */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Util.c
 *********************************************************************************************************************/
