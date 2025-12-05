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
 *         \file  PduR_RmTp.h
 *         \unit  RmTp
 *        \brief  Pdu Router Routing Manager Transport Protocol header file
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
#if !defined (PDUR_RMTP_H)
# define PDUR_RMTP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
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

/**********************************************************************************************************************
 * PduR_RmTp_Transmit_MultiDest
 *********************************************************************************************************************/
/*!
 * \brief       The function calls the Transmit function for every destination
 * \details     -
 * \param[in]   rmSrcRomIdx       internal RmSrcRom index
 * \param[in]   info              Pointer to the data to be transmitted
 * \return      Std_ReturnType
 *              E_OK              Transmit to at least one destination was successful.
 *              E_NOT_OK          Transmit to ALL destinations was not successful.
 * \pre         -
 * \context     TASK|ISR1|ISR2
 * \reentrant   TRUE
 * \synchronous TRUE
 * \spec
 *   requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 * \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_Transmit_MultiDest(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_ActivateNext
 *********************************************************************************************************************/
/*! \brief       This function sends the next pending element in the FiFo to its destination.
 *  \details     This function continues to empty the queue until a transmission to any destination
 *               of the corresponding Pdu is successful and there a still CopyTxData and TpTxConfirmation calls pending.
 *  \param[in]   fmFifoRamIdx        Index of the FIFO.
 *  \param[in]   memIdx              memory section index
 *  \param[in]   applicationContext  Application call context
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRamIdx < PduR_GetSizeOfFmFifoRam(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_ActivateNext(PduR_FmFifoRamIterType fmFifoRamIdx, PduR_MemIdxType memIdx, PduR_DestApplicationManagerRomIterType applicationContext);
# endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TriggerTransmissionOfDestinations
 *********************************************************************************************************************/
/*! \brief       Trigger the transmission on all queued and enabled destinations.
 *  \details     -
 *  \param[in]   rmSrcRomIdx    internal RmSrcRom index
 *  \param[in]   applicationContext Application call context
 *  \return      E_OK           Transmission was successful on at least one destination
 *               E_NOT_OK       Transmission was not successful on any destination
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TriggerTransmissionOfDestinations(PduR_RmSrcRomIterType rmSrcRomIdx, PduR_DestApplicationManagerRomIterType applicationContext);
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_FinishRouting
 *********************************************************************************************************************/
/*! \brief       This function will finish the routing of the latest Pdu in the queue if both the reception and transmission is finished.
 *  \details     If Rx is still active, but Tx is already finished, the reception can be actively cancelled depending on cancelReception flag.
 *               If Tx is still active, but Rx is already finished, the transmission can be actively cancelled depending on cancelTransmission flag.
 *               Cancellation is not supported if both Rx and Tx is still active. This means the routing is still active to some of the destinations.
 *  \param[in]   fmFifoRomIdx       fmFifo index
 *  \param[in]   memIdx             memory section index
 *  \param[in]   cancelReception    cancel the reception, if reception is still active and transmission is finished.
 *  \param[in]   cancelTransmission cancel the transmission, if transmission is still active and reception is finished.
 *  \param[in]   applicationContext Application call context
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_FinishRouting(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx, boolean cancelReception, boolean cancelTransmission, PduR_DestApplicationManagerRomIterType applicationContext);
#endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
 /**********************************************************************************************************************
 * PduR_RmTp_CancelTransmitOnDestinations
 *********************************************************************************************************************/
/*! \brief       This function cancels the transmission on all destinations of a source.
 *  \details     -
 *  \param[in]   rmSrcRomIdx  rmSrc index
 *  \param[in]   applicationContext Application call context
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_CancelTransmitOnDestinations(PduR_RmSrcRomIterType rmSrcRomIdx, PduR_DestApplicationManagerRomIterType applicationContext);
# endif

# if(PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
 /**********************************************************************************************************************
 * PduR_RmTp_GetValidTpSrcPduFromQueue
 *********************************************************************************************************************/
/*! \brief       Read and return source Pdu which is stored in the oldest element in the FIFO queue.
 *  \details     The read source Pdu ID is validated before being returned.
 *  \param[in]   fmFifoRomIdx Valid FmFifoRom table index. Internal handle of a PduRSharedBufferQueue.
 *  \param[in]   memIdx       memory section index
 *  \param[out]  rmSrcRomIdx     rmSrc index
 *  \return      E_OK         The read rmSrcRomIdx is valid and can be used.
 *               E_NOT_OK     The read rmSrcRomIdx is invalid and can NOT be used.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *    requires fmFifoRomIdx < PduR_GetSizeOfFmFifoRom(memIdx);
 *    requires rmSrcRomIdx != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_GetValidTpSrcPduFromQueue(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx, P2VAR(PduR_RmSrcRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmSrcRomIdx);
# endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RMTP_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp.h
 *********************************************************************************************************************/
