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
/*!        \file  Com_LLTxTp.c
 *         \unit  LLTxTp
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_LLTXTP_SOURCE

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
#include "Com_LLTxTp.h"
#include "Com_Cfg_LLTxTp.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_TxBuffer.h"

#include "Com_LLTxIf.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_TxBuffer.h"

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
  Com_LLTxTp_CopyTxData
**********************************************************************************************************************/
/*! \brief        This function does the copying of the tx data.
    \details      -
    \pre          -
    \param[in]    txTpPduInfoIdx     Index of TxTpPduInfo.
    \param[in]    PduId              ID of AUTOSAR COM I-PDU that is transmitted.
    \param[in]    PduInfoPtr         Pointer to a PduInfoType, which indicates the number of bytes to be copied (SduLength)
                                     and the location where the data have to be copied to (SduDataPtr).
    \param[out]   TxDataCntPtr       Remaining Tx data after completion of this call.
    \param[out]   BufReq_ReturnType  BUFREQ_OK:       Connection has been accepted.
                                                      RxBufferSizePtr indicates the available receive buffer.
                                     BUFREQ_E_NOT_OK: Connection has been rejected.
                                                      RxBufferSizePtr remains unchanged.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(BufReq_ReturnType, COM_CODE) Com_LLTxTp_CopyTxData(Com_TxTpInfoIterType txTpPduInfoIdx, Com_TxPduInfoIterType PduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) TxDataCntPtr);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_LLTxTp_CopyTxData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(BufReq_ReturnType, COM_CODE) Com_LLTxTp_CopyTxData(Com_TxTpInfoIterType txTpPduInfoIdx, Com_TxPduInfoIterType PduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) TxDataCntPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_IsTxBufferInfoUsedOfTxTpInfo(txTpPduInfoIdx))        /* for 0 byte pdus the state copying is never reached (switches directly to WAITING_FOR_CONFIRMATION) */
  {
    const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfTxTpInfo(txTpPduInfoIdx);
    /*@ assert txBufferInfoIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    if(Com_GetTxTpWrittenBytesCounter(txTpPduInfoIdx) < Com_TxBuffer_GetTxBufferLength(txBufferInfoIdx))
    {
      /* If enough data is present */
      if(PduInfoPtr->SduLength <= (Com_GetTxSduLength(PduId) - Com_GetTxTpWrittenBytesCounter(txTpPduInfoIdx)))
      {
        SduDataPtrType dataPtr = PduInfoPtr->SduDataPtr;
        const PduLengthType length = PduInfoPtr->SduLength;
        /* Copy the provided data segment from the Tx PDU buffer and increment the WrittenBytesCounter by the passed SduLength */
        /*@ assert $external(dataPtr); */ /* VCA_COM_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
        /*@ assert $lengthOf(dataPtr) >= length; */ /* VCA_COM_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
        VStdLib_MemCpy(dataPtr, Com_TxBuffer_GetAddrTxBufferAtIdx(txBufferInfoIdx, Com_GetTxTpWrittenBytesCounter(txTpPduInfoIdx)), length);  /* VCA_COM_MEMCPY_BOUNDARY_CHECK */ /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        Com_SetTxTpWrittenBytesCounter(txTpPduInfoIdx, (Com_GetTxTpWrittenBytesCounter(txTpPduInfoIdx) + PduInfoPtr->SduLength));

        /* Set the passed TxDataCntPtr to number of remaining bytes in the Tx PDU buffer */
        *TxDataCntPtr = (Com_GetTxSduLength(PduId) - Com_GetTxTpWrittenBytesCounter(txTpPduInfoIdx));

        /* If all bytes are transmitted set the state to WAITING_FOR_CONFIRMATION */
        if(*TxDataCntPtr == 0u)
        {
          Com_SetTxTpConnectionState(txTpPduInfoIdx, COM_WAITING_FOR_CONFIRMATION_TXTPCONNECTIONSTATE);
        }
        retVal = BUFREQ_OK;
      }
      /* Otherwise set the passed TxDataCntPtr to number of remaining bytes in the Tx PDU buffer */
      else
      {
        *TxDataCntPtr = (Com_GetTxSduLength(PduId) - Com_GetTxTpWrittenBytesCounter(txTpPduInfoIdx));
        retVal = BUFREQ_E_BUSY;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_LLTxTp_CopyTxDataProcessing
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCYC */
FUNC(BufReq_ReturnType, COM_CODE) Com_LLTxTp_CopyTxDataProcessing(Com_TxPduInfoIterType PduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, CONSTP2CONST(RetryInfoType, AUTOMATIC, AUTOMATIC) RetryInfoPtr, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) TxDataCntPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If I-PDU is active and TpDataState of RetryInfoPtr is not TP_DATARETRY */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  {
    if(Com_IPduGroupHdlr_IsTxPduActive(PduId))    /* \trace SPEC-2736849 */
    {
      if(Com_IsTxTpInfoUsedOfTxPduInfo(PduId))
      {
        if((RetryInfoPtr == NULL_PTR) || (RetryInfoPtr->TpDataState != TP_DATARETRY))
        {
          const Com_TxTpInfoIterType txTpPduInfoIdx = Com_GetTxTpInfoIdxOfTxPduInfo(PduId);
          /*@ assert txTpPduInfoIdx < Com_GetSizeOfTxTpInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */

          /* If TxTpConnectionState is... */
          switch (Com_GetTxTpConnectionState(txTpPduInfoIdx))
          {
            /* ...TRANSMISSION_STARTED, switch to COPYING and Copy Tx Data */
            case COM_TRANSMISSION_STARTED_TXTPCONNECTIONSTATE:
            {
              Com_SetTxTpConnectionState(txTpPduInfoIdx, COM_COPYING_TXTPCONNECTIONSTATE);
              retVal = Com_LLTxTp_CopyTxData(txTpPduInfoIdx, PduId, PduInfoPtr, TxDataCntPtr);
              break;
            }
            /* ...COPYING, Copy Tx Data */
            case COM_COPYING_TXTPCONNECTIONSTATE: /* PRQA S 2003 */ /* MD_Com_2003 */
            {
              retVal = Com_LLTxTp_CopyTxData(txTpPduInfoIdx, PduId, PduInfoPtr, TxDataCntPtr);
              break;
            }
            /* ...WAITING_FOR_CONFIRMATION and zero-sized segment shall be copy, return E_OK, otherwise reset the connection */
            case COM_WAITING_FOR_CONFIRMATION_TXTPCONNECTIONSTATE:
            {
              if(PduInfoPtr->SduLength == 0u)
              {
                *TxDataCntPtr = 0u;
                retVal = BUFREQ_OK;
              }
              else
              {
                Com_LLTxTp_ResetTxTpConnection(PduId);
              }
              break;
            }
            /* ...READY */
            default:
            {
              /* Intentionally Empty */
              break;
            }
          }
        }
      }
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
  return retVal;
}

/**********************************************************************************************************************
  Com_LLTxTp_TpTxConfirmationProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLTxTp_TpTxConfirmationProcessing(Com_TxPduInfoIterType PduId, Std_ReturnType Result)
{
  /* If I-PDU is active */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  {
    if(Com_IPduGroupHdlr_IsTxPduActive(PduId))
    {
      if(Com_IsTxTpInfoUsedOfTxPduInfo(PduId))
      {
        const Com_TxTpInfoIterType txTpPduInfoIdx = Com_GetTxTpInfoIdxOfTxPduInfo(PduId);
        /* If the passed Result is E_OK and the state is WAITING_FOR_CONFIRMATION or TRANSMISSION_STARTED */
        if((Result == E_OK) && (Com_GetTxTpConnectionState(txTpPduInfoIdx) == COM_WAITING_FOR_CONFIRMATION_TXTPCONNECTIONSTATE))
        {
          /* Call the normal Com_TxConfirmation */
          Com_LLTxIf_TxConfirmationProcessing(PduId);
        }

        /* Prepare the TP parameters for the start of a new transmission */
        Com_LLTxTp_ResetTxTpConnection(PduId);
      }
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
}

/**********************************************************************************************************************
  Com_LLTxTp_ResetTxTpConnection
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLTxTp_ResetTxTpConnection(Com_TxPduInfoIterType ComTxPduId)
{
  if(Com_IsTxTpInfoUsedOfTxPduInfo(ComTxPduId))
  {
    const Com_TxTpInfoIterType txTpPduInfoIdx = Com_GetTxTpInfoIdxOfTxPduInfo(ComTxPduId);
    /*@ assert txTpPduInfoIdx < Com_GetSizeOfTxTpInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_SetTxTpConnectionState(txTpPduInfoIdx, COM_READY_TXTPCONNECTIONSTATE);
    Com_SetTxTpWrittenBytesCounter(txTpPduInfoIdx, 0u);
  }
}

/**********************************************************************************************************************
  Com_LLTxTp_IsTpUsed
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_LLTxTp_IsTpUsed(Com_TxPduInfoIterType ComTxPduId)
{
  return Com_IsTxTpInfoUsedOfTxPduInfo(ComTxPduId);
}

/**********************************************************************************************************************
  Com_LLTxTp_SetTxTpConnectionState
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLTxTp_SetTxTpConnectionState(Com_TxPduInfoIterType ComTxPduId, Com_TxTpConnectionStateType state)
{
  const Com_TxTpInfoIterType txTpPduInfoIdx = Com_GetTxTpInfoIdxOfTxPduInfo(ComTxPduId);
  /*@ assert txTpPduInfoIdx < Com_GetSizeOfTxTpInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
  Com_SetTxTpConnectionState(txTpPduInfoIdx, state);
}

/**********************************************************************************************************************
  Com_LLTxTp_IsTpStateReady
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_LLTxTp_IsTpStateReady(Com_TxPduInfoIterType ComTxPduId)
{
  return (Com_GetTxTpConnectionState(Com_GetTxTpInfoIdxOfTxPduInfo(ComTxPduId)) == COM_READY_TXTPCONNECTIONSTATE);
}

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

/**********************************************************************************************************************
 *  END OF FILE: Com_LLTxTp.c
 *********************************************************************************************************************/
