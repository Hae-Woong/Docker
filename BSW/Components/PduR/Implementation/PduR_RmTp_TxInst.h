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
 *         \file  PduR_RmTp_TxInst.h
 *         \unit  RmTp_TxInst
 *        \brief  Pdu Router Routing Manager Transport Protocol Tx Instance header file
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
#if !defined (PDUR_RMTP_TXINST_H)
# define PDUR_RMTP_TXINST_H

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
 * PduR_RmTp_TxInst_Init
 *********************************************************************************************************************/
/*! \brief       This function initializes the RmTp_TxInst unit.
 *  \details     -
 *  \param[in]   memIdx  memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires memIdx < PduR_GetSizeOfPartitionIdentifiers();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_Init(PduR_MemIdxType memIdx);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_TriggerTransmit
 *********************************************************************************************************************/
/*! \brief       This function triggers the transmission on this destination.
 *  \details     -
 *  \param[in]   rmGDestRomIdx  Id of global PDU reference.
 *  \param[in]   applicationContext Application call context
 *  \return      E_OK           TriggerTransmission was successful.
 *               E_NOT_OK       TriggerTransmission failed.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
 ***********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_TriggerTransmit(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext);
# endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_TxInst_CopyTxData
**********************************************************************************************************************/
/*! \brief       This function checks the parameters of the CopyTxData API and forwards the call to the actual CopyTxData function if valid.
 *  \details     See documentation for PduR_RmTp_TxInst_CopyTxDataInternal for more details.
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
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_TxInst_CopyTxData(PduR_RmGDestRomIterType rmGDestRomIdx,
                                                                     P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                     P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr);
#endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
  PduR_RmTp_TxInst_TxConfirmation
**********************************************************************************************************************/
/*! \brief       This function confirms a transmission of a transport protocol Pdu.
 *  \details     -
 *  \param[in]   rmGDestRomIdx       Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   result              Result of the TP transmission
 *               E_OK                The TP transmission has been completed successfully.
 *               E_NOT_OK            The TP transmission has been aborted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
***********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_TxConfirmation(PduR_RmGDestRomIterType rmGDestRomIdx, Std_ReturnType result);
# endif

# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_CancelTransmit
 *********************************************************************************************************************/
/*! \brief       This function cancels the transmission on this destination.
 *  \details     -
 *  \param[in]   rmGDestRomIdx   Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   applicationContext Application call context
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
***********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_CancelTransmit(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext);
# endif

# if(PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_MainFunction
 *********************************************************************************************************************/
/*!
 * \brief        This function processes deferred Tp Pdus.
 * \details      -
 * \param[in]    rmGDestRomIdx            internal RmGDestRom index
 * \pre          -
 * \context      TASK
 * \reentrant    TRUE
 * \synchronous  TRUE
 * \spec
 *   requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *   requires PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(rmGDestRomIdx);
 * \endspec
**********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_TxInst_MainFunction(PduR_RmGDestRomIterType rmGDestRomIdx);
# endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmTp_TxInst_IsTxIdleOnAllQueuedDestinations
 *********************************************************************************************************************/
/*! \brief       Checks if the transmission of all the queued destinations of this source Pdu is finished.
 *  \details     -
 *  \param[in]   rmSrcRomIdx       internal RmSrcRom index
 *  \return      TRUE           Transmission on all queued destinations is finished.
 *               FALSE          Transmission on all queued destinations is NOT finished.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx);
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmTp_TxInst_IsTxIdleOnAllQueuedDestinations(PduR_RmSrcRomIterType rmSrcRomIdx);
#endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RMTP_TXINST_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp_TxInst.h
 *********************************************************************************************************************/
