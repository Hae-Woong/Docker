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
 *         File:  LinIf.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Application-Interface of the AUTOSAR LIN Interface
 *                Declaration of functions, variables and constants
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-07-20  visbmo  ESCAN00057174 AR4 Support (derived from AR3 LinIf/LinTp)
 *  01.00.01  2012-09-13  visbmo  ESCAN00061396 Use defines of LIN driver
 *  01.00.02  2012-11-30  visbmo  ESCAN00063447 Compiler error: Upper layer function declaration incorrect
 *  01.01.00  2013-01-22  visbmo  ESCAN00061783 AR4-220: Remove STATIC
 *                                ESCAN00062851 Compile error: LinIf_Wakeup and Lin_Wakeup undefined
 *                                ESCAN00063142 Add precompile optimization for upper layer interfaces
 *                                ESCAN00064415 Compiler warning: Conversion from 'uint16' to 'PduLengthType'
 *  01.01.01  2013-02-15  visbmo  ESCAN00064916 LinTp: Adapt receive buffer handling
 *                                ESCAN00065148 Compiler error: LIN_CH_OPERATIONAL is now in ASR4 LIN_OPERATIONAL
 *                                ESCAN00065330 Use LIN_CH_SLEEP instead of LIN_OPERATIONAL
 *  01.02.00  2013-04-02  visbmo  ESCAN00064305 AR4-325: Implement Post-Build Loadable
 *                                ESCAN00064584 Remove unnecessary Vector parameters
 *                                ESCAN00064929 LinTp: Accept new physical request during ongoing transmission
 *                                ESCAN00065058 AR4-315: Support IF/TP CancelTransmit, TP CancelReceive, TP ChangeParameter
 *                                ESCAN00065489 LinTp: Unexpected PduR notification of NTFRSLT_E_UNEXP_PDU
 *  02.00.00  2013-09-10  visbmo  ESCAN00068503 AR4-260: Remove ProdErrorDetection configuration switch
 *                                ESCAN00068048 Support for configurations without unconditional frames
 *                                ESCAN00069725 AR4-389: Support for multiple TP connections to the same slave
 *                                ESCAN00069898 Compiler error: MemMap.h(XXXX) : fatal error #error :  "MemMap.h, wrong pragma command"
 *                                ESCAN00067709 AR4-328: Predefined Runtime Measurement Points
 *  02.00.01  2013-12-02  visbmo  ESCAN00071844 LinTp: Incorrect PduId notified to PduR if transmission of functional request failed
 *  02.01.00  2014-01-07  visbmo  ESCAN00072802 Adapt MISRA deviations; description file updated
 *  03.00.00  2014-04-24  visbmo  ESCAN00076251 Support of ComStackLib
 *                                ESCAN00071904 AR4-619: Support the StartOfReception API (with the PduInfoType), TxConfirmation and RxIndication according ASR4.1.2
 *                                ESCAN00075079 Remove obsolete support for BRS or None upper layer component
 *                                ESCAN00075131 Unexpected slave responses not forwarded with LinTpDropNotRequestedNad set to false
 *                                ESCAN00074116 AR4-642: Deleting container at post-build time
 *                                ESCAN00074304 AR4-612: [LinSM][LinIf] Retry change to FULL_COM
 *                                ESCAN00074707 TX frame in first slot after schedule table change is incorrectly handled
 *  04.00.00  2014-10-07  visbmo  ESCAN00076772 AR4-698: Post-Build Selectable (Identity Manager)
 *                                ESCAN00076219 AR4-820: Configurable call offsets for each channel for load balancing
 *                                ESCAN00076480 AR4-832: External wake up via LIN transceiver
 *                                ESCAN00077717 Wrong schedule table activated if schedule table indices contain gaps
 *                                ESCAN00077838 Support for special handling of diagnostic broadcast requests
 *                                ESCAN00078807 Close diagnostic connection on intermediate node configuration command schedule slot
 *  04.01.00  2015-01-07  visbmo  ESCAN00080299 FEAT-427: SafeBSW Step I
 *                                ESCAN00080333 FEAT-690: LIN schedule end notification [AR4-1001]
 *                                ESCAN00080336 LinTp: Reject physical physical request during ongoing transmission
 *  04.02.00  2015-05-18  visbmo  ESCAN00081787 FEAT-1275: SafeBSW Step 2
 *  05.00.00  2015-11-05  visbmo  ESCAN00084958 Adapt to updated R14 generators (breaking change and CSL)
 *                                ESCAN00085822 Compiler error: LinTp_IsInvalidHndOfTxNSdu undefined
 *                                ESCAN00086031 Compiler error: Linker cannot found symbol LinIf_WakeupConfirmation
 *                                ESCAN00086282 Rework wakeup source handling
 *  05.01.00  2016-02-04  visbmo  ESCAN00087805 FEAT-1688: SafeBSW Step 4
 *                                ESCAN00087859 FEAT-1731: J2602 Frame Tolerance Support
 *  05.01.01  2016-07-07  visbmo  ESCAN00090787 Communication starts after external wakeup before wakeup delay has elapsed
 *  05.02.00  2017-01-19  visbmo  FEATC-565     FEAT-2161: Support for LIN drivers with different interface type
 *                                FEATC-568     FEAT-2263: LIN ScheduleTable change after frame transmission
 *                                ESCAN00093427 Support for callback to provide Lin status to application
 *                                ESCAN00093428 Support for schedule table handler information API
 *                                ESCAN00093616 Support for particular wakeup delay after external wakeup
 *  05.02.01  2018-05-18  visbmo  ESCAN00099446 Support to handle more than 255 TP NSDUs
 *  06.00.00  2018-07-16  visbmo  STORYC-6020   MISRA-C2012: Compliance
 *                                STORYC-6082   Mirror support in LinIf
 *  07.00.00  2019-03-06  visbmo  ESCAN00101534 LinTp: Available Rx buffer queried again after PduR indicated an error
 *                                ESCAN00101535 LinTp: Missing schedule table request notification if no data provided for functional request
 *                                TASK-88519    MISRA justification rework
 *                                TASK-92793    Move types from LinIf_GeneralTypes.h to LinIf_Types.h
 *                                TASK-94611    Remove superfluous parameter checks
 *                                TASK-99820    LinTp: Do not allow functional requests with length greater than six
 *                                TASK-100610   LinTp: Rework initialization
 *                                ESCAN00101726 No communication after wakeup request during sleep frame transmission
 *                                TASK-107603   Evaluate LIN driver status for sleep frame with pending wakeup
 *  08.00.00  2019-06-06  visbmo  STORY-11999   LIN slave support: LinIf/LinTp implementation concept
 *                                ESCAN00106928 Event-triggered frame not received
 *  08.01.00  2020-01-13  visbmo  LIN-68        Consistent sleep transition with other modules
 *                                ESCAN00106242 LinTp + Slave: Incorrect data transmitted in SRF
 *  08.02.00  2020-06-04  visbmo  ESCAN00105925 Compiler warning: cast truncates constant value (LinTp_InvalidNsduIdx and LinTp_FrameMaxLength)
 *                                ESCAN00106477 Bus idle timeout is started too late after wakeup request
 *                                LIN-94        Support for wakeup confirmation timeout
 *  09.00.00  2020-07-13  visbmo  LIN-92        LinIf Support for J2602:2012: TargetedReset
 *                                LIN-109       LinIf Support for J2602:2012: J2602 StatusByte Handling
 *                                LIN-114       [LinIf] Update to Helix QAC 2019-2
 *  09.01.00  2020-12-03  visbmo  LIN-209       LinIf Jitter consideration
 *  09.02.00  2021-01-18  visbmo  LIN-100       Support for optional node configuration service ReadByIdentifier 0xB2 with ID = 1
 *                                LIN-102       Support for optional node configuration service ReadByIdentifier 0xB2 with IDs [32, 63]
 *            2021-02-18  visbmo  LIN-98        Support for optional node configuration service DataDump 0xB4
 *                                LIN-103       Support for optional node configuration service AutoAddressingSlave 0xB8
 *  09.03.00  2021-06-04  visbmo  LIN-379       LinTp extension to support PduR meta data interface
 *                                LIN-397       [LinIf] Support Helix QAC 2021.1
 *  10.00.00  2021-11-25  visbmo  LIN-454       LinIf/LinTp: Usage of LinIf_MemMap
 *                                LIN-508       [LinIf] Channel specific main function
 *  10.01.00  2022-06-15  vispwf  LIN-601       Differentiated use of Det Service-Routines
 *  10.02.00  2023-03-06  vispwf  LIN-689       Finite Execution Time Analysis
 *  11.00.00  2023-06-22  visbmo  MSR4-35257    AUTOSAR 4.3.1 3rd party driver support
 *            2023-06-22  visbmo  LIN-916       Configurable LIN driver initial startup state
 *            2023-06-30  visbmo  LIN-755       Delay status call after wakeup transmission
 *            2023-07-11  visbmo  LIN-918       Configurable call to Lin_GetStatus after wakeup request
 *            2023-09-04  visbmo  LIN-920       Call to Lin_GetStatus after internal sleep request
 *            2023-09-07  visbmo  LIN-811       Improve initialization of local variables
 *  12.00.00  2024-02-07  vispwf  MSR4-35258    AUTOSAR 4.4 to 21-11 3rd party driver support
 *                                LIN-1025      LinIf_WakeupConfirmation declaration
 *                                LIN-1027      Adapt channel parameter in LinIf callbacks
 *********************************************************************************************************************/

#if !defined (LINIF_H)
# define LINIF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "LinIf_Cfg.h"
# include "LinIf_Cbk.h"

# if ( LINIF_TP_SUPPORTED == STD_ON )
#  include "LinTp_Cfg.h"
#  include "BswM_LinTp.h"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Component version information (decimal version of ALM implementation package) */
# define LINIF_SW_MAJOR_VERSION                     (12u)
# define LINIF_SW_MINOR_VERSION                     (0u)
# define LINIF_SW_PATCH_VERSION                     (0u)

/* Vendor and module identification */
# define LINIF_VENDOR_ID                            (30u)
# define LINIF_MODULE_ID                            (62u)

/* AUTOSAR Software specification version information */
# define LINIF_AR_RELEASE_MAJOR_VERSION             (4u)
# define LINIF_AR_RELEASE_MINOR_VERSION             (0u)
# define LINIF_AR_RELEASE_REVISION_VERSION          (3u)

/* Generator compatibility */
# define LINIF_GENERATOR_COMPATIBILITY_VERSION      ((uint16)0x0108U)

# define LINIF_INSTANCE_ID_DET                      (0x00u)

/* API service IDs */
# define LINIF_SID_INIT                             (0x01u)
# define LINIF_SID_GETVERSIONINFO                   (0x03u)
# define LINIF_SID_TRANSMIT                         (0x04u)
# define LINIF_SID_SCHEDULEREQUEST                  (0x05u)
# define LINIF_SID_GOTOSLEEP                        (0x06u)
# define LINIF_SID_WAKEUP                           (0x07u)
# define LINIF_SID_SETTRCVMODE                      (0x08u)
# define LINIF_SID_GETTRCVMODE                      (0x09u)
# define LINIF_SID_GETTRCVWAKEUPREASON              (0x0Au)
# define LINIF_SID_SETTRCVWAKEUPMODE                (0x0Bu)
# define LINIF_SID_CANCELTRANSMIT                   (0x0Cu)

# define LINTP_SID_INIT                             (0x40u)
# define LINTP_SID_TRANSMIT                         (0x41u)
# define LINTP_SID_GETVERSIONINFO                   (0x42u)
# define LINTP_SID_SHUTDOWN                         (0x43u)
# define LINTP_SID_CHANGEPARAMETER                  (0x44u)
# define LINTP_SID_CANCELTRANSMIT                   (0x46u)
# define LINTP_SID_CANCELRECEIVE                    (0x47u)

# define LINIF_SID_CHECKWAKEUP                      (0x60u)
# define LINIF_SID_WAKEUPCONFIRMATION               (0x61u)
# define LINIF_SID_GETCONFIGUREDNAD                 (0x70u)
# define LINIF_SID_SETCONFIGUREDNAD                 (0x71u)
# define LINIF_SID_GETPIDTABLE                      (0x72u)
# define LINIF_SID_SETPIDTABLE                      (0x73u)
# define LINIF_SID_HEADERINDICATION                 (0x78u)
# define LINIF_SID_RXINDICATION                     (0x79u)
# define LINIF_SID_TXCONFIRMATION                   (0x7Au)
# define LINIF_SID_LINERRORINDICATION               (0x7Bu)

# define LINIF_SID_ENABLE_BUSMIRRORING              (0x7Fu)

# define LINIF_SID_MAINFUNCTION                     (0x80u)

/* Additional Service ID */
# define LINIF_SID_GETSCHEDULEINFO                  (0x0Du)
# define LINIF_SID_SETLINPRODUCTIDENTIFICATION      (0x0Eu)
# define LINIF_SID_SETJ2602STATUSBYTERESETBIT       (0x0Fu)

/* List of frame types */
/*! LinIf frame type Unconditional
 * \trace CREQ-602
 */
# define LinIf_FrameType_UnconditionalFrame         ((uint8)0x00u)
# define LinIf_FrameType_MRF                        ((uint8)0x01u)
# define LinIf_FrameType_SRF                        ((uint8)0x02u)
/*! LinIf frame type Event-triggered (only Master)
 * \trace CREQ-605
 */
# define LinIf_FrameType_EventTriggeredFrame        ((uint8)0x04u)
/*! LinIf frame type Sporadic (only Master)
 * \trace CREQ-607
 */
# define LinIf_FrameType_SporadicFrame              ((uint8)0x05u)

/*! LinIf frame type Node configuration (only Master)
 * \trace CREQ-600
 */
# define LinIf_FrameType_AssignFrameId              ((uint8)0x06u)
# define LinIf_FrameType_UnassignFrameId            ((uint8)0x07u)
# define LinIf_FrameType_AssignNAD                  ((uint8)0x08u)
# define LinIf_FrameType_FreeFormat                 ((uint8)0x09u)
# define LinIf_FrameType_ConditionalChangeNAD       ((uint8)0x0Au)
# define LinIf_FrameType_AssignFrameIdRange         ((uint8)0x0Bu)
# define LinIf_FrameType_SaveConfiguration          ((uint8)0x0Cu)

/*! LinIf frame type Network Management */
# define LinIf_FrameType_SleepModeFrame             ((uint8)0x0Du)
# define LinIf_FrameType_WakeUpFrame                ((uint8)0x0Eu)

/* Values for LinIf_ChannelStateType */
# define LINIF_CHANNEL_UNINIT                       ((LinIf_ChannelStateType)0x00u)
/*! LinIf channel OPERATIONAL state
 * \trace CREQ-595
 */
# define LINIF_CHANNEL_OPERATIONAL                  ((LinIf_ChannelStateType)0x01u)
/*! LinIf channel SLEEP state
 * \trace CREQ-596
 */
# define LINIF_CHANNEL_SLEEP                        ((LinIf_ChannelStateType)0x02u)

/* Development error codes */
# define LINIF_E_NO_ERROR                           (0xBAu)
# define LINIF_E_UNINIT                             (0x00u)
# define LINIF_E_ALREADY_INITIALIZED                (0x10u)
# define LINIF_E_NONEXISTENT_CHANNEL                (0x20u)
# define LINIF_E_PARAMETER                          (0x30u)
# define LINIF_E_PARAMETER_POINTER                  (0x40u)
# define LINIF_E_SCHEDULE_REQUEST_ERROR             (0x51u)
# define LINIF_E_TRCV_INV_CHANNEL                   (0x52u)
# define LINIF_E_TRCV_INV_MODE                      (0x53u)
# define LINIF_E_TRCV_NOT_NORMAL                    (0x54u)
# define LINIF_E_PARAM_WAKEUPSOURCE                 (0x55u)
# define LINIF_E_RESPONSE                           (0x60u)
# define LINIF_E_NC_NO_RESPONSE                     (0x61u)
/* Added development error codes */
# define LINIF_E_CONFIG                             (0x71u)     /*!< Error code: Invalid configuration */
# define LINIF_E_TRIGGERTRANSMIT_NO_DATA            (0x72u)     /*!< Error code: No transmission data available */

/* Schedule Table Run Modes */
/*! LinIf schedule table mode RUN-CONTINUOUS
 * \trace CREQ-598
 */
# define LinIf_RunMode_RunContinuous                ((uint8)0x00u)
/*! LinIf schedule table mode RUN-CONTINUOUS
 * \trace CREQ-599
 */
# define LinIf_RunMode_RunOnce                      ((uint8)0x01u)

/* LIN Transceiver Handling */
# define LinIf_InvalidLinTrcv                       ((uint8)0xFFu)

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
# if ! defined ( LINIF_APPL_FRAME_STATUS_INFO )
#  define LINIF_APPL_FRAME_STATUS_INFO              STD_OFF
# endif

# if ! defined ( LINIF_SLAVE_J2602_STATUSBYTE_SKIP_FRAMES )
#  define LINIF_SLAVE_J2602_STATUSBYTE_SKIP_FRAMES  STD_OFF
# endif

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

# define LINIF_START_SEC_CODE
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  LinIf_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Power-up memory initialization.
 *  \details     Initializes component variables in *_INIT_* sections at power up.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-567
 *  \note        Use this function in case these variables are not initialized by the startup code.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_InitMemory
(
  void
);

/**********************************************************************************************************************
 *  LinIf_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes component LinIf.
 *  \details     Initializes all component variables and sets the component state to initialized.
 *  \param[in]   ConfigPtr               Pointer to LinIf configuration structure
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \pre         LinIf_InitMemory has been called unless LinIf_Dev_InitDetect is initialized by start-up code.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       CREQ-567
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_Init
(
  P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_INIT_DATA) ConfigPtr
);

# if ( LINIF_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  LinIf_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo               Pointer to location to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINIF_VERSION_INFO_API
 *  \trace       CREQ-568
 *  \note        Configuration parameter LinIfVersionInfoApi must be enabled.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR) versioninfo
);
# endif /* LINIF_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  LinIf_Transmit()
 *********************************************************************************************************************/
/*! \brief       Marks a sporadic frame as pending for transmission.
 *  \details     Indicates a request to transmit the frame specified by LinTxPduId.
 *               Only marks the sporadic frame as pending, transmission is postponed until the corresponding schedule
 *               slot is due.
 *  \param[in]   LinTxPduId              N-PDU ID of the LIN frame to be transmitted.
 *  \param[in]   PduInfoPtr              Pointer to LIN PDU related data containing DLC and pointer to LIN SDU buffer.
 *  \return      E_OK                    Transmit request was accepted.
 *  \return      E_NOT_OK                Transmit request was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       CREQ-581
 *  \note        This function is currently only applicable to LIN master nodes. It has no effect for LIN frames that
 *               belongs to LIN slave channels.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Transmit
(
  VAR(PduIdType, AUTOMATIC)                        LinTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) PduInfoPtr
);

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_ScheduleRequest()
 *********************************************************************************************************************/
/*! \brief       Requests a switch to a new schedule table.
 *  \details     Stores the request to enable a given schedule table after the current slot is due.
 *  \param[in]   Channel                 Network channel. The channel must reference a LIN master channel.
 *  \param[in]   ScheduleTbl             Handle of new schedule to be set
 *  \return      E_OK                    Schedule table request was accepted.
 *  \return      E_NOT_OK                Schedule table switch request was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *  \config      LINIF_NODETYPE_MASTER_SUPPORTED
 *  \trace       CREQ-575
 *  \note        This function is only applicable for LIN master nodes.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_ScheduleRequest
(
  VAR(NetworkHandleType, AUTOMATIC)   Channel,
  VAR(LinIf_SchHandleType, AUTOMATIC) ScheduleTbl
);
# endif

/**********************************************************************************************************************
 *  LinIf_GotoSleep()
 *********************************************************************************************************************/
/*! \brief       Requests the transition into sleep state.
 *  \details     Initiates a transition into sleep mode on the selected channel. After the current slot is
 *               finished, a sleep frame transmission is triggered. Afterwards the channel state is set to SLEEP.
 *  \param[in]   Channel                  Network channel
 *  \return      E_OK                     Request to go to sleep was accepted or channel is already in sleep state.
 *  \return      E_NOT_OK                 Request to go to sleep was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GotoSleep
(
  VAR(NetworkHandleType, AUTOMATIC) Channel
);

/**********************************************************************************************************************
 *  LinIf_Wakeup()
 *********************************************************************************************************************/
/*! \brief       Requests the transition into operational state.
 *  \details     Initiates the wake up process on the selected controller and transmits a wakeup frame.
 *  \param[in]   Channel                 Network channel
 *  \return      E_OK                    Request to wake up was accepted.
 *  \return      E_NOT_OK                Request to wake up was not accepted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Wakeup
(
  VAR(NetworkHandleType, AUTOMATIC) Channel
);

# if ( LINIF_TRCV_HANDLING == STD_ON )
/**********************************************************************************************************************
 *  LinIf_SetTrcvMode()
 *********************************************************************************************************************/
/*! \brief       Sets the transceiver to requested mode.
 *  \details     Requests the transceiver on the selected channel to set the given mode.
 *  \param[in]   Channel                 Network channel
 *  \param[in]   TransceiverMode         Requested transceiver mode
 *  \return      E_OK                    Request of transceiver state change was accepted.
 *  \return      E_NOT_OK                Request of transceiver state change was not accepted, previous state not changed.
 *  \pre         LinTrcv must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_TRCV_HANDLING
 *  \note        Configuration parameter LinIfTrcvDriverSupported must be enabled.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvMode
(
  VAR(NetworkHandleType, AUTOMATIC)    Channel,
  VAR(LinTrcv_TrcvModeType, AUTOMATIC) TransceiverMode
);

/**********************************************************************************************************************
 *  LinIf_GetTrcvMode()
 *********************************************************************************************************************/
/*! \brief       Gets the transceiver mode.
 *  \details     Gets the current transceiver mode on the selected channel.
 *  \param[in]   Channel                 Network channel
 *  \param[out]  TransceiverModePtr      Pointer to memory location for requested transceiver mode to be stored.
 *                                       Parameter must not be NULL.
 *  \return      E_OK                    Request was accepted and transceiver mode is stored in TransceiverModePtr.
 *  \return      E_NOT_OK                Request was not accepted, TransceiverModePtr is not valid.
 *  \pre         LinTrcv must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_TRCV_HANDLING
 *  \note        Configuration parameter LinIfTrcvDriverSupported must be enabled.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvMode
(
  VAR(NetworkHandleType, AUTOMATIC)                      Channel,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_VAR) TransceiverModePtr
);

/**********************************************************************************************************************
 *  LinIf_GetTrcvWakeupReason()
 *********************************************************************************************************************/
/*! \brief       Gets the transceiver wakeup reason.
 *  \details     Gets the reason for wakeup detected by transceiver on the selected channel.
 *  \param[in]   Channel                 Network channel
 *  \param[out]  TrcvWuReasonPtr         Pointer to memory location for transceiver wake up reason to be stored.
 *                                       Parameter must not be NULL.
 *  \return      E_OK                    Request was accepted and wakeup reason is stored in TrcvWuReasonPtr.
 *  \return      E_NOT_OK                Request was not accepted, TrcvWuReasonPtr is not valid.
 *  \pre         LinTrcv must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_TRCV_HANDLING
 *  \note        Configuration parameter LinIfTrcvDriverSupported must be enabled.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvWakeupReason
(
  VAR(NetworkHandleType, AUTOMATIC)                              Channel,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_VAR) TrcvWuReasonPtr
);

/**********************************************************************************************************************
 *  LinIf_SetTrcvWakeupMode()
 *********************************************************************************************************************/
/*! \brief       Sets the transceiver wakeup mode to requested mode.
 *  \details     Sets the wakeup mode of the transceiver on the selected channel.
 *  \param[in]   Channel                 Network channel
 *  \param[in]   TrcvWakeupMode          Requested transceiver wakeup mode
 *  \return      E_OK                    Request of transceiver wakeup mode change was accepted.
 *  \return      E_NOT_OK                Request of transceiver wakeup mode change was not accepted, transceiver wakeup
 *                                       mode is not changed.
 *  \pre         LinTrcv must be initialized.
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_TRCV_HANDLING
 *  \note        Configuration parameter LinIfTrcvDriverSupported must be enabled.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvWakeupMode
(
  VAR(NetworkHandleType, AUTOMATIC)          Channel,
  VAR(LinTrcv_TrcvWakeupModeType, AUTOMATIC) LinTrcvWakeupMode
);
# endif /* LINIF_TRCV_HANDLING == STD_ON */


# if ( LINIF_CANCEL_TRANSMIT_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_CancelTransmit()
 *********************************************************************************************************************/
/*! \brief       This is a dummy method introduced for interface compatibility.
 *  \details     This function has no functionality.
 *  \param[in]   LinTxPduId              N-PDU ID of the LIN frame for which a cancellation is requested.
 *  \return      E_NOT_OK                Cancellation request is always rejected.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_CANCEL_TRANSMIT_SUPPORTED
 *  \trace       CREQ-582
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CancelTransmit
(
  VAR(PduIdType, AUTOMATIC) LinTxPduId
);
# endif


# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  LinIf_CheckWakeup()
 *********************************************************************************************************************/
/*! \brief       Checks a possible wakeup event on a specific channel.
 *  \details     Checks the given wakeup source and calls either LIN transceiver or LIN driver on a specific channel
 *               to perform further wakeup validation.
 *  \param[in]   WakeupSource             Source which initiated the wakeup event: LIN controller or LIN transceiver.
 *  \return      E_OK                     Request was accepted, source of wakeup belongs to a LIN channel.
 *  \return      E_NOT_OK                 Request was not accepted, wakeup source invalid.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different wakeup sources
 *  \synchronous TRUE
 *  \config      LINIF_LIN_CHANNEL_WAKEUP_SUPPORT or LINIF_LINTRCV_WAKEUP_SUPPORT
 *  \note        Wakeup support in LinTrcv or Lin has to be enabled on at least one LIN channel.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CheckWakeup
(
  VAR(EcuM_WakeupSourceType, AUTOMATIC) WakeupSource
);
# endif

# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_GetConfiguredNAD()
 *********************************************************************************************************************/
/*! \brief       Returns the currently configured NAD of the slave node.
 *  \details     -
 *  \param[in]   Channel                  Network channel. The channel must reference a LIN slave channel.
 *  \param[out]  Nad                      Pointer to location to store the NAD. Parameter must not be NULL. After
 *                                        successful return, it contains the configured NAD value.
 *                                        It is only valid if E_OK is returned from the function call.
 *  \return      E_OK                     Request was accepted, configured NAD value is stored at provided location.
 *  \return      E_NOT_OK                 Request was not accepted, configured NAD is not stored at provided location.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED
 *  \trace       CREQ-224381
 *  \note        This function is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetConfiguredNAD
(
  VAR(NetworkHandleType, AUTOMATIC)       Channel,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR) Nad
);

/**********************************************************************************************************************
 *  LinIf_SetConfiguredNAD()
 *********************************************************************************************************************/
/*! \brief       Sets the currently configured NAD of the slave node.
 *  \details     Update the configured NAD with the provided value.
 *  \param[in]   Channel                  Network channel. The channel must reference a LIN slave channel.
 *  \param[in]   Nad                      Configured NAD to set as new slave NAD.
 *  \return      E_OK                     Request was accepted, configured NAD is updated with provided value.
 *  \return      E_NOT_OK                 Request was not accepted, configured NAD is not changed.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED
 *  \trace       CREQ-224381
 *  \note        This function is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetConfiguredNAD
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(uint8, AUTOMATIC)             Nad
);

/**********************************************************************************************************************
 *  LinIf_GetPIDTable()
 *********************************************************************************************************************/
/*! \brief          Return the configured PIDs of all relevant frames of the slave node.
 *  \details        The order of the returned configured PIDs is ascending, corresponding to frame index. The PIDs for
 *                  the master request frame and slave response frame are not included.
 *  \param[in]      Channel                 Network channel. The channel must reference a LIN slave channel.
 *  \param[out]     PidBuffer               Pointer to buffer to which the current configured PIDs shall be copied to.
 *                                          Parameter must not be NULL.
 *  \param[in,out]  PidBufferLength         Length of provided buffer, pointed to by PidBuffer. After successful return
 *                                          it contains the number of copied PID values.
 *                                          If zero value is provided, after successful return it contains the number of
 *                                          configured PID values, without updating the buffer.
 *                                          Parameter must not be NULL.
 *  \return         E_OK                    Request was accepted, configured PIDs are copied to buffer and the number
 *                                          of copied PID values is returned.
 *  \return         E_NOT_OK                Request was not accepted, buffer and length parameters remain unchanged.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         LINIF_NODETYPE_SLAVE_SUPPORTED
 *  \trace          CREQ-224382
 *  \note           This function is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetPIDTable
(
  VAR(NetworkHandleType, AUTOMATIC)                   Channel,
  P2VAR(Lin_FramePidType, AUTOMATIC, LINIF_APPL_VAR)  PidBuffer,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR)             PidBufferLength
);

/**********************************************************************************************************************
 *  LinIf_SetPIDTable()
 *********************************************************************************************************************/
/*! \brief          Sets the configured PIDs of all relevant frames of the slave node.
 *  \details        The configured PIDs of the slave node are updated with the PIDs of the provided buffer. The order
 *                  of configured PIDs shall be ascending, corresponding to frame index. The PIDs for the master
 *                  request frame and slave response frame are not included.
 *  \param[in]      Channel                  Network channel. The channel must reference a LIN slave channel.
 *  \param[in]      PidBuffer                Pointer to buffer which contains the PID values to configure.
 *                                           Parameter must not be NULL.
 *  \param[in]      PidBufferLength          Number of PID values in the provided buffer.
 *  \return         E_OK                     Request was accepted, configured PIDs are updated.
 *  \return         E_NOT_OK                 Request was not accepted, configured PIDs remain unchanged.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         LINIF_NODETYPE_SLAVE_SUPPORTED
 *  \trace          CREQ-224382
 *  \note           This function is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetPIDTable
(
  VAR(NetworkHandleType, AUTOMATIC)                     Channel,
  P2CONST(Lin_FramePidType, AUTOMATIC, LINIF_APPL_VAR)  PidBuffer,
  VAR(uint8, AUTOMATIC)                                 PidBufferLength
);

#  if ( LINIF_NC_PRODUCT_ID_CONFIGURABLE_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_SetLinProductIdentification()
 *********************************************************************************************************************/
/*! \brief       Sets the LIN product identification values.
 *  \details     Updates the LIN product identification, consisting of Supplier ID, Function ID and Variant ID, during
 *               runtime. Allows overwriting the static configured LIN product identification.
 *  \param[in]   Channel                  Network channel. The channel must reference a LIN slave channel.
 *  \param[in]   ProductIdentKey          Identifier of the property of the LIN product identification to update.
 *                                        Valid values are:
 *                                        LINIF_LINPRODIDENT_SUPPLIER_ID: Updates the Supplier ID
 *                                        LINIF_LINPRODIDENT_FUNCTION_ID: Updates the Function ID
 *                                        LINIF_LINPRODIDENT_VARIANT_ID: Updates the Variant ID
 *  \param[in]   Value                    New value to set as LIN product identification, selected by ProductIdentKey.
 *                                        Supplier ID: Valid range is [0, 0x7FFF].
 *                                        Function ID: Valid range is [0, 0xFFFF].
 *                                        Variant ID: Valid range is [0, 0xFF]. Only LSB of parameter Value is used.
 *  \return      E_OK                     Request was accepted, LIN product identification updated.
 *  \return      E_NOT_OK                 Request was not accepted, LIN product identification remain unchanged.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED and LINIF_NC_PRODUCT_ID_CONFIGURABLE_SUPPORTED
 *  \trace       CREQ-224383
 *  \note        This function is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetLinProductIdentification
(
  VAR(NetworkHandleType, AUTOMATIC)      Channel,
  VAR(LinIf_ProductIdentType, AUTOMATIC) ProductIdentKey,
  VAR(uint16, AUTOMATIC)                 Value
);
#  endif

#  if ( LINIF_SLAVE_J2602_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_SetJ2602StatusByteResetBit()
 *********************************************************************************************************************/
/*! \brief       Sets the reset state in the J2602 status byte and marks a positive response pending.
 *  \details     Requests to set reset state bit active in the J2602 status byte. After a software reset has been
 *               performed after reception of a Targeted Reset service, the application can set the reset state bit
 *               afterwards using this function.
 *               Optionally, a positive request is marked as pending and transmitted when the next slave response
 *               header is received. This is mandatory for a previous physical Targeted Reset request while no response
 *               shall be sent for broadcast reset command.
 *  \param[in]   Channel                  Network channel. The channel must reference a LIN slave channel.
 *  \param[in]   TriggerResponse          TRUE: Requests the positive response transmission for the reset request.
 *                                        FALSE: No response is transmitted for the reset request.
 *  \return      E_OK                     Request accepted, reset bit is set and positive response set to pending.
 *  \return      E_NOT_OK                 Request not accepted, status byte not updated and no response set.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED and LINIF_SLAVE_J2602_SUPPORTED
 *  \trace       CREQ-256442
 *  \note        This function is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetJ2602StatusByteResetBit
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(boolean, AUTOMATIC)           TriggerResponse
);
#  endif
# endif

/**********************************************************************************************************************
 *  LinIf_MainFunction_<shortname>()
 *********************************************************************************************************************/
/*! \brief       Performs the main processing for one channel of the LIN interface.
 *  \details     Processes the schedule and frame handling and performs the appropriate state transitions.
 *               It is called periodically.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
/* The LinIf_MainFunction_<shortname> definitions are located in LinIf_Lcfg.c.
   The declarations are provided by SchM or Rte. */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_SCHEDULE_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  LinIf_GetScheduleInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the schedule table handler information.
 *  \details     Returns the current state of the LinIf schedule table handler. The returned information contains the
 *               current table identifier, current slot index, requested (pending) table identifier, current slot length
 *               and position inside slot.
 *  \param[in]   Channel              Network channel. The channel must reference a LIN master channel.
 *  \param[out]  ScheduleInfo         Pointer to location to store the schedule information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      LINIF_SCHEDULE_INFO_API and LINIF_NODETYPE_MASTER_SUPPORTED
 *  \trace       CREQ-120270
 *  \note        Configuration parameter LinIfScheduleInfoApi must be enabled.
 *               This callback is only applicable for LIN master nodes.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinIf_GetScheduleInfo
(
  VAR(NetworkHandleType, AUTOMATIC)                        Channel,
  P2VAR(LinIf_ScheduleInfoType, AUTOMATIC, LINIF_APPL_VAR) ScheduleInfo
);
#  endif
# endif /* LINIF_SCHEDULE_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  LinIf_EnableBusMirroring()
 *********************************************************************************************************************/
/*! \brief       Enables or disables mirroring for a LIN channel.
 *  \details     Requests to change the mirror mode of a LIN channel to either enabled or disabled. Mirroring can only
 *               be enabled on a channel if mirroring support is globally enabled.
 *  \param[in]   Channel                  Network channel
 *  \param[in]   MirroringActive          TRUE: Mirror_ReportLinFrame will be called for each frame received or
 *                                              transmitted on the given channel.
 *                                        FALSE: Mirror_ReportLinFrame will not be called for the given channel.
 *  \return      E_OK                     Mirroring mode was changed.
 *  \return      E_NOT_OK                 Wrong Channel, or mirroring globally disabled (see LinIfBusMirroringSupport).
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_EnableBusMirroring
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(boolean, AUTOMATIC)           MirroringActive
);

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_APPL_FRAME_STATUS_INFO == STD_ON )
/**********************************************************************************************************************
 *  Appl_LinIfGetLinStatus()
 *********************************************************************************************************************/
/*! \brief       Informs the application about the driver status for the current frame.
 *  \details     This callback function is called after each frame to inform the upper layer directly about the status
 *               returned by LIN driver of the last processed frame.
 *               This callout is not called for sleep and wakeup frames.
 *  \param[in]   Channel                  Network channel
 *  \param[in]   Pid                      Protected identifier of frame
 *  \param[in]   Status                   LIN Status value retrieved from LIN driver
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_APPL_FRAME_STATUS_INFO and LINIF_NODETYPE_MASTER_SUPPORTED
 *  \note        This callback function has to be enabled with a user config file where LINIF_APPL_FRAME_STATUS_INFO is
 *               defined to STD_ON and has to be provided by application.
 *               This callback is only applicable for LIN master nodes.
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) Appl_LinIfGetLinStatus
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(Lin_FramePidType, AUTOMATIC)  Pid,
  VAR(Lin_StatusType, AUTOMATIC)    Status
);
#  endif
# endif

# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
#  if ( LINIF_NC_GENERAL_SUPPORTED == STD_ON )

#   if ( LINIF_NC_READ_BY_IDENTIFIER_1_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  Appl_LinIfReadByIdSerialNumber()
 *********************************************************************************************************************/
/*! \brief       Informs the application about the reception of a ReadByIdentifier request with identifier 1.
 *  \details     This callback function is called after reception of a ReadByIdentifier request with identifier 1
 *               (serial number) addressing this slave node. The application decides with the return value the type of
 *               the response: Either a positive response is transmitted if the request is accepted or a negative
 *               response is transmitted if the request is declined. In case of a positive response, the application
 *               must provide the response data using the pointer ResponseData, see parameter description below for
 *               usage details.
 *  \param[in]   Channel                            Network channel
 *  \param[in]   Nad                                Received NAD in request
 *  \param[out]  ResponseData                       Pointer to an uint8 buffer with four elements. If the return value
 *                                                  requests a positive response, this buffer must be filled by
 *                                                  application with the 4 byte serial number to be used in the
 *                                                  positive response. The LSB of the serial number shall be written
 *                                                  to ResponseData[0] and the MSB to ResponseData[3]. If the return
 *                                                  value requests a negative response, this buffer is not used and
 *                                                  needs not to be written.
 *                                                  Valid write access index range: [0, 3].
 *  \return      LinIf_ReadByIdSerialNumber_PosResp Positive response requested.
 *  \return      LinIf_ReadByIdSerialNumber_NegResp Negative response requested.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED and LINIF_NC_GENERAL_SUPPORTED and LINIF_NC_READ_BY_IDENTIFIER_1_SUPPORTED
 *  \note        This callback function is optionally configurable.
 *               This callback is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(LinIf_ReadByIdSerialNumberReturnType, LINIF_CODE) Appl_LinIfReadByIdSerialNumber
(
  VAR(NetworkHandleType, AUTOMATIC)           Channel,
  VAR(uint8, AUTOMATIC)                       Nad,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR)     ResponseData
);
#   endif

#   if ( LINIF_NC_READ_BY_IDENTIFIER_32_63_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  Appl_LinIfReadByIdUserDefined()
 *********************************************************************************************************************/
/*! \brief       Informs the application about the reception of a ReadByIdentifier request with identifier [32, 63].
 *  \details     This callback function is called after reception of a ReadByIdentifier request with identifier in
 *               range [32, 63] (user defined) addressing this slave node. The application decides with the return
 *               value the type of the response: either a positive response if the request is accepted, a negative
 *               response if the request is declined or no response at all. In case of a positive response, the
 *               application must also provide the response data and length, see parameter description below for
 *               usage details.
 *  \param[in]   Channel                            Network channel
 *  \param[in]   Nad                                Received NAD in request
 *  \param[in]   Id                                 Received identifier (ReadByIdentifier ID), possible range: [32, 63]
 *  \param[out]  ResponseData                       Pointer to an uint8 buffer with five elements. If the return value
 *                                                  requests a positive response, this buffer must be filled by the
 *                                                  application with the user defined data bytes. The actual response
 *                                                  length (PCI) must be written to parameter ResponseLength. Even if
 *                                                  the response length is less than 5, it is recommended to fill all 5
 *                                                  buffer elements to have defined response data on the bus.
 *                                                  Valid write access index range: [0, 4].
 *  \param[out]  ResponseLength                     Pointer to an uint8 variable to specify the response length. If the
 *                                                  return value requests a positive response, this value must be set
 *                                                  to the actual number of data bytes written into the buffer provided
 *                                                  via pointer ResponseData. Valid range of response length: [1, 5].
 *  \return      LinIf_ReadByIdUserDefined_PosResp  Positive response requested.
 *  \return      LinIf_ReadByIdUserDefined_NegResp  Negative response requested.
 *  \return      LinIf_ReadByIdUserDefined_NoResp   No response requested (Note: non LIN conform).
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED and LINIF_NC_GENERAL_SUPPORTED and
 *               LINIF_NC_READ_BY_IDENTIFIER_32_63_SUPPORTED
 *  \note        This callback function is optionally configurable.
 *               This callback is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(LinIf_ReadByIdUserDefinedReturnType, LINIF_CODE) Appl_LinIfReadByIdUserDefined
(
  VAR(NetworkHandleType, AUTOMATIC)       Channel,
  VAR(uint8, AUTOMATIC)                   Nad,
  VAR(uint8, AUTOMATIC)                   Id,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR) ResponseData,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR) ResponseLength
);
#   endif

#   if ( LINIF_NC_DATA_DUMP_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  Appl_LinIfDataDump()
 *********************************************************************************************************************/
/*! \brief       Informs the application about the reception of a DataDump request.
 *  \details     This callback function is called after reception of a DataDump request addressing this slave node.
 *               The application decides with the return value the type of the response: Either a positive response is
 *               transmitted if the request is accepted or no response is transmitted if the request is declined.
 *               In case of a positive response, the application must provide the response data using the pointer
 *               ResponseData, see parameter description below for usage details.
 *  \param[in]   Channel                            Network channel
 *  \param[in]   Nad                                Received NAD in request
 *  \param[in]   RequestData                        Pointer to an uint8 buffer with five elements which contains the
 *                                                  received five user defined data bytes of the request.
 *                                                  Valid read access index range: [0, 4].
 *  \param[out]  ResponseData                       Pointer to an uint8 buffer with five elements. If the return value
 *                                                  requests a positive response, this buffer must be filled by
 *                                                  application with the five user defined response bytes to be used
 *                                                  in the positive response. If the return value requests no response,
 *                                                  this buffer is not further used and needs not to be written.
 *                                                  Valid write access index range: [0, 4].
 *  \return      LinIf_DataDump_PosResp             Positive response requested.
 *  \return      LinIf_DataDump_NoResp              No response requested.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED and LINIF_NC_GENERAL_SUPPORTED and LINIF_NC_DATA_DUMP_SUPPORTED
 *  \note        This callback function is optionally configurable.
 *               This callback is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(LinIf_DataDumpReturnType, LINIF_CODE) Appl_LinIfDataDump
(
  VAR(NetworkHandleType, AUTOMATIC)         Channel,
  VAR(uint8, AUTOMATIC)                     Nad,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR) RequestData,
  P2VAR(uint8, AUTOMATIC, LINIF_APPL_VAR)   ResponseData
);
#   endif

#   if ( LINIF_NC_AUTO_ADDRESSING_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  Appl_LinIfAutoAddressing()
 *********************************************************************************************************************/
/*! \brief       Informs the application about the reception of an AutoAddressing request.
 *  \details     This callback function is called after reception of an AutoAddressing request.
 *               The application decides with the return value the type of the response: Either no response is
 *               transmitted (this is the default case) or an optional positive response is transmitted.
 *  \param[in]   Channel                            Network channel
 *  \param[in]   Nad                                Received NAD in request
 *  \param[in]   RequestData                        Pointer to an uint8 buffer with five elements which contains the
 *                                                  received five user defined data bytes of the request.
 *                                                  Valid read access index range: [0, 4].
 *  \return      LinIf_AutoAddressing_PosResp       Positive response requested.
 *  \return      LinIf_AutoAddressing_NoResp        No response requested.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED and LINIF_NC_GENERAL_SUPPORTED and LINIF_NC_AUTO_ADDRESSING_SUPPORTED
 *  \note        This callback function is optionally configurable.
 *               This callback is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(LinIf_AutoAddressingReturnType, LINIF_CODE) Appl_LinIfAutoAddressing
(
  VAR(NetworkHandleType, AUTOMATIC)         Channel,
  VAR(uint8, AUTOMATIC)                     Nad,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR) RequestData
);
#   endif

#   if ( LINIF_SLAVE_J2602_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  Appl_LinIfJ2602TargetedReset()
 *********************************************************************************************************************/
/*! \brief       Informs the application about the reception of a J2602 Targeted Reset request.
 *  \details     This callback function is called after reception of a J2602 Targeted Reset request addressing this
 *               slave node. The application decides with the return value the type of the response: either a positive
 *               response if the request is accepted, a negative response if the request is declined or no response at
 *               all. The latter case is intended to use if a reset is performed in software and the response is sent
 *               after the reset (either a positive or negative response is mandatory).
 *               The NAD of the request is provided to distinguish physical and broadcast requests. In case of a
 *               broadcast reset request, no response is sent at all, independent of the provided return value.
 *               If a positive response is requested, then the reset bit in the J2602 status byte is set.
 *  \param[in]   Channel                            Network channel
 *  \param[in]   Nad                                Received NAD in request
 *  \return      LinIf_J2602TargetedReset_PosResp   Positive response requested. In case of a broadcast request, the
 *                                                  response is suppressed.
 *  \return      LinIf_J2602TargetedReset_NegResp   Negative response requested. In case of a broadcast request, the
 *                                                  response is suppressed.
 *  \return      LinIf_J2602TargetedReset_NoResp    No response requested. It is assumed that the response is triggered
 *                                                  by application later. Note that according to SAE J2602 a response
 *                                                  shall be sent for a physical request in any case.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED and LINIF_NC_GENERAL_SUPPORTED and LINIF_SLAVE_J2602_SUPPORTED
 *  \note        This callback function is required for LIN slave channels using protocol version SAE J2602 and has to
 *               be provided by application.
 *               This callback is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(LinIf_J2602TargetedResetReturnType, LINIF_CODE) Appl_LinIfJ2602TargetedReset
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(uint8, AUTOMATIC)             Nad
);
#   endif
#  endif

#  if ( LINIF_SLAVE_J2602_SUPPORTED == STD_ON )
#   if ( LINIF_SLAVE_J2602_STATUSBYTE_SKIP_FRAMES == STD_ON )
/**********************************************************************************************************************
 *  Appl_LinIfJ2602StatusByteProcessFrame()
 *********************************************************************************************************************/
/*! \brief       Requests the application if the current frame contains a J2602 status byte.
 *  \details     In general the LinIf assumes, according to the SAE J2602 specification, that each unconditional Tx
 *               frame includes a J2602 status error field. So after successful transmission of an unconditional frame,
 *               a possible pending error status is cleared and the J2602 status error field is updated accordingly.
 *               However, in special configurations, some unconditional Tx frames might not contain a J2602 status
 *               error field. This callback function can be used to inform if a transmitted unconditional frame
 *               contains a J2602 status error field or does not contain it. In latter case, the update of the status
 *               error field is skipped. The integrator must ensure that the configuration of the J2602 status byte
 *               references in the LinIf coincides with the implementation of this callback.
 *  \param[in]   Channel                            Network channel
 *  \param[in]   Pid                                Pid of transmitted unconditional frame.
 *  \return      TRUE                               The frame contains a J2602 status byte and the status error field
 *                                                  shall be updated by the LinIf accordingly. This is the default
 *                                                  case.
 *  \return      FALSE                              The frame does not contain a J2602 status byte. The handling of the
 *                                                  status error field shall be skipped by LinIf for this frame.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE for different channels
 *  \synchronous TRUE
 *  \config      LINIF_NODETYPE_SLAVE_SUPPORTED and LINIF_SLAVE_J2602_SUPPORTED and
 *               LINIF_SLAVE_J2602_STATUSBYTE_SKIP_FRAMES
 *  \note        This callback function has to be enabled with a user config file where
 *               LINIF_SLAVE_J2602_STATUSBYTE_SKIP_FRAMES is defined to STD_ON and has to be provided by application.
 *               It is only invoked for LIN slave channels using protocol version SAE J2602.
 *               This callback is only applicable for LIN slave nodes.
 *********************************************************************************************************************/
FUNC(boolean, LINIF_CODE) Appl_LinIfJ2602StatusByteProcessFrame
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(uint8, AUTOMATIC)             Pid
);
#   endif
#  endif
# endif


# if ( LINIF_TP_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinTp_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes component LinTp.
 *  \details     Initializes all component variables and sets the component state to initialized.
 *  \param[in]   ConfigPtr               Pointer to LinTp configuration structure
 *  \pre         Module is uninitialized.
 *  \pre         Interrupts are disabled.
 *  \pre         LinIf_InitMemory has been called unless LinTp_Dev_InitDetect is initialized by start-up code.
 *  \pre         LinIf must be initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *  \trace       CREQ-587
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_Init
(
  P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_INIT_DATA) ConfigPtr
);

/**********************************************************************************************************************
 *  LinTp_Transmit()
 *********************************************************************************************************************/
/*! \brief       Requests the transmission of diagnostic data using transport protocol.
 *  \details     Request the transfer of segmented data over the LIN bus using master request frames.
 *  \param[in]   LinTpTxSduId            Unique N-SDU identifier of LIN N-SDU to be transmitted.
 *  \param[in]   LinTpTxInfoPtr          Pointer to LIN N-SDU related data containing DLC and pointer to LIN n-SDU buffer.
 *  \return      E_OK                    Request was accepted and can be started successfully.
 *  \return      E_NOT_OK                Request was not accepted and cannot be started.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \config      LINIF_TP_SUPPORTED
 *  \trace       CREQ-584
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_Transmit
(
  VAR(PduIdType, AUTOMATIC)                         LinTpTxSduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)  LinTpTxInfoPtr
);

#  if ( LINTP_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  LinTp_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo               Pointer to location to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-583
 *  \config      LINIF_TP_SUPPORTED and LINTP_VERSION_INFO_API
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR) versioninfo
);
#  endif /* LINIF_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  LinTp_Shutdown()
 *********************************************************************************************************************/
/*! \brief       Shutdowns the component.
 *  \details     Closes all pending transport protocol connection, frees all resources and sets the module into
 *               uninitialized state.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *  \trace       CREQ-585
 *********************************************************************************************************************/
FUNC(void, LINIF_CODE) LinTp_Shutdown
(
  void
);

#  if ( LINTP_CANCEL_TRANSMIT_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinTp_CancelTransmit()
 *********************************************************************************************************************/
/*! \brief       This is a dummy method introduced for interface compatibility.
 *  \details     This function has no functionality.
 *  \param[in]   LinTpTxSduId            Unique N-SDU identifier of LIN Tx N-SDU which transfer shall be cancelled.
 *  \return      E_NOT_OK                Cancellation request is always rejected.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED and LINTP_CANCEL_TRANSMIT_SUPPORTED
 *  \trace       CREQ-586
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelTransmit
(
  VAR(PduIdType, AUTOMATIC) LinTpTxSduId
);
#  endif

#  if ( LINTP_CHANGE_PARAMETER_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinTp_ChangeParameter()
 *********************************************************************************************************************/
/*! \brief       This is a dummy method introduced for interface compatibility.
 *  \details     This function has no functionality.
 *  \param[in]   id                      ID of the connection for whose channel the change shall be done.
 *  \param[in]   parameter               Selects the parameter that the request shall change (STmin).
 *  \param[in]   value                   New value of the parameter.
 *  \return      E_NOT_OK                Request is always rejected.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED and LINTP_CHANGE_PARAMETER_SUPPORTED
 *  \trace       CREQ-586
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_ChangeParameter
(
  VAR(PduIdType, AUTOMATIC)                 id,
  VAR(TPParameterType, AUTOMATIC)           parameter,
  VAR(uint16, AUTOMATIC)                    value
);
#  endif


#  if ( LINTP_CANCEL_RECEIVE_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinTp_CancelReceive()
 *********************************************************************************************************************/
/*! \brief       This is a dummy method introduced for interface compatibility.
 *  \details     This function has no functionality.
 *  \param[in]   LinTpRxSduId            Unique N-SDU identifier of LIN Rx N-SDU which transfer has to be cancelled.
 *  \return      E_NOT_OK                Cancellation request is always rejected.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED and LINTP_CANCEL_RECEIVE_SUPPORTED
 *  \trace       CREQ-586
 *********************************************************************************************************************/
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelReceive
(
  VAR(PduIdType, AUTOMATIC) LinTpRxSduId
);
#  endif /* LINTP_CANCEL_RECEIVE_SUPPORTED */

# endif /* LINIF_TP_SUPPORTED == STD_ON */

# define LINIF_STOP_SEC_CODE
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EXCLUSIVE AREA DEFINITION
 *********************************************************************************************************************/

/*! \exclusivearea LINIF_EXCLUSIVE_AREA_0
 * Ensures consistency while modifying global variables and states in LinIf main function. Ensures consistent reception data from LIN driver.
 * \protects LinIf_ChannelData and LinTp_Ctrl
 * \usedin LinIf_ChannelMainFunction
 * \exclude All functions provided by LinIf except LinIf_ChannelMainFunction.
 * \length LONG One channel main function cycle.
 * \endexclusivearea
 */

/*! \exclusivearea LINIF_EXCLUSIVE_AREA_1
 * Ensures consistency in LinIf service API functions if main function can interrupt it's own service functions.
 * \protects LinIf_ChannelData and LinTp_Ctrl
 * \usedin LinIf_ScheduleRequest, LinIf_GetScheduleInfo, LinIf_MasterWakeup, LinIf_SlaveWakeup, LinIf_GotoSleep,
 *         LinIf_CheckWakeup, LinTp_Shutdown, LinTp_Transmit, LinIf_EnableBusMirroring, LinIf_GetConfiguredNAD,
 *         LinIf_SetConfiguredNAD, LinIf_SetPIDTable, LinIf_SetLinProductIdentification
 * \exclude LinIf_ChannelMainFunction.
 * \length LONG Services of other modules like Lin driver are called.
 * \endexclusivearea
 */

/*! \exclusivearea LINIF_EXCLUSIVE_AREA_2
 * Ensures consistency in LinIf callback functions called by LIN driver if higher prior interrupts can interrupt the LIN interrupts.
 * \protects LinIf_ChannelData and LinTp_Ctrl
 * \usedin LinIf_HeaderIndication, LinIf_RxIndication, LinIf_TxConfirmation, LinIf_LinErrorIndication, LinIf_WakeupConfirmation
 * \exclude All other functions provided by LinIf functions.
 * \length LONG Services of other modules are called.
 * \endexclusivearea
 */

#endif /* LINIF_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf.h
 *********************************************************************************************************************/

