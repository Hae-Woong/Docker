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
 *         \file  PduR_MultipleSourceHandler.h
 *         \unit  MultipleSourceHandler
 *        \brief  Pdu Router Multiple Source Handler header file
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
#if !defined (PDUR_MULTIPLESOURCEHANDLER_H)
# define PDUR_MULTIPLESOURCEHANDLER_H

/**********************************************************************************************************************
   LOCAL MISRA / PCLINT JUSTIFICATION
**********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA  S 0777 EOF */ /* MD_MSR_Rule5.1_0777 */
/* *INDENT-ON* */

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
 * PduR_MultipleSourceHandler_Init
 *********************************************************************************************************************/
/*! \brief       This function initializes the MultipleSourceHandler unit.
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
FUNC(void, PDUR_CODE) PduR_MultipleSourceHandler_Init(PduR_MemIdxType memIdx);

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_Reset
 *********************************************************************************************************************/
/*! \brief       Reset MultipleSourceHandler state machine.
 *  \details     -
 *  \param[in]   rmGDestRomIdx  Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_MultipleSourceHandler_Reset(PduR_RmGDestRomIterType rmGDestRomIdx);

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_IfTransmit
**********************************************************************************************************************/
/*!  \brief       This function forwards the transmission request to the lower layer if no other source is transmitting to this global destination right now.
 *   \details     If the transmission request is forwarded, the parameters and the return value is forwarded without being changed.
 *   \param[in]   rmDestRomIdx  Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *   \param[in]   info          Pointer to the data to be transmitted.
 *   \return      E_OK          The transmission was successful.
 *                E_NOT_OK      The transmission failed.
 *   \pre         -
 *   \context     TASK|ISR1|ISR2
 *   \reentrant   TRUE
 *   \synchronous TRUE
 *   \spec
 *     requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *   \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_IfTransmit(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

 /**********************************************************************************************************************
 * PduR_MultipleSourceHandler_TriggerTransmit
**********************************************************************************************************************/
/*! \brief       This function forwards the TriggerTransmit request to the source of the last transmit request.
 *  \details     The request will be dropped if no previous transmit was done.
 *  \param[in]   rmGDestRomIdx     Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   info              Pointer to the buffer. The data is copied to this buffer.
 *  \return      E_OK              The data was copied to the buffer.
 *               E_NOT_OK          The data was not copied to the buffer.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *  \endspec
***********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_TriggerTransmit(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_IfCancelTransmit
 *********************************************************************************************************************/
/*! \brief        This function forwards the CancelTransmit requests to the destination.
 *  \details      The request will be dropped if no previous transmit was done or the transmit came from a different source.
 *  \param[in]    rmDestRomIdx   Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \return       E_OK           Cancellation was successful.
 *                E_NOT_OK       Cancellation was not successful.
 *  \pre          -
 *  \context      TASK|ISR1|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *   \spec
 *     requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *   \endspec
 **********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_IfCancelTransmit(PduR_RmDestRomIterType rmDestRomIdx);

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_IfTxConfirmation
**********************************************************************************************************************/
/*!  \brief       This function forwards the TxConfirmation request to the source of the last transmit request.
 *   \details     The request will be dropped if no previous transmit was done.
 *   \param[in]   rmGDestRomIdx  Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *   \pre         -
 *   \context     TASK|ISR1|ISR2
 *   \reentrant   TRUE
 *   \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *  \endspec
 **********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_MultipleSourceHandler_IfTxConfirmation(PduR_RmGDestRomIterType rmGDestRomIdx);

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_TpTriggerTransmission
**********************************************************************************************************************/
/*!  \brief       This function forwards the transmission request to the destination if no other source is transmitting to this global destination right now.
 *   \details     This function is only used for queued routing paths to also handle the Tx state machine for the destination.
 *                If the transmission request is forwarded, the parameters and the return value is forwarded without being changed.
 *   \param[in]   rmDestRomIdx  Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *   \param[in]   applicationContext Application call context
 *   \return      E_OK          The transmission was successfully triggered.
 *                E_NOT_OK      The transmission was not triggered.
 *   \pre         -
 *   \context     TASK|ISR1|ISR2
 *   \reentrant   TRUE
 *   \synchronous TRUE
 *   \spec
 *     requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *   \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_TpTriggerTransmission(PduR_RmDestRomIterType rmDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext);
#endif

/**********************************************************************************************************************
 *  PduR_MultipleSourceHandler_TpTransmit
**********************************************************************************************************************/
/*!  \brief       This function forwards the transmission request to the lower layer if no other source is transmitting to this global destination right now.
 *   \details     This function is only used for unqueued routing path with Tx source and Tx destination.
 *                If the transmission request is forwarded, the parameters and the return value is forwarded without being changed.
 *   \param[in]   rmDestRomIdx  Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *   \param[in]   info          Pointer to the data to be transmitted.
 *   \return      E_OK          The transmission was successful.
 *                E_NOT_OK      The transmission failed.
 *   \pre         -
 *   \context     TASK|ISR1|ISR2
 *   \reentrant   TRUE
 *   \synchronous TRUE
 *   \spec
 *     requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *   \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_TpTransmit(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

/**********************************************************************************************************************
 *  PduR_MultipleSourceHandler_TpCancelTransmit
**********************************************************************************************************************/
/*!  \brief       This function forwards the CancelTransmit requests to the destination.
 *   \details     The request will be dropped if no previous transmit was done or the transmit came from a different source.
 *   \param[in]   rmDestRomIdx   Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *   \return      E_OK           Cancellation was successful.
 *                E_NOT_OK       Cancellation was not successful.
 *   \pre         -
 *   \context     TASK|ISR1|ISR2
 *   \reentrant   TRUE
 *   \synchronous TRUE
 *   \spec
 *     requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *   \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_TpCancelTransmit(PduR_RmDestRomIterType rmDestRomIdx);

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_TpTxConfirmation
**********************************************************************************************************************/
/*!  \brief       This function forwards the TxConfirmation request to the source of the last transmit request.
 *   \details     The request will be dropped if no previous transmit was done.
 *   \param[in]   rmGDestRomIdx  Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *   \param[in]   E_OK           Transmission was successful.
 *                E_NOT_OK       Transmission was not successful.
 *   \pre         -
 *   \context     TASK|ISR1|ISR2
 *   \reentrant   TRUE
 *   \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *  \endspec
 **********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_MultipleSourceHandler_TpTxConfirmation(PduR_RmGDestRomIterType rmGDestRomIdx, Std_ReturnType result);

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_CopyTxData
**********************************************************************************************************************/
/*! \brief       This function forwards the CopyTxData request to the source of the last transmit request.
 *  \details     The request will be dropped if no previous transmit was done.
 *  \param[in]   rmGDestRomIdx              Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   info                       Pointer to the buffer. The data will be copied to this buffer.
 *  \param[in]   retry                      not supported
 *  \param[out]  availableDataPtr           Indicates the remaining number of bytes that are available in the PduR buffer.
 *  \return      BUFREQ_OK                  The data has been copied to the transmit buffer successfully.
 *               BUFREQ_E_NOT_OK            Copying the data has failed. No data has been copied to the provided buffer. The request may not be retried.
 *               BUFREQ_E_BUSY              Copying the data has failed. No data has been copied to the provided buffer. The request may be retried.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *  \endspec
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_CopyTxData(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                              P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr);

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_MULTIPLESOURCEHANDLER_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_MultipleSourceHandler.h
 *********************************************************************************************************************/
