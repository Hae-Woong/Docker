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
 *         \file  PduR_RmTp.c
 *         \unit  RmTp
 *        \brief  Pdu Router Routing Manager Transport Protocol source file
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
/* PRQA  S 3673 EOF */ /* MD_MSR_Rule8.13 */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_RMTP_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Fm.h"
#include "PduR_RmTp.h"
#include "PduR_RmTp_RxInst.h"
#include "PduR_RmTp_TxInst.h"
#include "PduR_MultipleSourceHandler.h"
#include "PduR_RPG.h"

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
 * PduR_RmTp_Transmit_MultiDest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_Transmit_MultiDest(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmDestRomIterType rmDestRomIdx;

  for(rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx < PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx++)  /* FETA_PDUR_01 */
  {
    if(PduR_GetDirectionOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)) == PDUR_TX_DIRECTIONOFRMGDESTROM)   /* COV_PDUR_FEATURE_ALWAYS_TRUE */
    {
      if(PduR_RPG_IsDestPduEnabled(rmDestRomIdx) == TRUE)
      {
        retVal &= PduR_MultipleSourceHandler_TpTransmit(rmDestRomIdx, info);
      }
    }
  }

  return retVal;
}

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TriggerTransmissionOfDestinations
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TriggerTransmissionOfDestinations(PduR_RmSrcRomIterType rmSrcRomIdx, PduR_DestApplicationManagerRomIterType applicationContext)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmDestRomIterType rmDestRomIdx;

  for(rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx < PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx++)  /* FETA_PDUR_01 */
  {
    if(PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
    {
      if(PduR_RPG_IsDestPduEnabled(rmDestRomIdx) == TRUE)
      {
        retVal &= PduR_MultipleSourceHandler_TpTriggerTransmission(rmDestRomIdx, applicationContext);
      }
    }
  }
  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_ActivateNext
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
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, PDUR_CODE) PduR_RmTp_ActivateNext(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx, PduR_DestApplicationManagerRomIterType applicationContext)
{
  PduR_FmFifoElementRamIterType fmFifoElementRamReadIdx = 0u;
  while(PduR_Fm_Peek(fmFifoRamIdx, &fmFifoElementRamReadIdx, memIdx) == E_OK) /* FETA_PDUR_03 */  /* PRQA S 0771 */ /* MD_PduR_0771 */
  {
    PduR_RmSrcRomIterType rmSrcRomIdx = 0u;

    if(PduR_RmTp_GetValidTpSrcPduFromQueue(fmFifoRamIdx, memIdx, &rmSrcRomIdx) == E_OK)
    {
      /*@ assert PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx); */ /* VCA_PDUR_IS_QUEUED_TP_ROUTING */
      /* Is Rx already finished? */
      if(PduR_RmTp_RxInst_IsReceptionFinished(rmSrcRomIdx, fmFifoRamIdx, memIdx))
      {
        if(PduR_RmTp_TriggerTransmissionOfDestinations(rmSrcRomIdx, applicationContext) == E_OK)
        {
          /* This situation can only occur if the transmission was already completed on the destinations inside of the triggerTransmission call.
          * This can only happen for upperlayer, RX destinations.
          * The triggerTransmission call will return E_OK. No API calls on Tx (and Rx) side are pending, thus this FIFO element must be released here. */
          if(PduR_RmTp_TxInst_IsTxIdleOnAllQueuedDestinations(rmSrcRomIdx) == TRUE)
          {
            /* Transmission was already finished. */
            PduR_Fm_RemoveOldestElement(fmFifoRamIdx, memIdx);
            continue;/* with next element in queue. */
          }
          else
          {
            break;
          }
        }
        else
        {
          PduR_Fm_RemoveOldestElement(fmFifoRamIdx, memIdx);
          continue;/* with next element in queue. */
        }
      }
      else
      {
        if(PduR_RmTp_RxInst_ThresholdReached(rmSrcRomIdx) == E_OK)
        {
          if(PduR_RmTp_TriggerTransmissionOfDestinations(rmSrcRomIdx, applicationContext) == E_OK)
          {
            PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PDUR_RM_RX_ACTIVE_TX_TRIGGERED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
          }
          else
          {
            PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler(rmSrcRomIdx);
          }
        }
        break;
      }
    }
    else
    {
      /* If the read handle id is erroneous and does not belong to this queue: free this element and look for next  */
      PduR_Fm_RemoveOldestElement(fmFifoRamIdx, memIdx);  /* COV_PDUR_ROBUSTNESS_UNCOVERED */
      continue;/* with next element in queue. */
    }
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
 /**********************************************************************************************************************
 * PduR_RmTp_CancelTransmitOnDestinations
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_CancelTransmitOnDestinations(PduR_RmSrcRomIterType rmSrcRomIdx, PduR_DestApplicationManagerRomIterType applicationContext)
{
  PduR_RmDestRomIterType rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx);
  PduR_RmDestRomIterType rmDestRomEndIdx = PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx);

  for(; rmDestRomIdx < rmDestRomEndIdx; rmDestRomIdx++)  /* FETA_PDUR_01 */
  {
    if(PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
    {
      PduR_RmTp_TxInst_CancelTransmit(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx), applicationContext);
    }
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_FinishRouting
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
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_FinishRouting(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx, boolean cancelReception, boolean cancelTransmission, PduR_DestApplicationManagerRomIterType applicationContext)
{
  PduR_RmSrcRomIterType rmSrcRomIdx = 0u;

  if(PduR_RmTp_GetValidTpSrcPduFromQueue(fmFifoRomIdx, memIdx, &rmSrcRomIdx) == E_OK)
  {
    /*@ assert PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx); */ /* VCA_PDUR_IS_QUEUED_TP_ROUTING */
    /* Tx idle. */
    if(PduR_RmTp_TxInst_IsTxIdleOnAllQueuedDestinations(rmSrcRomIdx) == TRUE)
    {
      /* Tx idle, Rx idle -> continue with the next Pdu in the queue. */
      if(PduR_RmTp_RxInst_IsReceptionFinished(rmSrcRomIdx, fmFifoRomIdx, memIdx) == TRUE)
      {
        PduR_Fm_RemoveOldestElement(fmFifoRomIdx, memIdx);
        PduR_RmTp_ActivateNext(fmFifoRomIdx, memIdx, applicationContext);
      }
      /* Tx idle, Rx active -> cancel reception depending on the call context. */
      else
      {
        if(cancelReception)
        {
          PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler(rmSrcRomIdx);
        }
      }
    }
    /* Tx active. */
    else
    {
      /* Tx active, Rx idle: cancel transmission depending on the call context. */
      if(PduR_RmTp_RxInst_IsReceptionFinished(rmSrcRomIdx, fmFifoRomIdx, memIdx) == TRUE)
      {
        if(cancelTransmission)
        {
          PduR_RmTp_CancelTransmitOnDestinations(rmSrcRomIdx, applicationContext);
        }
      }
      else
      {
        /* Nothing to be done. Only consider canceling Rx or Tx if one of those has already been finished normally. */
      }
    }
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_GetValidTpSrcPduFromQueue
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_GetValidTpSrcPduFromQueue(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx, P2VAR(PduR_RmSrcRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmSrcRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmSrcRomIterType localRmSrcRomIdx = 0u;

  if(PduR_Fm_GetRmSrcRomIdxOfOldestFmFifoElementSafe(fmFifoRomIdx, &localRmSrcRomIdx, memIdx) == E_OK)
  {
    if(PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(localRmSrcRomIdx))
    {
      PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType supposedMemIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(localRmSrcRomIdx);
      PduR_FmFifoRomIterType supposedFmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedTpPropertiesRom(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(localRmSrcRomIdx), supposedMemIdx);

      if((supposedFmFifoRomIdx == fmFifoRomIdx) && (supposedMemIdx == memIdx))  /* COV_PDUR_ROBUSTNESS */
      {
        retVal = E_OK;
        *rmSrcRomIdx = localRmSrcRomIdx;
      }
    }
  }

  return retVal;
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp.c
 *********************************************************************************************************************/
