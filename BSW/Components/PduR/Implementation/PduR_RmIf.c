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
 *         \file  PduR_RmIf.c
 *         \unit  RmIf
 *        \brief  Pdu Router Routing Manager Interface source file
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

#define PDUR_RMIF_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_McQ.h"
#include "PduR_Mm.h"
#include "PduR_RmIf.h"
#include "PduR_RmIf_QueueProcessing.h"
#include "PduR_RPG.h"
#include "PduR_MultipleSourceHandler.h"
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
 * PduR_RmIf_CheckInfoPointer
**********************************************************************************************************************/
/*! \brief       This function checks the validity of the PduInfoType pointer and reports an error if it is a NULL_PTR.
 *  \details     -
 *  \param[in]   info              Payload information of the received I-PDU (pointer to data and data length).
 *  \param[in]   apiId             Id of the API. This Id will be reported as an error.
 *  \return      Std_ReturnType    E_OK:     Check passed
 *                                 E_NOT_OK: Null pointer detected
 *  \pre         -
**********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_CheckInfoPointer(P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, uint8 apiId);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * PduR_RmIf_DispatchTriggerTransmit
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
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_DispatchTriggerTransmit(PduR_RmDestRomIterType rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

# if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  if(PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
  {
    retVal = PduR_RmIf_QueueProcessing_TriggerTransmit(rmDestRomIdx, info);
  }
  else
# endif
  if(PduR_IsTriggerTransmitSupportedOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)))
  {
    PduR_RmSrcRomIdxOfRmDestRomType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);
    retVal = PduR_Mm_CallIfTriggerTransmit(PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx), info);
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_RMIF_MULTIPLESOURCEHANDLER_TRIGGERTRANSMIT, PDUR_E_PDU_ID_INVALID);
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmIf_DispatchTxConfirmation
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
FUNC(void, PDUR_CODE) PduR_RmIf_DispatchTxConfirmation(PduR_RmDestRomIterType rmDestRomIdx)
{
  PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
  PduR_RmSrcRomIdxOfRmDestRomType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);

  /* #30 If upper layer requires a Tx Confirmation */
  if(PduR_IsTxConfirmationSupportedOfRmSrcRom(rmSrcRomIdx))
  {
    switch (PduR_GetPduRDestPduProcessingOfRmGDestRom(rmGDestRomIdx))
    {
        /* #40 If Processing type is immediate, call upper layer IF confirmation */
      case PDUR_IMMEDIATE_PDURDESTPDUPROCESSINGOFRMGDESTROM:
        PduR_Mm_CallIfTxConfirmation(PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx));
        break;
# if (PDUR_MCQBUFFERROM == STD_ON)
        /* #50 Otherwise if processing is of type DEFERRED and routing type is not IF-buffered */
      case PDUR_DEFERRED_PDURDESTPDUPROCESSINGOFRMGDESTROM:
        if(!PduR_IsQueuedOfRmDestRom(rmDestRomIdx))
        {
          /* Destination application in this context is the application of the SrcPdu */
          PduR_DestApplicationManagerRomIdxOfRmSrcRomType destApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx));
          PduR_DestApplicationManagerRomIdxOfRmGDestRomType srcApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(rmGDestRomIdx);
          PduR_SrcApplicationRomIterType dest2srcApplicationRomIdx = PduR_GetSrcApplicationRomStartIdxOfDestApplicationManagerRom(destApplicationIdx) + (PduR_SrcApplicationRomIterType) srcApplicationIdx;

          /* #60 Write Tx Confirmation into multicore queue if queue is not full, otherwise call DET */
          if(PduR_McQ_WriteTxConfirmation(PduR_GetMcQBufferRomIdxOfSrcApplicationRom(dest2srcApplicationRomIdx), rmDestRomIdx, PduR_GetMcQBufferRomPartitionIdxOfSrcApplicationRom(dest2srcApplicationRomIdx)) == E_NOT_OK)
          {
            PduR_Reporting_ReportError(PDUR_FCT_RMIF_TXCONF, PDUR_E_MCQ_QUEUE_OVERFLOW);
          }
        }
        break;
# endif
      default:     /* COV_PDUR_MISRA */
        /* Nothing to do: This should never be reached */
        break;
    }
  }

# if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  /* #70 If Routing Type is IF-Buffered, indicate the complete transmission */
  if(PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
  {
    PduR_RmIf_QueueProcessing_TxConfirmation(rmGDestRomIdx);
  }
# endif
}

/**********************************************************************************************************************
 * PduR_RmIf_RoutePdu
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
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_RoutePdu(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ----------------------------------------------- */
  PduR_RmDestRomIterType rmDestRomIdx;

  /* For each destination of 1:N Routing */
  for(rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx < PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx++)  /* FETA_PDUR_01 */
  {
    if(PduR_RPG_IsDestPduEnabled(rmDestRomIdx) == TRUE)
    {
# if (PDUR_MCQBUFFERROM == STD_ON)
      PduR_DestApplicationManagerRomIdxOfRmGDestRomType destApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));
      PduR_DestApplicationManagerRomIdxOfRmSrcRomType srcApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx));

      /* In IF-cross-core routing use case, write data into multicore queue and return E_OK. Call DET, if queue is already full and return E_NOT_OK */
      if((srcApplicationIdx != destApplicationIdx) && (!PduR_IsQueuedOfRmDestRom(rmDestRomIdx)))
      {
        PduR_SrcApplicationRomIterType dest2srcApplicationRomIdx = PduR_GetSrcApplicationRomStartIdxOfDestApplicationManagerRom(destApplicationIdx) + (PduR_SrcApplicationRomIterType) srcApplicationIdx;

        if(PduR_McQ_WriteData(PduR_GetMcQBufferRomIdxOfSrcApplicationRom(dest2srcApplicationRomIdx),
                              (PduR_RmDestRomEndIdxOfRmSrcRomType) rmDestRomIdx,
                              info,
                              PduR_GetMcQBufferRomPartitionIdxOfSrcApplicationRom(dest2srcApplicationRomIdx)) == E_OK)
        {
          retVal &= E_OK;
        }
        else
        {
          PduR_Reporting_ReportError(PDUR_FCT_RMIF_ROUTE, PDUR_E_MCQ_QUEUE_OVERFLOW);
          retVal &= E_NOT_OK;
        }
      }
      /* otherwise, call destination Indication function and return it's result */
      else
# endif
      {
        retVal &= PduR_RmIf_ProcessDestPdu(rmDestRomIdx, info);
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmIf_ProcessDestPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_ProcessDestPdu(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  switch (PduR_GetPduLengthHandlingStrategyOfRmDestRom(rmDestRomIdx))
  {
    case PDUR_SHORTEN_PDULENGTHHANDLINGSTRATEGYOFRMDESTROM:
      {
        if (PduR_RmIf_CheckInfoPointer(info, PDUR_FCT_RMIF_ROUTE) == E_OK)
        {
          PduLengthType maxPduLength = PduR_GetMaxPduLengthOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));
          PduInfoType destPduInfo;
          destPduInfo.SduDataPtr = info->SduDataPtr;
          destPduInfo.SduLength  = (info->SduLength > maxPduLength) ? maxPduLength : info->SduLength;

          retVal = PduR_RmIf_DispatchDestPdu(rmDestRomIdx, &destPduInfo);
        }
      }
      break;
    case PDUR_IGNORE_PDULENGTHHANDLINGSTRATEGYOFRMDESTROM:
      {
        retVal = PduR_RmIf_DispatchDestPdu(rmDestRomIdx, info);
      }
      break;
    case PDUR_DISCARD_PDULENGTHHANDLINGSTRATEGYOFRMDESTROM:
      if (PduR_RmIf_CheckInfoPointer(info, PDUR_FCT_RMIF_ROUTE) == E_OK)
      {
        if (info->SduLength <= PduR_GetMaxPduLengthOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))
        {
          retVal = PduR_RmIf_DispatchDestPdu(rmDestRomIdx, info);
        }
      }
      break;
    default:   /* COV_PDUR_MISRA */
      /* Nothing to do: This should never be reached */
      break;
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmIf_DispatchDestPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_DispatchDestPdu(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
  PduR_RmSrcRomIdxOfRmDestRomType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);

# if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  if(PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
  {
    retVal = PduR_RmIf_QueueProcessing_ProcessBufferedRouting(rmSrcRomIdx, rmDestRomIdx, rmGDestRomIdx, info);
  }
  else
# endif
  {
    switch (PduR_GetDirectionOfRmGDestRom(rmGDestRomIdx))
    {
      case PDUR_TX_DIRECTIONOFRMGDESTROM:
      {
        retVal = PduR_MultipleSourceHandler_IfTransmit(rmDestRomIdx, info);

        /* Report an error for unqueued Gateway-Routings if transmission fails. */
        if((retVal == E_NOT_OK) && (PduR_GetDirectionOfRmSrcRom(rmSrcRomIdx) == PDUR_RX_DIRECTIONOFRMSRCROM))
        {
          PduR_Reporting_ReportError(PDUR_FCT_RMIF_FQ, PDUR_E_PDU_INSTANCES_LOST);
        }

        break;
      }
      case PDUR_RX_DIRECTIONOFRMGDESTROM:
      {
        PduR_Mm_CallIfRxIndication(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), info);
        retVal = E_OK;
        break;
      }
      default:       /* COV_PDUR_MISRA */
        /* Will never be reached */
        break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmIf_IsRmDestRomIdxUnqueuedIFCrossPartition
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmIf_IsRmDestRomIdxUnqueuedIFCrossPartition(PduR_RmDestRomIterType rmDestRomIdx, PduR_DestApplicationManagerRomIterType srcApplicationIdx, PduR_DestApplicationManagerRomIterType destApplicationIdx)
{
  boolean retVal = FALSE;

  if(rmDestRomIdx < PduR_GetSizeOfRmDestRom())
  {
    PduR_RmSrcRomIdxOfRmDestRomType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);
    if(!PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsIfOfRmSrcRom(rmSrcRomIdx)) /* COV_PDUR_UNQUEUED_ROUTING_ONLY_STATIC */
    {
      PduR_DestApplicationManagerRomIterType supposedSrcApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(rmSrcRomIdx);
      PduR_DestApplicationManagerRomIterType supposedDestApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));

      if((supposedSrcApplicationIdx == srcApplicationIdx) && (supposedDestApplicationIdx == destApplicationIdx))
      {
        retVal = TRUE;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmIf_IsRmDestRomIdxUnqueuedIFCrossPartitionTxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmIf_IsRmDestRomIdxUnqueuedIFCrossPartitionTxConfirmation(PduR_RmDestRomIterType rmDestRomIdx, PduR_DestApplicationManagerRomIterType srcApplicationIdx, PduR_DestApplicationManagerRomIterType destApplicationIdx)
{
  boolean retVal = FALSE;

  if(rmDestRomIdx < PduR_GetSizeOfRmDestRom())
  {
    PduR_RmSrcRomIdxOfRmDestRomType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);
    if(!PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsIfOfRmSrcRom(rmSrcRomIdx) && PduR_IsTxConfirmationSupportedOfRmSrcRom(rmSrcRomIdx)) /* COV_PDUR_UNQUEUED_ROUTING_ONLY_STATIC */
    {
      /* Destination application in this context is the application of the SrcPdu */
      PduR_DestApplicationManagerRomIterType supposedDestApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(rmSrcRomIdx);
      PduR_DestApplicationManagerRomIterType supposedSrcApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));

      /* In case of the TxConfirmation the application context is the other way round. The current destination is the "source" which sends the TxConfirmation. */
      if((supposedSrcApplicationIdx == srcApplicationIdx) && (supposedDestApplicationIdx == destApplicationIdx))
      {
        retVal = TRUE;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmIf_CheckInfoPointer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_CheckInfoPointer(P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, uint8 apiId)
{
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  /* Check parameter 'info' */
  if(info == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else
# endif
  {
    retVal = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(apiId, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  PDUR_DUMMY_STATEMENT(info);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RmIf.c
 *********************************************************************************************************************/
