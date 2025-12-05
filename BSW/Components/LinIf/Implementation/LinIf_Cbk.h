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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  LinIf_Cbk.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Callback Functions of the AUTOSAR LIN Interface
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#if !defined (LINIF_CBK_H)
# define LINIF_CBK_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "ComStack_Types.h"
/* required for type EcuM_WakeupSourceType */
# include "EcuM.h"
# include "LinIf_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define LINIF_CBK_MAJOR_VERSION                    (12u)
# define LINIF_CBK_MINOR_VERSION                    (0u)
# define LINIF_CBK_PATCH_VERSION                    (0u)

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
**********************************************************************************************************************/

# define LINIF_START_SEC_CODE
# include "LinIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LinIf_WakeupConfirmation
 *********************************************************************************************************************/
/*! \brief       Called after an external wakeup event was detected.
 *  \details     This function is called by LIN Driver or LIN Transceiver Driver to report the wake up source after the
 *               successful wakeup detection during CheckWakeup or after power on by bus.
 *  \param[in]   WakeupSource            Source of the wakeup event.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE for different wakeup sources
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_WakeupConfirmation
(
  VAR(EcuM_WakeupSourceType, AUTOMATIC) WakeupSource
);

# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )

/**********************************************************************************************************************
 *  LinIf_HeaderIndication
 *********************************************************************************************************************/
/*! \brief         Called to report the reception of a LIN header.
 *  \details       This function is called by LIN Driver to report a received LIN header and provides the received PID.
 *  \param[in]     Channel           The channel parameter is dependent on the configuration of channel handle
 *                                   parameter LinIfChannelHandleTypeInCallbacks.
 *                                   CHANNEL_TYPE_LIN: LIN (driver) channel handle. The channel must reference a LIN
 *                                                     slave channel.
 *                                   CHANNEL_TYPE_COMM: ComM channel handle. The channel must reference a LIN slave
 *                                                      channel.
 *  \param[in,out] PduPtr            Pointer to PDU providing the received PID and pointer to the SDU data buffer. Upon
 *                                   return, the remaining members are set as out parameters. Parameter must not be NULL.
 *  \return        E_OK              Request was accepted, header indication is processed.
 *  \return        E_NOT_OK          Request was not accepted.
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     TRUE for different channels.
 *  \synchronous   TRUE
 *  \config        LINIF_NODETYPE_SLAVE_SUPPORTED
 *  \trace         CREQ-224379
 *  \note          This callback is only applicable for LIN Slave nodes.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_HeaderIndication
(
  VAR(NetworkHandleType, AUTOMATIC)             Channel,
  P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_VAR) PduPtr
);

/**********************************************************************************************************************
 *  LinIf_RxIndication
 *********************************************************************************************************************/
/*! \brief        Called to report the reception of a LIN response.
 *  \details      This function is called by LIN Driver to report a successfully received response and provides the
 *                reception data to the LIN Interface.
 *  \param[in]    Channel           The channel parameter is dependent on the configuration of channel handle
 *                                  parameter LinIfChannelHandleTypeInCallbacks.
 *                                  CHANNEL_TYPE_LIN: LIN (driver) channel handle. The channel must reference a LIN
 *                                                    slave channel.
 *                                  CHANNEL_TYPE_COMM: ComM channel handle. The channel must reference a LIN slave
 *                                                     channel.
 *  \param[in]    Lin_SduPtr        Pointer to SDU providing the received data bytes. Parameter must not be NULL.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different channels.
 *  \synchronous  TRUE
 *  \config       LINIF_NODETYPE_SLAVE_SUPPORTED
 *  \trace        CREQ-224379
 *  \note         This callback is only applicable for LIN Slave nodes.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_RxIndication
(
  VAR(NetworkHandleType, AUTOMATIC)       Channel,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR) Lin_SduPtr
);

/**********************************************************************************************************************
 *  LinIf_TxConfirmation
 *********************************************************************************************************************/
/*! \brief        Called to report the transmission of a LIN response.
 *  \details      This function is called by LIN Driver to report a successfully transmitted response.
 *  \param[in]    Channel           The channel parameter is dependent on the configuration of channel handle
 *                                  parameter LinIfChannelHandleTypeInCallbacks.
 *                                  CHANNEL_TYPE_LIN: LIN (driver) channel handle. The channel must reference a LIN
 *                                                    slave channel.
 *                                  CHANNEL_TYPE_COMM: ComM channel handle. The channel must reference a LIN slave
 *                                                     channel.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different channels.
 *  \synchronous  TRUE
 *  \config       LINIF_NODETYPE_SLAVE_SUPPORTED
 *  \trace        CREQ-224379
 *  \note         This callback is only applicable for LIN Slave nodes.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_TxConfirmation
(
  VAR(NetworkHandleType, AUTOMATIC) Channel
);

/**********************************************************************************************************************
 *  LinIf_LinErrorIndication
 *********************************************************************************************************************/
/*! \brief        Called to report the detection of a LIN error event.
 *  \details      This function is called by LIN Driver to report a detected error event during header or response processing.
 *  \param[in]    Channel           The channel parameter is dependent on the configuration of channel handle
 *                                  parameter LinIfChannelHandleTypeInCallbacks.
 *                                  CHANNEL_TYPE_LIN: LIN (driver) channel handle. The channel must reference a LIN
 *                                                    slave channel.
 *                                  CHANNEL_TYPE_COMM: ComM channel handle. The channel must reference a LIN slave
 *                                                     channel.
 *  \param[in]    ErrorStatus       Type of detected error.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different channels.
 *  \synchronous  TRUE
 *  \config       LINIF_NODETYPE_SLAVE_SUPPORTED
 *  \trace        CREQ-224379
 *  \note         This callback is only applicable for LIN Slave nodes.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_LinErrorIndication
(
  VAR(NetworkHandleType, AUTOMATIC)  Channel,
  VAR(Lin_SlaveErrorType, AUTOMATIC) ErrorStatus
);

# endif

# define LINIF_STOP_SEC_CODE
# include "LinIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* LINIF_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf_Cbk.h
 *********************************************************************************************************************/
