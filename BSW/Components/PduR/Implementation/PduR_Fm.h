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
 *         \file  PduR_Fm.h
 *         \unit  Fm
 *        \brief  Pdu Router FIFO Manager header file
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
# if !defined (PDUR_FM_H)
# define PDUR_FM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
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
# define PDUR_START_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

# if(PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_Init
 *********************************************************************************************************************/
/*! \brief       This function initializes the FifoManager Ram Variables
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Fm_Init(PduR_MemIdxType memIdx);
# endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_Peek
 *********************************************************************************************************************/
/*!  \brief       This function returns the next occupied element in the FIFO queue.
 *   \details     This function can skip gaps in the FIFO. This can happen if some occupied element was released inbetween.
 *   \param[in]   fmFifoRamIdx        Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *   \param[out]  fmFifoElementRamIdx Valid FmFifoElementRam table index. Internal handle of one element in the PduRSharedBufferQueue.
 *                                    The function will return the next element in the queue via this parameter. Only valid on return value E_OK!
 *   \param[in]   memIdx              memory section index
 *   \return      E_OK      There is another element in the queue which was returned via fmFifoElementRamIdx.
 *                E_NOT_OK  The queue is empty. No element was returned.
 *   \pre         -
 *   \context     TASK|ISR1|ISR2
 *   \reentrant   TRUE
 *   \synchronous TRUE
 *   \spec
 *     requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *     requires fmFifoRamIdx < PduR_GetSizeOfFmFifoRam(memIdx);
 *     requires fmFifoElementRamIdx != NULL_PTR;
 *   \endspec
***********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_Peek(PduR_FmFifoRamIterType fmFifoRamIdx, P2VAR(PduR_FmFifoElementRamIterType, AUTOMATIC, PDUR_APPL_DATA) fmFifoElementRamIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetBmTxBufferInstanceByFmFifoElement
 *********************************************************************************************************************/
/*! \brief       This function returns a free bmTxBufferInstance object if available for this fifo element.
 *  \details     -
 *  \param[in]   fmFifoElementRamIdx       Valid FmFifoElementRam table index. Internal handle of one element in the PduRSharedBufferQueue.
 *  \param[in]   memIdx                    memory section index
 *  \param[out]  bmTxBufferInstanceRomIdx  Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *                                         The function will return the found txBufferInstance object via this parameter. Only valid on return value E_OK!
 *  \return      E_OK       A bmTxBufferInstance object could be found and assigned to the destination.
 *               E_NOT_OK  No bmTxBufferInstance object could be found and assigned to the destination.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoElementRamIdx < PduR_GetSizeOfFmFifoElementRam(memIdx);
 *    requires bmTxBufferInstanceRomIdx != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetBmTxBufferInstanceByFmFifoElement(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, PduR_MemIdxType memIdx, P2VAR(PduR_BmTxBufferInstanceRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferInstanceRomIdx);
# endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe
 *********************************************************************************************************************/
/*! \brief       This function gets the read Idx of the fifo with optional error correction.
 *  \details     -
 *  \param[in]   fmFifoRamIdx        Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx              memory section index
 *  \return                          Valid FmFifoElementRam table index. Internal handle of one element in the PduRSharedBufferQueue. This will be the oldest/first element in the FIFO.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRamIdx < PduR_GetSizeOfFmFifoRam(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(PduR_FmFifoElementRamReadIdxOfFmFifoRamType, PDUR_CODE) PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_PutFifo
 *********************************************************************************************************************/
/*! \brief       This function writes the Pdu to the FIFO queue.
 *  \details     The Pdus will be written in first-in-first-out order.
 *  \param[in]   fmFifoRomIdx         Valid FmFifoRom table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx               memory section index
 *  \param[in]   rmDestRomIdx         Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   pduLength            Length of the Pdu.
 *  \param[in]   sduDataPtr           Pointer to the data of the Pdu.
 *  \return      E_OK                 Write to the queue was successful.
 *               E_NOT_OK             Write to the queue was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_PutFifo(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx, PduR_RmDestRomIterType rmDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr);
#endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetNextElement
 *********************************************************************************************************************/
/*! \brief       This function gets the next Pdu from the FIFO queue.
 *  \details     The Pdus will be read in first-in-first-out order. This will not remove the Pdu from the queue.
 *  \param[in]   fmFifoRomIdx        Valid FmFifoRom table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx              memory section index
 *  \param[out]  rmDestRomIdx        read RmDestRom table index. Internal handle of a PduRDestPdu. This PduRDestPdu was stored in the queue.
 *  \param[out]  info                read data from the queue. A pointer to the data and its length will be written to this value.
 *  \return      E_OK                next Pdu was read successfully.
 *               E_NOT_OK            no Pdu could be found. The queue is empty.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *    requires rmDestRomIdx != NULL_PTR;
 *    requires info != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetNextElement(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);
#endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_FlushFiFo
 *********************************************************************************************************************/
/*! \brief       Flushes and resets the queue.
 *  \details     All Pdus will be removed from the queue. It will be empty afterwards.
 *  \param[in]   fmFifoRomIdx   Valid FmFifoRom table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx         memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Fm_FlushFiFo(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx);
# endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetFillLevelSafe
 *********************************************************************************************************************/
/*! \brief       Gets the fill level with error detection.
 *  \details     -
 *  \param[in]   fmFifoRamIdx    Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx          memory section index
 *  \return                      the fill level
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRamIdx < PduR_GetSizeOfFmFifoRam(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint16, PDUR_CODE) PduR_Fm_GetFillLevelSafe(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_RemoveOldestElement
 *********************************************************************************************************************/
/*! \brief       Removes the oldest element in the queue.
 *  \details     -
 *  \param[in]   fmFifoRomIdx    Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx          memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Fm_RemoveOldestElement(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_ReleaseFifoElement
 *********************************************************************************************************************/
/*! \brief       This function resets the Fifo Element object. It will also reset an assigned BmTxBuffer if available.
 *  \details     -
 *  \param[in]   fmFifoElementRamIdx    Valid FmFifoElementRam table index. Internal handle of one element in the PduRSharedBufferQueue.
 *  \param[in]   memIdx                 memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoElementRamIdx < PduR_GetSizeOfFmFifoElementRam(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Fm_ReleaseFifoElement(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetCurrentlyUsedBmTxBufferRomIdxByFmFifoElementIdx
 *********************************************************************************************************************/
/*! \brief       Return the currently used BmTxBufferRomIdx of this fmFifoElement.
 *  \details     -
 *  \param[in]   fmFifoElementRamIdx Valid FmFifoElementRam table index. Internal handle of one element in the PduRSharedBufferQueue.
 *  \param[out]  bmTxBufferRomIdx    Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *                                   The currently used buffer will be returned via this parameter. Only valid if return value is E_OK!
 *  \param[in]   memIdx              memory section index
 *  \return      E_OK                 A buffer is assigned to this fifo element. The read bmTxBufferRomIdx is valid and can be used.
 *               E_NOT_OK            No buffer is assigned to this fifo element. The read bmTxBufferRomIdx is invalid and can not be used.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoElementRamIdx < PduR_GetSizeOfFmFifoElementRam(memIdx);
 *    requires bmTxBufferRomIdx != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetCurrentlyUsedBmTxBufferRomIdxByFmFifoElementIdx(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, P2VAR(PduR_BmTxBufferRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferRomIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetRmDestRomIdxOfFmFifoElementRamSafe
 *********************************************************************************************************************/
/*! \brief       Gets the rmDestRomIdx of fmFifoElement with error detection.
 *  \details     -
 *  \param[in]   fmFifoElementRamIdx  Valid FmFifoElementRam table index. Internal handle of one element in the PduRSharedBufferQueue.
 *  \param[out]  rmDestRomIdx         The read rmDestRomIdx. Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   memIdx               memory section index
 *  \return      E_OK                 The read was successful. A valid value was returned via rmDestRomIdx.
 *               E_NOT_OK             The read was not successful. No value was returned via rmDestRomIdx.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoElementRamIdx < PduR_GetSizeOfFmFifoElementRam(memIdx);
 *    requires rmDestRomIdx != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetRmDestRomIdxOfFmFifoElementRamSafe(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_AllocateFifoElement
 *********************************************************************************************************************/
/*! \brief       Allocate a new FIFO Element in the queue. This function will also allocate a suitable TxBuffer.
 *  \details     -
 *  \param[in]   fmFifoRomIdx           Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx                 memory section index
 *  \param[in]   rmDestRomIdx           Valid RmDestRom table index. Internal handle of a PduRDestPdu. This Pdu is stored in the queue.
 *  \param[in]   pduLength              Length of the Pdu for buffer allocation. Handed through to PduR_Bm_AllocateTxBuffer.
 *  \param[in]   optimalBufferSize      Optimal buffer size for buffer allocation. Handed through to PduR_Bm_AllocateTxBuffer.
 *  \param[in]   minimumBufferSize      Minimum buffer size for buffer allocation.  Handed through to PduR_Bm_AllocateTxBuffer.
 *  \param[out]  fmFifoElementRamIdx    Valid FmFifoElementRam table index. Internal handle of one element in the PduRSharedBufferQueue.
 *                                      The allocated fmFifoElement will be returned via this parameter. Only valid if return value is E_OK!
 *  \param[out]  bmTxBufferRomIdx       Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *                                      The allocated PduRTxBuffer will be returned via this parameter. Only valid if return value is E_OK!
 *  \return      E_OK                   A free FIFO Element could be allocated. A valid value was returned via fmFifoElementRamIdx and bmTxBufferRomIdx.
 *               E_NOT_OK               No FIFO Element could be allocated. No value was returned via fmFifoElementRamIdx and bmTxBufferRomIdx.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *    requires fmFifoElementRamIdx != NULL_PTR;
 *    requires bmTxBufferRomIdx != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_AllocateFifoElement(PduR_FmFifoRomIterType fmFifoRomIdx,
                                                            PduR_MemIdxType memIdx,
                                                            PduR_RmDestRomIterType rmDestRomIdx,
                                                            PduLengthType pduLength,
                                                            PduLengthType optimalBufferSize,
                                                            PduLengthType minimumBufferSize,
                                                            PduR_FmFifoElementRamIterType* fmFifoElementRamIdx,
                                                            PduR_BmTxBufferRomIdxOfBmTxBufferIndRomType* bmTxBufferRomIdx);
# endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
 /**********************************************************************************************************************
 * PduR_Fm_GetRmSrcRomIdxOfOldestFmFifoElementSafe
 *********************************************************************************************************************/
/*! \brief       Gets the rmSrc index of the oldest fifo element with optional error detection.
 *  \details     -
 *  \param[in]   fmFifoRomIdx Valid FmFifoRom table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[out]  rmSrcRomIdx  rmSrc index
 *  \param[in]   memIdx       memory section index
 *  \return      E_OK         The read rmSrcRomIdx is valid and can be used.
 *               E_NOT_OK     The read rmSrcRomIdx is invalid and can NOT be used.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *    requires rmSrcRomIdx != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetRmSrcRomIdxOfOldestFmFifoElementSafe(PduR_FmFifoRomIterType fmFifoRomIdx, P2VAR(PduR_RmSrcRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmSrcRomIdx, PduR_MemIdxType memIdx);
# endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_FM_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_Fm.h
 *********************************************************************************************************************/
