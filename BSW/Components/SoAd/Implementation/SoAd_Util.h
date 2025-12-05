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
/*!        \file  SoAd_Util.h
 *        \brief  Socket Adaptor header file
 *
 *      \details  Vector static code header file for AUTOSAR Socket Adaptor sub-component Util.
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

#if !defined (SOAD_UTIL_H)
# define SOAD_UTIL_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "SoAd.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

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

# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_Util_CompareIpAddr()
 *********************************************************************************************************************/
/*! \brief        Compares two IP addresses.
 *  \details      -
 *  \param[in]    Domain             IP domain (IPv4/6).
 *  \param[in]    IpAddrAPtr         Pointer to first IP address.
 *  \param[in]    IpAddrBPtr         Pointer to second IP address.
 *  \return       E_OK               IP addresses are equal.
 *  \return       E_NOT_OK           IP addresses are not equal.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Util_CompareIpAddr(
  SoAd_DomainType Domain,
  SoAd_IpAddrConstPtrType IpAddrAPtr,
  SoAd_IpAddrConstPtrType IpAddrBPtr);

# if ( SOAD_TX_DYN_LEN == STD_ON )
/**********************************************************************************************************************
 *  SoAd_Util_UpdatePduHdrLen
 *********************************************************************************************************************/
/*! \brief        Updates PDU header length field with a new length.
 *  \details      -
 *  \param[in]    PduHeaderLen       New length of PDU header.
 *  \param[in]    BufPtr             Pointer to first byte of PDU header.
 *                                   [range: at least PDU header length]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \config       SOAD_TX_DYN_LEN
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Util_UpdatePduHdrLen(
  SoAd_PduHdrLenType PduHeaderLen,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr);
# endif /* SOAD_TX_DYN_LEN == STD_ON */

/**********************************************************************************************************************
 *  SoAd_Util_CopySockAddr
 *********************************************************************************************************************/
/*! \brief        Copies a source socket address to a target socket address struct.
 *  \details      -
 *  \param[out]   TgtSockAddrPtr     Pointer to target socket address.
 *                                   [Points to one of the following structs depending on value of struct element
 *                                   domain of parameter SrcSockAddrPtr (expecting SoAd_SockAddrInet6Type >
 *                                   SoAd_SockAddrInetType so TgtSockAddrPtr may point to SoAd_SockAddrInet6Type even
 *                                   if domain of SrcSockAddrPtr indicates SoAd_SockAddrInetType):
 *                                     - SoAd_SockAddrInetType for IPv4
 *                                     - SoAd_SockAddrInet6Type for IPv6]
 *  \param[in]    SrcSockAddrPtr     Pointer to source socket address.
 *                                   [Points to one of the following structs depending on value of struct element
 *                                   domain:
 *                                     - SoAd_SockAddrInetType for IPv4
 *                                     - SoAd_SockAddrInet6Type for IPv6]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Util_CopySockAddr(
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) TgtSockAddrPtr,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) SrcSockAddrPtr);

/**********************************************************************************************************************
 *  SoAd_Util_CopyPduHdr2Buf
 *********************************************************************************************************************/
/*! \brief        Copies a PDU header to a buffer.
 *  \details      -
 *  \param[in]    PduHeaderId        PDU header identifier.
 *  \param[in]    PduHeaderLen       PDU header length.
 *  \param[out]   BufPtr             Pointer to buffer.
 *  \param[in]    Offset             Offset in PDU header.
 *                                   [range: Offset < SOAD_PDU_HDR_SIZE]
 *  \param[in]    Length             Length of PDU header segment to be copied.
 *                                   [range: Length <= (SOAD_PDU_HDR_SIZE - Offset)]
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Util_CopyPduHdr2Buf(
  SoAd_PduHdrIdType PduHeaderId,
  SoAd_PduHdrLenType PduHeaderLen,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  PduLengthType Offset,
  PduLengthType Length);

/**********************************************************************************************************************
 *  SoAd_Util_GetSupportedPduLength()
 *********************************************************************************************************************/
/*! \brief        Returns a requested value if it fits into the PDU length type or returns the maximum PDU length.
 *  \details      -
 *  \param[in]    Length             Length to be checked.
 *  \param[in]    IsUdp              Flag that indicates if UDP is used.
 *  \return       SupportedPduLength Returns value of parameter Length or maximum PDU length.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(PduLengthType, SOAD_CODE) SoAd_Util_GetSupportedPduLength(
  uint32 Length,
  boolean IsUdp);

/**********************************************************************************************************************
 *  SoAd_Util_LoadU8()
 *********************************************************************************************************************/
/*! \brief        Loads the value pointed to by the handed over pointer and returns its value.
 *  \details      -
 *  \param[in]    VariablePtr        The pointer to the variable to be returned.
 *                                   [range: pointer should not be null pointer]
 *  \return       LoadedValue        Value of the pointed to variable.
 *  \pre          Caller must grant atomic access if needed.
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(uint8, SOAD_CODE) SoAd_Util_LoadU8(
  SoAd_AtomicUint8ConstPtrType VariablePtr);

/**********************************************************************************************************************
 *  SoAd_Util_LoadU32()
 *********************************************************************************************************************/
/*! \brief        Loads the value pointed to by the handed over pointer and returns its value by usage of the Bmc load
 *                uint32 API if enabled and atomic access is necessary.
 *  \details      Depending on Bmc usage and if atomic access is necessary different mechanisms are used to load the
 *                value.
 *  \param[in]    VariablePtr        The pointer to the variable to be returned.
 *                                   [range: pointer should not be null pointer]
 *  \param[in]    UseAtomicAccess    Flag to grant atomic access in multi-partition use-case without Bmc usage.
 *  \return       LoadedValue        Value of the pointed to variable.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(uint32, SOAD_CODE) SoAd_Util_LoadU32(
  SoAd_AtomicUint32ConstPtrType VariablePtr,
  boolean UseAtomicAccess);

/**********************************************************************************************************************
 *  SoAd_Util_StoreU8()
 *********************************************************************************************************************/
/*! \brief        Replaces the value pointed to by the handed over pointer by usage of the Bmc store uint8 API if
 *                enabled and atomic access is necessary.
 *  \details      Depending on Bmc usage and if atomic access is necessary different mechanisms are used to store the
 *                value.
 *  \param[in]    VariablePtr        The pointer to the variable to be changed.
 *                                   [range: pointer should not be null pointer]
 *  \param[in]    DesiredValue       The desired value to be stored.
 *  \param[in]    UseAtomicAccess    Flag to grant atomic access in multi-partition use-case without Bmc usage.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Util_StoreU8(
  SoAd_AtomicUint8PtrType VariablePtr,
  uint8 DesiredValue,
  boolean UseAtomicAccess);

/**********************************************************************************************************************
 *  SoAd_Util_StoreU32()
 *********************************************************************************************************************/
/*! \brief        Replaces the value pointed to by the handed over pointer by usage of the Bmc store uint32 API if
 *                enabled and atomic access is necessary.
 *  \details      Depending on Bmc usage and if atomic access is necessary different mechanisms are used to store the
 *                value.
 *  \param[in]    VariablePtr        The pointer to the variable to be changed.
 *                                   [range: pointer should not be null pointer]
 *  \param[in]    DesiredValue       The desired value to be stored.
 *  \param[in]    UseAtomicAccess    Flag to grant atomic access in multi-partition use-case.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Util_StoreU32(
  SoAd_AtomicUint32PtrType VariablePtr,
  uint32 DesiredValue,
  boolean UseAtomicAccess);

/**********************************************************************************************************************
 *  SoAd_Util_FetchAddU32()
 *********************************************************************************************************************/
/*! \brief        Fetches the value pointed to by the handed over pointer and adds uint32 value by usage of the Bmc
 *                fetch and add API if enabled and atomic access is necessary.
 *  \details      Depending on Bmc usage and if atomic access is necessary different mechanisms are used to fetch and
 *                add the value.
 *  \param[in]    VariablePtr        The pointer to the variable to be incremented.
 *                                   [range: pointer should not be null pointer]
 *  \param[in]    Count              The count of increments to be made.
 *  \param[in]    UseAtomicAccess    Flag to grant atomic access in multi-partition use-case without Bmc usage.
 *  \pre          Caller must assert that the adding operation does not lead to an overflow.
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Util_FetchAddU32(
  SoAd_AtomicUint32PtrType VariablePtr,
  uint32 Count,
  boolean UseAtomicAccess);

/**********************************************************************************************************************
 *  SoAd_Util_FetchSubU32()
 *********************************************************************************************************************/
/*! \brief        Fetches the value pointed to by the handed over pointer and subtracts uint32 value by usage of the
 *                Bmc fetch and subtract API if enabled.
 *  \details      Critical section (spin-lock) is used for protection if Bmc is disabled.
 *  \param[in]    VariablePtr        The pointer to the variable to be decremented.
 *                                   [range: pointer should not be null pointer]
 *  \param[in]    Count              The count of decrements to be made.
 *  \pre          Caller must assert that the subtraction operation does not lead to an underflow.
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Util_FetchSubU32(
  SoAd_AtomicUint32PtrType VariablePtr,
  uint32 Count);

#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
/**********************************************************************************************************************
 *  SoAd_Util_CheckApplicationIdValidity()
 *********************************************************************************************************************/
/*! \brief        Checks the validity of the OS application identifier and the SoAd partition index.
 *  \details      -
 *  \pre          -
 *  \param[in]    PartitionIdx       Partition index.
 *                                   [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK               Application identifier is valid and matches to the partition index.
 *  \return       E_NOT_OK           Application identifier does not match to partition index.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       SOAD_DEV_ERROR_DETECT & SOAD_MULTI_PARTITION
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Util_CheckApplicationIdValidity(
  SoAd_PartitionConfigIdxType PartitionIdx);
# endif /* SOAD_MULTI_PARTITION == STD_ON */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */

/**********************************************************************************************************************
 *  SoAd_Util_GetNextArrayElement()
 *********************************************************************************************************************/
/*! \brief        Gets the next array element considering wrap around and the given indexes.
 *  \details      The next index is always calculated and returned. The API does not consider if the next calculated
 *                index is free or not, so this must be asserted by the caller.
 *  \pre          -
 *  \param[in]    CurrentFirstIdx    The index of the first used array element.
 *  \param[in]    FillLevel          The current fill level.
 *  \param[in]    FirstValidIdx      The first valid index in the whole array (StartIdx).
 *  \param[in]    FirstInvalidIdx    The first invalid index in the whole array (EndIdx).
 *  \return       NextIndex          The next index considering all given parameters.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(uint32, SOAD_CODE) SoAd_Util_GetNextArrayElement(
  uint32 CurrentFirstIdx,
  uint32 FillLevel,
  uint32 FirstValidIdx,
  uint32 FirstInvalidIdx);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SOAD_UTIL_H */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Util.h
 *********************************************************************************************************************/
