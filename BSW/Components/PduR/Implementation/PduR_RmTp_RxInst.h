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
 *         \file  PduR_RmTp_RxInst.h
 *         \unit  RmTp_RxInst
 *        \brief  Pdu Router Routing Manager Transport Protocol Rx Instance header file
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
#if !defined (PDUR_RMTP_RXINST_H)
# define PDUR_RMTP_RXINST_H

/**********************************************************************************************************************
   LOCAL MISRA / PCLINT JUSTIFICATION
**********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA  S 0777 EOF */ /* MD_MSR_Rule5.1_0777 */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* \trace SPEC-2020230, SPEC-38402 */

# include "PduR_Types.h"
# include "PduR_Cfg.h"
# include "PduR_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define PDUR_START_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_Init
 *********************************************************************************************************************/
/*! \brief       This function initializes this unit.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_Init(PduR_MemIdxType memIdx);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_RxInst_UpStartOfReception
**********************************************************************************************************************/
/*! \brief       This function can be called to 'receive' (put) data into the TP queue via the Transmit APIs. This API corresponds the normal Transmit API and has the same signature.
 *  \details     In this API, the upper layer CopyTxData and TpTxConfirmation will be called. All data has to be provided in the one CopyTxData call.
 *               The data will be written to the TP queue via the other public APIs of this unit.
 *  \param[in]   rmSrcRomIdx       Routing Manager source Id
 *  \param[in]   info              Pointer to the buffer (SduDataPtr) contains MetaData if this feature is enabled
 *  \return      Std_ReturnType
 *               E_OK              The Pdu has been put to the queue successfully.
 *               E_NOT_OK          The Pdu couldn't be put into the queue.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
***********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_UpStartOfReception(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_RxInst_StartOfReception
**********************************************************************************************************************/
/*! \brief       This function starts the reception of a TP Pdu.
 *  \details     It will allocate a slot in the fifo queue and get a corresponding buffer for it.
 *  \param[in]   rmSrcRomIdx       Routing Manager source Id
 *  \param[in]   info              Pointer to the buffer (SduDataPtr) and its length (SduLength). Only used optionally for meta data.
 *  \param[in]   TpSduLength       Length of the entire TP PDU which will be received.
 *  \param[out]  bufferSizePtr     Pointer to the bufferSize. The available buffer size will be written to this variable.
 *  \return
 *               BUFREQ_OK         StartOfReception request accepted.
 *               BUFREQ_E_NOT_OK   StartOfReception request not accepted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
***********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_StartOfReception(PduR_RmSrcRomIterType rmSrcRomIdx,
                                                              P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                              PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_RxInst_CopyRxData
**********************************************************************************************************************/
/*! \brief       This function copies data of a TP Pdu into the queue.
 *  \details     This API can be called multiple times to copy the data in small chunks.
 *  \param[in]   rmSrcRomIdx       Routing Manager source Id
 *  \param[in]   info              Pointer to the buffer (SduDataPtr) and its length (SduLength)
 *  \param[out]  bufferSizePtr     Available receive buffer after data has been copied.
 *  \return
 *               BUFREQ_OK         Copying was successful.
 *               BUFREQ_E_NOT_OK   Copying failed.
 *               BUFREQ_E_BUSY     Copying failed. Currently not enough free space in the buffer for this pduLength.
 *                                It's up the requester to retry request for a certain time.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_CopyRxData(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                        P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_RxInst_TpRxIndication
**********************************************************************************************************************/
/*! \brief       This function finishes the reception of the TP Pdu.
 *  \details     The result can be either successful or not successful.
 *  \param[in]   rmSrcRomIdx      Routing Manager source Id
 *  \param[out]  result           Result of the TP reception.
 *               E_OK             The TP reception has been completed successfully.
 *               E_NOT_OK         The TP reception has not been completed successfully.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
***********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_TpRxIndication(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler
 *********************************************************************************************************************/
/*! \brief       This function aborts/cancels the reception of the TP Pdu.
 *  \details     After this function was called, all TP Rx APIs will return with negative result afterwards.
 *  \param[in]   rmSrcRomIdx         Routing Manager source Id
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_RxInst_CancelReceive_TpRxSmStateHandler(PduR_RmSrcRomIterType rmSrcRomIdx);
# endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_ThresholdReached
 *********************************************************************************************************************/
/*! \brief       This function calculates the fill level and returns true if the configured TP threshold is reached.
 *  \details     -
 *  \param[in]   rmSrcRomIdx   Routing Manager Src Index
 *  \return      E_OK:     If threshold is reached.
 *               E_NOT_OK: If threshold is not reached.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_ThresholdReached(PduR_RmSrcRomIterType rmSrcRomIdx);
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_GetCurrentlyWrittenFmFifoElementRamIdxByRmSrcIdx
 *********************************************************************************************************************/
/*! \brief       This function returns the currently written FmFifoElement index of this source Pdu.
 *  \details     -
 *  \param[in]   rmSrcRomIdx         Routing Manager Src Index
 *  \param[out]  fmFifoElementRamIdx fifo element index
 *  \return      E_OK                the read fmFifoElementRamIdx is valid and can be used
 *               E_NOT_OK            the read fmFifoElementRamIdx is invalid and can not be used
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_RxInst_GetCurrentlyWrittenFmFifoElementRamIdxByRmSrcIdx(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduR_FmFifoElementRamIterType, AUTOMATIC, PDUR_APPL_DATA) fmFifoElementRamIdx);
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_RxInst_IsReceptionFinished
 *********************************************************************************************************************/
/*! \brief       Is this source Pdu being received into the oldest fifo element of the fifo queue?
 *  \details     -
 *  \param[in]   rmSrcRomIdx  internal RmSrcRom index
 *  \param[in]   fmFifoRomIdx fmFifo index
 *  \param[in]   memIdx       memory section index
 *  \return      TRUE
 *               FALSE
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmTp_RxInst_IsReceptionFinished(PduR_RmSrcRomIterType rmSrcRomIdx, PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx);
#endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RMTP_RXINST_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp_RxInst.h
 *********************************************************************************************************************/
