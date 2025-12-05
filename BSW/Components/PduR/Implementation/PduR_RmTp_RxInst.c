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
 *         \file  PduR_RmTp_RxInst.c
 *         \unit  RmTp_RxInst
 *        \brief  Pdu Router Routing Manager Transport Protocol Rx Instance source file
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

#define PDUR_RMTP_RXINST_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Bm.h"
#include "PduR_Fm.h"
#include "PduR_Mm.h"
#include "PduR_RmTp.h"
#include "PduR_RmTp_RxInst.h"
#include "PduR_Lock.h"
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

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_QueueMetaData
 *********************************************************************************************************************/
/*! \brief         This function writes the meta data to the queue.
 *  \details       -
 *  \param[in]     bmTxBufferRomIdx Valid BmTxBufferRom table index. Internal handle of a PduRTxBuffer.
 *  \param[in]     memIdx           memory section index
 *  \param[in]     metaDataLength   Meta data length
 *  \param[in]     pduLength        Total length of the data in sduDataPtr.
 *  \param[in]     sduDataPtr       Data + Meta Data pointer.
 *  \return        BUFREQ_OK        Meta data was queued.
 *                 BUFREQ_E_NOT_OK  Meta data was not queued. This should never happen in a valid configuration.
 *  \pre           -
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_QueueMetaData(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx, PduLengthType metaDataLength, PduLengthType pduLength, SduDataPtrType sduDataPtr);
#endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler
 *********************************************************************************************************************/
/*! \brief       Helper function which handles the StartOfReception depending on the Rx state.
 *  \details     -
 *  \param[in]   rmSrcRomIdx       Routing Manager source Id
 *  \param[in]   info              Pointer to the buffer (SduDataPtr) and its length (SduLength). Only used optionally for meta data.
 *  \param[in]   TpSduLength       Length of the entire TP PDU which will be received.
 *  \param[out]  bufferSizePtr     Pointer to the bufferSize. The available buffer size will be written to this variable.
 *  \return      BufReq_ReturnType
 *               BUFREQ_OK         StartOfReception request accepted.
 *               BUFREQ_E_NOT_OK   StartOfReception request not accepted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler_RM_RX_IDLE
 *********************************************************************************************************************/
/*! \brief       This functions handles the StartOfReception in the Rx state 'IDLE'.
 *  \details     -
 *  \param[in]   rmSrcRomIdx       Routing Manager source Id
 *  \param[in]   info              Pointer to the buffer (SduDataPtr) and its length (SduLength). Only used optionally for meta data.
 *  \param[in]   TpSduLength       Length of the entire TP PDU which will be received.
 *  \param[out]  bufferSizePtr     Pointer to the bufferSize. The available buffer size will be written to this variable.
 *  \return      BufReq_ReturnType
 *               BUFREQ_OK         StartOfReception request accepted.
 *               BUFREQ_E_NOT_OK   StartOfReception request not accepted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-1096, SPEC-2020151
***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler_RM_RX_IDLE(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                                            PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler
 *********************************************************************************************************************/
/*! \brief       Helper function which copies the rx data from this rmSrc depending on the TpRxSm state.
 *  \details     -
 *  \param[in]   rmSrcRomIdx       Routing Manager source Id
 *  \param[in]   info              Pointer to the buffer (SduDataPtr) and its length (SduLength). Only used optionally for meta data.
 *  \param[out]  bufferSizePtr     Pointer to the bufferSize. The available buffer size will be written to this variable.
 *  \return      BufReq_ReturnType
 *               BUFREQ_OK         StartOfReception request accepted.
 *               BUFREQ_E_NOT_OK   StartOfReception request not accepted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CONST) PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE
**********************************************************************************************************************/
/*! \brief      This function copies the data of a TP Pdu to the allocated TxBuffer in the queue.
 *  \details    This API can be called multiple times until the buffer is full. Then you have to wait until space
 *              is available in the buffer (e.g. because the Tx side reads data from the buffer).
 *              The Rx State machine is in state "Active".
 *  \param[in]  rmSrcRomIdx       Routing Manager source Id
 *  \param[in]  pduLength         Length of the data to be copied.
 *  \param[in]  sduDataPtr        Pointer to the data to be copied.
 *  \param[out] bufferSizePtr     Available receive buffer after data has been copied.
 *  \return     BufReq_ReturnType Result of the Copy request.
 *              BUFREQ_OK         Copying was successful.
 *              BUFREQ_E_NOT_OK   Copying failed.
 *              BUFREQ_E_BUSY     Copying failed. Currently not enough free space in the buffer for this pduLength.
 *                                It's up the requester to retry request for a certain time.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-1096, SPEC-2020151
**********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE(PduR_RmSrcRomIterType rmSrcRomIdx, PduLengthType pduLength,
                                                                                        SduDataPtrType sduDataPtr, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED
**********************************************************************************************************************/
/*! \brief      This function copies the data of a TP Pdu to the allocated TxBuffer in the queue.
 *  \details    This API can be called multiple times until the buffer is full. Then you have to wait until space
 *              is available in the buffer (e.g. because the Tx side reads data from the buffer).
 *              The Rx State machine is in state "RX_ACTIVE_TX_TRIGGERED".
 *  \param[in]  rmSrcRomIdx       Routing Manager source Id
 *  \param[in]  pduLength         Length of the data to be copied.
 *  \param[in]  sduDataPtr        Pointer to the data to be copied.
 *  \param[out] bufferSizePtr     Available receive buffer after data has been copied.
 *  \return     BufReq_ReturnType Result of the Copy request.
 *              BUFREQ_OK         Copying was successful.
 *              BUFREQ_E_NOT_OK   Copying failed.
 *              BUFREQ_E_BUSY     Copying failed. Currently not enough free space in the buffer for this pduLength.
 *                                It's up the requester to retry request for a certain time.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-1096, SPEC-2020151
**********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED(PduR_RmSrcRomIterType rmSrcRomIdx, PduLengthType pduLength,
                                                                                        SduDataPtrType sduDataPtr, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler
 *********************************************************************************************************************/
/*! \brief       Helper function which handles the TpRxIndication for the corresponding queue depending on the TpRxSm state.
 *  \details     -
 *  \param[in]   rmSrcRomIdx           Routing Manager source Id
 *  \param[in]   result                Result of the TP reception.
 *               E_OK    successful
 *               E_NO_OK not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_OR_RM_RX_ABORTED
 *********************************************************************************************************************/
/*! \brief       This function handles the TpRxIndication in the Rx state "RX_ACTIVE" or "RX_ABORTED"
 *  \details     -
 *  \param[in]   rmSrcRomIdx           Routing Manager source Id
 *  \param[in]   result                Result of the TP reception.
 *               E_OK    successful
 *               E_NO_OK not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-1096, SPEC-2020151
***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_OR_RM_RX_ABORTED(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED
 *********************************************************************************************************************/
/*! \brief       This function handles the TpRxIndication in the Rx state "RX_ACTIVE_TX_TRIGGERED"
 *  \details     -
 *  \param[in]   rmSrcRomIdx           Routing Manager source Id
 *  \param[in]   result                Result of the TP reception.
 *               E_OK    successful
 *               E_NO_OK not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       SPEC-1096, SPEC-2020151
***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler_RM_RX_ACTIVE
 *********************************************************************************************************************/
/*! \brief       The function is called to cancel an active reception.
 *  \details     -
 *  \param[in]   rmSrcRomIdx    Routing Manager source Id
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler_RM_RX_ACTIVE(PduR_RmSrcRomIterType rmSrcRomIdx);
# endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_IsSrcPduInstanceFirstInQueue
 *********************************************************************************************************************/
/*! \brief       Is this SrcPdu instance the first in the corresponding FIFO queue.
 *  \details     -
 *  \param[in]   rmSrcRomIdx            Routing Manager Src Index
 *  \return      TRUE
 *               FALSE
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_RmTp_RxInst_IsSrcPduInstanceFirstInQueue(PduR_RmSrcRomIterType rmSrcRomIdx);
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_GetCurrentlyUsedBmTxBufferRamIdxByRmSrcIdx
 *********************************************************************************************************************/
/*! \brief       This function returns the currently written bmTxBuffer index of this source Pdu.
 *  \details     -
 *  \param[in]   rmSrcRomIdx         Routing Manager Src Index
 *  \param[out]  bmTxBufferRomIdx    tx buffer index
 *  \return      E_OK                the read bmTxBufferRomIdx is valid and can be used
 *               E_NOT_OK            the read bmTxBufferRomIdx is invalid and can not be used
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_GetCurrentlyUsedBmTxBufferRamIdxByRmSrcIdx(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduR_BmTxBufferRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferRomIdx);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_Init
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_Init(PduR_MemIdxType memIdx)
{
  PduR_RmBufferedTpPropertiesRomIterType rmBufferedTpPropertiesRamIdx;

  for(rmBufferedTpPropertiesRamIdx = 0u; rmBufferedTpPropertiesRamIdx < PduR_GetSizeOfRmBufferedTpPropertiesRam(memIdx); rmBufferedTpPropertiesRamIdx++)  /* FETA_PDUR_01 */
  {
    PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(rmBufferedTpPropertiesRamIdx, PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
    PduR_SetFmFifoElementRamIdxOfRmBufferedTpPropertiesRam(rmBufferedTpPropertiesRamIdx, PDUR_NO_FMFIFOELEMENTRAMIDXOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_UpStartOfReception
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_UpStartOfReception(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)

{
  Std_ReturnType retval = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  PduR_Lock_LockRoutingPathBySrcPdu(rmSrcRomIdx);
  {
    PduLengthType bufferSizePtr = 0u;
    PduInfoType tempInfo;
    tempInfo.SduLength = info->SduLength;
    tempInfo.SduDataPtr = info->SduDataPtr;

    if(BUFREQ_OK == PduR_RmTp_RxInst_StartOfReception(rmSrcRomIdx, &tempInfo, tempInfo.SduLength, &bufferSizePtr))
    {
      PduR_BmTxBufferRomIterType bmTxBufferRomIdx = 0u;

      /* Get the internally assigned buffer to write the data. */
      if(PduR_RmTp_RxInst_GetCurrentlyUsedBmTxBufferRamIdxByRmSrcIdx(rmSrcRomIdx, &bmTxBufferRomIdx) == E_OK) /* COV_PDUR_ROBUSTNESS */
      {
        PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);

        PduInfoType bufferInfo;
        bufferInfo = PduR_Bm_GetWritePtrToBuffer(bmTxBufferRomIdx, memIdx);

        /* Is the provided internal buffer large enough? */
        if(bufferInfo.SduLength >= info->SduLength)
        {
          PduLengthType availableDataPtr = 0u;
          bufferInfo.SduLength = info->SduLength; /* pointer shall request the transmitted length, not the available length in the buffer */

          /* Copy data from the upper layer. */
          if(BUFREQ_OK == PduR_Mm_CallTpCopyTxData(PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx), &bufferInfo, (P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA)) NULL_PTR, &availableDataPtr))
          {
            /* Was only successful if there is no more data at the upper layer afterwards. */
            if(availableDataPtr == 0u)
            {
              PduR_Bm_IncrementWriteIdx(bmTxBufferRomIdx, memIdx, info->SduLength);
              retval = E_OK;
            }
          }
        }
      }

      /* Depending on the return value, finish the internal 'reception' and the transmission from the upper layer accordingly. */
      PduR_RmTp_RxInst_TpRxIndication(rmSrcRomIdx, retval);
      PduR_Mm_CallTpTxConfirmation(PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx), retval);
    }
    else
    {
      PduR_Mm_CallTpTxConfirmation(PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx), E_NOT_OK);
    }
  }
  PduR_Lock_UnlockRoutingPathBySrcPdu(rmSrcRomIdx);

  return retval;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_StartOfReception
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_StartOfReception(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                              PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  # if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);

  if(bufferSizePtr == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else if((PduR_GetMetaDataLengthOfRmBufferedTpPropertiesRom(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), memIdx) > 0u)
           && ((info == NULL_PTR) || (info->SduDataPtr == NULL_PTR) || (info->SduLength < (PduLengthType) PduR_GetMetaDataLengthOfRmBufferedTpPropertiesRom(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), memIdx))))
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    PduR_Lock_LockRoutingPathBySrcPdu(rmSrcRomIdx);
    {
      /* Handle src instance routing state */
      retVal = PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler(rmSrcRomIdx, info, TpSduLength, bufferSizePtr);
    }
    PduR_Lock_UnlockRoutingPathBySrcPdu(rmSrcRomIdx);
  }
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_RMTP_SORX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_QueueMetaData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_QueueMetaData(PduR_BmTxBufferRomIterType bmTxBufferRomIdx, PduR_MemIdxType memIdx, PduLengthType metaDataLength, PduLengthType pduLength, SduDataPtrType sduDataPtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* Metadata is appended after the payload data. This result can never be zero as it is check in a previous API pattern check. */
  PduLengthType metadataStartPosition = pduLength - metaDataLength;

  /* enqueue data will never fail in this context because the buffer is always empty */
  retVal = PduR_Bm_PutData(bmTxBufferRomIdx, metaDataLength, &sduDataPtr[metadataStartPosition], memIdx);
  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler
 *********************************************************************************************************************/
  /*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler(PduR_RmSrcRomIterType rmSrcRomIdx,
                                                                                 P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                                 PduLengthType TpSduLength,
                                                                                 P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  switch (PduR_GetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx)))
  {
    case PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      retVal = PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler_RM_RX_IDLE(rmSrcRomIdx, info, TpSduLength, bufferSizePtr);
      break;

    case PDUR_RM_RX_ACTIVE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_Reporting_ReportError(PDUR_FCT_SOR, PDUR_E_UNEXPECTED_CALL);
      break;

    case PDUR_RM_RX_ACTIVE_TX_TRIGGERED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_Reporting_ReportError(PDUR_FCT_SOR, PDUR_E_UNEXPECTED_CALL);
      break;

    case PDUR_RM_RX_ABORTED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_Reporting_ReportError(PDUR_FCT_SOR, PDUR_E_UNEXPECTED_CALL);
      break;

    default: /* COV_PDUR_MISRA */
      /* If state is out-of-bounds: do nothing and return negative result. */
      retVal = BUFREQ_E_NOT_OK;
      break;
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler_RM_RX_IDLE
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
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_StartOfReception_TpRxSmStateHandler_RM_RX_IDLE(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);
  PduR_RmBufferedTpPropertiesRomIdxOfRmSrcRomType rmBufferedTpPropertiesRomIdx = PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx);
  PduLengthType tpThreshold = PduR_GetTpThresholdOfRmBufferedTpPropertiesRom(rmBufferedTpPropertiesRomIdx, memIdx);
  PduLengthType metaDataLength = PduR_GetMetaDataLengthOfRmBufferedTpPropertiesRom(rmBufferedTpPropertiesRomIdx, memIdx);
  PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedTpPropertiesRom(rmBufferedTpPropertiesRomIdx, memIdx);

  PduR_FmFifoElementRamIterType allocatedFmFifoElementRamIdx = 0u;
  PduR_BmTxBufferRomIdxOfBmTxBufferIndRomType allocatedBmTxBufferRomIdx = 0u;

  if(PduR_Fm_AllocateFifoElement(fmFifoRomIdx, memIdx, PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx), TpSduLength, TpSduLength + metaDataLength, tpThreshold + metaDataLength, &allocatedFmFifoElementRamIdx, &allocatedBmTxBufferRomIdx) == E_OK) /* PRQA S 2985 */ /* MD_PduR_2985_2986 */
  {
    /* Assign allocated FIFO element to this source. */
    PduR_SetFmFifoElementRamIdxOfRmBufferedTpPropertiesRam(rmBufferedTpPropertiesRomIdx, (PduR_FmFifoElementRamIdxOfRmBufferedTpPropertiesRamType) allocatedFmFifoElementRamIdx, memIdx);

    if((metaDataLength > 0u) && (PduR_RmTp_RxInst_QueueMetaData(allocatedBmTxBufferRomIdx, memIdx, metaDataLength, info->SduLength, info->SduDataPtr) != BUFREQ_OK)) /* PRQA S 3415 */ /* MD_PduR_3415 */
    {
      PduR_Reporting_ReportError(PDUR_RXSM_STARTOFRECEPTION, PDUR_E_FATAL);
      retVal = BUFREQ_E_NOT_OK;
    }
    else
    {
      retVal = BUFREQ_OK;

      PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(rmBufferedTpPropertiesRomIdx, PDUR_RM_RX_ACTIVE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);

      /* Return the buffer size. */
      *bufferSizePtr = (PduLengthType) PduR_Bm_GetTotalBufferSize(allocatedBmTxBufferRomIdx, memIdx);

      /* Is this Pdu the first one in the FIFO? */
      if(PduR_RmTp_RxInst_IsSrcPduInstanceFirstInQueue(rmSrcRomIdx) == TRUE)
      {
        if(PduR_RmTp_RxInst_ThresholdReached(rmSrcRomIdx) == E_OK)
        {
          if(PduR_RmTp_TriggerTransmissionOfDestinations(rmSrcRomIdx, PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(rmSrcRomIdx)) == E_OK)
          {
            PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(rmBufferedTpPropertiesRomIdx, PDUR_RM_RX_ACTIVE_TX_TRIGGERED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
          }
          else
          {
            /* Reset srcPdu. */
            PduR_SetFmFifoElementRamIdxOfRmBufferedTpPropertiesRam(rmBufferedTpPropertiesRomIdx, PDUR_NO_FMFIFOELEMENTRAMIDXOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
            PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(rmBufferedTpPropertiesRomIdx, PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
            /* This was the last API call for this routing. Reception is finished and Transmission idle. Finish this routing. */
            PduR_RmTp_FinishRouting(fmFifoRomIdx, memIdx, FALSE, FALSE, PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(rmSrcRomIdx));
            retVal = BUFREQ_E_NOT_OK;
          }
        }
      }
    }
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CopyRxData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_CopyRxData(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                        P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(info == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else if((info->SduLength > 0u) && (info->SduDataPtr == NULL_PTR))
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else if(bufferSizePtr == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    PduR_Lock_LockRoutingPathBySrcPdu(rmSrcRomIdx);
    {
      /* Handle src instance routing state */
      retVal = PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler(rmSrcRomIdx, info, bufferSizePtr);
    }
    PduR_Lock_UnlockRoutingPathBySrcPdu(rmSrcRomIdx);
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_RMTP_CPYRX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler
 *********************************************************************************************************************/
  /*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CONST) PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler(PduR_RmSrcRomIterType rmSrcRomIdx,
                                                                            P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                            P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  switch (PduR_GetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx)))
  {
    case PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_Reporting_ReportError(PDUR_RXSM_COPYRXDATA_UNEXPECTEDCALL, PDUR_E_UNEXPECTED_CALL);
      break;

    case PDUR_RM_RX_ACTIVE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      retVal = PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE(rmSrcRomIdx, info->SduLength, info->SduDataPtr, bufferSizePtr);
      break;

    case PDUR_RM_RX_ACTIVE_TX_TRIGGERED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      retVal = PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED(rmSrcRomIdx, info->SduLength, info->SduDataPtr, bufferSizePtr);
      break;

    case PDUR_RM_RX_ABORTED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      /* Do nothing and just return negative return value. */
      retVal = BUFREQ_E_NOT_OK;
      break;

    default: /* COV_PDUR_MISRA */
      /* If state is out-of-bounds: do nothing and return negative result. */
      retVal = BUFREQ_E_NOT_OK;
      break;
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE
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
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE(PduR_RmSrcRomIterType rmSrcRomIdx, PduLengthType pduLength,
                                                                                        SduDataPtrType sduDataPtr, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  PduR_BmTxBufferRomIterType bmTxBufferRomIdx = 0u;

  if(PduR_RmTp_RxInst_GetCurrentlyUsedBmTxBufferRamIdxByRmSrcIdx(rmSrcRomIdx, &bmTxBufferRomIdx) == E_OK)
  {
    PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);
    PduR_RmBufferedTpPropertiesRomIdxOfRmSrcRomType rmBufferedTpPropertiesRomIdx = PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx);

    retVal = PduR_Bm_PutData(bmTxBufferRomIdx, pduLength, sduDataPtr, memIdx);

    if(retVal == BUFREQ_OK)
    {
      if(PduR_RmTp_RxInst_IsSrcPduInstanceFirstInQueue(rmSrcRomIdx) == TRUE)
      {
        if(PduR_RmTp_RxInst_ThresholdReached(rmSrcRomIdx) == E_OK)
        {
          if(PduR_RmTp_TriggerTransmissionOfDestinations(rmSrcRomIdx, PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(rmSrcRomIdx)) == E_OK)
          {
            PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(rmBufferedTpPropertiesRomIdx, PDUR_RM_RX_ACTIVE_TX_TRIGGERED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
            retVal = BUFREQ_OK;
          }
          else
          {
            retVal = BUFREQ_E_NOT_OK;
          }
        }
      }
    }
    *bufferSizePtr = (PduLengthType) PduR_Bm_GetTotalBufferSize(bmTxBufferRomIdx, memIdx);
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_CopyRxData_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED(PduR_RmSrcRomIterType rmSrcRomIdx, PduLengthType pduLength,
                                                                                        SduDataPtrType sduDataPtr, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  PduR_BmTxBufferRomIterType bmTxBufferRomIdx = 0u;

  if(PduR_RmTp_RxInst_GetCurrentlyUsedBmTxBufferRamIdxByRmSrcIdx(rmSrcRomIdx, &bmTxBufferRomIdx) == E_OK)
  {
    PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);

    retVal = PduR_Bm_PutData(bmTxBufferRomIdx, pduLength, sduDataPtr, memIdx);

    *bufferSizePtr = (PduLengthType) PduR_Bm_GetTotalBufferSize(bmTxBufferRomIdx, memIdx);
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_TpRxIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_TpRxIndication(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result)
{
  /* ----- Implementation ----------------------------------------------- */
  PduR_Lock_LockRoutingPathBySrcPdu(rmSrcRomIdx);
  {
    /* Handle src instance routing state */
    PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler(rmSrcRomIdx, result);
  }
  PduR_Lock_UnlockRoutingPathBySrcPdu(rmSrcRomIdx);
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler
 *********************************************************************************************************************/
  /*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result)
{
  switch (PduR_GetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx)))
  {
    case PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_Reporting_ReportError(PDUR_RMTP_RXSM_TPRXINDICATION_UNEXPECTEDCALL, PDUR_E_UNEXPECTED_CALL);
      break;

    case PDUR_RM_RX_ACTIVE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_OR_RM_RX_ABORTED(rmSrcRomIdx, result);
      break;

    case PDUR_RM_RX_ACTIVE_TX_TRIGGERED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED(rmSrcRomIdx, result);
      break;

    case PDUR_RM_RX_ABORTED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      /* Overwrite reception result to be unsuccessful: this hinders the function to trigger a transmission and clears Pdus from the queue if they are not in the first slot of the FIFO. */
      PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_OR_RM_RX_ABORTED(rmSrcRomIdx, E_NOT_OK);
      break;

    default: /* COV_PDUR_MISRA */
      /* If state is out-of-bounds: do nothing. */
      break;
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_OR_RM_RX_ABORTED
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
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_OR_RM_RX_ABORTED(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result)
{
  PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);
  PduR_FmFifoRamIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedTpPropertiesRom(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), memIdx);

  /* Is this rmSrcRomIdx Pdu instance the oldest Pdu in the queue? */
  if(PduR_RmTp_RxInst_IsSrcPduInstanceFirstInQueue(rmSrcRomIdx) == TRUE)
  {
    if(result == E_OK)
    {
      (void) PduR_RmTp_TriggerTransmissionOfDestinations(rmSrcRomIdx, PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(rmSrcRomIdx));
    }
    /* Reset srcPdu. */
    PduR_SetFmFifoElementRamIdxOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PDUR_NO_FMFIFOELEMENTRAMIDXOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
    PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);

    /* This is the last API call on Rx side. Reception finished, transmission may be active. Maybe finish the routing. */
    /* No need to cancel the transmission. If the reception was not successful, the transmission would not have been started. */
    PduR_RmTp_FinishRouting(fmFifoRomIdx, memIdx, FALSE, FALSE, PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(rmSrcRomIdx));
  }
  else
  {
    if(result != E_OK)
    {
      PduR_FmFifoElementRamIterType fmFifoElementRamWriteIdx = 0u;

      if(PduR_RmTp_RxInst_GetCurrentlyWrittenFmFifoElementRamIdxByRmSrcIdx(rmSrcRomIdx, &fmFifoElementRamWriteIdx) == E_OK)
      {
        PduR_Fm_ReleaseFifoElement(fmFifoElementRamWriteIdx, memIdx);
      }
    }
    /* Reset srcPdu. */
    PduR_SetFmFifoElementRamIdxOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PDUR_NO_FMFIFOELEMENTRAMIDXOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
    PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED
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
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_TpRxIndication_TpRxSmStateHandler_RM_RX_ACTIVE_TX_TRIGGERED(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result)
{
  PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);
  PduR_FmFifoRamIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedTpPropertiesRom(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), memIdx);

  /* No check 'PduR_RmTp_RxInst_IsSrcPduInstanceFirstInQueue' is needed. If it's in RX_ACTIVE_TX_TRIGGERED state, it has to be the first Pdu in the queue. */

  /* Reset srcPdu. */
  PduR_SetFmFifoElementRamIdxOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PDUR_NO_FMFIFOELEMENTRAMIDXOFRMBUFFEREDTPPROPERTIESRAM, memIdx);
  PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, memIdx);

  /* This is the last API call on Rx side. Reception is finished, transmission may be active. Maybe finishes the routing. */
  /* If the reception was not successful, the current transmission can be cancelled. */
  PduR_RmTp_FinishRouting(fmFifoRomIdx, memIdx, FALSE, (result != E_OK), PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(rmSrcRomIdx));
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler
 *********************************************************************************************************************/
  /*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler(PduR_RmSrcRomIterType rmSrcRomIdx)
{
  switch (PduR_GetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx)))
  {
    case PDUR_RM_RX_IDLE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_Reporting_ReportError(PDUR_RXSM_TPCANCELRECEIVE_FATALERROR, PDUR_E_FATAL);
      break;

    case PDUR_RM_RX_ACTIVE_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler_RM_RX_ACTIVE(rmSrcRomIdx);
      break;

    case PDUR_RM_RX_ACTIVE_TX_TRIGGERED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler_RM_RX_ACTIVE(rmSrcRomIdx);
      break;

    case PDUR_RM_RX_ABORTED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM:
      PduR_Reporting_ReportError(PDUR_RXSM_TPCANCELRECEIVE_FATALERROR, PDUR_E_FATAL);
      break;

    default: /* COV_PDUR_MISRA */
      /* If state is out-of-bounds: do nothing. */
      break;
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler_RM_RX_ACTIVE
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler_RM_RX_ACTIVE(PduR_RmSrcRomIterType rmSrcRomIdx)
{
  PduR_SetTpRxSmStateOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), PDUR_RM_RX_ABORTED_TPRXSMSTATEOFRMBUFFEREDTPPROPERTIESRAM, PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx));
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_IsSrcPduInstanceFirstInQueue
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_RmTp_RxInst_IsSrcPduInstanceFirstInQueue(PduR_RmSrcRomIterType rmSrcRomIdx)
{
  boolean retVal = FALSE;
  PduR_RmBufferedTpPropertiesRomIdxOfRmSrcRomType rmBufferedTpPropertiesRomIdx = PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx);
  PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);
  PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedTpPropertiesRom(rmBufferedTpPropertiesRomIdx, memIdx);

  PduR_FmFifoElementRamIterType fmFifoElementRamReadIdx = PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(fmFifoRomIdx, memIdx);
  PduR_FmFifoElementRamIterType fmFifoElementRamWriteIdx = 0u;

  if(PduR_RmTp_RxInst_GetCurrentlyWrittenFmFifoElementRamIdxByRmSrcIdx(rmSrcRomIdx, &fmFifoElementRamWriteIdx) == E_OK)
  {
    if(fmFifoElementRamWriteIdx == fmFifoElementRamReadIdx)
    {
      retVal = TRUE;
    }
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_ThresholdReached
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_ThresholdReached(PduR_RmSrcRomIterType rmSrcRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_BmTxBufferRomIterType bmTxBufferRomIdx = 0u;

  if(PduR_RmTp_RxInst_GetCurrentlyUsedBmTxBufferRamIdxByRmSrcIdx(rmSrcRomIdx, &bmTxBufferRomIdx) == E_OK)
  {
    PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);
    PduLengthType tpThreshold = PduR_GetTpThresholdOfRmBufferedTpPropertiesRom(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), memIdx);
    PduR_BmTxBufferArrayRamIterType bufferSize = PduR_Bm_GetSizeOfTxBuffer(bmTxBufferRomIdx, memIdx);
    PduR_BmTxBufferArrayRamIterType totalFreeBufferSize = PduR_Bm_GetTotalBufferSize(bmTxBufferRomIdx, memIdx);
    uint8 metaDataLength = PduR_GetMetaDataLengthOfRmBufferedTpPropertiesRom(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), memIdx);

    PduR_BmTxBufferArrayRamIterType availableTotalDataSize = bufferSize - totalFreeBufferSize;
    /* MetaData length is not taken into account to calculate the Threshold */
    availableTotalDataSize -= metaDataLength;     /* PRQA S 2985 */ /* MD_PduR_2985_2986 */

    /* Threshold reached */
    if(availableTotalDataSize >= tpThreshold)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_GetCurrentlyUsedBmTxBufferRamIdxByRmSrcIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_GetCurrentlyUsedBmTxBufferRamIdxByRmSrcIdx(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduR_BmTxBufferRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_FmFifoElementRamIterType fmFifoElementRamWriteIdx = 0u;

  if(PduR_RmTp_RxInst_GetCurrentlyWrittenFmFifoElementRamIdxByRmSrcIdx(rmSrcRomIdx, &fmFifoElementRamWriteIdx) == E_OK)
  {
    retVal = PduR_Fm_GetCurrentlyUsedBmTxBufferRomIdxByFmFifoElementIdx(fmFifoElementRamWriteIdx, bmTxBufferRomIdx, PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx));
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_GetCurrentlyWrittenFmFifoElementRamIdxByRmSrcIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_GetCurrentlyWrittenFmFifoElementRamIdxByRmSrcIdx(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduR_FmFifoElementRamIterType, AUTOMATIC, PDUR_APPL_DATA) fmFifoElementRamIdx)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_RmBufferedTpPropertiesRomPartitionIdxOfRmSrcRomType memIdx = PduR_GetRmBufferedTpPropertiesRomPartitionIdxOfRmSrcRom(rmSrcRomIdx);
  PduR_FmFifoRamIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmBufferedTpPropertiesRom(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), memIdx);
  PduR_FmFifoElementRamStartIdxOfFmFifoRomType fmFifoElementRamStartIdx = PduR_GetFmFifoElementRamStartIdxOfFmFifoRom(fmFifoRomIdx, memIdx);
  PduR_FmFifoElementRamEndIdxOfFmFifoRomType   fmFifoElementRamEndIdx   = PduR_GetFmFifoElementRamEndIdxOfFmFifoRom(fmFifoRomIdx, memIdx);

  PduR_FmFifoElementRamIterType readFmFifoElementRamIdx = PduR_GetFmFifoElementRamIdxOfRmBufferedTpPropertiesRam(PduR_GetRmBufferedTpPropertiesRomIdxOfRmSrcRom(rmSrcRomIdx), memIdx);

  if(readFmFifoElementRamIdx == PDUR_NO_FMFIFOELEMENTRAMIDXOFRMBUFFEREDTPPROPERTIESRAM)
  {
    retVal = E_NOT_OK;
  }
# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  else if(readFmFifoElementRamIdx >= fmFifoElementRamEndIdx)
  {
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETCURRENTLYWRITTENFIFOELEMENT, PDUR_E_IDXOUTOFBOUNDS);
    retVal = E_NOT_OK;
  }
  else if(readFmFifoElementRamIdx < fmFifoElementRamStartIdx)
  {
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETCURRENTLYWRITTENFIFOELEMENT, PDUR_E_IDXOUTOFBOUNDS);
    retVal = E_NOT_OK;
  }
# endif
  else if((readFmFifoElementRamIdx >= fmFifoElementRamStartIdx) && (readFmFifoElementRamIdx < fmFifoElementRamEndIdx))  /* COV_PDUR_VCA_CHECK */
  {
    *fmFifoElementRamIdx = readFmFifoElementRamIdx;
    retVal = E_OK;
  }
  else
  {
    /* Nothing should be done here*/
  }

  /* @ assert retVal != E_OK || (*fmFifoElementRamIdx < PduR_GetSizeOfFmFifoElementRam(memIdx)); */ /* VCA_PDUR_EXTENDED_ERROR_CHECK */
  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_IsReceptionFinished
 *********************************************************************************************************************/
  /*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmTp_RxInst_IsReceptionFinished(PduR_RmSrcRomIterType rmSrcRomIdx, PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx)
{
  boolean retVal = FALSE;
  PduR_FmFifoElementRamIterType fmFifoElementRamWriteIdx = 0u;

  if(PduR_RmTp_RxInst_GetCurrentlyWrittenFmFifoElementRamIdxByRmSrcIdx(rmSrcRomIdx, &fmFifoElementRamWriteIdx) == E_OK)
  {
    PduR_FmFifoElementRamIterType fmFifoElementRamReadIdx = PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(fmFifoRomIdx, memIdx);

    if(fmFifoElementRamWriteIdx != fmFifoElementRamReadIdx)
    {
      retVal = TRUE;
    }
    else
    {
      retVal = FALSE;
    }
  }
  else
  {
    retVal = TRUE;
  }

  return retVal;
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp_RxInst.c
 *********************************************************************************************************************/
