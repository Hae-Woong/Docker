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
 *         \file  PduR_RmTp_TxInst.c
 *         \unit  RmTp_TxInst
 *        \brief  Pdu Router Routing Manager Transport Protocol Tx Instance source file
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

#define PDUR_RMTP_TXINST_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Bm.h"
#include "PduR_Fm.h"
#include "PduR_Mm.h"
#include "PduR_RmTp.h"
#include "PduR_RmTp_TxInst.h"
#include "PduR_Lock.h"
#include "PduR_EvC.h"
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

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_CopyTxDataInternal
 *********************************************************************************************************************/
/*! \brief       This function copies the data of the Pdu in the queue to the provided 'info' pointer.
 *  \details     -
 *  \param[in]   rmGDestRomIdx     Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   info              Data pointer and length.
 *                                 Data will be copied to 'info->SduDataPtr' up to a maximum length of 'info->SduLength'.
 *                                 A info->SduLength of 0 can be used to get the available data size via 'availableDataPtr'.
 *  \param[in]   retry             not supported
 *  \param[out]  availableDataPtr  Indicates the remaining number of bytes that are available in the PduR buffer.
 *  \return      BufReq_ReturnType
 *               BUFREQ_OK         The data has been copied successfully.
 *               BUFREQ_E_NOT_OK   The data has not been copied.
 *               BUFREQ_E_BUSY     The request cannot be processed because no data is available. No data has been copied.
 *                                 The lower layer module might retry it later.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 ***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_CopyTxDataInternal(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                               P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr);
# endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_LoAndUpTransmit
 *********************************************************************************************************************/
/*! \brief      This function triggers the transmission on the destination.
 *  \details    In case of an upper layer destination, StartOfReception/CopyRxData/TpRxIndication API of the upper layer is called in this context.
 *              In case of an lower layer destination, it calls the destination module transmit function and handles the state machine accordingly.
 *  \param[in]  rmGDestRomIdx    Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \return     E_OK             Transmission successful.
 *              E_NOT_OK         Transmission failed.
 *  \pre        -
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_LoAndUpTransmit(PduR_RmGDestRomIterType rmGDestRomIdx);
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_UpTransmit
 *********************************************************************************************************************/
/*! \brief      This function transmits the Pdu in the queue (via reception APIs) to an upper layer destination.
 *  \details    This is done via the reception APIs StartOfReception/CopyRxData/TpRxIndication.
 *              The reception is finished in this context and the 'transmission' state is idle afterwards.
 *  \param[in]  rmGDestRomIdx    Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \return     E_OK             Transmission successful.
 *              E_NOT_OK         Transmission failed.
 *  \pre        -
 *  \trace      SPEC-2020176
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_UpTransmit(PduR_RmGDestRomIterType rmGDestRomIdx);
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_LoTransmit
 *********************************************************************************************************************/
/*! \brief      This function transmit the Pdu in the queue to a lower layer destination.
 *  \details    The transmission is done via the Transmit API of the lower layer. CopyTxData and TpTxConfirmation has to be called by the lower layer module.
 *  \param[in]  rmGDestRomIdx   Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \return     E_OK            Transmission successful.
 *              E_NOT_OK        Transmission not successful.
 *  \pre        -
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_LoTransmit(PduR_RmGDestRomIterType rmGDestRomIdx);
#endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TriggerTransmit_Immediate
 *********************************************************************************************************************/
/*! \brief       This function triggers the transmission to the destination immediately.
 *  \details     The PduRDestPdu is set to 'Immediate Processing'.
 *  \param[in]   rmGDestRomIdx  Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \return      E_OK           TriggerTransmission successful.
 *               E_NOT_OK       TriggerTransmission failed.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 ***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_TriggerTransmit_Immediate(PduR_RmGDestRomIterType rmGDestRomIdx);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TriggerTransmit_Deferred
 *********************************************************************************************************************/
/*! \brief       This function triggers the transmission in the next MainFunction call.
 *  \details     The PduRDestPdu is set to 'Deferred Processing'.
 *  \param[in]   rmGDestRomIdx  Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   applicationContext Application call context
 *  \return      E_OK           Transmission for deferred destinations is always successful in this context.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous FALSE
 ***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_TriggerTransmit_Deferred(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_CopyTxData_RmTxInstActive
 *********************************************************************************************************************/
/*! \brief       This function copies the data from the queue to the provided sduDataPtr and returns the remaining available data length.
 *  \details     -
 *  \param[in]   rmGDestRomIdx     Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   pduLength         Length of the buffer behind sduDataPtr.
 *                                 A length of 0 can be used to get the available data size via availableDataPtr.
 *  \param[out]  sduDataPtr        Pointer to the buffer of the lower layer.
 *  \param[in]   retry             not supported
 *  \param[out]  availableDataPtr  Indicates the remaining number of bytes that are available in the PduR Tx buffer.
 *  \return      BUFREQ_OK         The data has been copied to the transmit buffer successfully.
 *               BUFREQ_E_NOT_OK   No data has been copied.
 *               BUFREQ_E_BUSY     The request cannot be processed because no data is available. No data has been copied.
 *                                 The lower layer module might retry it later.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 ***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_CopyTxData_RmTxInstActive(PduR_RmGDestRomIterType rmGDestRomIdx, PduLengthType pduLength,
                                                                              SduDataPtrType sduDataPtr,
                                                                              P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC,
                                                                                                                                             PDUR_APPL_DATA) availableDataPtr);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TxConfirmation_RmTxInstActiveOrRmTxInstAborted
 *********************************************************************************************************************/
/*! \brief       This function confirms a transmission of a transport protocol Pdu.
 *  \details     Finishes the transmission for this destination and releases the corresponding used resources.
 *  \param[in]   rmGDestRomIdx   Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   result          Result of the TP transmission
 *               E_OK            The TP transmission has been completed successfully.
 *               E_NOT_OK        The TP transmission has been aborted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_TxConfirmation_RmTxInstActiveOrRmTxInstAborted(PduR_RmGDestRomIterType rmGDestRomIdx, Std_ReturnType result);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_AssignTxBuffer2Destination
 *********************************************************************************************************************/
/*! \brief       This function assigns the bmTxBuffer object to the mentioned destination.
 *               This will assign a read object (bmTxBufferInstance) so this destination can read from the buffer.
 *  \details     -
 *  \param[in]   rmGDestRomIdx             Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   bmTxBufferInstanceRomIdx  Valid BmTxBufferInstanceRom table index. Internal handle to an object which is used to read from a PduRTxBuffer.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_AssignTxBuffer2Destination(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_ResetTxBuffer2DestinationAssignment
 *********************************************************************************************************************/
/*! \brief       Resets the TxBufferInstance assignment of the destination.
 *  \details     -
 *  \param[in]   rmGDestRomIdx             Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_ResetTxBuffer2DestinationAssignment(PduR_RmGDestRomIterType rmGDestRomIdx);
# endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_GetCurrentlyUsedBmTxBufferInstanceIdxByRmGDestIdx
 *********************************************************************************************************************/
/*! \brief       Return the currently used BmTxBufferInstanceRomIdx for this global destination.
 *  \details     -
 *  \param[in]   rmGDestRomIdx             Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[out]  bmTxBufferInstanceRomIdx  The read BmTxBufferInstanceRom table index is returned via this parameter.
 *  \return      E_OK                      The read bmTxBufferInstanceRomIdx is valid and can be used.
 *               E_NOT_OK                  The read bmTxBufferInstanceRomIdx is invalid and can not be used.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_GetCurrentlyUsedBmTxBufferInstanceIdxByRmGDestIdx(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduR_BmTxBufferInstanceRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferInstanceRomIdx);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_Init
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_Init(PduR_MemIdxType memIdx)
{
  PduR_RmGDestQueuedTpRamIterType rmGDestQueuedTpRamIdx;

  for(rmGDestQueuedTpRamIdx = 0u; rmGDestQueuedTpRamIdx < PduR_GetSizeOfRmGDestQueuedTpRam(memIdx); rmGDestQueuedTpRamIdx++)  /* FETA_PDUR_01 */
  {
    PduR_SetBmTxBufferInstanceRomIdxOfRmGDestQueuedTpRam(rmGDestQueuedTpRamIdx, PDUR_NO_BMTXBUFFERINSTANCEROMIDXOFRMGDESTQUEUEDTPRAM, memIdx);
    PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(rmGDestQueuedTpRamIdx, PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, memIdx);
    PduR_SetInTransmissionContextOfRmGDestQueuedTpRam(rmGDestQueuedTpRamIdx, FALSE, memIdx);
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_CopyTxData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_CopyTxData(PduR_RmGDestRomIterType rmGDestRomIdx,
                                                                     P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                     P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  /* parameter 'Handle Id' already checked by the caller API */
  /* Check parameter 'info' */
  if(info == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else if((info->SduLength > 0u) && (info->SduDataPtr == NULL_PTR))
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else if(availableDataPtr == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    PduR_Lock_LockRoutingPathByGDestPdu(rmGDestRomIdx);
    {
      retVal = PduR_RmTp_TxInst_CopyTxDataInternal(rmGDestRomIdx, info, retry, availableDataPtr);
    }
    PduR_Lock_UnlockRoutingPathByGDestPdu(rmGDestRomIdx);
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_RMTP_CPYTX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_UpTransmit
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
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_UpTransmit(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType retValBufReq = BUFREQ_E_NOT_OK;
  PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx = 0u;

  PduR_RmGDestQueuedTpRomIdxOfRmGDestRomType rmGDestQueuedTpRomIdx = PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmGDestQueuedTpRom(rmGDestQueuedTpRomIdx, memIdx);

  PduR_FmFifoElementRamIterType fmFifoElementRamReadIdx = PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(fmFifoRomIdx, memIdx);

  if(PduR_Fm_GetBmTxBufferInstanceByFmFifoElement(fmFifoElementRamReadIdx, memIdx, &bmTxBufferInstanceRomIdx) == E_OK)
  {
    PduLengthType bufferSizePtr = 0u;
    uint8 metaDataLength = PduR_GetMetaDataLengthOfRmGDestQueuedTpRom(rmGDestQueuedTpRomIdx, memIdx);
    PduLengthType actualPduLengthToBeRouted = PduR_Bm_GetActualPduLengthToBeRouted(bmTxBufferInstanceRomIdx, memIdx);

    PduR_RmTp_TxInst_AssignTxBuffer2Destination(rmGDestRomIdx, bmTxBufferInstanceRomIdx);

    if(metaDataLength > 0u)
    {
      PduInfoType metaInfo;
      metaInfo = PduR_Bm_GetReadPtrToBuffer(bmTxBufferInstanceRomIdx, memIdx);

      if(metaInfo.SduLength >= metaDataLength)
      {
        metaInfo.SduLength = metaDataLength;

        retValBufReq = PduR_Mm_CallTpStartOfReception(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), &metaInfo, actualPduLengthToBeRouted, &bufferSizePtr);

        PduR_Bm_IncrementReadIdx(bmTxBufferInstanceRomIdx, memIdx, metaDataLength);
      }
    }
    else
    {
      retValBufReq = PduR_Mm_CallTpStartOfReception(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), (P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA)) NULL_PTR, actualPduLengthToBeRouted, &bufferSizePtr);
    }

    if(retValBufReq == BUFREQ_OK)
    {
      /* RfC 52242 - Transport Protocol Gateway Reception to upper layer modules is not functional */
      /* \trace SPEC-2020098 */
      if(bufferSizePtr >= actualPduLengthToBeRouted)
      {
        PduInfoType info;
        info = PduR_Bm_GetReadPtrToBuffer(bmTxBufferInstanceRomIdx, memIdx);

        if(BUFREQ_OK == PduR_Mm_CallTpCopyRxData(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), &info, &bufferSizePtr))
        {
          PduR_Bm_IncrementReadIdx(bmTxBufferInstanceRomIdx, memIdx, info.SduLength);
          retVal = E_OK;
        }
      }
      PduR_Mm_CallTpRxIndication(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), retVal);
    }
    PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, memIdx);
    PduR_Bm_ResetTxBufferInstance(bmTxBufferInstanceRomIdx, memIdx);
    PduR_RmTp_TxInst_ResetTxBuffer2DestinationAssignment(rmGDestRomIdx);
  }
  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_TxInst_LoTransmit
**********************************************************************************************************************/
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
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_LoTransmit(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx = 0u;

  PduR_RmGDestQueuedTpRomIdxOfRmGDestRomType rmGDestQueuedTpRomIdx = PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmGDestQueuedTpRom(rmGDestQueuedTpRomIdx, memIdx);

  PduR_FmFifoElementRamIterType fmFifoElementRamReadIdx = PduR_Fm_GetFmFifoElementRamReadIdxOfFmFifoRamSafe(fmFifoRomIdx, memIdx);

  if(PduR_Fm_GetBmTxBufferInstanceByFmFifoElement(fmFifoElementRamReadIdx, memIdx, &bmTxBufferInstanceRomIdx) == E_OK)
  {
    PduInfoType info;
    uint8 metaDataLength = PduR_GetMetaDataLengthOfRmGDestQueuedTpRom(rmGDestQueuedTpRomIdx, memIdx);
    PduLengthType actualPduLengthToBeRouted = PduR_Bm_GetActualPduLengthToBeRouted(bmTxBufferInstanceRomIdx, memIdx);

    PduR_RmTp_TxInst_AssignTxBuffer2Destination(rmGDestRomIdx, bmTxBufferInstanceRomIdx);

    /* Get Data Pointer for Transmit */
    info = PduR_Bm_GetReadPtrToBuffer(bmTxBufferInstanceRomIdx, memIdx);

    if(info.SduLength >= metaDataLength)/* Check if there is at least metaData length data in the buffer. */
    {
      /* Correction of the length according to AUTOSAR */
      info.SduLength = actualPduLengthToBeRouted;

      /* MetaData was 'read' from the buffer regardless of the success of the transmit API.
         This is done before the actual transmit call, because PduR could be called recursively inside the transmit call. */
      PduR_Bm_IncrementReadIdx(bmTxBufferInstanceRomIdx, memIdx, metaDataLength);

      PduR_SetInTransmissionContextOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, TRUE, memIdx);
      retVal = PduR_Mm_CallTpTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), &info);
      PduR_SetInTransmissionContextOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, FALSE, memIdx);
    }
    else
    {
      retVal = E_NOT_OK;
    }

    if(retVal != E_OK)
    {
      PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, memIdx);
      PduR_Bm_ResetTxBufferInstance(bmTxBufferInstanceRomIdx, memIdx);
      PduR_RmTp_TxInst_ResetTxBuffer2DestinationAssignment(rmGDestRomIdx);
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_LoAndUpTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/

PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_LoAndUpTransmit(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(PduR_GetDirectionOfRmGDestRom(rmGDestRomIdx) == PDUR_TX_DIRECTIONOFRMGDESTROM)
  {
    retVal = PduR_RmTp_TxInst_LoTransmit(rmGDestRomIdx);
  }
  else
  {
    retVal = PduR_RmTp_TxInst_UpTransmit(rmGDestRomIdx);
  }
  return retVal;
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TriggerTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_TriggerTransmit(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(PduR_GetTpTxInstSmStateOfRmGDestQueuedTpRam(PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx))
      == PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM)
  {
    PduR_PduRDestPduProcessingOfRmGDestRomType destPduProcessing = PduR_GetPduRDestPduProcessingOfRmGDestRom(rmGDestRomIdx);

    if(destPduProcessing == PDUR_DEFERRED_PDURDESTPDUPROCESSINGOFRMGDESTROM)
    {
      retVal = PduR_RmTp_TxInst_TriggerTransmit_Deferred(rmGDestRomIdx, applicationContext);
    }
    else if(applicationContext != PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(rmGDestRomIdx))  /* COV_PDUR_MULTIPARTITION_ONLY */
    {
      retVal = PduR_RmTp_TxInst_TriggerTransmit_Deferred(rmGDestRomIdx, applicationContext);
    }
    else if(destPduProcessing == PDUR_IMMEDIATE_PDURDESTPDUPROCESSINGOFRMGDESTROM)  /* COV_PDUR_MISRA */
    {
      retVal = PduR_RmTp_TxInst_TriggerTransmit_Immediate(rmGDestRomIdx);
    }
    else
    {
      /* If neither immediate or deferred is set (due to wrong generation): do nothing. */
    }
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_RM_TPTRIGGERTRANSMIT_FATALERROR, PDUR_E_FATAL);
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TriggerTransmit_Immediate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_TriggerTransmit_Immediate(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx), PDUR_RM_TXINST_ACTIVE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx));
  return PduR_RmTp_TxInst_LoAndUpTransmit(rmGDestRomIdx);
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TriggerTransmit_Deferred
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_TriggerTransmit_Deferred(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext)
{
  Std_ReturnType retVal = E_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx), PDUR_RM_TXINST_WAITING_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx));

# if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)
  (void) PduR_EvC_WriteDeferredEventCache(rmGDestRomIdx, applicationContext);
# endif
  PDUR_DUMMY_STATEMENT(applicationContext);   /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_CopyTxDataInternal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_CopyTxDataInternal(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)      /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  switch (PduR_GetTpTxInstSmStateOfRmGDestQueuedTpRam(PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx)))
  {
    case PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      PduR_Reporting_ReportError(PDUR_RMTP_TXINSTSM_COPYTXDATA_UNEXPECTEDCALL, PDUR_E_UNEXPECTED_CALL);
      break;

    case PDUR_RM_TXINST_ACTIVE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      retVal = PduR_RmTp_TxInst_CopyTxData_RmTxInstActive(rmGDestRomIdx, info->SduLength, info->SduDataPtr, retry, availableDataPtr);
      break;

    case PDUR_RM_TXINST_WAITING_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      PduR_Reporting_ReportError(PDUR_RMTP_TXINSTSM_COPYTXDATA_UNEXPECTEDCALL, PDUR_E_UNEXPECTED_CALL);
      break;

    case PDUR_RM_TXINST_ABORTED_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      /* Do nothing, just return negative result. */
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
 * PduR_RmTp_TxInst_CopyTxData_RmTxInstActive
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_CopyTxData_RmTxInstActive(PduR_RmGDestRomIterType rmGDestRomIdx,
                                                                              PduLengthType pduLength,
                                                                              SduDataPtrType sduDataPtr,
                                                                              P2CONST(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

  PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx = 0u;

  if(PduR_RmTp_TxInst_GetCurrentlyUsedBmTxBufferInstanceIdxByRmGDestIdx(rmGDestRomIdx, &bmTxBufferInstanceRomIdx) == E_OK)
  {
    retVal = PduR_Bm_GetData(bmTxBufferInstanceRomIdx, pduLength, sduDataPtr, memIdx);

    *availableDataPtr = (PduLengthType) PduR_Bm_GetTotalDataSizeInstance(bmTxBufferInstanceRomIdx, memIdx);
  }

  PDUR_DUMMY_STATEMENT(retry);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_TxConfirmation(PduR_RmGDestRomIterType rmGDestRomIdx, Std_ReturnType result)
{
  switch (PduR_GetTpTxInstSmStateOfRmGDestQueuedTpRam(PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx)))
  {
    case PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      PduR_Reporting_ReportError(PDUR_RMTP_TXINSTSM_TPTXCONFIRMATION_UNEXPECTEDCALL, PDUR_E_UNEXPECTED_CALL);
      break;

    case PDUR_RM_TXINST_ACTIVE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      PduR_RmTp_TxInst_TxConfirmation_RmTxInstActiveOrRmTxInstAborted(rmGDestRomIdx, result);
      break;

    case PDUR_RM_TXINST_WAITING_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      PduR_Reporting_ReportError(PDUR_RMTP_TXINSTSM_TPTXCONFIRMATION_UNEXPECTEDCALL, PDUR_E_UNEXPECTED_CALL);
      break;

    case PDUR_RM_TXINST_ABORTED_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      PduR_RmTp_TxInst_TxConfirmation_RmTxInstActiveOrRmTxInstAborted(rmGDestRomIdx, result);
      break;

    default: /* COV_PDUR_MISRA */
      /* If state is out-of-bounds: do nothing. */
      break;
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TxConfirmation_RmTxInstActiveOrRmTxInstAborted
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
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_TxConfirmation_RmTxInstActiveOrRmTxInstAborted(PduR_RmGDestRomIterType rmGDestRomIdx, Std_ReturnType result)
{
  PduR_RmGDestQueuedTpRomIdxOfRmGDestRomType rmGDestQueuedTpRomIdx = PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmGDestQueuedTpRom(rmGDestQueuedTpRomIdx, memIdx);

  PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx = 0u;

  PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx), PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, memIdx);

  if(PduR_RmTp_TxInst_GetCurrentlyUsedBmTxBufferInstanceIdxByRmGDestIdx(rmGDestRomIdx, &bmTxBufferInstanceRomIdx) == E_OK)
  {
    PduR_Bm_ResetTxBufferInstance(bmTxBufferInstanceRomIdx, memIdx);
  }
  PduR_RmTp_TxInst_ResetTxBuffer2DestinationAssignment(rmGDestRomIdx);

  /* Do not finish the routing if the TxConfirmation is called directly in the DestModule_Transmit context. Finish Routing will be called after the transmit call. */
  if(!PduR_IsInTransmissionContextOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, memIdx))
  {
    /* Transmission on this destination is finished. Transmission may be active on other destinations. Reception may be active as well. Maybe finish the routing.
    * If entire transmission is finished and the result for this destination is not successful, the reception can be aborted. */
    PduR_RmTp_FinishRouting(fmFifoRomIdx, memIdx, (result != E_OK), FALSE, PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(rmGDestRomIdx));
  }
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_CancelTransmit
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
FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_CancelTransmit(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext)
{
  PduR_RmGDestQueuedTpRomIdxOfRmGDestRomType rmGDestQueuedTpRomIdx = PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmGDestQueuedTpRom(rmGDestQueuedTpRomIdx, memIdx);

  switch (PduR_GetTpTxInstSmStateOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, memIdx))
  {
    case PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      /* Nothing to abort. */
      break;

    case PDUR_RM_TXINST_ACTIVE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, PDUR_RM_TXINST_ABORTED_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, memIdx);
      break;

    case PDUR_RM_TXINST_WAITING_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
    {
      PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, memIdx);

      /* Transmission on this destination is finished (aborted). Transmission may be active on other destinations. Reception may be active as well. Maybe finish the routing.
       * If entire transmission is finished, the reception can be aborted. */
      PduR_RmTp_FinishRouting(fmFifoRomIdx, memIdx, TRUE, FALSE, applicationContext);
    }
      break;

    case PDUR_RM_TXINST_ABORTED_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM:
      /* Already aborted. Nothing to do. */
      break;

    default: /* COV_PDUR_MISRA */
      /* If state is out-of-bounds: do nothing. */
      break;
  }
}
#endif

#if(PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_MainFunction
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
FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_MainFunction(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_Lock_LockRoutingPathByGDestPdu(rmGDestRomIdx);
  {
    PduR_RmGDestQueuedTpRomIdxOfRmGDestRomType rmGDestQueuedTpRomIdx = PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx); /* PRQA S 2983 */ /* MD_PduR_2982_2983 */
    PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx); /* PRQA S 2983 */ /* MD_PduR_2982_2983 */

    if(PDUR_RM_TXINST_WAITING_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM == PduR_GetTpTxInstSmStateOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, memIdx))
    {
      PduR_FmFifoRomIterType fmFifoRomIdx = PduR_GetFmFifoRomIdxOfRmGDestQueuedTpRom(rmGDestQueuedTpRomIdx, memIdx);

      PduR_SetTpTxInstSmStateOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, PDUR_RM_TXINST_ACTIVE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM, memIdx);
      (void) PduR_RmTp_TxInst_LoAndUpTransmit(rmGDestRomIdx);

      /* Transmission on this destination is finished. Transmission may be active on other destinations. Reception may be active as well. Maybe finish the routing.
       * If entire transmission is finished, the reception can be aborted. */
      PduR_RmTp_FinishRouting(fmFifoRomIdx, memIdx, TRUE, FALSE, PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(rmGDestRomIdx));
    }
  }
  PduR_Lock_UnlockRoutingPathByGDestPdu(rmGDestRomIdx);
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_IsTxIdleOnAllQueuedDestinations
 *********************************************************************************************************************/
  /*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmTp_TxInst_IsTxIdleOnAllQueuedDestinations(PduR_RmSrcRomIterType rmSrcRomIdx)
{
  boolean retVal = TRUE;
  PduR_RmDestRomIterType rmDestRomIdx;

  for(rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx < PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx++)  /* FETA_PDUR_01 */
  {
    PduR_RmGDestRomIterType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);

    if(PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(rmGDestRomIdx))
    {
      if(PduR_GetTpTxInstSmStateOfRmGDestQueuedTpRam(PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx)) != PDUR_RM_TXINST_IDLE_TPTXINSTSMSTATEOFRMGDESTQUEUEDTPRAM)
      {
        retVal = FALSE;
        break;
      }
    }
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_AssignTxBuffer2Destination
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
  *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_AssignTxBuffer2Destination(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_BmTxBufferInstanceRomIterType bmTxBufferInstanceRomIdx)
{
  PduR_RmGDestQueuedTpRomIdxOfRmGDestRomType rmGDestQueuedTpRomIdx = PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

  PduR_SetBmTxBufferInstanceRomIdxOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, (PduR_BmTxBufferInstanceRomIdxOfRmGDestQueuedTpRamType)bmTxBufferInstanceRomIdx, memIdx);
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_ResetTxBuffer2DestinationAssignment
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
  *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_ResetTxBuffer2DestinationAssignment(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_RmGDestQueuedTpRomIdxOfRmGDestRomType rmGDestQueuedTpRomIdx = PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

  PduR_SetBmTxBufferInstanceRomIdxOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, PDUR_NO_BMTXBUFFERINSTANCEROMIDXOFRMGDESTQUEUEDTPRAM, memIdx);
}
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_GetCurrentlyUsedBmTxBufferInstanceIdxByRmGDestIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
  *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_GetCurrentlyUsedBmTxBufferInstanceIdxByRmGDestIdx(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduR_BmTxBufferInstanceRomIterType, AUTOMATIC, PDUR_APPL_DATA) bmTxBufferInstanceRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmGDestQueuedTpRomIdxOfRmGDestRomType rmGDestQueuedTpRomIdx = PduR_GetRmGDestQueuedTpRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmGDestQueuedTpRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestQueuedTpRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

  PduR_BmTxBufferInstanceRomIterType readBmTxBufferInstanceRomIdxOfRmGDestQueuedTpRam = PduR_GetBmTxBufferInstanceRomIdxOfRmGDestQueuedTpRam(rmGDestQueuedTpRomIdx, memIdx);

  if(readBmTxBufferInstanceRomIdxOfRmGDestQueuedTpRam == PDUR_NO_BMTXBUFFERINSTANCEROMIDXOFRMGDESTQUEUEDTPRAM)
  {
    retVal = E_NOT_OK;
  }
# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  else if(readBmTxBufferInstanceRomIdxOfRmGDestQueuedTpRam >= PduR_GetSizeOfBmTxBufferInstanceRom(memIdx))
  {
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETTXBUFFERINSTANCEOFFIFOINSTANCE, PDUR_E_IDXOUTOFBOUNDS);
    retVal = E_NOT_OK;
  }
# endif
  else
  {
    *bmTxBufferInstanceRomIdx = readBmTxBufferInstanceRomIdxOfRmGDestQueuedTpRam;
    retVal = E_OK;
  }
  /*@ assert retVal != E_OK || *bmTxBufferInstanceRomIdx < PduR_GetSizeOfBmTxBufferInstanceRom(memIdx); */
  return retVal;
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp_TxInst.c
 *********************************************************************************************************************/
