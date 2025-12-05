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
 *         \file  IpduM_MuxTx.c
 *         \unit  MuxTx
 *        \brief  Microsar IpduM Multiplexer source file
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

#define IPDUM_MUXTX_SOURCE
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
#include "IpduM_TxConfirmation.h"
#include "IpduM_MuxTx.h"
#include "IpduM_MuxTxJit.h"
#include "IpduM_MuxUtil.h"
#include "IpduM_Reporting.h"
#include "SchM_IpduM.h"
#include "vstdlib.h"
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
 *  IpduM_MuxTx_GetTxBufferOfTxPathway
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduInfoType, IPDUM_CODE) IpduM_MuxTx_GetTxBufferOfTxPathway(IpduM_CTxPathwayIterType txPathwayId)
{
  PduInfoType destPduInfo;
  destPduInfo.SduDataPtr = IpduM_GetAddrTxBuffer(IpduM_GetTxBufferStartIdxOfTxPathway(txPathwayId));
  destPduInfo.SduLength = IpduM_GetTxBufferLengthOfTxPathway(txPathwayId);

  return destPduInfo;
}


/*******************************************************************************************
*  IpduM_MuxTx_Init()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_MuxTx_Init(void)
{
  /* Initialize multiplex PDU handle / index for "for" loop */
  IpduM_CTxPathwayIterType muxTxPduHnd;

  /* initialize all the Tx multiplex PDUs, reset the timeout counters and set the IpduM state to initialized */
  for(muxTxPduHnd = 0u; muxTxPduHnd < IpduM_GetSizeOfCTxPathway(); muxTxPduHnd++) /* FETA_IPDUM_01 */
  {
    /* record the dynamic PDU id */
    IpduM_SetTxPartDynamicPartIdxOfTxPathway(muxTxPduHnd, IpduM_GetTxPartInitialDynPartIdxOfTxPathway(muxTxPduHnd));
  }
  /* This check avoids copying from an empty buffer in Post-Build RxOnly configurations */
  if(IpduM_GetSizeOfTxInitValues() > 0u)    /* COV_IPDUM_INIT_MEMCPY */
  {
    VStdLib_MemCpy_s(IpduM_GetAddrTxBuffer(0), IpduM_GetSizeOfTxBuffer(), IpduM_GetAddrTxInitValues(0), IpduM_GetSizeOfTxInitValues()); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
  }
}

/*******************************************************************************************
 *  IpduM_MuxTx_Transmit()
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
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Transmit(IpduM_TxUpIndIdxOfTxUpInfoType txPartId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr) /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
  uint8 errorId = IPDUM_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;  /* PRQA S 2981*/ /* MD_MSR_RetVal*/
  IpduM_CTxPathwayIterType txPathway = IpduM_GetTxPathwayIdxOfTxPart(txPartId);

# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if(PduInfoPtr->SduLength != IpduM_GetPduLengthOfTxPart(txPartId))
  {
    errorId = IPDUM_E_PARAM;
  }
  else
# endif
  {
    if(IpduM_TxConfirmation_IsTransmissionAllowed(IpduM_GetTxLoInfoIdxOfTxPathway(txPathway)))
    {
      /* multiplex PDU buffer not engaged and accept transmission */
      /* jit update not required, assign associated multiplex buffer, multiplex data */
      /* as an optimization copying a source part requiring jit update can be avoided in this context */
      {
        PduInfoType segmentDestPduInfo;
        segmentDestPduInfo = IpduM_MuxTx_GetTxBufferOfTxPathway(txPathway);

        /* Write unused areas default value if necessary */
        SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
        {
          IpduM_SegmentIterType restSegmentIdx = IpduM_GetRestSegmentStartIdxOfTxPart(txPartId);
          for(; restSegmentIdx < IpduM_GetRestSegmentEndIdxOfTxPart(txPartId); restSegmentIdx++)  /* FETA_IPDUM_01 */
          {
            IpduM_MuxUtil_WriteMuxFillerSegment(restSegmentIdx, txPathway, &segmentDestPduInfo);  /* VCA_IPDUM_ZERO_TO_N_LOOP_NOT_SUPPORTED */
          }
        }
        SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();

        /* Copy data from the given buffer (function argument) to the TX buffer */
        SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
        {
          IpduM_SegmentIterType segmentIdx = IpduM_GetSegmentStartIdxOfTxPart(txPartId);
          for(; segmentIdx < IpduM_GetSegmentEndIdxOfTxPart(txPartId); segmentIdx++)  /* FETA_IPDUM_01 */
          {
            IpduM_MuxUtil_CopySegment(segmentIdx, &segmentDestPduInfo, PduInfoPtr->SduDataPtr);
          }
        }
        SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
      }

      /* set retVal to "OK" for successfully storing the non-triggering part for later transmission */
      retVal = E_OK; /* PRQA S 2982 */ /* MD_IpduM_2982_RetVal */

      if((!IpduM_IsTxPartStaticPartUsedOfTxPathway(txPathway)) || (IpduM_GetTxPartStaticPartIdxOfTxPathway(txPathway) != txPartId))  
      {
        SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
        IpduM_SetTxPartDynamicPartIdxOfTxPathway(txPathway, (IpduM_TxPartDynamicPartIdxOfTxPathwayType) txPartId); /* PRQA S 2842 */ /* MD_IpduM_2842 */
        SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
      }

      if(IpduM_IsTriggerEventOfTxPart(txPartId)) 
      {
        retVal = E_NOT_OK;

        if(IpduM_MuxTx_Jit_UpdateCounterPart(txPathway, txPartId) == E_OK)
        {
          PduInfoType pduInfo;
          pduInfo = IpduM_MuxTx_GetTxBufferOfTxPathway(txPathway);

          /* Interrupt disabling is required to avoid the timeout timer being reset by a tx confirmation of a previous transmission. The tx confirmation timeout is loaded before triggering transmission, because the If can confirm a previous transmission of the multiplex Pdu in the current tranmission trigger context, before the timer for the current transmission is loaded.  */
          SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
          IpduM_TxConfirmation_StartTimeout(IpduM_GetTxLoInfoIdxOfTxPathway(txPathway));
          retVal = PduR_IpduMTransmit(IpduM_GetLlPduRefOfTxPathway(txPathway), &pduInfo);

          if(retVal != E_OK)
          {
            IpduM_TxConfirmation_StopTimeout(IpduM_GetTxLoInfoIdxOfTxPathway(txPathway));
          }
          SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
        }
      }
    }
  }

  if(errorId != IPDUM_E_NO_ERROR)
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_MUXTX_TRANSMIT, errorId);  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_MuxTx_TriggerTransmit()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_TriggerTransmit(IpduM_TxPathwayIdxOfCTxLoInfoType txPathwayHnd, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(PduInfoPtr->SduLength >= IpduM_GetTxBufferLengthOfTxPathway(txPathwayHnd))
  {
    if(IpduM_MuxTx_Jit_UpdateAll(txPathwayHnd) == E_OK)
    {
      SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
      /* Copy PDU from mux buffer to provided destination buffer */
      VStdLib_MemCpy_s(PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength, IpduM_GetAddrTxBuffer(IpduM_GetTxBufferStartIdxOfTxPathway(txPathwayHnd)), IpduM_GetTxBufferLengthOfTxPathway(txPathwayHnd)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_IPDUM_MEMCPY_7 */
      PduInfoPtr->SduLength = IpduM_GetTxBufferLengthOfTxPathway(txPathwayHnd);

      IpduM_TxConfirmation_StartTimeout(IpduM_GetTxLoInfoIdxOfTxPathway(txPathwayHnd));
      SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
      retVal = E_OK;
    }
  }
  return retVal;
}

/*******************************************************************************************
 *  IpduM_MuxTx_TxConfirmation()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_MuxTx_TxConfirmation(IpduM_CTxPathwayIterType txPathwayIdx)
{
  IpduM_TxPartIterType txDynamicPartId;
  SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  txDynamicPartId = (IpduM_TxPartIterType) IpduM_GetTxPartDynamicPartIdxOfTxPathway(txPathwayIdx);
  SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();

  if(IpduM_IsTxConfirmationOfTxPart(txDynamicPartId))
  {
    /* Forward confirmation to PduR */
    PduR_IpduMTxConfirmation(IpduM_GetUlPduRefOfTxPart(txDynamicPartId));
  }
  if(IpduM_IsTxPartStaticPartUsedOfTxPathway(txPathwayIdx)) 
  {
    if(IpduM_IsTxConfirmationOfTxPart(IpduM_GetTxPartStaticPartIdxOfTxPathway(txPathwayIdx)))  
    {
      /* Forward confirmation to PduR */
      PduR_IpduMTxConfirmation(IpduM_GetUlPduRefOfTxPart(IpduM_GetTxPartStaticPartIdxOfTxPathway(txPathwayIdx)));
    }
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/* COV_JUSTIFICATION_BEGIN

  \ID COV_IPDUM_INIT_MEMCPY
    \ACCEPT TX
    \REASON This "if" statement was added to avoid illegal calls to Memcpy with a NULL ptr as the source
            address. The behavior without this "if" is always correct because the length is always zero
            in these cases, but a DET is thrown because of the NULL ptr. This "if" statement avoids the
            DET call but doesn't change any functionality.

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
  END OF FILE: IpduM_MuxTx.c
**********************************************************************************************************************/
