/* \trace SPEC-1878 */

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  CanIf_Cbk.h
 *        \brief  CAN Interface callback functions header file.
 *
 *      \details  This file contains the declaration of the callback function prototypes of the AUTOSAR 
 *                CAN Interface module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  05.00.00  2012-07-02  visrna  ESCAN00060132 Support ASR 4.0 Rev 3
 *  ...                                         see older versions for previous history
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CANIF_CBK_H)
# define CANIF_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
 
# include "CanIf_Cfg.h"
/* CanIf_Types.h  is included by CanIf_Cfg.h */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
 
# define CANIF_START_SEC_CODE
# include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* \trace SPEC-3696 */

/************************************************************************************************************
 *  CanIf_ControllerModeIndicationAr403()
 ***********************************************************************************************************/
/*! \brief         Called by the CAN Driver of EQC AR 4.0.3, AR 4.2.1 or MSRC to notify CAN Interface about a
 *                 successful controller mode transition at the specified CAN controller.
 *  \details       A suitable define is generated to CanIf_Cfg.h as interface for calls from the CAN Driver.
 *                 Also called by CanIf_ControllerModeIndicationAr431() to ensure the same interface to the
 *                 upper layer with CAN Driver of EQC AR 4.3.1.
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                               which controller mode has been transitioned.
 *  \param[in]     ControllerMode: Controller mode to which the CAN controller transitioned.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE only for different ControllerId
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_ControllerModeIndicationAr403(uint8 ControllerId, CanIf_ControllerModeType ControllerMode);

#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
/************************************************************************************************************
 *  CanIf_ControllerModeIndicationAr431()
 ***********************************************************************************************************/
/*! \brief         Called by the CAN Driver of EQC AR 4.3.1 to notify CAN Interface about a successful
 *                 controller mode transition at the specified CAN controller.
 *  \details       A suitable define is generated to CanIf_Cfg.h as interface for calls from the CAN Driver.
 *                 CanIf_ControllerModeIndicationAr431() converts the CAN controller state to the CAN controller
 *                 mode type and forwards it to the CanIf_ControllerModeIndicationAr403 (this ensures same
 *                 interface to upper layer for all CAN Driver EQCs).
 *  \param[in]     ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                               which controller mode has been transitioned.
 *  \param[in]     ControllerMode: Controller mode to which the CAN controller transitioned.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE only for different ControllerId
 *  \synchronous   TRUE
 *  \config        CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON
***********************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_ControllerModeIndicationAr431(uint8 ControllerId, Can_ControllerStateType ControllerMode);
#endif /* (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) */

/***********************************************************************************************************************
 *  CanIf_TxConfirmation()
 **********************************************************************************************************************/
/*! \brief       Called by the CAN Driver to notify the CAN Interface about the successful transmission of
 *               the specified Tx-PDU.
 *  \details     -
 *  \param[in]   CanTxPduId: Handle of the Tx-PDU which is successfully transmitted.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_TxConfirmation(PduIdType CanTxPduId);

/***********************************************************************************************************************
 *  CanIf_RxIndicationAsr403()
 **********************************************************************************************************************/
/*! \brief       Called by the CAN Driver to notify the CAN Interface about the reception of the specified CAN 2.0/FD Rx PDU.
 *  \details     This function searches whether the received PDU matches one of the configured ones. If yes, then the
 *               configured upper layer is notified.
 *               The signature of the CanIf_RxIndicationAsr403() callback function is according to CAN Driver of EQC AR 4.0.3 or MSRC.
 *               A suitable define is generated to CanIf_Cfg.h as interface for calls from the CAN Driver of EQC AR 4.0.3 or MSRC.
 *               A suitable wrapper is generated to CanIf_Cfg.h as interface for calls from the CAN Driver of EQC AR 4.2.1 or AR 4.3.1.
 *  \param[in]   Hrh: Hardware handle where the Rx PDU was received in.
 *  \param[in]   CanId: CAN identifier of the received Rx PDU.
 *  \param[in]   CanDlc: Data length of the received Rx PDU.
 *  \param[in]   CanSduPtr: Pointer to the data of the received Rx PDU.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_RxIndicationAsr403(CanIf_HwHandleType Hrh, Can_IdType CanId, uint8 CanDlc, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR)CanSduPtr);

#if (CANIF_SUPPORT_CAN_XL == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */
/***********************************************************************************************************************
*  CanIf_XLRxIndication()
**********************************************************************************************************************/
/*! \brief       Called by the CAN Driver to notify the CAN Interface about the reception of the specified CAN XL Rx PDU.
 *  \details     This function searches whether the received CAN XL Rx PDU matches one of the configured ones. If yes, then the
 *               configured upper layer is notified.
 *  \param[in]   Mailbox: Contains the CAN XL Hrh, the abstracted CanIf ControllerId (belonging to the CAN controller
                          at which the CAN XL Rx PDU was received) and the CAN XL parameters.
 *  \param[in]   PduInfoPtr: Pointer to the received CAN XL Rx PDU.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_SUPPORT_CAN_XL == STD_ON
**********************************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_XLRxIndication(P2CONST(CanXL_HwType, AUTOMATIC, CANIF_CBK_DRV_VAR) Mailbox, P2CONST(PduInfoType, AUTOMATIC, CANIF_CBK_DRV_VAR) PduInfoPtr);
#endif

/* void CanIf_CancelTxConfirmation(const Can_PduType *PduInfoPtr); */
# if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
#  if (CANIF_TRANSMIT_BUFFER_FIFO == STD_OFF)
#   if (CANIF_TRANSMIT_CANCELLATION == STD_ON)
/***********************************************************************************************************************
 *  CanIf_CancelTxConfirmation()
 **********************************************************************************************************************/
/*! \brief       Called by the CAN Driver to notify the CAN Interface about the transmission cancellation of the
 *               specifed Tx-PDU.
 *  \details     The specified Tx-PDU is re-queued in a transmit-buffer.
 *  \param[in]   CanTxPduId: Handle of the Tx-PDU which was cancelled.
 *  \param[in]   PduInfoPtr: Pointer to parameters of the canceled Tx-PDU.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER == STD_ON and CANIF_TRANSMIT_CANCELLATION == STD_ON
 **********************************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_CancelTxConfirmation(PduIdType CanTxPduId, P2CONST(Can_PduType, AUTOMATIC, CANIF_CBK_DRV_VAR) PduInfoPtr);
#   endif
#  endif
# endif

/***********************************************************************************************************************
 *  CanIf_ControllerBusOff()
 **********************************************************************************************************************/
/*! \brief       Called by the CAN Driver to notify the CAN Interface about an occurred BusOff-event at the 
 *               specified CAN controller.
 *  \details     -
 *  \param[in]   ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                             at which the BusOff-event occurred.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_ControllerBusOff(uint8 ControllerId);

# if (CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
/***********************************************************************************************************************
 *  CanIf_ControllerErrorStatePassive()
 **********************************************************************************************************************/
/*! \brief       Called by the CAN Driver to notify the CAN Interface about an occurred transistion to error state passive
 *               at the specified CAN controller
 *  \details     -
 *  \param[in]   ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                             at which the transistion to error state passive occurred.
 *  \param[in]   RxErrorCounter: Value of the rx error counter from the specified CAN controller.
 *  \param[in]   TxErrorCounter: Value of the tx error counter from the specified CAN controller.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_ENABLE_SECURITY_EVENT_REPORTING  == STD_ON
 **********************************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_ControllerErrorStatePassive(uint8 ControllerId, uint16 RxErrorCounter, uint16 TxErrorCounter);

/***********************************************************************************************************************
 *  CanIf_ErrorNotification()
 **********************************************************************************************************************/
/*! \brief       Called by the CAN Driver to notify the CAN Interface about an occurred bus error at the specified
 *               CAN controller
 *  \details     -
 *  \param[in]   ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller,
 *                             at which the bus error occurred.
 *  \param[in]   CanError: Occured bus error.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_ENABLE_SECURITY_EVENT_REPORTING  == STD_ON
 **********************************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_ErrorNotification(uint8 ControllerId, Can_ErrorType CanError);
# endif

/* NON AUTOSAR callback functions */

# if (CANIF_CANCEL_SUPPORT_API == STD_ON)
/***********************************************************************************************************************
 *  CanIf_CancelTxNotification()
 **********************************************************************************************************************/
/*! \brief       Called by the CAN Driver to notify the CAN Interface about the transmission cancelation of the
 *               specified Tx-PDU.
 *  \details     The specified Tx-PDU is not confirmed to the configured upper layer. The next Tx-PDU from the
 *               transmit-buffer is transmitted.
 *               Used for trigger-purpose to fill the free HW-object, after calling of CanIf_CancelTransmit().
 *  \param[in]   PduId: Handle of the Tx-PDU which was cancelled.
 *  \param[in]   IsCancelled: Parameter currently not evaluated.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_CANCEL_SUPPORT_API == STD_ON
 **********************************************************************************************************************/
FUNC(void, CANIF_CODE) CanIf_CancelTxNotification(PduIdType PduId, CanIf_CancelResultType IsCancelled);
# endif

# define CANIF_STOP_SEC_CODE
# include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CANIF_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: CanIf_Cbk.h
 *********************************************************************************************************************/
