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
/*!        \file  Com_CprQueueDescrGw.c
 *         \unit  CprQueueDescrGw
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_CPRQUEUEDESCRGW_SOURCE

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
#include "Com_CprQueueDescrGw.h"
#include "Com_Cfg_CrossPartitionQueueData.h"
#include "Com_Cfg_DescriptionGatewayData.h"

#include "Com_CprQueue.h"

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
 * Com_CprQueueDescrGw_CopyData
 *********************************************************************************************************************/
/*! \brief       This function copies the parameter from the queue starting at the readIdx position.
 *  \details     -
 *  \param[in]   routingQueueIndex   Index to the queue
 *  \param[in]   readIdx             Index to the position in the queue where the data shall be read
 *  \param[out]  gwTxPduIdx          Handle Id of the DescriptionRouting to be copied
 *  \param[out]  data                Pointer to the pointer to the data to be copied and the length
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueDescrGw_CopyData(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                                      CONSTP2VAR(Com_SizeOfGwTxPduDescriptionInfoType, AUTOMATIC, AUTOMATIC) gwTxPduIdx, CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data);

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_GetHeaderSize
 *********************************************************************************************************************/
/*! \brief       This function returns the size of the header which is put in front of the DescriptionRouting Payload in the CrossPartitionRoutingQueue.
 *  \details     -
 *  \return      the header size
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueDescrGw_GetHeaderSize(void);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_CopyData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueDescrGw_CopyData(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                                      CONSTP2VAR(Com_SizeOfGwTxPduDescriptionInfoType, AUTOMATIC, AUTOMATIC) gwTxPduIdx, CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data)
{
  Com_CprQueueBufferReadIdxOfCprQueueType readIdxTmp = readIdx;
  /* Skip Header */
  readIdxTmp += COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE;

  const uint8 sizeOfIndexType = sizeof(Com_SizeOfGwTxPduDescriptionInfoType);

  VStdLib_MemCpy(gwTxPduIdx, Com_GetAddrCprQueueBuffer(readIdxTmp), sizeOfIndexType);
  readIdxTmp += sizeOfIndexType;

  /* Return pointer pointing to payload */
  data->dataLength = Com_GetSourceMaxLengthOfGwTxPduDescriptionInfo(*gwTxPduIdx);
  data->dataPtr = Com_GetAddrCprQueueBuffer(readIdxTmp);
  readIdxTmp += (Com_CprQueueBufferReadIdxOfCprQueueType) data->dataLength; /* PRQA S 2985 */ /* MD_Com_2985_2986 */

  /* Set the returned ReadIdx as pending ReadIdx: the readIdx will be set to this value once the read is finished */
  Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, readIdxTmp);
}

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_GetHeaderSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueDescrGw_GetHeaderSize(void)
{
  return COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE + sizeof(Com_SizeOfGwTxPduDescriptionInfoType);
}
#endif


/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
 * Com_CprQueueDescrGw_ReadData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueDescrGw_ReadData(Com_CCprQueueIterType routingQueueIndex, CONSTP2VAR(Com_SizeOfGwTxPduDescriptionInfoType, AUTOMATIC, AUTOMATIC) gwTxPduIdx,
                                                  CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data, Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  /* Read data from queue */
  Com_CprQueueDescrGw_CopyData(routingQueueIndex, readIdx, gwTxPduIdx, data);

  /* Increase the number of bytes read */
  const Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType cprQueueBufferNumberOfBytesReadIdx = (Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType) Com_CprQueueDescrGw_GetHeaderSize() + (Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType) data->dataLength;
  Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, cprQueueBufferNumberOfBytesReadIdx);
}

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_WriteData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueDescrGw_WriteData(Com_CCprQueueIterType routingQueueIndex, Com_SizeOfGwTxPduDescriptionInfoType gwTxPduIdx,
                                                             CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) data)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx = 0u;

  /* If data fits into the CrossPartitionRoutingQueue, serialize data, update write index and return E_OK */
  if(Com_CprQueue_GetWriteIdx(routingQueueIndex, &writeIdx, data->dataLength + Com_CprQueueDescrGw_GetHeaderSize()) == E_OK)
  {
    Com_CprQueue_Write(writeIdx, (uint32) gwTxPduIdx, data, COM_CPRQUEUE_HEADERVALUE_DESCRIPTION, sizeof(Com_SizeOfGwTxPduDescriptionInfoType));
    /* Update the WriteIdx */
    Com_CprQueue_UpdateWriteIdx(routingQueueIndex);
    retVal = E_OK;
  }
  /* Otherwise return E_NOT_OK */
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_UpdateReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueDescrGw_UpdateReadIdx(Com_CCprQueueIterType routingQueueIndex)
{
  Com_CprQueue_UpdateReadIdx(routingQueueIndex);
}

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_GetReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueDescrGw_GetReadIdx(Com_CCprQueueIterType routingQueueIndex, CONSTP2VAR(Com_CprQueueBufferReadIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) readIdx)
{
  return Com_CprQueue_GetReadIdx(routingQueueIndex, readIdx);
}

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_ReadFromCprQueueDirectly
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_CprQueueDescrGw_ReadFromCprQueueDirectly(Com_CprQueueBufferIterType readIdx)
{
  return Com_CprQueue_ReadFromCprQueueDirectly(readIdx);
}

/**********************************************************************************************************************
 * Com_CprQueueDescrGw_ResetNumberOfBytesRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueDescrGw_ResetNumberOfBytesRead(Com_CCprQueueIterType routingQueueIndex)
{
  Com_CprQueue_ResetNumberOfBytesRead(routingQueueIndex);
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
