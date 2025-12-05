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
/*!        \file  Com_LLRxTp.c
 *         \unit  LLRxTp
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_LLRXTP_SOURCE

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
#include "Com_LLRxTp.h"
#include "Com_Cfg_LLRxTp.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#include "Com_LLRxIf.h"
#include "Com_IPduGroupHdlr.h"

#include "vstdlib.h"
#include "SchM_Com.h"

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
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_LLRxTp_StartOfReceptionProcessing
**********************************************************************************************************************/
/*!  Internal comment removed.
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
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, COM_CODE) Com_LLRxTp_StartOfReceptionProcessing(Com_RxPduInfoIterType rxPduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) TpSduInfoPtr,
                                                                        PduLengthType TpSduLength, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) RxBufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType bufReq_status = BUFREQ_E_NOT_OK;    /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ----------------------------------------------- */
  /* If I-PDU is active */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
  {
    if(Com_IPduGroupHdlr_IsRxPduActive(rxPduId))  /* \trace SPEC-2736846 */
    {
      if(Com_IsRxTpInfoUsedOfRxPduInfo(rxPduId))
      {
        const Com_RxTpInfoIterType rxTpPduInfoIdx = Com_GetRxTpInfoIdxOfRxPduInfo(rxPduId);
        /*@ assert rxTpPduInfoIdx < Com_GetSizeOfRxTpInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */

        switch (Com_GetRxTpConnectionState(rxTpPduInfoIdx))
        {
            /* If Com is ready for new TP connection */
          case COM_READY_RXTPCONNECTIONSTATE:
          {
            PduLengthType requiredBytes = TpSduLength;
            /* If the passed TpSduLength + MetaDataLength fits into the current available buffer */
            if(Com_IsRxTpBufferMetaDataUsedOfRxTpInfo(rxTpPduInfoIdx))
            {
              requiredBytes += (PduLengthType) Com_GetRxTpBufferMetaDataLengthOfRxTpInfo(rxTpPduInfoIdx);
            }
            if(requiredBytes <= Com_GetBufferSizeOfRxTpInfo(rxTpPduInfoIdx))
            {
              /* Store the passed TpSduLength, set the passed RxBufferSizePtr to the available buffer size and set the Rx Tp ComIPdu state to RECEPTION_STARTED */
              Com_SetRxTpSduLength(rxTpPduInfoIdx, TpSduLength);
              *RxBufferSizePtr = Com_GetBufferSizeOfRxTpInfo(rxTpPduInfoIdx);

              if(TpSduLength > 0u)
              {
                Com_SetRxTpConnectionState(rxTpPduInfoIdx, COM_RECEPTION_STARTED_RXTPCONNECTIONSTATE);
              }
              else
              {
                Com_SetRxTpConnectionState(rxTpPduInfoIdx, COM_WAITING_FOR_INDICATION_RXTPCONNECTIONSTATE);
              }

              /* If meta data is configured, copy the passed meta data to the buffer and set the passed RxBufferSizePtr value to the remaining receive buffer size */
              if(Com_IsRxTpBufferMetaDataUsedOfRxTpInfo(rxTpPduInfoIdx))
              {
                CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) metaDataSourcePtr = &(TpSduInfoPtr->SduDataPtr[TpSduInfoPtr->SduLength - Com_GetRxTpBufferMetaDataLengthOfRxTpInfo(rxTpPduInfoIdx)]);
                VStdLib_MemCpy(Com_GetAddrRxTpBuffer(Com_GetRxTpBufferMetaDataStartIdxOfRxTpInfo(rxTpPduInfoIdx)), metaDataSourcePtr, Com_GetRxTpBufferMetaDataLengthOfRxTpInfo(rxTpPduInfoIdx));  /* VCA_COM_MEMCPY_BOUNDARY_CHECK */     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

                *RxBufferSizePtr = (PduLengthType) Com_GetBufferSizeOfRxTpInfo(rxTpPduInfoIdx) - (PduLengthType) Com_GetMetaDataLengthOfRxPduInfo(rxPduId);
              }
              bufReq_status = BUFREQ_OK;
            }
            /* Otherwise return BUFREQ_E_OVFL. The bufferSizePtr remains unchanged */
            else
            {
              *RxBufferSizePtr = Com_GetBufferSizeOfRxTpInfo(rxTpPduInfoIdx);
              bufReq_status = BUFREQ_E_OVFL;
            }
            break;
          }
            /* Else if Com is waiting for deferred processing, reject connection */
          case COM_BUFFER_LOCKED_RXTPCONNECTIONSTATE:
          {
            /* Buffer stays locked */
            bufReq_status = BUFREQ_E_NOT_OK;
            break;
          }
            /* Else reset TP connection */
          default:
          {
            Com_LLRxTp_ResetRxTpConnection(rxPduId);
            break;
          }
        }
      }
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
  return bufReq_status;
}

/**********************************************************************************************************************
  Com_LLRxTp_TpRxIndicationProcessing
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
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, COM_CODE) Com_LLRxTp_TpRxIndicationProcessing(Com_RxPduInfoIterType rxPduId, Std_ReturnType Result)
{
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
  {
    if(Com_IPduGroupHdlr_IsRxPduActive(rxPduId))  /* \trace SPEC-2736846 */
    {
      if(Com_IsRxTpInfoUsedOfRxPduInfo(rxPduId))
      {
        /* ----- Local Variables ---------------------------------------------- */
        boolean resetRequired = FALSE;
        const Com_RxTpInfoIterType rxTpPduInfoIdx = Com_GetRxTpInfoIdxOfRxPduInfo(rxPduId);
        /*@ assert rxTpPduInfoIdx < Com_GetSizeOfRxTpInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        /* ----- Implementation ----------------------------------------------- */
        switch (Com_GetRxTpConnectionState(rxTpPduInfoIdx))
        {
          /* If RXTPCONNECTIONSTATE is COPYING or RECEPTION_STARTED, reset Tp connection. */
          case COM_RECEPTION_STARTED_RXTPCONNECTIONSTATE:
          {
            resetRequired = TRUE;
            break;
          }
          case COM_COPYING_RXTPCONNECTIONSTATE:
          {
            resetRequired = TRUE;
            break;
          }
          /* If RXTPCONNECTIONSTATE is WAITING_FOR_INDICATION */
          case COM_WAITING_FOR_INDICATION_RXTPCONNECTIONSTATE:
          {
            /* If the passed Result is negative, reset TP connection. */
            if(Result != E_OK)
            {
              resetRequired = TRUE;
            }
            /* Otherwise the Rx TP ComIPdu reception was successful */
            else
            {
              /* If meta data is configured, add meta data length */
              const PduLengthType constSduLength = Com_GetRxTpSduLength(rxTpPduInfoIdx) + (PduLengthType) Com_GetMetaDataLengthOfRxPduInfo(rxPduId);
              PduInfoType rxPduInfo = { NULL_PTR, constSduLength };

              if(Com_IsRxTpBufferUsedOfRxTpInfo(rxTpPduInfoIdx))
              {
                rxPduInfo.SduDataPtr = Com_GetAddrRxTpBuffer(Com_GetRxTpBufferStartIdxOfRxTpInfo(rxTpPduInfoIdx));
              }

              /* If deferred processing is required, set RxTpConnectionState to BUFFER_LOCKED. */
              if(Com_IsHandleRxPduDeferredUsedOfRxPduInfo(rxPduId))
              {
                Com_SetRxTpConnectionState(rxTpPduInfoIdx, COM_BUFFER_LOCKED_RXTPCONNECTIONSTATE);
                resetRequired = !Com_LLRxIf_RxIndicationProcessing(rxPduId, &rxPduInfo);
              }
              else
              /* Otherwise initiate processing and reset TP machine afterwards. */
              {
                (void) Com_LLRxIf_RxIndicationProcessing(rxPduId, &rxPduInfo);
                resetRequired = TRUE;
              }
            }
            break;
          }
          /* Do nothing if RxTpConnectionState is BUFFER_LOCKED or READY */
          default:
          {
            /* Intentionally Empty */
            break;
          }
        }

        /* Reset TP state machine if required. */
        if(resetRequired)
        {
          Com_LLRxTp_ResetRxTpConnection(rxPduId);
        }
      }
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
}

/**********************************************************************************************************************
  Com_LLRxTp_CopyRxDataProcessing
**********************************************************************************************************************/
/*!  Internal comment removed.
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
**********************************************************************************************************************/
/* PRQA S 6080, 6030 1 */ /* MD_MSR_STMIF, MD_MSR_STCYC */
FUNC(BufReq_ReturnType, COM_CODE) Com_LLRxTp_CopyRxDataProcessing(Com_RxPduInfoIterType rxPduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPointer, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) RxBufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType bufReq_status = BUFREQ_E_NOT_OK;    /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ----------------------------------------------- */
  /* If I-PDU is active */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
  {
    if(Com_IPduGroupHdlr_IsRxPduActive(rxPduId))  /* \trace SPEC-2736846 */
    {
      if(Com_IsRxTpInfoUsedOfRxPduInfo(rxPduId))
      {
        const Com_RxTpInfoIterType rxTpPduInfoIdx = Com_GetRxTpInfoIdxOfRxPduInfo(rxPduId);
        /*@ assert rxTpPduInfoIdx < Com_GetSizeOfRxTpInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */

        /* If RxTpConnectionState is RECEPTION_STARTED, set RxTpConnectionState to COPYING */
        if(Com_GetRxTpConnectionState(rxTpPduInfoIdx) == COM_RECEPTION_STARTED_RXTPCONNECTIONSTATE)
        {
          Com_SetRxTpConnectionState(rxTpPduInfoIdx, COM_COPYING_RXTPCONNECTIONSTATE);
        }
        /* If TxTpConnectionState is... */
        switch (Com_GetRxTpConnectionState(rxTpPduInfoIdx))
        {
          /* ... COPYING */
          case COM_COPYING_RXTPCONNECTIONSTATE: /* PRQA S 2003 */ /* MD_Com_2003 */
          {
            /* If segment fits into buffer */
            if(PduInfoPointer->SduLength <= (Com_GetRxTpSduLength(rxTpPduInfoIdx) - Com_GetRxTpWrittenBytesCounter(rxTpPduInfoIdx)))
            {
              /* Copy the passed bytes to the buffer, update the written bytes counter and set the passed RxBufferSizePtr value to the remaining receive buffer size */
              if(Com_IsRxTpBufferUsedOfRxTpInfo(rxTpPduInfoIdx)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */ /* in startofreception the state directly switches to WAITING_FOR_INDICATION for 0 byte Pdus */
              {
                if((Com_GetRxTpBufferStartIdxOfRxTpInfo(rxTpPduInfoIdx) + (Com_RxTpBufferStartIdxOfRxTpInfoType) Com_GetRxTpWrittenBytesCounter(rxTpPduInfoIdx)) < Com_GetRxTpBufferEndIdxOfRxTpInfo(rxTpPduInfoIdx)) /* COV_COM_RX_TP_BUFFER */
                {
                  const Com_RxTpBufferIterType rxTpBufferIdx = (Com_RxTpBufferIterType) Com_GetRxTpBufferStartIdxOfRxTpInfo(rxTpPduInfoIdx) + (Com_RxTpBufferIterType) Com_GetRxTpWrittenBytesCounter(rxTpPduInfoIdx);
                  VStdLib_MemCpy(Com_GetAddrRxTpBuffer(rxTpBufferIdx), PduInfoPointer->SduDataPtr, PduInfoPointer->SduLength); /* VCA_COM_MEMCPY_BOUNDARY_CHECK */ /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
                  Com_SetRxTpWrittenBytesCounter(rxTpPduInfoIdx, (Com_GetRxTpWrittenBytesCounter(rxTpPduInfoIdx) + PduInfoPointer->SduLength));
                  *RxBufferSizePtr = (Com_GetBufferSizeOfRxTpInfo(rxTpPduInfoIdx) - Com_GetRxTpWrittenBytesCounter(rxTpPduInfoIdx));
                  /* If meta data is configured adapt the RxBufferSizePtr */
                  *RxBufferSizePtr -= Com_GetMetaDataLengthOfRxPduInfo(rxPduId);
                  /* If the expected amount of bytes is received, set Rx Tp ComIPdu state to WAITING_FOR_INDICATION */
                  if(Com_GetRxTpSduLength(rxTpPduInfoIdx) == Com_GetRxTpWrittenBytesCounter(rxTpPduInfoIdx))
                  {
                    Com_SetRxTpConnectionState(rxTpPduInfoIdx, COM_WAITING_FOR_INDICATION_RXTPCONNECTIONSTATE);
                  }
                  bufReq_status = BUFREQ_OK;
                }
              }
            }
            break;
          }
          /* ... WAITING_FOR_INDICATION */
          case COM_WAITING_FOR_INDICATION_RXTPCONNECTIONSTATE:
          {
            /* If zero-sized segment is provided, do nothing, otherwise reset state machine */
            if(PduInfoPointer->SduLength == 0u)
            {
              *RxBufferSizePtr = 0u;
              bufReq_status = BUFREQ_OK;
            }
            else
            {
              Com_LLRxTp_ResetRxTpConnection(rxPduId);
            }
            break;
          }
          /* ... READY or BUFFER_LOCKED */
          default:
          {
            /* Intentionally Empty */
            break;
          }
        }
      }
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
  return bufReq_status;
}

/**********************************************************************************************************************
  Com_LLRxTp_ResetRxTpConnection
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLRxTp_ResetRxTpConnection(Com_RxPduInfoIterType rxPduId)
{
  if(Com_IsRxTpInfoUsedOfRxPduInfo(rxPduId))
  {
    const Com_RxTpInfoIterType rxTpPduInfoIdx = Com_GetRxTpInfoIdxOfRxPduInfo(rxPduId);
    /*@ assert rxTpPduInfoIdx < Com_GetSizeOfRxTpInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */

    Com_SetRxTpWrittenBytesCounter(rxTpPduInfoIdx, 0);
    Com_SetRxTpSduLength(rxTpPduInfoIdx, 0);
    Com_SetRxTpConnectionState(rxTpPduInfoIdx, COM_READY_RXTPCONNECTIONSTATE);
  }
}

/**********************************************************************************************************************
  Com_LLRxTp_GetAddrRxTpBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_RxTpBufferPtrType, COM_CODE) Com_LLRxTp_GetAddrRxTpBuffer(Com_RxPduInfoIterType rxPduId)
{
  Com_RxTpBufferPtrType rxTpBufferPtr = NULL_PTR;
  if(Com_IsRxTpInfoUsedOfRxPduInfo(rxPduId) && Com_IsRxTpBufferUsedOfRxTpInfo(Com_GetRxTpInfoIdxOfRxPduInfo(rxPduId)))
  {
    rxTpBufferPtr = Com_GetAddrRxTpBuffer(Com_GetRxTpBufferStartIdxOfRxTpInfo(Com_GetRxTpInfoIdxOfRxPduInfo(rxPduId)));
  }
  return rxTpBufferPtr;
}

#if (COM_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
  Com_LLRxTp_IsRxTpBufferMetaDataUsed
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_LLRxTp_IsRxTpBufferMetaDataUsed(Com_RxPduInfoIterType rxPduId)
{
  boolean ret = FALSE;
  if (Com_IsRxTpInfoUsedOfRxPduInfo(rxPduId))
  {
    ret = Com_IsRxTpBufferMetaDataUsedOfRxTpInfo(Com_GetRxTpInfoIdxOfRxPduInfo(rxPduId));
  }

  return ret;
}

/**********************************************************************************************************************
  Com_LLRxTp_GetRxTpBufferMetaDataLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_LLRxTp_GetRxTpBufferMetaDataLength(Com_RxPduInfoIterType rxPduId)
{
  PduLengthType ret = 0u;
  if(Com_IsRxTpInfoUsedOfRxPduInfo(rxPduId))
  {
    ret = (PduLengthType) Com_GetRxTpBufferMetaDataLengthOfRxTpInfo(Com_GetRxTpInfoIdxOfRxPduInfo(rxPduId));
  }
  return ret;
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
 *  END OF FILE: Com_LLRxTp.c
 *********************************************************************************************************************/
