/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  EthTSyn_Memory_Int.h
 *         \unit  Memory
 *        \brief  EthTSyn internal header file of the Memory unit.
 *      \details  Interface and type definitions and implementation of the Memory unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_MEMORY_INT_H)
# define ETHTSYN_MEMORY_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"
# include "EthTSyn_GeneratedCfgAccess.h"

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
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint8()
 *********************************************************************************************************************/
/*! \brief       Writes a one-byte unsigned integer into a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to store the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 1]
 *  \param[in]   Value      Value to be written
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different buffers
 *  \synchronous TRUE
 *  \spec
 *    requires BufferPtr != NULL_PTR;
 *    requires $lengthOf(BufferPtr) >= (Offset + 1)
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint8(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint8  Value);

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint16()
 *********************************************************************************************************************/
/*! \brief       Writes a two-byte unsigned integer into a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to store the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 2]
 *  \param[in]   Value      Value to be written
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different buffers
 *  \synchronous TRUE
 *  \spec
 *    requires BufferPtr != NULL_PTR;
 *    requires $lengthOf(BufferPtr) >= (Offset + 2)
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint16(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint16 Value);

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint24()
 *********************************************************************************************************************/
/*! \brief       Writes a three-byte unsigned integer into a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to store the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 3]
 *  \param[in]   Value      Value to be written
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different buffers
 *  \synchronous TRUE
 *  \spec
 *    requires BufferPtr != NULL_PTR;
 *    requires $lengthOf(BufferPtr) >= (Offset + 3)
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint24(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint32 Value);

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint32()
 *********************************************************************************************************************/
/*! \brief       Writes a four-byte unsigned integer into a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to store the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 4]
 *  \param[in]   Value      Value to be written
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different buffers
 *  \synchronous TRUE
 *  \spec
 *    requires BufferPtr != NULL_PTR;
 *    requires $lengthOf(BufferPtr) >= (Offset + 4)
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint32(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint32 Value);

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint48()
 *********************************************************************************************************************/
/*! \brief       Writes a six-byte unsigned integer into a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to store the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 6]
 *  \param[in]   Value      Value to be written
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different buffers
 *  \synchronous TRUE
 *  \spec
 *    requires BufferPtr != NULL_PTR;
 *    requires $lengthOf(BufferPtr) >= (Offset + 6)
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint48(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint64 Value);

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint64()
 *********************************************************************************************************************/
/*! \brief       Writes an eight-byte unsigned integer into a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to store the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 8]
 *  \param[in]   Value      Value to be written
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different buffers
 *  \synchronous TRUE
 *  \spec
 *    requires BufferPtr != NULL_PTR;
 *    requires $lengthOf(BufferPtr) >= (Offset + 8)
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint64(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint64 Value);

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint8()
 *********************************************************************************************************************/
/*! \brief       Reads a one-byte unsigned integer from a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to read the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 1]
 *  \return      8bit Data read from buffer
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Mem_GetUint8(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset);

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint16()
 *********************************************************************************************************************/
/*! \brief       Reads a two-byte unsigned integer from a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to read the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 2]
 *  \return      16bit Data read from buffer
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_Mem_GetUint16(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset);

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint24()
 *********************************************************************************************************************/
/*! \brief       Reads a three-byte unsigned integer from a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to read the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 3]
 *  \return      24bit Data read from buffer
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint32, ETHTSYN_CODE) EthTSyn_Mem_GetUint24(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset);

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint32()
 *********************************************************************************************************************/
/*! \brief       Reads a four-byte unsigned integer from a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to read the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 4]
 *  \return      32bit Data read from buffer
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint32, ETHTSYN_CODE) EthTSyn_Mem_GetUint32(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset);

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint48()
 *********************************************************************************************************************/
/*! \brief       Reads a six-byte unsigned integer from a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to read the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 6]
 *  \return      48bit Data read from buffer
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint64, ETHTSYN_CODE) EthTSyn_Mem_GetUint48(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset);

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint64()
 *********************************************************************************************************************/
/*! \brief       Reads an eight-byte unsigned integer from a buffer.
 *  \details     Byte-wise access for array data in network byte order.
 *  \param[in]   BufferPtr  Buffer to read the value
 *  \param[in]   Offset     Byte offset within buffer
 *                          [range: 0 <= Offset <= len(BufferPtr) - 8]
 *  \return      64bit Data read from buffer
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint64, ETHTSYN_CODE) EthTSyn_Mem_GetUint64(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_MEMORY)
/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint8(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint8 Value)
{
  BufferPtr[Offset] = Value;
} /* EthTSyn_Mem_PutUint8() */

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint16(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                    uint16 Offset,
                    uint16 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 8u);
  BufferPtr[Offset + 1u] = (uint8)(Value);
} /* EthTSyn_Mem_PutUint16 */

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint24()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint24(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint32 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 16u);
  BufferPtr[Offset + 1u] = (uint8)(Value >> 8u);
  BufferPtr[Offset + 2u] = (uint8)(Value);
} /* EthTSyn_Mem_PutUint16() */

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint32(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint32 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 24u);
  BufferPtr[Offset + 1u] = (uint8)(Value >> 16u);
  BufferPtr[Offset + 2u] = (uint8)(Value >> 8u);
  BufferPtr[Offset + 3u] = (uint8)(Value);
} /* EthTSyn_Mem_PutUint32() */

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint48()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint48(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint64 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 40u);
  BufferPtr[Offset + 1u] = (uint8)(Value >> 32u);
  BufferPtr[Offset + 2u] = (uint8)(Value >> 24u);
  BufferPtr[Offset + 3u] = (uint8)(Value >> 16u);
  BufferPtr[Offset + 4u] = (uint8)(Value >> 8u);
  BufferPtr[Offset + 5u] = (uint8)(Value);
} /* EthTSyn_Mem_PutUint48() */

/**********************************************************************************************************************
 * EthTSyn_Mem_PutUint64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Mem_PutUint64(
  ETHTSYN_CONSTP2VAR(uint8) BufferPtr,
                     uint16 Offset,
                     uint64 Value)
{
  BufferPtr[Offset]      = (uint8)(Value >> 56u);
  BufferPtr[Offset + 1u] = (uint8)(Value >> 48u);
  BufferPtr[Offset + 2u] = (uint8)(Value >> 40u);
  BufferPtr[Offset + 3u] = (uint8)(Value >> 32u);
  BufferPtr[Offset + 4u] = (uint8)(Value >> 24u);
  BufferPtr[Offset + 5u] = (uint8)(Value >> 16u);
  BufferPtr[Offset + 6u] = (uint8)(Value >> 8u);
  BufferPtr[Offset + 7u] = (uint8)(Value);
} /* EthTSyn_Mem_PutUint64() */

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Mem_GetUint8(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset)
{
  return BufferPtr[Offset];
} /* EthTSyn_Mem_GetUint8() */

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_Mem_GetUint16(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset)
{
  return (uint16)(
    ((uint16)(BufferPtr[Offset]) << 8u) |
    ((uint16)(BufferPtr[Offset + 1u]))
    );
} /* EthTSyn_Mem_GetUint16() */

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint24()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(uint32, ETHTSYN_CODE) EthTSyn_Mem_GetUint24(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset)
{
  return (uint32)(
    ((uint32)(BufferPtr[Offset]) << 16u)     |
    ((uint32)(BufferPtr[Offset + 1u]) << 8u) |
    ((uint32)(BufferPtr[Offset + 2u]))
    );
} /* EthTSyn_Mem_GetUint24() */

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(uint32, ETHTSYN_CODE) EthTSyn_Mem_GetUint32(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset)
{
  return (uint32)(
    ((uint32)(BufferPtr[Offset]) << 24u)      |
    ((uint32)(BufferPtr[Offset + 1u]) << 16u) |
    ((uint32)(BufferPtr[Offset + 2u]) << 8u)  |
    ((uint32)(BufferPtr[Offset + 3u]))
    );
} /* EthTSyn_Mem_GetUint32() */

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint48()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(uint64, ETHTSYN_CODE) EthTSyn_Mem_GetUint48(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset)
{
  return (uint64)(
    ((uint64)(BufferPtr[Offset]) << 40u)      |
    ((uint64)(BufferPtr[Offset + 1u]) << 32u) |
    ((uint64)(BufferPtr[Offset + 2u]) << 24u) |
    ((uint64)(BufferPtr[Offset + 3u]) << 16u) |
    ((uint64)(BufferPtr[Offset + 4u]) << 8u)  |
    ((uint64)(BufferPtr[Offset + 5u])));
} /* EthTSyn_Mem_GetUint48() */

/**********************************************************************************************************************
 * EthTSyn_Mem_GetUint64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 3219 1 */ /* MD_EthTSyn_UnusedStaticFunc_MemUnit */
ETHTSYN_LOCAL_INLINE FUNC(uint64, ETHTSYN_CODE) EthTSyn_Mem_GetUint64(
  ETHTSYN_CONSTP2CONST(uint8) BufferPtr,
                       uint16 Offset)
{
  return (uint64)(
    ((uint64)(BufferPtr[Offset]) << 56u)      |
    ((uint64)(BufferPtr[Offset + 1u]) << 48u) |
    ((uint64)(BufferPtr[Offset + 2u]) << 40u) |
    ((uint64)(BufferPtr[Offset + 3u]) << 32u) |
    ((uint64)(BufferPtr[Offset + 4u]) << 24u) |
    ((uint64)(BufferPtr[Offset + 5u]) << 16u) |
    ((uint64)(BufferPtr[Offset + 6u]) << 8u)  |
    ((uint64)(BufferPtr[Offset + 7u])));
} /* EthTSyn_Mem_GetUint64() */
# endif /* !defined(ETHTSYN_NOUNIT_MEMORY) */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_MEMORY_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Memory_Int.h
 *********************************************************************************************************************/
