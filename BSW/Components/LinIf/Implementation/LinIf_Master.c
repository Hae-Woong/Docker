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
 *         File:  LinIf_Master.c
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  LIN master specific implementation of the AUTOSAR LIN Interface
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

#define LINIF_MASTER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "LinIf.h"

#if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )

# include "LinIf_Int.h"
# include "LinIf_Master.h"

# if ( LINIF_BUSMIRRORING == STD_ON )
#  include "Mirror_Cbk.h"
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
# if ( LINIF_MASTER_MAJOR_VERSION != (12u) )
#  error "Source and Header file LinIf_Master.h are inconsistent (LINIF_MASTER_MAJOR_VERSION)!"
# endif
# if ( LINIF_MASTER_MINOR_VERSION != (0u) )
#  error "Source and Header file LinIf_Master.h are inconsistent (LINIF_MASTER_MINOR_VERSION)!"
# endif
# if ( LINIF_MASTER_PATCH_VERSION != (0u) )
#  error "Source and Header file LinIf_Master.h are inconsistent (LINIF_MASTER_PATCH_VERSION)!"
# endif

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
# define LINIF_START_SEC_CODE
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LinIf_ScheduleTableHandler()
 *********************************************************************************************************************/
/*! \brief       Handles the current schedule table.
 *  \details     Checks the current schedule state and performs the switch to the next slot or to another schedule table.
 *  \param[out]  FrameMaxDelayPtr              Pointer to a delay value in ticks of the next schedule slot.
 *  \param[out]  FrameHandlePtr                Pointer to a global frame index of the frame in the next schedule slot.
 *  \param[in]   LinIfChannel                  Local LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         -
 *  \return      LinIf_SchdlHdlrRet_OnNullSchedule   NULL table is active
 *  \return      LinIf_SchdlHdlrRet_NoNewMessage     No new message is started
 *  \return      LinIf_SchdlHdlrRet_NewMessage       A new message is started. This also means a new schedule slot begins.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(LinIf_ScheduleTableHandlerRetType, LINIF_CODE) LinIf_ScheduleTableHandler
(
  CONSTP2VAR(LinIf_ScheduleTableTickType, AUTOMATIC, LINIF_VAR_NOINIT)  FrameMaxDelayPtr,
  CONSTP2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, AUTOMATIC)       FrameHandlePtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_ScheduleTableHandler_RunOnceFinalization()
 *********************************************************************************************************************/
/*! \brief       Checks if a run-once schedule table is finished and completes the handling.
 *  \details     Checks if a run-once schedule table is finished in current slot and switches back to
 *               previous schedule table and slot. Also prepares the schedule change notification to upper layer.
 *  \param[in]   SchedTabDataPtr               Pointer to current schedule table data structure.
 *  \param[in]   LinIfChannel                  Local LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         -
 *  \pre         Must only be called if a new schedule slot is due in current cycle.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleTableHandler_RunOnceFinalization
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)       SchedTabDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_ScheduleTableHandler_RequestedTableSwitch()
 *********************************************************************************************************************/
/*! \brief       Performs the switch to a new schedule table if a request is pending.
 *  \details     Performs the actual switch of a pending schedule table request if allowed. Activates the
 *               new table and slot index. It stores backup information if the new table is of type run-once or is an
 *               event-triggered frame collision table and prepares the schedule change notification to upper layer.
 *  \param[in]   SchedTabDataPtr               Pointer to current schedule table data structure
 *  \param[in]   LinIfChannel                  Local LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         -
 *  \pre         Must only be called if a new schedule slot is due in current cycle.
 *  \pre         Must be called after LinIf_ScheduleTableHandler_RunOnceFinalization.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleTableHandler_RequestedTableSwitch
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)       SchedTabDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_CurrentMessageProcessing()
 *********************************************************************************************************************/
/*! \brief       Processes the currently transmitted or received message.
 *  \details     Polls the driver for status of the currently transmitted message and finishes the handling
 *               of the current schedule slot.
 *  \param[in]   ChannelDataPtr       Pointer to channel data structure
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure
 *  \param[in]   LinIfChannel         Local LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_CurrentMessageProcessing
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_MessageProcessing_Unconditional()
 *********************************************************************************************************************/
/*! \brief       Processes a transmitted or received unconditional frame in the current schedule slot.
 *  \details     Handles an unconditional frame in the current slot. If the frame is Tx, the confirmation to upper layer
 *               is prepared and sporadic frame flags are updated. If it's Rx, the received data is copied to local
 *               buffer and indication to upper layer is prepared.
 *  \param[in]   ChannelDataPtr       Pointer to current channel data structure
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure
 *  \param[in]   LinIfChannel         Local LinIf channel identifier. Must reference a LIN master channel.
 *                                    Only available if LINIF_SPORADIC_FRM_SUPPORTED is STD_ON.
 *  \param[in]   LinSduPtr            Pointer to current Lin SDU received from driver
 *  \param[in]   LinStatus            Status value from driver of currently handled frame
 *  \pre         An unconditional frame was transmitted in current schedule slot.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_Unconditional
(
  CONSTP2CONST(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)      ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
# if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
# endif
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
);

# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_MessageProcessing_EventTriggeredFrame()
 *********************************************************************************************************************/
/*! \brief       Processes an elapsed event-triggered frame slot in the current schedule slot.
 *  \details     Depending on the reported status of the driver, the event-triggered frame is either matched with the
 *               corresponding unconditional frame and indicated to upper layer or discarded and the collision resolving
 *               schedule table is set as pending.
 *  \param[in]   ChannelDataPtr       Pointer to current channel data structure.
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure.
 *  \param[in]   LinSduPtr            Pointer to current Lin SDU received from driver.
 *  \param[in]   LinStatus            Status value from driver of currently handled frame.
 *  \pre         An event-triggered frame was transmitted in current schedule slot.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_EVT_FRM_SUPPORTED and LINIF_NODETYPE_MASTER_SUPPORTED
 *  \note        Function only exists if support for event-triggered frames is enabled.
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_EventTriggeredFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
);
# endif

/**********************************************************************************************************************
 *  LinIf_MessageProcessing_SleepModeFrame()
 *********************************************************************************************************************/
/*! \brief       Processes a transmitted sleep mode frame in the current schedule slot.
 *  \details     Performs the transition to sleep state and reinits the schedule table data if the driver indicates sleep status.
 *               In case a wakeup request occurred during sleep frame transmission, the wakeup procedure is restarted.
 *               Prepares a sleep confirmation to upper layer if necessary.
 *  \param[in]   ChannelDataPtr       Pointer to current channel data structure.
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure.
 *  \param[in]   LinIfChannel         Local LinIf channel identifier. Must reference a LIN master channel.
 *  \param[in]   LinStatus            Status value from driver of currently handled frame.
 *  \pre         A sleep mode frame was transmitted in current schedule slot.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_SleepModeFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
);

/**********************************************************************************************************************
 *  LinIf_MessageProcessing_SRF()
 *********************************************************************************************************************/
/*! \brief       Processes a received slave response frame in the current schedule slot.
 *  \details     On successful reception, either forwards the frame to LinTp, completes a node configuration command or is
 *               ignored if no request was transmitted previously. Report DET errors if necessary.
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure.
 *                                    Only available if LINIF_FIXEDFRAMESDULIST is STD_ON.
 *  \param[in]   LinSduPtr            Pointer to current Lin SDU received from driver.
 *  \param[in]   LinIfChannel         Local LinIf channel identifier. Must reference a LIN master channel.
 *  \param[in]   LinStatus            Status value from driver of currently handled frame
 *  \pre         A slave response header shall was transmitted in current schedule slot.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_SRF
(
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
# endif
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
);

# if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing_SporadicFrame()
 *********************************************************************************************************************/
/*! \brief       Searches for a flagged pending frame to be transmitted in current sporadic slot.
 *  \details     If frame marked as pending is found, updates the slot data with information of this frame.
 *  \param[in]   ChannelDataPtr       Pointer to current channel data structure
 *  \param[in]   LinPduPtr            Pointer to Lin PDU to be passed to driver
 *  \param[out]  NewFrameHandlePtr    Pointer to frame handle of selected sporadic frame. Only valid if function
 *                                    returns TRUE
 *  \returns     TRUE                 if frame to be transmitted in this sporadic slot was found
 *  \returns     FALSE                if no frame is marked as pending in this sporadic slot
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_SPORADIC_FRM_SUPPORTED and LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_SporadicFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)          ChannelDataPtr,
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                    LinPduPtr,
  CONSTP2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, LINIF_VAR_NOINIT)  NewFrameHandlePtr
);
# endif

/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing_SleepHandling()
 *********************************************************************************************************************/
/*! \brief       Checks if a sleep request is pending for current schedule slot and starts the sleep transition.
 *  \details     Triggers the driver to transit to sleep state, either by internal transition or with sleep frame transmission.
 *  \param[in]   ChannelDataPtr       Pointer to current channel data structure.
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure.
 *  \param[in]   LinIfChannel         Local LinIf channel identifier. Must reference a LIN master channel.
 *  \returns     TRUE                 A sleep request is handled in current schedule slot
 *  \returns     FALSE                No sleep handling is performed in current schedule slot
 *  \pre         This function shall only be called if a new schedule table slot starts.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_SleepHandling
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing_UnconditionalFrame()
 *********************************************************************************************************************/
/*! \brief       Prepares an unconditional Tx frame for transmission in current slot.
 *  \details     Triggers PduR for provision of transmission data.
 *  \param[in]   LinPduPtr                Pointer to Lin PDU to be passed to driver
 *  \param[in]   FrameHandle              Handle of unconditional frame to be processed.
 *  \returns     TRUE                     The header of this slot shall be transmitted, data was copied to LIN Pdu
 *  \returns     FALSE                    No header shall be transmitted, no transmission data available
 *  \pre         This function shall only be called if a new schedule table slot starts.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_UnconditionalFrame
(
  CONSTP2CONST(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                LinPduPtr,
  VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC)                         FrameHandle
);

/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing_FrameHandling()
 *********************************************************************************************************************/
/*! \brief         Handles a new/starting schedule table slot depending on the frame type.
 *  \details       Depending on the frame type, transmission data is requested and conditions are checked if a header or
 *                 complete frame shall be transmitted in the slot or if the slot shall remain empty.
 *  \param[in]     ChannelDataPtr       Pointer to current channel data structure
 *  \param[in]     MasterChannelDataPtr Pointer to master-specific channel data structure.
 *  \param[in]     TpCtrlPtr            Pointer to Tp channel data.
 *                                      Only applicable if LINIF_TP_SUPPORTED is STD_ON.
 *  \param[in]     LinPduPtr            Pointer to Lin PDU to be passed to driver
 *  \param[in,out] FrameHandlePtr       Pointer to frame handle to be processed in current slot. Might be overwritten
 *                                      if frame is replaced by another frame in this slot.
 *  \returns       TRUE                 Slot shall be handled and header and/or response shall be transmitted
 *  \returns       FALSE                Slot shall remain silent
 *  \pre           This function shall only be called if a new schedule table slot starts.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_FrameHandling
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
# if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)                 TpCtrlPtr,
# endif
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                  LinPduPtr,
  P2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, LINIF_VAR_NOINIT)     FrameHandlePtr
);

/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing()
 *********************************************************************************************************************/
/*! \brief       Processes schedule tables and slots.
 *  \details     Performs general processing of the current schedule slot and triggers the transmission of
 *               header and response. Called every main function cycle if channel is operational.
 *  \param[in]   ChannelDataPtr       Pointer to current channel data structure.
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure.
 *  \param[in]   LinIfChannel         Local LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleProcessing
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_ChannelMainFunctionPostProcessing_LinSM()
 *********************************************************************************************************************/
/*! \brief       Calls necessary indication and confirmation functions of LinSM module.
 *  \details     Indicates schedule end and schedule change confirmation to LinSM.
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure.
 *  \param[in]   LinIfChannel         Local LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         Schedule and message processing was already performed for this cycle.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelMainFunctionPostProcessing_LinSM
(
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

/**********************************************************************************************************************
 *  LinIf_ProcessWakeupTransition()
 *********************************************************************************************************************/
/*! \brief       Sets the channel to OPERATIONAL state and requests the transmission of a wakeup frame if necessary.
 *  \details     Sets the LinIf and LIN driver to operational state. It depends on the value of the external wakeup
 *               flag if an internal or external wakeup process is executed. The configuration of the status call
 *               behavior afer wakeup frame transmission is considered.
 *  \param[in]   ChannelDataPtr       Pointer to current channel data structure.
 *  \param[in]   MasterChannelDataPtr Pointer to master-specific channel data structure.
 *  \param[in]   LinIfChannel         Local LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL FUNC(void, LINIF_CODE) LinIf_ProcessWakeupTransition
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);

# if ( LINIF_BUSMIRRORING == STD_ON )
/**********************************************************************************************************************
 *  LinIf_ChannelMainFunctionPostProcessing_Mirror()
 *********************************************************************************************************************/
/*! \brief       Informs the mirror module about frame reception / transmission.
 *  \details     If a frame is pending to be reported, the mirror module is called for the frame processed in the
 *               current cycle.
 *  \param[in]   MasterChannelDataPtr   Pointer to master-specific channel data structure
 *  \param[in]   LinIfChannel           Local LinIf channel identifier. Must reference a LIN master channel.
 *  \pre         Schedule and message processing was already performed for this cycle.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_BUSMIRRORING and LINIF_NODETYPE_MASTER_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelMainFunctionPostProcessing_Mirror
(
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
);
# endif


# if ( LINIF_TP_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LOCAL LINTP FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_MasterRxIndication_Main
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a SRF.
 *  \details     This function processes a received SRF containing transport protocol data.
 *  \param[in]   LinIfChannel             Local LinIf channel identifier.  Must reference a LIN master channel.
 *  \param[in]   LinSduPtr                Pointer to receive buffer of slave response frame
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED and LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_MasterRxIndication_Main
(
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr
);

/**********************************************************************************************************************
 *  LinTp_MasterRxIndication_SF()
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a single frame N-PDU.
 *  \details     This function processes a received SRF containing a single frame. Terminates a possible ongoing reception
 *               on the same channel, performs response pending frame handling if necessary and initiates a new receive
 *               connection.
 *  \param[in]   CtrlPtr             Pointer to Tp channel control structure
 *  \param[in]   LinIfChannel        Local LinIf channel identifier.  Must reference a LIN master channel.
 *  \param[in]   LinSduPtr           Pointer to receive buffer of slave response frame
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED and LINIF_TP_SUPPORTED
 *  \trace       CREQ-593
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_MasterRxIndication_SF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)           CtrlPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr
);

/**********************************************************************************************************************
 *  LinTp_MasterRxIndication_FF()
 *********************************************************************************************************************/
/*! \brief       Handles the reception of a first frame N-PDU.
 *  \details     This function processes a received SRF containing a first frame. Terminates a possible ongoing reception
 *               on the same channel and initiates a new receive connection.
 *  \param[in]   CtrlPtr             Pointer to Tp channel control structure
 *  \param[in]   LinSduPtr           Pointer to receive buffer of slave response frame
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED and LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_MasterRxIndication_FF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)           CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr
);

/**********************************************************************************************************************
 *  LinTp_MasterTxMessageProcessing()
 *********************************************************************************************************************/
/*! \brief       Post-processes the transmission of a MRF containing transport protocol data.
 *  \details     Closes connection after transmission of last frame of connection, otherwise prepares next consecutive
 *               frame for transmission.
 *  \param[in]   CtrlPtr                  Pointer to Tp channel control structure
 *  \param[in]   LinIfChannel             Local LinIf channel identifier.  Must reference a LIN master channel.
 *  \param[in]   LinStatus                Status value from driver for transmitted MRF
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED and LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_MasterTxMessageProcessing
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)            CtrlPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
);

/**********************************************************************************************************************
 *  LinTp_MasterTxScheduleProcessing()
 *********************************************************************************************************************/
/*! \brief       Prepares a MRF for transmission used by transport protocol.
 *  \details     Checks whether TP transmission data is available for a connection on given channel and prepares the given
 *               LIN PDU.
 *  \param[in]   CtrlPtr                  Pointer to Tp channel control structure
 *  \param[in]   MasterChannelDataPtr     Pointer to master-specific channel data
 *  \param[in]   LinPduPtr                Pointer to Lin PDU to be passed to driver
 *  \returns     TRUE                     Header is transmitted in current slot.
 *  \returns     FALSE                    No header is transmitted, slot remains empty.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED and LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinTp_MasterTxScheduleProcessing
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)            CtrlPtr,
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON ) || ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
#  endif
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                  LinPduPtr
);

# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinIf Initialization, Task and General Function Interface - Local Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_ChannelMainFunctionPostProcessing_LinSM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelMainFunctionPostProcessing_LinSM
(
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
)
{
  /* ----- Implementation ----------------------------------------------- */
  /**********************************************************
  * LinSM notification that current schedule is run through
  **********************************************************/
  /* #10 Report schedule table end confirmation if notification is pending */
# if ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
  if (MasterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedEndNotifyTable != LinIf_Schedule_Invalid)
  {
    LinSM_ScheduleEndNotification(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), MasterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedEndNotifyTable );
    MasterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedEndNotifyTable = LinIf_Schedule_Invalid; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
  }
# endif

  /**********************************************************
  * LinIf post notification to LinSm of schedule change
  **********************************************************/
  /* #20 Report schedule table change confirmation if notification is pending */
  /* delayed schedule request confirmation to LinSM */
  if (MasterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable != LinIf_Schedule_Invalid)
  {
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
    LinIf_GetScheduleReqConfFctList(LinIf_GetScheduleReqConfFctListIdxOfMasterChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel)))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                        LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel),
                                        MasterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable);
# else
    LinSM_ScheduleRequestConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel),
                                      MasterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable);
# endif
    /* invalidate schedule request confirmation notification flag */
    MasterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable = LinIf_Schedule_Invalid; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
  }
}

# if ( LINIF_BUSMIRRORING == STD_ON )
/**********************************************************************************************************************
 *  LinIf_ChannelMainFunctionPostProcessing_Mirror()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelMainFunctionPostProcessing_Mirror
(
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If a frame reception / transmission is pending for reporting */
  if ( MasterChannelDataPtr->LinIf_MirroringEventPending == TRUE )
  {
    PduInfoType pduInfo;

    /* #20 Setup frame data and parameters to be reported */
    if ( (MasterChannelDataPtr->LinIf_MirroringLinStatus == LIN_RX_OK) || (MasterChannelDataPtr->LinIf_MirroringLinStatus == LIN_TX_OK) )
    {
      pduInfo.SduLength = MasterChannelDataPtr->LinIf_PduRNotificationPduLength;
      pduInfo.SduDataPtr = &(MasterChannelDataPtr->LinIf_RxMirrorData[0]);
    }
    else
    {
      pduInfo.SduLength = 0x00u;
      pduInfo.SduDataPtr = NULL_PTR;
    }

    /* #30 Inform mirror module about reception / transmission */
    Mirror_ReportLinFrame(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), MasterChannelDataPtr->LinIf_MirroringPid, /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR */
                          &pduInfo, MasterChannelDataPtr->LinIf_MirroringLinStatus);

    /* clear pending mirror notification */
    MasterChannelDataPtr->LinIf_MirroringEventPending = FALSE; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
  }
}
# endif

/**********************************************************************************************************************
 *
 * END - LinIf Initialization, Task and General Function Interface - Local Functions
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * LOCAL FUNCTIONS - LinIf Schedule Table Handler
 *
 * The LinIf schedule table handler determines the current schedule position and checks if a new slot is due. It also
 * checks if a new schedule table is requested or if the current table is run through. If necessary, the actual switch
 * to a new schedule table or schedule slot is performed.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_ScheduleTableHandler_RunOnceFinalization()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleTableHandler_RunOnceFinalization
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)         SchedTabDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                       LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  LinIf_ScheduleTableListIndStartIdxOfMasterChannelConfigType             schedTabListIndStartIdx; /* initialized below before being used */

  /* ----- Implementation ----------------------------------------------- */
  schedTabListIndStartIdx = LinIf_GetScheduleTableListIndStartIdxOfMasterChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel));

  /* #10 If current schedule table is of type run-once and run through */
  if ( ( LinIf_GetRunModeOfScheduleTableList( LinIf_GetScheduleTableListInd(schedTabListIndStartIdx) +
         SchedTabDataPtr->LinIf_CurrentTable) == LinIf_RunMode_RunOnce ) &&
       ( SchedTabDataPtr->LinIf_CurrentSlot == (uint8)0x00u ) )
  {
    /* Current table is run once and table is all run through */

    /* #20 Change to to last continuous schedule table and saved resume slot */
    SchedTabDataPtr->LinIf_CurrentTable = SchedTabDataPtr->LinIf_PreviousContinuousTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    if ( LinIf_GetResumePositionOfScheduleTableList( LinIf_GetScheduleTableListInd(schedTabListIndStartIdx) +
          SchedTabDataPtr->LinIf_CurrentTable ) == LinIf_CONTINUE_AT_IT_POINT )
    {
      /* Start from interrupted slot */
      SchedTabDataPtr->LinIf_CurrentSlot = SchedTabDataPtr->LinIf_PreviousContinuousSlot; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    }
    else
    {
      /* Start from beginning slot */
      SchedTabDataPtr->LinIf_CurrentSlot = (uint8)0x00u; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    }

    /* #30 If no schedule request is pending, inform about change to continuous table */
    if ( SchedTabDataPtr->LinIf_RequestedTable == LinIf_Schedule_Invalid )
    {
      SchedTabDataPtr->LinIf_SchedReqConfirmTable = SchedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    }
  }
}

/**********************************************************************************************************************
 *  LinIf_ScheduleTableHandler_RequestedTableSwitch()
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
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleTableHandler_RequestedTableSwitch
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)         SchedTabDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                       LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  LinIf_ScheduleTableListIndStartIdxOfMasterChannelConfigType             schedTabListIndStartIdx; /* initialized below before being used */

  /* ----- Implementation ----------------------------------------------- */
  schedTabListIndStartIdx = LinIf_GetScheduleTableListIndStartIdxOfMasterChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel));

  /* #10 If a new table is requested */
  if ( SchedTabDataPtr->LinIf_RequestedTable != LinIf_Schedule_Invalid )
  {
    /* #20 If current active table is of type run-continuous */
    if ( LinIf_GetRunModeOfScheduleTableList( LinIf_GetScheduleTableListInd(schedTabListIndStartIdx) +
         SchedTabDataPtr->LinIf_CurrentTable ) == LinIf_RunMode_RunContinuous )
    {
      /* #30 A normal continuous table is active, switch to requested schedule */
      if ( LinIf_GetRunModeOfScheduleTableList( LinIf_GetScheduleTableListInd(schedTabListIndStartIdx) +
           SchedTabDataPtr->LinIf_RequestedTable) == LinIf_RunMode_RunOnce )
      { /* #40 If requested table is a run-once table, save position of current table */
        SchedTabDataPtr->LinIf_PreviousContinuousTable   = SchedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
        SchedTabDataPtr->LinIf_PreviousContinuousSlot    = SchedTabDataPtr->LinIf_CurrentSlot; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      }
# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
      else if ( SchedTabDataPtr->LinIf_EvtCollResolvingState == LinIf_EvtCollResolvingState_Detected )
      { /* #50 If new table is a collision resolving schedule, save position of current table */
        SchedTabDataPtr->LinIf_PreviousContinuousTable   = SchedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
        SchedTabDataPtr->LinIf_PreviousContinuousSlot    = SchedTabDataPtr->LinIf_CurrentSlot; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
        SchedTabDataPtr->LinIf_EvtCollResolvingState     = LinIf_EvtCollResolvingState_OnResolving; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      }
      else
      {
        /* MISRA only */
      }
# endif
      /* #60 Activate new schedule table and slot */
      SchedTabDataPtr->LinIf_CurrentTable                = SchedTabDataPtr->LinIf_RequestedTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      SchedTabDataPtr->LinIf_CurrentSlot                 = (uint8)0x00u; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      /* schedule table change handled, remove request */
      SchedTabDataPtr->LinIf_RequestedTable              = LinIf_Schedule_Invalid; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      /* #70 Confirm schedule request, only if new schedule is not a collision resolving table */
# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
      if ( SchedTabDataPtr->LinIf_EvtCollResolvingState != LinIf_EvtCollResolvingState_OnResolving )
# endif
      { /* inform upper layer about schedule table change */
        SchedTabDataPtr->LinIf_SchedReqConfirmTable      = SchedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      }
    }
    /* else : currently a run once table is active which is not run through yet so wait until table end */
  }
}

/**********************************************************************************************************************
 *  LinIf_ScheduleTableHandler()
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
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
LINIF_LOCAL_INLINE FUNC(LinIf_ScheduleTableHandlerRetType, LINIF_CODE) LinIf_ScheduleTableHandler
(
  CONSTP2VAR(LinIf_ScheduleTableTickType, AUTOMATIC, LINIF_VAR_NOINIT)    FrameMaxDelayPtr,
  CONSTP2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, AUTOMATIC)         FrameHandlePtr,
  VAR(NetworkHandleType, AUTOMATIC)                                       LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)              schedTabDataPtr; /* initialized below before being used */
  LinIf_ScheduleTableHandlerRetType                                       retVal; /* initialized below before being used */

  /* ----- Implementation ----------------------------------------------- */
  schedTabDataPtr   = &(LinIf_GetMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel)).LinIf_SchedTabData);

  /* #10 If Null schedule table is active and no request is pending, do nothing and quit */
  if ( (schedTabDataPtr->LinIf_CurrentTable == LinIf_Schedule_NULL) &&
       (schedTabDataPtr->LinIf_RequestedTable == LinIf_Schedule_Invalid) )
  {
    retVal          = LinIf_SchdlHdlrRet_OnNullSchedule;
  }
  /* #20 Otherwise: */
  else
  {
    /* #30 Evaluate if handling of next schedule entry can be started: */
    retVal          = LinIf_SchdlHdlrRet_NoNewMessage;

    /* #31 If current slot not yet finished, decrease delay counter */
    if ( schedTabDataPtr->LinIf_CurrentSlotDelay > (LinIf_ScheduleTableTickType)0x00u )
    {
      schedTabDataPtr->LinIf_CurrentSlotDelay--; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
    }

    /* #32 If slot delay has elapsed, next slot can be started in any case */
    if ( schedTabDataPtr->LinIf_CurrentSlotDelay == (LinIf_ScheduleTableTickType)0u )
    {
      retVal            = LinIf_SchdlHdlrRet_NewMessage;
    }
    /* #33 If schedule change after frame delay is enabled, the next slot can also be started if a schedule table change request is pending and
     *     and the frame inside the current slot is already finished
     *     and the current schedule table is of type run-continuous or the current schedule table is of type run-once and the table is all run through */
    else
    {
      if ( ( LinIf_IsScheduleChangeBeforeSlotEndOfMasterChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel)) ) &&
           ( schedTabDataPtr->LinIf_RequestedTable != LinIf_Schedule_Invalid ) &&
           ( LinIf_GetMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel)).LinIf_FrameMaxDelay == (LinIf_ScheduleTableTickType)0u ) ) /* COV_LINIF_SCHEDULECHANGE_POINT_1 */
      {
        LinIf_ScheduleTableListIndStartIdxOfMasterChannelConfigType schedTabListIndStartIdx;
        LinIf_RunModeOfScheduleTableListType                        schedRunMode;

        schedTabListIndStartIdx = LinIf_GetScheduleTableListIndStartIdxOfMasterChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel));
        schedRunMode            = LinIf_GetRunModeOfScheduleTableList( LinIf_GetScheduleTableListInd(schedTabListIndStartIdx) +
                                                                       schedTabDataPtr->LinIf_CurrentTable);
        if ( (( schedRunMode == LinIf_RunMode_RunOnce ) && ( schedTabDataPtr->LinIf_CurrentSlot == (uint8)0x00u )) ||
              ( schedRunMode == LinIf_RunMode_RunContinuous ) ) /* COV_LINIF_SCHEDULECHANGE_POINT_2 */
        {
          retVal       = LinIf_SchdlHdlrRet_NewMessage;
        }
      }
    }

    /* #40 If handling of next schedule entry can be started */
    if ( retVal == LinIf_SchdlHdlrRet_NewMessage )
    {
      /* #50 if NULL schedule table is requested, set it and quit, no further handling required */
      if ( schedTabDataPtr->LinIf_RequestedTable == LinIf_Schedule_NULL )
      {
        /* switch to NULL schedule table */
        LinIf_FlushAllSchedules(schedTabDataPtr); /* SBSW_LINIF_FLUSHALLSCHED_CALL_2 */
        schedTabDataPtr->LinIf_SchedReqConfirmTable = (uint8)0x00u; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
        retVal                                      = LinIf_SchdlHdlrRet_OnNullSchedule;
        /* Set delay to 0, no message started */
        *FrameMaxDelayPtr                           = (LinIf_ScheduleTableTickType)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
        /* schedule change handled, remove request */
        schedTabDataPtr->LinIf_RequestedTable       = LinIf_Schedule_Invalid; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
      }
      /* #60 Otherwise: Do actual schedule table handling */
      else
      {
        /* Preset return value to LinIf_SchdlHdlrRet_NewMessage, maybe changed later in function */
        retVal                                      = LinIf_SchdlHdlrRet_NewMessage;

        /* #70 At first check if a RUN_ONCE schedule is finished. In this case, switch back to previous table and slot */
        LinIf_ScheduleTableHandler_RunOnceFinalization(schedTabDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_WITH_SCHEDTABPTR */

        /* #80 If a new table is requested, perform the actual switch (if not a RUNONCE schedule is active) and notify the schedule change */
        LinIf_ScheduleTableHandler_RequestedTableSwitch(schedTabDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_WITH_SCHEDTABPTR */

        /* #90 If new table is NULL schedule table, reset frame delay and quit */
        if ( schedTabDataPtr->LinIf_CurrentTable == LinIf_Schedule_NULL )
        {
          /* RUN_ONCE finished and switched back to NULL schedule table, set delay to 0, no message started */
          *FrameMaxDelayPtr               = (LinIf_ScheduleTableTickType)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
          retVal                          = LinIf_SchdlHdlrRet_OnNullSchedule;
        }
        /* #100 Otherwise */
        else
        {
# if ( LINIF_J2602_FRAME_TOLERANCE_SUPPORT == STD_ON )
# else
          uint8 frameLength;
# endif

          /* #110 Retrieve current message information */
          /* get index into global schedule table list for current table */
          LinIf_ScheduleTableListIndStartIdxOfMasterChannelConfigType schedTabListIdx = (LinIf_ScheduleTableListIndStartIdxOfMasterChannelConfigType)(
                                                  LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfMasterChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel))) +
                                                  schedTabDataPtr->LinIf_CurrentTable);
          /* get index into global entry slot list for current slot */
          LinIf_EntryStartIdxOfScheduleTableListType slotListIdx = (LinIf_EntryStartIdxOfScheduleTableListType)(
                                                  LinIf_GetEntryStartIdxOfScheduleTableList(schedTabListIdx) +
                                                  schedTabDataPtr->LinIf_CurrentSlot);
          /* store delay in ticks of current slot */
          schedTabDataPtr->LinIf_CurrentSlotDelay = LinIf_GetDelayInTimebaseTicksOfEntry(slotListIdx); /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
          /* get global index into LinIf_FrameList of referenced frame */
          *FrameHandlePtr                 = LinIf_GetFrameListIdxOfEntry(slotListIdx); /* SBSW_LINIF_PASSED_PTR_ACCESS */

# if ( LINIF_J2602_FRAME_TOLERANCE_SUPPORT == STD_ON )
          /* Get the transmission of current frame in timebase ticks for this specific frame */
          *FrameMaxDelayPtr               = LinIf_GetFrameLengthDelayInTicksOfFrameList(*FrameHandlePtr); /* SBSW_LINIF_PASSED_PTR_ACCESS */
# else
          /* get length of frame in bytes. Because global index is directly used from entry, no need to do channel-based recalculation */
          frameLength                     = LinIf_GetLengthOfFrameList(*FrameHandlePtr);
          /* Get the transmission time of current frame in timebase ticks depending on the length of the frame */
          *FrameMaxDelayPtr               = (LinIf_ScheduleTableTickType)LinIf_GetFrameLengthDelayOfFrameLengthDelayList( /* SBSW_LINIF_PASSED_PTR_ACCESS */
                                                         LinIf_GetFrameLengthDelayListStartIdxOfChannelConfig(LinIfChannel) + ((uint16)frameLength - 1u));
# endif

          /* #120 If the frame duration exceeds the schedule slot duration, use slot duration for frame handling */
          /* Note that this case means an inconsistent configuration which the generator has detected and reported to the user.
             However, as the maximum jitter duration is considered in general and might not effect all frames (e.g. Tx frames),
             this case might also happen in real configuration without negative impact. Following check ensures that the LIN driver is
             requested for the status of the frame in every case before a new slot is started. */
          if ( *FrameMaxDelayPtr > schedTabDataPtr->LinIf_CurrentSlotDelay )
          {
            *FrameMaxDelayPtr = schedTabDataPtr->LinIf_CurrentSlotDelay; /* SBSW_LINIF_PASSED_PTR_ACCESS */
          }

          /* #130 Update slot index and check if new slot is last one in table */
          schedTabDataPtr->LinIf_CurrentSlot++; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
          if ( schedTabDataPtr->LinIf_CurrentSlot >= LinIf_GetNumberOfScheduleEntriesOfScheduleTableList(schedTabListIdx))
          {
            schedTabDataPtr->LinIf_CurrentSlot = (uint8)0x00u; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */

            /* #140 Inform LinSM if header of last frame of current schedule table is going to be transmitted */
# if ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
            schedTabDataPtr->LinIf_SchedEndNotifyTable = schedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
# endif

            /* #150 Switch back to previous table if collision resolving table is run through */
# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
            if ( schedTabDataPtr->LinIf_EvtCollResolvingState == LinIf_EvtCollResolvingState_OnResolving )
            { /* collision resolving table is run through, so switch back to previous table */
              schedTabDataPtr->LinIf_CurrentSlot            = schedTabDataPtr->LinIf_PreviousContinuousSlot; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
              schedTabDataPtr->LinIf_CurrentTable           = schedTabDataPtr->LinIf_PreviousContinuousTable; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
              schedTabDataPtr->LinIf_EvtCollResolvingState  = LinIf_EvtCollResolvingState_Idle; /* SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER */
            }
# endif
          }
        }
      }
    }
  }
  return(retVal);
}

/**********************************************************************************************************************
 *
 * END - LinIf Schedule Table Handler
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * LOCAL FUNCTIONS - LinIf Schedule Processing
 *
 * Following functions handle a new schedule slot. In case a sleep request is pending, the actual slot is discarded and
 * a sleep frame is transmitted instead in this slot. Otherwise the new slot is handled by determining the actual frame
 * type of this slot, requesting transmission data if necessary, preparing the Lin PDU and sending the message by
 * triggering the LIN driver.
 *
 *********************************************************************************************************************/


# if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing_SporadicFrame()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_SporadicFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)           ChannelDataPtr,
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                     LinPduPtr,
  CONSTP2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, LINIF_VAR_NOINIT)   NewFrameHandlePtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* LinIf_FixedFrameSduReference contains the sporadic slot index into SporadicFrameInfo slot list */
  uint8_least                                                              sporFrmCounterIdx = 0u;
  boolean                                                                  sporFrameRequested = FALSE; /* Skip frame slot by default */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all sporadic unconditional frames assigned to the sporadic slot */
  while ( sporFrmCounterIdx < (uint8)LinIf_GetUncFrameCountOfSporadicFrameInfo(LinIf_GetFixedFrameSduListIdxOfFrameList(ChannelDataPtr->LinIf_FrameHandle)) ) /* FETA_LINIF_UPPER_LIMIT_IS_GENERATED_CONSTANT */
  {
    /* get the index of the sporadic unconditional frame by LinIf_SporadicFrameInfo -> LinIf_SporUncFrameByPriorityList -> LinIf_SporUnconditionalFrame */
    LinIf_SporUnconditionalFrameIdxOfSporUncFrameByPriorityListType sporUncFrmIdx =
                          LinIf_GetSporUnconditionalFrameIdxOfSporUncFrameByPriorityList(
                           LinIf_GetSporUncFrameByPriorityListStartIdxOfSporadicFrameInfo(
                            LinIf_GetFixedFrameSduListIdxOfFrameList(ChannelDataPtr->LinIf_FrameHandle)) + sporFrmCounterIdx);
    uint8 sporUncFrmByteMsk = LinIf_GetByteMskOfSporUnconditionalFrame(sporUncFrmIdx);
    uint8 sporUncFrmBitMsk  = LinIf_GetBitMskOfSporUnconditionalFrame(sporUncFrmIdx);

    /* #20 If the transmission flag of the sporadic unconditional frame is set */
    if ( (LinIf_GetSporTransmitFlags(sporUncFrmByteMsk) & sporUncFrmBitMsk) != (LinIf_SporTransmitFlagsType)0u) /* COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL */
    { /* Flag is set for the current unconditional frame - transmit this frame. No further search necessary as in order by priority */

      /* #30 Replace current frame information with flagged unconditional frame info to be transmitted in current cycle */
      ChannelDataPtr->LinIf_FrameHandle             = LinIf_GetFrameListIdxOfSporUnconditionalFrame(sporUncFrmIdx); /* SBSW_LINIF_PASSED_PTR_ACCESS */
      *NewFrameHandlePtr                            = (LinIf_FrameListIdxOfEntryType)ChannelDataPtr->LinIf_FrameHandle; /* SBSW_LINIF_PASSED_PTR_ACCESS */

      /* set sporadic frame to LinIf_FrameType_UnconditionalFrame */
      ChannelDataPtr->LinIf_CurrentFrameType        = LinIf_GetFrameTypeOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS */

      /* #40 Setup given LIN Pdu with new message information */
      LinPduPtr->Cs                                 = (Lin_FrameCsModelType) LinIf_GetChecksumTypeOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS */
      LinPduPtr->Dl                                 = (Lin_FrameDlType) LinIf_GetLengthOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS */
      LinPduPtr->Drc                                = (Lin_FrameResponseType) LinIf_GetDirectionOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS */
      LinPduPtr->Pid                                = (Lin_FramePidType) LinIf_GetPidOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS */

      /* #50 Activate transmission of unconditional frame in this slot */
      sporFrameRequested                            = TRUE;

      /* quit search for flagged unconditional frame */
      break;
    }
    sporFrmCounterIdx++;
  }

  return sporFrameRequested;
}
# endif


/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing_SleepHandling()
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
 */
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_SleepHandling
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                                        retVal = E_NOT_OK;
  boolean                                                               isSleepRequestHandled = FALSE;
  boolean                                                               linSmSuccessParamValue = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If a (normal) goto-sleep request is pending */
  if ( MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReq )
  {
    /* #20 Trigger sleep frame transmission in LIN driver */
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
    retVal = LinIf_GetLin_GoToSleepFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
    retVal = LinIf_Lin_GoToSleep_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel));
# endif

    /* #30 If sleep frame transmission request is accepted */
    if ( retVal == E_OK )
    {
      /* #40 Update slot info and set transmission delay */
      MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling  = LinIf_OnSleepModeFrameHandling_OnTrans; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      ChannelDataPtr->LinIf_CurrentFrameType                = LinIf_FrameType_SleepModeFrame; /* SBSW_LINIF_PASSED_PTR_ACCESS */
# if ( LINIF_J2602_FRAME_TOLERANCE_SUPPORT == STD_ON )
      MasterChannelDataPtr->LinIf_FrameMaxDelay             = (LinIf_ScheduleTableTickType)LinIf_GetSleepFrameLengthDelayInTicksOfMasterChannelConfig( /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
                                                                                            LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel));
# else
      MasterChannelDataPtr->LinIf_FrameMaxDelay             = LinIf_GetFrameLengthDelayOfFrameLengthDelayList( /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
                                                                LinIf_GetFrameLengthDelayListStartIdxOfChannelConfig(LinIfChannel) + 7u);
# endif
    }

    isSleepRequestHandled = TRUE;
  }
  /* #50 Else if an internal goto-sleep request is pending */
  else if ( MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReqInt )
  {
    /* #60 Trigger internal sleep mode transition in LIN driver */
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
    retVal = LinIf_GetLin_GoToSleepInternalFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
    retVal = LinIf_Lin_GoToSleepInternal_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel));
# endif

    /* #70 If internal sleep mode transition request is accepted */
    if ( retVal == E_OK )
    {
      /* #80 Update slot info and set transmission delay */
      MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling  = LinIf_OnSleepModeFrameHandling_OnTransInt; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      ChannelDataPtr->LinIf_CurrentFrameType                = LinIf_FrameType_SleepModeFrame; /* SBSW_LINIF_PASSED_PTR_ACCESS */
      MasterChannelDataPtr->LinIf_FrameMaxDelay             = (LinIf_ScheduleTableTickType)0x01u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    }

    linSmSuccessParamValue = TRUE;
    isSleepRequestHandled = TRUE;
  }
  else
  {
    /* nothing to do */
  }

  /* #90 If the goto-sleep request has been handled above */
  if ( isSleepRequestHandled == TRUE )
  {
    /* #100 If the request was not accepted, directly transmit to SLEEP state and finalize sleep mode transition  */
    if ( retVal != E_OK )
    {
      ChannelDataPtr->LinIf_ChannelState                    = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_PASSED_PTR_ACCESS */
      MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling  = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

      LinIf_FlushAllSchedules(&(MasterChannelDataPtr->LinIf_SchedTabData)); /* SBSW_LINIF_FLUSHALLSCHED_CALL_1 */

      /* Sleep mode frame will not be sent, but also inform LinSM */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                    LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), linSmSuccessParamValue);
# else
      LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), linSmSuccessParamValue);
# endif
    }

# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
    /* #110 Reset wakeup flag in any case */
    ChannelDataPtr->LinIf_WakeupFlag = (uint8)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
# endif
  }

  return isSleepRequestHandled;
}


/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing_UnconditionalFrame()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_UnconditionalFrame
(
  CONSTP2CONST(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)            LinPduPtr,
  VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC)                     FrameHandle
)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean                                                           sendFrameFlag = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If unconditional frame has direction Tx */
  if ( LinIf_GetDirectionOfFrameList(FrameHandle) == LIN_FRAMERESPONSE_TX )
  {
    PduInfoType pduInfo;

    /* #20 Request data from PduR for this frame */
    pduInfo.SduLength     = LinIf_GetLengthOfFrameList(FrameHandle);
    pduInfo.SduDataPtr    = LinPduPtr->SduPtr;
# if ( LINIF_IGNORE_TRIGGERTRANSMIT_RETVAL == STD_OFF )
#  if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
    if ( LinIf_GetTxTriggerFctList(LinIf_GetTxTriggerFctListTxTriggerFctIdxOfFrameList(FrameHandle)) /* SBSW_LINIF_FCT_POINTER_CALL_PDUINFO */
              (LinIf_GetPduIdOfFrameList(FrameHandle), &pduInfo) != E_OK)
#  else
    if ( PduR_LinIfTriggerTransmit(LinIf_GetPduIdOfFrameList(FrameHandle), &pduInfo) != E_OK) /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR_PDUINFO */
#  endif
    { /* #30 Leave slot empty if transmission data cannot be provided */
      sendFrameFlag      = FALSE;
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
      (void)Det_ReportRuntimeError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, LINIF_E_TRIGGERTRANSMIT_NO_DATA);
#  endif
    }
# else
#  if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
    (void)LinIf_GetTxTriggerFctList(LinIf_GetTxTriggerFctListTxTriggerFctIdxOfFrameList(FrameHandle))  /* SBSW_LINIF_FCT_POINTER_CALL_PDUINFO */
              (LinIf_GetPduIdOfFrameList(FrameHandle), &pduInfo);
#  else
    (void)PduR_LinIfTriggerTransmit(LinIf_GetPduIdOfFrameList(FrameHandle), &pduInfo); /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR_PDUINFO */
#  endif
# endif
  }

  return sendFrameFlag;
}

/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing_FrameHandling()
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
 */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinIf_ScheduleProcessing_FrameHandling
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)            ChannelDataPtr, /* PRQA S 3673 */ /* MD_LINIF_Rule8.13_3673_3678 */
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)      MasterChannelDataPtr,
# if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)                     TpCtrlPtr,
# endif
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                      LinPduPtr,
  P2VAR(LinIf_FrameListIdxOfEntryType, AUTOMATIC, LINIF_VAR_NOINIT)         FrameHandlePtr /* PRQA S 3673 */ /* MD_LINIF_Rule8.13_3673_3678 */
)
{
  /* ----- Local Variables ---------------------------------------------- */
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  uint8_least                                                               bufferCounterIdx; /* initialized below in for loop */
# endif
  boolean                                                                   sendFrameFlag = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle frame depending on frame type */
  switch ( ChannelDataPtr->LinIf_CurrentFrameType )
  {

# if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
    /***********************************************************************************************************
     *  LinIf_FrameType_SporadicFrame
     **********************************************************************************************************/
    /* #20 Sporadic frame: */
    case LinIf_FrameType_SporadicFrame:

      /* #30 If an associated unconditional frame is flagged for transmission, process unconditional Tx frame in this slot */
      sendFrameFlag = LinIf_ScheduleProcessing_SporadicFrame(ChannelDataPtr, LinPduPtr, FrameHandlePtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

      if ( sendFrameFlag == TRUE ) /* COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL */
      {
        /* if sporadic frame was activated for transmission in sporadic slot, its type is set to unconditional frame and processed as unconditional frame */
        /* set SDU pointer to transmit data by default for unconditional frames */
        LinPduPtr->SduPtr = (uint8*)(&(MasterChannelDataPtr->LinIf_TxData[0])); /* SBSW_LINIF_PASSED_PTR_ACCESS */
        sendFrameFlag     = LinIf_ScheduleProcessing_UnconditionalFrame(LinPduPtr, *FrameHandlePtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
      /* else: no sporadic frames marked for transmission or exists on channel, nothing further to do */
      break;
# endif

    /***********************************************************************************************************
     *  LinIf_FrameType_UnconditionalFrame
     **********************************************************************************************************/
    /* #40 Unconditional frame: Default RX and TX frame handling */
    case LinIf_FrameType_UnconditionalFrame:
      /* set SDU pointer to transmit data by default for unconditional frames */
      LinPduPtr->SduPtr = (uint8*)(&(MasterChannelDataPtr->LinIf_TxData[0])); /* SBSW_LINIF_PASSED_PTR_ACCESS */
      sendFrameFlag     = LinIf_ScheduleProcessing_UnconditionalFrame(LinPduPtr, *FrameHandlePtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      break;


    /***********************************************************************************************************
     *  LinIf_FrameType_MRF
     **********************************************************************************************************/
      /* #50 Master request frame: Forward to LinTp if supported, otherwise skip MRF transmission */
    case LinIf_FrameType_MRF:
# if ( LINIF_TP_SUPPORTED == STD_ON )
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON ) || ( LINIF_FIXEDFRAMESDULIST == STD_ON )
      sendFrameFlag = LinTp_MasterTxScheduleProcessing(TpCtrlPtr, MasterChannelDataPtr, LinPduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
#  else
      sendFrameFlag = LinTp_MasterTxScheduleProcessing(TpCtrlPtr, LinPduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
#  endif
# else
      sendFrameFlag = FALSE;
# endif
      break;

    /***********************************************************************************************************
     *  LinIf_FrameType_SRF
     **********************************************************************************************************/
    /* #60 Slave response frame: */
    case LinIf_FrameType_SRF:
      /* #70 Only send SRF header if node configuration response expected and LinTp is not in pending state (if LinTp is supported) */
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
      if ( MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd == LinIf_AtNodeConfigurationCmd_Idle )
# endif
      {
# if ( LINIF_TP_SUPPORTED == STD_ON )
        if ( (TpCtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED ) || ( TpCtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START ) ) /* COV_LINTP_SHORT_SRF_SLOT */
        {
          /* skip current SRF frame */
          sendFrameFlag = FALSE;
        }
# else
        /* Header of SlaveResp is always send. Nothing to do. */
# endif
      }
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
      else if ( MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd == LinIf_AtNodeConfigurationCmd_SkipSrf )
      {
        /* Skip transmission of SRF after not successful Node Configuration Command */
        sendFrameFlag = FALSE;
      }
      else
      {
        /* No handling required, MISRA only */
      }
# endif
      break;

    /***********************************************************************************************************
     *  LinIf_FrameType_... Node Configuration Command
     **********************************************************************************************************/
    /* #80 Node configuration frame: */
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
    case LinIf_FrameType_AssignFrameId:
    case LinIf_FrameType_UnassignFrameId:
    case LinIf_FrameType_AssignNAD:
    case LinIf_FrameType_FreeFormat:
    case LinIf_FrameType_ConditionalChangeNAD:
    case LinIf_FrameType_AssignFrameIdRange:
    case LinIf_FrameType_SaveConfiguration:

      /* #90 Abort any active TP connection if a node configuration command is going to be transmitted */
#  if ( LINIF_TP_SUPPORTED == STD_ON )
      LinTp_CloseActiveConnection(TpCtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
#  endif
      /* #100 Prepare fixed transmission data */
      /* set pointer to transmit buffer */
      LinPduPtr->SduPtr = &(MasterChannelDataPtr->LinIf_TxData[0]); /* SBSW_LINIF_PASSED_PTR_ACCESS */
      /* copy fixed data bytes to buffer */
      for (bufferCounterIdx = (uint8_least)0x00u; bufferCounterIdx < (uint8_least)LINIF_LINPDU_SIZE; bufferCounterIdx++)
      {
        LinPduPtr->SduPtr[bufferCounterIdx] = LinIf_GetSduByteOfFixedFrameSduList(LinIf_GetFixedFrameSduListIdxOfFrameList(*FrameHandlePtr) + bufferCounterIdx); /* SBSW_LINIF_LINPDU_SDUPTR */
      }
      MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_SendMrf; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

      break;
# endif

    /***********************************************************************************************************
     *  LinIf_FrameType_EventTriggeredFrame
     **********************************************************************************************************/
    /* case LinIf_FrameType_EventTriggeredFrame: */
      /* nothing to do, header is always sent */

    /* #110 Any other frame type: do nothing */
    default: /* COV_LINIF_MISRA_DEFAULTBRANCH */
      break;
  }

  return sendFrameFlag;
}

/**********************************************************************************************************************
 *  LinIf_ScheduleProcessing()
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
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ScheduleProcessing
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
)
{
# if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)                 tpCtrlPtr; /* initialized below before being used */
# endif
  LinIf_ScheduleTableHandlerRetType                                     scheduleTableHandlerRetVal; /* initialized below before being used */
  LinIf_ScheduleTableTickType                                           frameMaxDelay = 0u; /* actual frame transmission time in multiple time base ticks */
  LinIf_FrameListIdxOfEntryType                                         frameHandle = 0u;

  /* #10 LinTp: Request periodically PduR for transmission data or reception buffer (if necessary) */
# if ( LINIF_TP_SUPPORTED == STD_ON )
  tpCtrlPtr = LinTp_GetAddrCtrl(LinIfChannel);

  LinTp_BufferPolling(tpCtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# endif

  /* #20 Investigate the current time base tick for any necessary action (start new frame, on frame transmission, no scheduling) */
  scheduleTableHandlerRetVal  = LinIf_ScheduleTableHandler(&frameMaxDelay, &frameHandle, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

  /* #30 If a new schedule table slot can be started */
  /* Current slot is due or NULL schedule table is active and no wakeup frame or sleep mode frame transmission is ongoing */
  if ( ( scheduleTableHandlerRetVal != LinIf_SchdlHdlrRet_NoNewMessage ) &&
       ( MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling != LinIf_OnSleepModeFrameHandling_OnTrans ) &&
       ( MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling != LinIf_OnSleepModeFrameHandling_OnReWakeInt ) )
  {
    /* Next message will be started or NULL schedule table is active (and no sleep mode frame transmission is ongoing!) */

    /* #40 If a sleep request is pending, handle it if necessary */
    boolean sleepHandlerFlag = LinIf_ScheduleProcessing_SleepHandling(ChannelDataPtr, MasterChannelDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

    /* #50 Otherwise: */
    if ( (sleepHandlerFlag == FALSE) && (scheduleTableHandlerRetVal == LinIf_SchdlHdlrRet_NewMessage) )
    {
      /* new message shall be started, no sleep request was handled in this slot, null schedule table is not set */
      Lin_PduType linPdu; /* initialized below before being used */
      boolean     sendFrameFlag; /* initialized below before being used */

      /* #60 Process new slot: update slot information and setup LIN PDU for driver */
      ChannelDataPtr->LinIf_FrameHandle         = frameHandle; /* SBSW_LINIF_PASSED_PTR_ACCESS */
      ChannelDataPtr->LinIf_CurrentFrameType    = LinIf_GetFrameTypeOfFrameList(frameHandle); /* SBSW_LINIF_PASSED_PTR_ACCESS */

      /* set LinPdu for driver */
      linPdu.Cs         = (Lin_FrameCsModelType) LinIf_GetChecksumTypeOfFrameList(frameHandle);
      linPdu.Dl         = (Lin_FrameDlType) LinIf_GetLengthOfFrameList(frameHandle);
      linPdu.Drc        = (Lin_FrameResponseType) LinIf_GetDirectionOfFrameList(frameHandle);
      linPdu.Pid        = (Lin_FramePidType) LinIf_GetPidOfFrameList(frameHandle);
      linPdu.SduPtr     = NULL_PTR; /* set SDU pointer to a defined value, will be overwritten in case of Tx */

      /* #70 Check possible skipping of slot and request transmission data for Tx frames simultaneously */
# if ( LINIF_TP_SUPPORTED == STD_ON )
      sendFrameFlag     = LinIf_ScheduleProcessing_FrameHandling(ChannelDataPtr, MasterChannelDataPtr, tpCtrlPtr, &linPdu, &frameHandle); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# else
      sendFrameFlag     = LinIf_ScheduleProcessing_FrameHandling(ChannelDataPtr, MasterChannelDataPtr, &linPdu, &frameHandle); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# endif

      if ( sendFrameFlag == TRUE )
      {
        Std_ReturnType retVal; /* initialized below before being used */

        /* #80 If frame is handled in this slot, send header / message and setup transmission time */
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
        retVal = LinIf_GetLin_SendFrameFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel), &linPdu); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
        retVal = LinIf_Lin_SendFrame_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel), &linPdu); /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR */
# endif

        if ( ( retVal == E_OK ) &&
             ( LinIf_GetDirectionOfFrameList(frameHandle) != LIN_FRAMERESPONSE_IGNORE ) )
        {
          /* transmission successful, S2S frames are the only type which do not require further handling */
          /* Store maximum frame transmission time within current slot for rx and tx frame */
          MasterChannelDataPtr->LinIf_FrameMaxDelay   = frameMaxDelay; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
        }
        /* else: Transmission of frame not successful, skip further processing, no error must reported according to spec */
      }
    } /* new message handled */
  }
}

/**********************************************************************************************************************
 *
 * END - LinIf Schedule Processing
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * LOCAL FUNCTIONS - LinIf Message Processing
 *
 * Following functions handle transmitted / received messages of the current slot. After the maximum transmission time,
 * the LIN driver is requested for the status of the last message. In case of successful reception, the received data
 * is indicated to the appropriate module depending on the frame type. A successful transmission is confirmed. If an
 * error is reported by the driver, appropriate error handling is performed.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * Functions for handling transmitted / received messages of current slot.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_MessageProcessing_Unconditional()
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
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_Unconditional
(
  CONSTP2CONST(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)      ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
# if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
# endif
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16_least                                                          indexTmp = 0u;
  uint8                                                                 errorId = LINIF_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If unconditional frame is Tx */
  if ( LinIf_GetDirectionOfFrameList(ChannelDataPtr->LinIf_FrameHandle) == LIN_FRAMERESPONSE_TX )
  {
    /* #20 If transmission was successful */
    if ( LinStatus == (Lin_StatusType)LIN_TX_OK )
    {
# if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
      LinIf_NodeChannelConfigIdxOfChannelConfigType masterChannelCfgIdx;
# endif

      /* #30 Inform upper layer about transmitted frame */
      MasterChannelDataPtr->LinIf_PduRNotificationFlag     = LinIf_PduRNotificationTx; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      MasterChannelDataPtr->LinIf_PduRNotificationPduId    = LinIf_GetPduIdOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      MasterChannelDataPtr->LinIf_PduRNotificationFctIndex = (uint8)LinIf_GetIndConfFctIdxOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif

      /* #40 Search over all unconditional frames which are used in sporadic slots on current channel */
# if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
      /* clear flag of unconditional frame if sporadic slot was occupied or unconditional frame was transmitted regularly in schedule table */
      /* search all unconditional frames assigned to sporadic slots on this channel in list sorted by frame handle which was saved before transmission */
      masterChannelCfgIdx = LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel);
      while (indexTmp < (uint16)LinIf_GetNumberOfSporadicUncFramesOfMasterChannelConfig(masterChannelCfgIdx)) /* COV_LINIF_SPOR_FRAME_HANDLE_CONFIG */ /* FETA_LINIF_UPPER_LIMIT_IS_GENERATED_CONSTANT */
      {
        /* get the index of the sporadic unconditional frame by LinIf_SporUncFrameByHandleList -> LinIf_SporUnconditionalFrame */
        LinIf_SporUnconditionalFrameIdxOfSporUncFrameByHandleListType idxToSporUncFrame =
          LinIf_GetSporUnconditionalFrameIdxOfSporUncFrameByHandleList(LinIf_GetSporUncFrameByHandleListStartIdxOfMasterChannelConfig(masterChannelCfgIdx) + indexTmp);
        if ( ChannelDataPtr->LinIf_FrameHandle <= LinIf_GetFrameListIdxOfSporUnconditionalFrame(idxToSporUncFrame))
        {
          /* #50 If current transmitted unconditional frame matches one in a sporadic slot, clear transmission flags of this frame */
          if ( ChannelDataPtr->LinIf_FrameHandle == LinIf_GetFrameListIdxOfSporUnconditionalFrame(idxToSporUncFrame))
          {
            uint8 sporUncFrmByteMsk = LinIf_GetByteMskOfSporUnconditionalFrame(idxToSporUncFrame);
            uint8 sporUncFrmBitMsk  = LinIf_GetBitMskOfSporUnconditionalFrame(idxToSporUncFrame);
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
            if ( sporUncFrmByteMsk >= LinIf_GetSizeOfSporTransmitFlags() ) /* defensive programming, condition will never be true in a valid configuration */ /* COV_LINIF_INV_STATE_ALWAYSFALSE */
            {
#   if ( LINIF_DEV_ERROR_REPORT == STD_ON )
             (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, LINIF_E_CONFIG);
#   endif
            }
            else
#  endif
            {
              uint8 sporTxFlags = (uint8)(LinIf_GetSporTransmitFlags(sporUncFrmByteMsk) & (uint8)(~sporUncFrmBitMsk));
              LinIf_SetSporTransmitFlags(sporUncFrmByteMsk, sporTxFlags); /* SBSW_LINIF_SPOR_TXFLAGS_ACCESS */
            }
          }
          /* frame was transmitted in sporadic slot (flag cleared afterwards) or frame handle not transmitted at all in this slot */
          /* in each case abort the loop as the search is finished */
          break;
        }
        indexTmp++;
      }
# endif
    }
    /* #60 Report Det error if frame was not successfully transmitted */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      if ( ( LinStatus == (Lin_StatusType)LIN_TX_BUSY ) || ( LinStatus == (Lin_StatusType)LIN_TX_ERROR ) )
      { /* Inform about error in transmitted frame */
        errorId = LINIF_E_RESPONSE;
      }
    }
# endif

# if ( LINIF_BUSMIRRORING == STD_ON )
    /* #65 If bus mirroring is enabled, store frame status information and transmission data to be reported to mirror module */
    if ( ChannelDataPtr->LinIf_MirroringActivationState == TRUE )
    {
      /* store bus mirroring event data for later reporting and set pending flag */
      MasterChannelDataPtr->LinIf_MirroringEventPending    = TRUE; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      MasterChannelDataPtr->LinIf_MirroringPid             = LinIf_GetPidOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      MasterChannelDataPtr->LinIf_MirroringLinStatus       = LinStatus; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

      if ( LinStatus == (Lin_StatusType)LIN_TX_OK )
      {
        /* if transmission was successful, store transmitted data in mirror buffer before it could be overwritten in schedule processing later in this cycle */
        for (indexTmp = 0u; indexTmp < (uint16)LinIf_GetLengthOfFrameList(ChannelDataPtr->LinIf_FrameHandle); indexTmp++) /* FETA_LINIF_UPPER_LIMIT_LENGTHOF */
        {
          MasterChannelDataPtr->LinIf_RxMirrorData[indexTmp & LINIF_LINPDU_SIZE_MASK] = MasterChannelDataPtr->LinIf_TxData[indexTmp & LINIF_LINPDU_SIZE_MASK]; /* SBSW_LINIF_RX_BUFFER_ARRAY */
        }
        MasterChannelDataPtr->LinIf_PduRNotificationPduLength = LinIf_GetLengthOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      }
    }
# endif

  }
  /* #70 otherwise if unconditional frame is Rx */
  else if ( LinIf_GetDirectionOfFrameList(ChannelDataPtr->LinIf_FrameHandle) == LIN_FRAMERESPONSE_RX ) /* COV_LINIF_MISRA_ELSEBRANCH */
  {
    /* #80 If reception was successful */
    if ( LinStatus == (Lin_StatusType)LIN_RX_OK )
    {
      /* #90 Inform PduR about received frame and data */
      MasterChannelDataPtr->LinIf_PduRNotificationFlag        = LinIf_PduRNotificationRx; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      MasterChannelDataPtr->LinIf_PduRNotificationPduId       = LinIf_GetPduIdOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      MasterChannelDataPtr->LinIf_PduRNotificationFctIndex    = (uint8)LinIf_GetIndConfFctIdxOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif
      /* save length because FrameInfoPtr may be overwritten in LinIf_ScheduleProcessing() before PduR confirmation */
      MasterChannelDataPtr->LinIf_PduRNotificationPduLength   = LinIf_GetLengthOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

      for (indexTmp = 0u; indexTmp < LinIf_GetLengthOfFrameList(ChannelDataPtr->LinIf_FrameHandle); indexTmp++) /* FETA_LINIF_UPPER_LIMIT_LENGTHOF */
      { /* copy Lin receive data to LinIf buffer for data consistency */
        MasterChannelDataPtr->LinIf_RxMirrorData[indexTmp & LINIF_LINPDU_SIZE_MASK] = LinSduPtr[indexTmp & LINIF_LINPDU_SIZE_MASK]; /* SBSW_LINIF_RX_BUFFER_ARRAY */
      }
    }
    /* #100 Report Det error if frame was not successfully received */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    else
    {
      if ( ( LinStatus == (Lin_StatusType)LIN_RX_BUSY ) ||
           ( LinStatus == (Lin_StatusType)LIN_RX_ERROR ) ||
           ( LinStatus == (Lin_StatusType)LIN_RX_NO_RESPONSE ) )
      {
        /* Inform about error in received frame */
        errorId = LINIF_E_RESPONSE;
      }
    }
# endif

# if ( LINIF_BUSMIRRORING == STD_ON )
    /* #105 If bus mirroring is enabled, store frame and status information to be reported to mirror module */
    if ( ChannelDataPtr->LinIf_MirroringActivationState == TRUE )
    {
      /* store bus mirroring event data for later reporting and set pending flag */
      MasterChannelDataPtr->LinIf_MirroringEventPending    = TRUE; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      MasterChannelDataPtr->LinIf_MirroringPid             = LinIf_GetPidOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      MasterChannelDataPtr->LinIf_MirroringLinStatus       = LinStatus; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    }
# endif

  }
  /* #110 otherwise on S2S frame: do nothing */
  else
  {
    /* On S2S Frame - never reach this point as the response part of a S2S frame is not handled at all */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportRuntimeError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}


# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_MessageProcessing_EventTriggeredFrame()
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
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_EventTriggeredFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If EVT frame is successfully received without collision, handle frame as any unconditional frame */
  if ( LinStatus == (Lin_StatusType)LIN_RX_OK )
  {
    /* evaluate first byte of frame for received PID to assign appropriate unconditional frame */
    uint8 pidReceived = LinSduPtr[0];

    /* store index into event-triggered frame info list as the current frame handle gets overwritten below */
    LinIf_FixedFrameSduListIdxOfFrameListType evtFrameInfoIdx = LinIf_GetFixedFrameSduListIdxOfFrameList( ChannelDataPtr->LinIf_FrameHandle );
    /* get the start index of the assigned unconditional frame of current EVT slot */
    LinIf_EvtUnconditionalFrameInfoStartIdxOfEvtFrameInfoType indexTmp = LinIf_GetEvtUnconditionalFrameInfoStartIdxOfEvtFrameInfo( evtFrameInfoIdx );

    /* #20 Iterate over each unconditional frame assigned to the EVT slot and search for a match with the received frame indicated by the PID */
    while ( indexTmp < (uint16)LinIf_GetEvtUnconditionalFrameInfoEndIdxOfEvtFrameInfo( evtFrameInfoIdx ) ) /* FETA_LINIF_UPPER_LIMIT_ENDINDEX */
    {
      if ( pidReceived == LinIf_GetPidOfEvtUnconditionalFrameInfo(indexTmp) )
      {
        /* PID matches - convert current EVT frame information to unconditional frame info */
        /* replace current frame information with flagged unconditional frame */
        ChannelDataPtr->LinIf_FrameHandle                       = LinIf_GetFrameListIdxOfEvtUnconditionalFrameInfo(indexTmp); /* SBSW_LINIF_PASSED_PTR_ACCESS */
        /* quit search for matching unconditional frame */
        break;
      }
      indexTmp++;
    }

    /* #30 If corresponding unconditional frame is found, inform PduR about received frame and data */
    if ( indexTmp < LinIf_GetEvtUnconditionalFrameInfoEndIdxOfEvtFrameInfo( evtFrameInfoIdx ) )
    {
      MasterChannelDataPtr->LinIf_PduRNotificationFlag          = LinIf_PduRNotificationRx; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      MasterChannelDataPtr->LinIf_PduRNotificationPduId         = LinIf_GetPduIdOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
#  if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      MasterChannelDataPtr->LinIf_PduRNotificationFctIndex      = (uint8)LinIf_GetIndConfFctIdxOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
#  endif
      /* save length because FrameInfoPtr may be overwritten in LinIf_ScheduleProcessing() before PduR confirmation */
      MasterChannelDataPtr->LinIf_PduRNotificationPduLength     = LinIf_GetLengthOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

      for (indexTmp = 0u; indexTmp < (uint16)LinIf_GetLengthOfFrameList(ChannelDataPtr->LinIf_FrameHandle); indexTmp++) /* FETA_LINIF_UPPER_LIMIT_LENGTHOF */
      { /* copy Lin receive data to LinIf buffer for data consistency */
        MasterChannelDataPtr->LinIf_RxMirrorData[indexTmp]      = LinSduPtr[indexTmp]; /* SBSW_LINIF_RX_BUFFER_ARRAY */
      }
    }
  }
  /* #40 Otherwise: */
  else
  {
    /* #50 If error occurred, treat as collision and set collision resolving schedule table as pending */
    if ( ( LinStatus == (Lin_StatusType)LIN_RX_BUSY ) || ( LinStatus == (Lin_StatusType)LIN_RX_ERROR ) )
    {
      /* collision on EVT frame detected */
      P2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT) schedTabDataPtr = &(MasterChannelDataPtr->LinIf_SchedTabData);

      if ( schedTabDataPtr->LinIf_RequestedTable == LinIf_Schedule_Invalid )
      {
        /* collision resolving is only started, if no schedule table change has been requested */
        /* if a sleep mode transition was requested, collision resolving is discarded - this case will be handled in ScheduleProcessing() */
        schedTabDataPtr->LinIf_RequestedTable = LinIf_GetCollResolvSchedHandleOfEvtFrameInfo( LinIf_GetFixedFrameSduListIdxOfFrameList( ChannelDataPtr->LinIf_FrameHandle) ); /* SBSW_LINIF_PASSED_PTR_ACCESS */
        schedTabDataPtr->LinIf_EvtCollResolvingState = LinIf_EvtCollResolvingState_Detected; /* SBSW_LINIF_PASSED_PTR_ACCESS */
      }
      /* else: schedule table change was requested before, so collision resolving is lost */
    }
    /* #60 If no response received, no slave has responded, nothing to do */
    else
    {
      if ( LinStatus == (Lin_StatusType)LIN_RX_NO_RESPONSE )
      {
        /* silent EVT frame - no further action required */
      }
      /* else: ignore all other reportings */
    }
  }

#  if ( LINIF_BUSMIRRORING == STD_ON )
  /* #70 If bus mirroring is enabled, store frame and status information to be reported to mirror module */
  if ( ChannelDataPtr->LinIf_MirroringActivationState == TRUE )
  {
    /* store bus mirroring event data for later reporting and set pending flag */
    /* in case the frame was correctly received, the data has already been copied above */
    MasterChannelDataPtr->LinIf_MirroringEventPending    = TRUE; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    MasterChannelDataPtr->LinIf_MirroringPid             = LinIf_GetPidOfFrameList(ChannelDataPtr->LinIf_FrameHandle); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    MasterChannelDataPtr->LinIf_MirroringLinStatus       = LinStatus; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
  }
#  endif
}
# endif


/**********************************************************************************************************************
 *  LinIf_MessageProcessing_SleepModeFrame()
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
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_SleepModeFrame
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If sleep mode frame state is still on transmission */
  if ( MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnTrans )
  {
    /* The LIN driver transits to SLEEP state in every case, even if the transmission of the sleep mode frame was erroneous */
    /* #20 Enter SLEEP state and inform LinSM with result value depending on LIN driver status */
    MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    LinIf_FlushAllSchedules(&(MasterChannelDataPtr->LinIf_SchedTabData)); /* SBSW_LINIF_FLUSHALLSCHED_CALL_1 */
    ChannelDataPtr->LinIf_ChannelState = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_PASSED_PTR_ACCESS */

    if ( LinStatus == (Lin_StatusType)LIN_CH_SLEEP )
    {
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                          LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
# else
      LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
# endif
    }
    else
    {
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                      LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), FALSE);
# else
      LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), FALSE);
# endif
    }
  }
  /* #30 Otherwise if an internal sleep mode transition is active */
  else if ( MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnTransInt )
  {
    /* #40 Enter SLEEP state and inform LinSM about successful sleep transition */
    MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    LinIf_FlushAllSchedules(&(MasterChannelDataPtr->LinIf_SchedTabData)); /* SBSW_LINIF_FLUSHALLSCHED_CALL_1 */
    ChannelDataPtr->LinIf_ChannelState = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_PASSED_PTR_ACCESS */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
    LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                        LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
# else
    LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
# endif
  }
  /* #50 Otherwise if a wakeup request occurred during sleep mode frame transmission */
  else if ( MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReWakeInt ) /* COV_LINIF_MISRA_ELSEBRANCH */
  {
    /* #60 Remain in operational state and send wakeup frame */
    LinIf_ProcessWakeupTransition(ChannelDataPtr, MasterChannelDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
  }
  else
  {
    /* MISRA only, cannot happen */
  }
}

/**********************************************************************************************************************
 *  LinIf_MessageProcessing_SRF()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_MessageProcessing_SRF
(
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
# endif
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                             LinSduPtr, /* PRQA S 3673 */ /* MD_LINIF_Rule8.13_3673_3678 */
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                                        LinStatus
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                                 errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(LinSduPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(LinIfChannel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(LinStatus); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If no node configuration response is expected */
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  if ( MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd == LinIf_AtNodeConfigurationCmd_Idle )
# endif
  {
    /* #20 Forward received SRF to LinTp in case no bus error occurred, otherwise report error */
# if ( LINIF_TP_SUPPORTED == STD_ON )
    if ( LinStatus == (Lin_StatusType)LIN_RX_OK )
    {
      /* LinTp SRF indicated check current state */
      LinTp_MasterRxIndication_Main(LinIfChannel, LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    else if ( LinStatus == (Lin_StatusType)LIN_RX_NO_RESPONSE )
    {
      /* No response must be handled different for SlaveResp message to error in response. */
    }
    else if ( ( LinStatus == (Lin_StatusType)LIN_RX_BUSY ) || ( LinStatus == (Lin_StatusType)LIN_RX_ERROR ) )
    {
      /* Error in node configuration slave response */
      errorId = LINIF_E_RESPONSE;
    }
    else
    {
      /* No handling required, MISRA only */
    }
#  endif
# endif
  }
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  else if ( MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd == LinIf_AtNodeConfigurationCmd_SendSrf ) /* COV_LINIF_NODECONFIG_ELSEBRANCH */
  {
    /* #30 Otherwise (node configuration response expected): Ignore SRF if no bus error occurred, otherwise report error */
    /* This is a node configuration command answer, which must be processed within the LinIf */

    MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( LinStatus == (Lin_StatusType)LIN_RX_NO_RESPONSE )
    {
      errorId = LINIF_E_NC_NO_RESPONSE;
    }
    else if ( ( LinStatus == (Lin_StatusType)LIN_RX_BUSY ) || ( LinStatus == (Lin_StatusType)LIN_RX_ERROR ) )
    {
      /* Error in node configuration slave response */
      errorId = LINIF_E_RESPONSE;
    }
    /* else if ( LinStatus == (Lin_StatusType)LIN_RX_OK ) : nothing to do */
    else
    {
      /* No handling required, MISRA only */
    }
#  endif
  }
  else
  {
    /* Never happens, MISRA only */
  }
# endif

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportRuntimeError( LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId );
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *  LinIf_CurrentMessageProcessing()
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
 */
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_CurrentMessageProcessing
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                                 errorId = LINIF_E_NO_ERROR;
  uint8                                                                 defaultData[8] = { 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu };
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR)                               linSduPtr = &(defaultData[0]);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If maximum frame duration has expired */
  if ( MasterChannelDataPtr->LinIf_FrameMaxDelay > (LinIf_ScheduleTableTickType)0x00u )
  {
    MasterChannelDataPtr->LinIf_FrameMaxDelay--; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    if ( MasterChannelDataPtr->LinIf_FrameMaxDelay == (LinIf_ScheduleTableTickType)0x00u )
    {
# if ( LINIF_TP_SUPPORTED == STD_ON )
      P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT) tpCtrlPtr; /* initialized below before being used */
# endif
      Lin_StatusType linStatusVal; /* initialized below before being used */

      /* #20 Frame delay expired, check LIN driver for status of last frame */
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
      linStatusVal = LinIf_GetLin_GetStatusFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel), &linSduPtr); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
      linStatusVal = LinIf_Lin_GetStatus_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel), &linSduPtr); /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR */
# endif

      /* #21 Inform application about status of last frame if enabled, excluding sleep and wakeup frames */
# if ( LINIF_APPL_FRAME_STATUS_INFO == STD_ON )
      if ( !((ChannelDataPtr->LinIf_CurrentFrameType == LinIf_FrameType_SleepModeFrame) || (ChannelDataPtr->LinIf_CurrentFrameType == LinIf_FrameType_WakeUpFrame)) )
      {
        Appl_LinIfGetLinStatus(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), LinIf_GetPidOfFrameList(ChannelDataPtr->LinIf_FrameHandle), linStatusVal);
      }
# endif

# if ( LINIF_TP_SUPPORTED == STD_ON )
      tpCtrlPtr = LinTp_GetAddrCtrl(LinIfChannel);
# endif

      /* #30 Further handling depends on frame type: */
      switch ( ChannelDataPtr->LinIf_CurrentFrameType )
      {
        /************************************************************************************************************
         *  LinIf_FrameType_MRF
         ***********************************************************************************************************/
        /* #40 On MRF: LinTp is the only user, so a MRF is never scheduled if LinTp is not activated */
# if ( LINIF_TP_SUPPORTED == STD_ON )
        case LinIf_FrameType_MRF:
          /* #50 Forward transmitted MRF to LinTp if supported */
          LinTp_MasterTxMessageProcessing(tpCtrlPtr, LinIfChannel, linStatusVal); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

          /* #60 Report DET error on transmission failure */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
          if ( ( linStatusVal == (Lin_StatusType)LIN_TX_BUSY ) || ( linStatusVal == (Lin_StatusType)LIN_TX_ERROR ) )
          {
            /* Inform DET about error in transmitted frame */
            errorId = LINIF_E_RESPONSE;
          }
#  endif
          break;
# endif

        /************************************************************************************************************
         *  LinIf_FrameType_UnconditionalFrame
         ***********************************************************************************************************/
        /* #70 On unconditional frame: perform handling */
        case LinIf_FrameType_UnconditionalFrame:
# if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
          LinIf_MessageProcessing_Unconditional(ChannelDataPtr, MasterChannelDataPtr, LinIfChannel, linSduPtr, linStatusVal); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# else
          LinIf_MessageProcessing_Unconditional(ChannelDataPtr, MasterChannelDataPtr, linSduPtr, linStatusVal); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# endif
          break;

        /************************************************************************************************************
         *  LinIf_FrameType_SRF
         ***********************************************************************************************************/
        /* #80 On SRF: perform handling */
        case LinIf_FrameType_SRF:
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
          LinIf_MessageProcessing_SRF(MasterChannelDataPtr, linSduPtr, LinIfChannel, linStatusVal); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# else
          LinIf_MessageProcessing_SRF(linSduPtr, LinIfChannel, linStatusVal); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# endif
          break;

# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
        /************************************************************************************************************
         *  LinIf_FrameType_EventTriggeredFrame
         ***********************************************************************************************************/
        /* #90 On EVT frame: perform performed */
        case LinIf_FrameType_EventTriggeredFrame:
          LinIf_MessageProcessing_EventTriggeredFrame(ChannelDataPtr, MasterChannelDataPtr, linSduPtr, linStatusVal); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
          break;
# endif

# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
        /************************************************************************************************************
         *  LinIf_FrameType_... Node Configuration Command
         ***********************************************************************************************************/
        /* #100 On node configuration command: on successful transmission, send succeeding SRF header, otherwise skip it */
        case LinIf_FrameType_AssignFrameId:
        case LinIf_FrameType_UnassignFrameId:
        case LinIf_FrameType_AssignNAD:
        case LinIf_FrameType_FreeFormat:
        case LinIf_FrameType_ConditionalChangeNAD:
        case LinIf_FrameType_AssignFrameIdRange:
        case LinIf_FrameType_SaveConfiguration:
          /* These are schedule table commands and always tx messages, which must be processed in the LinIf */
          if ( linStatusVal == (Lin_StatusType)LIN_TX_OK )
          {
            /* Store that the frame has been send and the following SRF has to be handled without the LinTp */
            MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_SendSrf; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
          }
          else
          {
            MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_SkipSrf; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
          }
          break;
# endif

        /************************************************************************************************************
         *  LinIf_FrameType_SleepModeFrame
         ***********************************************************************************************************/
        /* #110 On sleep mode frame: perform handling */
        case LinIf_FrameType_SleepModeFrame:
          /* Sleep Mode Frame has been send */
          LinIf_MessageProcessing_SleepModeFrame(ChannelDataPtr, MasterChannelDataPtr, LinIfChannel, linStatusVal); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
          break;

        /************************************************************************************************************
         *  LinIf_FrameType_WakeUpFrame
         ***********************************************************************************************************/
        /* #120 On wakeup frame: inform upper layer about wakeup, only if no sleep request occurred during wakeup frame transmission */
        case LinIf_FrameType_WakeUpFrame:
          /* Wake Up Frame has been send */
          if ( MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnWakeupFrameHandling_OnTrans )
          {
            /* Only Process the wakeup frame transmission if no other request (i.e. Sleep Mode Frame) is pending */
            MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling  = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
            LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                    LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
# else
            LinSM_WakeupConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
# endif
          }
          break;

        /* #130 On any other frame type, nothing to do, especially not for sporadic frame as they are handled as unconditional frames */
        /************************************************************************************************************
         *  case LinIf_FrameType_SporadicFrame:
         *
         *  Case not required here as a sporadic slot is empty or replaced by an unconditional frame
         *
         *  break;
         *
         *
         ***********************************************************************************************************/
        /************************************************************************************************************
         *  default
         ***********************************************************************************************************/
        default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_LINIF_MISRA_DEFAULTBRANCH */
          break;

      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_TP_SUPPORTED == STD_ON ) /* currently only Tp could report an error in this function */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportRuntimeError( LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId );
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *
 * END - LinIf Message Processing
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinIf Master NetWork Managemant - Local Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_ProcessWakeupTransition()
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
 */
LINIF_LOCAL FUNC(void, LINIF_CODE) LinIf_ProcessWakeupTransition
(
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr,
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set channel to OPERATIONAL state */
  ChannelDataPtr->LinIf_ChannelState               = LINIF_CHANNEL_OPERATIONAL; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */

  /* #20 If the external wakeup flag is not set */
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  if (ChannelDataPtr->LinIf_WakeupFlag == (uint8)0x00u)
# endif
  {
    /* no external wakeup was indicated previously */
    /* #30 Request wakeup frame transmission from LIN driver */
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
    (void) LinIf_GetLin_WakeupFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
    (void) LinIf_Lin_Wakeup_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel));
# endif

    /* #40 Update status machine depending on Lin_GetStatus() call behavior */

    /* If Lin_GetStatus() shall be called after the wakeup frame has been transmitted */
    if ( LinIf_GetStatusCallAfterWakeupTransmissionOfMasterChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel)) != 0u )
    {
      /* Update state that wakeup frame is ongoing, but has not yet been checked */
      MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnWakeupFrameHandling_OnTrans; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      ChannelDataPtr->LinIf_CurrentFrameType               = LinIf_FrameType_WakeUpFrame; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */

      /* Set duration to check status of LIN Driver after wakeup frame transmission on main function level */
      MasterChannelDataPtr->LinIf_FrameMaxDelay            = (LinIf_ScheduleTableTickType)LinIf_GetStatusCallDelayAfterWakeupTxInTicksOfMasterChannelConfig( /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
                                                                                          LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel));
    }
    /* Otherwise confirm wakeup to upper layer directly and do not call Lin_GetStatus() afterwards */
    else
    {
      /* clear possible pending internal sleep request */
      MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
      MasterChannelDataPtr->LinIf_FrameMaxDelay            = 0u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                  LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
# else
      LinSM_WakeupConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
# endif
    }

    /* #50 Start internal wakeup delay */
    ChannelDataPtr->LinIf_WakeupDelayTimer         = LinIf_GetWakeupDelayInternalOfChannelConfig(LinIfChannel); /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
  }
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  /* #60 Otherwise if the external wakeup flag is set */
  else
  {
    /* external wakeup already reported by driver or transceiver, suppress active wakeup frame transmission */

    /* #70 Set the LIN driver to operational state and directly confirm wakeup to upper layer */
#  if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
    (void) LinIf_GetLin_WakeupInternalFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
#  else
    (void) LinIf_Lin_WakeupInternal_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel));
#  endif

#  if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
    LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(LinIfChannel))( /* SBSW_LINIF_FCT_POINTER_CALL */
                                LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
#  else
    LinSM_WakeupConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), TRUE);
#  endif

    /* clear possible pending internal sleep request */
    MasterChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

    /* #80 Start external wakeup delay */
    ChannelDataPtr->LinIf_WakeupDelayTimer               = LinIf_GetWakeupDelayExternalOfChannelConfig(LinIfChannel); /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
  }
# endif
}

/**********************************************************************************************************************
 *
 * END - LinIf Master NetWork Managemant - Local Functions
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * LinIf Initialization, Task and General Functions for master nodes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_MasterChannelMainFunction()
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
 */
FUNC(void, LINIF_CODE) LinIf_MasterChannelMainFunction
(
  VAR(NetworkHandleType, AUTOMATIC)                               LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)       channelDataPtr; /* initialized below before being used */
  P2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) masterChannelDataPtr; /* initialized below before being used */
# if ( LINIF_TP_SUPPORTED == STD_ON )
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)           tpCtrlPtr; /* initialized below before being used */
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* get pointer to general and master-specific channel data */
  channelDataPtr = LinIf_GetAddrChannelData(LinIfChannel);
  masterChannelDataPtr = LinIf_GetAddrMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel));
# if ( LINIF_TP_SUPPORTED == STD_ON )
  tpCtrlPtr = LinTp_GetAddrCtrl(LinIfChannel);
# endif

  /* #10 If channel state is OPERATIONAL or an internal sleep mode transition is pending */
  if ( ( channelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_OPERATIONAL ) ||
       ( ( channelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_SLEEP ) &&
         ( ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnTransInt ) ||
           ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReWakeInt ) ) ) )
  {
    /* #20 Call main message processing function */
    /**********************************************************
     * Processing of last message
     **********************************************************/
    LinIf_CurrentMessageProcessing(channelDataPtr, masterChannelDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

    /* #30 Update Tp timer and abort connection on timeout */
    /**********************************************************
     * Tp timeout observation
     **********************************************************/
# if ( LINIF_TP_SUPPORTED == STD_ON )
    /* Lin Tp timeout handling */
    if (tpCtrlPtr->LinTp_Timer > 0u)
    {
      tpCtrlPtr->LinTp_Timer--; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
      if (tpCtrlPtr->LinTp_Timer == 0u)
      {
        /* timeout on running transmission / reception occurred check current state */
        LinTp_CloseActiveConnection(tpCtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
    }
# endif

    /* #40 If channel is still in operation state */
    if ( channelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_OPERATIONAL )
    {
      /**********************************************************
       * Processing of next time slot
       **********************************************************/
      /* #50 If wakeup delay timer is active, update it, otherwise call main schedule table processing function */
      if ( channelDataPtr->LinIf_WakeupDelayTimer == (uint8)0x00u )
      {
        /* Normal Message processing */
        LinIf_ScheduleProcessing(channelDataPtr, masterChannelDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
      else
      {
        /* Wait for end of wakeup delay */
        channelDataPtr->LinIf_WakeupDelayTimer--; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
      }
    }
    /* else: Skip further processing if sleep mode has been entered */
  }
  /* #60 Otherwise if channel state is sleep and an internal goto sleep request is pending */
  else if ( ( channelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_SLEEP ) &&
            ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReqInt ) )
  {
    /* #70 Process one tick of main schedule table processing function for internal sleep transition */
    LinIf_ScheduleProcessing(channelDataPtr, masterChannelDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
  } /* sleep state */
  else
  {
    /* MISRA only */
  }
}

# if ( LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING == STD_ON )
/**********************************************************************************************************************
 *  LinIf_MainFunctionPostProcessingExternal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, LINIF_CODE) LinIf_MainFunctionPostProcessingExternal
(
  VAR(NetworkHandleType, AUTOMATIC)                               Channel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                           errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
#   if ( LINIF_TP_SUPPORTED == STD_ON )
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
#   else
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
#   endif
  {
    errorId = LINIF_E_UNINIT;
  }
   /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetSizeOfSystemToLinIfChannel() )
  {
    errorId = LINIF_E_NONEXISTENT_CHANNEL;
  }
  else
#  endif
  {
    /* #30 Check if channel is active */
    NetworkHandleType linIfChannelIdx = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);
    if ( linIfChannelIdx < LinIf_GetSizeOfChannelData() )
    {
      /* #40 Call channel post processing main function */
      LinIf_ChannelMainFunctionPostProcessing(linIfChannelIdx);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
}
# endif

/**********************************************************************************************************************
 *  LinIf_ChannelMainFunctionPostProcessing()
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
 */
FUNC(void, LINIF_CODE) LinIf_ChannelMainFunctionPostProcessing
(
  VAR(NetworkHandleType, AUTOMATIC)                               LinIfChannel
)
{
  /* ----- Implementation ----------------------------------------------- */
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  if ( LinIf_GetNodeTypeOfChannelConfig(LinIfChannel) == LinIf_ChannelNodeType_Master )
# endif
  {
    P2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) masterChannelDataPtr = LinIf_GetAddrMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel));

    /**********************************************************
    * LinTp diagnostic schedule change notification
    **********************************************************/
    /* #10 Inform BswM module about diagnostic schedule change notification (if supported) */
# if ( LINIF_TP_SUPPORTED == STD_ON )
    P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT) tpCtrl = LinTp_GetAddrCtrl(LinIfChannel);

    if (tpCtrl->LinTp_PortNotification != LinTp_PortNotificationIdle)
    {
      LinTp_ChannelType linTpChannelIdx = LinTp_GetChannelOfLinIfToLinTpChannel(LinIfChannel);
      /* no check for linTpChannelIdx required because port notification flag cannot be set on an inactive tp channel */
      if (LinTp_IsSchedChangeNotifyOfChannelConfig(linTpChannelIdx)) /* COV_LINTP_SCHEDCHANGENOTIFY */
      {
        /* schedule change notification enabled */
        BswM_LinTp_RequestMode(LinIf_GetSystemChannelIndexOfChannelIndTable(LinIfChannel), (LinTp_Mode)(tpCtrl->LinTp_PortNotification));
      }
      /* clear notification */
      tpCtrl->LinTp_PortNotification = LinTp_PortNotificationIdle; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
    }
# endif /* LINIF_TP_SUPPORTED */

    /**********************************************************
    / LinIf post notification to the PduR of last transmitted/ received frame
    **********************************************************/
    /* #20 Inform PduR about transmitted non-diagnostic frame (with upper layer PduId) */
    if ( masterChannelDataPtr->LinIf_PduRNotificationFlag == LinIf_PduRNotificationTx )
    {
      /* report transmitted frame to PduR with enabled interrupts */
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetTxConfFctList(masterChannelDataPtr->LinIf_PduRNotificationFctIndex)(masterChannelDataPtr->LinIf_PduRNotificationPduId); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
      PduR_LinIfTxConfirmation(masterChannelDataPtr->LinIf_PduRNotificationPduId);
# endif
    }
    /* #30 Inform PduR about received frame with reception data */
    else if ( masterChannelDataPtr->LinIf_PduRNotificationFlag == LinIf_PduRNotificationRx )
    {
      PduInfoType pduInfo;

      /* report received frame to PduR with enabled interrupts */
      pduInfo.SduLength = masterChannelDataPtr->LinIf_PduRNotificationPduLength;
      pduInfo.SduDataPtr = &(masterChannelDataPtr->LinIf_RxMirrorData[0]);
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetRxIndFctList(masterChannelDataPtr->LinIf_PduRNotificationFctIndex)(masterChannelDataPtr->LinIf_PduRNotificationPduId, &pduInfo); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
      PduR_LinIfRxIndication(masterChannelDataPtr->LinIf_PduRNotificationPduId, &pduInfo); /* SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR */
# endif
    }
    else
    {
      /* nothing to be done */
    }
    /* clear pending notification to PduR in any case */
    masterChannelDataPtr->LinIf_PduRNotificationFlag = LinIf_PduRNotificationIdle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

    /**********************************************************
    * Bus mirroring notification
    **********************************************************/
# if ( LINIF_BUSMIRRORING == STD_ON )
    /* #40 Inform mirror module about frame reception / transmission (if bus mirroring is enabled) */
    LinIf_ChannelMainFunctionPostProcessing_Mirror(masterChannelDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# endif

    /**********************************************************
    * LinSM notification and confirmations
    **********************************************************/
    /* #50 Inform LinSM or CDD about schedule table handling */
    LinIf_ChannelMainFunctionPostProcessing_LinSM(masterChannelDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

# if ( LINIF_TP_SUPPORTED == STD_ON )
    /**********************************************************
    / LinTp post notification to the PduR of last transmitted/ received TP message
    **********************************************************/
    /* #60 Inform PduR about transmitted diagnostic frame */
    if ( (tpCtrl->LinTp_PduRNotification & LinTp_PduRNotificationTx) != LinTp_PduRNotificationIdle )
    {
      /* report transmitted frame to PduR with enabled interrupts */
      PduR_LinTpTxConfirmation(tpCtrl->LinTp_PduRNSduIdTx, tpCtrl->LinTp_PduRNotifyResultTx);
    }

    if ( (tpCtrl->LinTp_PduRNotification & LinTp_PduRNotificationRx) != LinTp_PduRNotificationIdle )
    {
      /* report received frame to PduR with enabled interrupts */
      PduR_LinTpRxIndication(tpCtrl->LinTp_PduRNSduIdRx, tpCtrl->LinTp_PduRNotifyResultRx);
    }

#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    /**********************************************************
    / LinTp post notification to the PduR of last requested TP message of LinTp_Transmit
    **********************************************************/
    /* #70 Inform PduR about received diagnostic frame */
    if ( (tpCtrl->LinTp_PduRNotification & LinTp_PduRNotificationTxTm) != LinTp_PduRNotificationIdle )
    {
      /* report transmitted frame to PduR with enabled interrupts */
      PduR_LinTpTxConfirmation(tpCtrl->LinTp_PduRNSduIdTm, E_NOT_OK);
    }
#  endif

    /* clear pending notification to PduR if set */
    tpCtrl->LinTp_PduRNotification       = LinTp_PduRNotificationIdle; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
# endif
  }
}

/**********************************************************************************************************************
 *
 * END - LinIf Initialization, Task and General Functions for master nodes
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 * LinIf Schedule Handling Functions
 *
 * The schedule handler manages the different LIN schedule tables.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_FlushAllSchedules()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, LINIF_CODE) LinIf_FlushAllSchedules
(
  CONSTP2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)  SchedTabDataPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset all schedule table related variables, set NULL schedule table, abort collision resolving table */
  SchedTabDataPtr->LinIf_CurrentTable              = (uint8)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
  SchedTabDataPtr->LinIf_CurrentSlot               = (uint8)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
  SchedTabDataPtr->LinIf_PreviousContinuousTable   = (uint8)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
  SchedTabDataPtr->LinIf_PreviousContinuousSlot    = (uint8)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
  SchedTabDataPtr->LinIf_CurrentSlotDelay          = (LinIf_ScheduleTableTickType)0x00u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
  SchedTabDataPtr->LinIf_RequestedTable            = LinIf_Schedule_Invalid; /* SBSW_LINIF_PASSED_PTR_ACCESS */
  SchedTabDataPtr->LinIf_SchedReqConfirmTable      = LinIf_Schedule_Invalid; /* SBSW_LINIF_PASSED_PTR_ACCESS */
# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
  SchedTabDataPtr->LinIf_EvtCollResolvingState     = LinIf_EvtCollResolvingState_Idle; /* SBSW_LINIF_PASSED_PTR_ACCESS */
# endif
}

/**********************************************************************************************************************
 *  LinIf_ScheduleRequest()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_ScheduleRequest
(
  VAR(NetworkHandleType, AUTOMATIC)                           Channel,
  VAR(LinIf_SchHandleType, AUTOMATIC)                         ScheduleTbl
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                              retVal = E_NOT_OK;
  uint8                                                       errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetSizeOfSystemToLinIfChannel() )
  {
    errorId = LINIF_E_NONEXISTENT_CHANNEL;
  }
  else
# endif
  {
    NetworkHandleType linIfChannelIdx           = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() )
    {
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
    }
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
    else if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) != LinIf_ChannelNodeType_Master )
    {
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
    }
#  endif
    else
# endif
    {
      P2CONST(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtr;
      LinIf_NodeChannelConfigIdxOfChannelConfigType masterChannelCfgIdx;
      P2VAR(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT) schedTabDataPtr;

      channelDataPtr       = LinIf_GetAddrChannelData(linIfChannelIdx);
      masterChannelCfgIdx  = LinIf_GetNodeChannelConfigIdxOfLinIfChannel(linIfChannelIdx);
      schedTabDataPtr      = &(LinIf_GetMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(linIfChannelIdx)).LinIf_SchedTabData);

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();

      /* #30 Check if channel state is operational */
      if ( channelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_SLEEP )
      {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_SCHEDULE_REQUEST_ERROR;
# endif
      }
      /* #40 Check validity of parameter ScheduleTbl */
      else if ( ( ScheduleTbl > (LinIf_SchHandleType)((LinIf_SchHandleType)LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndEndIdxOfMasterChannelConfig(masterChannelCfgIdx) - 1u) -
                                                       LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfMasterChannelConfig(masterChannelCfgIdx))) )
# if (LINIF_INVALIDHNDOFSCHEDULETABLELIST == STD_ON)
             || ( LinIf_IsInvalidHndOfScheduleTableList( LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfMasterChannelConfig(masterChannelCfgIdx)) + ScheduleTbl) )
# endif
              ) /* COV_LINIF_HANDLE_GAPS_CONFIG */
      {
        /* schedule index larger than biggest available schedule handle id on channel or schedule index is a gap in the schedule handle id range */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_SCHEDULE_REQUEST_ERROR;
# endif
      }
      else
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #50 Store the requested schedule and mark it pending (to be set after the next slot) */
        /* continuous, run once or NULL table requested */
        schedTabDataPtr->LinIf_RequestedTable        = ScheduleTbl; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */

        /* #60 Abort a possibly ongoing event-triggered frame collision resolving handling */
# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
        /* collision resolving table is discontinued on a new schedule request */
        schedTabDataPtr->LinIf_EvtCollResolvingState = LinIf_EvtCollResolvingState_Idle; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
# endif

        retVal = E_OK;
      }
      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_SCHEDULEREQUEST, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}

# if ( LINIF_SCHEDULE_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  LinIf_GetScheduleInfo()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, LINIF_CODE) LinIf_GetScheduleInfo
(
  VAR(NetworkHandleType, AUTOMATIC)                             Channel,
  P2VAR(LinIf_ScheduleInfoType, AUTOMATIC, LINIF_APPL_VAR)      ScheduleInfo
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                         errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter ScheduleInfo */
  else if ( ScheduleInfo == (P2VAR(LinIf_ScheduleInfoType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  /* #30 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetSizeOfSystemToLinIfChannel() )
  {
    errorId = LINIF_E_NONEXISTENT_CHANNEL;
  }
  else
#  endif
  {
    NetworkHandleType linIfChannelIdx           = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() )
    {
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
    }
#   if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
    else if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) != LinIf_ChannelNodeType_Master )
    {
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
    }
#   endif
    else
#  endif
    {
      /* ----- Implementation ----------------------------------------------- */
      P2CONST(LinIf_SchedTabDataType, AUTOMATIC, LINIF_VAR_NOINIT)  schedTabDataPtr;
      LinIf_ScheduleTableListIndStartIdxOfMasterChannelConfigType   schedTabListIdx;

      schedTabDataPtr                     = &(LinIf_GetMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(linIfChannelIdx)).LinIf_SchedTabData);

      /* get index into global schedule table list for current table */
      schedTabListIdx                     = (LinIf_ScheduleTableListIndStartIdxOfMasterChannelConfigType)
                                            (LinIf_GetScheduleTableListInd(LinIf_GetScheduleTableListIndStartIdxOfMasterChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(linIfChannelIdx))) +
                                                                           schedTabDataPtr->LinIf_CurrentTable);

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();

      /* #40 Set current and requested schedule table values of ScheduleInfo parameter */
      ScheduleInfo->CurrentTable          = schedTabDataPtr->LinIf_CurrentTable; /* SBSW_LINIF_SCHEDULEINFO_PTR */
      ScheduleInfo->RequestedTable        = schedTabDataPtr->LinIf_RequestedTable; /* SBSW_LINIF_SCHEDULEINFO_PTR */

      /* #50 If current table is the NULL schedule table, set slot index, length and position to zero */
      if ( ScheduleInfo->CurrentTable == LinIf_Schedule_NULL )
      {
        ScheduleInfo->CurrentSlot         = 0u; /* SBSW_LINIF_SCHEDULEINFO_PTR */
        ScheduleInfo->CurrentSlotLength   = 0u; /* SBSW_LINIF_SCHEDULEINFO_PTR */
        ScheduleInfo->CurrentSlotPosition = 0u; /* SBSW_LINIF_SCHEDULEINFO_PTR */
      }
      /* #60 Else set slot information based on current schedule table handler state */
      else
      {
        LinIf_EntryStartIdxOfScheduleTableListType slotListIdx;

        /* the stored slot index addresses already the subsequent slot, so the current slot has to be set to the previous slot */
        if ( schedTabDataPtr->LinIf_CurrentSlot == 0u)
        {
          ScheduleInfo->CurrentSlot       = (uint8)(LinIf_GetNumberOfScheduleEntriesOfScheduleTableList(schedTabListIdx) - 1u); /* SBSW_LINIF_SCHEDULEINFO_PTR */
        }
        else
        {
          ScheduleInfo->CurrentSlot       = (uint8)(schedTabDataPtr->LinIf_CurrentSlot - 1u); /* SBSW_LINIF_SCHEDULEINFO_PTR */
        }

        /* get index into global entry slot list for current slot */
        slotListIdx                       = (LinIf_EntryStartIdxOfScheduleTableListType)
                                            (LinIf_GetEntryStartIdxOfScheduleTableList(schedTabListIdx) + ScheduleInfo->CurrentSlot);
        /* set total delay of current slot */
        ScheduleInfo->CurrentSlotLength   = LinIf_GetDelayInTimebaseTicksOfEntry(slotListIdx); /* SBSW_LINIF_SCHEDULEINFO_PTR */
        /* set remaining delay of current slot */
        ScheduleInfo->CurrentSlotPosition = (LinIf_ScheduleTableTickType)(ScheduleInfo->CurrentSlotLength -
                                             schedTabDataPtr->LinIf_CurrentSlotDelay); /* SBSW_LINIF_SCHEDULEINFO_PTR */
      }

      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GETSCHEDULEINFO, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
}
# endif

/**********************************************************************************************************************
 *
 * END - LinIf Schedule Handling Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinIf Node Management for master nodes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_MasterWakeup()
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
 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_MasterWakeup
(
  VAR(NetworkHandleType, AUTOMATIC)                                     Channel,
  VAR(NetworkHandleType, AUTOMATIC)                                     LinIfChannel,
  CONSTP2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)        ChannelDataPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)       masterChannelDataPtr; /* initialized below before being used */

  /* ----- Implementation ----------------------------------------------- */
  masterChannelDataPtr   = LinIf_GetAddrMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel));

  SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();

  /* #10 If channel state is OPERATIONAL */
  if ( ChannelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_OPERATIONAL )
  {
    /* #20 If a gotosleep transition has been requested but not been started */
    if ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnReq )
    {
      /* #30 Reset pending gotosleep request */
      /* Clear current pending condition, sleep mode frame not yet started */
      masterChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

      /* #40 Confirm the wakeup to upper layer */
      /* Call function directly called, so no wakeup is send, remain in operational state, inform LinSM */
# if ( LINIF_WAKEUPCONF_AR403_COMPATIBILITY == STD_ON )
      /* FALSE returned because no wakeup frame is sent */
#  if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(LinIfChannel))(Channel, FALSE); /* SBSW_LINIF_FCT_POINTER_CALL */
#  else
      LinSM_WakeupConfirmation(Channel, FALSE);
#  endif
# else
      /* TRUE returned because wakeup is accepted according to AR4-612 (AR4.1.x) */
#  if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(LinIfChannel))(Channel, TRUE); /* SBSW_LINIF_FCT_POINTER_CALL */
#  else
      LinSM_WakeupConfirmation(Channel, TRUE);
#  endif
# endif
    }
    /* #50 If a gotosleep frame is currently transmitted */
    else if ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnTrans )
    {
      /* #60 Set flag to send wakeup frame afterwards */
      /* Sleep mode frame has been started, but has not yet been checked */
      masterChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_OnReWakeInt; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    }
    /* #70 Otherwise just confirm the wakeup to upper layer because the state is already OPERATIONAL */
    else
    {
      /* Ignore request */
      /* Call function directly called, so no wakeup is send, to inform upper layer */
# if ( LINIF_WAKEUPCONF_AR403_COMPATIBILITY == STD_ON )
      /* FALSE returned because no wakeup frame is sent */
#  if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(LinIfChannel))(Channel, FALSE); /* SBSW_LINIF_FCT_POINTER_CALL */
#  else
      LinSM_WakeupConfirmation(Channel, FALSE);
#  endif
# else
      /* TRUE returned because wakeup is accepted according to AR4-612 (AR4.1.x) */
#  if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
      LinIf_GetWakeupConfFctList(LinIf_GetWakeupConfFctListIdxOfChannelConfig(LinIfChannel))(Channel, TRUE); /* SBSW_LINIF_FCT_POINTER_CALL */
#  else
      LinSM_WakeupConfirmation(Channel, TRUE);
#  endif
# endif
    }
  }
  /* #80 Otherwise if channel state is SLEEP */
  else if ( ChannelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_SLEEP ) /* COV_LINIF_MISRA_ELSEBRANCH */
  {
    /* #90 If an internal sleep mode transition is currently active */
    if ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_OnTransInt )
    {
      /* #100 Set flag to send wakeup frame afterwards */
       masterChannelDataPtr->LinIf_OnSleepModeFrameHandling = LinIf_OnSleepModeFrameHandling_OnReWakeInt; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    }
    /* #110 Otherwise set channel state to OPERATIONAL state and transmit wakeup frame */
    else
    {
      LinIf_ProcessWakeupTransition(ChannelDataPtr, masterChannelDataPtr, LinIfChannel); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
  else
  {
    /* MISRA only */
  }

  /* #120 Clear wakeup flag, except the wakeup request processing is postponed to main function */
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  if ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling != LinIf_OnSleepModeFrameHandling_OnReWakeInt )
  {
    ChannelDataPtr->LinIf_WakeupFlag                   = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
  }
# endif

  SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();

  return E_OK;
}


/**********************************************************************************************************************
 *
 * END - LinIf Node Management for master nodes
 *
 *********************************************************************************************************************/


# if ( LINIF_TP_SUPPORTED == STD_ON )

/**********************************************************************************************************************
 *
 * LIN TRANSPORT PROTOCOL
 *
 * Description:  Master implementation of the AUTOSAR LIN Transport Protocol (part of LIN Interface)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL LINTP FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinTp Reception Handling - Local Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_MasterRxIndication_SF()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_MasterRxIndication_SF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT) CtrlPtr,
  VAR(NetworkHandleType, AUTOMATIC)                           LinIfChannel,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                   LinSduPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean                                                     proceedHandlingFlag = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* NAD checked for validity already before */
  /* #10 Verify if length information in SF is valid */
  if ( (LinSduPtr[1] > 0x00u) && (LinSduPtr[1] <= 0x06u) )
  {
    proceedHandlingFlag = TRUE;

    /* #20 If RX connection already active on channel, abort it and inform PduR */
    if( ( CtrlPtr->LinTp_Ch_State & LINTP_CHANNEL_RX_MSK) != 0u )
    { /* Indicate error and restart reception with new SF */
      CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNotifyResultRx     = E_NOT_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNSduIdRx           = CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* Upper layer ID of currently active connection */ /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }
    /* #30 Otherwise if received frame is a response pending frame (in this case channel state always idle) */
    else if ( (LinSduPtr[1] == 0x03u) &&
              (LinSduPtr[2] == 0x7Fu) &&
              (LinSduPtr[4] == 0x78u) )
    { /* response pending frame */

      /* #40 Update RP counter and check if maximum allowed number of response pending frames received */
      CtrlPtr->LinTp_RespPendingCounter++; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      if (CtrlPtr->LinTp_RespPendingCounter > LinTp_GetMaxRespPendingFrames())
      {
        /* #50 Abort reception, indication to PduR depends if forward-RP-frames-to-PduR feature activated, stop further handling */
#  if ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR == STD_ON )
        /* do only report the stop of reception if a connection was established, thus if preceding RP frames were reported to PduR */
        CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNotifyResultRx     = E_NOT_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNSduIdRx           = CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  endif
        /* reset requested NAD - only one response accepted for each request */
        CtrlPtr->LinTp_NAD_Current            = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_P2Timer                = 0u; /* important to abort timeout handling */ /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

        CtrlPtr->LinTp_PortNotification       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
        CtrlPtr->LinTp_ComModeRestore         = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  endif
        /* no further handling required */
        proceedHandlingFlag = FALSE; /* PRQA S 2982 */ /* MD_LINIF_Rule2.2_2982_RP */
      }
      else
      {
        /* #60 Reinitialize P2 timeout with P2Max */
        CtrlPtr->LinTp_P2Timer = LinTp_GetP2MaxTimeOfChannelConfig(LinTp_GetChannelOfLinIfToLinTpChannel(LinIfChannel)); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }

      /* #70 If forward-RP-frames-to-PduR feature deactivated, stop further handling */
#  if ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR == STD_ON )
#  else
      proceedHandlingFlag = FALSE;
#  endif
    /* #80 Otherwise: Normal SF received */
    }
    else
    { /* reset RP counter to trigger schedule change request in LinTp_Rx_Fill_Buffer */
      CtrlPtr->LinTp_RespPendingCounter = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }

    if ( proceedHandlingFlag == TRUE )
    {
      /* #90 Initialize new RX connection */
      CtrlPtr->LinTp_Timer                       = LinTp_GetNcrOfRxNSdu(CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_RemainingLength             = (PduLengthType)(LinSduPtr[1]); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_Tx_Rx_Buffer_Length         = (uint8)(CtrlPtr->LinTp_RemainingLength); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_Tx_Rx_Buffer_Idx            = 2u; /* index of the buffer copied from - SF:2 */ /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

      CtrlPtr->LinTp_Curr_UL_NSdu_Id             = LinTp_GetUpperLayerPduIdOfRxNSdu(CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

      /* #100 Reset P2 timer */
#  if ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR == STD_ON )
#  else
      /* stop P2 timer after receiving start of response */
      CtrlPtr->LinTp_P2Timer                     = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  endif

      CtrlPtr->LinTp_Ch_State                    = LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

      /* #110 Start receive buffer handling */
      /* do buffer handling in any case, on suspended state data is copied to local buffer */
      LinTp_RxDataBufferHandling_Main(CtrlPtr, LinSduPtr); /* SBSW_LINIF_TP_RXDATABUFHANDLING_CALL */
    }
  }
  /* else: ignore SF with invalid length - conform to LIN standard but not fully LINIF652, see bugzilla 52237 */
}


/**********************************************************************************************************************
 *  LinTp_MasterRxIndication_FF()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_MasterRxIndication_FF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)   CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                     LinSduPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* NAD checked for validity already before */
  /* #10 Verify if length information in FF is valid */
  if ( ( (((uint16)LinSduPtr[1] & 0x0Fu) << 8u) | (LinSduPtr[2]) ) > 6u)
  { /* valid FF length */

    /* #20 If RX connection already active on channel, abort it and inform PduR */
    if( ( CtrlPtr->LinTp_Ch_State & LINTP_CHANNEL_RX_MSK) != 0u )
    { /* Indicate error and postpone actual reception handling of FF to next cycle */
      CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNotifyResultRx     = E_NOT_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNSduIdRx           = CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }

    /* #30 Initialize new RX connection */
    CtrlPtr->LinTp_Timer                       = LinTp_GetNcrOfRxNSdu(CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_RemainingLength             = (PduLengthType)( (((uint16)LinSduPtr[1] & 0x0Fu) << 8u) | (LinSduPtr[2]) ); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_Tx_Rx_Buffer_Length         = 5u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_Tx_Rx_Buffer_Idx            = 3u; /* index of the buffer copied from - FF:3 */ /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_SN                          = 1u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

    CtrlPtr->LinTp_Curr_UL_NSdu_Id             = LinTp_GetUpperLayerPduIdOfRxNSdu(CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

    /* #40 Reset P2 timer */
    /* stop P2 timer after receiving start of response */
    CtrlPtr->LinTp_P2Timer                     = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    /* reset RP counter after receiving start of response to trigger schedule change request */
    CtrlPtr->LinTp_RespPendingCounter          = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

    CtrlPtr->LinTp_Ch_State = LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

    /* #50 Start receive buffer handling */
    LinTp_RxDataBufferHandling_Main(CtrlPtr, LinSduPtr); /* SBSW_LINIF_TP_RXDATABUFHANDLING_CALL */
  }
  /* else: ignore FF with invalid length - conform to LIN standard but not fully to LINIF652, see bugzilla 52237 */
}


/**********************************************************************************************************************
 *  LinTp_MasterRxIndication_Main()
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
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, LINIF_CODE) LinTp_MasterRxIndication_Main
(
  VAR(NetworkHandleType, AUTOMATIC)                         LinIfChannel,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                 LinSduPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)    tpCtrl; /* initialized below before being used */
  LinTp_ChannelType                                         linTpChannelIdx; /* initialized below before being used */

  /* ----- Implementation ----------------------------------------------- */
  tpCtrl = LinTp_GetAddrCtrl(LinIfChannel);
  linTpChannelIdx = LinTp_GetChannelOfLinIfToLinTpChannel(LinIfChannel);

  /* #10 If NAD of response does not match with NAD used in previous request */
  if (tpCtrl->LinTp_NAD_Current != LinSduPtr[0])
  {
    /* #20 If Strict-NAD check is disabled and received frame is valid SF or FF, inspect NAD */
    if ( ( !(LinTp_IsStrictNADCheckOfChannelConfig(linTpChannelIdx)) ) &&
         ( ( (LinSduPtr[1] & 0xF0u) == LinTp_FrameType_SF) || ( (LinSduPtr[1] & 0xF0u) == LinTp_FrameType_FF) ) ) /* COV_LINTP_STRICTNADCHECK_CONDITION */
    {
      /* NAD check disabled; an unexpected NAD is only expected for new reception (SF or FF); during a multi frame reception a new NAD is discarded */
      PduIdType sduIdx;

      /* #30 Search for a possible matching configured RxNSdu */
      for (sduIdx = 0u; sduIdx < (PduIdType)LinTp_GetNumberOfLinTpRxNSdu(); sduIdx++) /* COV_LINTP_STRICTNADCHECK_UNREACHEDCODE */  /* FETA_LINIF_UPPER_LIMIT_IS_GENERATED_CONSTANT */
      {
        /* Check if NAD is valid and matches the NAD of the configured RxNSdu on the same channel, consider handle gaps */
        if ( (LinTp_GetNADOfRxNSdu(sduIdx) == LinSduPtr[0]) && (LinTp_GetNADOfRxNSdu(sduIdx) != 0x00u) &&
#  if ( LINTP_INVALIDHNDOFRXNSDU == STD_ON )
             (!(LinTp_IsInvalidHndOfRxNSdu(sduIdx))) &&
#  endif
             (LinTp_GetCtrlIdxOfRxNSdu(sduIdx) == LinIfChannel) ) /* COV_LINTP_STRICTNADCHECK_NSDU_CONDITION */
        {
          /* #40 If RxNSdu found, reception of frame is accepted */
          tpCtrl->LinTp_Curr_Rx_Id = (PduIdType)sduIdx; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
#  if ( LINTP_META_DATA_SUPPORTED == STD_ON )
          /* If meta data is used, update the current NAD with the received one, otherwise use the statically configured one */
          if ( LinTp_IsMetaDataSupportedOfRxNSdu(sduIdx) ) /* COV_LINTP_STRICTNADCHECK_UNREACHEDCODE */
          {
            tpCtrl->LinTp_NAD_Current = LinSduPtr[0]; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
          }
          else
#  endif
          {
            tpCtrl->LinTp_NAD_Current = LinTp_GetNADOfRxNSdu(tpCtrl->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
          }
          break;
        }
      }
      /* #50 If no RxNSdu found, skip processing of frame below */
    }
  }

  /* #60 If no transmission is active and received NAD is accepted */
  /* a possible active Rx connection is handled below */
  if ( ((tpCtrl->LinTp_Ch_State & LINTP_CHANNEL_TX_MSK ) == 0u) && (tpCtrl->LinTp_NAD_Current == LinSduPtr[0]) )
  {
    /* #70 Evaluate frame type from received PCI */
    switch (LinSduPtr[1] & 0xF0u)
    {

      /***********************************************************************************************************
       *  LinTp_FrameType_SF
       ***********************************************************************************************************/
      case LinTp_FrameType_SF:
        /* #80 Process received single frame */
        LinTp_MasterRxIndication_SF(tpCtrl, LinIfChannel, LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
        break;

      /***********************************************************************************************************
       *  LinTp_FrameType_FF
       ***********************************************************************************************************/
      case LinTp_FrameType_FF:
        /* #90 Process received first frame */
        LinTp_MasterRxIndication_FF(tpCtrl, LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
        break;

      /***********************************************************************************************************
       *  LinTp_FrameType_CF
       ***********************************************************************************************************/
      case LinTp_FrameType_CF:
        /* #100 Process received consecutive frame */
        LinTp_RxIndication_CF(tpCtrl, LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
        break;

      default:
        /* #110 Ignore PDUs with unexpected PCI types (according to LIN standard) */
        break;
    }

  }
  /* #120 Otherwise if invalid NAD received and reception active, close Rx connection */
  else
  {
    /* stop of reception and notification to PduR is only performed if channel state is not idle, thus frames with
       unknown NADs are ignored if they do not interfere an ongoing reception. Furthermore, if no connection is
       established there is no upper layer PduId to report anyway */
    if ( ( tpCtrl->LinTp_Ch_State & LINTP_CHANNEL_RX_MSK ) != 0u )
    { /* Unexpected NAD received - stop receiving Tp message */
      LinTp_RxCloseConnection(tpCtrl); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
}

/**********************************************************************************************************************
 *
 * END - LinTp Reception Handling
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * LinTp Transmission Handling - Local Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_MasterTxMessageProcessing()
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
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_MasterTxMessageProcessing
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)  CtrlPtr,
  VAR(NetworkHandleType, AUTOMATIC)                           LinIfChannel,
  VAR(Lin_StatusType, AUTOMATIC)                              LinStatus
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If MRF is successfully transmitted */
  if ( LinStatus == (Lin_StatusType)LIN_TX_OK )
  {
    /* #20 If MRF is a functional request, confirm it by notifying PduR and reset schedule request to previous schedule */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    if (CtrlPtr->LinTp_FuncRequest == LINTP_FUNC_REQUEST_TRANSMITTED)
    { /* functional request transmitted - reset schedule request to previous schedule */
      CtrlPtr->LinTp_FuncRequest            = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PortNotification       = CtrlPtr->LinTp_ComModeRestore; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationTx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNotifyResultTx     = E_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNSduIdTx           = CtrlPtr->LinTp_Curr_Func_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }
    /* #30 Otherwise a physical Tx connection is active */
    /* Note that an ongoing transmission cannot be cancelled, also not by another transmit request */
    else
#  endif
    {
      /* #40 If complete Tp message is finished */
      if ( CtrlPtr->LinTp_RemainingLength == 0u )
      {
        /* #50 Tp request transmitted successfully, close connection and inform PduR */
        CtrlPtr->LinTp_Timer                  = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationTx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNotifyResultTx     = E_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNSduIdTx           = CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

        /* #60 If request is a broadcast request */
#  if ( LINTP_BROADCAST_REQUEST_HANDLING == STD_ON )
        if ( CtrlPtr->LinTp_NAD_Current == 0x7Fu ) /* COV_LINTP_BROADCASTHANDLING */
        {
          /* #70 Broadcast request transmitted, switch directly back to applicative schedule */
          CtrlPtr->LinTp_NAD_Current          = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
          CtrlPtr->LinTp_PortNotification     = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#   if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          CtrlPtr->LinTp_ComModeRestore       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#   endif
        }
        else
#  endif
        {
          /* #80 Normal physical request transmitted, switch to response schedule and activate P2 timeout observation */
          LinTp_ChannelType linTpChannelIdx;

          CtrlPtr->LinTp_PortNotification     = LINTP_DIAG_RESPONSE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          CtrlPtr->LinTp_ComModeRestore       = LINTP_DIAG_RESPONSE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  endif
          /* set up P2 time */
          linTpChannelIdx = LinTp_GetChannelOfLinIfToLinTpChannel(LinIfChannel);
          CtrlPtr->LinTp_P2Timer              = LinTp_GetP2TimeOfChannelConfig(linTpChannelIdx); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        }
      }
      /* #90 Otherwise: more frames to be transmitted for current request */
      else
      {
        Std_ReturnType retVal;

        /* #100 Prepare buffer for next CF */
        CtrlPtr->LinTp_SN                     = (uint8)((CtrlPtr->LinTp_SN + 1u) & 0x0Fu); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        /* set PCI */
        CtrlPtr->LinTp_Tx_Rx_Buffer[1]        = (uint8)(LinTp_FrameType_CF | CtrlPtr->LinTp_SN); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_Tx_Rx_Buffer_Idx       = 2u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        /* prepare pdu structure for buffer request */
        CtrlPtr->LinTp_PduInfoPtr.SduDataPtr  = &CtrlPtr->LinTp_Tx_Rx_Buffer[2]; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        if ( CtrlPtr->LinTp_RemainingLength <= 6u)
        {
          CtrlPtr->LinTp_PduInfoPtr.SduLength = CtrlPtr->LinTp_RemainingLength; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        }
        else
        {
          CtrlPtr->LinTp_PduInfoPtr.SduLength = 6u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        }

        /* #110 Request new transmit data */
        retVal = LinTp_TxDataRequest(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
        if ( retVal == E_OK )
        {
          /* #120 Request accepted, reload timer with CF timeout value */
          CtrlPtr->LinTp_Timer                  = LinTp_GetNcsOfTxNSdu(CtrlPtr->LinTp_Curr_Tx_Id); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        }
        else
        {
          /* #130 If request is denied, close connection and inform PduR - permanent failure */
          LinTp_TxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
        }
      }
    }
  }
  /* #150 Otherwise: MRF transmission failed */
  else
  {
    /* #160 If MRF is a functional request, inform PduR and request change to previous schedule */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    if (CtrlPtr->LinTp_FuncRequest == LINTP_FUNC_REQUEST_TRANSMITTED)
    {
      CtrlPtr->LinTp_FuncRequest            = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PortNotification       = CtrlPtr->LinTp_ComModeRestore; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNSduIdTx           = CtrlPtr->LinTp_Curr_Func_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationTx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduRNotifyResultTx     = E_NOT_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }
    /* 170 Otherwise (physcial request): Close connection and request change to application schedule */
    else
#  endif
    {
      LinTp_TxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
}


/**********************************************************************************************************************
 *  LinTp_MasterTxScheduleProcessing()
 *********************************************************************************************************************/
/*!
 *
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
 */
LINIF_LOCAL_INLINE FUNC(boolean, LINIF_CODE) LinTp_MasterTxScheduleProcessing
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)            CtrlPtr,
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON ) || ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  CONSTP2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)  MasterChannelDataPtr,
#  endif
  CONSTP2VAR(Lin_PduType, AUTOMATIC, LINIF_VAR_NOINIT)                  LinPduPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean                                                         sendHeaderFlag = TRUE;  /* be default, MRF header shall be transmitted */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
  PduLengthType                                                   remainingPduRBufferSize = 0u;
#  endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If a functional request is pending for transmission */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
  if ( CtrlPtr->LinTp_FuncRequest == LINTP_FUNC_REQUEST_PENDING )
  {
    BufReq_ReturnType bufferReqRetVal; /* initialized below before being used */

    /* #20 Initialize local buffer for functional request (always SF) */
#   if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
    MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_Idle; /* SBSW_LINIF_PASSED_PTR_ACCESS */
#   endif

    /* abuse the LinIf buffer for a functional MRF because this is handled in one cycle  */
    LinPduPtr->SduPtr = &(MasterChannelDataPtr->LinIf_TxData[0]); /* SBSW_LINIF_PASSED_PTR_ACCESS */

    /* prepare buffer */
    LinPduPtr->SduPtr[0] = LinIf_NAD_Functional; /* SBSW_LINIF_TP_LINPDU_SDUPTR */
    LinPduPtr->SduPtr[1] = (uint8)CtrlPtr->LinTp_FuncPduInfoPtr.SduLength; /* SBSW_LINIF_TP_LINPDU_SDUPTR */
    CtrlPtr->LinTp_FuncPduInfoPtr.SduDataPtr = &(LinPduPtr->SduPtr[2]); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

    /* #30 Request PduR to copy transmission data into provided local Tp buffer */
    bufferReqRetVal = PduR_LinTpCopyTxData(CtrlPtr->LinTp_Curr_Func_NSdu_Id, &(CtrlPtr->LinTp_FuncPduInfoPtr), /* SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_TX */
                                                  NULL_PTR, &remainingPduRBufferSize);
    /* #40 If transmission data is successfully provided */
    if (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK)
    {
      /* #50 Stuff unused payload bytes with fill value if necessary */
      uint8 i;
      for (i = (uint8)(LinPduPtr->SduPtr[1] + 2u); i < LINIF_LINPDU_SIZE; i++)
      {
        LinPduPtr->SduPtr[i] = 0xFFu; /* SBSW_LINIF_TP_LINPDU_SDUPTR */
      }
      CtrlPtr->LinTp_FuncRequest = LINTP_FUNC_REQUEST_TRANSMITTED; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }
    /* #60 Otherwise: request for transmission data not successful */
    else
    {
      /* #70 Abort functional request if data cannot be provided at this point */
      sendHeaderFlag                          = FALSE;
      CtrlPtr->LinTp_FuncRequest              = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PortNotification         = CtrlPtr->LinTp_ComModeRestore; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      /* reject attempt and free buffer */
      CtrlPtr->LinTp_PduRNotification        |= LinTp_PduRNotificationTxTm; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      /* result is always E_NOT_OK for TxTm notification */
      CtrlPtr->LinTp_PduRNSduIdTm             = CtrlPtr->LinTp_Curr_Func_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }
  }
  /* #80 Otherwise if transmission data for physical request already available in local buffer, transmit MRF */
  else if ( CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_TX_VALID )
#  else
  if ( CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_TX_VALID )
#  endif
  {
    /* #90 Reset node configuration command state because the following SRF must be handled by TP */
#  if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
    MasterChannelDataPtr->LinIf_AtNodeConfigurationCmd = LinIf_AtNodeConfigurationCmd_Idle; /* SBSW_LINIF_PASSED_PTR_ACCESS */
#  endif
    /* #100 Update provided Lin Pdu with transmission data */
    LinPduPtr->SduPtr = (uint8*)(&(CtrlPtr->LinTp_Tx_Rx_Buffer[0])); /* SBSW_LINIF_PASSED_PTR_ACCESS */
  }
  /* #110 Otherwise: Skip transmission of MRF frame */
  else
  {
    sendHeaderFlag = FALSE;
  }

  return sendHeaderFlag;
}

/**********************************************************************************************************************
 *
 * END - LinTp
 *
 *********************************************************************************************************************/

# endif /* LINIF_TP_SUPPORTED == STD_ON */

# define LINIF_STOP_SEC_CODE
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* LINIF_NODETYPE_MASTER_SUPPORTED */
