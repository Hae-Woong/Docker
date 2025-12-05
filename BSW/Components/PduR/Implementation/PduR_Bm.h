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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  PduR_Bm.h
 *         \unit  Bm
 *        \brief  Pdu Router Buffer Manager header file
 *      \details  This is the implementation of the MICROSAR Pdu Router module.
 *                The basic software module is based on the AUTOSAR PduR specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined (PDUR_BM_H)
# define PDUR_BM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* \trace SPEC-2020230, SPEC-38402 */

# include "PduR_Types.h"
# include "PduR_Cfg.h"
# include "PduR_Lcfg.h"

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
# if(PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/*! Value based type definition for PduR_BmTxBufferArrayRamReadIdxOfBmTxBufferRamType */
typedef PduR_BmTxBufferArrayRamWriteIdxOfBmTxBufferRamType PduR_BmTxBufferArrayRamReadIdxOfBmTxBufferRamType;
# endif

# define PDUR_START_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

# if(PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_Bm_Init
**********************************************************************************************************************/
/*! \brief       This function initializes the BufferManager.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *  \endspec
 ********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_Init(PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_PutData
 *********************************************************************************************************************/
/*! \brief       This function copies data to the buffer.
 *  \details     -
 *  \param[in]   bmTxBufferRomIdx         Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]   pduLength                Length of the Pdu in sduDataPtr.
 *  \param[in]   sduDataPtr               Pointer to the data to be copied to the buffer.
 *  \param[in]   memIdx                   memory section index
 *  \pre         -
 *  \return      BUFREQ_OK                Copying Data was successful.
 *               BUFREQ_E_NOT_OK          Copying Data was not successful.
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Bm_PutData(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetWritePtrToBuffer
 *********************************************************************************************************************/
/*! \brief       This function returns a pointer to the internal buffer which can be used to write to the buffer.
 *  \details     -
 *  \param[in]   bmTxBufferRomIdx         Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]   memIdx                   memory section index
 *  \pre         -
 *  \return      Pointer to the internal buffer and its length.
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduInfoType, PDUR_CODE) PduR_Bm_GetWritePtrToBuffer(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetData
 *********************************************************************************************************************/
/*!  \brief      This function copies data from the internal buffer to the provided pointer.
 *   \details    -
 *   \param[in]  bmTxBufferInstanceRomIdx Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *   \param[in]  requestedPduLength       Length of the array referred by 'sduDataPtr'. This amount will be copied to the buffer.
 *   \param[out] sduDataPtr               The data from the internal buffer is copied to this pointer.
 *   \param[in]  memIdx                   memory section index
 *   \pre        -
 *   \return     BUFREQ_OK                Copying Data was successful.
 *               BUFREQ_E_BUSY            Copying Data was not successful. There is not enough data in the buffer.
 *               BUFREQ_E_NOT_OK          Copying Data was not successful. Either the sduDataPtr is not valid or the bufferInstance object is not enabled.
 *   \context    TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Bm_GetData(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduLengthType requestedPduLength, CONST(SduDataPtrType, AUTOMATIC) sduDataPtr, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_ResetTxBuffer
 *********************************************************************************************************************/
/*! \brief       Resets the administration variables of the Buffer (BmTxBuffer) and the associated BmTxBufferInstances.
 *  \details     The actual content of the Buffer is not cleared.
 *  \param[in]   bmTxBufferRomIdx  Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]   memIdx            memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_ResetTxBuffer(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_ResetTxBufferInstance
 *********************************************************************************************************************/
/*! \brief       Resets the administration variables of the BufferInstance (BmTxBufferInstanceRom).
 *  \details     The actual content of the Buffer is not cleared.
 *  \param[in]   bmTxBufferInstanceRomIdx  Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \param[in]   memIdx                    memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_ResetTxBufferInstance(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_EnableBmTxBufferInstance
 *********************************************************************************************************************/
/*! \brief       This function enables the BmTxBuffer read instance (bmTxBufferInstanceRomIdx). This can then be used to read from the TxBuffer.
 *  \details     -
 *  \param[in]   bmTxBufferRomIdx          Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]   bmTxBufferInstanceRomIdx  Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \param[in]   memIdx                    memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *    requires bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_EnableBmTxBufferInstance(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetBmTxBufferInstanceByBmTxBufferChecked
 *********************************************************************************************************************/
/*! \brief       This function returns the next unused BmTxBufferInstance object which can be used for reading from the buffer.
 *  \details     -
 *  \param[in]   bmTxBufferRomIdx                 Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[out]  bmTxBufferInstanceRomIdx2Return  Index to the BmTxBufferInstanceRom element which can be used to read the buffer.
 *  \param[in]   memIdx                           memory section index
 *  \return      E_OK      A free BmTxBufferInstance object was found and returned.
 *               E_NOT_OK  No free BmTxBufferInstance object was found.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *    requires bmTxBufferInstanceRomIdx2Return != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Bm_GetBmTxBufferInstanceByBmTxBufferChecked(PduR_BmTxBufferRomIterType bmTxBufferRomIdx,
                                                                                 P2VAR(PduR_BmTxBufferInstanceRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferInstanceRomIdx2Return,
                                                                                 PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetTotalDataSizeInstance
 *********************************************************************************************************************/
/*! \brief       This function returns the total data size of the buffer for the provided Buffer instance object.
 *  \details     The total data size is the total amount of stored bytes in the buffer.
 *               This total data size is from the readers point of view. This value can differ per reader depending on how much data was already read.
 *  \param[in]   bmTxBufferInstanceRomIdx  Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \param[in]   memIdx                    memory section index
 *  \return      The total data size of this buffer instance.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetTotalDataSizeInstance(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetTotalBufferSize
 *********************************************************************************************************************/
/*! \brief       This function returns the total free buffer size of the buffer.
 *  \details     The total free buffer size is the total amount of free bytes in the buffer.
 *               This total free buffer size is from the writers point of view. The slowest reader determines this value.
 *  \param[in]   bmTxBufferRomIdx  Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]   memIdx            memory section index
 *  \return      The total free buffer size.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetTotalBufferSize(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetReadPtrToBuffer
 *********************************************************************************************************************/
/*! \brief       This function returns a pointer to the internal buffer which can be used to read from it.
 *  \details     This function does NOT actually read and increment the read Idx of this buffer instance object.
 *               To tell Bm unit how much data was actually read from the internal buffer, call PduR_Bm_IncrementReadIdx afterwards.
 *  \param[in]   bmTxBufferInstanceRomIdx  Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \param[in]   memIdx                    memory section index
 *  \return      Pointer to the internal buffer and its size.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduInfoType, PDUR_CODE) PduR_Bm_GetReadPtrToBuffer(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetActualPduLengthToBeRouted
 *********************************************************************************************************************/
/*! \brief       The actual entire Pdu length which shall be routed.
 *  \details     -
 *  \param[in]   bmTxBufferInstanceRomIdx  Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \param[in]   memIdx                    memory section index
 *  \return      Entire Length of Pdu to be routed. This could be larger than the buffer and was initally stored when the buffer was allocated.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduLengthType, PDUR_CODE) PduR_Bm_GetActualPduLengthToBeRouted(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetSizeOfTxBuffer
 *********************************************************************************************************************/
/*! \brief       Returns the configured size of the TxBuffer
 *  \details     -
 *  \param[in]   bmTxBufferRomIdx   Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]   memIdx             memory section index
 *  \return      The configured size of the PduRTxBuffer.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetSizeOfTxBuffer(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_IsTxBufferAllocated
 *********************************************************************************************************************/
/*! \brief       Check if buffer is allocated and used by any queue.
 *  \details     This flag will be reset via PduR_Bm_ResetTxBuffer.
 *  \param[in]   bmTxBufferRomIdx   Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]   memIdx             memory section index
 *  \return      TRUE if buffer is allocated, otherwise FALSE
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_Bm_IsTxBufferAllocated(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_AllocateTxBuffer
 *********************************************************************************************************************/
/*! \brief       Allocates the TxBuffer. This TxBuffer can then not be used by any other queue.
 *  \details     -
 *  \param[in]   bmTxBufferRomIdx   Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]   pduLength          Pdu length to be stored. This value can be larger than the buffer size and can be read with PduR_Bm_GetActualPduLengthToBeRouted.
 *  \param[in]   memIdx             memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_AllocateTxBuffer(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduLengthType pduLength, PduR_MemIdxType memIdx);
# endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetBmTxBufferArrayRamReadIdxOfBmTxBufferRam
 **********************************************************************************************************************/
/*! \brief      Get BmTxBufferArrayRamRead Idx Of BmTxBufferRam
 *  \details    -
 *  \param[in]  bmTxBufferRomIdx Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]  memIdx memory section index
 *  \param[in]  writeIdx Write index of the buffer.
 *  \param[in]  startIdxOfBuffer Start index of the buffer.
 *  \param[in]  endIdxOfBuffer End index of the buffer. This has to be the CSL EndIdx of a 1:N indirection. Thus this is already the first out-of-bounds index.
 *  \return     Buffer read index
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *    requires writeIdx < PduR_GetSizeOfBmTxBufferArrayRam(memIdx);
 *    requires startIdxOfBuffer < PduR_GetSizeOfBmTxBufferArrayRam(memIdx);
 *    requires endIdxOfBuffer < PduR_GetSizeOfBmTxBufferArrayRam(memIdx) + 1;
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetBmTxBufferArrayRamReadIdxOfBmTxBufferRam(PduR_BmTxBufferRomIterType bmTxBufferRomIdx,
                                                                                                     PduR_MemIdxType memIdx,
                                                                                                     PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                                     PduR_BmTxBufferArrayRamIterType startIdxOfBuffer,
                                                                                                     PduR_BmTxBufferArrayRamIterType endIdxOfBuffer);
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_IncrementWriteIdx
 **********************************************************************************************************************/
/*! \brief      Increment the buffer writeIdx by pduLength.
 *  \details    This API shall only be used with PduR_Bm_GetWritePtrToBuffer API.
                After data  was written to the pointer to the internal buffer (returned by PduR_Bm_GetWritePtrToBuffer), call this API to let the Bm unit know how much data was written.
 *  \param[in]  bmTxBufferRomIdx Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]  memIdx memory section index
 *  \param[in]  pduLength Amount of data which was written to the buffer. This value has to be smaller or equal to the current linearBufferSize.
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_IncrementWriteIdx(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx, PduLengthType pduLength);
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_IncrementReadIdx
 **********************************************************************************************************************/
/*! \brief      Increment the buffer readIdx by pduLength.
 *  \details    This API shall only be used with PduR_Bm_GetReadPtrToBuffer API.
                After data was read from the pointer to the internal buffer (returned by PduR_Bm_GetReadPtrToBuffer), call this API to let the Bm unit know how much data was read.
 *  \param[in]  bmTxBufferInstanceRomIdx Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \param[in]  memIdx memory section index
 *  \param[in]  pduLength Amount of data which was read from the buffer. This value has to be smaller or equal to the current linearDataSize.
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_IncrementReadIdx(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx, PduLengthType pduLength);
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe
 **********************************************************************************************************************/
/*! \brief      Get BmTxBufferArrayRamWrite Idx Of BmTxBufferRam with safe check
 *  \details    -
 *  \param[in]  bmTxBufferRomIdx Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]  memIdx  memory section index
 *  \return     Buffer write index
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked
 *********************************************************************************************************************/
/*! \brief       This function returns the currently read BmTxBufferArrayRamReadIdx
 *  \details     -
 *  \param[in]   bmTxBufferInstanceRomIdx   Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \param[out]  bmTxBufferArrayRamReadIdx  The BmTxBufferArrayRam read index
 *  \param[in]   memIdx                     memory section index
 *  \return      E_OK                       the read BmTxBufferArrayRamReadIdx is valid and can be used
 *               E_NOT_OK                   the read BmTxBufferArrayRamReadIdx is invalid and can not be used
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx);
 *    requires bmTxBufferArrayRamReadIdx != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx,
                                                                                                      PduR_BmTxBufferArrayRamIterType* bmTxBufferArrayRamReadIdx,
                                                                                                      PduR_MemIdxType memIdx);
#endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_BM_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_Bm.h
 *********************************************************************************************************************/
