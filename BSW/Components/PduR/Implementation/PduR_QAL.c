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
 *         \file  PduR_QAL.c
 *         \unit  QAL
 *        \brief  Pdu Router Queue Abstraction Layer source file
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

#define PDUR_QAL_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_QAL.h"
#include "PduR_SingleBuffer.h"
#include "PduR_Fm.h"
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

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * PduR_QAL_Put
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
FUNC(Std_ReturnType, PDUR_CODE) PduR_QAL_Put(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_RmDestRomIterType rmDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_RmBufferedIfPropertiesRomIdxOfRmGDestRomType          rmBufferedIfPropertiesRomIdx = PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType memIdx                       = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

  {
# if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
    if(PduR_IsSingleBufferRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
    {
      retVal = PduR_SingleBuffer_Put(PduR_GetSingleBufferRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx), memIdx, pduLength, sduDataPtr);
    }
    else
# endif
# if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
    if(PduR_IsInterfaceFifoQueueRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
    {
      retVal = PduR_IFQ_PutFifo(PduR_GetInterfaceFifoQueueRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx), memIdx, pduLength, sduDataPtr);
    }
    else
# endif
# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
    if(PduR_IsFmFifoRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))  /* COV_PDUR_MISRA */
    {
      PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedIfPropertiesRom(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx), memIdx);

      retVal = PduR_Fm_PutFifo(fmFifoRomIdx, memIdx, rmDestRomIdx, pduLength, sduDataPtr);
    }
    else
# endif
    {
      PduR_Reporting_ReportError(PDUR_FCT_QAL_PUT, PDUR_E_QAL_UNSUPPORTEDAPI);
      retVal = E_NOT_OK;
    }
  }

  PDUR_DUMMY_STATEMENT(rmBufferedIfPropertiesRomIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(memIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_Get
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_QAL_Get(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_RmBufferedIfPropertiesRomIdxOfRmGDestRomType          rmBufferedIfPropertiesRomIdx = PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType memIdx                       = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

# if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsSingleBufferRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
  {
    retVal = PduR_SingleBuffer_Get(PduR_GetSingleBufferRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx), memIdx, rmDestRomIdx, info);
  }
  else
# endif
# if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsInterfaceFifoQueueRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
  {
    retVal = PduR_IFQ_GetNextElement(PduR_GetInterfaceFifoQueueRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx), memIdx, rmDestRomIdx, info);
  }
  else
# endif
# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsFmFifoRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))  /* COV_PDUR_MISRA */
  {
    PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx);

    retVal = PduR_Fm_GetNextElement(fmFifoRomIdx, memIdx, rmDestRomIdx, info);
  }
  else
# endif
  {
    PduR_Reporting_ReportError(PDUR_FCT_QAL_GET, PDUR_E_QAL_UNSUPPORTEDAPI);
    retVal = E_NOT_OK;
  }

  PDUR_DUMMY_STATEMENT(rmBufferedIfPropertiesRomIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(memIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_Remove
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_QAL_Remove(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_RmBufferedIfPropertiesRomIdxOfRmGDestRomType          rmBufferedIfPropertiesRomIdx = PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType memIdx                       = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

# if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsSingleBufferRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
  {
    PduR_Reporting_ReportError(PDUR_FCT_QAL_REMOVE, PDUR_E_QAL_UNSUPPORTEDAPI);
  }
  else
# endif
# if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsInterfaceFifoQueueRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
  {
    PduR_IFQ_RemoveElement(PduR_GetInterfaceFifoQueueRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx), memIdx);
  }
  else
# endif
# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsFmFifoRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))  /* COV_PDUR_MISRA */
  {
    PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx);

    PduR_Fm_RemoveOldestElement(fmFifoRomIdx, memIdx);
  }
  else
# endif
  {
    PduR_Reporting_ReportError(PDUR_FCT_QAL_REMOVE, PDUR_E_QAL_UNSUPPORTEDAPI);
  }

  PDUR_DUMMY_STATEMENT(rmBufferedIfPropertiesRomIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(memIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_GetFillLevel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(uint16, PDUR_CODE) PduR_QAL_GetFillLevel(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  uint16 retVal = 0;
  PduR_RmBufferedIfPropertiesRomIdxOfRmGDestRomType          rmBufferedIfPropertiesRomIdx = PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType memIdx                       = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

# if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsSingleBufferRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
  {
    PduR_Reporting_ReportError(PDUR_FCT_QAL_GETFILLLEVEL, PDUR_E_QAL_UNSUPPORTEDAPI);
    retVal = 0;
  }
  else
# endif
# if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsInterfaceFifoQueueRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
  {
    retVal = PduR_IFQ_GetFillLevel(PduR_GetInterfaceFifoQueueRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx), memIdx);
  }
  else
# endif
# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsFmFifoRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))  /* COV_PDUR_MISRA */
  {
    PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx);

    retVal = PduR_Fm_GetFillLevelSafe(fmFifoRomIdx, memIdx);
  }
  else
# endif
  {
    PduR_Reporting_ReportError(PDUR_FCT_QAL_GETFILLLEVEL, PDUR_E_QAL_UNSUPPORTEDAPI);
    retVal = 0;
  }

  PDUR_DUMMY_STATEMENT(rmBufferedIfPropertiesRomIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(memIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_QAL_Flush
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_QAL_Flush(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_RmBufferedIfPropertiesRomIdxOfRmGDestRomType          rmBufferedIfPropertiesRomIdx = PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType memIdx                       = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

# if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsSingleBufferRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
  {
    PduR_Reporting_ReportError(PDUR_FCT_QAL_FLUSH, PDUR_E_QAL_UNSUPPORTEDAPI);
  }
  else
# endif
# if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsInterfaceFifoQueueRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))
  {
    PduR_IFQ_FlushQueue(PduR_GetInterfaceFifoQueueRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx), memIdx);
  }
  else
# endif
# if (PDUR_FMFIFORAM == STD_ON)  /* COV_PDUR_VAR_QAL_QUEUE_TYPES */
  if(PduR_IsFmFifoRomUsedOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx))  /* COV_PDUR_MISRA */
  {
    PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedIfPropertiesRom(rmBufferedIfPropertiesRomIdx, memIdx);

    PduR_Fm_FlushFiFo(fmFifoRomIdx, memIdx);
  }
  else
# endif
  {
    PduR_Reporting_ReportError(PDUR_FCT_QAL_FLUSH, PDUR_E_QAL_UNSUPPORTEDAPI);
  }

  PDUR_DUMMY_STATEMENT(rmBufferedIfPropertiesRomIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(memIdx);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_QAL.c
 *********************************************************************************************************************/
