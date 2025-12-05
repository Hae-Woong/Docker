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
 *         \file  PduR_Bm.c
 *         \unit  Bm
 *        \brief  Pdu Router Buffer Manager source file
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

#define PDUR_BM_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Fm.h"
#include "PduR_Bm.h"
#include "PduR_Lock.h"
#include "PduR_Reporting.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# if(PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
#  define PduR_Bm_GetLengthOfBmTxBuffer(Index, memIdx)                       ((PduR_BmTxBufferArrayRamIterType) PduR_GetBmTxBufferArrayRamLengthOfBmTxBufferRom(Index, memIdx) - 1u)
# endif

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

#if(PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_Init_TxBuffer
 *********************************************************************************************************************/
/*! \brief      This function initializes the BufferManager TxBuffer
 *  \details    -
 *  \param[in]  memIdx  memory section index
 *  \pre        -
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Bm_Init_TxBuffer(PduR_MemIdxType memIdx);
#endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_IsBmTxBufferInstanceUsed
 *********************************************************************************************************************/
/*! \brief       Checks if this BmTxBufferInstance object is currently used for reading from the buffer.
 *  \details     -
 *  \param[in]   bmTxBufferInstanceRomIdx  Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \param[in]   memIdx                    memory section index
 *  \return      TRUE if BmTxBufferInstance object is currently used, otherwise FALSE.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_Bm_IsBmTxBufferInstanceUsed(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetTotalBufferSizeInternal
 *********************************************************************************************************************/
/*! \brief       This function returns the total free buffer size of the buffer.
 *  \details     The total free buffer size is the total amount of free bytes in the buffer.
 *               This total free buffer size is from the writers point of view. The slowest reader determines this value.
 *  \param[in]   readIdx Overall readIdx. Hand over the slowest of all read indices.
 *  \param[in]   writeIdx Write index of the buffer.
 *  \param[in]   startIdxOfBuffer Start index of the buffer.
 *  \param[in]   endIdxOfBuffer End index of the buffer. This has to be the CSL EndIdx of a 1:N indirection. Thus this is already the first out-of-bounds index.
 *  \return      The total free buffer size.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetTotalBufferSizeInternal(PduR_BmTxBufferArrayRamIterType readIdx,
                                                                                                      PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                                      PduR_BmTxBufferArrayRamIterType startIdxOfBuffer,
                                                                                                      PduR_BmTxBufferArrayRamIterType endIdxOfBuffer);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetLinearDataSizeInstance
 *********************************************************************************************************************/
/*! \brief       This function returns the linear data size of the buffer.
 *  \details     The linear data size is the amount of stored bytes in the buffer which can be read without doing a wrap-around.
 *  \param[in]   readIdx Instance readIdx. Hand over the readIdx of one of the BmTxBufferInstances
 *  \param[in]   writeIdx Write index of the buffer.
 *  \param[in]   endIdxOfBuffer End index of the buffer. This has to be the CSL EndIdx of a 1:N indirection. Thus this is already the first out-of-bounds index.
 *  \return      The linear data size.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetLinearDataSizeInstance(PduR_BmTxBufferArrayRamIterType readIdx,
                                                                                                     PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                                     PduR_BmTxBufferArrayRamIterType endIdxOfBuffer);
# endif

# if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetTotalDataSizeInstanceInternal
 *********************************************************************************************************************/
/*! \brief       This function returns the total data size.
 *  \details     The total data size is the total amount of stored bytes in the buffer.
 *               This total data size is from the readers point of view. This value can differ per reader depending on how much data was already read.
 *  \param[in]   readIdx Instance readIdx. Hand over the readIdx of one of the BmTxBufferInstances
 *  \param[in]   writeIdx Write index of the buffer.
 *  \param[in]   startIdxOfBuffer Start index of the buffer.
 *  \param[in]   endIdxOfBuffer End index of the buffer. This has to be the CSL EndIdx of a 1:N indirection. Thus this is already the first out-of-bounds index.
 *  \return      The total data size of this buffer instance.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetTotalDataSizeInstanceInternal(PduR_BmTxBufferArrayRamIterType readIdx,
                                                                                                            PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                                            PduR_BmTxBufferArrayRamIterType startIdxOfBuffer,
                                                                                                            PduR_BmTxBufferArrayRamIterType endIdxOfBuffer);
# endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetLinearBufferSize
 *********************************************************************************************************************/
/*! \brief      Get the free buffer size which is available without doing a wrap around.
 *  \details    -
 *  \param[in]  readIdx Overall readIdx. Hand over the slowest of all read indices.
 *  \param[in]  writeIdx Write index of the buffer.
 *  \param[in]  startIdxOfBuffer Start index of the buffer.
 *  \param[in]  endIdxOfBuffer End index of the buffer. This has to be the CSL EndIdx of a 1:N indirection. Thus this is already the first out-of-bounds index.
 *  \return     the calculated free buffer size
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetLinearBufferSize(PduR_BmTxBufferArrayRamIterType readIdx,
                                                                                               PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                               PduR_BmTxBufferArrayRamIterType startIdxOfBuffer,
                                                                                               PduR_BmTxBufferArrayRamIterType endIdxOfBuffer);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_Init_TxBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Bm_Init_TxBuffer(PduR_MemIdxType memIdx)
{
  PduR_BmTxBufferRomIterType bmTxBufferRomIdx;
  for(bmTxBufferRomIdx = 0u; bmTxBufferRomIdx < PduR_GetSizeOfBmTxBufferRam(memIdx); bmTxBufferRomIdx++)  /* FETA_PDUR_01 */
  {
    PduR_Bm_ResetTxBuffer(bmTxBufferRomIdx, memIdx);
  }
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_EnableBmTxBufferInstance
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_EnableBmTxBufferInstance(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx)
{
  PduR_SetBmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRam(bmTxBufferInstanceRomIdx, PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx), memIdx);
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetLinearBufferSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetLinearBufferSize(PduR_BmTxBufferArrayRamIterType readIdx,
                                                                                               PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                               PduR_BmTxBufferArrayRamIterType startIdxOfBuffer,
                                                                                               PduR_BmTxBufferArrayRamIterType endIdxOfBuffer)
{
  PduR_BmTxBufferArrayRamIterType linearBufferSize = 0u;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(readIdx == startIdxOfBuffer)
  {
    /* the writeIdx can't move up to the readIdx position (which is equal to the startIdx), that's why the buffer can only be filled to the end - 1. */
    linearBufferSize = (endIdxOfBuffer - writeIdx) - 1u;
  }
  else if(readIdx == writeIdx)
  {
    /* The writeIdx can make a wrap-around here, as the readIdx is NOT equal to the startIdx. */
    linearBufferSize = (endIdxOfBuffer - writeIdx);
  }
  else if(readIdx < writeIdx)
  {
    /* The writeIdx can make a wrap-around here, as the readIdx is NOT equal to the startIdx. */
    linearBufferSize = (endIdxOfBuffer - writeIdx);
  }
  else if(readIdx > writeIdx)  /* COV_PDUR_MISRA */
  {
    /* the writeIdx can't move up to the readIdx position, that's why there's the -1. */
    linearBufferSize = (readIdx - writeIdx) - 1u;
  }
  else
  {
    /* This should never be reached. */
  }

  return linearBufferSize;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 1505, 6080 1 */ /* MD_PduR_1505, MD_MSR_STMIF */
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Bm_GetData(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduLengthType requestedPduLength, CONST(SduDataPtrType, AUTOMATIC) sduDataPtr, PduR_MemIdxType memIdx) /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if((requestedPduLength == 0u) || (sduDataPtr != NULL_PTR))
  {
    PduR_BmTxBufferArrayRamIterType readIdx = 0u;

    if(PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked(bmTxBufferInstanceRomIdx, &readIdx, memIdx) == E_OK)
    {
      PduR_BmTxBufferRomIterType bmTxBufferRomIdx = PduR_GetBmTxBufferRomIdxOfBmTxBufferInstanceRom(bmTxBufferInstanceRomIdx, memIdx);
      PduR_BmTxBufferArrayRamIterType writeIdx = PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(bmTxBufferRomIdx, memIdx);
      PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
      PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);

      PduR_BmTxBufferArrayRamIterType newReadIdx = readIdx;

      PduR_BmTxBufferArrayRamIterType totalDataSize = PduR_Bm_GetTotalDataSizeInstanceInternal(readIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);

      if(requestedPduLength <= totalDataSize)
      {
        PduR_BmTxBufferArrayRamIterType linearDataSize = PduR_Bm_GetLinearDataSizeInstance(readIdx, writeIdx, endIdxOfBuffer);
        PduR_BmTxBufferArrayRamIterType sizeOfFirstChunk  = 0u;
        PduR_BmTxBufferArrayRamIterType sizeOfSecondChunk = 0u;
        PduR_BmTxBufferArrayRamIterType localRequestedPduLength = (PduR_BmTxBufferArrayRamIterType) requestedPduLength; /* The value of requestedPduLength is always smaller than totalDataSize and fits into the PduR_BmTxBufferArrayRamIterType type due to the if condition above */

        if(localRequestedPduLength > linearDataSize)
        {
          sizeOfFirstChunk = linearDataSize;
          sizeOfSecondChunk = localRequestedPduLength - linearDataSize;
        }
        else
        {
          sizeOfFirstChunk = localRequestedPduLength;
          sizeOfSecondChunk = 0u;
        }

        if(sizeOfFirstChunk > 0u)
        {
          if(sduDataPtr != NULL_PTR)  /* COV_PDUR_VCA_CHECK */
          {
            /* @ assert $external(sduDataPtr); */  /* VCA_PDUR_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
            /* @ assert ($lengthOf(sduDataPtr) >= localRequestedPduLength); */  /* VCA_PDUR_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
            VStdLib_MemCpy_s(sduDataPtr, (VStdLib_CntType) localRequestedPduLength, PduR_GetAddrBmTxBufferArrayRam(readIdx, memIdx), (VStdLib_CntType) sizeOfFirstChunk); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* VCA_PDUR_BM_MEMCPY_S_3 */
          }
          newReadIdx = ((readIdx + sizeOfFirstChunk) == endIdxOfBuffer) ? startIdxOfBuffer : (readIdx + sizeOfFirstChunk);

          if(sizeOfSecondChunk > 0u)
          {
            if(sduDataPtr != NULL_PTR)  /* COV_PDUR_VCA_CHECK */
            {
              VStdLib_MemCpy_s(&sduDataPtr[sizeOfFirstChunk], ((VStdLib_CntType) localRequestedPduLength - sizeOfFirstChunk), PduR_GetAddrBmTxBufferArrayRam(startIdxOfBuffer, memIdx), (VStdLib_CntType) sizeOfSecondChunk); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* VCA_PDUR_BM_MEMCPY_S_4 */
            }
            newReadIdx = startIdxOfBuffer + sizeOfSecondChunk; /* PRQA S 2986 */ /* MD_PduR_2985_2986 */
          }

          PduR_SetBmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRam(bmTxBufferInstanceRomIdx, (PduR_BmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRamType) newReadIdx, memIdx);
        }

        retVal = BUFREQ_OK;
      }
      else
      {
        retVal = BUFREQ_E_BUSY;
      }
    }
  }

  return retVal;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx)
{
  PduR_BmTxBufferArrayRamIterType bmTxBufferArrayRamWriteIdxOfBmTxBufferRam = PduR_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRam(bmTxBufferRomIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);

  if(bmTxBufferArrayRamWriteIdxOfBmTxBufferRam >= endIdxOfBuffer)
  {
    bmTxBufferArrayRamWriteIdxOfBmTxBufferRam = endIdxOfBuffer - 1u;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GW_BUFMGR_GET_WRITEIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
  if(bmTxBufferArrayRamWriteIdxOfBmTxBufferRam < startIdxOfBuffer)
  {
    bmTxBufferArrayRamWriteIdxOfBmTxBufferRam = startIdxOfBuffer;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GW_BUFMGR_GET_WRITEIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif
  /*@ assert (bmTxBufferArrayRamWriteIdxOfBmTxBufferRam  < PduR_GetSizeOfBmTxBufferArrayRam(memIdx)); */  /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return bmTxBufferArrayRamWriteIdxOfBmTxBufferRam;
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if(PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_Init(PduR_MemIdxType memIdx)
{
  /* Init Buffer manager */
  PduR_Bm_Init_TxBuffer(memIdx);
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetBmTxBufferArrayRamReadIdxOfBmTxBufferRam
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetBmTxBufferArrayRamReadIdxOfBmTxBufferRam(PduR_BmTxBufferRomIterType bmTxBufferRomIdx,
                                                                                                     PduR_MemIdxType memIdx,
                                                                                                     PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                                     PduR_BmTxBufferArrayRamIterType startIdxOfBuffer,
                                                                                                     PduR_BmTxBufferArrayRamIterType endIdxOfBuffer)
{
  PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx = 0u;
  PduR_BmTxBufferArrayRamIterType slowestInstanceReadIndex = startIdxOfBuffer;

  PduR_BmTxBufferArrayRamIterType smallestReadIdxLargerThanWriteIdx = endIdxOfBuffer - 1u;
  boolean smallestReadIdxLargerThanWriteIdxFound = FALSE;

  PduR_BmTxBufferArrayRamIterType smallestReadIdxSmallerOrEqualThanWriteIdx = writeIdx;
  boolean smallestReadIdxSmallerOrEqualThanWriteIdxFound = FALSE;

  for(bmTxBufferInstanceRomIdx = PduR_GetBmTxBufferInstanceRomStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);  /* FETA_PDUR_01 */
      bmTxBufferInstanceRomIdx < PduR_GetBmTxBufferInstanceRomEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx); bmTxBufferInstanceRomIdx++)
  {
    PduR_BmTxBufferArrayRamIterType bmTxBufferArrayRamReadIdx = 0u;

    if(PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked(bmTxBufferInstanceRomIdx, &bmTxBufferArrayRamReadIdx, memIdx) == E_OK) /* COV_PDUR_IF_ONLY */
    {
      if(bmTxBufferArrayRamReadIdx > writeIdx)
      {
        if(bmTxBufferArrayRamReadIdx <= smallestReadIdxLargerThanWriteIdx)
        {
          smallestReadIdxLargerThanWriteIdx = bmTxBufferArrayRamReadIdx;
          smallestReadIdxLargerThanWriteIdxFound = TRUE;
        }
      }
      else /* bmTxBufferArrayRamReadIdx <= writeIdx */
      {
        if(bmTxBufferArrayRamReadIdx <= smallestReadIdxSmallerOrEqualThanWriteIdx)
        {
          smallestReadIdxSmallerOrEqualThanWriteIdx = bmTxBufferArrayRamReadIdx;
          smallestReadIdxSmallerOrEqualThanWriteIdxFound = TRUE;
        }
      }
    }
  }

  if(smallestReadIdxLargerThanWriteIdxFound)
  {
    slowestInstanceReadIndex = smallestReadIdxLargerThanWriteIdx;
  }
  else if(smallestReadIdxSmallerOrEqualThanWriteIdxFound)
  {
    slowestInstanceReadIndex = smallestReadIdxSmallerOrEqualThanWriteIdx;
  }
  else
  {
    slowestInstanceReadIndex = startIdxOfBuffer;
  }

  return slowestInstanceReadIndex;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_PutData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Bm_PutData(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr, PduR_MemIdxType memIdx) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if((pduLength == 0u) || (sduDataPtr != NULL_PTR))
  {
    PduR_BmTxBufferArrayRamIterType writeIdx = PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(bmTxBufferRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType readIdx = PduR_Bm_GetBmTxBufferArrayRamReadIdxOfBmTxBufferRam(bmTxBufferRomIdx, memIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);

    PduR_BmTxBufferArrayRamIterType newWriteIdx = writeIdx;

    PduR_BmTxBufferArrayRamIterType totalBufferSize = PduR_Bm_GetTotalBufferSizeInternal(readIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);

    if(pduLength <= totalBufferSize)
    {
      PduR_BmTxBufferArrayRamIterType linearBufferSize = PduR_Bm_GetLinearBufferSize(readIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);
      PduR_BmTxBufferArrayRamIterType sizeOfFirstChunk  = 0u;
      PduR_BmTxBufferArrayRamIterType sizeOfSecondChunk = 0u;
      PduR_BmTxBufferArrayRamIterType localPduLength = (PduR_BmTxBufferArrayRamIterType) pduLength; /* The value of pduLength is always smaller than totalBufferSize and fits into the PduR_BmTxBufferArrayRamIterType type due to the if condition above */


      if(localPduLength > linearBufferSize)
      {
        sizeOfFirstChunk = linearBufferSize;
        sizeOfSecondChunk = localPduLength - linearBufferSize;
      }
      else
      {
        sizeOfFirstChunk = localPduLength;
        sizeOfSecondChunk = 0u;
      }

      /* Copy first chunk: writeIdx -> ReadIdx/EndIdx */
      if(sizeOfFirstChunk > 0u)
      {
        if(sduDataPtr != NULL_PTR)  /* COV_PDUR_VCA_CHECK */
        {
          VStdLib_MemCpy_s(PduR_GetAddrBmTxBufferArrayRam(writeIdx, memIdx), (VStdLib_CntType) linearBufferSize, sduDataPtr, (VStdLib_CntType) sizeOfFirstChunk); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* VCA_PDUR_BM_MEMCPY_S_1 */
        }
        newWriteIdx = ((writeIdx + sizeOfFirstChunk) == endIdxOfBuffer) ? startIdxOfBuffer : (writeIdx + sizeOfFirstChunk);

        /* Copy second chunk: startIdx -> ReadIdx */
        if(sizeOfSecondChunk > 0u)
        {
          if(sduDataPtr != NULL_PTR)  /* COV_PDUR_VCA_CHECK */
          {
            VStdLib_MemCpy_s(PduR_GetAddrBmTxBufferArrayRam(startIdxOfBuffer, memIdx), (VStdLib_CntType) sizeOfSecondChunk, &sduDataPtr[sizeOfFirstChunk], (VStdLib_CntType) sizeOfSecondChunk); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* VCA_PDUR_BM_MEMCPY_S_2 */
          }
          newWriteIdx = startIdxOfBuffer + sizeOfSecondChunk; /* PRQA S 2986 */ /* MD_PduR_2985_2986 */
        }

        /* Increment writeIdx */
        PduR_SetBmTxBufferArrayRamWriteIdxOfBmTxBufferRam(bmTxBufferRomIdx, (PduR_BmTxBufferArrayRamWriteIdxOfBmTxBufferRamType) newWriteIdx, memIdx);
      }

      retVal = BUFREQ_OK;
    }
    else
    {
      retVal = BUFREQ_E_NOT_OK;
    }
  }

  return retVal;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetWritePtrToBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(PduInfoType, PDUR_CODE) PduR_Bm_GetWritePtrToBuffer(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx)      /* COV_PDUR_IF_ONLY */  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  PduInfoType bufferInfo;
  PduR_BmTxBufferArrayRamIterType writeIdx = PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType readIdx = PduR_Bm_GetBmTxBufferArrayRamReadIdxOfBmTxBufferRam(bmTxBufferRomIdx, memIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);

  bufferInfo.SduDataPtr = PduR_GetAddrBmTxBufferArrayRam(writeIdx, memIdx);
  bufferInfo.SduLength = (PduLengthType) PduR_Bm_GetLinearBufferSize(readIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);

  return bufferInfo;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_IncrementWriteIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_IncrementWriteIdx(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx, PduLengthType pduLength)
{
  PduR_BmTxBufferArrayRamIterType writeIdx = PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType  startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType localPduLength = (PduR_BmTxBufferArrayRamIterType) pduLength;

  if((writeIdx + localPduLength) >= endIdxOfBuffer)
  {
    PduR_SetBmTxBufferArrayRamWriteIdxOfBmTxBufferRam(bmTxBufferRomIdx, (PduR_BmTxBufferArrayRamWriteIdxOfBmTxBufferRamType) startIdxOfBuffer, memIdx);
  }
  else
  {
    PduR_AddBmTxBufferArrayRamWriteIdxOfBmTxBufferRam(bmTxBufferRomIdx, (PduR_BmTxBufferArrayRamWriteIdxOfBmTxBufferRamType) localPduLength, memIdx);
  }
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_IncrementReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_IncrementReadIdx(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx, PduLengthType pduLength)
{
  PduR_BmTxBufferArrayRamIterType readIdx = 0u;

  if(PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked(bmTxBufferInstanceRomIdx, &readIdx, memIdx) == E_OK)
  {
    PduR_BmTxBufferRomIterType bmTxBufferRomIdx = PduR_GetBmTxBufferRomIdxOfBmTxBufferInstanceRom(bmTxBufferInstanceRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType localPduLength = (PduR_BmTxBufferArrayRamIterType) pduLength;

    if((readIdx + localPduLength) >= endIdxOfBuffer)
    {
      PduR_SetBmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRam(bmTxBufferInstanceRomIdx, (PduR_BmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRamType) startIdxOfBuffer, memIdx);
    }
    else
    {
      /* Increment readIdx */
      PduR_AddBmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRam(bmTxBufferInstanceRomIdx, (PduR_BmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRamType) localPduLength, memIdx);
    }
  }
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_ResetTxBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_ResetTxBuffer(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx)
{
  PduR_SetRxLengthOfBmTxBufferRam(bmTxBufferRomIdx, 0u, memIdx);
  PduR_SetAllocatedOfBmTxBufferRam(bmTxBufferRomIdx, FALSE, memIdx);
  PduR_SetBmTxBufferArrayRamWriteIdxOfBmTxBufferRam(bmTxBufferRomIdx, (PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx)), memIdx);

  {/* Also reset the associated bmTxBufferInstances */
    PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx = PduR_GetBmTxBufferInstanceRomStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);

    for(; bmTxBufferInstanceRomIdx < PduR_GetBmTxBufferInstanceRomEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx); bmTxBufferInstanceRomIdx++)  /* FETA_PDUR_01 */
    {
      PduR_Bm_ResetTxBufferInstance(bmTxBufferInstanceRomIdx, memIdx);
    }
  }

}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_Bm_ResetTxBufferInstance
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
  *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_ResetTxBufferInstance(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx)
{
  PduR_SetBmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRam(bmTxBufferInstanceRomIdx, PDUR_NO_BMTXBUFFERARRAYRAMREADIDXOFBMTXBUFFERINSTANCERAM, memIdx); /* PRQA S 2841 */ /* MD_PduR_2841 */
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetLinearDataSizeInstance
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetLinearDataSizeInstance(PduR_BmTxBufferArrayRamIterType readIdx,
                                                                                                     PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                                     PduR_BmTxBufferArrayRamIterType endIdxOfBuffer)
{
  PduR_BmTxBufferArrayRamIterType linearDataSize = 0u;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(readIdx == writeIdx)
  {
    linearDataSize = 0u;
  }
  else if(readIdx < writeIdx)
  {
    /* readIdx can move up to the writeIdx. So no -1 required. */
    linearDataSize = writeIdx - readIdx;
  }
  else if(readIdx > writeIdx)  /* COV_PDUR_MISRA */
  {
    /* readIdx can move up to the writeIdx (if the writeIdx is set to the startIdxOfBuffer). So no -1 required. */
    linearDataSize = endIdxOfBuffer - readIdx;
  }
  else
  {
    /* This should never be reached. */
  }

  return linearDataSize;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetBmTxBufferInstanceByBmTxBufferChecked
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Bm_GetBmTxBufferInstanceByBmTxBufferChecked(PduR_BmTxBufferRomIterType bmTxBufferRomIdx,
                                                                                 P2VAR(PduR_BmTxBufferInstanceRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferInstanceRomIdx2Return,
                                                                                 PduR_MemIdxType memIdx)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx = PduR_GetBmTxBufferInstanceRomStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);

  for(; bmTxBufferInstanceRomIdx < PduR_GetBmTxBufferInstanceRomEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx); bmTxBufferInstanceRomIdx++)  /* FETA_PDUR_01 */
  {
    if(!PduR_Bm_IsBmTxBufferInstanceUsed(bmTxBufferInstanceRomIdx, memIdx))
    {
      *bmTxBufferInstanceRomIdx2Return = bmTxBufferInstanceRomIdx;
      retVal = E_OK;
      break;
    }
  }
  /*@ assert retVal != E_OK || *bmTxBufferInstanceRomIdx2Return < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx); */  /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return retVal;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx,
                                                                                                      PduR_BmTxBufferArrayRamIterType* bmTxBufferArrayRamReadIdx,
                                                                                                      PduR_MemIdxType memIdx)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_BmTxBufferRomIterType bmTxBufferRomIdx = PduR_GetBmTxBufferRomIdxOfBmTxBufferInstanceRom(bmTxBufferInstanceRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);

  PduR_BmTxBufferArrayRamIterType readBmTxBufferArrayRamReadIdx = PduR_GetBmTxBufferArrayRamReadIdxOfBmTxBufferInstanceRam(bmTxBufferInstanceRomIdx, memIdx);

  if (readBmTxBufferArrayRamReadIdx == PDUR_NO_BMTXBUFFERARRAYRAMREADIDXOFBMTXBUFFERINSTANCERAM)
  {
    retVal = E_NOT_OK;
  }
# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  else if(readBmTxBufferArrayRamReadIdx >= endIdxOfBuffer)
  {
    *bmTxBufferArrayRamReadIdx = endIdxOfBuffer - 1u;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GW_BUFMGR_GET_READIDX, PDUR_E_IDXOUTOFBOUNDS);
    retVal = E_OK;
  }
  else if(readBmTxBufferArrayRamReadIdx < startIdxOfBuffer)
  {
    *bmTxBufferArrayRamReadIdx = startIdxOfBuffer;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GW_BUFMGR_GET_READIDX, PDUR_E_IDXOUTOFBOUNDS);
    retVal = E_OK;
  }
# endif
  else if((readBmTxBufferArrayRamReadIdx >= startIdxOfBuffer) && (readBmTxBufferArrayRamReadIdx < endIdxOfBuffer))  /* COV_PDUR_VCA_CHECK */
  {
    *bmTxBufferArrayRamReadIdx = readBmTxBufferArrayRamReadIdx;
    retVal = E_OK;
  }
  else
  {
    /* This should never be reached. */
  }

  PDUR_DUMMY_STATEMENT(startIdxOfBuffer);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(endIdxOfBuffer);          /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  /*@ assert retVal != E_OK || (*bmTxBufferArrayRamReadIdx < PduR_GetSizeOfBmTxBufferArrayRam(memIdx)); */  /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return retVal;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetTotalBufferSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetTotalBufferSize(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx)        /* PRQA S 1505 */ /* MD_PduR_1505 */
{
  PduR_BmTxBufferArrayRamIterType writeIdx = PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType readIdx = PduR_Bm_GetBmTxBufferArrayRamReadIdxOfBmTxBufferRam(bmTxBufferRomIdx, memIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);

  return PduR_Bm_GetTotalBufferSizeInternal(readIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetTotalBufferSizeInternal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
/* PRQA S 1505, 6080 1 */ /* MD_PduR_1505, MD_MSR_STMIF */
PDUR_LOCAL_INLINE FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetTotalBufferSizeInternal(PduR_BmTxBufferArrayRamIterType readIdx,
                                                                                                      PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                                      PduR_BmTxBufferArrayRamIterType startIdxOfBuffer,
                                                                                                      PduR_BmTxBufferArrayRamIterType endIdxOfBuffer)
{
  PduR_BmTxBufferArrayRamIterType totalBufferSize = 0u;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_BmTxBufferArrayRamIterType sizeOfTxBuffer = (endIdxOfBuffer - startIdxOfBuffer) - 1u;

  if(readIdx == writeIdx)
  {
    totalBufferSize = sizeOfTxBuffer;
  }
  else if((writeIdx + 1u) == readIdx)
  {
    /* Full condition 1. */
    totalBufferSize = 0u;
  }
  else if((writeIdx == (endIdxOfBuffer - 1u)) && (readIdx == startIdxOfBuffer))
  {
    /* Full condition 2. */
    totalBufferSize = 0u;
  }
  else if(writeIdx > readIdx)
  {
    /* Available size is the total buffer size minus the data which can be read. */
    totalBufferSize = sizeOfTxBuffer - (writeIdx - readIdx);
  }
  else if(writeIdx < readIdx)  /* COV_PDUR_MISRA */
  {
    /* WriteIdx can't move to the readIdx position. That's the reason for -1. */
    totalBufferSize = (readIdx - writeIdx) - 1u;
  }
  else
  {
    /* This should never be reached. */
  }

  return totalBufferSize;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetTotalDataSizeInstance
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetTotalDataSizeInstance(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx)  /* PRQA S 1505 */ /* MD_PduR_1505 */
{
  PduR_BmTxBufferArrayRamIterType totalDataSize = 0u;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  PduR_BmTxBufferArrayRamIterType readIdx = 0u;

  if(PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked(bmTxBufferInstanceRomIdx, &readIdx, memIdx) == E_OK)
  {
    PduR_BmTxBufferRomIterType bmTxBufferRomIdx = PduR_GetBmTxBufferRomIdxOfBmTxBufferInstanceRom(bmTxBufferInstanceRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType writeIdx = PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(bmTxBufferRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);

    totalDataSize = PduR_Bm_GetTotalDataSizeInstanceInternal(readIdx, writeIdx, startIdxOfBuffer, endIdxOfBuffer);
  }
  return totalDataSize;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetTotalDataSizeInstanceInternal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetTotalDataSizeInstanceInternal(PduR_BmTxBufferArrayRamIterType readIdx,
                                                                                                            PduR_BmTxBufferArrayRamIterType writeIdx,
                                                                                                            PduR_BmTxBufferArrayRamIterType startIdxOfBuffer,
                                                                                                            PduR_BmTxBufferArrayRamIterType endIdxOfBuffer)  /* PRQA S 1505 */ /* MD_PduR_1505 */
{
  PduR_BmTxBufferArrayRamIterType totalDataSize = 0u;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(readIdx == writeIdx)
  {
    totalDataSize = 0u;
  }
  else if(readIdx > writeIdx)
  {
    totalDataSize = (endIdxOfBuffer - readIdx) + (writeIdx - startIdxOfBuffer);
  }
  else if(readIdx < writeIdx)  /* COV_PDUR_MISRA */
  {
    totalDataSize = writeIdx - readIdx;
  }
  else
  {
    /* This should never be reached. */
  }

  return totalDataSize;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetReadPtrToBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(PduInfoType, PDUR_CODE) PduR_Bm_GetReadPtrToBuffer(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx)
{
  PduInfoType bufferInfo;
  PduR_BmTxBufferRomIterType bmTxBufferRomIdx = PduR_GetBmTxBufferRomIdxOfBmTxBufferInstanceRom(bmTxBufferInstanceRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType startIdxOfBuffer = PduR_GetBmTxBufferArrayRamStartIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType endIdxOfBuffer = PduR_GetBmTxBufferArrayRamEndIdxOfBmTxBufferRom(bmTxBufferRomIdx, memIdx);
  PduR_BmTxBufferArrayRamIterType readIdx = 0u;

  /* Add default 'initialization' for VCA analysis. */
  bufferInfo.SduDataPtr = PduR_GetAddrBmTxBufferArrayRam(startIdxOfBuffer, memIdx);
  bufferInfo.SduLength  = 0u;

  if(PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked(bmTxBufferInstanceRomIdx, &readIdx, memIdx) == E_OK)
  {
    PduR_BmTxBufferArrayRamIterType writeIdx = PduR_Bm_GetBmTxBufferArrayRamWriteIdxOfBmTxBufferRamSafe(bmTxBufferRomIdx, memIdx);

    bufferInfo.SduDataPtr = PduR_GetAddrBmTxBufferArrayRam(readIdx, memIdx);
    bufferInfo.SduLength  = (PduLengthType) PduR_Bm_GetLinearDataSizeInstance(readIdx, writeIdx, endIdxOfBuffer);
  }

  return bufferInfo;
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetActualPduLengthToBeRouted
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(PduLengthType, PDUR_CODE) PduR_Bm_GetActualPduLengthToBeRouted(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  return PduR_GetRxLengthOfBmTxBufferRam(PduR_GetBmTxBufferRomIdxOfBmTxBufferInstanceRom(bmTxBufferInstanceRomIdx, memIdx), memIdx);
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_IsBmTxBufferInstanceUsed
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_Bm_IsBmTxBufferInstanceUsed(PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx, PduR_MemIdxType memIdx)
{
  PduR_BmTxBufferArrayRamIterType bmTxBufferArrayRamReadIdx = 0u;

  return (PduR_Bm_GetBmTxBufferArrayRamReadIdxByBmTxBufferInstanceRomIdxChecked(bmTxBufferInstanceRomIdx, &bmTxBufferArrayRamReadIdx, memIdx) == E_OK);
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_GetSizeOfTxBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(PduR_BmTxBufferArrayRamIterType, PDUR_CODE) PduR_Bm_GetSizeOfTxBuffer(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  return PduR_Bm_GetLengthOfBmTxBuffer(bmTxBufferRomIdx, memIdx);
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_IsTxBufferAllocated
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_Bm_IsTxBufferAllocated(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  return PduR_IsAllocatedOfBmTxBufferRam(bmTxBufferRomIdx, memIdx);
}
#endif

#if (PDUR_BMTXBUFFERRAM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_Bm_AllocateTxBuffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Bm_AllocateTxBuffer(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduLengthType pduLength, PduR_MemIdxType memIdx)
{
  PduR_SetAllocatedOfBmTxBufferRam(bmTxBufferRomIdx, TRUE, memIdx);
  PduR_SetRxLengthOfBmTxBufferRam(bmTxBufferRomIdx, pduLength, memIdx);
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* *INDENT-OFF* */

/* module specific MISRA deviations:
     MD_PduR_2841: Dereference of an invalid pointer value.
     Reason:       QAC cannot determine whether the pointer value is correct. The callers of this function guarantee, that the index is always in bound.
     Risk:         No risk.
     Prevention:   No prevention necessary.
*/

/* *INDENT-ON* */
/**********************************************************************************************************************
 *  END OF FILE: PduR_Bm.c
 *********************************************************************************************************************/
