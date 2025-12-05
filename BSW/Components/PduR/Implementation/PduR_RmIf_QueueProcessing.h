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
 *         \file  PduR_RmIf_QueueProcessing.h
 *         \unit  RmIf_QueueProcessing
 *        \brief  Pdu Router Routing Manager Interface Queue Processing header file
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
#if !defined (PDUR_RMIF_QUEUEPROCESSING_H)
# define PDUR_RMIF_QUEUEPROCESSING_H

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

# if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_MainFunctionRx
 *********************************************************************************************************************/
/*!
 * \brief        Process deferred If Rx Pdus in context of cyclic MainFunction
 * \details      -
 * \param[in]    rmGDestRomIdx    Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 * \pre         -
 * \context      TASK
 * \reentrant    TRUE
 * \synchronous  TRUE
 * \spec
 *   requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *   requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 * \endspec
**********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_MainFunctionRx(PduR_RmGDestRomIterType rmGDestRomIdx);
# endif

# if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_TxConfirmation
 *********************************************************************************************************************/
/*!
 * \brief        Confirm the transmission of the last Pdu from the queue. This will remove this Pdu and send the next one if available.
 * \details      -
 * \param[in]    rmGDestRomIdx    Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 * \pre          -
 * \context      TASK
 * \reentrant    TRUE
 * \synchronous  TRUE
 * \spec
 *   requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *   requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 * \endspec
**********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_TxConfirmation(PduR_RmGDestRomIterType rmGDestRomIdx);
# endif

# if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_TransmitImmediately
 *********************************************************************************************************************/
/*!
 * \brief        Transmits the oldest Pdu from the queue.
 * \details      -
 * \param[in]    rmGDestRomIdx    Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 * \param[in]    apiID            API ID to report in case of a unsuccessful transmission.
 * \return       Std_ReturnType
 *               E_OK             The Pdu was transmitted successfully.
 *               E_NOT_OK         The Pdu was not transmitted or there was no Pdu in the queue.
 * \pre          -
 * \context      TASK
 * \reentrant    TRUE
 * \synchronous  TRUE
 * \spec
 *   requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *   requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 * \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_QueueProcessing_TransmitImmediately(PduR_RmGDestRomIterType rmGDestRomIdx, uint8 apiId);
# endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_ProcessBufferedRouting
**********************************************************************************************************************/
/*! \brief       The function is called by the PduR to process buffered If Pdus
 *  \details     -
 *  \param[in]   rmSrcRomIdx       Valid RmSrcRom table index. Internal handle of a PduRSrcPdu.
 *  \param[in]   rmDestRomIdx      Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   rmGDestRomIdx     Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   info              Payload information of the received I-PDU (pointer to data and data length).
 *  \return      Std_ReturnType
 *               E_OK              The Pdu was buffered successfully.
 *               E_NOT_OK          The Pdu was not buffered.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx);
 *  \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_QueueProcessing_ProcessBufferedRouting(PduR_RmSrcRomIterType rmSrcRomIdx, PduR_RmDestRomIterType rmDestRomIdx, PduR_RmGDestRomIterType rmGDestRomIdx,
                                                                              P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);
#endif

#if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_TriggerTransmit
**********************************************************************************************************************/
/*! \brief           This function copies the next Pdu from the queue to info->SduDataPtr.
 *  \details         The actual length of the copied data will be returned via info->SduLength.
 *  \param[in]       rmDestRomIdx         Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in,out]   info              Buffer to where the Pdu will be copied.
 *                                     The buffer has to be at least as large as the stored Pdu in the queue.
 *  \return          Std_ReturnType
 *                   E_OK              The SDU has been copied and the SduLength indicates the number of copied bytes.
 *                   E_NOT_OK          The SDU has not been copied and the SduLength has not been set.
 *  \pre             -
 *  \context         TASK|ISR1|ISR2
 *  \reentrant       TRUE
 *  \synchronous     TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));
 *  \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_QueueProcessing_TriggerTransmit(PduR_RmDestRomIterType rmDestRomIdx, CONSTP2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_DisableRoutingAction
 *********************************************************************************************************************/
/*! \brief       This function flushes the Interface queue if the queue is in use while the RoutingPathGroup is disabled.
 *  \details     -
 *  \param[in]   rmDestRomIdx  Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *    requires PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_DisableRoutingAction(PduR_RmDestRomIterType rmDestRomIdx);
#endif

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RMIF_QUEUEPROCESSING_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RmIf_QueueProcessing.h
 *********************************************************************************************************************/
