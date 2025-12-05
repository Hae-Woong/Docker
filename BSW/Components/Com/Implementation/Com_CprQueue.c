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
/*!        \file  Com_CprQueue.c
 *         \unit  CprQueue
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_CPRQUEUE_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_CprQueue.h"
#include "Com_Cfg_CrossPartitionQueueData.h"
#include "Com.h"
#include "Com_Reporting.h"
#include "vstdlib.h"

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
 * Com_CprQueue_Flush
 *********************************************************************************************************************/
/*! \brief       Flush the Queue by setting the readIdx and writeIdx to the Start index.
 *  \details     -
 *  \param[in]   routingQueueIndex    Id of the queue to be used
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueue_Flush(Com_CCprQueueIterType routingQueueIndex);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
 * Com_CprQueue_Flush
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueue_Flush(Com_CCprQueueIterType routingQueueIndex)
{
  const Com_CprQueueBufferStartIdxOfCprQueueType startIdxOfBuffer = Com_GetCprQueueBufferStartIdxOfCprQueue(routingQueueIndex);

  Com_SetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);
  Com_SetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);

  Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);
  Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
 * Com_CprQueue_GetWriteIdx
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
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, COM_CODE) Com_CprQueue_GetWriteIdx(Com_CCprQueueIterType routingQueueIndex,
                                                        CONSTP2VAR(Com_CprQueueBufferWriteIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) writeIdx, PduLengthType payloadLengthWithHeader)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  Com_CprQueueBufferReadIdxOfCprQueueType readIdx = Com_GetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex);
  Com_CprQueueBufferWriteIdxOfCprQueueType localWriteIdx = Com_GetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex);

  const Com_CprQueueBufferStartIdxOfCprQueueType startIdxOfBuffer = Com_GetCprQueueBufferStartIdxOfCprQueue(routingQueueIndex);
  /* ComStackLibs EndIdx is actually the first out of bounds index */
  const Com_CprQueueBufferEndIdxOfCprQueueType endIdxOfBuffer = Com_GetCprQueueBufferEndIdxOfCprQueue(routingQueueIndex) - 1u; /* PRQA S 2986 */ /* MD_Com_2985_2986 */

  Com_CprQueueBufferPendingWriteIdxOfCprQueueType nextWriteSlot = (Com_CprQueueBufferPendingWriteIdxOfCprQueueType) (localWriteIdx + payloadLengthWithHeader); /* PRQA S 2986 */ /* MD_Com_2985_2986 */

  if((localWriteIdx > endIdxOfBuffer) || (localWriteIdx < startIdxOfBuffer))
  {
    Com_CprQueue_Flush(routingQueueIndex);
    readIdx = Com_GetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex);
    localWriteIdx = Com_GetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex);
    Com_Reporting_CallDetReportError(COMServiceId_CprQueue_WriteIdx, COM_E_IDXOUTOFBOUNDS); /* COV_COM_REPORT_ERROR */
    /*@ assert localWriteIdx > endIdxOfBuffer; */ /* VCA_COM_BOUNDARY_CHECK */
    /*@ assert localWriteIdx < startIdxOfBuffer; */ /* VCA_COM_BOUNDARY_CHECK */
  }

  /* If buffer is full, return E_NOT_OK */
  if(((localWriteIdx + 1u) == readIdx) || ((readIdx == startIdxOfBuffer) && (localWriteIdx == endIdxOfBuffer)))
  {
    retVal = E_NOT_OK;
  }
  /* else if wrap around of write index has occurred */
  else if(localWriteIdx < readIdx)      /* write is possible up to the read Idx. Still needs to be checked for sufficient size */
  {
    const Com_CprQueueBufferWriteIdxOfCprQueueType linearBufferSize = readIdx - localWriteIdx - 1u;
    /* If CrossPartitionRoutingQueue has enough space */
    if(linearBufferSize >= payloadLengthWithHeader)
    {
      *writeIdx = localWriteIdx;
      /* Set write index to internal write index and increase pending-write-index to next slot */
      Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, nextWriteSlot);
      retVal = E_OK;
    }
    /* Otherwise, return E_NOT_OK */
    else
    {
      retVal = E_NOT_OK;
    }
  }
  /* else no wrap around of write index has occurred or both indicies have wrapped around */
  else
  {
    /* readIdx < localWriteIdx OR readIdx == localWriteIdx */
    const Com_CprQueueBufferWriteIdxOfCprQueueType linearBufferSizeAtEnd = (Com_CprQueueBufferWriteIdxOfCprQueueType) (endIdxOfBuffer - localWriteIdx);
    const Com_CprQueueBufferWriteIdxOfCprQueueType linearBufferSizeAtFront = (readIdx == startIdxOfBuffer) ? 0u : (readIdx - startIdxOfBuffer - 1u); /* PRQA S 2983 */ /* MD_Com_2982_2983 */
    /* If there is enough space at the end of the queue, set write index to internal write index and increase pending-write-index to next slot and return E_OK */
    if(linearBufferSizeAtEnd >= payloadLengthWithHeader)
    {
      *writeIdx = localWriteIdx;
      /* Update write Idx */
      Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, nextWriteSlot);
      retVal = E_OK;
    }
    /* Otherwise, if there is enough space at the beginning of the queue */
    else if(linearBufferSizeAtFront >= payloadLengthWithHeader)
    {
      /* Mark remaining bytes at end as unused */
      Com_SetCprQueueBuffer(localWriteIdx, COM_CPRQUEUE_HEADERVALUE_UNUSED);
      /* Set write index to start index and increase pending-write-index to next slot */
      *writeIdx = startIdxOfBuffer;
      /* Update write Idx */
      nextWriteSlot = (Com_CprQueueBufferPendingWriteIdxOfCprQueueType) (startIdxOfBuffer + payloadLengthWithHeader);
      Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, nextWriteSlot);

      retVal = E_OK;
    }
    /* Otherwise, return E_NOT_OK */
    else
    {
      /* neither fits at the end nor the beginning: do nothing */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * Com_CprQueue_GetReadIdx
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
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueue_GetReadIdx(Com_CCprQueueIterType routingQueueIndex, CONSTP2VAR(Com_CprQueueBufferReadIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) readIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  Com_CprQueueBufferReadIdxOfCprQueueType localReadIdx = Com_GetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex);
  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx = Com_GetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex);

  /* Get the appropriate start and end index of the buffer from the ROM array */
  const Com_CprQueueBufferStartIdxOfCprQueueType startIdxOfBuffer = Com_GetCprQueueBufferStartIdxOfCprQueue(routingQueueIndex);
  /* ComStackLibs EndIdx is actually the first out of bounds index */
  const Com_CprQueueBufferEndIdxOfCprQueueType endIdxOfBuffer = Com_GetCprQueueBufferEndIdxOfCprQueue(routingQueueIndex) - 1u; /* PRQA S 2986 */ /* MD_Com_2985_2986 */

  if((localReadIdx > endIdxOfBuffer) || (localReadIdx < startIdxOfBuffer))
  {
    Com_CprQueue_Flush(routingQueueIndex);
    localReadIdx = Com_GetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex);
    writeIdx = Com_GetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex);
    Com_Reporting_CallDetReportError(COMServiceId_CprQueue_ReadIdx, COM_E_IDXOUTOFBOUNDS); /* COV_COM_REPORT_ERROR */
  }
  /* If CrossPartitionRoutingQueue is empty, return E_NOT_OK */
  if(localReadIdx == writeIdx)
  {
    retVal = E_NOT_OK;
  }
  /* If the current element is marked with the unused pattern, return the start index of the queue, otherwise return the stored read index */
  else
  {
    if(Com_GetCprQueueBuffer(localReadIdx) == COM_CPRQUEUE_HEADERVALUE_UNUSED)
    {   /* adjust localReadIdx if the end of the buffer is not used */
      localReadIdx = Com_GetCprQueueBufferStartIdxOfCprQueue(routingQueueIndex);
    }
    *readIdx = localReadIdx;
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Com_CprQueue_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_Init(void)
{
  for(Com_CCprQueueIterType routingQueueIndex = 0u; routingQueueIndex < Com_GetSizeOfCCprQueue(); routingQueueIndex++)  /* FETA_COM_01 */
  {
    const Com_CprQueueBufferStartIdxOfCprQueueType startIdxOfBuffer = Com_GetCprQueueBufferStartIdxOfCprQueue(routingQueueIndex);

    Com_SetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);
    Com_SetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);

    Com_SetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);
    Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, startIdxOfBuffer);
  }
}

/**********************************************************************************************************************
 * Com_CprQueue_UpdateReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_UpdateReadIdx(Com_CCprQueueIterType routingQueueIndex)
{
  Com_SetCprQueueBufferReadIdxOfCprQueue(routingQueueIndex, Com_GetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex));
}

/**********************************************************************************************************************
 * Com_CprQueue_UpdateWriteIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_UpdateWriteIdx(Com_CCprQueueIterType routingQueueIndex)
{
  Com_SetCprQueueBufferWriteIdxOfCprQueue(routingQueueIndex, Com_GetCprQueueBufferPendingWriteIdxOfCprQueue(routingQueueIndex));
}
/**********************************************************************************************************************
 * Com_CprQueue_ResetNumberOfBytesRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_ResetNumberOfBytesRead(Com_CCprQueueIterType routingQueueIndex)
{
  Com_SetCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, 0u);
}

/**********************************************************************************************************************
 * Com_CprQueue_ReadFromCprQueueDirectly
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_CprQueue_ReadFromCprQueueDirectly(Com_CprQueueBufferIterType readIdx)
{
  return Com_GetCprQueueBuffer(readIdx);
}

/**********************************************************************************************************************
 * Com_CprQueue_Write
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_Write(Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx, uint32 index, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) data, uint8 header, uint8 sizeOfIndexType)
{
  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdxIntern = writeIdx;

  /* Write Header to queue */
  Com_CprQueue_WriteHeader(&writeIdxIntern, index, header, sizeOfIndexType);

  /* Write data to queue */
  if(data->dataPtr != NULL_PTR)
  {
    VStdLib_MemCpy(Com_GetAddrCprQueueBuffer(writeIdxIntern), data->dataPtr, data->dataLength);     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
  }
}

/**********************************************************************************************************************
 * Com_CprQueue_WriteHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_WriteHeader(CONSTP2VAR(Com_CprQueueBufferWriteIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) writeIdx, uint32 index, uint8 headerIdentifier, uint8 sizeOfIndexType)
{
  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdxIntern = *writeIdx;

  /* Write Header Identifier to queue */
  Com_SetCprQueueBuffer(writeIdxIntern, headerIdentifier);
  writeIdxIntern += COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE;

  /* Write index (size of datatype depends on configuration) to queue */
  switch(sizeOfIndexType)
  {
    /* uint8 */
    case 1:
    {
      const uint8 indexTmp = (uint8) index;
      VStdLib_MemCpy(Com_GetAddrCprQueueBuffer(writeIdxIntern), &indexTmp, sizeOfIndexType);     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      break;
    }
    /* uint16 */
    case 2:
    {
      const uint16 indexTmp = (uint16) index;
      VStdLib_MemCpy(Com_GetAddrCprQueueBuffer(writeIdxIntern), &indexTmp, sizeOfIndexType);     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      break;
    }
    /* uint32 */
    case 4:
    {
      const uint32 indexTmp = index;
      VStdLib_MemCpy(Com_GetAddrCprQueueBuffer(writeIdxIntern), &indexTmp, sizeOfIndexType);     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      break;
    }
    default:
      /* Intentionally Empty */
      break;
  }
  writeIdxIntern += sizeOfIndexType;

   /* Update WriteIdx*/
  *writeIdx = writeIdxIntern;
}

/**********************************************************************************************************************
 * Com_CprQueue_WriteData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueue_WriteData(CONSTP2VAR(Com_CprQueueBufferWriteIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) writeIdx, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) data)
{
  if(data->dataPtr != NULL_PTR)
  {
    Com_CprQueueBufferWriteIdxOfCprQueueType writeIdxIntern = *writeIdx;
    /* Write data to queue */
    VStdLib_MemCpy(Com_GetAddrCprQueueBuffer(writeIdxIntern), data->dataPtr, data->dataLength);     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
    writeIdxIntern += (Com_CprQueueBufferWriteIdxOfCprQueueType) data->dataLength;

    /* Update WriteIdx*/
    *writeIdx = writeIdxIntern;
  }
}
#endif


#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION
    
END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_CprQueue.c
 *********************************************************************************************************************/
