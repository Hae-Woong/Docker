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
/*!        \file  Com_DesGwCp.c
 *         \unit  DesGwCp
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_DESGWCP_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com.h"
#include "Com_DesGwCp.h"

#include "Com_Cfg_DescriptionGatewayData.h"
#include "Com_Cfg_CrossPartitionQueueData.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_DesGw.h"
#include "Com_CprQueueDescrGw.h"
#include "Com_GwTout.h"
#include "Com_ISRThreshold.h"
#include "Com_Reporting.h"
#include "Com_CheckUpdateBit.h"
#include "Com_Transmission.h"

/**********************************************************************************************************************
 * LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_DesGwCp_ProcessRoutingTx
**********************************************************************************************************************/
/*! \brief        This function handles the tx side of cross partition gateway description events and initiates the transmission if the conditions of
 *                the configured transfer properties are satisfied.
    \details      -
    \pre          -
    \param[in]    routingQueueIndex   Idx of the cross partition routing queue
    \param[in]    readIdx             Index to the position in the queue where the data shall be read
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
*****************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DesGwCp_ProcessRoutingTx(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx);

/**********************************************************************************************************************
 * LOCAL FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
  Com_DesGwCp_ProcessRoutingTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DesGwCp_ProcessRoutingTx(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  Com_TxPduInfoIterType ComTxPduId = 0u;

  boolean sendTrigger = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  boolean withoutRepetitionFlag = TRUE; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Com_SizeOfGwTxPduDescriptionInfoType gwTxPduIdx = 0u;

  ComDataTypeVar sourceBuffer = { NULL_PTR, 0u };

  /* Read data from queue */
  Com_CprQueueDescrGw_ReadData(routingQueueIndex, &gwTxPduIdx, &sourceBuffer, readIdx);

  if(gwTxPduIdx < Com_GetSizeOfGwTxPduDescriptionInfo())
  {
    Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx = Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx);
    ComTxPduId = Com_GetTxPduInfoIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx);

    /* Process all descriptions that correspond to the same destination txPdu */
    for(; gwDescrAccessIdx < Com_GetGwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfo(gwTxPduIdx); gwDescrAccessIdx++)  /* FETA_COM_01 */
    {
      /* Process Description Routing */
      CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) sourceBufferGwDescrAccess = (P2CONST(uint8, AUTOMATIC, AUTOMATIC))(sourceBuffer.dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      switch (Com_DesGw_ProcessDestinationDescription(sourceBufferGwDescrAccess, gwDescrAccessIdx))
      {
        case COM_TRANSMIT_TRIGGERWITHOUTREP:
          sendTrigger = TRUE;   /* Set transmit flag if at least one description fulfills the transmit condition */
          break;
        case COM_TRANSMIT_TRIGGER:
          sendTrigger = TRUE;
          withoutRepetitionFlag = FALSE;        /* Set repetition flag if at least one description requires repetitive transmit */
          break;
        case COM_TRANSMIT_TRIGGER_NOTFULFILLED:
        default: /* COV_COM_MISRA */
        /* Intentionally Empty */
          break;
      }
    }

    /* Handle Gw Timeout if configured */
    Com_GwTout_Event(ComTxPduId);

    /* If transmit condition is fulfilled (transmit flag is set) */
    if(sendTrigger)
    {
      const Com_ImmediateProcessingOfGwDescriptionAccessInfoType isImmediate = Com_IsImmediateProcessingOfGwDescriptionAccessInfo(Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx));
      /* Trigger the transmission of the PDU */
      Com_Transmission_DesGwTransmit(ComTxPduId, isImmediate, withoutRepetitionFlag);
    }
  }
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */

#endif

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_DesGwCp_ProcessQueuesTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGwCp_ProcessQueuesTx(Com_MainFunctionRouteSignalsStructIterType mainFunctionId,
                                                 Com_ISRThresholdInfoIterType gwDescriptionProcessingISRLockCounterIdx)
{
  /* If cross partition routing queues leading to this partition are used */
  if(Com_IsPartitionsTxStructUsedOfMainFunctionRouteSignalsStruct(mainFunctionId))
  {
    /* Iterate over cross partition queues leading to this partition */
    for(Com_CprQueueDescriptionRoutingIndIterType routingQueueIndIndex = Com_GetCprQueueDescriptionRoutingIndStartIdxOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId));
        routingQueueIndIndex < Com_GetCprQueueDescriptionRoutingIndEndIdxOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId)); routingQueueIndIndex++) /* FETA_COM_01 */
    {
      const Com_CCprQueueIterType routingQueueIndex = Com_GetCprQueueDescriptionRoutingInd(routingQueueIndIndex);
      Com_CprQueueBufferReadIdxOfCprQueueType readIdx = 0u;
      const Com_CprQueueBufferLengthOfCprQueueType readThreshold = Com_GetCprQueueBufferLengthOfCprQueue(routingQueueIndex);

      /* Reset NumberOfBytesRead */
      Com_CprQueueDescrGw_ResetNumberOfBytesRead(routingQueueIndex);

      /* While cross partition queue has stored elements, read data from queue */
      while(Com_CprQueueDescrGw_GetReadIdx(routingQueueIndex, &readIdx) == E_OK)  /* FETA_COM_02 */
      {
        if(Com_CprQueueDescrGw_ReadFromCprQueueDirectly(readIdx) == COM_CPRQUEUE_HEADERVALUE_DESCRIPTION)
        {
          /* Process cross partition description routing tx side */
          Com_DesGwCp_ProcessRoutingTx(routingQueueIndex, readIdx);

          /* Update the read index */
          Com_CprQueueDescrGw_UpdateReadIdx(routingQueueIndex);
        }
        else
        {
          /* Break from while loop in case there is no valid header */
          Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, readThreshold);
        }

        /* Abort processing if number of read bytes increases the the queue size */
        if(Com_GetCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex) > readThreshold)
        {
          break;
        }

        Com_ISRThreshold_ThresholdCheck(gwDescriptionProcessingISRLockCounterIdx);
      }
    }
  }
}

/**********************************************************************************************************************
  Com_DesGwCp_ProcessRoutingRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGwCp_ProcessRoutingRx(P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_GwTxPduDescriptionInfoIterType gwTxPduIdx)
{
  /* Process all descriptions that correspond to the same destination txPdu */
  boolean gwDescriptionUpdated = FALSE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  for(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx = Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx);
      gwDescrAccessIdx < Com_GetGwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfo(gwTxPduIdx); gwDescrAccessIdx++) /* FETA_COM_01 */
  {
    /*@ assert gwDescrAccessIdx < Com_GetSizeOfGwDescriptionAccessInfo(); */ /* VCA_COM_CSL03 */
    /* Check if the source description is fully contained in received buffer */
    if(PduInfoPtr->SduLength >= Com_GetValidDlcOfGwDescriptionAccessInfo(gwDescrAccessIdx))
    {
      /* Check if the source description was updated */
      if(Com_CheckUpdateBit_GwDescription(gwDescrAccessIdx, PduInfoPtr))
      {
        gwDescriptionUpdated = TRUE;
      }
    }
  }
  /* If any gateway description belonging to same source and destination Pdu was updated */
  if(gwDescriptionUpdated)
  {
    /* Write gateway description into Queue. If write is successful E_OK is returned, if write isn't successful E_NOT_OK is returned and DET is called. */
    CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &(PduInfoPtr->SduDataPtr[Com_GetSourceMinStartBytePositionOfGwTxPduDescriptionInfo(gwTxPduIdx)]);
    const PduLengthType constDataLength = Com_GetSourceMaxLengthOfGwTxPduDescriptionInfo(gwTxPduIdx);
    const ComDataTypeConst descriptionroutingInfoPtr = { constDataPtr, constDataLength };

    if(E_OK != Com_CprQueueDescrGw_WriteData(Com_GetCprQueueIdxOfGwTxPduDescriptionInfo(gwTxPduIdx), (Com_SizeOfGwTxPduDescriptionInfoType) gwTxPduIdx, &descriptionroutingInfoPtr))
    {
      Com_Reporting_CallDetReportError(COMServiceId_CprQueueDescrGw_Write, COM_E_CPRQUEUE_OVERFLOW);
    }
  }
}
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  END OF FILE: Com_DesGwCp.c
 *********************************************************************************************************************/
