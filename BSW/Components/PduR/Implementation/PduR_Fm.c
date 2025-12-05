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
 *         \file  PduR_Fm.c
 *         \unit  Fm
 *        \brief  Pdu Router FIFO Manager source file
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

/**********************************************************************************************************************
 * LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_FM_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Bm.h"
#include "PduR_Fm.h"
#include "PduR_Reporting.h"
#include "PduR_Lock.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_IsFifoElementAllocated
 *********************************************************************************************************************/
/*! \brief       Helper function to check if the queue element is in use.
 *  \details     -
 *  \param[in]   fmFifoElementRamIdx    Valid FmFifoElementRam table index. Internal handle of one element in the PduRSharedBufferQueue.
 *  \param[in]   memIdx                 memory section index
 *  \return      E_OK                   The fifo element is used.
 *               E_NOT_OK               The fifo element is not not used.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_IsFifoElementAllocated(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_IncrementReadIndex
 *********************************************************************************************************************/
/*! \brief       This function increments the read index of the FIFO.
 *  \details     It also handles the wrap-around.
 *  \param[in]   fmFifoRamIdx        Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx              memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Fm_IncrementReadIndex(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_IncrementWriteIndex
 *********************************************************************************************************************/
/*! \brief       This function increments the write index of the FIFO.
 *  \details     It also handles the wrap-around.
 *  \param[in]   fmFifoRamIdx        Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx              memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Fm_IncrementWriteIndex(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx);
# endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetFmFifoElementRamWriteIdxSafe
 *********************************************************************************************************************/
/*! \brief       Gets the current write idx for the FIFO element.
 *  \details     -
 *  \param[in]   fmFifoRamIdx    Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx          memory section index
 *  \return                      the write index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_FmFifoElementRamWriteIdxOfFmFifoRamType, PDUR_CODE) PduR_Fm_GetFmFifoElementRamWriteIdxSafe(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_ValidateReadRmDestRomIdxInIFContext
 *********************************************************************************************************************/
/*! \brief       Checks if rmDestRomIdx uses the handed over queue.
 *  \details     -
 *  \param[in]   rmDestRomIdx            Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   supposedFmFifoRomIdx    Valid FmFifoRam table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   supposedMemIdx          memory section index
 *  \return      TRUE                    rmDestRomIdx uses the queue referred by supposedFmFifoRomIdx and supposedMemIdx.
 *               FALSE                   rmDestRomIdx does NOT use the queue referred by supposedFmFifoRomIdx and supposedMemIdx OR is not even IF queued.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_Fm_ValidateReadRmDestRomIdxInIFContext(PduR_RmDestRomIterType rmDestRomIdx, PduR_FmFifoRomIterType supposedFmFifoRomIdx, PduR_MemIdxType supposedMemIdx);
#endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_AllocateTxBuffer
 *********************************************************************************************************************/
/*! \brief       Search a suitable buffer from the list of assigned TxBuffers.
 *  \details     -
 *  \param[in]   fmFifoRomIdx              Valid FmFifoRom table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   pduLength                 Pdu length. This value will be stored in the TxBuffer.
 *  \param[in]   optimalBufferSize         The optimal buffer size (which fits the whole Pdu)
 *  \param[in]   minimumBufferSize         The minimum buffer size which still can be used for Tp routings (which only fits the TpThreshold + Metadata)
 *  \param[out]  allocatedTxBufferIdxPtr   Id of the allocated TxBuffer
 *  \param[in]   memIdx                    memory section index
 *  \return      BUFREQ_OK                 Buffer request was successful.
 *               BUFREQ_E_NOT_OK           Currently no buffer is available.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Fm_AllocateTxBuffer(PduR_FmFifoRomIterType fmFifoRomIdx,
                                                                              PduLengthType pduLength,
                                                                              PduLengthType optimalBufferSize,
                                                                              PduLengthType minimumBufferSize,
                                                                              P2VAR(PduR_BmTxBufferRomIdxOfBmTxBufferIndRomType, AUTOMATIC, PDUR_APPL_DATA) allocatedTxBufferIdxPtr,
                                                                              PduR_MemIdxType memIdx);
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if(PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Fm_Init(PduR_MemIdxType memIdx)
{
  {
    PduR_FmFifoRamIterType fmFifoRamIdx;
    for(fmFifoRamIdx = 0u; fmFifoRamIdx < PduR_GetSizeOfFmFifoRam(memIdx); fmFifoRamIdx++)  /* FETA_PDUR_01 */
    {
      PduR_SetFmFifoElementRamReadIdxOfFmFifoRam(fmFifoRamIdx, PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRamIdx, memIdx), memIdx);
      PduR_SetFmFifoElementRamWriteIdxOfFmFifoRam(fmFifoRamIdx, PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRamIdx, memIdx), memIdx);
      PduR_SetFillLevelOfFmFifoRam(fmFifoRamIdx, 0u, memIdx);
    }
  }
  {
    PduR_FmFifoElementRamIterType fmFifoElementRamIdx;
    for(fmFifoElementRamIdx = 0u; fmFifoElementRamIdx < PduR_GetSizeOfFmFifoElementRam(memIdx); fmFifoElementRamIdx++)  /* FETA_PDUR_01 */
    {
      PduR_SetBmTxBufferRomIdxOfFmFifoElementRam(fmFifoElementRamIdx, PDUR_NO_BMTXBUFFERROMIDXOFFMFIFOELEMENTRAM, memIdx);
      PduR_SetRmDestRomIdxOfFmFifoElementRam(fmFifoElementRamIdx, PDUR_NO_RMDESTROMIDXOFFMFIFOELEMENTRAM, memIdx);
    }
  }
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_Peek
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
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_Peek(PduR_FmFifoRamIterType fmFifoRamIdx, P2VAR(PduR_FmFifoElementRamIterType, AUTOMATIC, PDUR_APPL_DATA) fmFifoElementRamIdx, PduR_MemIdxType memIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* FifoElements in the queue? */
  while(PduR_Fm_GetFillLevelSafe(fmFifoRamIdx, memIdx) > 0u)  /* FETA_PDUR_03 */
  {
    PduR_FmFifoElementRamIterType fmFifoElementRamReadIdx = PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(fmFifoRamIdx, memIdx);

    if(PduR_Fm_IsFifoElementAllocated(fmFifoElementRamReadIdx, memIdx) == E_OK)
    {
      *fmFifoElementRamIdx = fmFifoElementRamReadIdx;
      retVal = E_OK;
      break;
    }
    else
    {
      /* If a gap in the FIFO was left, increment readIdx and skip this element. Try with the next one if available. */
      PduR_DecFillLevelOfFmFifoRam(fmFifoRamIdx, memIdx);
      PduR_Fm_IncrementReadIndex(fmFifoRamIdx, memIdx);
      continue;
    }
  }
  /*@ assert retVal != E_OK || *fmFifoElementRamIdx < PduR_GetSizeOfFmFifoElementRam(memIdx); */
  return retVal;
}
#endif

#if (PDUR_FMFIFORAM  == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_AllocateFifoElement
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
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_AllocateFifoElement(PduR_FmFifoRomIterType fmFifoRomIdx,
                                                            PduR_MemIdxType memIdx,
                                                            PduR_RmDestRomIterType rmDestRomIdx,
                                                            PduLengthType pduLength,
                                                            PduLengthType optimalBufferSize,
                                                            PduLengthType minimumBufferSize,
                                                            PduR_FmFifoElementRamIterType* fmFifoElementRamIdx,
                                                            PduR_BmTxBufferRomIdxOfBmTxBufferIndRomType* bmTxBufferRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_FmFifoElementRamWriteIdxOfFmFifoRamType fmFifoElementRamWriteIdx = PduR_Fm_GetFmFifoElementRamWriteIdxSafe(fmFifoRomIdx, memIdx);

  if(PduR_Fm_IsFifoElementAllocated(fmFifoElementRamWriteIdx, memIdx) != E_OK)
  {
    if(PduR_Fm_AllocateTxBuffer(fmFifoRomIdx, pduLength, optimalBufferSize, minimumBufferSize, bmTxBufferRomIdx, memIdx) == BUFREQ_OK)
    {
      PduR_IncFillLevelOfFmFifoRam(fmFifoRomIdx, memIdx);
      PduR_Fm_IncrementWriteIndex(fmFifoRomIdx, memIdx);

      PduR_SetBmTxBufferRomIdxOfFmFifoElementRam(fmFifoElementRamWriteIdx, *bmTxBufferRomIdx, memIdx);
      PduR_SetRmDestRomIdxOfFmFifoElementRam(fmFifoElementRamWriteIdx, (PduR_RmDestRomIdxOfFmFifoElementRamType)rmDestRomIdx, memIdx);

      /* return this fifo element */
      *fmFifoElementRamIdx = fmFifoElementRamWriteIdx;

      retVal = E_OK;
    }
  }
  /*@ assert retVal != E_OK || *fmFifoElementRamIdx < PduR_GetSizeOfFmFifoElementRam(memIdx); */
  return retVal;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_IsFifoElementAllocated
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_IsFifoElementAllocated(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, PduR_MemIdxType memIdx)
{
  PduR_BmTxBufferRomIterType bmTxBufferRomIdx = 0u;

  /* Is a TxBuffer assigned to the FifoElement? */
  return PduR_Fm_GetCurrentlyUsedBmTxBufferRomIdxByFmFifoElementIdx(fmFifoElementRamIdx, &bmTxBufferRomIdx, memIdx);
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetBmTxBufferInstanceByFmFifoElement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
  *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetBmTxBufferInstanceByFmFifoElement(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, PduR_MemIdxType memIdx, P2VAR(PduR_BmTxBufferInstanceRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferInstanceRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;

  PduR_BmTxBufferRomIterType bmTxBufferRomIdx = 0u;

  if(PduR_Fm_GetCurrentlyUsedBmTxBufferRomIdxByFmFifoElementIdx(fmFifoElementRamIdx, &bmTxBufferRomIdx, memIdx) == E_OK)
  {
    if(PduR_Bm_GetBmTxBufferInstanceByBmTxBufferChecked(bmTxBufferRomIdx, bmTxBufferInstanceRomIdx, memIdx) == E_OK)
    {
      PduR_Bm_EnableBmTxBufferInstance(bmTxBufferRomIdx, *bmTxBufferInstanceRomIdx, memIdx);
      retVal = E_OK;
    }
  }
  /*@ assert retVal != E_OK || *bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx); */
  return retVal;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_ReleaseFifoElement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
  *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Fm_ReleaseFifoElement(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, PduR_MemIdxType memIdx)
{
  PduR_BmTxBufferRomIterType bmTxBufferRomIdx = 0u;

  /* Reset (optional) referenced TxBuffer */
  if(PduR_Fm_GetCurrentlyUsedBmTxBufferRomIdxByFmFifoElementIdx(fmFifoElementRamIdx, &bmTxBufferRomIdx, memIdx) == E_OK)
  {
    PduR_Bm_ResetTxBuffer(bmTxBufferRomIdx, memIdx);
  }

  PduR_SetRmDestRomIdxOfFmFifoElementRam(fmFifoElementRamIdx, PDUR_NO_RMDESTROMIDXOFFMFIFOELEMENTRAM, memIdx);
  /* Release Tx buffer Fifo Element connection */
  PduR_SetBmTxBufferRomIdxOfFmFifoElementRam(fmFifoElementRamIdx, PDUR_NO_BMTXBUFFERROMIDXOFFMFIFOELEMENTRAM, memIdx);
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_RemoveOldestElement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
  *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Fm_RemoveOldestElement(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx)
{
  if(PduR_Fm_GetFillLevelSafe(fmFifoRomIdx, memIdx) > 0u)
  {
    PduR_Fm_ReleaseFifoElement(PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(fmFifoRomIdx, memIdx), memIdx);

    PduR_DecFillLevelOfFmFifoRam(fmFifoRomIdx, memIdx);
    PduR_Fm_IncrementReadIndex(fmFifoRomIdx, memIdx);
  }
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetCurrentlyUsedBmTxBufferRomIdxByFmFifoElementIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
  *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetCurrentlyUsedBmTxBufferRomIdxByFmFifoElementIdx(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, P2VAR(PduR_BmTxBufferRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferRomIdx, PduR_MemIdxType memIdx)
{
  Std_ReturnType retVal;
  PduR_BmTxBufferRomIterType readBmTxBufferRomIdx = PduR_GetBmTxBufferRomIdxOfFmFifoElementRam(fmFifoElementRamIdx, memIdx);

  if(readBmTxBufferRomIdx == PDUR_NO_BMTXBUFFERROMIDXOFFMFIFOELEMENTRAM)
  {
    retVal = E_NOT_OK;
  }
# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  else if(readBmTxBufferRomIdx >= PduR_GetSizeOfBmTxBufferRom(memIdx))
  {
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETTXBUFFEROFFIFOELEMENT, PDUR_E_IDXOUTOFBOUNDS);
    retVal = E_NOT_OK;
  }
# endif
  else
  {
    *bmTxBufferRomIdx = readBmTxBufferRomIdx;
    retVal = E_OK;
  }
  /*@ assert retVal != E_OK || *bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx); */  /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return retVal;
}
#endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_IncrementReadIndex
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
  *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Fm_IncrementReadIndex(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx)
{
  PduR_FmFifoElementRamReadIdxOfFmFifoRamType fmFifoElementRamReadIdx = PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(fmFifoRamIdx, memIdx);

  /* Get next element */
  if(fmFifoElementRamReadIdx >= ((PduR_GetFmFifoElementRamEndIdxOfFmFifoRom(fmFifoRamIdx, memIdx) - 1u)))
  {
    /* Wrap-around */
    fmFifoElementRamReadIdx = PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRamIdx, memIdx);
  }
  else
  {
    fmFifoElementRamReadIdx++;
  }
  PduR_SetFmFifoElementRamReadIdxOfFmFifoRam(fmFifoRamIdx, fmFifoElementRamReadIdx, memIdx);
}
#endif

# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_IncrementWriteIndex
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
  *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Fm_IncrementWriteIndex(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx)
{
  PduR_FmFifoElementRamWriteIdxOfFmFifoRamType fmFifoElementRamWriteIdx = PduR_Fm_GetFmFifoElementRamWriteIdxSafe(fmFifoRamIdx, memIdx);

  /* Get next fifo element */ /* Check if the write Idx is at the end (EndIdx -1) */
  if(fmFifoElementRamWriteIdx >= (PduR_GetFmFifoElementRamEndIdxOfFmFifoRom(fmFifoRamIdx, memIdx) - 1u))
  {
    /* Wrap-around */
    fmFifoElementRamWriteIdx = PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRamIdx, memIdx);
  }
  else
  {
    fmFifoElementRamWriteIdx++;
  }
  /* Update global FifoElement write pointer. This pointer points to the next available Fifo element. */
  PduR_SetFmFifoElementRamWriteIdxOfFmFifoRam(fmFifoRamIdx, fmFifoElementRamWriteIdx, memIdx);
}
#endif

/**********************************************************************************************************************
 * PduR_Fm_PutFifo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_PutFifo(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx, PduR_RmDestRomIterType rmDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  PduR_FmFifoElementRamIterType allocatedFmFifoElementRamIdx = 0u;
  PduR_BmTxBufferRomIdxOfBmTxBufferIndRomType allocatedBmTxBufferRomIdx = 0u;

  if(PduR_Fm_AllocateFifoElement(fmFifoRomIdx, memIdx, rmDestRomIdx, pduLength, pduLength, pduLength, &allocatedFmFifoElementRamIdx, &allocatedBmTxBufferRomIdx) == E_OK)
  {
    (void) PduR_Bm_PutData(allocatedBmTxBufferRomIdx, pduLength, sduDataPtr, memIdx);
    retVal = E_OK;
  }
  return retVal;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetNextElement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetNextElement(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_FmFifoElementRamIterType fmFifoElementRamReadIdx = 0u;

  if(PduR_Fm_Peek(fmFifoRomIdx, &fmFifoElementRamReadIdx, memIdx) == E_OK)
  {
    PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx = 0u;

    if(PduR_Fm_GetBmTxBufferInstanceByFmFifoElement(fmFifoElementRamReadIdx, memIdx, &bmTxBufferInstanceRomIdx) == E_OK)
    {
      PduR_RmDestRomIterType localRmDestRomIdx = 0u;

      if(PduR_Fm_GetRmDestRomIdxOfFmFifoElementRamSafe(fmFifoElementRamReadIdx, &localRmDestRomIdx, memIdx) == E_OK)
      {
        if(PduR_Fm_ValidateReadRmDestRomIdxInIFContext(localRmDestRomIdx, fmFifoRomIdx, memIdx))
        {
          *info = PduR_Bm_GetReadPtrToBuffer(bmTxBufferInstanceRomIdx, memIdx);
          *rmDestRomIdx = localRmDestRomIdx;
          retVal = E_OK;
        }
      }
      /* Release the bufferInstance as the reading from the buffer was finished. If this element is read again, the same bufferInstance can be used again. */
      PduR_Bm_ResetTxBufferInstance(bmTxBufferInstanceRomIdx, memIdx);
    }
  }

  return retVal;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_ValidateReadRmDestRomIdxInIFContext
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_Fm_ValidateReadRmDestRomIdxInIFContext(PduR_RmDestRomIterType rmDestRomIdx, PduR_FmFifoRomIterType supposedFmFifoRomIdx, PduR_MemIdxType supposedMemIdx)
{
  boolean retVal = FALSE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
  /* IsIfQueued? */
  if(PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))
  {
    PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType actualMemIdx = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));

    /* Uses SharedBufferQueue? */
    if(PduR_IsFmFifoRomUsedOfRmBufferedIfPropertiesRom(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)), actualMemIdx))
    {
      PduR_FmFifoRomIterType actualFmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedIfPropertiesRom(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)), actualMemIdx);

      /* Uses *supposed* sharedBufferQueue? */
      if((supposedFmFifoRomIdx == actualFmFifoRomIdx) && (supposedMemIdx == actualMemIdx))  /* COV_PDUR_ROBUSTNESS */
      {
        retVal = TRUE;
      }
    }
  }
#endif

  PDUR_DUMMY_STATEMENT(rmDestRomIdx);          /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(supposedFmFifoRomIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(supposedMemIdx);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  return retVal;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetFillLevelSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(uint16, PDUR_CODE) PduR_Fm_GetFillLevelSafe(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  uint16 fillLevel = PduR_GetFillLevelOfFmFifoRam(fmFifoRamIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_FmFifoElementRamLengthOfFmFifoRomType fifoDepth = PduR_GetFmFifoElementRamLengthOfFmFifoRom(fmFifoRamIdx, memIdx);

  if(fillLevel > fifoDepth)
  {
    fillLevel = fifoDepth;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETFILLLEVEL, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif

  return fillLevel;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_FlushFiFo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Fm_FlushFiFo(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  PduR_FmFifoElementRamIterType fmFifoElementRamIdx;

  for(fmFifoElementRamIdx = PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRomIdx, memIdx); fmFifoElementRamIdx < PduR_GetFmFifoElementRamEndIdxOfFmFifoRom(fmFifoRomIdx, memIdx); fmFifoElementRamIdx++)  /* FETA_PDUR_01 */
  {
    PduR_Fm_ReleaseFifoElement(fmFifoElementRamIdx, memIdx);
  }

  PduR_SetFillLevelOfFmFifoRam(fmFifoRomIdx, 0u, memIdx);
  PduR_SetFmFifoElementRamReadIdxOfFmFifoRam(fmFifoRomIdx, PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRomIdx, memIdx), memIdx);
  PduR_SetFmFifoElementRamWriteIdxOfFmFifoRam(fmFifoRomIdx, PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRomIdx, memIdx), memIdx);
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(PduR_FmFifoElementRamReadIdxOfFmFifoRamType, PDUR_CODE) PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  PduR_FmFifoElementRamReadIdxOfFmFifoRamType readIdx = PduR_GetFmFifoElementRamReadIdxOfFmFifoRam(fmFifoRamIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_FmFifoElementRamStartIdxOfFmFifoRomType startIdxOfBuffer = PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRamIdx, memIdx);
  PduR_FmFifoElementRamEndIdxOfFmFifoRomType endIdxOfBuffer = PduR_GetFmFifoElementRamEndIdxOfFmFifoRom(fmFifoRamIdx, memIdx);

  if(readIdx >= endIdxOfBuffer)
  {
    readIdx = endIdxOfBuffer - 1u;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETFMREADIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
  if(readIdx < startIdxOfBuffer)
  {
    readIdx = startIdxOfBuffer;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETFMREADIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif
  /*@ assert readIdx < PduR_GetSizeOfFmFifoElementRam(memIdx); */  /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return readIdx;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetFmFifoElementRamWriteIdxSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_FmFifoElementRamWriteIdxOfFmFifoRamType, PDUR_CODE) PduR_Fm_GetFmFifoElementRamWriteIdxSafe(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx)
{
  PduR_FmFifoElementRamWriteIdxOfFmFifoRamType writeIdx = PduR_GetFmFifoElementRamWriteIdxOfFmFifoRam(fmFifoRamIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_FmFifoElementRamStartIdxOfFmFifoRomType startIdxOfBuffer = PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRamIdx, memIdx);
  PduR_FmFifoElementRamEndIdxOfFmFifoRomType endIdxOfBuffer = PduR_GetFmFifoElementRamEndIdxOfFmFifoRom(fmFifoRamIdx, memIdx);

  if(writeIdx >= endIdxOfBuffer)
  {
    writeIdx = endIdxOfBuffer - 1u;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETFMWRITEIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
  if(writeIdx < startIdxOfBuffer)
  {
    writeIdx = startIdxOfBuffer;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETFMWRITEIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif
  /*@ assert (writeIdx < PduR_GetSizeOfFmFifoElementRam(memIdx)); */
  return writeIdx;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetRmDestRomIdxOfFmFifoElementRamSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetRmDestRomIdxOfFmFifoElementRamSafe(PduR_FmFifoElementRamIterType fmFifoElementRamIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, PduR_MemIdxType memIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmDestRomIdxOfFmFifoElementRamType readRmDestRomIdx = PduR_GetRmDestRomIdxOfFmFifoElementRam(fmFifoElementRamIdx, memIdx);

  if(readRmDestRomIdx == PDUR_NO_RMDESTROMIDXOFFMFIFOELEMENTRAM)
  {
    /* No Pdu is stored in this fifoElement right now. */
    retVal = E_NOT_OK;  /* PRQA S 2982 */ /* MD_PduR_2982_2983 */
  }
# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  else if(readRmDestRomIdx >= PduR_GetSizeOfRmDestRom())
  {
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETDESTHNDOFFIFOELEMENT, PDUR_E_IDXOUTOFBOUNDS);
    retVal = E_NOT_OK;
  }
  else
# endif
  {
    *rmDestRomIdx = readRmDestRomIdx;
    retVal = E_OK;
  }
  /*@ assert retVal != E_OK || *rmDestRomIdx < PduR_GetSizeOfRmDestRom(); */  /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return retVal;
}
#endif

#if(PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_GetRmSrcRomIdxOfOldestFmFifoElementSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *   -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Fm_GetRmSrcRomIdxOfOldestFmFifoElementSafe(PduR_FmFifoRomIterType fmFifoRomIdx, P2VAR(PduR_RmSrcRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmSrcRomIdx, PduR_MemIdxType memIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_RmDestRomIterType rmDestRomIdx = 0u;

  if(PduR_Fm_GetRmDestRomIdxOfFmFifoElementRamSafe(PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(fmFifoRomIdx, memIdx), &rmDestRomIdx, memIdx) == E_OK)
  {
    *rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);
    retVal = E_OK;
  }
  /*@ assert retVal != E_OK || *rmSrcRomIdx < PduR_GetSizeOfRmSrcRom(); */  /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return retVal;
}
#endif

#if (PDUR_FMFIFORAM == STD_ON )  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Fm_AllocateTxBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Fm_AllocateTxBuffer(PduR_FmFifoRomIterType fmFifoRomIdx,
                                                                              PduLengthType pduLength,
                                                                              PduLengthType optimalBufferSize,
                                                                              PduLengthType minimumBufferSize,
                                                                              P2VAR(PduR_BmTxBufferRomIdxOfBmTxBufferIndRomType, AUTOMATIC, PDUR_APPL_DATA) allocatedTxBufferIdxPtr,
                                                                              PduR_MemIdxType memIdx)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* A suitable buffer has been found if a valid index was set (< Size of table). */
  PduR_BmTxBufferRomIdxOfBmTxBufferIndRomType lastSuitableBmTxBufferRomIdx = PduR_GetSizeOfBmTxBufferRom(memIdx);
  PduR_BmTxBufferIndRomIterType bmTxBufferRomIndIdx;

  {
    PduR_BmTxBufferIndRomIterType bmTxBufferRomIndStartId = PduR_GetBmTxBufferIndRomStartIdxOfFmFifoRom(fmFifoRomIdx, memIdx);
    PduR_BmTxBufferIndRomIterType bmTxBufferRomIndEndIdx = PduR_GetBmTxBufferIndRomEndIdxOfFmFifoRom(fmFifoRomIdx, memIdx);

    PduR_Lock_LockBuffersByFmFifo(fmFifoRomIdx, memIdx);
    {
      for(bmTxBufferRomIndIdx = bmTxBufferRomIndStartId; bmTxBufferRomIndIdx < bmTxBufferRomIndEndIdx; bmTxBufferRomIndIdx++)  /* FETA_PDUR_01 */
      {
        PduR_BmTxBufferRomIdxOfBmTxBufferIndRomType bmTxBufferRomIdx = PduR_GetBmTxBufferRomIdxOfBmTxBufferIndRom(bmTxBufferRomIndIdx, memIdx);

        if(!PduR_Bm_IsTxBufferAllocated(bmTxBufferRomIdx, memIdx))
        {
          PduR_BmTxBufferArrayRamIterType bufferSize = PduR_Bm_GetSizeOfTxBuffer(bmTxBufferRomIdx, memIdx);

          if(bufferSize >= (PduR_BmTxBufferArrayRamIterType) optimalBufferSize)
          {
            PduR_Bm_AllocateTxBuffer(bmTxBufferRomIdx, pduLength, memIdx);
            *allocatedTxBufferIdxPtr = bmTxBufferRomIdx;
            retVal = BUFREQ_OK;
            break;
          }
          else if(bufferSize >= (PduR_BmTxBufferArrayRamIterType) minimumBufferSize)
          {
            /* This buffer is not large enough to hold the optimal size but the buffer is available and could be used in case no other buffer is available.
             * Reason: the buffer is large enough to hold at least the minimum size. */
            lastSuitableBmTxBufferRomIdx = bmTxBufferRomIdx; /* PRQA S 2983 */ /* MD_PduR_2982_2983 */
          }
          else
          {
            /* nothing to be done */
          }
        }
      }
      /* No suitable buffer found which is large enough to store the full TP PDU. Use ring buffer mechanism (routing-on-the-fly) instead. */
      if(retVal != BUFREQ_OK)
      {
        /* The received TP PDU is larger than the largest available TP buffer element. The routing is executed using a ring buffer mechanism (routing on the fly) that allows a smaller receive buffer. */
        /* A suitable buffer was found if some valid index was set to this local variable. */
        if(lastSuitableBmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRom(memIdx))
        {
          /* Routing is possible, but the gateway will use the ring-buffer mechanism to perform routing. */
          PduR_Bm_AllocateTxBuffer(lastSuitableBmTxBufferRomIdx, pduLength, memIdx);
          *allocatedTxBufferIdxPtr = lastSuitableBmTxBufferRomIdx;
          retVal = BUFREQ_OK;
        }
        else
        {
          /* Currently no buffer is available. */
          retVal = BUFREQ_E_NOT_OK;
        }
      }
    }
    PduR_Lock_UnlockBuffersByFmFifo(fmFifoRomIdx, memIdx);
  }
  /*@ assert retVal != BUFREQ_OK || *allocatedTxBufferIdxPtr < PduR_GetSizeOfBmTxBufferRom(memIdx); */  /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return retVal;
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_Fm.c
 *********************************************************************************************************************/
