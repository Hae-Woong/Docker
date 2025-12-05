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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  IpduM_ContainerTxRequestQueue.c
 *         \unit  ContainerTxRequestQueue
 *        \brief  Microsar IpduM Container Tx Request Queue source file
 *      \details  This is the implementation of the MICROSAR IpduM module.
 *                The basic software module is based on the AUTOSAR IpduM specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define IPDUM_CONTAINERTXREQUESTQUEUE_SOURCE
/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_IpduM_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "IpduM.h"
#include "IpduM_Reporting.h"
#include "IpduM_ContainerTxRequestQueue.h"

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
  LOCAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_ContainsPdu()
*******************************************************************************************/
/*! \brief        Checks if a Pdu is already queued in the request queue.
 *  \details      -
 *  \param[in]    requestQueueInfoIdx
 *  \param[in]    txContainedId
 *  \return       TRUE if the PDU is in the container
 *  \return       FALSE if the PDU is not in the container
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_ContainsPdu(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx, IpduM_TxContainedPduIterType txContainedId);

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_ContainsPdu()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
IPDUM_LOCAL_INLINE FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_ContainsPdu(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx, IpduM_TxContainedPduIterType txContainedId)
{
  boolean retVal = FALSE;
  
  IpduM_RequestQueueBufferStartIdxOfRequestQueueInfoType requestQueueBufferStartIdx = IpduM_GetRequestQueueBufferStartIdxOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_RequestQueueBufferLengthOfRequestQueueInfoType requestQueueBufferLength = IpduM_GetRequestQueueBufferLengthOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_ReadIdxOfRequestQueueInfoType readIdxIterator = IpduM_GetReadIdxOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_ReadIdxOfRequestQueueInfoType writeIdx = IpduM_GetWriteIdxOfRequestQueueInfo(requestQueueInfoIdx);
  
  while(readIdxIterator != writeIdx) /* FETA_IPDUM_04 */
  {
    if(IpduM_GetTxContainedPduIdxOfRequestQueueBuffer((IpduM_RequestQueueBufferIterType) requestQueueBufferStartIdx + (IpduM_RequestQueueBufferIterType) readIdxIterator) == txContainedId)
    {
      retVal = TRUE;
      break;
    }
    readIdxIterator++;
    if(readIdxIterator >= requestQueueBufferLength)
    {
      readIdxIterator = 0u;
    }
  }
  return retVal;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_Init()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_Init(void)
{
  IpduM_CRequestQueueInfoIterType requestQueueInfoIdx = 0u;

  for(; requestQueueInfoIdx < IpduM_GetSizeOfCRequestQueueInfo(); requestQueueInfoIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_SetReadIdxOfRequestQueueInfo(requestQueueInfoIdx, 0u);
    IpduM_SetReadTmpIdxOfRequestQueueInfo(requestQueueInfoIdx, 0u);
    IpduM_SetWriteIdxOfRequestQueueInfo(requestQueueInfoIdx, 0u);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_Remove
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_Remove(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx)
{
  IpduM_ReadIdxOfRequestQueueInfoType readIdx = IpduM_GetReadIdxOfRequestQueueInfo(requestQueueInfoIdx);

  readIdx++;
  if(readIdx >= IpduM_GetRequestQueueBufferLengthOfRequestQueueInfo(requestQueueInfoIdx))
  {
    readIdx = 0u;
  }

  IpduM_SetReadIdxOfRequestQueueInfo(requestQueueInfoIdx, readIdx);
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_Peek
*******************************************************************************************/
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
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_Peek(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx, P2VAR(IpduM_TxContainedPduIdxOfRequestQueueBufferType, AUTOMATIC, IPDUM_APPL_DATA) containedPduId, P2VAR(PduLengthType, AUTOMATIC, IPDUM_APPL_DATA) containedPduLength)
{
  Std_ReturnType retVal = E_NOT_OK;

  IpduM_ReadIdxOfRequestQueueInfoType readIdx = IpduM_GetReadIdxOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_WriteIdxOfRequestQueueInfoType writeIdx = IpduM_GetWriteIdxOfRequestQueueInfo(requestQueueInfoIdx);

  IpduM_RequestQueueBufferStartIdxOfRequestQueueInfoType requestQueueBufferStartIdx = IpduM_GetRequestQueueBufferStartIdxOfRequestQueueInfo(requestQueueInfoIdx);

  if(readIdx != writeIdx)
  {
    IpduM_TxContainedPduIdxOfRequestQueueBufferType localContainedPduId = IpduM_GetTxContainedPduIdxOfRequestQueueBuffer((IpduM_RequestQueueBufferIterType) requestQueueBufferStartIdx + (IpduM_RequestQueueBufferIterType) readIdx);

    if(localContainedPduId < IpduM_GetSizeOfTxContainedPdu())
    {
      *containedPduId = localContainedPduId;
      *containedPduLength = IpduM_GetDlcOfRequestQueueBuffer((IpduM_RequestQueueBufferIterType) requestQueueBufferStartIdx + (IpduM_RequestQueueBufferIterType) readIdx);
      retVal = E_OK;
    }
    else
    {
      IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERTX_REQUESTQUEUE_PEEK, IPDUM_E_GENERATED_DATA);
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_IsEmpty()
*******************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_IsEmpty(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx)
{
  IpduM_ReadIdxOfRequestQueueInfoType readIdx = IpduM_GetReadIdxOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_ReadIdxOfRequestQueueInfoType writeIdx = IpduM_GetWriteIdxOfRequestQueueInfo(requestQueueInfoIdx);

  /* If read and write Idx are the same, this queue is empty */
  return (readIdx == writeIdx);
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_CalculateSize()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(PduLengthType, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_CalculateSize(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx, PduLengthType headerSize)
{
  PduLengthType sizeOfData = 0u;
  IpduM_RequestQueueBufferStartIdxOfRequestQueueInfoType requestQueueBufferStartIdx = IpduM_GetRequestQueueBufferStartIdxOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_RequestQueueBufferLengthOfRequestQueueInfoType requestQueueBufferLength = IpduM_GetRequestQueueBufferLengthOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_ReadIdxOfRequestQueueInfoType readIdxIterator = IpduM_GetReadIdxOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_ReadIdxOfRequestQueueInfoType writeIdx = IpduM_GetWriteIdxOfRequestQueueInfo(requestQueueInfoIdx);
  
  while(readIdxIterator != writeIdx) /* FETA_IPDUM_04 */
  {
    sizeOfData += IpduM_GetDlcOfRequestQueueBuffer((IpduM_RequestQueueBufferIterType) requestQueueBufferStartIdx + (IpduM_RequestQueueBufferIterType) readIdxIterator);
    sizeOfData += headerSize;

    readIdxIterator++;
    if(readIdxIterator >= requestQueueBufferLength)
    {
      readIdxIterator = 0u;
    }
  }
  return sizeOfData;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_Put()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_Put(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx, IpduM_TxContainedPduIterType txContainedId, PduLengthType sduLength)
{
  IpduM_RequestQueueBufferStartIdxOfRequestQueueInfoType requestQueueBufferStartIdx = IpduM_GetRequestQueueBufferStartIdxOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_RequestQueueBufferLengthOfRequestQueueInfoType requestQueueBufferLength = IpduM_GetRequestQueueBufferLengthOfRequestQueueInfo(requestQueueInfoIdx);
  IpduM_ReadIdxOfRequestQueueInfoType writeIdx = IpduM_GetWriteIdxOfRequestQueueInfo(requestQueueInfoIdx);
  
  if(IpduM_ContainerTx_RequestQueue_ContainsPdu(requestQueueInfoIdx, txContainedId) != TRUE)
  {
    IpduM_RequestQueueBufferIterType requestQueueBufferWriteIdx = (IpduM_RequestQueueBufferIterType) requestQueueBufferStartIdx + (IpduM_RequestQueueBufferIterType) writeIdx;

    if(requestQueueBufferWriteIdx < IpduM_GetSizeOfRequestQueueBuffer()) /* COV_IPDUM_DEFENSIVE_PROGRAMMING */
    {
      /* add PDU to queue */
      IpduM_SetTxContainedPduIdxOfRequestQueueBuffer(requestQueueBufferWriteIdx, (IpduM_TxContainedPduIdxOfRequestQueueBufferType)txContainedId);
      IpduM_SetDlcOfRequestQueueBuffer(requestQueueBufferWriteIdx, sduLength);
      writeIdx++;
      if(writeIdx >= requestQueueBufferLength)
      {
        writeIdx = 0u;
      }
      IpduM_SetWriteIdxOfRequestQueueInfo(requestQueueInfoIdx, writeIdx);
    }
    else
    {
      IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERTX_REQUESTQUEUE_ADDPDU, IPDUM_E_GENERATED_DATA);
    }
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_RestoreReadIDs
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_RestoreReadIDs(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx)
{
  IpduM_SetReadIdxOfRequestQueueInfo(requestQueueInfoIdx, IpduM_GetReadTmpIdxOfRequestQueueInfo(requestQueueInfoIdx));
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_SetRestorePoint
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_SetRestorePoint(IpduM_CRequestQueueInfoIterType requestQueueInfoIdx)
{
  IpduM_SetReadTmpIdxOfRequestQueueInfo(requestQueueInfoIdx, IpduM_GetReadIdxOfRequestQueueInfo(requestQueueInfoIdx));
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxRequestQueue.c
**********************************************************************************************************************/
