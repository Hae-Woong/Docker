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
 *         File:  LinIf_Int.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Internal header file of the AUTOSAR LIN Interface.
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

#if !defined (LINIF_INT_H)
# define LINIF_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "LinIf_Cfg.h"

# if ( LINIF_DIAGNOSTIC_FRAMES_ONLY == STD_OFF ) /* COV_LINIF_FBL_USECASE TX */
#  include "PduR_LinIf.h"
# endif

# if ( LINIF_TP_SUPPORTED == STD_ON )
#  include "PduR_LinTp.h"
# endif

# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
#  include "Det.h"
# endif

# include "SchM_LinIf.h"

# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_ON ) || ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
/* necessary if LinSM functions are called directly from LinIf.c and not via function tables from generated data or end notification used */
#  include "LinSM_Cbk.h"
# endif

/**********************************************************************************************************************
 *  CONSTANT MACROS
 *********************************************************************************************************************/
# define LINIF_INT_MAJOR_VERSION                      (12u)
# define LINIF_INT_MINOR_VERSION                      (0u)
# define LINIF_INT_PATCH_VERSION                      (0u)

# if !defined ( LINIF_LOCAL ) /* COV_LINIF_COMPATIBILITY TX */
#  define LINIF_LOCAL                                 static
# endif

# if !defined ( LINIF_LOCAL_INLINE ) /* COV_LINIF_COMPATIBILITY TX */
#  define LINIF_LOCAL_INLINE                          LOCAL_INLINE
# endif

# if ! defined ( LINTP_CANCEL_RECEIVE_SUPPORTED )
#  define LINTP_CANCEL_RECEIVE_SUPPORTED              STD_ON
# endif

# if ! defined ( LINTP_CANCEL_TRANSMIT_SUPPORTED )
#  define LINTP_CANCEL_TRANSMIT_SUPPORTED             STD_ON
# endif

# if ! defined ( LINTP_CHANGE_PARAMETER_SUPPORTED )
#  define LINTP_CHANGE_PARAMETER_SUPPORTED            STD_ON
# endif

# if ! defined ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED )
#  define LINTP_FUNCTIONAL_REQUEST_SUPPORTED          STD_ON
# endif

# if ! defined ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR )
#  define LINTP_FORWARD_RESPONSEPENDING_TO_PDUR       STD_ON
# endif

# if ! defined ( LINIF_WAKEUPCONF_AR403_COMPATIBILITY )
#  define LINIF_WAKEUPCONF_AR403_COMPATIBILITY        STD_OFF
# endif

/* AR4-612 RFC57401 requires to check the return value of <UL>_ TriggerTransmit and to discard the slot if E_NOT_OK is
   returned. Per default, BswM is configured in such a way that the Pdu groups are disabled / enabled inside the
   schedule request confirmation callback. Because TriggerTransmit() is called before the schedule confirmation due to
   optimization reasons, it returns E_NOT_OK if the first frame inside a schedule is a Tx frame.
   Per default, the return value of TriggerTransmit is then ignored and the frame is transmitted, because MSR COM copies
   the data to the buffer even if it returns E_NOT_OK. To support RFC57401, define LINIF_IGNORE_TRIGGERTRANSMIT_RETVAL
   to STD_OFF in a user config file. Special care has to be taken when configuring BswM in this case. */
# if ! defined ( LINIF_IGNORE_TRIGGERTRANSMIT_RETVAL )
#  define LINIF_IGNORE_TRIGGERTRANSMIT_RETVAL         STD_ON
# endif

# if ! defined ( LINIF_NC_AUTO_ADDRESSING_SID_B5 )
#  define LINIF_NC_AUTO_ADDRESSING_SID_B5             STD_OFF
# endif

/**********************************************************************************************************************
 *  FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*!
 * \defgroup LinIfInitState Component initialization states of LinIf
 * \{
 */
# define LINIF_UNINIT                                 ((uint8)0x00u) /*!< LinIf uninitialized module state */
# define LINIF_INIT                                   ((uint8)0xA5u) /*!< LinIf initialized module state */
/*! \} */

# if ( LINIF_TP_SUPPORTED == STD_ON )
/*!
 * \defgroup LinTpInitState Component initialization states of LinTp
 * \{
 */
#  define LINTP_UNINIT                                ((uint8)0x00u) /*!< LinTp uninitialized module state */
#  define LINTP_INIT                                  ((uint8)0xA5u) /*!< LinTp initialized module state */
/*! \} */
# endif

/*! Invalid frame handle constant */
# define LinIf_FrameHandle_Invalid                    ((uint16)0xFFFFu) /*!< Invalid frame handle */

/*!
 * \defgroup LinIfPduRNotification LinIf notification state for Rx/Tx frames to upper layer
 * \{
 */
# define LinIf_PduRNotificationIdle                   ((uint8)0x00u) /*!< No LinIf Rx/Tx notification pending */
# define LinIf_PduRNotificationTx                     ((uint8)0x01u) /*!< Rx LinIf notification pending */
# define LinIf_PduRNotificationRx                     ((uint8)0x02u) /*!< Tx LinIf notification pending */
/*! \} */

# if ( LINIF_TP_SUPPORTED == STD_ON )
/*!
 * \defgroup LinTpPduRNotification LinTp notification state for Rx/Tx messages to PduR
 * \{
 */
#  define LinTp_PduRNotificationIdle                  ((uint8)0x00u) /*!< No LinTp Rx/Tx notification pending */
#  define LinTp_PduRNotificationTx                    ((uint8)0x01u) /*!< Tx LinTp notification pending */
#  define LinTp_PduRNotificationTxTm                  ((uint8)0x02u) /*!< Additional Tx LinTp notification pending */
#  define LinTp_PduRNotificationRx                    ((uint8)0x10u) /*!< Rx LinTp notification pending */
/*! \} */

#  define LinTp_PortNotificationIdle                  ((uint8)0xFFu) /*!< No LinTp schedule request notification pending */

#  define LinTp_InvalidNsduIdx                        (0xFFFFu) /*!< Invalid NSdu handle */
# endif

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
/*!
 * \defgroup SchdlHdlrRet Schedule table handler return codes
 * \{
 */
/*! Schedule table handler return type */
typedef uint8                                         LinIf_ScheduleTableHandlerRetType;
/*! NULL schedule is active */
#  define LinIf_SchdlHdlrRet_OnNullSchedule           ((LinIf_ScheduleTableHandlerRetType)0x00u)
/*! Schedule table is active and slot is not completed */
#  define LinIf_SchdlHdlrRet_NoNewMessage             ((LinIf_ScheduleTableHandlerRetType)0x01u)
/*! Schedule table is active and new slot is started */
#  define LinIf_SchdlHdlrRet_NewMessage               ((LinIf_ScheduleTableHandlerRetType)0x02u)
/*! \} */

/*!
 * \defgroup LinIfScheduleConstants Schedule table constants
 * \{
 */
/*! LinIf NULL schedule table
 * \trace CREQ-601
 */
#  define LinIf_Schedule_NULL                         (NULL_SCHEDULE)
/*! Invalid schedule table handle */
#  define LinIf_Schedule_Invalid                      ((LinIf_SchHandleType)0xFFu)
/*! \} */

/*!
 * \defgroup LinIfAtNodeConfigurationCmd Node configuration command handling states for master nodes
 * \{
 */
#  define LinIf_AtNodeConfigurationCmd_Idle           ((uint8)0x00u) /*!< No node configuration service active */
#  define LinIf_AtNodeConfigurationCmd_SendMrf        ((uint8)0x01u) /*!< Node configuration request sent */
#  define LinIf_AtNodeConfigurationCmd_SkipSrf        ((uint8)0x02u) /*!< Do not send next SRF header response */
#  define LinIf_AtNodeConfigurationCmd_SendSrf        ((uint8)0x03u) /*!< Send SRF header for node configuration response */
/*! \} */

/*!
 * \defgroup LinIf_OnSleepModeFrameHandling Internal sleep command frame states for master nodes
 * \{
 */
#  define LinIf_OnSleepModeFrameHandling_Idle         ((uint8)0x00u) /*!< No sleep transition active */
#  define LinIf_OnSleepModeFrameHandling_OnReq        ((uint8)0x01u) /*!< Go to sleep command transmission requested */
#  define LinIf_OnSleepModeFrameHandling_OnTrans      ((uint8)0x02u) /*!< Go to sleep command is currently transmitted */
#  define LinIf_OnSleepModeFrameHandling_OnReWakeInt  ((uint8)0x03u) /*!< Wakeup request during sleep command transmission */
#  define LinIf_OnSleepModeFrameHandling_OnReqInt     ((uint8)0x04u) /*!< Internal go to sleep transition requested */
#  define LinIf_OnSleepModeFrameHandling_OnTransInt   ((uint8)0x05u) /*!< Status call for internal sleep transition pending */
#  define LinIf_OnWakeupFrameHandling_OnTrans         ((uint8)0x06u) /*!< Wakeup frame is currently transmitted */
/*! \} */

/*!
 * \defgroup LinIf_ScheduleResumePos Schedule table resume position for master nodes
 * \{
 */
#  define LinIf_CONTINUE_AT_IT_POINT                  ((uint8)0x00u) /*!< Resume schedule at interrupted slot */
#  define LinIf_START_FROM_BEGINNING                  ((uint8)0x01u) /*!< Resume schedule at beginning of table */
/*! \} */

/*!
 * \defgroup LinIf_EvtCollResolvingState Event-triggered collision resolving table states for master nodes
 * \{
 */
#  define LinIf_EvtCollResolvingState_Idle            ((uint8)0x00u) /*!< No collision resolving table active */
#  define LinIf_EvtCollResolvingState_Detected        ((uint8)0x01u) /*!< Collision detected, collision resolving table pending */
#  define LinIf_EvtCollResolvingState_OnResolving     ((uint8)0x02u) /*!< Collision resolving table active */
/*! \} */
# endif


# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
/*!
 * \defgroup LinIf_SlaveFrameState Frame states for slave nodes
 * \{
 */
#  define LinIf_SlaveFrameState_Idle                  ((uint8)0x00u) /*!< No frame is currently handled */
#  define LinIf_SlaveFrameState_Rx                    ((uint8)0x01u) /*!< Header received, waiting for response reception indication */
#  define LinIf_SlaveFrameState_Tx                    ((uint8)0x02u) /*!< Header received, waiting for response transmission confirmation */
#  define LinIf_SlaveFrameState_Irrelevant            ((uint8)0x03u) /*!< Header received, subsequent response is ignored */
/*! \} */

/*!
 * \defgroup LinIf_SlaveNmState Wakeup / sleep transition states for slave nodes
 * \{
 */
#  define LinIf_SlaveNmState_Idle                     ((uint8)0x00u) /*!< No wakeup or sleep transition is ongoing */
#  define LinIf_SlaveNmState_WaitForWakeupConf        ((uint8)0x01u) /*!< Wakeup requested, waiting for first header */
/*! \} */

#  if ( LINIF_SLAVE_J2602_SUPPORTED == STD_ON )
/*!
 * \defgroup LinIf_J2602ErrorStateMask Bit masks for J2602 error states
 * \{
 */
#   define LinIf_J2602ErrorStateMask_NoError          ((uint8)0x00u) /*!< No error */
#   define LinIf_J2602ErrorStateMask_Reset            ((uint8)0x02u) /*!< Reset state bit mask */
#   define LinIf_J2602ErrorStateMask_DataErr          ((uint8)0x10u) /*!< Data error state bit mask */
#   define LinIf_J2602ErrorStateMask_ChecksumErr      ((uint8)0x20u) /*!< Checksum error state bit mask */
#   define LinIf_J2602ErrorStateMask_ByteFramingErr   ((uint8)0x40u) /*!< Byte field framing error state bit mask */
#   define LinIf_J2602ErrorStateMask_IDParityErr      ((uint8)0x80u) /*!< ID parity error state bit mask */
/*! \} */

/*!
 * \defgroup LinIf_J2602ErrorStateValues Values of J2602 error states
 * \{
 */
#   define LinIf_J2602ErrorStateVal_NoError           ((uint8)0x00u) /*!< No error */
#   define LinIf_J2602ErrorStateVal_Reset             ((uint8)0x01u) /*!< Reset state value  */
#   define LinIf_J2602ErrorStateVal_DataErr           ((uint8)0x04u) /*!< Data error state value */
#   define LinIf_J2602ErrorStateVal_ChecksumErr       ((uint8)0x05u) /*!< Checksum error state value */
#   define LinIf_J2602ErrorStateVal_ByteFramingErr    ((uint8)0x06u) /*!< Byte field framing error state value */
#   define LinIf_J2602ErrorStateVal_IDParityErr       ((uint8)0x07u) /*!< ID parity error state value */
/*! \} */
#  endif
# endif

# define LinIf_NAD_Functional                         ((uint8)0x7Eu) /*!< Functional NAD */
# define LinIf_NAD_Broadcast                          ((uint8)0x7Fu) /*!< Broadcast NAD */

# if ( LINIF_TP_SUPPORTED == STD_ON )
/*!
 * \defgroup LinTp_ChannelState LIN TP sub channel states
 * \trace CREQ-603
 * \trace CREQ-604
 * \{
 */
/*! LinTp channel state IDLE, no diagnostic communication active */
#  define LINTP_CHANNEL_IDLE                          ((uint8)0x00u)
/*! LinTp channel state Tx pending, transmission data currently not available */
#  define LINTP_CHANNEL_BUSY_TX_INVALID               ((uint8)0x10u)
/*! LinTp channel state Tx active, transmission ongoing */
#  define LINTP_CHANNEL_BUSY_TX_VALID                 ((uint8)0x11u)
/*! LinTp channel state Rx active, reception ongoing */
#  define LINTP_CHANNEL_BUSY_RX                       ((uint8)0x20u)
/*! LinTp channel state Rx pending, currently no buffer for reception available suppress SRF header transmission */
#  define LINTP_CHANNEL_BUSY_RX_SUSPENDED             ((uint8)0x21u)
/*! LinTp channel state Rx start pending, a new reception is started but PduR not informed yet */
#  define LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START        ((uint8)0x22u)
/*! LinTp channel state Rx start pending, schedule next SRF header but poll for receive buffer after reception */
#  define LINTP_CHANNEL_BUSY_RX_POLLBUFFER            ((uint8)0x23u)
/*! \} */

/*! LinTp channel state Tx mask */
#  define LINTP_CHANNEL_TX_MSK                        ((uint8)0x10u)
/*! LinTp channel state Rx mask */
#  define LINTP_CHANNEL_RX_MSK                        ((uint8)0x20u)

/* List of TP frame types */
#  define LinTp_FrameType_SF                          ((uint8)0x00u) /*!< Tp frame type: Single frame */
#  define LinTp_FrameType_FF                          ((uint8)0x10u) /*!< Tp frame type: First frame */
#  define LinTp_FrameType_CF                          ((uint8)0x20u) /*!< Tp frame type: Consecutive frame */

#  define LinTp_FrameMaxLength                        (0x0FFFu) /*! Maximum message length supported by LinTp */
# endif

/**********************************************************************************************************************
 *  DATA PROTOTYPES
 *********************************************************************************************************************/

# if ( LINIF_USE_INIT_POINTER == STD_ON )
#  define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to global LinIf Configuration */
extern P2CONST(LinIf_ConfigType, LINIF_VAR_ZERO_INIT, LINIF_INIT_DATA)     LinIf_ConfigDataPtr;

#  define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
# endif


# if ( LINIF_TP_SUPPORTED == STD_ON )
#  if ( LINTP_USE_INIT_POINTER == STD_ON )
#   define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
#   include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to global LinTp Configuration */
extern P2CONST(LinTp_ConfigType, LINTP_VAR_ZERO_INIT, LINTP_INIT_DATA)     LinTp_ConfigDataPtr;

#   define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
#   include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
#  endif
# endif

# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
#  define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization check variable for LinIf */
extern VAR(uint8, LINIF_VAR_ZERO_INIT)       LinIf_Dev_InitDetect;

#  if ( LINIF_TP_SUPPORTED == STD_ON )
/*! Initialization check variable for LinTp */
extern VAR(uint8, LINTP_VAR_ZERO_INIT)       LinTp_Dev_InitDetect;
#  endif

#  define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
# endif

/**********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define LINIF_START_SEC_CODE
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LinIf_ChannelMainFunction()
 *********************************************************************************************************************/
/*! \brief       Performs the main processing for one channel of the LIN interface.
 *  \details     Processes the schedule and frame handling and performs the appropriate state transitions for LIN
 *               master channels and performs various timeout observations.
 *  \param[in]   Channel           Network channel
 *  \pre         This module must be initialized before this service function is called.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_ChannelMainFunction
(
  VAR(NetworkHandleType, AUTOMATIC)                                 Channel
);

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING == STD_ON )
/**********************************************************************************************************************
 *  LinIf_MainFunctionPostProcessingExternal()
 *********************************************************************************************************************/
/*! \brief       Notifies the upper layers after each main processing function cycle of the LIN interface.
 *  \details     Performs all notification and indication to upper layers. It may be used to decouple the
 *               main processing (frame and schedule handling) from the interactions with upper layer modules.
 *               The possibility to call this function as separate schedulable entitiy optional and should only be
 *               enabled if needed due to timing requirements. In this case it is mandatory to call it once after each
 *               call of LinIf_Mainfunction_<shortname> of the appropriate channel.
 *  \param[in]   Channel           Network channel. Must reference a LIN master channel.
 *  \pre         This module must be initialized before this service function is called.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED and LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING
 *  \note        This function is only applicable if at least LIN master channel exists.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_MainFunctionPostProcessingExternal
(
  VAR(NetworkHandleType, AUTOMATIC)                                 Channel
);
#  endif
# endif

# if ( LINIF_TP_SUPPORTED == STD_ON )

/**********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_CopyRxData()
 *********************************************************************************************************************/
/*! \brief       Handles the interface with PduR for reception data.
 *  \details     Provides PduR with TP reception data and handles internal buffer states.
 *  \param[in]   CtrlPtr             Pointer to Tp channel control structure
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_CopyRxData
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)       CtrlPtr
);

/**********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_StartOfReception()
 *********************************************************************************************************************/
/*! \brief       Handles the interface with PduR for setting up a receive connection.
 *  \details     Initiates a Tp receive connection to PduR.
 *  \param[in]   CtrlPtr        Pointer to Tp channel control structure
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_StartOfReception
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)       CtrlPtr
);

/**********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_Main()
 *********************************************************************************************************************/
/*! \brief       Handles the received data and passes it to PduR.
 *  \details     Passes pending local received data to PduR and performs delayed buffer retry handling.
 *  \param[in]   CtrlPtr        Pointer to Tp channel control structure
 *  \param[in]   LinSduPtr      Source pointer (SRF buffer), provide null at repeated call to work on local buffer
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_Main
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinSduPtr
);

/**********************************************************************************************************************
 *  LinTp_BufferPolling()
 *********************************************************************************************************************/
/*! \brief       Requests data from PduR for current Tp connection, both Rx and Tx.
 *  \details     Polls for suspended buffer handling. Handles Rx and Tx connections.
 *  \param[in]   CtrlPtr        Pointer to Tp channel control structure
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *  \note        Function can be polled.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_BufferPolling
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        CtrlPtr
);

/**********************************************************************************************************************
 *  LinTp_RxIndication_CF()
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a consecutive frame N-PDU.
 *  \details     This function processes a received SRF containing a consecutive frame. Check for a valid sequence
 *               number and passes received data to buffer handling on active connection.
 *  \param[in]   CtrlPtr        Pointer to Tp channel control structure
 *  \param[in]   LinSduPtr      Pointer to receive buffer of current frame
 *  \pre         For slave nodes, the received NAD must address the slave node, NAD is not further validated.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_RxIndication_CF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)       CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                         LinSduPtr
);

/**********************************************************************************************************************
 *  LinTp_RxCloseConnection()
 *********************************************************************************************************************/
/*! \brief       Closes a Tp Rx connection.
 *  \details     Resets a Rx connection back to idle and notifies the PduR. Does not check if an Rx connection
 *               is active.
 *  \param[in]   CtrlPtr        Pointer to Tp channel control structure
 *  \pre         An Rx connection shall be active.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_RxCloseConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        CtrlPtr
);

/**********************************************************************************************************************
 *  LinTp_TxDataRequest()
 *********************************************************************************************************************/
/*! \brief       Handles the transmission data retrieval from PduR.
 *  \details     Requests data from PduR and store it to local LinTp buffer.
 *  \param[in]   CtrlPtr        Pointer to Tp channel control structure
 *  \return      E_OK           Data retrieved or request pending, connection to PduR is alive
 *  \return      E_NOT_OK       Data request denied, PduR has aborted current connection
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_TxDataRequest
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        CtrlPtr
);

/**********************************************************************************************************************
 *  LinTp_TxCloseConnection()
 *********************************************************************************************************************/
/*! \brief       Closes a Tp Tx connection.
 *  \details     Resets a Tx connection back to idle and notifies the PduR. It does not check if a Tx connection
 *               is active.
 *  \param[in]   CtrlPtr        Pointer to Tp channel control structure
 *  \pre         A Tx connection shall be active.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_TxCloseConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        CtrlPtr
);

/**********************************************************************************************************************
 *  LinTp_CloseActiveConnection()
 *********************************************************************************************************************/
/*! \brief       Terminates an ongoing Rx or Tx connection.
 *  \details     Aborts an active TP connection on given channel and notify upper layer. Afterwards, the TP channel
 *               state is idle.
 *  \param[in]   CtrlPtr        Pointer to Tp channel control structure
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_CloseActiveConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        CtrlPtr
);

# endif

# define LINIF_STOP_SEC_CODE
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* LINIF_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf_Int.h
 *********************************************************************************************************************/
