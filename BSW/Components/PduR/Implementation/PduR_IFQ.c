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
 *         \file  PduR_IFQ.c
 *         \unit  IFQ
 *        \brief  Pdu Router Interface FIFO Queue source file
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

#define PDUR_IFQ_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_IFQ.h"
#include "PduR_Reporting.h"

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
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetActualLengthOfInterfaceFifoQueueElementRamSafe
 *********************************************************************************************************************/
/*! \brief       Get the actual length of the latest Pdu in the FIFO queue.
 *  \details     -
 *  \param[in]   interfaceFifoQueueElementRamIdx    id of the interface FIFO queue element
 *  \param[in]   interfaceFifoQueueRomIdx           id of the interface FIFO queue
 *  \param[in]   memIdx                             memory section index
 *  \return      the actual length
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_ActualLengthOfInterfaceFifoQueueElementRamType, PDUR_CODE) PduR_IFQ_GetActualLengthOfInterfaceFifoQueueElementRamSafe(PduR_InterfaceFifoQueueElementRamIterType interfaceFifoQueueElementRamIdx,
                                                                                                                                             PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx,
                                                                                                                                             PduR_MemIdxType memIdx);
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe
 *********************************************************************************************************************/
/*! \brief       Gets the current read idx for the FIFO queue.
 *  \details     -
 *  \param[in]   interfaceFifoQueueRomIdx    id of the interface FIFO queue
 *  \param[in]   memIdx                      memory section index
 *  \return      the read index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_InterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRamType, PDUR_CODE) PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe
 *********************************************************************************************************************/
/*! \brief       Get the current write index for the FIFO queue.
 *  \details     -
 *  \param[in]   interfaceFifoQueueRomIdx    id of the interface FIFO queue
 *  \param[in]   memIdx                      memory section index
 *  \return      the write index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_InterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRamType, PDUR_CODE) PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_IsQueueFull
 *********************************************************************************************************************/
/*! \brief      Returns if the queue is full.
 *  \details    -
 *  \param[in]  interfaceFifoQueueRomIdx  id of the interface fifo queue
 *  \param[in]  memIdx                    memory section index
 *  \return     true                      queue is full
 *              false                     queue is not full
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_IFQ_IsQueueFull(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_IsQueueEmpty
 *********************************************************************************************************************/
/*! \brief      Returns if the queue is empty.
 *  \details    -
 *  \param[in]  interfaceFifoQueueRomIdx  id of the interface fifo queue
 *  \param[in]  memIdx                    memory section index
 *  \return     true                      queue is empty
 *              false                     queue is not empty
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_IFQ_IsQueueEmpty(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_IncrementReadIdx
 *********************************************************************************************************************/
/*! \brief      Increments the read Idx with wrap around.
 *  \details    -
 *  \param[in]  interfaceFifoQueueRomIdx  id of the interface fifo queue
 *  \param[in]  memIdx                    memory section index
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_IFQ_IncrementReadIdx(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

#if(PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_IncrementWriteIdx
 *********************************************************************************************************************/
/*! \brief      Increments the write Idx with wrap around.
 *  \details    -
 *  \param[in]  interfaceFifoQueueRomIdx  id of the interface fifo queue
 *  \param[in]  memIdx                    memory section index
 *  \pre        -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_IFQ_IncrementWriteIdx(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx);
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_IFQ_Init(PduR_MemIdxType memIdx)
{
  PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx;
  for(interfaceFifoQueueRomIdx = 0u; interfaceFifoQueueRomIdx < PduR_GetSizeOfInterfaceFifoQueueRom(memIdx); interfaceFifoQueueRomIdx++)  /* FETA_PDUR_01 */
  {
    PduR_InterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRomType startIdxOfQueue = PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);

    PduR_SetInterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, startIdxOfQueue, memIdx);
    PduR_SetInterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, startIdxOfQueue, memIdx);
    PduR_SetFifoFullOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, FALSE, memIdx);
  }
}
#endif

/**********************************************************************************************************************
 * PduR_IFQ_PutFifo
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
#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
FUNC(Std_ReturnType, PDUR_CODE) PduR_IFQ_PutFifo(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr)  /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(PduR_IFQ_IsQueueFull(interfaceFifoQueueRomIdx, memIdx))
  {
    retVal = E_NOT_OK;
  }
  else
  {
    PduR_InterfaceFifoQueueElementRamIterType startIdxOfQueue = PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);
    PduR_InterfaceFifoQueueElementRamIterType interfaceFifoQueueElementRamWriteIdx = PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe(interfaceFifoQueueRomIdx, memIdx);
    PduR_InterfaceFifoQueueArrayRamIterType maxPduLengthOfInterfaceFifoQueueRom = (PduR_InterfaceFifoQueueArrayRamIterType) PduR_GetMaxPduLengthOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);

    PduR_InterfaceFifoQueueArrayRamIterType writeOffset = ((PduR_InterfaceFifoQueueArrayRamIterType) interfaceFifoQueueElementRamWriteIdx - startIdxOfQueue) * maxPduLengthOfInterfaceFifoQueueRom;
    PduR_InterfaceFifoQueueArrayRamIterType interfaceFifoQueueArrayRamWriteIdx = ((PduR_InterfaceFifoQueueArrayRamIterType) PduR_GetInterfaceFifoQueueArrayRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx)) + writeOffset;

    if((interfaceFifoQueueArrayRamWriteIdx + maxPduLengthOfInterfaceFifoQueueRom) <= PduR_GetInterfaceFifoQueueArrayRamEndIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx))  /* COV_PDUR_ROBUSTNESS */
    {
      PduR_SetActualLengthOfInterfaceFifoQueueElementRam(interfaceFifoQueueElementRamWriteIdx, pduLength, memIdx);
      VStdLib_MemCpy_s(PduR_GetAddrInterfaceFifoQueueArrayRam(interfaceFifoQueueArrayRamWriteIdx, memIdx), (VStdLib_CntType) maxPduLengthOfInterfaceFifoQueueRom, sduDataPtr, pduLength);   /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* VCA_PDUR_IFQ_MEMCPY_S */
      PduR_IFQ_IncrementWriteIdx(interfaceFifoQueueRomIdx, memIdx);
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
      PduR_Reporting_ReportError(PDUR_IFQ_PUT, PDUR_E_FATAL);
    }
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
 * PduR_IFQ_IsQueueFull
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_IFQ_IsQueueFull(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)
{
  return PduR_IsFifoFullOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, memIdx);
}
#endif

/**********************************************************************************************************************
 * PduR_IFQ_IsQueueEmpty
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_IFQ_IsQueueEmpty(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)
{
  return (PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe(interfaceFifoQueueRomIdx, memIdx) == PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe(interfaceFifoQueueRomIdx, memIdx))
          && !PduR_IFQ_IsQueueFull(interfaceFifoQueueRomIdx, memIdx);
}
#endif

/**********************************************************************************************************************
 * PduR_IFQ_IncrementReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_IFQ_IncrementReadIdx(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)
{
  PduR_InterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRamType localReadIdx = PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe(interfaceFifoQueueRomIdx, memIdx);
  localReadIdx++;

  if(localReadIdx >= PduR_GetInterfaceFifoQueueElementRamEndIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx))
  {
    PduR_SetInterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx), memIdx);
  }
  else
  {
    PduR_SetInterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, localReadIdx, memIdx);
  }

  PduR_SetFifoFullOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, FALSE, memIdx);
}
#endif

/**********************************************************************************************************************
 * PduR_IFQ_IncrementWriteIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_IFQ_IncrementWriteIdx(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)
{
  PduR_InterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRamType localWriteIdx = PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe(interfaceFifoQueueRomIdx, memIdx);
  localWriteIdx++;

  if(localWriteIdx >= PduR_GetInterfaceFifoQueueElementRamEndIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx))
  {
    PduR_SetInterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx), memIdx);
  }
  else
  {
    PduR_SetInterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, localWriteIdx, memIdx);
  }

  if(PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe(interfaceFifoQueueRomIdx, memIdx) == PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe(interfaceFifoQueueRomIdx, memIdx))
  {
    PduR_SetFifoFullOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, TRUE, memIdx);
  }
}
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetNextElement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_IFQ_GetNextElement(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(PduR_IFQ_IsQueueEmpty(interfaceFifoQueueRomIdx, memIdx))
  {
    retVal = E_NOT_OK;
  }
  else
  {
    PduR_InterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRomType startIdxOfQueue = PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);
    PduR_InterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRamType interfaceFifoQueueElementRamReadIdx = PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe(interfaceFifoQueueRomIdx, memIdx);

    PduR_InterfaceFifoQueueArrayRamEndIdxOfInterfaceFifoQueueRomType offset = ((PduR_InterfaceFifoQueueArrayRamEndIdxOfInterfaceFifoQueueRomType) interfaceFifoQueueElementRamReadIdx - startIdxOfQueue) * PduR_GetMaxPduLengthOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);

    info->SduDataPtr = PduR_GetAddrInterfaceFifoQueueArrayRam(PduR_GetInterfaceFifoQueueArrayRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx)  + offset, memIdx);
    info->SduLength  = PduR_IFQ_GetActualLengthOfInterfaceFifoQueueElementRamSafe(interfaceFifoQueueElementRamReadIdx, interfaceFifoQueueRomIdx, memIdx);
    /* the stored DestPdu handle ID is static and does not need to be checked. */
    *rmDestRomIdx = PduR_GetRmDestRomIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);
    retVal = E_OK;
  }

  return retVal;
}
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetActualLengthOfInterfaceFifoQueueElementRamSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_ActualLengthOfInterfaceFifoQueueElementRamType, PDUR_CODE) PduR_IFQ_GetActualLengthOfInterfaceFifoQueueElementRamSafe(PduR_InterfaceFifoQueueElementRamIterType interfaceFifoQueueElementRamIdx,
                                                                                                                                             PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx,
                                                                                                                                             PduR_MemIdxType memIdx)
{
  PduR_ActualLengthOfInterfaceFifoQueueElementRamType actualLengthOfInterfaceFifoQueueElement = PduR_GetActualLengthOfInterfaceFifoQueueElementRam(interfaceFifoQueueElementRamIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduLengthType maxPduLength = PduR_GetMaxPduLengthOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);

  if(actualLengthOfInterfaceFifoQueueElement > maxPduLength)
  {
    actualLengthOfInterfaceFifoQueueElement = maxPduLength;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETACTUALLENGTHOFIFQ, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif

  PDUR_DUMMY_STATEMENT(interfaceFifoQueueRomIdx);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  return actualLengthOfInterfaceFifoQueueElement;
}
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_RemoveElement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_IFQ_RemoveElement(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)
{
  if(!PduR_IFQ_IsQueueEmpty(interfaceFifoQueueRomIdx, memIdx))
  {
    PduR_IFQ_IncrementReadIdx(interfaceFifoQueueRomIdx, memIdx);
  }
}
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_FlushQueue
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_IFQ_FlushQueue(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)
{
  PduR_InterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRomType startIdxOfQueue = PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);

  PduR_SetInterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, startIdxOfQueue, memIdx);
  PduR_SetInterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, startIdxOfQueue, memIdx);
  PduR_SetFifoFullOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, FALSE, memIdx);
}
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetFillLevel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(uint16, PDUR_CODE) PduR_IFQ_GetFillLevel(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  uint32 fillLevel;

  PduR_InterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRamType  readIdx         = PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe(interfaceFifoQueueRomIdx, memIdx);
  PduR_InterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRamType writeIdx        = PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe(interfaceFifoQueueRomIdx, memIdx);
  PduR_InterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRomType startIdxOfQueue = PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);
  PduR_InterfaceFifoQueueElementRamEndIdxOfInterfaceFifoQueueRomType   endIdxOfQueue   = PduR_GetInterfaceFifoQueueElementRamEndIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);

  if(readIdx < writeIdx)
  {
    fillLevel = (uint32) writeIdx - readIdx;
  }
  else if(writeIdx < readIdx)
  {
    fillLevel = ((uint32) endIdxOfQueue - readIdx) + ((uint32) writeIdx - startIdxOfQueue);
  }
  else /* (readIdx == writeIdx) */
  {
    if(PduR_IFQ_IsQueueFull(interfaceFifoQueueRomIdx, memIdx))
    {
      fillLevel = (uint32) endIdxOfQueue - startIdxOfQueue;
    }
    else
    {
      fillLevel = 0u;
    }
  }

  return (uint16) fillLevel;
}
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_InterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRamType, PDUR_CODE) PduR_IFQ_GetInterfaceFifoQueueElementRamReadIdxSafe(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)
{
  PduR_InterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRamType readIdx = PduR_GetInterfaceFifoQueueElementRamReadIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_InterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRomType startIdxOfQueue = PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);
  PduR_InterfaceFifoQueueElementRamEndIdxOfInterfaceFifoQueueRomType   endIdxOfQueue   = PduR_GetInterfaceFifoQueueElementRamEndIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);

  if(readIdx >= endIdxOfQueue)
  {
    readIdx = endIdxOfQueue - 1u;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETIFQREADIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
  if(readIdx < startIdxOfQueue)
  {
    readIdx = startIdxOfQueue;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETIFQREADIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif

  return readIdx;
}
#endif


#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_InterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRamType, PDUR_CODE) PduR_IFQ_GetInterfaceFifoQueueElementRamWriteIdxSafe(PduR_InterfaceFifoQueueRomIterType interfaceFifoQueueRomIdx, PduR_MemIdxType memIdx)
{
  PduR_InterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRamType writeIdx = PduR_GetInterfaceFifoQueueElementRamWriteIdxOfInterfaceFifoQueueRam(interfaceFifoQueueRomIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_InterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRomType startIdxOfQueue = PduR_GetInterfaceFifoQueueElementRamStartIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);
  PduR_InterfaceFifoQueueElementRamEndIdxOfInterfaceFifoQueueRomType   endIdxOfQueue   = PduR_GetInterfaceFifoQueueElementRamEndIdxOfInterfaceFifoQueueRom(interfaceFifoQueueRomIdx, memIdx);

  if(writeIdx >= endIdxOfQueue)
  {
    writeIdx = endIdxOfQueue - 1u;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETIFQWRITEIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
  if(writeIdx < startIdxOfQueue)
  {
    writeIdx = startIdxOfQueue;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETIFQWRITEIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif

  return writeIdx;
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_IFQ.c
 *********************************************************************************************************************/
