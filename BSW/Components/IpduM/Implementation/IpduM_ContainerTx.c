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
 *         \file  IpduM_ContainerTx.c
 *         \unit  ContainerTx
 *        \brief  Microsar IpduM Container source file
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

#define IPDUM_CONTAINERTX_SOURCE
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
#include "IpduM_ContainerTx.h"
#include "IpduM_ContainerTxRequestQueueHandling.h"
#include "IpduM_ContainerTxDataQueueHandling.h"
#include "IpduM_TxConfBuffer.h"
#include "SchM_IpduM.h"
#include "PduR_IpduM.h"

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

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerTx_Transmit()
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
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_Transmit(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  uint8 errorId = IPDUM_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  IpduM_PduLengthOfTxContainerPduType bufferInstanceLength = IpduM_GetPduLengthOfTxContainerPdu(IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId));
  PduLengthType pduLength = PduInfoPtr->SduLength;
  
/* *INDENT-OFF* */
  /* Long headers are 8 bytes, short headers are 4 bytes in size.
     Long headers have 4 bytes ID and 4 bytes DLC.
     Short headers have 3 bytes ID and 1 byte DLC. */
/* *INDENT-ON* */
  uint8 headerSize = IpduM_GetHeaderSizeOfTxContainerPdu(IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId));
  IpduM_PduLengthOfTxContainerPduType pduAndHeaderLength = (IpduM_PduLengthOfTxContainerPduType)pduLength + (IpduM_PduLengthOfTxContainerPduType)headerSize;

  if(pduAndHeaderLength > bufferInstanceLength)
  {
    errorId = IPDUM_E_PARAM;
  }
  else if((pduLength > 255u) && (headerSize == IPDUM_HEADERSIZE_SHORT))
  {
    /* For short headers, only 8 bits are available for DLC. So values >255 are not allowed here. */
    errorId = IPDUM_E_PARAM;
  }
  else
# endif
  {
    SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
    {
      if(IpduM_IsRequestQueuePrioIndirectionUsedOfTxContainerPdu(IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId)))
      {
        IpduM_ContainerTx_RequestQueueHandling_Transmit(txContainedId, PduInfoPtr);
      }
      if(IpduM_IsDataQueueInfoUsedOfTxContainerPdu(IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId)))
      {
        IpduM_ContainerTx_DataQueueHandling_Transmit(txContainedId, PduInfoPtr);
      }

      /* PDU is added to the PDU by here */
      retVal = E_OK;
    }
    SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  }

  if(errorId != IPDUM_E_NO_ERROR)
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_CONTAINERTX_TRANSMIT, errorId);  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_TriggerTransmit()
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
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_TriggerTransmit(IpduM_TxContainerPduIdxOfCTxLoInfoType txContainerId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  IpduM_PduLengthOfTxContainerPduType bufferInstanceLength = IpduM_GetPduLengthOfTxContainerPdu(txContainerId);

  if(PduInfoPtr->SduLength >= bufferInstanceLength)
  {
    if(IpduM_IsRequestQueuePrioIndirectionUsedOfTxContainerPdu(txContainerId))
    {
      retVal = IpduM_ContainerTx_RequestQueueHandling_TriggerTransmit(txContainerId, PduInfoPtr);
    }
    if(IpduM_IsDataQueueInfoUsedOfTxContainerPdu(txContainerId))
    {
      retVal = IpduM_ContainerTx_DataQueueHandling_TriggerTransmit(txContainerId, PduInfoPtr);
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
 *  IpduM_ContainerTx_TxConfirmation()
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
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_TxConfirmation(IpduM_CTxContainerPduIterType containerPduId, Std_ReturnType result)
{
  SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  {
    if(result == E_OK)
    {
      PduIdType txConfId;
      IpduM_ContainerTxConfirmationBufferLengthOfContainerTxConfirmationBufferInfoType sizeOfTxConfBuffer = IpduM_TxConfBuffer_GetSize(containerPduId);
      IpduM_ContainerTxConfirmationBufferLengthOfContainerTxConfirmationBufferInfoType iterationCounter = 0u;

      while(IpduM_TxConfBuffer_Get(containerPduId, &txConfId) == E_OK)  /* FETA_IPDUM_01 */
      {
        if(IpduM_IsTxConfirmationOfTxContainedPdu((IpduM_TxContainedPduIterType) txConfId))
        {
          PduR_IpduMTxConfirmation(IpduM_GetTxContainedPduRefOfTxContainedPdu((IpduM_TxContainedPduIterType) txConfId));
        }
      
        /* ensure the loop terminates latest after the whole buffer was read. */
        iterationCounter++;
        if(iterationCounter >= sizeOfTxConfBuffer)
        {
          break;
        }
      }
      /* Reset the buffer, as this is not implemented as a ring buffer. */
      IpduM_TxConfBuffer_Clear(containerPduId);
    }
    else
    {
      /* If the TxConfirmation timeout has elapsed, remove all stored TxConfirmation IDs. */
      IpduM_TxConfBuffer_Clear(containerPduId);
    }
  }
  SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
}

/*******************************************************************************************
 *  IpduM_ContainerTx_MainFunction()
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
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_MainFunction(IpduM_MainFunctionTxIndirectionIterType mainFunctionTxIndirectionIdx)
{
  IpduM_TxContainerPduIndStartIdxOfMainFunctionTxIndirectionType txContainerPduIndIdx = IpduM_GetTxContainerPduIndStartIdxOfMainFunctionTxIndirection(mainFunctionTxIndirectionIdx);
  IpduM_TxContainerPduIndEndIdxOfMainFunctionTxIndirectionType   txContainerPduIndEndIdx = IpduM_GetTxContainerPduIndEndIdxOfMainFunctionTxIndirection(mainFunctionTxIndirectionIdx);

  for(; txContainerPduIndIdx < txContainerPduIndEndIdx; txContainerPduIndIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_CTxContainerPduIterType txContainerId = IpduM_GetTxContainerPduInd(txContainerPduIndIdx);

    SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
    {
      if(IpduM_IsRequestQueuePrioIndirectionUsedOfTxContainerPdu(txContainerId))
      {
        IpduM_ContainerTx_RequestQueueHandling_MainFunction(txContainerId);
      }
      if(IpduM_IsDataQueueInfoUsedOfTxContainerPdu(txContainerId))
      {
        IpduM_ContainerTx_DataQueueHandling_MainFunction(txContainerId);
      }
    }
    SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTx.c
**********************************************************************************************************************/
