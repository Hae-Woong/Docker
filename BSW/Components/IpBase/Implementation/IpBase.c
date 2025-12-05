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
/*!        \file  IpBase.c
 *         \unit  GetVersionInfo
 *        \brief  IpBase source file
 *      \details  IpBase version information. 
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
#define IPBASE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/*lint -e451 */ /* LD_IpBase_451 */
#include "IpBase.h"           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#include "IpBase_Priv.h"

/**********************************************************************************************************************
 *  CONSISTENCY CHECK
 *********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (IPBASE_SW_MAJOR_VERSION != (4u)) \
   || (IPBASE_SW_MINOR_VERSION != (2u)) \
   || (IPBASE_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of IpBase.c and IpBase.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define IPBASE_START_SEC_CODE
#include "IpBase_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  IpBase_GetVersionInfo
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
IPBASE_FUNCCODE(void) IpBase_GetVersionInfo( IPBASE_P2VARAPPLDATA(Std_VersionInfoType) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check VersionInfoPtr for NULL pointer */
  if ( VersionInfoPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set vendor ID, module ID, major+minor+patch version */
    VersionInfoPtr->vendorID = (IPBASE_VENDOR_ID);                /* SBSW_IPBASE_GETVERSION_PARAM */
    VersionInfoPtr->moduleID = (IPBASE_MODULE_ID);                /* SBSW_IPBASE_GETVERSION_PARAM */
    VersionInfoPtr->sw_major_version = (IPBASE_SW_MAJOR_VERSION); /* SBSW_IPBASE_GETVERSION_PARAM */
    VersionInfoPtr->sw_minor_version = (IPBASE_SW_MINOR_VERSION); /* SBSW_IPBASE_GETVERSION_PARAM */
    VersionInfoPtr->sw_patch_version = (IPBASE_SW_PATCH_VERSION); /* SBSW_IPBASE_GETVERSION_PARAM */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_GET_VERSION_INFO, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
} /* IpBase_GetVersionInfo() */
/**********************************************************************************************************************
 * IpBase_ByteSwap16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
# if (IPBASE_ENABLE_INLINE_FUNCTIONS != STD_ON)
IPBASE_FUNCCODE(uint16) IpBase_ByteSwap16(uint16 Data) /* PRQA S 3219 */ /* MD_MSR_Unreachable */
{
  return (uint16)(((Data & 0xFF00u) >> 8u) | ((Data & 0x00FFu) << 8u));
}
/**********************************************************************************************************************
 * IpBase_ByteSwap32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPBASE_FUNCCODE(uint32) IpBase_ByteSwap32(uint32 Data) /* PRQA S 3219 */ /* MD_MSR_Unreachable */
{
  return (uint32)(((Data & 0xFF000000u) >> 24u) | ((Data & 0x000000FFu) << 24u) |
                  ((Data & 0x00FF0000u) >>  8u) | ((Data & 0x0000FF00u) <<  8u));
}
/**********************************************************************************************************************
 * IpBase_ByteSwap64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPBASE_FUNCCODE(uint64) IpBase_ByteSwap64(uint64 Data) /* PRQA S 3219 */ /* MD_MSR_Unreachable */
{
  return (uint64)(((Data & 0xFF00000000000000ULL) >> 56u) | ((Data & 0x00000000000000FFULL) << 56u) |
                  ((Data & 0x00FF000000000000ULL) >> 40u) | ((Data & 0x000000000000FF00ULL) << 40u) |
                  ((Data & 0x0000FF0000000000ULL) >> 24u) | ((Data & 0x0000000000FF0000ULL) << 24u) |
                  ((Data & 0x000000FF00000000ULL) >>  8u) | ((Data & 0x00000000FF000000ULL) <<  8u));
}
/**********************************************************************************************************************
 * IpBase_PutUint8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPBASE_FUNCCODE(void) IpBase_PutUint8(IPBASE_CONSTP2VARAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                      uint32_least Offset, uint8 Value)
{
  BufferPtr[Offset] = Value; /* SBSW_IPBASE_API_ARGUMENT_IDX */
}
/**********************************************************************************************************************
 * IpBase_PutUint16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPBASE_FUNCCODE(void) IpBase_PutUint16(IPBASE_CONSTP2VARAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                       uint32_least Offset, uint16 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 8); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 1u] = (uint8)(Value);      /* SBSW_IPBASE_API_ARGUMENT_IDX */
}
/**********************************************************************************************************************
 * IpBase_PutUint24()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
IPBASE_FUNCCODE(void) IpBase_PutUint24(IPBASE_CONSTP2VARAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                       uint32_least Offset, uint32 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 16u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 1u] = (uint8)(Value >> 8u);  /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 2u] = (uint8)(Value);        /* SBSW_IPBASE_API_ARGUMENT_IDX */
}
/**********************************************************************************************************************
 * IpBase_PutUint32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPBASE_FUNCCODE(void) IpBase_PutUint32(IPBASE_CONSTP2VARAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                       uint32_least Offset, uint32 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 24u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 1u] = (uint8)(Value >> 16u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 2u] = (uint8)(Value >> 8u);  /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 3u] = (uint8)(Value);        /* SBSW_IPBASE_API_ARGUMENT_IDX */
}
/**********************************************************************************************************************
 * IpBase_PutUint48()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
IPBASE_FUNCCODE(void) IpBase_PutUint48(IPBASE_CONSTP2VARAPPLDATA(uint8) BufferPtr,  /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                       uint32_least Offset, uint64 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 40u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 1u] = (uint8)(Value >> 32u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 2u] = (uint8)(Value >> 24u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 3u] = (uint8)(Value >> 16u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 4u] = (uint8)(Value >> 8u);  /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 5u] = (uint8)(Value);        /* SBSW_IPBASE_API_ARGUMENT_IDX */
}
/**********************************************************************************************************************
 * IpBase_PutUint64()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
IPBASE_FUNCCODE(void) IpBase_PutUint64(IPBASE_CONSTP2VARAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                       uint32_least Offset, uint64 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 56u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 1u] = (uint8)(Value >> 48u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 2u] = (uint8)(Value >> 40u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 3u] = (uint8)(Value >> 32u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 4u] = (uint8)(Value >> 24u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 5u] = (uint8)(Value >> 16u); /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 6u] = (uint8)(Value >> 8u);  /* SBSW_IPBASE_API_ARGUMENT_IDX */
  BufferPtr[Offset + 7u] = (uint8)(Value);        /* SBSW_IPBASE_API_ARGUMENT_IDX */
}
/**********************************************************************************************************************
 * IpBase_GetUint8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPBASE_FUNCCODE(uint8) IpBase_GetUint8(IPBASE_CONSTP2CONSTAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                       uint32_least Offset)
{
  return BufferPtr[Offset];
}
/**********************************************************************************************************************
 * IpBase_GetUint16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPBASE_FUNCCODE(uint16) IpBase_GetUint16(IPBASE_CONSTP2CONSTAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                         uint32_least Offset)
{
  return (uint16)(((uint16)(BufferPtr[Offset]) << 8u) | BufferPtr[Offset + 1u]);
}
/**********************************************************************************************************************
 * IpBase_GetUint16()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
IPBASE_FUNCCODE(uint32) IpBase_GetUint24(IPBASE_CONSTP2CONSTAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                         uint32_least Offset)
{
  return (uint32)(((uint32)(BufferPtr[Offset]) << 16u) | ((uint32)(BufferPtr[Offset + 1u]) << 8u) |
                  ((uint32)(BufferPtr[Offset + 2u])));
}
/**********************************************************************************************************************
 * IpBase_GetUint32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPBASE_FUNCCODE(uint32) IpBase_GetUint32(IPBASE_CONSTP2CONSTAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                         uint32_least Offset)
{
  return (uint32)(((uint32)(BufferPtr[Offset]) << 24u)     | ((uint32)(BufferPtr[Offset + 1u]) << 16u) | 
                  ((uint32)(BufferPtr[Offset + 2u]) << 8u) | BufferPtr[Offset + 3u]);
}
/**********************************************************************************************************************
 * IpBase_GetUint48()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
IPBASE_FUNCCODE(uint64) IpBase_GetUint48(IPBASE_CONSTP2CONSTAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                         uint32_least Offset)
{
  return (uint64)(((uint64)(BufferPtr[Offset]) << 40u)      | ((uint64)(BufferPtr[Offset + 1u]) << 32u) |
                  ((uint64)(BufferPtr[Offset + 2u]) << 24u) | ((uint64)(BufferPtr[Offset + 3u]) << 16u) |
                  ((uint64)(BufferPtr[Offset + 4u]) << 8u)  | ((uint64)(BufferPtr[Offset + 5u])));
}
/**********************************************************************************************************************
 * IpBase_GetUint64()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
IPBASE_FUNCCODE(uint64) IpBase_GetUint64(IPBASE_CONSTP2CONSTAPPLDATA(uint8) BufferPtr, /* PRQA S 3219 */ /* MD_MSR_Unreachable */
                                         uint32_least Offset)
{
  return (uint64)(((uint64)(BufferPtr[Offset]) << 56u)      | ((uint64)(BufferPtr[Offset + 1u]) << 48u) |
                  ((uint64)(BufferPtr[Offset + 2u]) << 40u) | ((uint64)(BufferPtr[Offset + 3u]) << 32u) |
                  ((uint64)(BufferPtr[Offset + 4u]) << 24u) | ((uint64)(BufferPtr[Offset + 5u]) << 16u) |
                  ((uint64)(BufferPtr[Offset + 6u]) << 8u)  | ((uint64)(BufferPtr[Offset + 7u])));
}
# endif /* IPBASE_ENABLE_INLINE_FUNCTIONS */

#define IPBASE_STOP_SEC_CODE
#include "IpBase_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 MD_IpBase_310_SockPtr:
      Reason:     Type cast for socket pointer.
      Risk:       Memory Access violation.
      Prevention: Covered by integration tests.
 MD_IpBase_4391:
      Reason:     Type cast to wider type.
      Risk:       Enlarged code size.
      Prevention: Covered by code review.
 MD_IpBase_3218:
      Reason:     Global static variable used in one function only.
      Risk:       Variable may be used within unintended functions of the module.
      Prevention: Covered by code review.
 MD_IpBase_1039:
      Reason:     Array with dummy length of 1 for socket address.
      Risk:       Variable may be used within the module.
      Prevention: Covered by code review.
 MD_IpBase_2981:
      Reason:     Redundant variable initialization.
      Risk:       Small runtime impact.
      Prevention: No mitigation.
 MD_IpBase_310_3305_PtrCast:
      Reason:     Type cast for target or source pointer.
      Risk:       Memory Access violation.
      Prevention: Memory access restrictions are respected. Covered by code review.
MD_IpBase_0306_PtrAddrCastToInt:
      Reason:     Type cast of a pointer address (signed integer) to an unsigned integer type.
      Risk:       Misleading code, difficult to understand and maintain.
      Prevention: Covered by code review, explained by comments.
*/
/* Justification for module-specific PCLINT deviations:
 LD_IpBase_451:
      Reason:     AUTOSAR MemMap
      Risk:       Uncritical
      Prevention: Covered by code review
*/

/* SBSW_JUSTIFICATION_BEGIN
  \ID SBSW_IPBASE_PASSING_API_ENCODE_ARRAY
    \DESCRIPTION      A pointer that is passed as function argument to IpBase_Encode is used to call 
                      IpBase_EncodeBase64Block
    \COUNTERMEASURE   \R  A runtime check ensures that the used index is smaller than the length provided to the API and
                          that the called API IpBase_EncodeBase64Block can access at least 4 bytes.
                      \S  The TgtDataPtr must reference at least *TgtLenBytePtr elements of type uint8.
                          This length has to be ensured by the caller. 
                          SMI-16

  \ID SBSW_IPBASE_PASSING_API_DECODE_ARRAY_WRITE
    \DESCRIPTION      A pointer that is passed as function argument to IpBase_Decode is used to call 
                      IpBase_DecodeBase64BlockWrite, other pointer references local variables with sufficient space.
    \COUNTERMEASURE   \R  A runtime check in IpBase_DecodeBase64BlockWrite ensures that no more than
                          *TgtLenBytePtr - tgtByteIdx are written to TgtDataPtr.
                      \S  The TgtDataPtr must reference at least *TgtLenBytePtr elements of type uint8.
                          This length has to be ensured by the caller. 
                          SMI-16

  \ID SBSW_IPBASE_PASSING_API_DECODE_ARRAY_READ
    \DESCRIPTION      Local array with IPBASE_CODE_BASE64_CODED_BLOCK_LEN(4) Bytes and local variable are passed to
                      IpBase_DecodeBase64BlockRead(), this function writes up to 4 Bytes to the array, the second 
                      pointer references a local variable.
    \COUNTERMEASURE   \T  TCASE-702208

  \ID SBSW_IPBASE_GETVERSION_PARAM
    \DESCRIPTION      The function IpBase_GetVersionInfo writes to the object referenced by parameter VersionInfoPtr
    \COUNTERMEASURE   \N  The caller ensures that the pointers passed to the parameters VersionInfoPtr is valid.

  \ID SBSW_IPBASE_VALID_API_POINTER
    \DESCRIPTION      Write access to a pointer which was passed as function argument.
    \COUNTERMEASURE   \N  The validity of the pointer has to be ensured by the caller.

  \ID SBSW_IPBASE_VALID_API_STRUCT_POINTER
    \DESCRIPTION      Write access to element which is passed as function argument.
    \COUNTERMEASURE   \N  It is assumed, that the input pointer is valid and the function only writes members of the
                          pointed struct.

  \ID SBSW_IPBASE_PASSING_API_POINTER 
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  Pointer was passed as function argument and is used to call sub-function.
                          The validity of the pointer has to be ensured by the caller.

  \ID SBSW_IPBASE_API_ARGUMENT_IDX
    \DESCRIPTION      Access of array element with index given as function argument is not checked for validity.
    \COUNTERMEASURE   \N  The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function.

  \ID SBSW_IPBASE_API_ARGUMENT_LEN
    \DESCRIPTION      Access of array element with index limited by function argument is not checked for validity.
    \COUNTERMEASURE   \N  The valid limit of the function argument is defined at the function declaration and has to be
                          ensured by the calling function.

  \ID SBSW_IPBASE_REFERENCE_OF_VARIABLE
    \DESCRIPTION      Pointer to variable is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing a variable by the address operator.

  \ID SBSW_IPBASE_REFERENCE_OF_POINTER_ELEMENT
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing a member of a struct passed as reference function
                          argument. The validity of the function argument has to be ensured by the caller.

  \ID SBSW_IPBASE_BER_REFERENCE_OF_POINTER_ELEMENT
    \DESCRIPTION      Pointer is passed as function argument and used to parse BER document.
    \COUNTERMEASURE   \N  Pointer is generated by referencing a member of a struct passed as reference function
                          argument to parse BER document. The validity of the BER document has to be ensured by the
                          caller.

  \ID SBSW_IPBASE_API_ZERO_TERMINATED_STRING
    \DESCRIPTION      Access of an array element with index which is not checked for validity.
    \COUNTERMEASURE   \N  The string has to be terminated by '\0'. The string termination has to be ensured by the
                          caller.

  \ID SBSW_IPBASE_API_STRING_TARGET_FOR_DATA
    \DESCRIPTION      Access of an array element with index which is not checked for validity.
    \COUNTERMEASURE   \N  The string has to be large enough to store parameter limited number of characters.
                          The string length has to be ensured by the caller.

  \ID SBSW_IPBASE_INT_DIGIT_MAX
    \DESCRIPTION      Access of an array element with index which is not checked for validity.
    \COUNTERMEASURE   \N  The definition of IPBASE_INT_DIGIT_MAX has to be large enough to store all digits of uint32
                          parameter IntVal.

  \ID SBSW_IPBASE_FUNCTION_CALL
    \DESCRIPTION      Function is called.
    \COUNTERMEASURE   \N  The implementation passes all arguments into a function.
                          It has to be ensured that the called function provides the same or higher safety level as the
                          caller.

  \ID SBSW_IPBASE_BASE64_ENCODED_BLOCK_LEN
    \DESCRIPTION      Access of target data pointer with index is not checked for validity.
    \COUNTERMEASURE   \N  The valid size of the function argument is defined as IPBASE_CODE_BASE64_CODED_BLOCK_LEN and
                          has to be ensured by the calling function.

  \ID SBSW_IPBASE_ROOT_PTR_VALIDITY
    \DESCRIPTION      Pointers including their dedicated length are passed to the callee. The pointers and the length
                      were passed as function arguments.
    \COUNTERMEASURE   \S  The pointers and their length were passed as function arguments. The actual length of the
                          arrays has to be at least the passed length. This has to be ensured by the caller.
                          SMI-1134760

  \ID SBSW_IPBASE_STRUCT_CREATED_TGTOFFSET_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct that is locally created or modified 
                      with a dedicated 'create' or 'consume' function. The content of the struct (target array, source
                      array, length) references a subset of the arrays that where passed to the public API
                      'IpBase_TcpIpChecksumCopyAdd'.
                      Additionally the new target offset is passed, which is used as array length value in the callee.
    \COUNTERMEASURE   \N  [CM_CREATED_OR_MODIFIED_STRUCT]

  \ID SBSW_IPBASE_STRUCT_CREATED_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct that is locally created or modified 
                      with a dedicated 'create' or 'consume' function. The content of the struct (target array, source
                      array, length) references a subset of the arrays that where passed to the public API
                      'IpBase_TcpIpChecksumCopyAdd'.
    \COUNTERMEASURE   \N  [CM_CREATED_OR_MODIFIED_STRUCT]
                          [CM_OFFSET_NOT_GREATER_THAN_3]

  \ID SBSW_IPBASE_NUMERIC_POINTER_ADDRESS
    \DESCRIPTION      A pointer is passed to IpBase_AlignmentOffset32Bit.
    \COUNTERMEASURE   \N  The function IpBase_AlignmentOffset32Bit does not access the pointer, it only uses the
                          pointer address to calculate its offset from the next 32-bit boundary.

  \ID SBSW_IPBASE_STRUCT_PASSED_SRCOFFSET_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct, the struct was passed as function
                      argument. Additionally the source offset is passed, which is used as array length value in the
                      callee.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          [CM_OFFSET_NOT_GREATER_THAN_3]

  \ID SBSW_IPBASE_STRUCT_PASSED_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct, the struct was passed as function
                      argument.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]

  \ID SBSW_IPBASE_STRUCT_PTR_IDX_ACCESS
    \DESCRIPTION      Access of an array element in a for loop with an index that is limited by the dedicated array
                      length. The pointer to the array and its length were passed as function arguments inside a
                      struct.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]

  \ID SBSW_IPBASE_STRUCT_PASSED_CHECKSUMIDX_LOCAL_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct, the struct was passed as function
                      argument. The checksum index pointer references a local variable.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          [CM_CHECKSUMIDX_LOCAL]

  \ID SBSW_IPBASE_PTR_PASSED_CASTED_VALIDITY
    \DESCRIPTION      Pointers and their dedicated length are passed to the callee. The pointers are generated by 
                      casting the pointers that were passed as function arguments inside a struct to uint8. The length
                      is the passed length of the struct.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          Casting the pointers to uint8 is always allowed.

  \ID SBSW_IPBASE_STRUCT_CREATED_CHECKSUMIDX_ARG_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct that is locally created or modified 
                      with a dedicated 'create' or 'consume' function. The content of the struct (target array, source
                      array, length) references a subset of the arrays that where passed to the public API
                      'IpBase_TcpIpChecksumCopyAdd'.
                      The checksum index pointer was passed as function argument.
    \COUNTERMEASURE   \N  [CM_CREATED_OR_MODIFIED_STRUCT]
                          The checksum index pointer was passed as function argument, its validity has to be ensured by
                          the caller.

  \ID SBSW_IPBASE_PTR_LEN_OFFSET_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee, the pointers were passed as function arguments inside a
                      struct. The struct also contains the dedicated pointer length. Additionally the source pointer
                      offset is passed as length.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          The function specification of the caller ensures that the source pointer offset always is
                          smaller than the dedicated pointer length. 

  \ID SBSW_IPBASE_STRUCT_CREATED_CHECKSUMIDX_LOCAL_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct that is locally created or modified 
                      with a dedicated 'create' or 'consume' function. The content of the struct (target array, source
                      array, length) references a subset of the arrays that where passed to the public API
                      'IpBase_TcpIpChecksumCopyAdd'.
                      The checksum index pointer references a local variable.
    \COUNTERMEASURE   \N  [CM_CREATED_OR_MODIFIED_STRUCT]
                          [CM_CHECKSUMIDX_LOCAL]

\ID SBSW_IPBASE_STRUCT_PASSED_IDX_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct, the struct was passed as function
                      argument. Additionally a for loop index is passed to the callee which is used to do a 32-bit
                      access on the pointers. The index is the loop counter in the for loop of the caller iterating
                      through the array.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          [CM_ARRAY_INDEX_ACCESS_16_BYTE_VALID]

  \ID SBSW_IPBASE_TGT_IDXINC_ACCESS
    \DESCRIPTION      32-bit access of an array element with an index given as function argument. The array is accessed
                      with 'index + [0-3]'.
    \COUNTERMEASURE   \N  It has to be ensured by the caller that at least 4 bytes can be written from the given index.

  \ID SBSW_IPBASE_PTR_PARTIAL_FIRST_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee. The target pointer and its dedicated length was passed as
                      function argument inside a struct. The passed length parameter is the target pointer offset. The
                      content of the struct (target array, source array, length) references a subset of the arrays that
                      where passed to the public API 'IpBase_TcpIpChecksumCopyAdd'.
                      The partial source word and the checksum index pointer reference local variables.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          [CM_PARTIAL_SRC_WORD_LOCAL]
                          [CM_CHECKSUMIDX_LOCAL]
                          The function specification of the caller ensures that the passed length is always smaller
                          than the dedicated pointer length. This ensures the validity of the target pointer for the
                          passed length.

  \ID SBSW_IPBASE_PTR_PARTIAL_SECOND_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee. The target pointer and its dedicated length is locally
                      generated via the function 'IpBase_ConsumeCopiedDataBytewise' as part of a struct. The length
                      parameter is the length of this second part. The partial source word and the checksum index
                      pointer reference local variables. The content of the struct (target array, source array, length)
                      references a subset of the arrays that where passed to the public API 
                      'IpBase_TcpIpChecksumCopyAdd'.
    \COUNTERMEASURE   \N  [CM_CREATED_OR_MODIFIED_STRUCT]
                          [CM_PARTIAL_SRC_WORD_LOCAL]
                          [CM_CHECKSUMIDX_LOCAL]
                          The function specification of the caller ensures that the passed length is always smaller
                          than the dedicated pointer length. This ensures the validity of the target pointer for the
                          passed length.

  \ID SBSW_IPBASE_STRUCT_CREATED_SRCWORD_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct that is locally created or modified 
                      with a dedicated 'create' or 'consume' function. The content of the struct (target array, source
                      array, length) references a subset of the arrays that where passed to the public API
                      'IpBase_TcpIpChecksumCopyAdd'.
                      The partial source word pointer references a local variable.
    \COUNTERMEASURE   \N  [CM_CREATED_OR_MODIFIED_STRUCT]
                          [CM_PARTIAL_SRC_WORD_LOCAL]

  \ID SBSW_IPBASE_STRUCT_PASSED_IDX_SRCWORD_VALIDITY
    \DESCRIPTION      Pointers are passed to the callee as elements of a struct, the struct was passed as function
                      argument. Additionally a for loop index is passed to the callee which is used to do a 32-bit
                      access on the pointers. The left over bytes pointer was passed as function argument, the modified
                      source word pointer references a local variable.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          [CM_ARRAY_INDEX_ACCESS_16_BYTE_VALID]
                          The left over bytes pointer was passed as function argument, its validity has to be ensured
                          by the caller. The modified source word pointer is generated by referencing a local variable
                          by the address operator.

SBSW_JUSTIFICATION_END */

/* VCA_JUSTIFICATION_BEGIN
  \ID VCA_IPBASE_BER_ELEMENT_IN_BOUNDS
    \DESCRIPTION      BER element access can not be checked for consistency and has thus be tested thoroughly.
    \COUNTERMEASURE   \T  TCASE-705363

  \ID VCA_IPBASE_API_STRING_TARGET_FOR_DATA
    \DESCRIPTION      Access of an array element with index which is not checked for validity.
    \COUNTERMEASURE   \S  The string has to be large enough to store parameter limited number of characters.
                          The string length has to be ensured by the caller.
                          SMI-1026681

  \ID VCA_IPBASE_API_STRING_TARGET_NULL
    \DESCRIPTION      Access of a string parameter. Content of string parameter is tested against NULL during runtime.
    \COUNTERMEASURE   \T  TCASE-11062

  \ID VCA_IPBASE_INT_DIGIT_MAX
    \DESCRIPTION      Access of an array element with index which is not checked for validity.
    \COUNTERMEASURE   \T  TCASE-702439

  \ID VCA_IPBASE_API_PBUF_CONSISTENCY
    \DESCRIPTION      Access of an array element in PBuf struct with index which is not checked for validity. A runtime 
                      test checks the length limitation within the function.
    \COUNTERMEASURE   \S  The total length of PBuf structure has to be large enough to store the specified total
                          length. The length has to be ensured by the caller.
                          SMI-1031276
                      \T  TCASE-702071

  \ID VCA_IPBASE_API_SOCK_CONSISTENCY
    \DESCRIPTION      Access of an array element in Socket struct with index which is not checked for validity. A 
                      test checks the length limitation within the function for valid and invalid address families.
    \COUNTERMEASURE   \S  The total length of SockPtr structure has to be large enough to store either IPBASE_AF_INET 
                          or IPBASE_AF_INET6 bytes. The length has to be ensured by the caller.
                          SMI-1031277
                      \T  TCASE-702073, TCASE-11230

  \ID VCA_IPBASE_API_SOCK6_TYPE_CONSISTENCY
    \DESCRIPTION      The parameter have to be of the specified type IpBase_AddrIn6Type.
    \COUNTERMEASURE   \R  The size of the IpBase_AddrIn6Type is used, type consistency is checked by the compiler.

  \ID VCA_IPBASE_API_PTR_CONSISTENCY
    \DESCRIPTION      Pointers including their dedicated length are passed to the callee. The pointers and the length
                      were passed as function arguments.
    \COUNTERMEASURE   \S  The pointers and their length were passed as function arguments. The actual length of the
                          arrays has to be at least the passed length. This has to be ensured by the caller.
                          SMI-1134760

  \ID VCA_IPBASE_PTR_CONSISTENCY
    \DESCRIPTION      Pointers and their dedicated length are passed to the callee. The pointers are generated by 
                      casting the pointers that were passed as function arguments inside a struct to uint8. The length
                      is the passed length of the struct.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          Casting the pointers to uint8 is always allowed.

  \ID VCA_IPBASE_ARRAY_AND_OFFSET_LEN_ALIGNED_COPY_CONSISTENCY
    \DESCRIPTION      Function is called with a length parameter inside a struct. The length parameter must be greater
                      or equal the minimum length for the 16-byte aligned block copy.
                      The function also is called with a source offset parameter which must be smaller than the maximum
                      allowed offset.
    \COUNTERMEASURE   \N  The caller requirements ensure that the minimum array length that was passed inside a struct
                          (IPBASE_SOCK_BYTE_LEN_THRESHOLD_BIG_LEN_COPY) always is greater than the minimum required
                          length for the 16-byte unaligned block copy (IPBASE_SOCK_BYTE_LEN_BLOCK_BIG_LEN_BLOCK_COPY).
                          [CM_OFFSET_NOT_GREATER_THAN_3]
    
  \ID VCA_IPBASE_ARRAY_AND_OFFSET_LEN_UNALIGNED_COPY_CONSISTENCY
    \DESCRIPTION      Function is called with a length parameter inside a struct. The length parameter must be greater
                      or equal the minimum length for the 16-byte aligned block copy.
                      The function also is called with a target and source offset parameter which must be smaller than
                      the maximum allowed offset.
    \COUNTERMEASURE   \N  The caller requirements ensure that the minimum array length that was passed inside a struct
                          (IPBASE_SOCK_BYTE_LEN_THRESHOLD_BIG_LEN_COPY) always is greater than the minimum required
                          length for the 16-byte unaligned block copy
                          (IPBASE_SOCK_MIN_BYTE_LEN_BIG_LEN_UNALIGNED_BLOCK_COPY).
                          [CM_OFFSET_NOT_GREATER_THAN_3]

  \ID VCA_IPBASE_MOD_STRUCT_OFFSET_CONSISTENCY
    \DESCRIPTION      Pointers are passed to the callee, the pointers were passed as function arguments inside a
                      struct. The struct also contains the dedicated pointer length. Additionally the source pointer
                      offset is passed as length.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          The function specification of the caller ensures that the source pointer offset always is
                          smaller than the dedicated pointer length.

  \ID VCA_IPBASE_PTR_PARTIAL_FIRST_CONSISTENCY
    \DESCRIPTION      Pointers are passed to the callee. The target pointer and its dedicated length was passed as
                      function argument inside a struct. The passed length parameter is the target pointer offset. The
                      content of the struct (target array, source array, length) references a subset of the arrays that
                      where passed to the public API 'IpBase_TcpIpChecksumCopyAdd'.
    \COUNTERMEASURE   \N  [CM_STRUCT_AS_ARGUMENT]
                          The function specification of the caller ensures that the passed length is always smaller
                          than the dedicated pointer length. This ensures the validity of the target pointer for the
                          passed length.

  \ID VCA_IPBASE_PTR_PARTIAL_SECOND_CONSISTENCY
    \DESCRIPTION      Pointers are passed to the callee. The target pointer and its dedicated length is locally
                      generated via the function 'IpBase_ConsumeCopiedDataBytewise' as part of a struct. The length
                      parameter is the length of this second part. The content of the struct (target array, source
                      array, length) references a subset of the arrays that where passed to the public API
                      'IpBase_TcpIpChecksumCopyAdd'.
    \COUNTERMEASURE   \N  [CM_CREATED_OR_MODIFIED_STRUCT]
                          The function specification of the caller ensures that the passed length is always smaller
                          than the dedicated pointer length. This ensures the validity of the target pointer for the
                          passed length.

  \ID VCA_IPBASE_STRUCT_IDX_VALIDITY
    \DESCRIPTION      A for loop index is passed to the callee which is used to do a 32-bit access on the pointers.
    \COUNTERMEASURE   \N  [CM_ARRAY_INDEX_ACCESS_16_BYTE_VALID]

VCA_JUSTIFICATION_END */

/* Reuse of counter measure statements for SBSW and VCA justifications: */

/*
\CM CM_CREATED_OR_MODIFIED_STRUCT
                          When the struct is created the validity of its content is ensured by the used 'create'
                          function 
                          - 'IpBase_CreateBytewiseCopyStruct' or
                          - 'IpBase_CreateAlignedCopyStruct'.
                          The requirements of 'IpBase_CreateBytewiseCopyStruct' are ensured by review. 
                          'IpBase_CreateAlignedCopyStruct' casts the pointers of the input struct to uint32 to return a
                          new struct for a 32-bit access. The input struct was created with
                          'IpBase_CreateBytewiseCopyStruct', therefore also the returned struct content is valid.
                          When the struct is modified the validity of its content is ensured by the used 'consume' 
                          function
                          - 'IpBase_ConsumeCopiedDataBytewise' or
                          - 'IpBase_ConsumeReadWrittenDataBytewise'.
                          Runtime checks in the consume functions ensure that the modified content always references a 
                          subset of the arrays of the input struct. The struct is never modified directly, what is
                          ensured by review.
                          The content of the struct (target array, source array, length) is a subset of the content
                          that was passed to the public API, which must fulfill SMI-1134760.

\CM CM_STRUCT_AS_ARGUMENT
                          The struct was passed as function argument. The validity of its content has to be ensured
                          where the struct was created or modified. Accessing the array is valid for the dedicated
                          length.

\CM CM_CHECKSUMIDX_LOCAL
                          The checksum index pointer is generated by referencing a local variable by the address
                          operator.

\CM CM_ARRAY_INDEX_ACCESS_16_BYTE_VALID
                          The caller for loop iterates till the array limit in 16-byte steps (4 * 4-bytes). The index
                          + 16 bytes are always smaller than the array limit, so it's ensured that always 16 bytes can
                          be written starting from the passed array index.

\CM CM_PARTIAL_SRC_WORD_LOCAL
                          The partial source word pointer is generated by referencing a local variable by the address
                          operator.

\CM CM_OFFSET_NOT_GREATER_THAN_3
                          The target and source offset only is calculated via function 'IpBase_AlignmentOffset32Bit'.
                          The maximum value this function can return for the offset is 3 (because a modulo 4 is
                          executed). This is ensured by review. Because the big length copy only is executed for a
                          minimum array length of IPBASE_SOCK_BYTE_LEN_THRESHOLD_BIG_LEN_COPY (42 bytes) the offsets
                          always are smaller than this threshold and can be used as array lengths for offset correction
                          where needed.
*/

/* FETA_JUSTIFICATION_BEGIN
  \ID FETA_IPBASE_BER_PARSER
    \DESCRIPTION      The parser scans through an entire BER encoded document which is a complex loop. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-705363

  \ID FETA_IPBASE_CODE
    \DESCRIPTION      Encoding in Base64 and Decoding will always end. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-702207, TCASE-11246

  \ID FETA_IPBASE_PBUF_BUF
    \DESCRIPTION      PBuf handling will always end. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-702068, TCASE-702070, TCASE-702071, TCASE-702072, TCASE-702633, TCASE-11229, TCASE-702685

  \ID FETA_IPBASE_PBUF_STR
    \DESCRIPTION      PBuf string handling will always end. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-702633, TCASE-11229, TCASE-702685

  \ID FETA_IPBASE_SOCK_LEN_PARAM
    \DESCRIPTION      Checksum calculation will always end. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-702075, TCASE-705360, TCASE-705361

  \ID FETA_IPBASE_SOCK_CHKSUM_SHIFT
    \DESCRIPTION      32 bit Checksum is bit shifted by 16 bits. Loop will end with 2 iterations. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-705360

  \ID FETA_IPBASE_SOCK_ADDRIN_TYPE
    \DESCRIPTION      Address type limits loop iterations to max size of IPv6 address, i.e. 16 iterations. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-702074

  \ID FETA_IPBASE_STR_ZEROTERM
    \DESCRIPTION      Abortion criteria relies on '\0' terminated strings. There are alternative functions available in IpBase with length limitation checks.
    \COUNTERMEASURE   \S  SMI-1027479

  \ID FETA_IPBASE_STR_LEN_PARAM
    \DESCRIPTION      Loop termination criteria relies on input parameter. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-706365, TCASE-706366, TCASE-706368, TCASE-702436, TCASE-702439, TCASE-707105, 
                          TCASE-702440, TCASE-707185, TCASE-707186, TCASE-707187, TCASE-707190, TCASE-702438, TCASE-707104

  \ID FETA_IPBASE_STR_DIGIT_TYPE32
    \DESCRIPTION      Max number of digits limits loop iterations to max size of digits in uint32, i.e. 10 iterations. Abortion criteria is tested.
    \COUNTERMEASURE   \T  TCASE-707104, TCASE-707106, TCASE-707109
FETA_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION
  \ID COV_IPBASE_BYTE_ORDER
    \ACCEPT   TX
    \ACCEPT   XF
    \REASON   Tests are executed on Windows thus the byte order can not be changed
  \ID COV_IPBASE_NOUNIT_INLINE
    \ACCEPT   TX
    \REASON   Switch is only used for unit testing to switch off inline implementation
END_COVERAGE_JUSTIFICATION */
/**********************************************************************************************************************
 *  END OF FILE: IpBase.c
 *********************************************************************************************************************/
