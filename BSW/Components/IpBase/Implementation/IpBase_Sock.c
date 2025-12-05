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
/*!        \file  IpBase_Sock.c
 *         \unit  Sock
 *        \brief  IpBase socket handling source file
 *      \details  IpBase socket handling routines definition.
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
#define IPBASE_SOCK_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "IpBase.h"                                                                                                     /* PRQA S 3453 3 */ /* MD_MSR_FctLikeMacro */
#include "IpBase_Priv.h"

/***********************************************************************************************************************
 *  CONSISTENCY CHECK
 **********************************************************************************************************************/
/* Check consistency of source and header file. */
#if ( (IPBASE_SW_MAJOR_VERSION != (4u)) \
   || (IPBASE_SW_MINOR_VERSION != (2u)) \
   || (IPBASE_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of IpBase_Sock.c and IpBase.h are inconsistent"
#endif

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/
 /* Byte lengths in bits. */
#define IPBASE_SOCK_BIT_LEN_1_BYTE  (8u * 1u) /*  8 bits. */
#define IPBASE_SOCK_BIT_LEN_2_BYTES (8u * 2u) /* 16 bits. */
#define IPBASE_SOCK_BIT_LEN_3_BYTES (8u * 3u) /* 24 bits. */
#define IPBASE_SOCK_BIT_LEN_4_BYTES (8u * 4u) /* 32 bits. */
/* Only used in VCA spec: Have the same value as and 'IPBASE_SOCK_BIT_LEN_1_BYTE' 'IPBASE_SOCK_BIT_LEN_3_BYTES' above
   but are used in a different context. */
#define IPBASE_SOCK_MIN_BIT_SHIFT_1_BYTE_IN_32BIT_WORD (8u * 1u) /* 24 bits. */
#define IPBASE_SOCK_MAX_BIT_SHIFT_1_BYTE_IN_32BIT_WORD (8u * 3u) /* 24 bits. */

/* Lengths in bytes. */
#define IPBASE_SOCK_BYTE_LEN_32_BIT_WORD                      (4u) /* 4-byte word. */
/* Length in bytes that is copied in one iteration of the big length block copy (aligned and unaligned). */
#define IPBASE_SOCK_BYTE_LEN_BLOCK_BIG_LEN_BLOCK_COPY         (16u)
/* Only used in VCA spec: Minimum length that is required by the algorithm of the big length unaligned copy. '16u' is
   the length of one block. '4u' is the 32-bit word length resulting from the read-ahead of the unaligned before the
   block copy. If the length is shorter an illegal array-pointer access will happen. */
#define IPBASE_SOCK_MIN_BYTE_LEN_BIG_LEN_UNALIGNED_BLOCK_COPY (16u + 4u)
/* Threshold in bytes from which the big length copy algorithms (16-byte block copy) are used (because it makes sense
   speed-wise). '32u' is the length of two blocks, so the big length copy is only used when the block copy loop is
   iterated at least twice. '6u' is the maximum offset sum of the target and source array. '4u' is the 32-bit word
   length resulting from the read-ahead of the unaligned copy. */
#define IPBASE_SOCK_BYTE_LEN_THRESHOLD_BIG_LEN_COPY           (32u + 6u + 4u)

/* Defines for modulo offset calculations. */
#define IPBASE_SOCK_MODULO_2_OFFSET_EVEN_ODD 2u
#define IPBASE_SOCK_MODULO_4_BYTE_OFFSET 4u

/* Pointer offsets in bytes from the next 32-bit boundary. */
#define IPBASE_SOCK_POINTER_NO_BYTE_OFFSET        (0u) /* No pointer offset. */
#define IPBASE_SOCK_POINTER_MIN_BYTE_OFFSET       (1u) /* Minimum offset for an offset correction. */
#define IPBASE_SOCK_POINTER_MAX_BYTE_OFFSET       (3u) /* Maximum pointer offset. */

/* Byte indexes in 32-bit words.  */
#define IPBASE_SOCK_MIN_BYTE_IDX_IN_32_BIT_WORD   (0u) /* Minimum byte index of a 32-bit word. */
#define IPBASE_SOCK_MAX_BYTE_IDX_IN_32_BIT_WORD   (3u) /* Maximum byte index of a 32-bit word. */
/* Only used in VCA spec: Maximum word index in a 16-byte block. */
#define IPBASE_SOCK_MAX_WORD_IDX_IN_16_BYTE_BLOCK (3u)

/* Value that is 1 greater than the maximum value of a byte, where the maximum value of a byte is 0xFF. */
#define IPBASE_SOCK_VALUE_BY_1_GREATER_THAN_MAX_BYTE_VALUE (0x100u)

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* Array data as passed via the public API and as used in internal functions for bytewise copy/checksum add.
   The struct shall only be created and modified with the functions 'IpBase_CreateBytewiseCopyStruct',
   'IpBase_CreateAlignedCopyStruct', 'IpBase_ConsumeCopiedDataBytewise' and 'IpBase_ConsumeReadWrittenDataBytewise'. */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
typedef struct
{
  IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr;    /* Pointer to the target array. */
  IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr;  /* Pointer to the source array. */
  uint32 LenByte;                            /* Array length in bytes. */
} IpBase_SockBytewiseArrayCopyStructType;

/* Array data with 32-bit aligned pointers. Is used for an aligned array-pointer accesses. The struct shall only be
   created and modified with the function 'IpBase_CreateAlignedCopyStruct'. */
typedef struct
{
  IPBASE_P2VARAPPLDATA(uint32) TgtDataPtr;   /* Pointer to the target array. */
  IPBASE_P2CONSTAPPLDATA(uint32) SrcDataPtr; /* Pointer to the source array. */
  uint32 LenByte;                            /* Array length in bytes. Must not be used to access the arrays directly.
                                                The length in words must be calculated to access the arrays. */
} IpBase_SockAlignedArrayCopyStructType;

/* Bit shift counts used for unaligned copy. */
typedef struct
{
  uint8 TowardWordStartIdx;                /* Used to shift toward the byte start index of the 'high byte first'
                                              representation of a word (index 0). */
  uint8 TowardWordEndIdx;                  /* Used to shift toward the byte end index of the 'high byte first'
                                              representation of a word (index 3 in the 32-bit word). */
} IpBase_SockUnalignedArrayCopyBitShiftStructType;

/* Modified source words used in and returned by the unaligned copy function. */
typedef struct
{
  uint32 SrcWord0;
  uint32 SrcWord1;
  uint32 SrcWord2;
  uint32 SrcWord3;
} IpBase_SockUnalignedCopyModSrcWordsStructType;
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define IPBASE_START_SEC_CODE
#include "IpBase_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  IpBase_SockIpAddrIsEqualHlp
 **********************************************************************************************************************/
/*! \brief         Check if IP address and family is equal helper
 *  \details       Check if IP address and family of sockets is equal actual comparison
 *  \param[in]     SockAPtr             first IP socket address
 *  \param[in]     SockBPtr             second IP socket address
 *  \return        TRUE                 IP address is equal
 *  \return        FALSE                IP address is not equal
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(boolean) IpBase_SockIpAddrIsEqualHlp(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                                  IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr);

/***********************************************************************************************************************
 *  IpBase_SockIpV6AddrIsEqualHlp
 **********************************************************************************************************************/
/*! \brief         Check if IPv6 address is equal helper
 *  \details       Check if IPv6 address is equal actual comparison
 *  \param[in]     SockAPtr             first IPv6 socket address
 *  \param[in]     SockBPtr             second IPv6 socket address
 *  \return        TRUE                 IPv6 address is equal
 *  \return        FALSE                IPv6 address is not equal
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(boolean) IpBase_SockIpV6AddrIsEqualHlp(
                                        IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrIn6Type) SockAPtr,
                                        IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrIn6Type) SockBPtr);

/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumAddHlp
 **********************************************************************************************************************/
/*! \brief         Adds to TCP/IP checksum helper
 *  \details       Adds a range to TcpIp checksum calculation
 *  \param[in]     DataPtr           pointer to the data
 *  \param[in]     LenByte           data length in bytes
 *  \param[in]     Checksum          current checksum. 0 to start a new checksum calculation
 *  \return        calculated checksum in network byte order
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \trace         CREQ-120246
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumAddHlp(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr,
                                                                uint32 LenByte,
                                                                uint32 Checksum);

/***********************************************************************************************************************
 *  IpBase_CreateBytewiseCopyStruct
 **********************************************************************************************************************/
/*! \brief         Creates a struct for bytewise copy
 *  \details       -
 *  \param[in]     TgtDataPtr        Pointer to target data.
 *                                   \spec requires
 *                                     $lengthOf(TgtDataPtr) >= LenByte;
 *                                   \endspec
 *  \param[in]     SrcDataPtr        Pointer to source data.
 *                                   \spec requires
 *                                     $lengthOf(SrcDataPtr) >= LenByte;
 *                                   \endspec
 *  \param[in]     LenByte           Original data length in bytes.
 *  \return        Struct for bytewise copy.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(IpBase_SockBytewiseArrayCopyStructType) IpBase_CreateBytewiseCopyStruct(
                                                                              IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                                              IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                                              IPBASE_CONSTCONST(uint32) LenByte);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_CreateAlignedCopyStruct
 **********************************************************************************************************************/
/*! \brief         Creates a struct for aligned copy
 *  \details       Creates a struct for aligned copy out of a bytewise copy struct. Is only allowed to be called with
 *                 32-bit aligned target and source pointers.
 *  \param[in]     BytewiseArrayDataPtr
 *                                   Pointer to struct of array data for the copy. The struct contains pointers to the
 *                                   target and source array and the array length.
 *  \return        Struct for aligned copy.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(IpBase_SockAlignedArrayCopyStructType) IpBase_CreateAlignedCopyStruct(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayDataPtr);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ConsumeCopiedDataBytewise
 **********************************************************************************************************************/
/*! \brief         Consumes copied data bytewise
 *  \details       Consumes the already copied data. Sets the target and source pointer to the array element where the
 *                 preceding copy stopped and reduces the length.
 *  \param[in,out] BytewiseArrayDataPtr
 *                                   Pointer to struct of array data for the copy. The struct contains pointers to the
 *                                   target and source array and the array length.
 *  \param[in]     CopiedBytes       Number of copied bytes.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(void) IpBase_ConsumeCopiedDataBytewise(
                      IPBASE_CONSTP2VARAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayDataPtr,
                      IPBASE_CONSTCONST(uint32) CopiedBytes);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ConsumeReadWrittenDataBytewise
 **********************************************************************************************************************/
/*! \brief         Consumes the read and written data bytewise
 *  \details       Consumes the already read and written data. Sets the target and source pointer to the array element
 *                 where the preceding copy stopped and reduces the length. Is only used when the write byte count is
 *                 smaller than the read byte count after the 1st partial word copy.
 *  \param[in,out] BytewiseArrayDataPtr
 *                                   Pointer to struct of array data for the copy. The struct contains pointers to the
 *                                   target and source array and the array length. The array length is the length of
 *                                   the shorter source array.
 *  \param[in]     WrittenBytes      Number of bytes written to the target.
 *  \param[in]     ReadBytes         Number of bytes read from the source.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(void) IpBase_ConsumeReadWrittenDataBytewise(
                      IPBASE_CONSTP2VARAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayDataPtr,
                      IPBASE_CONSTCONST(uint32) WrittenBytes,
                      IPBASE_CONSTCONST(uint32) ReadBytes);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumCopyAddHlp
 **********************************************************************************************************************/
/*! \brief         Copies data and calculates its additive checksum
 *  \details       Implements 'IpBase_TcpIpChecksumCopyAdd'.
 *  \param[in,out] RootArrayDataPtr  Pointer to struct of the root array data for the copy. The struct contains
 *                                   pointers to the target and source array and the array length.
 *  \param[in]     Checksum          Current 16-bit checksum in host byte order. 0 to start a new checksum calculation.
 *  \return        Calculated 16-bit checksum in host byte order (Stop==FALSE) or 16-bit checksum in network byte order
 *                 (Stop==TRUE).
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) RootArrayDataPtr,
                      IPBASE_CONSTCONST(uint32) Checksum,
                      IPBASE_CONSTCONST(boolean) Stop);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_AlignmentOffset32Bit
***********************************************************************************************************************/
/*! \brief         Calculates the pointer offset from the next 32-bit boundary
 *  \details       -
 *  \param[in]     Pointer           Pointer.
 *  \return        Offset of the passed pointer from the next 32-bit boundary.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint8) IpBase_AlignmentOffset32Bit(IPBASE_CONSTP2CONSTAPPLDATA(uint8) Pointer);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_SmallLenChecksumCopyAddHlp
 **********************************************************************************************************************/
/*! \brief         Copies data of small length and calculates its additive checksum
 *  \details       Depending on the target and source alignment a 4-byte block copy or a bytewise copy is used.
 *  \param[in,out] RootArrayDataPtr  Pointer to struct of the root array data for the copy. The struct contains
 *                                   pointers to the target and source array and the array length.
 *  \param[in]     TgtOffset         Target offset from the next 32-bit boundary.
 *  \param[in]     SrcOffset         Source offset from the next 32-bit boundary.
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_SmallLenChecksumCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) RootArrayDataPtr,
                      IPBASE_CONSTCONST(uint8) TgtOffset,
                      IPBASE_CONSTCONST(uint8) SrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumCopyAddBlockwiseHlp
 **********************************************************************************************************************/
/*! \brief         Copies 32-bit aligned source data to a 32-bit aligned target and calculates the additive checksum
 *  \details       Copies data in 4-byte blocks and calculates the checksum in 4-byte blocks by adding.
 *  \param[in,out] AlignedArrayCopyDataPtr
 *                                   Pointer to struct of aligned array data for an aligned copy. The struct contains
 *                                   pointers to the target and source array and the remaining array length.
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumCopyAddBlockwiseHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumCopyAddRemainingBytesHlp
 **********************************************************************************************************************/
/*! \brief         Copies the remaining bytes after a preceding block copy
 *  \details       Creates a struct for bytewise copy and calls the bytewise copy and checksum add function.
 *  \param[in,out] AlignedArrayCopyDataPtr
 *                                   Pointer to struct of aligned array data. The struct contains pointers to the
 *                                   target and source array and the remaining array length.
 *  \param[in]     CopiedBytes       Number of bytes already copied in the preceding copy.
 *  \param[in]     ChecksumIdxPtr    Pointer to the index of the next byte in the uint32 checksum.
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumCopyAddRemainingBytesHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint32) CopiedBytes,
                      IPBASE_CONSTP2VARAPPLDATA(uint8) ChecksumIdxPtr,
                      IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/**********************************************************************************************************************
 *  IpBase_ChecksumCopyAddBytewiseHlp
 **********************************************************************************************************************/
/*! \brief         Copies data and calculates its additive checksum bytewise
 *  \details       Copies data bytewise and calculates the checksum by adding bytewise to the current index of the
 *                 checksum. For each copied byte the checksum index is adapted and finally returned. Bytewise copy
 *                 can be used for any alignment.
 *  \param[in,out] BytewiseArrayDataPtr
 *                                   Pointer to struct of array data for the copy. The struct contains pointers to the
 *                                   target and source array and the array length.
 *  \param[in,out] ChecksumIdxPtr    Pointer to the index of the next byte in the uint32 checksum.
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumCopyAddBytewiseHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayDataPtr,
                      IPBASE_CONSTP2VARAPPLDATA(uint8) ChecksumIdxPtr,
                      IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumAddByteHlp
***********************************************************************************************************************/
/*! \brief         Adds the passed byte to the checksum
 *  \details       Adds the passed byte to the correct index in the checksum word (in dependence of the underlying
 *                 platform endianness).
 *  \param[in]     Data              Data to add (one byte).
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \param[in,out] ChecksumIdxPtr    8-bit pointer to the position of the next byte to copy to in the uint32 checksum.
 *                                   \spec requires
 *                                     ChecksumIdxPtr != NULL_PTR;
 *                                   \endspec
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumAddByteHlp(
                                              IPBASE_CONSTCONST(uint8) Data,
                                              IPBASE_CONSTCONST(uint32) Checksum,
                                              IPBASE_CONSTP2VARAPPLDATA(uint8) ChecksumIdxPtr);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_BigLenChecksumCopyAddHlp
 **********************************************************************************************************************/
/*! \brief         Copies data of big length and calculates its additive checksum
 *  \details       Depending on the target and source alignment a faster 'aligned' or slower 'unaligned' 16-byte block
 *                 copy algorithm is used.
 *  \param[in,out] RootArrayDataPtr  Pointer to struct of the root array data for the copy. The struct contains
 *                                   pointers to the target and source array and the array length.
 *                                   \spec requires
 *                                     RootArrayDataPtr->LenByte >= IPBASE_SOCK_BYTE_LEN_THRESHOLD_BIG_LEN_COPY;
 +                                   \endspec
 *  \param[in]     RootSrcOffset     Source offset from the next 32-bit boundary.
 *                                   \spec requires
 *                                     RootSrcOffset <= IPBASE_SOCK_POINTER_MAX_BYTE_OFFSET;
 *                                   \endspec
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_BigLenChecksumCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) RootArrayDataPtr,
                      IPBASE_CONSTCONST(uint8) RootSrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumAlignedCopyAddHlp
 **********************************************************************************************************************/
/*! \brief         Copies 32-bit aligned source data to a 32-bit aligned target and calculates the checksum
 *  \details       Copies the main data in 16-byte blocks and the remaining bytes bytewise.
 *  \param[in,out] AlignedArrayCopyDataPtr
 *                                   Pointer to struct of aligned array data for an aligned copy. The struct contains
 *                                   pointers to the target and source array and the remaining array length.
 *                                   \spec requires
 *                                     AlignedArrayCopyDataPtr->LenByte >= IPBASE_SOCK_BYTE_LEN_BLOCK_BIG_LEN_BLOCK_COPY;
 *                                   \endspec
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumAlignedCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint8) RootSrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/**********************************************************************************************************************
 * IpBase_AlignedOneBlockCopyHlp
 **********************************************************************************************************************/
/*! \brief         Copies one 16-byte block of data from an aligned source to an aligned target.
 *  \details       Is executed inside a loop.
 *  \param[in,out] AlignedArrayCopyDataPtr
 *                                   Pointer to struct of aligned array data for an aligned copy. The struct contains
 *                                   pointers to the target and source array and the remaining array length.
 *                                   \spec requires
 *                                     AlignedArrayCopyDataPtr->LenByte >= IPBASE_SOCK_BYTE_LEN_BLOCK_BIG_LEN_BLOCK_COPY;
 *                                   \endspec
 *  \param[in]     WordIdx           Index of the next 32-bit word in the target array.
 *                                   \spec requires
 *                                     (WordIdx + IPBASE_SOCK_MAX_WORD_IDX_IN_16_BYTE_BLOCK) <
 *                                     $lengthOf(AlignedArrayCopyDataPtr->TgtDataPtr);
 *                                   \endspec
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(void) IpBase_AlignedOneBlockCopyHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint32_least) WordIdx);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/**********************************************************************************************************************
 *  IpBase_ChecksumUnalignedCopyAddHlp
 **********************************************************************************************************************/
/*! \brief         Copies 32-bit aligned source data to an unaligned target and calculates the checksum
 *  \details       Uses different copy and checksum algorithms for head, main and tail data depending on the target and
 *                 source offset. Copies the head and tail bytewise and the main data in 16-byte blocks (4 x 4 bytes).
 *  \param[in,out] BytewiseArrayCopyDataPtr
 *                                   Pointer to struct of bytewise array data for an unaligned copy. The struct
 *                                   contains pointers to the target and source array and the remaining array length.
 *                                   \spec requires
 *                                     BytewiseArrayCopyDataPtr->LenByte >=
 *                                     IPBASE_SOCK_MIN_BYTE_LEN_BIG_LEN_UNALIGNED_BLOCK_COPY;
 *                                   \endspec
 *  \param[in]     TgtOffset         Target offset from the next 32-bit boundary.
 *                                   \spec requires
 *                                     TgtOffset <= IPBASE_SOCK_POINTER_MAX_BYTE_OFFSET;
 *                                   \endspec
 *  \param[in]     SrcOffset         Source offset from the next 32-bit boundary.
 *                                   \spec requires
 *                                     SrcOffset <= IPBASE_SOCK_POINTER_MAX_BYTE_OFFSET;
 *                                   \endspec
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumUnalignedCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint8) TgtOffset,
                      IPBASE_CONSTCONST(uint8) SrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/**********************************************************************************************************************
 * IpBase_ChecksumUnalignedCopyAddBlockwiseHlp
 **********************************************************************************************************************/
/*! \brief         Copies 32-bit aligned source data to a previously unaligned target and calculates the checksum
 *  \details       -
 *  \param[in,out] AlignedArrayCopyDataPtr
 *                                   Pointer to struct of aligned array data for an unaligned copy. The struct contains
 *                                   pointers to the target and source array and the remaining array length. The offset
 *                                   of the target pointer was corrected in a previous step.
 *                                   \spec requires
 *                                     AlignedArrayCopyDataPtr->LenByte >= IPBASE_SOCK_BYTE_LEN_BLOCK_BIG_LEN_BLOCK_COPY;
 *                                   \endspec
 *  \param[in]     LeftoverBytesPtr  Pointer to the bytes of the 32-bit word that are not copied yet in the preceding
 *                                   partial word copy.
 *                                   \spec requires
 *                                     LeftoverBytesPtr != NULL_PTR;
 *                                   \endspec
 *  \param[in]     TgtOffset         Target offset from the next 32-bit boundary.
 *                                   \spec requires
 *                                     (TgtOffset <= IPBASE_SOCK_POINTER_MAX_BYTE_OFFSET) &&
 *                                     (TgtOffset >= IPBASE_SOCK_POINTER_MIN_BYTE_OFFSET);
 *                                   \endspec
 *  \param[in]     SrcOffset         Source offset from the next 32-bit boundary.
 *                                   \spec requires
 *                                     (SrcOffset <= IPBASE_SOCK_POINTER_MAX_BYTE_OFFSET);
 *                                   \endspec
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumUnalignedCopyAddBlockwiseHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_P2VARAPPLDATA(uint32) LeftoverBytesPtr,
                      IPBASE_CONSTCONST(uint8) TgtOffset,
                      IPBASE_CONSTCONST(uint8) SrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/**********************************************************************************************************************
 * IpBase_UnalignedOneBlockCopyHlp
 **********************************************************************************************************************/
/*! \brief         Copies one 16-byte block of data to a previously unaligned target.
 *  \details       Is executed inside a loop and after a target offset correction. To allow an aligned 32-bit access
 *                 the data is byte-shifted in dependence of the target array offset. This makes the copy slower
 *                 compared to the aligned copy.
 *  \param[in,out] AlignedArrayCopyDataPtr
 *                                   Pointer to struct of aligned array data for an unaligned copy. The struct contains
 *                                   pointers to the target and source array and the remaining array length. The offset
 *                                   of the target pointer was corrected in a previous step.
 *                                   \spec requires
 *                                     AlignedArrayCopyDataPtr->LenByte >= IPBASE_SOCK_BYTE_LEN_BLOCK_BIG_LEN_BLOCK_COPY;
 *                                   \endspec
 *  \param[in]     WordIdx           Index of the next word in the target array.
 *                                   \spec requires
 *                                     (WordIdx + IPBASE_SOCK_MAX_WORD_IDX_IN_16_BYTE_BLOCK) <
 *                                     $lengthOf(AlignedArrayCopyDataPtr->TgtDataPtr);
 *  \param[in,out] LeftoverBytesPtr  Pointer to the bytes of the 32-bit word that are not copied yet in the preceding
 *                                   target offset correction.
 *                                   \spec requires
 *                                     LeftoverBytesPtr != NULL_PTR;
 *                                   \endspec
 *  \param[in]     BitShiftCount     Structure of bit shift counts in dependence of the target array offset.
 *                                   \spec requires
 *                                     (BitShiftCount.TowardWordStartIdx == IPBASE_SOCK_MIN_BIT_SHIFT_1_BYTE_IN_32BIT_WORD) ||
 *                                     (BitShiftCount.TowardWordStartIdx == IPBASE_SOCK_MAX_BIT_SHIFT_1_BYTE_IN_32BIT_WORD);
 *
 *                                     (BitShiftCount.TowardWordEndIdx == IPBASE_SOCK_MIN_BIT_SHIFT_1_BYTE_IN_32BIT_WORD) ||
 *                                     (BitShiftCount.TowardWordEndIdx == IPBASE_SOCK_MAX_BIT_SHIFT_1_BYTE_IN_32BIT_WORD);
 *                                   \endspec
 *  \param[out]    ModSrcWordsPtr    Pointer to the struct that stores the modified source words.
 *                                   \spec requires
 *                                     ModSrcWordsPtr != NULL_PTR;
 *                                   \endspec
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(void) IpBase_UnalignedOneBlockCopyHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint32_least) WordIdx,
                      IPBASE_P2VARAPPLDATA(uint32) LeftoverBytesPtr,
                      IPBASE_CONSTCONST(IpBase_SockUnalignedArrayCopyBitShiftStructType) BitShiftCount,
                      IPBASE_CONSTP2VARAPPLDATA(IpBase_SockUnalignedCopyModSrcWordsStructType) ModSrcWordsPtr);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/**********************************************************************************************************************
*  IpBase_PartialChecksumCopyAddHlp
*********************************************************************************************************************/
/*! \brief         Copies a partial word and calculates the checksum bytewise
 *  \details       Copies a partial word (of 1 to 3 bytes) bytewise from an unaligned source to reach the next 32-bit
 *                 boundary. Calculates the checksum by adapting the byte indexes in the checksum word.
 *  \param[out]    TgtDataPtr        Pointer to target array.
 *                                   \spec requires
 *                                     $lengthOf(TgtDataPtr) >= PartialWordLen;
 *                                   \endspec
 *  \param[in,out] TempSrcWordPtr    Pointer to the 32-bit source word. The value is returned modified.
 *                                   \spec requires
 *                                     $lengthOf(TempSrcWordPtr) >= PartialWordLen;
 *                                   \endspec
 *  \param[in]     PartialWordLen    Word length in bytes.
 *                                   \spec requires
 *                                     (PartialWordLen <= IPBASE_SOCK_POINTER_MAX_BYTE_OFFSET) &&
 *                                     (PartialWordLen >= IPBASE_SOCK_POINTER_MIN_BYTE_OFFSET);
 *                                   \endspec
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \param[in]     ChecksumIdxPtr    Pointer to the index of the next byte in the uint32 checksum.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_PartialChecksumCopyAddHlp(
                                              IPBASE_CONSTP2VARAPPLDATA(uint8) TgtDataPtr,
                                              IPBASE_CONSTP2VARAPPLDATA(uint32) TempSrcWordPtr,
                                              IPBASE_CONSTCONST(uint8) PartialWordLen,
                                              IPBASE_CONSTCONST(uint32) Checksum,
                                              IPBASE_CONSTP2VARAPPLDATA(uint8) ChecksumIdxPtr);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumAddHlp
**********************************************************************************************************************/
/*! \brief         Adds 4 bytes to the checksum
 *  \details       -
 *  \param[in]     Data              Data to add.
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \return        Calculated 32-bit checksum in network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumAddHlp(IPBASE_CONSTCONST(uint32) Data,
                                                                  IPBASE_CONSTCONST(uint32) Checksum);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumIntermediateOrFinalize
 **********************************************************************************************************************/
/*! \brief         Calculates the intermediate or finalized checksum
 *  \details       Takes a 32-bit checksum and returns a 16-bit checksum. The checksum is intermediate and in host byte
 *                 order or finalized and in network byte order.
 *  \param[in]     Checksum          Current 32-bit checksum of processed data in network byte order.
 *  \param[in]     Stop              Build the one's complement to finalize the checksum.
 *  \return        Calculated 16-bit checksum in host or network byte order.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \trace         CREQ-120251
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumIntermediateOrFinalize(IPBASE_CONSTCONST(uint32) Checksum,
                                                                                  boolean Stop);
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */
/* IPBASE_SOCK_DECL_ALIGNMENT_TEST_PLACEHOLDER */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  IpBase_DelSockAddr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_DelSockAddr(IPBASE_P2VARAPPLDATA(IpBase_SockAddrType) SockPtr, uint16 Family)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_OK;
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check SockPtr for NULL pointer */
  if ( SockPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 lenByte = 0u;                                                                                                 /* PRQA S 2981 */ /* MD_IpBase_2981 */

    /* #20 Check address family */
    if(Family == IPBASE_AF_INET)
    {
      lenByte = sizeof(IpBase_SockAddrInType);
    }
    else if(Family == IPBASE_AF_INET6)
    {
      lenByte = sizeof(IpBase_SockAddrIn6Type);
    }
    else
    {
      retVal = E_NOT_OK;
    }

    /* #30 Reset all bytes of socket address */
    if(retVal == E_OK)
    {
      /* PRQA S 314 1 */ /* MD_MSR_VStdLibCopy */
      VStdMemSet((IPBASE_P2VARAPPLDATA(void))SockPtr, 0, lenByte);                                                      /* SBSW_IPBASE_PASSING_API_POINTER */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_DEL_SOCK_ADDR, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* IpBase_DelSockAddr() */
/***********************************************************************************************************************
 *  IpBase_CopySockAddr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopySockAddr(IPBASE_P2VARAPPLDATA(IpBase_SockAddrType) TgtSockPtr,
                                                    IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SrcSockPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_OK;
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check SockPtr for NULL pointer */
  if ( (TgtSockPtr == NULL_PTR) || (SrcSockPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 lenByte = 0u;                                                                                                 /* PRQA S 2981 */ /* MD_IpBase_2981 */

    /* #20 Check address family */
    if(SrcSockPtr->sa_family == IPBASE_AF_INET)
    {
      lenByte = sizeof(IpBase_SockAddrInType);
    }
    else if(SrcSockPtr->sa_family == IPBASE_AF_INET6)
    {
      lenByte = sizeof(IpBase_SockAddrIn6Type);
    }
    else
    {
      retVal = E_NOT_OK;
    }

    /* #30 Copy all bytes of socket address */
    if(retVal == E_OK)
    { /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy  */
      VStdMemCpy((IPBASE_P2VARAPPLDATA(void))TgtSockPtr,                                                                /* SBSW_IPBASE_PASSING_API_POINTER */ /* VCA_IPBASE_API_SOCK_CONSISTENCY */
                 (IPBASE_P2CONSTAPPLDATA(void))SrcSockPtr, lenByte);
    }
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #40 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_COPY_SOCK_ADDR, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* IpBase_CopySockAddr() */
/***********************************************************************************************************************
 *  IpBase_CopyIpV6Addr
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(Std_ReturnType) IpBase_CopyIpV6Addr(IPBASE_P2VARAPPLDATA(IpBase_AddrIn6Type) TgtIpAddrPtr,
                                                    IPBASE_P2CONSTAPPLDATA(IpBase_AddrIn6Type) SrcIpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_OK;
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check IpAddrPtr for NULL pointer */
  if ( (TgtIpAddrPtr == NULL_PTR) || (SrcIpAddrPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Copy entire IPv6 address */
    /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy */
    VStdMemCpy((IPBASE_P2VARAPPLDATA(void))TgtIpAddrPtr,                                                                /* SBSW_IPBASE_PASSING_API_POINTER */ /* VCA_IPBASE_API_SOCK6_TYPE_CONSISTENCY */
               (IPBASE_P2CONSTAPPLDATA(void))SrcIpAddrPtr, sizeof(IpBase_AddrIn6Type));
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_COPY_IPV6_ADDR, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* IpBase_CopyIpV6Addr() */
/***********************************************************************************************************************
 *  IpBase_SockIpAddrIsEqual
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(boolean) IpBase_SockIpAddrIsEqual(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                  IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = TRUE;
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check SockPtr for NULL pointer */
  if ( (SockAPtr == NULL_PTR) || (SockBPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check for equal address using helper function */
    retVal = IpBase_SockIpAddrIsEqualHlp(SockAPtr, SockBPtr);                                                           /* SBSW_IPBASE_PASSING_API_POINTER */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_SOCK_IP_ADDR_IS_EQUAL, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* IpBase_SockIpAddrIsEqual() */

/***********************************************************************************************************************
 *  IpBase_SockPortIsEqual
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(boolean) IpBase_SockPortIsEqual(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = TRUE;
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check SockPtr for NULL pointer */
  if ( (SockAPtr == NULL_PTR) || (SockBPtr == NULL_PTR) )
  {
    errorId = IPBASE_E_INV_POINTER;
    retVal  = FALSE;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check for equal port */ /* PRQA S 310,3305 2 */ /* MD_IpBase_310_SockPtr */
    if( ((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrInType))SockAPtr)->sin_port !=
        ((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrInType))SockBPtr)->sin_port )
    {
      retVal = FALSE;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_SOCK_PORT_IS_EQUAL, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* IpBase_SockPortIsEqual() */
/***********************************************************************************************************************
 *  IpBase_CalcTcpIpChecksum
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint16) IpBase_CalcTcpIpChecksum(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 retVal = 0;
  uint8 errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check DataPtr for NULL pointer */
  if ( DataPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Calculate checksum */
    retVal = IpBase_CalcTcpIpChecksum2(DataPtr, LenByte, (IPBASE_P2VARAPPLDATA(uint8))NULL_PTR, 0);                     /* SBSW_IPBASE_PASSING_API_POINTER */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #30 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_CALC_TCPIP_CHECKSUM, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* IpBase_CalcTcpIpChecksum() */
/***********************************************************************************************************************
 *  IpBase_CalcTcpIpChecksum2
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint16) IpBase_CalcTcpIpChecksum2(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte,
                                                  IPBASE_P2CONSTAPPLDATA(uint8) PseudoHdrPtr,
                                                  uint32 PseudoHdrLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 retVal = 0;
  uint8  errorId = IPBASE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check DataPtr, PseudoHdrPtr for NULL pointer */
  if ( DataPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    uint32_least idx      = (uint32_least)(LenByte >> 1);  /* 16 bit LenByte, 8 bit idx -> divide by 2 */               /* PRQA S 4391 */ /* MD_IpBase_4391 */
    uint32       checksum = 0u;

    /* #20 Add checksum data */
    while ( idx != 0u ) /* FETA_IPBASE_SOCK_LEN_PARAM */
    {
      idx--;
      checksum += (uint16)(((uint32)(DataPtr[(idx << 1)]) << 8)+DataPtr[(idx << 1) + 1u]);
    }
    /* #30 Add odd byte */
    if ( (LenByte & 0x00000001u) != 0u )
    {
      checksum += (uint16)(((uint32)(DataPtr[LenByte - 1u]) << 8));
    }
    /* #40 Add pseudo header checksum */
    if ( PseudoHdrPtr != NULL_PTR )
    {
      idx   = (uint32_least)(PseudoHdrLenByte >> 1);                                                                    /* PRQA S 4391 */ /* MD_IpBase_4391 */
      while ( idx != 0u ) /* FETA_IPBASE_SOCK_LEN_PARAM */
      {
        idx--;
        checksum += (uint16)(((uint32)(PseudoHdrPtr[(idx << 1)]) << 8)+PseudoHdrPtr[(idx << 1) + 1u]);
      }
    }
    /* #50 Consider overflow */
    while ( (checksum>>16) != 0u ) /* FETA_IPBASE_SOCK_CHKSUM_SHIFT */
    {
      checksum = (uint32)((checksum & 0xFFFFu) + (checksum >> 16));
    }
    /* #60 Build one's complement */
    retVal = (uint16) ~checksum;
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #70 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_CALC_TCPIP_CHECKSUM2, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* IpBase_CalcTcpIpChecksum2() */
/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumAdd
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumAdd(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte,
                                                uint32 Checksum, boolean Stop)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8  errorId = IPBASE_E_NO_ERROR;
  uint32 retChksum = Checksum;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check DataPtr for NULL pointer */
  if ( DataPtr == NULL_PTR )
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    uint16 lower16bit;

    /* #20 Calculate additive checksum in helper function */
    retChksum = IpBase_TcpIpChecksumAddHlp(DataPtr, LenByte, retChksum);                                                /* SBSW_IPBASE_PASSING_API_POINTER */

    if ( Stop == (boolean)TRUE )
    { /* #30 Consider overflow after last data chunk */
      lower16bit = (uint16)(retChksum);
      retChksum  = (uint16)(lower16bit + (uint16)(retChksum>>16));
      if ( retChksum < lower16bit )
      { /* overflow */
        retChksum++;
      }
      /* #40 Build one's complement after last data chunk */
      retChksum = (uint16) ~retChksum;
      /* #50 Checksum is expected in network byte order -> change byte order depending on host platform */
#if ( CPU_BYTE_ORDER == LOW_BYTE_FIRST )                                                                                /* COV_IPBASE_BYTE_ORDER */
      retChksum = IpBase_ByteSwap16((uint16)retChksum);
#endif
    }
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  /* #60 Check ErrorId and report DET */
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_CALC_TCPIP_CHECKSUMADD, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 3112, 3199 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retChksum;
} /* IpBase_TcpIpChecksumAdd() */

/**********************************************************************************************************************
 * IpBase_TcpIpChecksumCopyAdd
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumCopyAdd(IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                    IPBASE_CONSTP2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                    uint32 LenByte,
                                                    uint32 Checksum,
                                                    boolean Stop)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = IPBASE_E_NO_ERROR;
  uint32 checksumRet = Checksum;

  /* ----- Development Error Checks ------------------------------------- */

  /* #10 Check the passed pointers for 'NULL_PTR'. */
  if ((TgtDataPtr == NULL_PTR) || (SrcDataPtr == NULL_PTR))
  {
    errorId = IPBASE_E_INV_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
    /* Store the original array data in a struct that is passed to the implementation sub-API.
       'Root' to distinguish from other array copy structs where the pointers do have a different alignment, type
       or length. */

    /* #20 Safely create the root struct as base for all checksum and copy operations. */
    IPBASE_CONSTCONST(IpBase_SockBytewiseArrayCopyStructType) rootArrayData = IpBase_CreateBytewiseCopyStruct(          /* SBSW_IPBASE_ROOT_PTR_VALIDITY */ /* VCA_IPBASE_API_PTR_CONSISTENCY */
                                                                                TgtDataPtr, SrcDataPtr, LenByte);

    /* #30 Execute the checksum and copy algorithm. */
    checksumRet = IpBase_TcpIpChecksumCopyAddHlp(&rootArrayData, Checksum, Stop);                                       /* SBSW_IPBASE_STRUCT_CREATED_VALIDITY */
#else
    /* #40 Copy data from source to destination. */
    IpBase_Copy(TgtDataPtr, SrcDataPtr, LenByte); /* SBSW_IPBASE_PASSING_API_POINTER */ /* VCA_IPBASE_API_TGT_SRC_CONSISTENCY */
    
    /* #50 Calculate checksum of data. */
    checksumRet = IpBase_TcpIpChecksumAdd(SrcDataPtr, /* SBSW_IPBASE_PASSING_API_POINTER */ /* VCA_IPBASE_API_TGT_SRC_CONSISTENCY */
                                          LenByte, checksumRet, Stop);

#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (IPBASE_DEV_ERROR_REPORT == STD_ON)
  if (errorId != IPBASE_E_NO_ERROR)
  {
    (void)Det_ReportError(IPBASE_MODULE_ID, IPBASE_INSTANCE_ID, IPBASE_API_SID_CALC_TCPIP_CHECKSUMCPYADD, errorId);
  }
#else
  IPBASE_DUMMY_STATEMENT(errorId);                                                                                      /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return checksumRet;
} /* IpBase_TcpIpChecksumCopyAdd() */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  IpBase_SockIpAddrIsEqualHlp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(boolean) IpBase_SockIpAddrIsEqualHlp(IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockAPtr,
                                                                  IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal  = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for equal address family */
  if(SockAPtr->sa_family != SockBPtr->sa_family)
  {
    retVal = FALSE;
  }
  else if(SockAPtr->sa_family == IPBASE_AF_INET)
  { /* #20 Check for equal IPv4 address */ /* PRQA S 310,3305 2 */ /* MD_IpBase_310_SockPtr */
    if(((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrInType))SockAPtr)->sin_addr !=
       ((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrInType))SockBPtr)->sin_addr)
    {
      retVal = FALSE;
    }
  }
  else if(SockAPtr->sa_family == IPBASE_AF_INET6)
  { /* #30 Check for equal IPv6 address */
    /* PRQA S 310,3305 2 */ /* MD_IpBase_310_SockPtr */
    retVal = IpBase_SockIpV6AddrIsEqualHlp((IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrIn6Type))SockAPtr,                    /* SBSW_IPBASE_PASSING_API_POINTER */
                                           (IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrIn6Type))SockBPtr);
  }
  else
  { /* #40 Unknown address family */
    retVal = FALSE;
  }

  return retVal;
} /* IpBase_SockIpAddrIsEqualHlp() */
/**********************************************************************************************************************
 *  IpBase_SockIpV6AddrIsEqualHlp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(boolean) IpBase_SockIpV6AddrIsEqualHlp(
                                        IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrIn6Type) SockAPtr,
                                        IPBASE_P2CONSTAPPLDATA(IpBase_SockAddrIn6Type) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal  = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for equal IPv6 address */
  uint16_least i = sizeof(IpBase_AddrIn6Type);

  while(i != 0u) /* FETA_IPBASE_SOCK_ADDRIN_TYPE */
  {
    i--;
    if( SockAPtr->sin6_addr.addr[i] != SockBPtr->sin6_addr.addr[i] )
    {
      retVal = FALSE;
      break;
    }
  }

  return retVal;
} /* IpBase_SockIpV6AddrIsEqualHlp() */
/***********************************************************************************************************************
 *  IpBase_TcpIpChecksumAddHlp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
IPBASE_LOCAL IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumAddHlp(IPBASE_P2CONSTAPPLDATA(uint8) DataPtr, uint32 LenByte,
                                                                uint32 Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 dataIdx  = 0u;
  uint32 checksumRet  = Checksum;
  uint32 lenByteLoc = LenByte;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add Checksum */
  while ( lenByteLoc > 3u ) /* FETA_IPBASE_SOCK_LEN_PARAM */
  { /* #20 Add 32 bit values */
    lenByteLoc -= 4u;
    checksumRet += IpBase_GetUint32(DataPtr, dataIdx);                                                                  /* SBSW_IPBASE_PASSING_API_POINTER */
    if ( checksumRet < IpBase_GetUint32(DataPtr, dataIdx) )                                                             /* SBSW_IPBASE_PASSING_API_POINTER */
    { /* overflow */
      checksumRet++;
    }
    dataIdx += 4u;
  }

  if ( lenByteLoc > 1u )
  { /* #30 Add two '32-odd' bytes */
    lenByteLoc -= 2u;
    checksumRet += IpBase_GetUint16(DataPtr, dataIdx);                                                                  /* SBSW_IPBASE_PASSING_API_POINTER */
    if ( checksumRet < IpBase_GetUint16(DataPtr, dataIdx) )                                                             /* SBSW_IPBASE_PASSING_API_POINTER */
    { /* overflow */
      checksumRet++;
    }
    dataIdx += 2u;
  }

  if ( lenByteLoc > 0u )
  { /* #40 Add one '32-odd' bytes */
    checksumRet += (uint32)(DataPtr[dataIdx]) << 8u;
    if ( checksumRet < ((uint32)(DataPtr[dataIdx]) << 8u) )
    { /* overflow */
      checksumRet++;
    }
  }

  return checksumRet;
} /* IpBase_TcpIpChecksumAddHlp() */

/* ---- Checksum and Copy Functions ---- */
/***********************************************************************************************************************
 * IpBase_CreateBytewiseCopyStruct
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(IpBase_SockBytewiseArrayCopyStructType) IpBase_CreateBytewiseCopyStruct(
                                                                              IPBASE_P2VARAPPLDATA(uint8) TgtDataPtr,
                                                                              IPBASE_P2CONSTAPPLDATA(uint8) SrcDataPtr,
                                                                              IPBASE_CONSTCONST(uint32) LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_SockBytewiseArrayCopyStructType bytewiseArrayData;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Create the bytewise copy struct using the input parameters. */
  bytewiseArrayData.TgtDataPtr = TgtDataPtr;
  bytewiseArrayData.SrcDataPtr = SrcDataPtr;
  bytewiseArrayData.LenByte    = LenByte;

  return bytewiseArrayData;
} /* IpBase_CreateBytewiseCopyStruct */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_CreateAlignedCopyStruct
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(IpBase_SockAlignedArrayCopyStructType) IpBase_CreateAlignedCopyStruct(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_SockAlignedArrayCopyStructType alignedArrayData;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Create the aligned copy struct using the input parameters. */

  /* A cast to uint32 is allowed because the input pointers are 32-bit aligned */
  alignedArrayData.TgtDataPtr = (IPBASE_P2VARAPPLDATA(uint32))(BytewiseArrayDataPtr->TgtDataPtr);                       /* PRQA S 310,3305 */ /* MD_IpBase_310_3305_PtrCast */
  alignedArrayData.SrcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint32))(BytewiseArrayDataPtr->SrcDataPtr);                     /* PRQA S 310,3305 */ /* MD_IpBase_310_3305_PtrCast */
  alignedArrayData.LenByte    = BytewiseArrayDataPtr->LenByte;

  return alignedArrayData;
} /* IpBase_CreateAlignedCopyStruct */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_ConsumeCopiedDataBytewise
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(void) IpBase_ConsumeCopiedDataBytewise(
                      IPBASE_CONSTP2VARAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayDataPtr,
                      IPBASE_CONSTCONST(uint32) CopiedBytes)
{
  /* ----- Implementation ----------------------------------------------- */

  /* #10 If the input values are plausible, set the pointers to the array element where the preceding copy stopped. Set
     the length to the remaining length. */
  if (CopiedBytes <= BytewiseArrayDataPtr->LenByte)
  {
    BytewiseArrayDataPtr->TgtDataPtr = &(BytewiseArrayDataPtr->TgtDataPtr[CopiedBytes]);                                /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
    BytewiseArrayDataPtr->SrcDataPtr = &(BytewiseArrayDataPtr->SrcDataPtr[CopiedBytes]);                                /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
    BytewiseArrayDataPtr->LenByte    = BytewiseArrayDataPtr->LenByte - CopiedBytes;                                     /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  }
} /* IpBase_ConsumeCopiedDataBytewise */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_ConsumeReadWrittenDataBytewise
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(void) IpBase_ConsumeReadWrittenDataBytewise(
                      IPBASE_CONSTP2VARAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayDataPtr,
                      IPBASE_CONSTCONST(uint32) WrittenBytes,
                      IPBASE_CONSTCONST(uint32) ReadBytes)
{
  /* ----- Implementation ----------------------------------------------- */

  /* #10 If the input values are plausible, set the pointers to the array element where the preceding copy stopped. Set
     the length to the remaining read length. */
  if ( (ReadBytes    <= BytewiseArrayDataPtr->LenByte) &&
       (WrittenBytes <= ReadBytes) )
  {
    BytewiseArrayDataPtr->TgtDataPtr = &(BytewiseArrayDataPtr->TgtDataPtr[WrittenBytes]);                               /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
    BytewiseArrayDataPtr->SrcDataPtr = &(BytewiseArrayDataPtr->SrcDataPtr[ReadBytes]);                                  /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
    BytewiseArrayDataPtr->LenByte    = BytewiseArrayDataPtr->LenByte - ReadBytes;                                       /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  }
} /* IpBase_ConsumeReadWrittenDataBytewise */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_TcpIpChecksumCopyAddHlp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_TcpIpChecksumCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) RootArrayDataPtr,
                      IPBASE_CONSTCONST(uint32) Checksum,
                      IPBASE_CONSTCONST(boolean) Stop)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* Get the target and source array-pointer offset from the next 32-bit boundary. */
  IPBASE_CONSTCONST(uint8) srcOffset = IpBase_AlignmentOffset32Bit(RootArrayDataPtr->SrcDataPtr);                       /* SBSW_IPBASE_NUMERIC_POINTER_ADDRESS */
  IPBASE_CONSTCONST(uint8) tgtOffset = IpBase_AlignmentOffset32Bit(RootArrayDataPtr->TgtDataPtr);                       /* SBSW_IPBASE_NUMERIC_POINTER_ADDRESS */
  uint32 checksumRet = Checksum;

  /* ----- Implementation ----------------------------------------------- */

  /* Convert the checksum to 'high byte first' byte order on 'low byte first' platforms. */
  checksumRet = IPBASE_HTON32(checksumRet);

  /* #10 If less than 42 bytes (32 bytes + max offset sum + 32-bit word length) have to be copied, use the small length
     copy and checksum algorithms, otherwise use the big length algorithms. */
  if (RootArrayDataPtr->LenByte < IPBASE_SOCK_BYTE_LEN_THRESHOLD_BIG_LEN_COPY)
  {
    checksumRet = IpBase_SmallLenChecksumCopyAddHlp(RootArrayDataPtr, tgtOffset, srcOffset, checksumRet);               /* SBSW_IPBASE_STRUCT_PASSED_VALIDITY */
  }
  else
  {
    /* The big length algorithms (containing a 16-byte block copy and checksum add) make sense when at least 32 bytes
       are copied blockwise (to iterate the for loop at least twice). The threshold of 42 bytes assures that at least
       32 bytes reach the 16-byte block copy algorithms even in the scenario where a maximum offset correction for the
       target and source pointer (3 + 3 bytes) and an unaligned copy (with one word read ahead) is used. */
    checksumRet = IpBase_BigLenChecksumCopyAddHlp(RootArrayDataPtr, srcOffset, checksumRet);                            /* SBSW_IPBASE_STRUCT_PASSED_SRCOFFSET_VALIDITY */
  }

  /* #20 Calculate the intermediate or finalized checksum. */
  checksumRet = IpBase_ChecksumIntermediateOrFinalize(checksumRet, Stop);

  return checksumRet;
} /* IpBase_TcpIpChecksumCopyAddHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_AlignmentOffset32Bit
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint8) IpBase_AlignmentOffset32Bit(IPBASE_CONSTP2CONSTAPPLDATA(uint8) Pointer)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* Cast the pointer to an integer type to get the address for the offset calculation.
     A double cast is done: First cast the pointer to its integral type (uint32 or uint64 depending on the platform).
     Then cast it to uint8 because only the last two bits are of interest. Directly casting to uint8 leads to compiler
     warnings (pointer truncation/sign-extended warning and uint32/uint64 conversion to uint8 warning). */
  uint8 offset = (uint8)((IPBASE_CHECKSUM_DATA_POINTER_TYPE)Pointer);                                                   /* PRQA S 0306 */ /* MD_IpBase_0306_PtrAddrCastToInt */

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Calculate the offset from the next 32-bit boundary */

  /* Subtract the offset from the value that resembles an aligned pointer. */
  offset = (uint8)(IPBASE_SOCK_VALUE_BY_1_GREATER_THAN_MAX_BYTE_VALUE - offset);
  /* Only take the last two bits to get the offset between 0 and 3 (modulo 4). */
  offset = offset % IPBASE_SOCK_MODULO_4_BYTE_OFFSET;

  return offset;
} /* IpBase_AlignmentOffset32Bit() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_SmallLenChecksumCopyAddHlp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_SmallLenChecksumCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) RootArrayDataPtr,
                      IPBASE_CONSTCONST(uint8) TgtOffset,
                      IPBASE_CONSTCONST(uint8) SrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 checksumRet = Checksum;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 If the target and source offset sum is 0 (target and source are 32-bit aligned), do a 4-byte bock copy
         (faster). Else do a bytewise copy (slower). */
  if ( (TgtOffset == 0u) && (SrcOffset == 0u) )
  {
    /* Cast the original uint8 to uint32 pointers for a 32-bit access. This is allowed because the target and source are
       32-bit aligned. */
    IPBASE_CONSTCONST(IpBase_SockAlignedArrayCopyStructType) alignedArrayData = IpBase_CreateAlignedCopyStruct(         /* SBSW_IPBASE_STRUCT_PASSED_VALIDITY */
                                                                                  RootArrayDataPtr);

    /* Copy in 4-byte blocks. */
    checksumRet = IpBase_ChecksumCopyAddBlockwiseHlp(&alignedArrayData,                                                 /* SBSW_IPBASE_STRUCT_CREATED_VALIDITY */
                                                     checksumRet);
  }
  else
  {
    uint8 checksumIdx = 0; /* A new checksum word starts at byte index 0. */

    /* Copy all bytes bytewise. */
    checksumRet = IpBase_ChecksumCopyAddBytewiseHlp(RootArrayDataPtr,                                                   /* SBSW_IPBASE_STRUCT_PASSED_CHECKSUMIDX_LOCAL_VALIDITY */
                                                    &checksumIdx,
                                                    checksumRet);
  }

  return checksumRet;
} /* IpBase_SmallLenChecksumCopyAddHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/**********************************************************************************************************************
 * IpBase_ChecksumCopyAddBlockwiseHlp
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumCopyAddBlockwiseHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint32) Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* Resulting word length for the block copy. */
  IPBASE_CONSTCONST(uint32) wordLenBlockCopy = AlignedArrayCopyDataPtr->LenByte / IPBASE_SOCK_BYTE_LEN_32_BIT_WORD;
  uint32_least wordIdx = 0u; /* Counter in the for loop. */
  uint32 srcWord = 0; /* Stores the words of the source array. */
  uint32 checksumRet = Checksum;

  /* ----- Implementation ----------------------------------------------- */

  /* IPBASE_SOCK_CALL_ALIGNMENT_TEST_PLACEHOLDER */

  /* #10 Copy from the source to the target and add to the checksum in 4-byte blocks. */
  for (wordIdx = 0; wordIdx < wordLenBlockCopy; wordIdx++)
  {
    /* Read 4 bytes from the source array. */
    srcWord = AlignedArrayCopyDataPtr->SrcDataPtr[wordIdx];
    /* Store the 4 bytes in the target array. */
    AlignedArrayCopyDataPtr->TgtDataPtr[wordIdx] = srcWord;                                                             /* SBSW_IPBASE_STRUCT_PTR_IDX_ACCESS */
    /* Add 4 bytes to the checksum. */
    checksumRet = IpBase_ChecksumAddHlp(srcWord, checksumRet);
  }

  /* #20 Copy the remaining bytes and add to the checksum bytewise. */
  {
    uint8 checksumIdx = 0u; /* A new checksum word starts at byte index 0. */
    /* The bytes already copied via block copy. */
    uint32 copiedBytesAfterBlockCopy = wordLenBlockCopy * IPBASE_SOCK_BYTE_LEN_32_BIT_WORD;

    checksumRet = IpBase_ChecksumCopyAddRemainingBytesHlp(AlignedArrayCopyDataPtr,                                      /* SBSW_IPBASE_STRUCT_PASSED_CHECKSUMIDX_LOCAL_VALIDITY */
                                                          copiedBytesAfterBlockCopy,
                                                          &checksumIdx,
                                                          checksumRet);
  }

  return checksumRet;
} /* IpBase_ChecksumCopyAddBlockwiseHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_ChecksumCopyAddRemainingBytesHlp
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumCopyAddRemainingBytesHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint32) CopiedBytes,
                      IPBASE_CONSTP2VARAPPLDATA(uint8) ChecksumIdxPtr,
                      IPBASE_CONSTCONST(uint32) Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 checksumRet = Checksum;

  /* Re-cast the passed uint32 to uint8 pointers for an 8-bit access. A cast to unit8 is always allowed. */
  IPBASE_P2VARAPPLDATA(uint8) tgtDataPtr   = (IPBASE_P2VARAPPLDATA(uint8))(AlignedArrayCopyDataPtr->TgtDataPtr);
  IPBASE_P2CONSTAPPLDATA(uint8) srcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint8))(AlignedArrayCopyDataPtr->SrcDataPtr);

  /* Create the copy structure for bytewise copy. */
  IpBase_SockBytewiseArrayCopyStructType bytewiseArrayData = IpBase_CreateBytewiseCopyStruct(                           /* SBSW_IPBASE_PTR_PASSED_CASTED_VALIDITY */ /* VCA_IPBASE_PTR_CONSISTENCY */
                                                               tgtDataPtr,
                                                               srcDataPtr,
                                                               AlignedArrayCopyDataPtr->LenByte);

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Copy the remaining bytes and add to the checksum bytewise. */

  /* Consume the already copied data. */
  IpBase_ConsumeCopiedDataBytewise(&bytewiseArrayData, CopiedBytes);                                                    /* SBSW_IPBASE_STRUCT_CREATED_VALIDITY */

  /* Copy and add to the checksum bytewise. */
  checksumRet = IpBase_ChecksumCopyAddBytewiseHlp(&bytewiseArrayData,                                                   /* SBSW_IPBASE_STRUCT_CREATED_CHECKSUMIDX_ARG_VALIDITY */
                                                  ChecksumIdxPtr,
                                                  checksumRet);

  return checksumRet;
} /* IpBase_ChecksumCopyAddRemainingBytesHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_ChecksumCopyAddBytewiseHlp
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumCopyAddBytewiseHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayDataPtr,
                      IPBASE_CONSTP2VARAPPLDATA(uint8) ChecksumIdxPtr,
                      IPBASE_CONSTCONST(uint32) Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */

  uint32_least byteIdx = 0u; /* Counter in for loop. */
  uint32 checksumRet = Checksum;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Copy and add to the checksum bytewise. */
  for (byteIdx = 0u; byteIdx < BytewiseArrayDataPtr->LenByte; byteIdx++)
  {
    BytewiseArrayDataPtr->TgtDataPtr[byteIdx] = BytewiseArrayDataPtr->SrcDataPtr[byteIdx];                              /* SBSW_IPBASE_STRUCT_PTR_IDX_ACCESS */
    checksumRet = IpBase_ChecksumAddByteHlp(BytewiseArrayDataPtr->SrcDataPtr[byteIdx], checksumRet, ChecksumIdxPtr);    /* SBSW_IPBASE_PASSING_API_POINTER */
  }

  return checksumRet;
} /* IpBase_ChecksumCopyAddBytewiseHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumAddByteHlp
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumAddByteHlp(
                                              IPBASE_CONSTCONST(uint8) Data,
                                              IPBASE_CONSTCONST(uint32) Checksum,
                                              IPBASE_CONSTP2VARAPPLDATA(uint8) ChecksumIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  IPBASE_CONSTCONST(uint8) bitShiftCountLowByteFirst = (IPBASE_SOCK_BIT_LEN_1_BYTE * (*ChecksumIdxPtr));
  uint32 byteInWord = 0; /* The data byte positioned in the 32-bit word. */
  uint32 checksumRet = Checksum;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Add one byte to the next index in the 'high byte first' representation of the checksum word. */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)                                                                                  /* COV_IPBASE_BYTE_ORDER */
  byteInWord = (uint32)Data << bitShiftCountLowByteFirst;
#else
  IPBASE_CONSTCONST(uint8) bitShiftCountHighByteFirst = (IPBASE_SOCK_BIT_LEN_3_BYTES - bitShiftCountLowByteFirst);
  byteInWord = (uint32)Data << bitShiftCountHighByteFirst;
#endif
  checksumRet += byteInWord;
  if (checksumRet < byteInWord)
  {
    /* Overflow */
    checksumRet++;
  }
  /* #20 If the checksum word is not full yet add 1 to the byte index. Else set the index to 0. */
  if (*ChecksumIdxPtr < IPBASE_SOCK_MAX_BYTE_IDX_IN_32_BIT_WORD)
  {
    (*ChecksumIdxPtr)++;                                                                                                /* SBSW_IPBASE_VALID_API_POINTER */
  }
  else
  {
    *ChecksumIdxPtr = IPBASE_SOCK_MIN_BYTE_IDX_IN_32_BIT_WORD;                                                          /* SBSW_IPBASE_VALID_API_POINTER */
  }

  return checksumRet;
} /* IpBase_ChecksumAddByteHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_BigLenChecksumCopyAddHlp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_BigLenChecksumCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) RootArrayDataPtr,
                      IPBASE_CONSTCONST(uint8) RootSrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* Target pointer offset from the next 32-bit boundary after the source offset correction. */
  uint8 tgtOffsetAfterSrcOffsetCorrection = 0;
  /* Remaining length in bytes after the source offset correction. */
  uint8 checksumIdx = 0; /* A new checksum word starts at byte index 0. */
  uint32 checksumRet = Checksum;
  /* Local struct for bytewise copy and source offset corrected data. */
  IpBase_SockBytewiseArrayCopyStructType currentBytewiseArrayData = *RootArrayDataPtr;

  /* ----- Implementation ----------------------------------------------- */

  /* Copy of root struct in case the next if statement is not entered. */

  /* #10 If the source is not 32-bit aligned do a source pointer offset correction. */
  if (RootSrcOffset != 0u)
  {
    /* Create the copy struct for bytewise copy. */
    IpBase_SockBytewiseArrayCopyStructType correctOffsetBytewiseArrayData = IpBase_CreateBytewiseCopyStruct(            /* SBSW_IPBASE_PTR_LEN_OFFSET_VALIDITY */ /* VCA_IPBASE_MOD_STRUCT_OFFSET_CONSISTENCY */
                                                                              RootArrayDataPtr->TgtDataPtr,
                                                                              RootArrayDataPtr->SrcDataPtr,
                                                                              RootSrcOffset);

    /* Copy the offset bytes to reach the next 32-bit boundary of the source. This will be the source pointer starting
       point for the 16-byte block copy. */
    checksumRet = IpBase_ChecksumCopyAddBytewiseHlp(&correctOffsetBytewiseArrayData,                                    /* SBSW_IPBASE_STRUCT_CREATED_CHECKSUMIDX_LOCAL_VALIDITY */
                                                    &checksumIdx, /* Checksum index is passed as 0 and returned as
                                                                     'srcOffset'. */
                                                    checksumRet);

    /* Consume data, set the pointers to the new start addresses and reduce the length. */
    IpBase_ConsumeCopiedDataBytewise(&currentBytewiseArrayData, RootSrcOffset);                                         /* SBSW_IPBASE_STRUCT_PASSED_VALIDITY */
  }

  /* Get the new target pointer offset after the source is aligned. */
  tgtOffsetAfterSrcOffsetCorrection = IpBase_AlignmentOffset32Bit(currentBytewiseArrayData.TgtDataPtr);                 /* SBSW_IPBASE_NUMERIC_POINTER_ADDRESS */

  /* #20 If the target is 32-bit aligned, do a 16-byte block copy via the 'aligned' algorithm. Else do a 16-byte
     block copy via the 'unaligned' algorithm. */
  if (tgtOffsetAfterSrcOffsetCorrection == 0u)
  {
    /* Create the copy struct for aligned copy. */
    IPBASE_CONSTCONST(IpBase_SockAlignedArrayCopyStructType) alignedArrayData = IpBase_CreateAlignedCopyStruct(
                                                                                  &currentBytewiseArrayData);           /* SBSW_IPBASE_STRUCT_CREATED_VALIDITY */

    /* Do an aligned 16-byte block copy and calculate the checksum via the full or partial checksum algorithm. */
    checksumRet = IpBase_ChecksumAlignedCopyAddHlp(&alignedArrayData,                                                   /* SBSW_IPBASE_STRUCT_CREATED_VALIDITY */ /* VCA_IPBASE_ARRAY_AND_OFFSET_LEN_ALIGNED_COPY_CONSISTENCY */
                                                   RootSrcOffset,
                                                   checksumRet);
  }
  else
  {
    /* Do an unaligned copy and calculate the checksum. */
    checksumRet = IpBase_ChecksumUnalignedCopyAddHlp(&currentBytewiseArrayData,                                         /* SBSW_IPBASE_STRUCT_CREATED_VALIDITY */ /* VCA_IPBASE_ARRAY_AND_OFFSET_LEN_UNALIGNED_COPY_CONSISTENCY */
                                                     tgtOffsetAfterSrcOffsetCorrection,
                                                     RootSrcOffset,
                                                     checksumRet);
  }

  return checksumRet;
} /* IpBase_BigLenChecksumCopyAddHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_ChecksumAlignedCopyAddHlp
 **********************************************************************************************************************/
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
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumAlignedCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint8) RootSrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* Number of full 16-byte blocks in the arrays. */
  IPBASE_CONSTCONST(uint32) full16ByteBlocks = AlignedArrayCopyDataPtr->LenByte /
                                                 IPBASE_SOCK_BYTE_LEN_BLOCK_BIG_LEN_BLOCK_COPY;
  /* Resulting word length for the block copy. */
  IPBASE_CONSTCONST(uint32) wordLenBlockCopy = full16ByteBlocks * IPBASE_SOCK_BYTE_LEN_32_BIT_WORD;
  uint32_least wordIdx = 0u; /* Counter in for loop. */
   /* 'Source offset' bytes are already copied and added to the partial checksum. So the next index in the checksum
       word is 'RootSrcOffset'. */
  uint8 checksumIdx = RootSrcOffset;
  uint32 checksumRet = Checksum;

  /* ----- Implementation ----------------------------------------------- */

  /* IPBASE_SOCK_CALL_ALIGNMENT_TEST_PLACEHOLDER */

  /* #10 If the root source offset is odd, swap the current checksum. */
  if ((RootSrcOffset % IPBASE_SOCK_MODULO_2_OFFSET_EVEN_ODD) != 0u)
  {
    checksumRet = IPBASE_BYTE_SWAP32(checksumRet);
  }


  /* #20 Copy and add 16 bytes to the checksum. */
  for (wordIdx = 0; wordIdx < wordLenBlockCopy; wordIdx += 4u)
  {
    /* #30 Copy one 16-byte block. */
    IpBase_AlignedOneBlockCopyHlp(AlignedArrayCopyDataPtr, wordIdx);                                                    /* SBSW_IPBASE_STRUCT_PASSED_IDX_VALIDITY */ /* VCA_IPBASE_STRUCT_IDX_VALIDITY */

    /* #40 Calculate the checksum for 4 x 4 bytes. */
    checksumRet = IpBase_ChecksumAddHlp(AlignedArrayCopyDataPtr->SrcDataPtr[wordIdx]     , checksumRet);
    checksumRet = IpBase_ChecksumAddHlp(AlignedArrayCopyDataPtr->SrcDataPtr[wordIdx + 1u], checksumRet);
    checksumRet = IpBase_ChecksumAddHlp(AlignedArrayCopyDataPtr->SrcDataPtr[wordIdx + 2u], checksumRet);
    checksumRet = IpBase_ChecksumAddHlp(AlignedArrayCopyDataPtr->SrcDataPtr[wordIdx + 3u], checksumRet);
  }

  /* #50 If the root source offset is odd, swap the calculated checksum. */
  if ((RootSrcOffset % IPBASE_SOCK_MODULO_2_OFFSET_EVEN_ODD) != 0u)
  {
    checksumRet = IPBASE_BYTE_SWAP32(checksumRet);
  }

  /* #60 Copy the tail bytewise and add to the checksum bytewise. */
  {
    /* The number of bytes already copied. */
    uint32 copiedBytesAfterBlockCopy = wordLenBlockCopy * IPBASE_SOCK_BYTE_LEN_32_BIT_WORD;

    checksumRet = IpBase_ChecksumCopyAddRemainingBytesHlp(AlignedArrayCopyDataPtr,                                      /* SBSW_IPBASE_STRUCT_PASSED_CHECKSUMIDX_LOCAL_VALIDITY */
                                                          copiedBytesAfterBlockCopy,
                                                          &checksumIdx,
                                                          checksumRet);
  }

  return checksumRet;
} /* IpBase_ChecksumAlignedCopyAddHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_AlignedOneBlockCopyHlp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(void) IpBase_AlignedOneBlockCopyHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint32_least) WordIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* Store the source words in local variables. */
  IPBASE_CONSTCONST(uint32) srcWord0 = AlignedArrayCopyDataPtr->SrcDataPtr[WordIdx];
  IPBASE_CONSTCONST(uint32) srcWord1 = AlignedArrayCopyDataPtr->SrcDataPtr[WordIdx + 1u];
  IPBASE_CONSTCONST(uint32) srcWord2 = AlignedArrayCopyDataPtr->SrcDataPtr[WordIdx + 2u];
  IPBASE_CONSTCONST(uint32) srcWord3 = AlignedArrayCopyDataPtr->SrcDataPtr[WordIdx + 3u];

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Copy the 16-byte block from the source to the target in 4 x 4 bytes. */
  AlignedArrayCopyDataPtr->TgtDataPtr[WordIdx]      = srcWord0;                                                         /* SBSW_IPBASE_TGT_IDXINC_ACCESS */
  AlignedArrayCopyDataPtr->TgtDataPtr[WordIdx + 1u] = srcWord1;                                                         /* SBSW_IPBASE_TGT_IDXINC_ACCESS */
  AlignedArrayCopyDataPtr->TgtDataPtr[WordIdx + 2u] = srcWord2;                                                         /* SBSW_IPBASE_TGT_IDXINC_ACCESS */
  AlignedArrayCopyDataPtr->TgtDataPtr[WordIdx + 3u] = srcWord3;                                                         /* SBSW_IPBASE_TGT_IDXINC_ACCESS */
} /* IpBase_AlignedOneBlockCopyHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_ChecksumUnalignedCopyAddHlp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumUnalignedCopyAddHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockBytewiseArrayCopyStructType) BytewiseArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint8) TgtOffset,
                      IPBASE_CONSTCONST(uint8) SrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum)
{
  /*  Different copy and checksum calculation algorithms are used for copying head, main and tail data:
   *      1)  Copy the head
   *          Do a bytewise partial word copy for the target offset correction (1 to 3 bytes) and accordingly calculate
   *          the checksum by adding bytewise.
   *      2)  Copy the main data
   *          Copy the main data in 16-byte blocks (4 x 4 bytes) by using the unaligned copy and accordingly calculate
   *          the checksum by adding 4 x 4 bytes. The leftover bytes from step 1) are used to start a new word which is
   *          filled with the read-ahead bytes - what produces new leftover bytes etc. Due to this mechanism the
   *          unaligned copy is slower than the aligned copy.
   *      3)  Copy the tail
   *          a)  Fill the partial word of step 1) bytewise and accordingly calculate the checksum by adding bytewise.
   *          b)  Copy the remaining bytes bytewise and accordingly calculate the checksum by adding bytewise.

   * ----- Local Variables ---------------------------------------------- */

  uint8 checksumIdx = SrcOffset; /* Source offset bytes are already copied and added to the partial checksum. So the
                                    next index in the checksum word is 'SrcOffset'. */
  uint32 checksumRet = Checksum;
  /* Cast the uint8 to an uint32 pointer to read the read-ahead source word. A cast to unit32 is allowed because the
     source data pointer is 32-bit aligned. */
  IPBASE_P2CONSTAPPLDATA(uint32) srcDataPtr = (IPBASE_P2CONSTAPPLDATA(uint32))(BytewiseArrayCopyDataPtr->SrcDataPtr);   /* PRQA S 310,3305 */ /* MD_IpBase_310_3305_PtrCast */
  /* Read-ahead source word that will be copied partially to the target in two steps. */
  uint32 readAheadPartialSrcWord = srcDataPtr[0u];
  /* Current array data, will be modified for each copy step. */
  IpBase_SockBytewiseArrayCopyStructType currentBytewiseArrayData = *BytewiseArrayCopyDataPtr;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Partially copy the read-ahead source word till the 32-bit boundary of the target is reached. */
  checksumRet = IpBase_PartialChecksumCopyAddHlp(BytewiseArrayCopyDataPtr->TgtDataPtr,                                  /* SBSW_IPBASE_PTR_PARTIAL_FIRST_VALIDITY */ /* VCA_IPBASE_PTR_PARTIAL_FIRST_CONSISTENCY */
                                                 &readAheadPartialSrcWord,
                                                 TgtOffset, /* The target offset is passed as new length. */
                                                 checksumRet,
                                                 &checksumIdx);

  /* Consume the written and read bytes of the first partial word copy. */
  IpBase_ConsumeReadWrittenDataBytewise(&currentBytewiseArrayData,                                                      /* SBSW_IPBASE_STRUCT_PASSED_VALIDITY */
                                        TgtOffset,
                                        IPBASE_SOCK_BYTE_LEN_32_BIT_WORD);

  /* At this point 'currentBytewiseArrayDataPtr->TgtDataPtr[TgtOffset]' is aligned to a 32-bit boundary. Now an aligned
     32-bit pointer access is possible. */

  /* #20 Do an unaligned 16-byte block copy and calculate the checksum via the full or partial checksum algorithm. */
  {
    /* Create the aligned copy struct. */
    IPBASE_CONSTCONST(IpBase_SockAlignedArrayCopyStructType) alignedArrayData = IpBase_CreateAlignedCopyStruct(         /* SBSW_IPBASE_STRUCT_CREATED_VALIDITY */
                                                                                  &currentBytewiseArrayData);

    /* Copy and add to the checksum blockwise. */
    checksumRet = IpBase_ChecksumUnalignedCopyAddBlockwiseHlp(&alignedArrayData,                                        /* SBSW_IPBASE_STRUCT_CREATED_SRCWORD_VALIDITY */
                                                              &readAheadPartialSrcWord,
                                                              TgtOffset,
                                                              SrcOffset,
                                                              checksumRet);
  }

  /* #30 Copy the tail bytewise. */
  {
    /* Calculate the remaining lengths and the read/written byte counts. */

    IpBase_SockBytewiseArrayCopyStructType tailBytewiseArrayData = *BytewiseArrayCopyDataPtr;

    /* The remaining source length after one word read-ahead. */
    uint32 remSrcLenAfterReadAhead = BytewiseArrayCopyDataPtr->LenByte - IPBASE_SOCK_BYTE_LEN_32_BIT_WORD;
    /* The remaining length in bytes after the 16-byte block copy.*/
    IPBASE_CONSTCONST(uint32) remSrcLenAfterBlockCopy = remSrcLenAfterReadAhead % 16u;
    /* The number of read source bytes after the block copy (includes the read-ahead source word). This equals the
       number of copied bytes after the second partial word copy is done. */
    uint32 srcReadBytesAfterBlockCopy = BytewiseArrayCopyDataPtr->LenByte - remSrcLenAfterBlockCopy;
    /* The remaining partial word length after the first partial word copy. */
    uint8 remPartialWordLen = IPBASE_SOCK_BYTE_LEN_32_BIT_WORD - TgtOffset;
    /* The number of written target bytes after the block copy (excluding the remaining partial source word). */
    uint32 tgtWrittenBytesAfterBlockCopy = srcReadBytesAfterBlockCopy - remPartialWordLen;

    /* Consume the number of written bytes of the block copy. Only the target pointer is used in the next step. */
    IpBase_ConsumeCopiedDataBytewise(&tailBytewiseArrayData,                                                            /* SBSW_IPBASE_STRUCT_PASSED_VALIDITY */
                                     tgtWrittenBytesAfterBlockCopy);

    /* #40 Finish the partial word copy and add to the checksum bytewise. */
    checksumRet = IpBase_PartialChecksumCopyAddHlp(tailBytewiseArrayData.TgtDataPtr,                                    /* SBSW_IPBASE_PTR_PARTIAL_SECOND_VALIDITY */ /* VCA_IPBASE_PTR_PARTIAL_SECOND_CONSISTENCY */
                                                   &readAheadPartialSrcWord,
                                                   remPartialWordLen,
                                                   checksumRet,
                                                   &checksumIdx);

    /* Consume the copied bytes of the second partial word copy. */
    IpBase_ConsumeCopiedDataBytewise(&tailBytewiseArrayData,                                                            /* SBSW_IPBASE_STRUCT_PASSED_VALIDITY */
                                     remPartialWordLen);

    /* #50 Copy the remaining bytes and add to the checksum bytewise. */
    checksumRet = IpBase_ChecksumCopyAddBytewiseHlp(&tailBytewiseArrayData,                                             /* SBSW_IPBASE_STRUCT_CREATED_CHECKSUMIDX_LOCAL_VALIDITY */
                                                    &checksumIdx,
                                                    checksumRet);
}
  return checksumRet;
} /* IpBase_ChecksumUnalignedCopyAddHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_ChecksumUnalignedCopyAddBlockwiseHlp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumUnalignedCopyAddBlockwiseHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_P2VARAPPLDATA(uint32) LeftoverBytesPtr,
                      IPBASE_CONSTCONST(uint8) TgtOffset,
                      IPBASE_CONSTCONST(uint8) SrcOffset,
                      IPBASE_CONSTCONST(uint32) Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* Number of full 16-byte blocks in the arrays. */
  IPBASE_CONSTCONST(uint32) full16ByteBlocks = AlignedArrayCopyDataPtr->LenByte /
                                                 IPBASE_SOCK_BYTE_LEN_BLOCK_BIG_LEN_BLOCK_COPY;
  /* Resulting word length for the block copy. */
  IPBASE_CONSTCONST(uint32) wordLenBlockCopy = full16ByteBlocks * IPBASE_SOCK_BYTE_LEN_32_BIT_WORD;
  IPBASE_CONSTCONST(uint8)  offsetSum = TgtOffset + SrcOffset;
  uint32_least tgtWordIdx = 0u; /* Counter in for loop. */
  /* Modified source words as returned by the unaligned copy function. The source words are modified to build the
     target words and copy the bytes of the words across alignment boundaries. */
  IpBase_SockUnalignedCopyModSrcWordsStructType modSrcWords = {0u, 0u, 0u, 0u};
  uint32 checksumRet = Checksum;
  IpBase_SockUnalignedArrayCopyBitShiftStructType copyBitShiftCount = {0u, 0u};

  /* ----- Implementation ----------------------------------------------- */

  /* IPBASE_SOCK_CALL_ALIGNMENT_TEST_PLACEHOLDER */

  /* The number of bits of the 32-bit source word from the previous partial word copy that are already copied to the
     target. The respective bytes are the start bytes in the original source word and are now used as the end bytes
     for the new target word. Inversely the shift count is used as shift width in a shift operation to obtain the end
     bytes of the previous source word as start bytes for the new target word (by shifting them toward the start byte
     index of the word). The shift direction is either right (endianness is 'low byte first') or left (endianness is
     'high byte first'). */
  copyBitShiftCount.TowardWordStartIdx = TgtOffset * IPBASE_SOCK_BIT_LEN_1_BYTE;

  /* The number of bits of the 32-bit source word from the previous partial word copy that are not copied yet to the
     target. The respective bytes are the end bytes in the original source word and are now used as the start bytes
     for the new target word. Inversely the shift count is used as shift width in a shift operation to obtain the start
     bytes of the next source word as end bytes for the new target word (by shifting them toward the end byte index of
     the word). The shift direction is either left (endianness is 'low byte first') or right (endianness is 'high byte
     first'). */
  copyBitShiftCount.TowardWordEndIdx = IPBASE_SOCK_BIT_LEN_4_BYTES - copyBitShiftCount.TowardWordStartIdx;

 /* #10 If the offset sum is odd, swap the current checksum. */
  if ((offsetSum % IPBASE_SOCK_MODULO_2_OFFSET_EVEN_ODD) != 0u)
  {
    checksumRet = IPBASE_BYTE_SWAP32(checksumRet);
  }

  /* #20 Copy and add 16 bytes to the checksum. */
  for (tgtWordIdx = 0; tgtWordIdx < wordLenBlockCopy; tgtWordIdx += 4u)
  {
    /* #30 Copy one 16-byte block. */
    IpBase_UnalignedOneBlockCopyHlp(AlignedArrayCopyDataPtr,                                                            /* SBSW_IPBASE_STRUCT_PASSED_IDX_SRCWORD_VALIDITY */ /* VCA_IPBASE_STRUCT_IDX_VALIDITY */
                                    tgtWordIdx,
                                    LeftoverBytesPtr,
                                    copyBitShiftCount,
                                    &modSrcWords);

    /* #40 Calculate the checksum for 4 x 4 bytes. */
    checksumRet = IpBase_ChecksumAddHlp(modSrcWords.SrcWord0, checksumRet);
    checksumRet = IpBase_ChecksumAddHlp(modSrcWords.SrcWord1, checksumRet);
    checksumRet = IpBase_ChecksumAddHlp(modSrcWords.SrcWord2, checksumRet);
    checksumRet = IpBase_ChecksumAddHlp(modSrcWords.SrcWord3, checksumRet);
  }

  /* #50 If the offset sum is odd, swap the calculated checksum. */
  if ((offsetSum % IPBASE_SOCK_MODULO_2_OFFSET_EVEN_ODD) != 0u)
  {
    checksumRet = IPBASE_BYTE_SWAP32(checksumRet);
  }

  return checksumRet;
}
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_UnalignedOneBlockCopyHlp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(void) IpBase_UnalignedOneBlockCopyHlp(
                      IPBASE_CONSTP2CONSTAPPLDATA(IpBase_SockAlignedArrayCopyStructType) AlignedArrayCopyDataPtr,
                      IPBASE_CONSTCONST(uint32_least) WordIdx,
                      IPBASE_P2VARAPPLDATA(uint32) LeftoverBytesPtr,
                      IPBASE_CONSTCONST(IpBase_SockUnalignedArrayCopyBitShiftStructType) BitShiftCount,
                      IPBASE_CONSTP2VARAPPLDATA(IpBase_SockUnalignedCopyModSrcWordsStructType) ModSrcWordsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* Stores the last source word of the block. This will be partial after one block is copied. */
  IPBASE_CONSTCONST(uint32) srcWord4 = AlignedArrayCopyDataPtr->SrcDataPtr[WordIdx + 3u];

  /* ----- Implementation ----------------------------------------------- */

  /* Store the other words. */
  ModSrcWordsPtr->SrcWord1 = AlignedArrayCopyDataPtr->SrcDataPtr[WordIdx];                                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  ModSrcWordsPtr->SrcWord2 = AlignedArrayCopyDataPtr->SrcDataPtr[WordIdx + 1u];                                         /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  ModSrcWordsPtr->SrcWord3 = AlignedArrayCopyDataPtr->SrcDataPtr[WordIdx + 2u];                                         /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */

  /* #10 Copy 16-byte blocks (4 x 4 bytes) using a 32-bit accesses by correcting the byte displacement of the target
     and source. */
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)                                                                                  /* COV_IPBASE_BYTE_ORDER */
  /* 1) Create new source word 0.
        a) Copy the end bytes (as a leftover from the previous partial word copy) to the start of srcWord0.
        b) Then fill the end of srcWord0 with the start bytes of srcWord1.
     2) Create new source word 1.
        a) Move the end bytes of srcWord1 (as a leftover form the previous partial word copy) to the start of srcWord1.
        b) Then fill the end of srcWord1 with the start bytes of srcWord2.
     3) Proceed accordingly with the other source words. */
  ModSrcWordsPtr->SrcWord0 = *LeftoverBytesPtr                                              |
                             (ModSrcWordsPtr->SrcWord1 << BitShiftCount.TowardWordEndIdx);                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  ModSrcWordsPtr->SrcWord1 = (ModSrcWordsPtr->SrcWord1 >> BitShiftCount.TowardWordStartIdx) |
                             (ModSrcWordsPtr->SrcWord2 << BitShiftCount.TowardWordEndIdx);                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  ModSrcWordsPtr->SrcWord2 = (ModSrcWordsPtr->SrcWord2 >> BitShiftCount.TowardWordStartIdx) |
                             (ModSrcWordsPtr->SrcWord3 << BitShiftCount.TowardWordEndIdx);                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  ModSrcWordsPtr->SrcWord3 = (ModSrcWordsPtr->SrcWord3 >> BitShiftCount.TowardWordStartIdx) |
                             (srcWord4 << BitShiftCount.TowardWordEndIdx);                                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  /* Store the leftover bytes of the current block copy. They will be used as start bytes for the next block. */
  *LeftoverBytesPtr        = srcWord4                  >> BitShiftCount.TowardWordStartIdx;                             /* SBSW_IPBASE_VALID_API_POINTER */

#else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */                                                                           /* COV_IPBASE_BYTE_ORDER */
  ModSrcWordsPtr->SrcWord0 = *LeftoverBytesPtr                                              |
                             (ModSrcWordsPtr->SrcWord1 >> BitShiftCount.TowardWordEndIdx);                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  ModSrcWordsPtr->SrcWord1 = (ModSrcWordsPtr->SrcWord1 << BitShiftCount.TowardWordStartIdx) |
                             (ModSrcWordsPtr->SrcWord2 >> BitShiftCount.TowardWordEndIdx);                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  ModSrcWordsPtr->SrcWord2 = (ModSrcWordsPtr->SrcWord2 << BitShiftCount.TowardWordStartIdx) |
                             (ModSrcWordsPtr->SrcWord3 >> BitShiftCount.TowardWordEndIdx);                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  ModSrcWordsPtr->SrcWord3 = (ModSrcWordsPtr->SrcWord3 << BitShiftCount.TowardWordStartIdx) |
                             (srcWord4 >> BitShiftCount.TowardWordEndIdx);                                              /* SBSW_IPBASE_VALID_API_STRUCT_POINTER */
  *LeftoverBytesPtr        = srcWord4 << BitShiftCount.TowardWordStartIdx;                                              /* SBSW_IPBASE_VALID_API_POINTER */
#endif /* CPU_BYTE_ORDER */

  AlignedArrayCopyDataPtr->TgtDataPtr[WordIdx]      = ModSrcWordsPtr->SrcWord0;                                         /* SBSW_IPBASE_TGT_IDXINC_ACCESS */
  AlignedArrayCopyDataPtr->TgtDataPtr[WordIdx + 1u] = ModSrcWordsPtr->SrcWord1;                                         /* SBSW_IPBASE_TGT_IDXINC_ACCESS */
  AlignedArrayCopyDataPtr->TgtDataPtr[WordIdx + 2u] = ModSrcWordsPtr->SrcWord2;                                         /* SBSW_IPBASE_TGT_IDXINC_ACCESS */
  AlignedArrayCopyDataPtr->TgtDataPtr[WordIdx + 3u] = ModSrcWordsPtr->SrcWord3;                                         /* SBSW_IPBASE_TGT_IDXINC_ACCESS */
}
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 * IpBase_PartialChecksumCopyAddHlp
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_PartialChecksumCopyAddHlp(
                                              IPBASE_CONSTP2VARAPPLDATA(uint8) TgtDataPtr,
                                              IPBASE_CONSTP2VARAPPLDATA(uint32) TempSrcWordPtr,
                                              IPBASE_CONSTCONST(uint8) PartialWordLen,
                                              IPBASE_CONSTCONST(uint32) Checksum,
                                              IPBASE_CONSTP2VARAPPLDATA(uint8) ChecksumIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least tgtIdx = 0u; /* Counter in for loop. */
  uint8 srcWordHighByte = 0u; /* Source word high byte in the 'high byte first' representation. */
  uint32 checksumRet = Checksum;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Copy the partial word bytewise and add each byte to the checksum. */
  for (tgtIdx = 0u; tgtIdx < PartialWordLen; tgtIdx++)
  {
#if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)                                                                                 /* COV_IPBASE_BYTE_ORDER */
    /* Shift the uint32 source word right to position the high byte as low byte. */
    uint32 srcWordHighByteToLowByte = (*TempSrcWordPtr) >> IPBASE_SOCK_BIT_LEN_3_BYTES;
    /* Cast it to uint8. */
    srcWordHighByte = (uint8)srcWordHighByteToLowByte;
    /* Shift left by one byte to 'delete' the copied byte and to set the next byte as high byte. */
    (*TempSrcWordPtr) <<= IPBASE_SOCK_BIT_LEN_1_BYTE;                                                                   /* SBSW_IPBASE_VALID_API_POINTER */
#else /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
    /* Cast the uint32 source word to uint8 to get the high byte in the 'high byte first' representation of the word
       (which is the low byte in 'low byte first' endianness here). */
    srcWordHighByte = (uint8)*TempSrcWordPtr;
    /* Shift right by one byte to 'delete' the copied byte and to set the next byte as high byte (in the 'high byte
       first' representation). */
    (*TempSrcWordPtr) >>= IPBASE_SOCK_BIT_LEN_1_BYTE;                                                                   /* SBSW_IPBASE_VALID_API_POINTER */

#endif /* CPU_BYTE_ORDER */
    /* Store the high byte it in the 8-bit target array. */
    TgtDataPtr[tgtIdx] = srcWordHighByte;                                                                               /* SBSW_IPBASE_STRUCT_PTR_IDX_ACCESS */
    /* And add it to the checksum. */
    checksumRet = IpBase_ChecksumAddByteHlp(srcWordHighByte, checksumRet, ChecksumIdxPtr);                              /* SBSW_IPBASE_PASSING_API_POINTER */
  }

  return checksumRet;
} /* IpBase_PartialChecksumCopyAddHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumAddHlp
 **********************************************************************************************************************/
 /*! Internal comment removed.
 *
 *
  **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumAddHlp(IPBASE_CONSTCONST(uint32) Data,
                                                                  IPBASE_CONSTCONST(uint32) Checksum)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 checksumRet = Checksum;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Add an uint32 word to the checksum. */
  checksumRet += Data;
  if (checksumRet < Data)
  {
    /* Overflow */
    checksumRet++;
  }

  return checksumRet;
} /* IpBase_ChecksumAddHlp() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

/***********************************************************************************************************************
 *  IpBase_ChecksumIntermediateOrFinalize
 **********************************************************************************************************************/
 /*! Internal comment removed.
 *
 *
 *
 *
  **********************************************************************************************************************/
#if IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY == STD_ON
IPBASE_LOCAL_INLINE IPBASE_FUNCCODE(uint32) IpBase_ChecksumIntermediateOrFinalize(IPBASE_CONSTCONST(uint32) Checksum,
                                                                                  boolean Stop)
{
  /* ----- Local Variables ---------------------------------------------- */

  uint32 checksumRet = Checksum;
  uint16 lower16bit = 0u;
  uint16 upper16bit = 0u;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 If the checksum shall not be finalized convert the checksum back to 'low byte first' byte order on 'low byte
     first' platforms. This is for the compatibility to the existing checksum API. */
  if (Stop == FALSE)
  {
    checksumRet = IPBASE_HTON32(Checksum);
  }

  /* #20 Create the final or intermediate uint16 checksum.  */

  lower16bit = (uint16)(checksumRet);
  upper16bit = (uint16)(checksumRet >> IPBASE_SOCK_BIT_LEN_2_BYTES);

  /* Add lower and upper half word to create a 16-bit checksum. Overflow differences in the 32-bit checksums on
     big/little endian machines will be harmonized by adding the lower and upper half word. */
  checksumRet = (uint16)(lower16bit + upper16bit);
  if (checksumRet < lower16bit)
  {
    /* Overflow */
    checksumRet++;
  }

  /* If the checksum shall be finalized build the one's complement. */
  if (Stop == TRUE)
  {
    checksumRet = (uint16)~checksumRet;
  }

  return checksumRet;
} /* IpBase_ChecksumIntermediateOrFinalize() */
#endif /* IPBASE_ENABLE_ONE_STEP_CHECKSUM_COPY */

# define IPBASE_STOP_SEC_CODE
# include "IpBase_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/***********************************************************************************************************************
 *  END OF FILE: IpBase_Sock.c
 **********************************************************************************************************************/
