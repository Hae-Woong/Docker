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
 *         \file  PduR_RmTp_RxRouting.c
 *         \unit  RmTp_RxRouting
 *        \brief  Pdu Router Routing Manager Transport Protocol Rx Routing source file
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

#define PDUR_RMTP_RXROUTING_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Mm.h"
#include "PduR_RmTp_RxRouting.h"
#include "PduR_RmTp_RxInst.h"
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
 * PduR_RmTp_RxRouting_DispatchStartOfReception
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
/* PRQA S 3673, 6080 1 */ /* MD_MSR_Rule8.13, MD_MSR_STMIF */
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxRouting_DispatchStartOfReception(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                              PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmDestRomIterType rmDestRomIdxOfApiFwdDest = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx);
  boolean apiFwdDestWasCalled = FALSE;
  PduR_RmDestRomIterType rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx);

  for(; rmDestRomIdx < PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx++)  /* FETA_PDUR_01 */  /* COV_PDUR_UNQUEUED_ROUTING_ONLY */
  {
    if(!PduR_IsQueuedOfRmDestRom(rmDestRomIdx))  /* COV_PDUR_UNQUEUED_ROUTING_ONLY */
    {
      if(PduR_RPG_IsDestPduEnabled(rmDestRomIdx) == TRUE)
      {
        PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
        retVal = PduR_Mm_CallTpStartOfReception(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), info, TpSduLength, bufferSizePtr);
        rmDestRomIdxOfApiFwdDest = rmDestRomIdx;
        apiFwdDestWasCalled = TRUE;
        break;
      }
    }
  }

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  if(PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx))
  {
    if(apiFwdDestWasCalled == TRUE)
    {
      if(retVal == BUFREQ_OK)
      {
        PduLengthType bufferSizeOfQueuedDestinations;

        retVal = PduR_RmTp_RxInst_StartOfReception(rmSrcRomIdx, info, TpSduLength, &bufferSizeOfQueuedDestinations);
        if(retVal == BUFREQ_OK)
        {
          if(bufferSizeOfQueuedDestinations < *bufferSizePtr)
          {
            *bufferSizePtr = bufferSizeOfQueuedDestinations;
          }
        }
        else
        {
          PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdxOfApiFwdDest);
          PduR_Mm_CallTpRxIndication(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), E_NOT_OK);
        }
      }
     }
    else
    {
      retVal = PduR_RmTp_RxInst_StartOfReception(rmSrcRomIdx, info, TpSduLength, bufferSizePtr);
    }
  }
# endif

  PDUR_DUMMY_STATEMENT(apiFwdDestWasCalled);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(rmDestRomIdxOfApiFwdDest);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmTp_RxRouting_DispatchCopyRxData
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxRouting_DispatchCopyRxData(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                        P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */
  boolean apiFwdDestWasCalled = FALSE;
  PduR_RmDestRomIterType rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx);

  /*----- Iterate destPdus, find out the only unbuffered destPdu (only one unbuffered destPdu is supported), call the Forwarding Tp module CopyRxData function -----*/
  for (; rmDestRomIdx < PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx++)  /* FETA_PDUR_01 */  /* COV_PDUR_UNQUEUED_ROUTING_ONLY */
  {
    if(!PduR_IsQueuedOfRmDestRom(rmDestRomIdx))  /* COV_PDUR_UNQUEUED_ROUTING_ONLY */
    {
      if(PduR_RPG_IsDestPduEnabled(rmDestRomIdx) == TRUE)
      {
        PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
        retVal = PduR_Mm_CallTpCopyRxData(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), info, bufferSizePtr);
        apiFwdDestWasCalled = TRUE;
        break;
      }
    }
  }

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  /* ----- when Unbuffered destRoms exist, call the gateway or Tp module start of reception function ----- */
  if(PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx))
  { /* buffered Tp Routings */
    if(apiFwdDestWasCalled == TRUE)
    {
      if(retVal == BUFREQ_OK)
      {/* buffered Tp Routings */
        PduLengthType bufferSizeOfQueuedDestinations;

        retVal = PduR_RmTp_RxInst_CopyRxData(rmSrcRomIdx, info, &bufferSizeOfQueuedDestinations);
        if(retVal == BUFREQ_OK)
        {
          if(bufferSizeOfQueuedDestinations < *bufferSizePtr)
          {
            *bufferSizePtr = bufferSizeOfQueuedDestinations;
          }
        }
      }
    }
    else
    {
      retVal = PduR_RmTp_RxInst_CopyRxData(rmSrcRomIdx, info, bufferSizePtr);
    }
  }
# endif

  PDUR_DUMMY_STATEMENT(apiFwdDestWasCalled);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmTp_RxRouting_DispatchTpRxIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_RxRouting_DispatchTpRxIndication(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result)
{
  PduR_RmDestRomIterType rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx);

  /*----- Iterate destPdus, find out the only unbuffered destPdu (only one unbuffered destPdu is supported), call the Forwarding Tp module RxIndication -----*/
  for (; rmDestRomIdx < PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx++)  /* FETA_PDUR_01 */  /* COV_PDUR_UNQUEUED_ROUTING_ONLY */
  {
    if(!PduR_IsQueuedOfRmDestRom(rmDestRomIdx))  /* COV_PDUR_UNQUEUED_ROUTING_ONLY */
    {
      PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
      PduR_Mm_CallTpRxIndication(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), result);
      break;
    }
  }

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  if(PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx))
  {
    /* buffered Tp Routings */
    PduR_RmTp_RxInst_TpRxIndication(rmSrcRomIdx, result);
  }
# endif
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp_RxRouting.c
 *********************************************************************************************************************/
