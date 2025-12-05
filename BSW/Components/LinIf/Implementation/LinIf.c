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
 *         File:  LinIf.c
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Implementation of the AUTOSAR LIN Interface
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

#define LINIF_SOURCE

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/
/* Disable the following MISRA warnings because they would appear too often for individual suppression.
   See justification at the end of file */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "LinIf.h"
#include "LinIf_Int.h"
#include "LinIf_Master.h"
#include "LinIf_Slave.h"

#if ( LINIF_USE_INIT_POINTER == STD_ON )
# include "EcuM_Error.h"
#else
# if ( LINIF_TP_SUPPORTED == STD_ON )
#  if ( LINTP_USE_INIT_POINTER == STD_ON )
#   include "EcuM_Error.h"
#  endif
# endif
#endif

#if ( LINIF_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
# include "Rtm.h"
#else
# if ( LINIF_TP_SUPPORTED == STD_ON )
#  if ( LINTP_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
#   include "Rtm.h"
#  endif
# endif
#endif


/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

#if ( LINIF_SW_MAJOR_VERSION != (12u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf.h are inconsistent (LINIF_SW_MAJOR_VERSION)!"
#endif
#if ( LINIF_SW_MINOR_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf.h are inconsistent (LINIF_SW_MINOR_VERSION)!"
#endif
#if ( LINIF_SW_PATCH_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf.h are inconsistent (LINIF_SW_PATCH_VERSION)!"
#endif

#if ( LINIF_TPS_MAJOR_VERSION != (12u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Types.h are inconsistent (LINIF_TPS_MAJOR_VERSION)!"
#endif
#if ( LINIF_TPS_MINOR_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Types.h are inconsistent (LINIF_TPS_MINOR_VERSION)!"
#endif
#if ( LINIF_TPS_PATCH_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Types.h are inconsistent (LINIF_TPS_PATCH_VERSION)!"
#endif

#if ( LINIF_CBK_MAJOR_VERSION != (12u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Cbk.h are inconsistent (LINIF_CBK_MAJOR_VERSION)!"
#endif
#if ( LINIF_CBK_MINOR_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Cbk.h are inconsistent (LINIF_CBK_MINOR_VERSION)!"
#endif
#if ( LINIF_CBK_PATCH_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
# error "Source and Header file LinIf_Cbk.h are inconsistent (LINIF_CBK_PATCH_VERSION)!"
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
# if ( LINTP_TPS_MAJOR_VERSION != (12u) ) /* COV_LINIF_CONFIG XF */
#  error "Source and Header file LinTp_Types.h are inconsistent (LINTP_TPS_MAJOR_VERSION)!"
# endif
# if ( LINTP_TPS_MINOR_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
#  error "Source and Header file LinTp_Types.h are inconsistent (LINTP_TPS_MINOR_VERSION)!"
# endif
# if ( LINTP_TPS_PATCH_VERSION != (0u) ) /* COV_LINIF_CONFIG XF */
#  error "Source and Header file LinTp_Types.h are inconsistent (LINTP_TPS_PATCH_VERSION)!"
# endif
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/

#if defined ( LINIF_CONFIGURATION_VARIANT ) /* COV_LINIF_CONFIG TX */
# if !( ( LINIF_CONFIGURATION_VARIANT == LINIF_CONFIGURATION_VARIANT_PRECOMPILE ) || \
        ( LINIF_CONFIGURATION_VARIANT == LINIF_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) ) /* COV_LINIF_CONFIG XF tf tf */
#  error "LINIF_CONFIGURATION_VARIANT has an invalid value, check settings in generation tool!"
# endif
#else
# error "LINIF_CONFIGURATION_VARIANT not defined, check settings in generation tool!"
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
# if defined ( LINTP_CONFIGURATION_VARIANT ) /* COV_LINIF_CONFIG TX */
#  if !( ( LINTP_CONFIGURATION_VARIANT == LINTP_CONFIGURATION_VARIANT_PRECOMPILE ) || \
         ( LINTP_CONFIGURATION_VARIANT == LINTP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE ) ) /* COV_LINIF_CONFIG XF tf tf */
#   error "LINTP_CONFIGURATION_VARIANT has an invalid value, check settings in generation tool!"
#  endif
# else
#  error "LINTP_CONFIGURATION_VARIANT not defined, check settings in generation tool!"
# endif

/* LINTP_LOCAL_BUFFER_SIZE is statically defined. The check is for SafeBsw and a hint that a change has impact on implementation */
# if defined ( LINTP_LOCAL_BUFFER_SIZE ) /* COV_LINIF_CONFIG TX */
#  if ( LINTP_LOCAL_BUFFER_SIZE != 8 ) /* COV_LINIF_CONFIG XF */
#   error "LINTP_LOCAL_BUFFER_SIZE shall be 8. If this changes, adapt the access of LinTp_Tx_Rx_Buffer."
#  endif
# else
#  error "LINTP_LOCAL_BUFFER_SIZE is not defined".
# endif
#endif

/* LINIF_LINPDU_SIZE is statically defined. The check is for SafeBsw and a hint that a change has impact on implementation */
#if defined ( LINIF_LINPDU_SIZE ) /* COV_LINIF_CONFIG TX */
# if ( LINIF_LINPDU_SIZE != 8 ) /* COV_LINIF_CONFIG XF */
#  error "LINIF_LINPDU_SIZE shall be 8. If this changes, adapt the access of Lin Pdus."
# endif
#else
# error "LINIF_LINPDU_SIZE is not defined."
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define LinIf_Channel_Invalid                       ((NetworkHandleType)0xFFu) /*!< Invalid LinIf channel identifier */
#define LinTp_Channel_Invalid                       ((NetworkHandleType)0xFFu) /*!< Invalid LinTp channel identifier */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#if ( LINIF_USE_INIT_POINTER == STD_ON )
# define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to global LinIf Configuration */
P2CONST(LinIf_ConfigType, LINIF_VAR_ZERO_INIT, LINIF_INIT_DATA)     LinIf_ConfigDataPtr = NULL_PTR;

# define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

#if ( LINIF_TP_SUPPORTED == STD_ON )
# if ( LINTP_USE_INIT_POINTER == STD_ON )
#  define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to global LinTp Configuration */
P2CONST(LinTp_ConfigType, LINTP_VAR_ZERO_INIT, LINTP_INIT_DATA)     LinTp_ConfigDataPtr = NULL_PTR;

#  define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
#  include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
# endif
#endif


#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
# define LINIF_START_SEC_VAR_ZERO_INIT_8BIT
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization check variable for LinIf. Must be set to UNINIT by startup code if initialized global variables
    are supported by LinIf_InitMemory (needs to be called as very first service function) */
VAR(uint8, LINIF_VAR_ZERO_INIT)                                     LinIf_Dev_InitDetect  = LINIF_UNINIT;

# if ( LINIF_TP_SUPPORTED == STD_ON )
/*! Initialization check variable for LinTp. Must be set to UNINIT by startup code if initialized global variables
    are supported by LinIf_InitMemory (needs to be called as very first service function) */
VAR(uint8, LINTP_VAR_ZERO_INIT)                                     LinTp_Dev_InitDetect  = LINTP_UNINIT;
# endif

# define LINIF_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL LINIF FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define LINIF_START_SEC_CODE
#include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LinIf_ChannelInit()
 *********************************************************************************************************************/
/*! \brief       Initializes a LinIf channel.
 *  \details     Initializes the variables of a specific channel of the component.
 *  \param[in]   LinIfChannel        Local LinIf channel identifier
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelInit
(
  VAR(NetworkHandleType, AUTOMATIC)                             LinIfChannel
);

#if ( LINIF_TP_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinTp_TxProcessRequest()
 *********************************************************************************************************************/
/*! \brief       Starts the processing of a transmission request.
 *  \details     Initiliazes a new Tx connection, requests data from upper layer and prepares first frame of message
 *               for transmission.
 *  \param[in]   TxSduId              Unique N-SDU identifier of LIN N-SDU to be transmitted.
 *  \param[in]   CtrlPtr              Pointer to Tp channel control structure
 *  \param[in]   TxLength             Length of request
 *  \param[in]   Nad                  Nad to use in request
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(Std_ReturnType, LINIF_CODE) LinTp_TxProcessRequest
(
  VAR(PduIdType, AUTOMATIC)                                     TxSduId,
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)   CtrlPtr,
  VAR(PduLengthType, AUTOMATIC)                                 TxLength,
  VAR(uint8, AUTOMATIC)                                         Nad
);

/**********************************************************************************************************************
 *  LinTp_TxGetNad()
 *********************************************************************************************************************/
/*! \brief       Retrieves the NAD to use in the request.
 *  \details     Determines the NAD used in transmission based on the Tx SduId, node type and configuration settings.
 *  \param[in]   TpTxSduId            Unique N-SDU identifier of LIN N-SDU to be transmitted.
 *  \param[in]   TpTxInfoPtr          Pointer to LIN N-SDU related data containing DLC and pointer to LIN n-SDU buffer.
 *  \return      Nad value to use in request. Returns 0 if no valid NAD can be found based on the given settings and
 *               the transmission request shall be rejected.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(uint8, LINIF_CODE) LinTp_TxGetNad
(
  VAR(PduIdType, AUTOMATIC)                                     TxSduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)              TxInfoPtr
);

/**********************************************************************************************************************
 *  LinTp_TxTransmitInternal()
 *********************************************************************************************************************/
/*! \brief       Requests the transmission of diagnostic data using transport protocol.
 *  \details     See LinTp_Transmit() for details.
 *  \param[in]   LinTpTxSduId            Unique N-SDU identifier of LIN N-SDU to be transmitted.
 *  \param[in]   LinTpTxInfoPtr          Pointer to LIN N-SDU related data containing DLC and pointer to LIN n-SDU buffer.
 *  \return      E_OK                    Request was accepted and can be started successfully.
 *  \return      E_NOT_OK                Request was not accepted and cannot be started.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(Std_ReturnType, LINIF_CODE) LinTp_TxTransmitInternal
(
  VAR(PduIdType, AUTOMATIC)                                     LinTpTxSduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)              LinTpTxInfoPtr
);

/**********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_PollBuffer()
 *********************************************************************************************************************/
/*! \brief       Requests the PduR for receive buffer.
 *  \details     Polls the PduR if sufficient receive buffer for next frame is available and updates the Tp reception
 *               state accordingly.
 *  \param[in]   CtrlPtr              Pointer to Tp channel control structure
 *  \param[in]   LinSduPtr            Pointer to receive buffer of current frame
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      LINIF_TP_SUPPORTED
 *********************************************************************************************************************/
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_PollBuffer
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)   CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                     LinSduPtr
);
#endif

/**********************************************************************************************************************
 *  LOCAL LINIF FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinIf Initialization, Task and General Function Interface - Local Functions
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LinIf_ChannelInit()
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
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinIf_ChannelInit
(
  VAR(NetworkHandleType, AUTOMATIC)                               LinIfChannel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT)       channelDataPtr; /* initialized below before being used */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all general channel specific variables to default */
  channelDataPtr                                        = LinIf_GetAddrChannelData(LinIfChannel);

  /* Set default values of internal variables */
  channelDataPtr->LinIf_FrameHandle                     = LinIf_FrameHandle_Invalid; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */

#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  channelDataPtr->LinIf_WakeupFlag                      = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
#endif
  channelDataPtr->LinIf_WakeupDelayTimer                = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
#if ( LINIF_BUSMIRRORING == STD_ON )
  channelDataPtr->LinIf_MirroringActivationState        = FALSE; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
#endif

  /* #30 If current channel is a LIN Master channel */
#if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  if ( LinIf_GetNodeTypeOfChannelConfig(LinIfChannel) == LinIf_ChannelNodeType_Master )
# endif
  {
    /* Master channel */
    P2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) masterChannelDataPtr = LinIf_GetAddrMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel));

    /* #40 Initialize master specific channel frame data */
    masterChannelDataPtr->LinIf_FrameMaxDelay               = (LinIf_ScheduleTableTickType)0x00u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    masterChannelDataPtr->LinIf_OnSleepModeFrameHandling    = LinIf_OnSleepModeFrameHandling_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
    masterChannelDataPtr->LinIf_AtNodeConfigurationCmd      = LinIf_AtNodeConfigurationCmd_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif

    masterChannelDataPtr->LinIf_PduRNotificationFlag        = LinIf_PduRNotificationIdle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */

# if ( LINIF_BUSMIRRORING == STD_ON )
    masterChannelDataPtr->LinIf_MirroringEventPending       = FALSE; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    masterChannelDataPtr->LinIf_MirroringPid                = 0x40u; /* invalid PID - dummy value which is always overwritten before being used */ /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    masterChannelDataPtr->LinIf_MirroringLinStatus          = LIN_CH_SLEEP; /*  dummy value which is always overwritten before being used */ /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif

    /* #50 Clear schedule table data and set NULL schedule table */
    LinIf_FlushAllSchedules(&(masterChannelDataPtr->LinIf_SchedTabData)); /* SBSW_LINIF_FLUSHALLSCHED_CALL_1 */
# if ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
    masterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedEndNotifyTable   = LinIf_Schedule_Invalid; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif
    masterChannelDataPtr->LinIf_SchedTabData.LinIf_SchedReqConfirmTable  = LinIf_Schedule_Invalid; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
  }
#endif

  /* #60 Otherwise (current channel is a LIN Slave channel) */
#if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  else
# endif
  {
    /* Slave channel */
    LinIf_ConfiguredPIDTableStartIdxOfSlaveChannelConfigType  configuredPidTblIdx; /* initialized below in for loop */
    LinIf_FrameListStartIdxOfChannelConfigType                frameListIdx; /* initialized below before being used */
    P2VAR(LinIf_SlaveChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) slaveChannelDataPtr = LinIf_GetAddrSlaveChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel));

    /* #70 Initialize slave specific channel frame data */
    slaveChannelDataPtr->LinIf_FrameState         = LinIf_SlaveFrameState_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    slaveChannelDataPtr->LinIf_NmWakeupState      = LinIf_SlaveNmState_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    slaveChannelDataPtr->LinIf_RespErrValue       = 0x00u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    slaveChannelDataPtr->LinIf_BusIdleTimer       = 0u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    slaveChannelDataPtr->LinIf_ConfiguredNAD      = LinIf_GetConfiguredNADOfSlaveChannelConfig(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel)); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# if ( LINIF_NC_GENERAL_SUPPORTED == STD_ON )
    slaveChannelDataPtr->LinIf_NasTimer           = 0x00u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    slaveChannelDataPtr->LinIf_NcRespBuffer[0]    = 0x00u; /* first byte as zero means no response is pending */ /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif

# if ( LINIF_NC_PRODUCT_ID_CONFIGURABLE_SUPPORTED == STD_ON )
    slaveChannelDataPtr->LinIf_SupplierId         = LinIf_GetSupplierIdOfSlaveChannelConfig(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel)); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    slaveChannelDataPtr->LinIf_FunctionId         = LinIf_GetFunctionIdOfSlaveChannelConfig(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel)); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    slaveChannelDataPtr->LinIf_VariantId          = LinIf_GetVariantIdOfSlaveChannelConfig(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel)); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif

# if ( LINIF_SLAVE_J2602_SUPPORTED == STD_ON )
    slaveChannelDataPtr->LinIf_J2602ErrorBits     = LinIf_J2602ErrorStateMask_NoError; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    slaveChannelDataPtr->LinIf_J2602TxErrorStatus = LinIf_J2602ErrorStateVal_NoError; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif

    /* init PID RAM table */
    frameListIdx = LinIf_GetFrameListStartIdxOfChannelConfig(LinIfChannel);
    for (configuredPidTblIdx = LinIf_GetConfiguredPIDTableStartIdxOfSlaveChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel));
         configuredPidTblIdx < (uint16)LinIf_GetConfiguredPIDTableEndIdxOfSlaveChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel));
         configuredPidTblIdx++) /* FETA_LINIF_UPPER_LIMIT_ENDINDEX */
    {
      LinIf_SetConfiguredPIDTable(configuredPidTblIdx, LinIf_GetPidOfFrameList(frameListIdx)); /* SBSW_LINIF_SET_CONFIGURED_PID */
      frameListIdx++;
    }

  }
#endif

  /* #80 If configured LinIf channel startup state is SLEEP state */
  if ( LinIf_GetStartupStateOfChannelConfig(LinIfChannel) == LinIf_StartupState_Sleep )
  {
    /* #90 Set channel state to SLEEP mode */
    channelDataPtr->LinIf_ChannelState        = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */

    /* #100 If configured LIN driver channel startup state is OPERATIONAL, request internal LIN driver sleep mode transition */
    if ( LinIf_GetLinDriverStartupStateOfChannelConfig(LinIfChannel) == LinIf_LinDrvStartupState_Normal )
    {
#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
      (void) LinIf_GetLin_GoToSleepInternalFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
      (void) LinIf_Lin_GoToSleepInternal_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel));
#endif
    }
  }
  /* #110 Otherwise (configured LinIf channel startup state is OPERATIONAL state) */
  else
  {
    /* #120 Set channel to OPERATIONAL mode */
    channelDataPtr->LinIf_ChannelState        = LINIF_CHANNEL_OPERATIONAL; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */

    /* #130 If configured LIN driver channel startup state is SLEEP, request internal LIN driver wakeup mode transition */
    if ( LinIf_GetLinDriverStartupStateOfChannelConfig(LinIfChannel) == LinIf_LinDrvStartupState_Sleep )
    {
#if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
      (void) LinIf_GetLin_WakeupInternalFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(LinIfChannel)) (LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel)); /* SBSW_LINIF_FCT_POINTER_CALL */
#else
      (void) LinIf_Lin_WakeupInternal_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(LinIfChannel));
#endif
    }

    /* #140 If current channel is a LIN Slave channel, start bus idle timeout observation */
#if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
    if ( LinIf_GetNodeTypeOfChannelConfig(LinIfChannel) == LinIf_ChannelNodeType_Slave )
# endif
    {
      P2VAR(LinIf_SlaveChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) slaveChannelDataPtr;

      slaveChannelDataPtr = LinIf_GetAddrSlaveChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(LinIfChannel));
      slaveChannelDataPtr->LinIf_BusIdleTimer = LinIf_GetBusIdleTimeoutOfSlaveChannelConfig(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(LinIfChannel)); /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
    }
#endif

  }
}

/**********************************************************************************************************************
 *
 * END - LinIf Initialization, Task and General Function Interface - Local Functions
 *
 *********************************************************************************************************************/

#if ( LINIF_TP_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LOCAL LINTP FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinTp Initialization, Task and General Function Interface - Local Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_TxProcessRequest()
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
LINIF_LOCAL_INLINE FUNC(Std_ReturnType, LINIF_CODE) LinTp_TxProcessRequest
(
  VAR(PduIdType, AUTOMATIC)                                     TxSduId,
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)   CtrlPtr,
  VAR(PduLengthType, AUTOMATIC)                                 TxLength,
  VAR(uint8, AUTOMATIC)                                         Nad
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                                retVal; /* initialized below before being used */
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON ) && ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  uint16                                                        linIfChannel = 0u;
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Setup identifier and length of connection and init local buffer */
  /* Save current N-SDU */
  CtrlPtr->LinTp_Curr_Tx_Id               = TxSduId; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
  CtrlPtr->LinTp_Curr_UL_NSdu_Id          = LinTp_GetUpperLayerPduIdOfTxNSdu(TxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
  /* Save internal ID of corresponding RxNSdu (only relevant for master nodes) */
  CtrlPtr->LinTp_Curr_Rx_Id               = LinTp_GetAssociatedRxNSduIdOfTxNSdu(TxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
  /* Setup NAD and length */
  CtrlPtr->LinTp_Tx_Rx_Buffer[0]          = Nad; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
  CtrlPtr->LinTp_RemainingLength          = TxLength; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */

  /* #20 Prepare frame PCI information depending in frame type */
  if ( TxLength <= 6u )
  {
    /* SF request */
    CtrlPtr->LinTp_Tx_Rx_Buffer[1]        = (uint8)(TxLength); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
    CtrlPtr->LinTp_Tx_Rx_Buffer_Idx       = 2u; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
    /* prepare pdu structure for buffer request */
    CtrlPtr->LinTp_PduInfoPtr.SduDataPtr  = &CtrlPtr->LinTp_Tx_Rx_Buffer[2]; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
    CtrlPtr->LinTp_PduInfoPtr.SduLength   = TxLength; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
  }
  else
  {
    /* multi frame request */
    /* set FF token and upper nibble of data length */
    CtrlPtr->LinTp_Tx_Rx_Buffer[1]        = (uint8)(((TxLength & 0x0F00u) >> 8u) | LinTp_FrameType_FF); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
    /* set lower data length byte */
    CtrlPtr->LinTp_Tx_Rx_Buffer[2]        = (uint8)  (TxLength & 0x00FFu); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
    CtrlPtr->LinTp_Tx_Rx_Buffer_Idx       = 3u; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
    /* init sequence number, this number will be incremented after FF transmission for correct CF start value 1 */
    CtrlPtr->LinTp_SN                     = 0u; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
    /* prepare pdu structure for buffer request */
    CtrlPtr->LinTp_PduInfoPtr.SduDataPtr  = &CtrlPtr->LinTp_Tx_Rx_Buffer[3]; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
    CtrlPtr->LinTp_PduInfoPtr.SduLength   = 5u; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
  }

  /* #30 Request PduR to provide transmission data */
  retVal = LinTp_TxDataRequest(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
  /* #40 If data request is accepted by PduR */
  if ( retVal == E_OK )
  {
    /* #50 Activate SF/FF confirmation timeout */
    CtrlPtr->LinTp_Timer                  = LinTp_GetNasOfTxNSdu(TxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */

    /* #60 If channel is master, store NAD of request and trigger diagnostic request schedule */
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
    linIfChannel                          = LinTp_GetCtrlIdxOfTxNSdu(TxSduId);
    if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannel) == LinIf_ChannelNodeType_Master )
#  endif
    {
      /* save current NAD - only matching responses are accepted */
      CtrlPtr->LinTp_NAD_Current            = Nad; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      /* clear response pending counter */
      CtrlPtr->LinTp_RespPendingCounter     = 0u; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      /* clear P2 timer */
      CtrlPtr->LinTp_P2Timer                = 0u; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */

      CtrlPtr->LinTp_PortNotification       = LINTP_DIAG_REQUEST; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
      CtrlPtr->LinTp_ComModeRestore         = LINTP_DIAG_REQUEST; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
#  endif
    }
# endif
  }
  /* #70 Otherwise */
  else
  {
    /* unable to obtain data from PduR - permanent failure, inform PduR directly */
    PduR_LinTpTxConfirmation(CtrlPtr->LinTp_Curr_UL_NSdu_Id, E_NOT_OK);
    /* #80 Close Tp connection */
    CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  LinTp_TxGetNad()
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
LINIF_LOCAL_INLINE FUNC(uint8, LINIF_CODE) LinTp_TxGetNad
(
  VAR(PduIdType, AUTOMATIC)                                 TxSduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)          TxInfoPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                       retNad = 0u;
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  uint16                      linIfChannel = 0u;
# endif

  LINIF_DUMMY_STATEMENT(TxInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If N-SDU identifier refers to a LIN slave channel and configurable NAD support is used, return configured NAD */
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  linIfChannel = LinTp_GetCtrlIdxOfTxNSdu(TxSduId);
#  if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  if ( (LinIf_GetNodeTypeOfChannelConfig(linIfChannel) == LinIf_ChannelNodeType_Slave) &&
       (LinTp_IsConfigurableNadUsedOfTxNSdu(TxSduId)) )
#  else
  if ( LinTp_IsConfigurableNadUsedOfTxNSdu(TxSduId) )
#  endif
  {
    /* If the NAD is zero on a slave channel, the current configured NAD is used */
    retNad = LinIf_GetSlaveChannelData(LinIf_GetNodeChannelConfigIdxOfLinIfChannel(linIfChannel)).LinIf_ConfiguredNAD; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
  }
  else
# endif
  {
    /* #20 Otherwise if meta data is used for the N-SDU identifier */
# if ( LINTP_META_DATA_SUPPORTED == STD_ON )
    if ( LinTp_IsMetaDataSupportedOfTxNSdu(TxSduId) )
    {
      /* #30 Extract the NAD from the provided meta data buffer */
      retNad = TxInfoPtr->SduDataPtr[0];
      /* #40 Verify provided NAD and reject transmission in case of invalid values */
      /* No explicit check for NAD = 0 (sleep mode frame) required as return value of 0 corresponds to a negative return code */
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
#   if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
      if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannel) == LinIf_ChannelNodeType_Slave )
#   endif
      {
        if ( (retNad == LinIf_NAD_Functional) || (retNad == LinIf_NAD_Broadcast ) )
        {
          retNad = 0u;
        }
      }
#  endif
    }
    /* #50 Otherwise use statically configured NAD of N-SDU */
    else
# endif
    {
      retNad = LinTp_GetNADOfTxNSdu(TxSduId);
    }
  }

  return retNad;
}

/**********************************************************************************************************************
 *  LinTp_TxTransmitInternal()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
LINIF_LOCAL_INLINE FUNC(Std_ReturnType, LINIF_CODE) LinTp_TxTransmitInternal
(
  VAR(PduIdType, AUTOMATIC)                                 LinTpTxSduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)          LinTpTxInfoPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)     tpCtrl; /* initialized below before being used */
  uint8                                                     txNad; /* initialized below before being used */
  Std_ReturnType                                            retVal = E_NOT_OK;
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON ) && ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  uint16                                                    linIfChannel = 0u;
# endif

  LINIF_DUMMY_STATEMENT(LinTpTxSduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(LinTpTxInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Implementation ----------------------------------------------- */
  tpCtrl        = LinTp_GetAddrCtrl(LinTp_GetCtrlIdxOfTxNSdu(LinTpTxSduId));
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON ) && ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  linIfChannel  = LinTp_GetCtrlIdxOfTxNSdu(LinTpTxSduId);
# endif

  SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();

  /* #10 Retrieve NAD to use in transmission and reject transmit request if no valid NAD is returned */
  txNad = LinTp_TxGetNad(LinTpTxSduId, LinTpTxInfoPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
# if ( LINTP_META_DATA_SUPPORTED == STD_ON )
  /* An invalid NAD can only be returned if meta data handling is used */
  if ( txNad != 0u )
# endif
  {

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    /* #20 If requested NSdu is a valid functional request (master channel only) */
#   if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
    if ( (LinIf_GetNodeTypeOfChannelConfig(linIfChannel) == LinIf_ChannelNodeType_Master) &&
         (txNad == LinIf_NAD_Functional) && (LinTpTxInfoPtr->SduLength <= 6u) && (LinTpTxInfoPtr->SduLength > 0u) )
#   else
    if ( (txNad == LinIf_NAD_Functional) && (LinTpTxInfoPtr->SduLength <= 6u) && (LinTpTxInfoPtr->SduLength > 0u) )
#   endif
    {
      /* #30 Set functional request flag and trigger diagnostic schedule table */
      /* postpone buffer request of functional request until MRF is actually triggered to be transmitted, just store length of request here */
      tpCtrl->LinTp_FuncPduInfoPtr.SduLength    = LinTpTxInfoPtr->SduLength; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      /* request appropriate schedule */
      tpCtrl->LinTp_PortNotification            = LINTP_DIAG_REQUEST; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      /* store NSdu of functional request */
      tpCtrl->LinTp_Curr_Func_NSdu_Id           = LinTp_GetUpperLayerPduIdOfTxNSdu(LinTpTxSduId); /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      tpCtrl->LinTp_FuncRequest                 = LINTP_FUNC_REQUEST_PENDING; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
      retVal                                    = E_OK;
      /* further handling of functional frame is done on task level */
    }
    /* #40 Otherwise: (physical request) */
    else if ( txNad != LinIf_NAD_Functional )
#  endif
# endif
    {
      /* #50 If Tp Rx connection already active, abort it to accept new request (LIN master channel) or ignore new request (LIN slave channel) */
      if ( ( tpCtrl->LinTp_Ch_State & LINTP_CHANNEL_RX_MSK ) == LINTP_CHANNEL_RX_MSK )
      {
        /* Rx connection active */
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
        /* If channel is a LIN master channel, abort Rx connection to accept the new request */
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
        if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannel) == LinIf_ChannelNodeType_Master )
#  endif
        {
          PduR_LinTpRxIndication(tpCtrl->LinTp_Curr_UL_NSdu_Id, E_NOT_OK); /* direct notification to PduR */
          tpCtrl->LinTp_Timer                   = 0u; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          tpCtrl->LinTp_Ch_State                = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */

          tpCtrl->LinTp_P2Timer                 = 0u; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
          /* request applicative schedule here, overwritten below if request is accepted */
          tpCtrl->LinTp_PortNotification        = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          tpCtrl->LinTp_ComModeRestore          = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_TRANSMIT_CTRL_PTR */
#  endif
        }
# endif
      /* If channel is a LIN slave channel, ignore new request and keep the Rx connection */
      }

      /* #60 If Tp channel state is IDLE */
      /* An ongoing Tp Tx connection should not be interfered, the new request is rejected in this case */
      if ( tpCtrl->LinTp_Ch_State == LINTP_CHANNEL_IDLE )
      {
        /* #70 Process new Tp request */
        retVal = LinTp_TxProcessRequest(LinTpTxSduId, tpCtrl, LinTpTxInfoPtr->SduLength, txNad); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
    }
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    else
    {
      /* MISRA only */
    }
#  endif
# endif
  }

  SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();

  return retVal;
}


/**********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_PollBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
LINIF_LOCAL_INLINE FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_PollBuffer
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)   CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                     LinSduPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType                                             bufferReqRetVal; /* initialized below before being used */
  PduLengthType                                                 remainingPduRBufferSize = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Poll PduR for current buffer size */
  /* continue reception if data can be copied in the next call */
  CtrlPtr->LinTp_PduInfoPtr.SduLength         = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
  bufferReqRetVal = PduR_LinTpCopyRxData( CtrlPtr->LinTp_Curr_UL_NSdu_Id, &CtrlPtr->LinTp_PduInfoPtr, &remainingPduRBufferSize); /* SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_RX */
  CtrlPtr->LinTp_PduInfoPtr.SduLength         = CtrlPtr->LinTp_Tx_Rx_Buffer_Length; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

  /* #20 If PduR indicates sufficient buffer, continue normal reception handling */
  if ( (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK) && (remainingPduRBufferSize >= CtrlPtr->LinTp_Tx_Rx_Buffer_Length) )
  {
    /* buffer available, provide below the received data to PduR */
    CtrlPtr->LinTp_Ch_State = LINTP_CHANNEL_BUSY_RX; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
  }
  /* #30 Else if PduR indicates failure, abort reception */
  else if (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_E_NOT_OK)
  {
    LinTp_RxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
  }
  /* #40 Otherwise transit to suspended state if new Rx data has been received */
  else
  {
    /* Transit to LINTP_CHANNEL_BUSY_RX_SUSPENDED to flag that if the buffer cannot be provided until the next frame is received,
       the reception must be aborted. */
    if ( LinSduPtr != NULL_PTR )
    {
      CtrlPtr->LinTp_Ch_State = LINTP_CHANNEL_BUSY_RX_SUSPENDED; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }
  }
}

/**********************************************************************************************************************
 *
 * END - LinTp Initialization, Task and General Function Interface - Local Functions
 *
 *********************************************************************************************************************/
#endif


/**********************************************************************************************************************
 *  GLOBAL LINIF FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinIf Initialization, Task and General Function Interface - Service Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, LINIF_CODE) LinIf_InitMemory
(
  void
)
{
  /* ----- Implementation ----------------------------------------------- */

  /* #10 Set initialization detection flags for LinIf and LinTp to UNINIT */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  LinIf_Dev_InitDetect  = LINIF_UNINIT;

# if ( LINIF_TP_SUPPORTED == STD_ON )
  /* LIN TP set to uninit state after reset */
  LinTp_Dev_InitDetect  = LINTP_UNINIT;
# endif
#endif
}

/**********************************************************************************************************************
 *  LinIf_Init()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, LINIF_CODE) LinIf_Init
(
  P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_INIT_DATA)   ConfigPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                               errorId = LINIF_E_NO_ERROR;

#if ( LINIF_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  Rtm_Start(RtmConf_RtmMeasurementPoint_LinIf_Init);
#endif
  LINIF_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is already initialized */
  if ( LinIf_Dev_InitDetect == LINIF_INIT )
  {
    errorId = LINIF_E_ALREADY_INITIALIZED;
  }
  else
#endif
  {
#if ( LINIF_USE_INIT_POINTER == STD_ON )
    LinIf_ConfigDataPtr         = ConfigPtr;
    /* #20 Check validity of config pointer (only post-build) */
    if ( LinIf_ConfigDataPtr == (P2CONST(LinIf_ConfigType, AUTOMATIC, LINIF_INIT_DATA))NULL_PTR )
    {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_PARAMETER_POINTER;
# endif
# if ( LINIF_CONFIGURATION_VARIANT == LINIF_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_NULLPTR );
# endif
    }
# if ( LINIF_CONFIGURATION_VARIANT == LINIF_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    /* #30 Check generator version (only post-build loadable) */
    else if ( LinIf_GetGeneratorCompatibilityVersion() != (uint16) LINIF_GENERATOR_COMPATIBILITY_VERSION )
    {
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_COMPATIBILITYVERSION );
    }
    /* #40 Check magic number of config structure (only post-build loadable) */
    else if ( LinIf_GetFinalMagicNumber() != LINIF_FINAL_MAGIC_NUMBER )
    {
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER );
    }
# endif
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      LinIf_ChannelDataIterType linIfChannelIdx;

      /* #50 Set LinIf initialization flag to INIT */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      LinIf_Dev_InitDetect  = LINIF_INIT;
#endif

      /* #60 Initialize all channel dependent variables for all LinIf channels */
      for (linIfChannelIdx = 0x00u; linIfChannelIdx < LinIf_GetSizeOfChannelData(); linIfChannelIdx++) /* FETA_LINIF_UPPER_LIMIT_SIZEOF */
      {
        LinIf_GetChannelData(linIfChannelIdx).LinIf_ChannelState = LINIF_CHANNEL_UNINIT; /* SBSW_LINIF_CHANNEL_DATA_ACCESS */
        LinIf_ChannelInit((NetworkHandleType)linIfChannelIdx);
      }

      /* #70 Clear sporadic transmission flags of all sporadic slots (if sporadic frames supported) */
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
      if ( LinIf_GetSporUncFrmTotalNumber(0) != 0u ) /* COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL */
      {
        LinIf_SporTransmitFlagsIterType sporTxFlagIdx;
        /* sporadic frames exist in current configuration - clear flags */
        for (sporTxFlagIdx = 0u; sporTxFlagIdx < LinIf_GetSizeOfSporTransmitFlags(); sporTxFlagIdx++) /* FETA_LINIF_UPPER_LIMIT_SIZEOF */
        {
          LinIf_SetSporTransmitFlags(sporTxFlagIdx, 0u); /* SBSW_LINIF_SPOR_TXFLAGS_ACCESS_CSL */
        }
      }
#endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_INIT, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

#if ( LINIF_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  Rtm_Stop(RtmConf_RtmMeasurementPoint_LinIf_Init);
#endif
}



#if ( LINIF_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  LinIf_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LINIF_CODE) LinIf_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR) versioninfo
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                 errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter versioninfo. Note: no uninit check is performed */
  if ( versioninfo == (P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set versioninfo parameter with component information */
    versioninfo->vendorID           = (uint16)LINIF_VENDOR_ID; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->moduleID           = (uint16)LINIF_MODULE_ID; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_major_version   = (uint8)LINIF_SW_MAJOR_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_minor_version   = (uint8)LINIF_SW_MINOR_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_patch_version   = (uint8)LINIF_SW_PATCH_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GETVERSIONINFO, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif /* LINIF_VERSION_INFO_API == STD_ON */


/**********************************************************************************************************************
 *  LinIf_ChannelMainFunction()
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
FUNC(void, LINIF_CODE) LinIf_ChannelMainFunction
(
  VAR(NetworkHandleType, AUTOMATIC)                     Channel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                 errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
# if ( LINIF_TP_SUPPORTED == STD_ON )
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
# else
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
# endif
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter Channel */
  else if ( Channel >= LinIf_GetSizeOfSystemToLinIfChannel() )
  {
    errorId = LINIF_E_NONEXISTENT_CHANNEL;
  }
  else
#endif
  {
    /* #30 Check if channel is active */
    NetworkHandleType linIfChannelIdx = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);
    if ( linIfChannelIdx < LinIf_GetSizeOfChannelData() )
    {
      /* ----- Implementation ----------------------------------------------- */

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_0();

      /* #40 Call channel main function depending on channel node type */
#if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
      if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) == LinIf_ChannelNodeType_Master )
# endif
      {
        LinIf_MasterChannelMainFunction(linIfChannelIdx);
      }
#endif
#if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
      else
# endif
      {
        LinIf_SlaveChannelMainFunction(linIfChannelIdx);
      }
#endif


      /* #50 Update LinTp P2 timeout (only if LinTp is supported) */
#if ( LINIF_TP_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
      if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) == LinIf_ChannelNodeType_Master )
#  endif
      {
        P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT) tpCtrl = LinTp_GetAddrCtrl(linIfChannelIdx);
        if ( tpCtrl->LinTp_P2Timer > 0u )
        {
          tpCtrl->LinTp_P2Timer--; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
          if ( tpCtrl->LinTp_P2Timer == 0u )
          { /* timeout occurred - indicate release */
            tpCtrl->LinTp_PortNotification        = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
            tpCtrl->LinTp_ComModeRestore          = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
#  endif
            /* clear request NAD to prevent further reception */
            tpCtrl->LinTp_NAD_Current             = 0u; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
          }
        }
      }
# endif
#endif

      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_0();

      /* #60 Call post processing function (if necessary) for upper layer interactions */
      /**********************************************************
       * Postprocessing of main task
       **********************************************************/
#if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
# if ( LINIF_MAINFUNC_POSTPROCESSING_MANUAL_HANDLING == STD_ON )
      /* environment is responsible to call LinIf_MainFunctionPostProcessing_<LinIfChannel.ShortName>() */
# else
      /* inform upper layers after processing of all channels to reduce jitter */
      LinIf_ChannelMainFunctionPostProcessing(linIfChannelIdx);
# endif
#endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_MAINFUNCTION, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}

/**********************************************************************************************************************
 *  LinIf_EnableBusMirroring()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_EnableBusMirroring
(
  VAR(NetworkHandleType, AUTOMATIC) Channel,
  VAR(boolean, AUTOMATIC) MirroringActive
)
{
#if ( LINIF_BUSMIRRORING == STD_ON )
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                            retVal = E_NOT_OK;
  uint8                                                     errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(MirroringActive); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

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
    NetworkHandleType linIfChannelIdx = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() )
    {
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #30 Update bus mirroring mode for requested channel */
      P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtr = LinIf_GetAddrChannelData(linIfChannelIdx);

      SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();

      /* enable or disable mirroring */
      channelDataPtr->LinIf_MirroringActivationState = MirroringActive; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
      if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) == LinIf_ChannelNodeType_Master )
#  endif
      {
        /* clear a possibly pending mirror event. If mirroring is disabled, no further events shall be reported.
           If mirroring is going to be enabled, then the event pending flag is never set and following line has no effect */
        LinIf_GetMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(linIfChannelIdx)).LinIf_MirroringEventPending = FALSE; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
      }
# endif

      SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_ENABLE_BUSMIRRORING, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
#else

  LINIF_DUMMY_STATEMENT(Channel); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(MirroringActive); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return E_NOT_OK;
#endif
}

/**********************************************************************************************************************
 *
 * END - LinIf Initialization, Task and General Function Interface - Service Functions
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LinIf Transmission Handler
 *
 * The LinIf Transmission handler interface processes sporadic frame transmission requests.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_Transmit()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Transmit
(
  VAR(PduIdType, AUTOMATIC)                         LinTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)  PduInfoPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                    retVal = E_NOT_OK;
  uint8                                             errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(LinTxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter PduInfoPtr */
  else if ( PduInfoPtr == (P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  /* #30 Check validity of parameter LinTxPduId */
  else if ( LinTxPduId >= LinIf_GetUncTxFrmTotalNumber() )
  {
    errorId = LINIF_E_PARAMETER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Search all sporadic frames for the one with given LinTxPduId and set its transmission bit flag */
#if ( LINIF_SPORADIC_FRM_SUPPORTED == STD_ON )
    if ( LinIf_GetSporUncFrmTotalNumber(0) != 0u) /* COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL */
    {
      LinIf_SporUncFrmTotalNumberIterType sporUncFrmIdx;
      /* sporadic frames exist in current configuration */
      /* search all entries of SporadicFrameLookupTable for the entry with the given PduId */
      for (sporUncFrmIdx = 0u; sporUncFrmIdx < LinIf_GetSporUncFrmTotalNumber(0); sporUncFrmIdx++) /* FETA_LINIF_UPPER_LIMIT_IS_GENERATED_CONSTANT */
      {
        if ( LinIf_GetTxPduIdOfSporadicFrameLookupTable(sporUncFrmIdx) == LinTxPduId )
        {
          /* LinTxPduId matches - set transmit request flag */
          uint8 sporByteMsk = LinIf_GetByteMskOfSporadicFrameLookupTable(sporUncFrmIdx);
          uint8 sporBitMsk  = LinIf_GetBitMskOfSporadicFrameLookupTable(sporUncFrmIdx);
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
          if ( sporByteMsk >= LinIf_GetSizeOfSporTransmitFlags() ) /* defensive programming, condition will never be true in a valid configuration */ /* COV_LINIF_INV_STATE_ALWAYSFALSE */
          {
            errorId = LINIF_E_CONFIG;
          }
          else
# endif
          {
            LinIf_SetSporTransmitFlags( sporByteMsk, (LinIf_SporTransmitFlagsType)(LinIf_GetSporTransmitFlags(sporByteMsk) | sporBitMsk)); /* SBSW_LINIF_SPOR_TXFLAGS_ACCESS */
          }
          break; /* quit search */
        }
      }
    }
#endif

    /* #50 Set the return value to E_OK if LinTxPduId is valid */
    /* E_NOT_OK shall only be returned if LinIf is not initialized or referenced PDU does not exist (identification is out of range) */
    /* That means E_OK should also be returned for non-sporadic frames */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    retVal = E_OK;  /* TxPduId exists, already checked above */
#else
    if ( LinTxPduId < LinIf_GetUncTxFrmTotalNumber() )
    {
      /* Only return E_OK for existing PduIds, this is independent of DET check usage */
      retVal = E_OK;
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_TRANSMIT, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}


#if ( LINIF_CANCEL_TRANSMIT_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinIf_CancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CancelTransmit
(
  VAR(PduIdType, AUTOMATIC)                                 LinTxPduId
)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* request is always rejected by returning E_NOT_OK */
  Std_ReturnType                                            retVal = E_NOT_OK;
  uint8                                                     errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(LinTxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter LinTxPduId */
  else if ( LinTxPduId >= LinIf_GetUncTxFrmTotalNumber() )
  {
    errorId = LINIF_E_PARAMETER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* nothing to do */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_CANCELTRANSMIT, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}
#endif

/**********************************************************************************************************************
 *
 * END - LinIf Transmission Handler
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LinIf Node Management - Service Functions
 *
 * The LinIf node management handles the transition of a LIN channel between operational and sleep state.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinIf_GotoSleep()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GotoSleep
(
  VAR(NetworkHandleType, AUTOMATIC)                               Channel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                                  retVal = E_NOT_OK;
  uint8                                                           errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
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
#endif
  {
    NetworkHandleType linIfChannelIdx = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() )
    {
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtr = LinIf_GetAddrChannelData(linIfChannelIdx); /* PRQA S 3678 */ /* MD_LINIF_Rule8.13_3673_3678 */

      /* #25 If current channel is a LIN Master channel */
#if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
      if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) == LinIf_ChannelNodeType_Master )
# endif
      {
        P2VAR(LinIf_MasterChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) masterChannelDataPtr = LinIf_GetAddrMasterChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(linIfChannelIdx));

        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();

        /* #30 If channel state is operational */
        if ( channelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_OPERATIONAL )
        {
          /* #40 If no sleep transition is already ongoing, store gotosleep request as pending */
          if ( ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_Idle ) ||
               ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnWakeupFrameHandling_OnTrans ) )
          {
            /* Only set new request if not already on processing a request */
            masterChannelDataPtr->LinIf_OnSleepModeFrameHandling   = LinIf_OnSleepModeFrameHandling_OnReq; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
          }
        }
        /* #50 Otherwise if channel state is sleep */
        else if ( channelDataPtr->LinIf_ChannelState == LINIF_CHANNEL_SLEEP )
        {
          /* #60 If no sleep transition is already ongoing, store gotosleep request as pending and mark it as internal request */
          /* Actually we could call Lin_GotoSleepInternal here directly because in sleep state no communication is active.
             However LINIF597 explicitly states that Lin_GotoSleepInternal is called by LinIf_MainFunction_<LinIfChannel.ShortName>. */
          if ( ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnSleepModeFrameHandling_Idle ) ||
               ( masterChannelDataPtr->LinIf_OnSleepModeFrameHandling == LinIf_OnWakeupFrameHandling_OnTrans ) )
          {
            masterChannelDataPtr->LinIf_OnSleepModeFrameHandling   = LinIf_OnSleepModeFrameHandling_OnReqInt; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
          }
        }
        else
        {
          /* MISRA only */
        }

        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();
      }
#endif
      /* #70 Otherwise (current channel is a LIN Slave channel) */
#if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
      else
# endif
      {
        P2VAR(LinIf_SlaveChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) slaveChannelDataPtr = LinIf_GetAddrSlaveChannelData(LinIf_GetNodeChannelDataIdxOfLinIfChannel(linIfChannelIdx));

        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();

        /* #80 Request internal sleep mode transition of LIN driver */
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
        (void) LinIf_GetLin_GoToSleepInternalFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(linIfChannelIdx)) (LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelIdx)); /* SBSW_LINIF_FCT_POINTER_CALL */
# else
        (void) LinIf_Lin_GoToSleepInternal_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelIdx));
# endif

        /* #90 Clear wakeup flag */
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
        channelDataPtr->LinIf_WakeupFlag            = (uint8)0x00u; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
# endif

        /* #100 Set channel to SLEEP state */
        channelDataPtr->LinIf_ChannelState          = LINIF_CHANNEL_SLEEP; /* SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL */
        slaveChannelDataPtr->LinIf_BusIdleTimer     = 0u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
        slaveChannelDataPtr->LinIf_NmWakeupState    = LinIf_SlaveNmState_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
        slaveChannelDataPtr->LinIf_FrameState       = LinIf_SlaveFrameState_Idle; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# if ( LINIF_NC_GENERAL_SUPPORTED == STD_ON )
        /* clear possibly pending node configuration service response */
        slaveChannelDataPtr->LinIf_NcRespBuffer[0]  = 0x00u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
        slaveChannelDataPtr->LinIf_NasTimer         = 0u; /* SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR */
# endif

        /* #110 Confirm sleep mode transition to upper layer */
# if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
        LinIf_GetGotoSleepConfFctList(LinIf_GetGotoSleepConfFctListIdxOfChannelConfig(linIfChannelIdx))( /* SBSW_LINIF_FCT_POINTER_CALL */
                        LinIf_GetSystemChannelIndexOfChannelIndTable(linIfChannelIdx), TRUE);
# else
        LinSM_GotoSleepConfirmation(LinIf_GetSystemChannelIndexOfChannelIndTable(linIfChannelIdx), TRUE);
# endif

        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();
      }
#endif

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GOTOSLEEP, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  LinIf_Wakeup()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Wakeup
(
  VAR(NetworkHandleType, AUTOMATIC)                         Channel
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                            retVal = E_NOT_OK;
  uint8                                                     errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
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
#endif
  {
    NetworkHandleType linIfChannelIdx   = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() )
    {
      errorId = LINIF_E_NONEXISTENT_CHANNEL;
    }
    else
#endif
    {
      /* ----- Implementation ----------------------------------------------- */
      P2VAR(LinIf_ChannelDataType, AUTOMATIC, LINIF_VAR_NOINIT) channelDataPtr = LinIf_GetAddrChannelData(linIfChannelIdx);

      /* #30 Invoke internal wakeup function depending in node type of current channel */
#if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
      if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) == LinIf_ChannelNodeType_Master )
# endif
      {
        /* Master channel */
        retVal = LinIf_MasterWakeup(Channel, linIfChannelIdx, channelDataPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
#endif
#if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
      else
# endif
      {
        /* Slave channel */
        retVal = LinIf_SlaveWakeup(Channel, linIfChannelIdx, channelDataPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
#endif
    }
  }

#if ( LINIF_DEV_ERROR_DETECT == STD_ON )
# if ( LINIF_WAKEUPCONF_AR403_COMPATIBILITY == STD_ON )
  /* no wakeup confirmation called if E_NOT_OK is returned */
# else
  if ( retVal != E_OK)
  {
    /* according to AR4-612, inform LinSM with parameter FALSE if wakeup is not accepted */
#  if ( LINIF_LINSM_ONLY_UL_OPTIMIZATION == STD_OFF )
    /* possible invalid channel parameter, not possible to select valid configured upper layer */
#  else
    /* FALSE returned because wakeup is accepted according to AR4-612 (AR4.1.x) */
    LinSM_WakeupConfirmation(Channel, FALSE);
#  endif
  }
# endif
#endif

  /* ----- Development Error Report --------------------------------------- */
#if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_WAKEUP, errorId);
  }
#else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}


#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  LinIf_CheckWakeup()
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CheckWakeup
(
  VAR(EcuM_WakeupSourceType, AUTOMATIC)                 WakeupSource
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                        retVal = E_NOT_OK;
  uint8                                                 errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(WakeupSource); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least linIfChannelLoopIdx; /* initialized below in for loop */
    /* #20 Check for each LinIf channel if given wakeup source is configured for this channel */
    for (linIfChannelLoopIdx = 0u; linIfChannelLoopIdx < (uint8)LinIf_GetSizeOfChannelData(); linIfChannelLoopIdx++) /* FETA_LINIF_UPPER_LIMIT_SIZEOF */
    {
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON )
      /* #30 If wakeup source belongs to LIN driver module, call Lin_CheckWakeup for the identified channel */
      if ( (LinIf_GetLinWakeupSourceOfChannelToDrvWakeupInfo(linIfChannelLoopIdx) & WakeupSource) != (EcuM_WakeupSourceType)0x00u)
      {
        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();
#  if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
        (void) LinIf_GetLin_CheckWakeupFctOfLinInstFctTable(LinIf_GetLinInstFctTableIdxOfChannelConfig(linIfChannelLoopIdx)) (LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelLoopIdx)); /* SBSW_LINIF_FCT_POINTER_CALL */
#  else
        (void) LinIf_Lin_CheckWakeup_FctCall(LinIf_GetLinChannelIndexOfChannelIndTable(linIfChannelLoopIdx));
#  endif
        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();
        retVal = E_OK;
      }
# endif

# if ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
      /* #40 If wakeup source belongs to a LIN transceiver, call LinTrcv_CheckWakeup for the identified transceiver and channel */
      if ( (LinIf_GetLinTrcvWakeupSourceOfChannelToTrcvWakeupInfo(linIfChannelLoopIdx) & WakeupSource) != (EcuM_WakeupSourceType)0x00u )
      {
        /* LIN Transceiver Driver reports a woken up channel with EcuM_SetWakeupEvent() */
        uint8 trcvChannelIdx = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelLoopIdx);
        /* at this point the transceiver channel index cannot be invalid because a wakeup source is assigned to it, no need for check of LinIf_InvalidLinTrcv */
        uint8 trcvInstanceNr = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelLoopIdx);

        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();
        (void) LinIf_GetLinTrcv_CheckWakeupFct(trcvInstanceNr)(trcvChannelIdx); /* SBSW_LINIF_FCT_POINTER_CALL */
        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();
        retVal = E_OK;
      }
# endif
    }

    /* #50 If wakeup source does not belong to any Lin channel: report error */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if (retVal != E_OK)
    {
      errorId = LINIF_E_PARAM_WAKEUPSOURCE;
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_CHECKWAKEUP, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}
#endif

/**********************************************************************************************************************
 *  LinIf_WakeupConfirmation()
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
FUNC(void, LINIF_CODE) LinIf_WakeupConfirmation
(
  VAR(EcuM_WakeupSourceType, AUTOMATIC)                 WakeupSource
)
{
#if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                 errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least linIfChannelLoopIdx; /* initialized below in for loop */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
    uint8 wakeupsourceFound = 0u;
# endif

    /* #20 Check for each LinIf channel if given wakeup source is configured for this channel */
    for (linIfChannelLoopIdx = 0u; linIfChannelLoopIdx < (uint8)LinIf_GetSizeOfChannelData(); linIfChannelLoopIdx++) /* FETA_LINIF_UPPER_LIMIT_SIZEOF */
    {
      /* #30 If wakeup source belongs to a Lin (driver) channel, set the external wakeup flag for the identified channel */
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON )
      if ( (LinIf_GetLinWakeupSourceOfChannelToDrvWakeupInfo(linIfChannelLoopIdx) & WakeupSource) != (EcuM_WakeupSourceType)0x00u)
      {
        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_2();
        LinIf_GetChannelData(linIfChannelLoopIdx).LinIf_WakeupFlag = (uint8)0x01u; /* SBSW_LINIF_CHANNEL_DATA_ACCESS */
        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_2();
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
        wakeupsourceFound = 1u;
#  endif
      }
# endif

# if ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
      /* #40 If wakeup source belongs to a Lin transceiver channel, set the external wakeup flag for the identified channel */
      if ( (LinIf_GetLinTrcvWakeupSourceOfChannelToTrcvWakeupInfo(linIfChannelLoopIdx) & WakeupSource) != (EcuM_WakeupSourceType)0x00u)
      {
        SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_2();
        LinIf_GetChannelData(linIfChannelLoopIdx).LinIf_WakeupFlag = (uint8)0x01u; /* SBSW_LINIF_CHANNEL_DATA_ACCESS */
        SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_2();
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
        wakeupsourceFound = 1u;
#  endif
      }
# endif
    }

    /* #50 If wakeup source does not belong to any Lin channel: report error */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
    if (wakeupsourceFound == 0u)
    {
      errorId = LINIF_E_PARAM_WAKEUPSOURCE;
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_WAKEUPCONFIRMATION, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
#else
  LINIF_DUMMY_STATEMENT(WakeupSource); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}

/**********************************************************************************************************************
 *
 * END - LinIf Node Management
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LinIf Transceiver Handler
 *
 * Handles the interface to one or more LIN transceivers.
 *********************************************************************************************************************/
#if ( LINIF_TRCV_HANDLING == STD_ON )

/**********************************************************************************************************************
 *  LinIf_SetTrcvMode()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvMode
(
  VAR(NetworkHandleType, AUTOMATIC)         Channel,
  VAR(LinTrcv_TrcvModeType, AUTOMATIC)      TransceiverMode
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                     errorId = LINIF_E_NO_ERROR;
  Std_ReturnType                            retVal = E_NOT_OK;

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
    errorId = LINIF_E_TRCV_INV_CHANNEL;
  }
  /* #30 Check validity of parameter TransceiverMode */
  else if ( (TransceiverMode != LINTRCV_TRCV_MODE_NORMAL) && (TransceiverMode != LINTRCV_TRCV_MODE_SLEEP) && (TransceiverMode != LINTRCV_TRCV_MODE_STANDBY) )
  { /* PRQA S 2880 */ /* MD_LINIF_Rule2.1_2880 */
    errorId = LINIF_E_TRCV_INV_MODE;
  }
  else
# endif
  {
    NetworkHandleType linIfChannelIdx = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() ) /* COV_LINIF_TRCV */
    {
      errorId = LINIF_E_TRCV_INV_CHANNEL;
    }
    else
# endif
    {
      /* #40 Check if transceiver instance exists on given channel */
      uint8 trcvChannelIdx = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelIdx);

      if ( trcvChannelIdx == LinIf_InvalidLinTrcv ) /* COV_LINIF_TRCV */
      {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
      }
      else
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #50 Get transceiver instance and set given mode */
        uint8 trcvInstanceNr = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelIdx);
        retVal               = LinIf_GetLinTrcv_SetOpModeFct(trcvInstanceNr)(trcvChannelIdx, TransceiverMode); /* SBSW_LINIF_FCT_POINTER_CALL */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_SETTRCVMODE, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  LinIf_GetTrcvMode()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvMode
(
  VAR(NetworkHandleType, AUTOMATIC)                           Channel,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_VAR)      TransceiverModePtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                       errorId = LINIF_E_NO_ERROR;
  Std_ReturnType                                              retVal = E_NOT_OK;

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
    errorId = LINIF_E_TRCV_INV_CHANNEL;
  }
  /* #30 Check validity of parameter TransceiverModePtr */
  else if ( TransceiverModePtr == (P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
# endif
  {
    NetworkHandleType linIfChannelIdx = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() ) /* COV_LINIF_TRCV */
    {
      errorId = LINIF_E_TRCV_INV_CHANNEL;
    }
    else
# endif
    {
      /* #40 Check if transceiver instance exists on given channel */
      uint8 trcvChannelIdx = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelIdx);

      if ( trcvChannelIdx == LinIf_InvalidLinTrcv ) /* COV_LINIF_TRCV */
      {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
      }
      else
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #50 Get transceiver instance and retrieve transceiver mode */
        uint8 trcvInstanceNr = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelIdx);
        retVal               = LinIf_GetLinTrcv_GetOpModeFct(trcvInstanceNr)(trcvChannelIdx, TransceiverModePtr); /* SBSW_LINIF_FCT_POINTER_CALL */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GETTRCVMODE, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}


/**********************************************************************************************************************
 *  LinIf_GetTrcvWakeupReason()
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
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GetTrcvWakeupReason
(
  VAR(NetworkHandleType, AUTOMATIC)                               Channel,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_VAR)  TrcvWuReasonPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                                           errorId = LINIF_E_NO_ERROR;
  Std_ReturnType                                                  retVal = E_NOT_OK;

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
    errorId = LINIF_E_TRCV_INV_CHANNEL;
  }
  /* #30 Check validity of parameter TrcvWuReasonPtr */
  else if ( TrcvWuReasonPtr == (P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
# endif
  {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    /* #40 Check if transceiver is in normal mode */
    LinTrcv_TrcvModeType transceiverModePtr = LINTRCV_TRCV_MODE_NORMAL;
    Std_ReturnType retTrcvMode = LinIf_GetTrcvMode(Channel, &transceiverModePtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    if ( (retTrcvMode == E_OK) && (transceiverModePtr != LINTRCV_TRCV_MODE_NORMAL) )
    {
      errorId = LINIF_E_TRCV_NOT_NORMAL;
    }
    else
# endif
    {
      NetworkHandleType linIfChannelIdx = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() ) /* COV_LINIF_TRCV */
      {
        errorId = LINIF_E_TRCV_INV_CHANNEL;
      }
      else
# endif
      {
        /* #50 Check if transceiver instance exists on given channel */
        uint8 trcvChannelIdx = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelIdx);

        if ( trcvChannelIdx == LinIf_InvalidLinTrcv ) /* COV_LINIF_TRCV */
        {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
          errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
        }
        else
        {
          /* ----- Implementation ----------------------------------------------- */
          /* #60 Get transceiver instance and retrieve wakeup reason */
          uint8 trcvInstanceNr = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelIdx);
          retVal               = LinIf_GetLinTrcv_GetBusWuReasonFct(trcvInstanceNr)(trcvChannelIdx, TrcvWuReasonPtr); /* SBSW_LINIF_FCT_POINTER_CALL */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_GETTRCVWAKEUPREASON, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  LinIf_SetTrcvWakeupMode()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_SetTrcvWakeupMode
(
  VAR(NetworkHandleType, AUTOMATIC)           Channel,
  VAR(LinTrcv_TrcvWakeupModeType, AUTOMATIC)  LinTrcvWakeupMode
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                       errorId = LINIF_E_NO_ERROR;
  Std_ReturnType                              retVal = E_NOT_OK;

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
    errorId = LINIF_E_TRCV_INV_CHANNEL;
  }
  /* #30 Check validity of parameter LinTrcvWakeupMode */
  else if ( !((LinTrcvWakeupMode == LINTRCV_WUMODE_ENABLE) || (LinTrcvWakeupMode == LINTRCV_WUMODE_DISABLE) || (LinTrcvWakeupMode == LINTRCV_WUMODE_CLEAR)) )
  { /* PRQA S 2880 */ /* MD_LINIF_Rule2.1_2880 */
    errorId = LINIF_E_PARAMETER;
  }
  else
# endif
  {
    NetworkHandleType linIfChannelIdx = LinIf_GetLinIfChannelIndexOfSystemToLinIfChannel(Channel);

# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    if ( linIfChannelIdx >= LinIf_GetSizeOfChannelData() ) /* COV_LINIF_TRCV */
    {
      errorId = LINIF_E_TRCV_INV_CHANNEL;
    }
    else
# endif
    {
      /* #40 Check if transceiver instance exists on given channel */
      uint8 trcvChannelIdx = LinIf_GetTrcvChannelOfLinIfToLinTrcvChannel(linIfChannelIdx);

      if ( trcvChannelIdx == LinIf_InvalidLinTrcv ) /* COV_LINIF_TRCV */
      {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        errorId = LINIF_E_TRCV_INV_CHANNEL;
# endif
      }
      else
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #50 Get transceiver instance and set wakeup mode */
        uint8 trcvInstanceNr = LinIf_GetTrcvNrOfLinIfToLinTrcvNr(linIfChannelIdx);
        retVal               = LinIf_GetLinTrcv_SetWakeupModeFct(trcvInstanceNr)(trcvChannelIdx, LinTrcvWakeupMode); /* SBSW_LINIF_FCT_POINTER_CALL */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINIF_SID_SETTRCVWAKEUPMODE, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}

#endif /* LINIF_TRCV_HANDLING == STD_ON */

/**********************************************************************************************************************
 *
 * END - LinIf Transceiver Handler
 *
 *********************************************************************************************************************/









/**********************************************************************************************************************
 *
 * LIN TRANSPORT PROTOCOL
 *
 * Description:  Implementation of the AUTOSAR LIN Transport Protocol (part of LIN Interface)
 *
 *********************************************************************************************************************/

#if ( LINIF_TP_SUPPORTED == STD_ON )

/**********************************************************************************************************************
 *  LOCAL LINTP FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinTp Buffer Handling - Local Functions
 *
 * Handling of transmission / reception data and buffer.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_BufferPolling()
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
FUNC(void, LINIF_CODE) LinTp_BufferPolling
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)  CtrlPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If TX connection active but not sufficient data for transmission available */
  if ( CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_TX_INVALID)
  {
    /* #20 Request transmission data from PduR */
    retVal = LinTp_TxDataRequest(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */

    /* #30 If PduR request is denied, abort TX connection */
    if ( retVal != E_OK )
    { /* unable to obtain data from PduR - permanent failure */
      LinTp_TxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
  /* #40 Otherwise if RX connection active but not sufficient reception buffer previously indicated by PduR */
  /* Master: Poll for buffer in general suspended state and while waiting for start of reception. Second case occurs when a new reception is
     started but the notification flag is already occupied, e.g. due to an abort connection to be reported.
     The poll-buffer state is excluded by default: In this state, the buffer is polled once after reception of next slave response */
  /* Slave: Poll for buffer in general suspended state and during poll-buffer state (suspended and local buffer already occupied).
      The waiting for start of reception is excluded by default as it cannot happen on a slave channel. Either the reception start is accepted
      or denied, no delayed notification is used */
# if ( LINIF_NODETYPE_MASTER_ONLY == STD_ON )
  if ( (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED ) || (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START) )
# elif ( LINIF_NODETYPE_SLAVE_ONLY == STD_ON )
  if ( (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED) || (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_POLLBUFFER) )
# else
  if ( ( (CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Master) &&
         ((CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED ) || (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START)) ) ||
       ( (CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Slave) &&
         ((CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED) || (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_POLLBUFFER)) ) )
# endif
  {
    /* #50 Poll PduR again if receive buffer is available */
    /* provide NULL pointer as second argument as the buffer handling is performed on existing local buffer */
    LinTp_RxDataBufferHandling_Main(CtrlPtr, NULL_PTR); /* SBSW_LINIF_TP_RXDATABUFHANDLING_CALL */
  }
  else
  {
    /* No TP connection active or data available, nothing to do */
  }
}

/**********************************************************************************************************************
 *
 *  LinTp Buffer Handling - Rx
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_StartOfReception()
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
FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_StartOfReception
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT) CtrlPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType                                           bufferReqRetVal; /* initialized below before being used */
  PduLengthType                                               remainingPduRBufferSize = 0u;
  P2VAR(PduInfoType, AUTOMATIC, LINIF_VAR_NOINIT)             pduInfoPtr = NULL_PTR;
# if ( LINTP_META_DATA_SUPPORTED == STD_ON )
  PduInfoType                                                 pduInfo; /* initialized below before being used */
  uint8                                                       metaData[1] = { 0u };
# endif

  /* ----- Implementation ----------------------------------------------- */
# if ( LINTP_META_DATA_SUPPORTED == STD_ON )
  /* #10 If meta data handling is used for the current NSdu */
  if ( LinTp_IsMetaDataSupportedOfRxNSdu(CtrlPtr->LinTp_Curr_Rx_Id) )
  {
    /* #20 Fill pdu info structure with length of meta data and received NAD */
    pduInfo.SduLength = 1u;
    metaData[0] = CtrlPtr->LinTp_NAD_Current; /* SBSW_LINIF_TP_META_DATA_ARRAY_LOCAL */
    pduInfo.SduDataPtr = &(metaData[0]);
    pduInfoPtr = &pduInfo;
  }
# endif

  /* #30 Start actual reception handling with PduR */
  /* Pass meta data as info pointer parameter and the NULL_PTR in case of no meta data */
  bufferReqRetVal = PduR_LinTpStartOfReception(CtrlPtr->LinTp_Curr_UL_NSdu_Id, pduInfoPtr, CtrlPtr->LinTp_RemainingLength, &remainingPduRBufferSize); /* SBSW_LINIF_TP_PDUR_FCT_CALL_START_RX */

  /* #40 If request is accepted and available buffer size sufficient, proceed */
  if ( (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK) && (remainingPduRBufferSize >= CtrlPtr->LinTp_Tx_Rx_Buffer_Length ) )
  {
    /* sufficient buffer can be provided for first frame, just proceed */
    CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_BUSY_RX; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
  }
  /* #50 Otherwise: */
  else
  {
    /* #60 Close active TP connection and inform upper layer if necessary */
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
    if ( CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Master )
#  endif
    {
      /* Master channel */
      /* Reset TP connection */
      CtrlPtr->LinTp_Timer                  = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_NAD_Current            = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      if ( CtrlPtr->LinTp_PortNotification == LinTp_PortNotificationIdle ) /* COV_LINTP_FUNCREQUEST_SRF */
      { /* No new functional request pending */
        CtrlPtr->LinTp_PortNotification     = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
      CtrlPtr->LinTp_ComModeRestore         = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  endif
      /* clear P2 timer */
      CtrlPtr->LinTp_P2Timer                = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

      /* Report termination of reception if request was not rejected but provided buffer is too small */
      if ( (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK) || (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_E_BUSY) )
      {
        /* report abort of reception to PduR, e.g. if the provided buffer is too small.
            Actually this should never happen - would be a configuration fault in PduR */
        CtrlPtr->LinTp_PduRNotification    |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNotifyResultRx   = E_NOT_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNSduIdRx         = CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }
      /* else: nothing to report to PduR, no connection was established */
    }
# endif
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
    else
#  endif
    {
      /* Slave channel */
      /* Reset TP connection */
      CtrlPtr->LinTp_Timer                  = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

      /* Report termination of reception if request was not rejected but provided buffer is too small */
      if ( (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK) || (bufferReqRetVal == (BufReq_ReturnType)BUFREQ_E_BUSY) )
      {
        /* report abort of reception to PduR, e.g. if the provided buffer is too small.
            Actually this should never happen - would be a configuration fault in PduR */
        PduR_LinTpRxIndication(CtrlPtr->LinTp_Curr_UL_NSdu_Id, E_NOT_OK);
      }
      /* else: nothing to report to PduR, no connection was established */
    }
# endif
  }
}

/**********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_CopyRxData()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_CopyRxData
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT) CtrlPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType                                           bufferReqRetVal; /* initialized below before being used */
  PduLengthType                                               remainingPduRBufferSize = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request PduR to copy provided reception data */
  bufferReqRetVal = PduR_LinTpCopyRxData( CtrlPtr->LinTp_Curr_UL_NSdu_Id, &CtrlPtr->LinTp_PduInfoPtr, &remainingPduRBufferSize); /* SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_RX */

  /* #20 If request is accepted */
  if ( bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK )
  {
    /* Rx data copied by PduR */
    /* #30 If all expected data of whole message is received
     *     Close TP connection and inform PduR about completed successful reception */
    CtrlPtr->LinTp_RemainingLength            = (PduLengthType)(CtrlPtr->LinTp_RemainingLength - CtrlPtr->LinTp_PduInfoPtr.SduLength); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    if (CtrlPtr->LinTp_RemainingLength == 0u)
    {
      CtrlPtr->LinTp_Timer                    = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_Ch_State                 = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
      if ( CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Master )
#  endif
      {
        /* Master channel */
        CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNotifyResultRx     = E_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        CtrlPtr->LinTp_PduRNSduIdRx           = CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

        /* reset requested NAD and change schedule only if previous received frame was not a response pending */
#  if ( LINTP_FORWARD_RESPONSEPENDING_TO_PDUR == STD_ON )
        if (CtrlPtr->LinTp_RespPendingCounter == 0u)
#  endif
        {
          /* no response pending frame */
          CtrlPtr->LinTp_NAD_Current          = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

          if ( CtrlPtr->LinTp_PortNotification == LinTp_PortNotificationIdle ) /* COV_LINTP_FUNCREQUEST_SRF */
          { /* No new functional request pending */
            CtrlPtr->LinTp_PortNotification   = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
          }
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
          CtrlPtr->LinTp_ComModeRestore       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  endif
          /* clear P2 timer */
          CtrlPtr->LinTp_P2Timer              = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
        }
      }
# endif
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
      else
#  endif
      {
        /* Slave channel */
        PduR_LinTpRxIndication(CtrlPtr->LinTp_Curr_UL_NSdu_Id, E_OK);
      }
# endif
    }
    /* #50 Otherwise: more frames expected */
    else
    {
      /* #60 Calculate payload size of next frame */
      PduLengthType succFramePayloadSize; /* initialized below before being used */
      if ( CtrlPtr->LinTp_RemainingLength < 6u )
      {
        succFramePayloadSize                = CtrlPtr->LinTp_RemainingLength;
      }
      else
      {
        succFramePayloadSize                = 6u;
      }

      /* #70 If next frame fits in remaining PduR buffer just proceed, otherwise enter pending state (poll buffer, no SRF header will be transmitted) */
      if ( remainingPduRBufferSize >= succFramePayloadSize)
      {
        /* next frame can be stored, just proceed */
        CtrlPtr->LinTp_Ch_State             = LINTP_CHANNEL_BUSY_RX; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }
      else
      {
        /* next frame cannot be stored, however local buffer is empty in any case, so schedule next header */
        /* This case is only entered when a FF or CF was handled, received data was successfully given to PduR
            but the remaining buffer size is too small to store the following CF. For efficiency, the next CF
            header is scheduled anyway but remember to check PduR for reception buffer:
            - either there will be enough free buffer available so the handling is as usual (but no header
              transmission delay was introduced!)
            - or still not sufficient buffer available, then transit to LINTP_CHANNEL_BUSY_RX_SUSPENDED state */
        CtrlPtr->LinTp_Ch_State           = LINTP_CHANNEL_BUSY_RX_POLLBUFFER; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }
    }
  }
  /* #80 Otherwise: */
  else
  {
    /* #90 Quit reception and report abortion to PduR */
    LinTp_RxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
  }
}

/**********************************************************************************************************************
 *  LinTp_RxDataBufferHandling_Main()
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
/* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, LINIF_CODE) LinTp_RxDataBufferHandling_Main
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT) CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                   LinSduPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a PduR notification is already pending in this cycle (master channel only) */
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  if ( ((CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Master) && ((CtrlPtr->LinTp_PduRNotification & LinTp_PduRNotificationRx) == LinTp_PduRNotificationIdle)) ||
        (CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Slave) )
#  else
  if ( (CtrlPtr->LinTp_PduRNotification & LinTp_PduRNotificationRx) == LinTp_PduRNotificationIdle )
#  endif
# endif
  { /* no PduR event to notify in this cycle, proceed with normal handling */

    /* #20 If reception handling to PduR not yet started, initiate new Tp connection */
    if (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START)
    {
      /* actual start of PduR reception handling */
      LinTp_RxDataBufferHandling_StartOfReception(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }

    /* #30 If temporarily no sufficient receive buffer is available */
    if ( (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED) || (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_POLLBUFFER) )
    {
      /* #40 If new frame is received and local buffer is free, store reception data locally and continue polling for buffer */
      /* Master: Check for LinSduPtr != NULL_PTR is sufficient. If this is the case, the state is always poll-buffer - Reason:
         In suspended state, no SRF header are scheduled anymore, so LinSduPtr is always NULL because cyclic buffer polling
         on task level on a LIN master channel is only performed in suspended state with LinSduPtr equal to NULL. A new SRF
         response can only be received in poll-buffer state. */
      /* Slave: Additional check for poll-buffer state necessary because location is reached also in suspended state, when
         new MRF is received in state poll-buffer - however, then the local buffer is alreay occupied and reception handling
         is aborted (see below next check). Received data can only be stored into local buffer in poll-buffer state */
# if ( LINIF_NODETYPE_MASTER_ONLY == STD_ON )
      if ( LinSduPtr != NULL_PTR )
# elif ( LINIF_NODETYPE_SLAVE_ONLY == STD_ON )
      if ( (LinSduPtr != NULL_PTR) && (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_POLLBUFFER) )
# else
      if ( ( (CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Master) && ( LinSduPtr != NULL_PTR ) ) ||
           ( (CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Slave) && (LinSduPtr != NULL_PTR) && (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_POLLBUFFER) ) )
# endif
      {
        uint8_least byteIndex;
        for (byteIndex = 0u; byteIndex < (uint8)(CtrlPtr->LinTp_Tx_Rx_Buffer_Length); byteIndex++) /* FETA_LINIF_CORRECT_USAGE_OF_INTERFACE */
        {
          CtrlPtr->LinTp_Tx_Rx_Buffer[byteIndex] = LinSduPtr[(uint8)(CtrlPtr->LinTp_Tx_Rx_Buffer_Idx + byteIndex) & LINTP_LOCAL_BUFFER_SIZE_MASK]; /* SBSW_LINIF_TP_CTRL_BUFFER_ARRAY */
        }
      }

      /* #50 Else if new frame is received and local buffer is not available, abort active Tp connection and inform PduR (slave channel only) */
      /* Following condition cannot happen on master channels: LinSduPtr is always NULL_PTR in suspended state because in this state no new
         SRF response is received because no SRF header are scheduled */
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
      if ( (CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Slave) &&
           (LinSduPtr != NULL_PTR) && (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED) )
#  else
      if ( (LinSduPtr != NULL_PTR) && (CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX_SUSPENDED) )
#  endif
      {
        /* New frame received in LINTP_CHANNEL_BUSY_RX_SUSPENDED state. Frame cannot be stored as already the previous frame is pending
           to be indicated to PduR due to unavailable buffer and currently occupies the local buffer. In this case the reception must be aborted */
        LinTp_RxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
      /* #60 Otherwise poll PduR for current buffer size */
      else
# endif
      {
        LinTp_RxDataBufferHandling_PollBuffer(CtrlPtr, LinSduPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
      }
    }

    /* #70 If receive buffer available, provide received data to PduR */
    if ( CtrlPtr->LinTp_Ch_State == LINTP_CHANNEL_BUSY_RX )
    {
      /* Valid state, provide receive data to upper layer. If on retry handling, provide the local buffer with Tp data,
         otherwise provide the data of the newly received frame */
      if ( LinSduPtr == NULL_PTR )
      {
        CtrlPtr->LinTp_PduInfoPtr.SduDataPtr = CtrlPtr->LinTp_Tx_Rx_Buffer; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }
      else
      {
        CtrlPtr->LinTp_PduInfoPtr.SduDataPtr = (uint8*)&LinSduPtr[CtrlPtr->LinTp_Tx_Rx_Buffer_Idx]; /* PRQA S 0311 */ /* MD_LINIF_Rule11.8_0311 */ /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }
      CtrlPtr->LinTp_PduInfoPtr.SduLength  = CtrlPtr->LinTp_Tx_Rx_Buffer_Length; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      LinTp_RxDataBufferHandling_CopyRxData(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  /* #80 Otherwise: PduR notification pending, postpone buffer handling till next cycle, copy newly received data to local buffer */
  else
  {
    /* postpone buffer handling till next cycle in order to not overwrite post notification */
    if ( LinSduPtr != NULL_PTR )
    {
      uint8_least byteIndex;

      /* prepare pdu info structure, copy received data of SF or FF to local buffer */
      CtrlPtr->LinTp_PduInfoPtr.SduLength = CtrlPtr->LinTp_Tx_Rx_Buffer_Length; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_PduInfoPtr.SduDataPtr = &CtrlPtr->LinTp_Tx_Rx_Buffer[0]; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      for (byteIndex = 0u; byteIndex < (uint8)(CtrlPtr->LinTp_Tx_Rx_Buffer_Length); byteIndex++) /* FETA_LINIF_CORRECT_USAGE_OF_INTERFACE */
      {
        CtrlPtr->LinTp_Tx_Rx_Buffer[byteIndex] = LinSduPtr[(uint8)(CtrlPtr->LinTp_Tx_Rx_Buffer_Idx + byteIndex) & LINTP_LOCAL_BUFFER_SIZE_MASK]; /* SBSW_LINIF_TP_CTRL_BUFFER_ARRAY */
      }
    }
  }
# endif
}



/**********************************************************************************************************************
 *
 *  LinTp Buffer Handling - TX
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_TxDataRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_TxDataRequest
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)  CtrlPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType                                           bufferReqRetVal; /* initialized below before being used */
  Std_ReturnType                                              retVal = E_OK; /* request is accepted by default */
  PduLengthType                                               remainingPduRBufferSize = 0u;  /* dummy variable for call, not used till now */

  /* ----- Implementation ----------------------------------------------- */
  CtrlPtr->LinTp_Ch_State                           = LINTP_CHANNEL_BUSY_TX_INVALID; /* SBSW_LINIF_PASSED_PTR_ACCESS */

  /* #10 Request PduR to copy transmission data into provided local Tp buffer */
  bufferReqRetVal = PduR_LinTpCopyTxData(CtrlPtr->LinTp_Curr_UL_NSdu_Id, &(CtrlPtr->LinTp_PduInfoPtr), NULL_PTR, &remainingPduRBufferSize); /* SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_TX */

  /* #20 If transmission data is successfully provided */
  if ( bufferReqRetVal == (BufReq_ReturnType)BUFREQ_OK )
  { /* all requested data was copied into local buffer */

    /* #30 In case it's the last frame in current connection, stuff frame will fill bytes */
    CtrlPtr->LinTp_RemainingLength                  = (PduLengthType)(CtrlPtr->LinTp_RemainingLength - CtrlPtr->LinTp_PduInfoPtr.SduLength); /* SBSW_LINIF_PASSED_PTR_ACCESS */
    if (CtrlPtr->LinTp_RemainingLength == 0u)
    {
      /* SF or last CF - check if stuffing is necessary */
      if ((CtrlPtr->LinTp_Tx_Rx_Buffer_Idx + CtrlPtr->LinTp_PduInfoPtr.SduLength) < 8u)
      {
        /* stuff with fill bytes */
        uint8 byteIndex;
        for (byteIndex = (uint8)(CtrlPtr->LinTp_Tx_Rx_Buffer_Idx + CtrlPtr->LinTp_PduInfoPtr.SduLength); byteIndex < 8u; byteIndex++)
        {
          CtrlPtr->LinTp_Tx_Rx_Buffer[byteIndex]    = 0xFFu; /* SBSW_LINIF_PASSED_PTR_ACCESS */
        }
      }
      CtrlPtr->LinTp_RemainingLength                = 0u; /* SBSW_LINIF_PASSED_PTR_ACCESS */
    }
    /* else: more frames to come */

    CtrlPtr->LinTp_Ch_State = LINTP_CHANNEL_BUSY_TX_VALID; /* SBSW_LINIF_PASSED_PTR_ACCESS */
  }
  /* #40 Otherwise if PduR does not accept request, abort or delay transmission */
  else
  {
    if ( bufferReqRetVal == (BufReq_ReturnType)BUFREQ_E_BUSY )
    {
      /* retry in next main cycle, return E_OK to not abort request */
    }
    else /* (BufReq_ReturnType)BUFREQ_E_NOT_OK ) */
    {
      /* abort transmission */
      retVal = E_NOT_OK;
    }
  }

  return(retVal);
}

/**********************************************************************************************************************
 *
 * END - LinTp Buffer Handling
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * LinTp Reception Handling - Local Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_RxIndication_CF()
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
FUNC(void, LINIF_CODE) LinTp_RxIndication_CF
(
  CONSTP2VAR(LinTp_ControlType,  AUTOMATIC, LINIF_VAR_NOINIT)   CtrlPtr,
  P2CONST(uint8, AUTOMATIC, LINIF_APPL_VAR)                     LinSduPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if consecutive frame is expected */
  if (CtrlPtr->LinTp_RemainingLength > 0u)
  { /* CF awaited, note that NAD is already checked */

    /* #20 Check if sequence number is correct */
    if ( CtrlPtr->LinTp_SN == (uint8)(LinSduPtr[1] & 0x0Fu) )
    {
      /* #30 Update sequence number and Tp timeout counter */
      CtrlPtr->LinTp_SN                       = (uint8)((CtrlPtr->LinTp_SN + 1u) & 0x0Fu); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      CtrlPtr->LinTp_Timer                    = LinTp_GetNcrOfRxNSdu(CtrlPtr->LinTp_Curr_Rx_Id); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

      /* #40 Continue with receive buffer handling */
      /* check for end condition */
      if (CtrlPtr->LinTp_RemainingLength < 6u)
      {
        CtrlPtr->LinTp_Tx_Rx_Buffer_Length    = (uint8)(CtrlPtr->LinTp_RemainingLength); /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }
      else
      {
        CtrlPtr->LinTp_Tx_Rx_Buffer_Length    = 6u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
      }

      CtrlPtr->LinTp_Tx_Rx_Buffer_Idx         = 2u; /* index of the buffer copied from - CF:2 */ /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

      LinTp_RxDataBufferHandling_Main(CtrlPtr, LinSduPtr); /* SBSW_LINIF_TP_RXDATABUFHANDLING_CALL */
    }
    /* #50 Otherwise: Invalid sequence number in CF */
    else
    {
      /* #60 Abort connection and inform PduR */
      LinTp_RxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
  /* else: ignore unexpected consecutive frames */
}




/**********************************************************************************************************************
 *  LinTp_RxCloseConnection()
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
FUNC(void, LINIF_CODE) LinTp_RxCloseConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        CtrlPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Disable TP timer and set channel state to idle */
  CtrlPtr->LinTp_Timer                  = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
  CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
  CtrlPtr->LinTp_RemainingLength        = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  /* #20 If channel is a LIN master channel */
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  if ( CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Master )
#  endif
  {
    /* #30 Reset master-specific data of closing Tp connection */
    CtrlPtr->LinTp_NAD_Current            = 0u; /* reset requested NAD - only one response accepted for each request */ /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    /* clear P2 timer */
    CtrlPtr->LinTp_P2Timer                = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

    /* #40 Inform PduR about connection abortion, request applicative schedule from BswM */
    CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationRx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_PduRNotifyResultRx     = E_NOT_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_PduRNSduIdRx           = CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    if ( CtrlPtr->LinTp_PortNotification == LinTp_PortNotificationIdle ) /* COV_LINTP_FUNCREQUEST_SRF */
    { /* No new functional request pending */
      CtrlPtr->LinTp_PortNotification     = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    }
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    CtrlPtr->LinTp_ComModeRestore         = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  endif
  }
# endif
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  /* #50 If channel is a LIN slave channel */
#  if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  else
#  endif
  {
    /* #60 Inform PduR about connection abortion */
    PduR_LinTpRxIndication(CtrlPtr->LinTp_Curr_UL_NSdu_Id, E_NOT_OK);
  }
# endif
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
 *  LinTp_TxCloseConnection()
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
FUNC(void, LINIF_CODE) LinTp_TxCloseConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)        CtrlPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Disable TP timer and set channel state to idle */
  CtrlPtr->LinTp_Timer                  = 0u; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
  CtrlPtr->LinTp_Ch_State               = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  /* #20 If channel is a LIN master channel */
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  if ( CtrlPtr->LinTp_NodeType == LinIf_ChannelNodeType_Master )
#  endif
  {
    /* #30 Inform PduR about connection abortion and request applicative schedule from BswM */
    CtrlPtr->LinTp_PduRNotification      |= LinTp_PduRNotificationTx; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_PduRNotifyResultTx     = E_NOT_OK; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_PduRNSduIdTx           = CtrlPtr->LinTp_Curr_UL_NSdu_Id; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
    CtrlPtr->LinTp_PortNotification       = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
    CtrlPtr->LinTp_ComModeRestore         = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_PASSED_PTR_ACCESS */
#  endif
  }
# endif
# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
  /* #40 If channel is a LIN slave channel */
#  if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
  else
#  endif
  {
    /* #50 Inform PduR about connection abortion */
    PduR_LinTpTxConfirmation(CtrlPtr->LinTp_Curr_UL_NSdu_Id, E_NOT_OK);
  }
# endif
}

/**********************************************************************************************************************
 *
 * END - LinTp Transmission Handling
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_CloseActiveConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LINIF_CODE) LinTp_CloseActiveConnection
(
  CONSTP2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT)      CtrlPtr
)
{
  /* ----- Implementation ----------------------------------------------- */
  if ( CtrlPtr->LinTp_Ch_State != LINTP_CHANNEL_IDLE ) /* COV_LINTP_NODECONFIGURATION */
  {
    /* #10 If Tx connection is active, abort Tp connection and inform PduR */
    if ( (CtrlPtr->LinTp_Ch_State & LINTP_CHANNEL_TX_MSK ) == LINTP_CHANNEL_TX_MSK )
    {
      LinTp_TxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
    /* #20 If Rx connection is active, abort Tp connection and inform PduR */
    else
    {
      /* not idle and no Tx connection implies Rx connection */
      LinTp_RxCloseConnection(CtrlPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
    }
  }
}


/**********************************************************************************************************************
 *  GLOBAL LINTP FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinTp Initialization and General Function Interface - Service Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_Init()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, LINIF_CODE) LinTp_Init
(
  P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_INIT_DATA) ConfigPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least                                         linIfChannelIdx; /* initialized below before being used in loop */
  uint8                                               errorId = LINIF_E_NO_ERROR;

# if ( LINTP_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  Rtm_Start(RtmConf_RtmMeasurementPoint_LinTp_Init);
# endif

  LINIF_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if LinIf component is not yet initialized */
  if ( LinIf_Dev_InitDetect != LINIF_INIT )
  {
    errorId = LINIF_E_UNINIT;
  }
  /* #20 Check if LinTp component is already initialized */
  else if ( LinTp_Dev_InitDetect == LINTP_INIT )
  {
    errorId = LINIF_E_ALREADY_INITIALIZED;
  }
  else
# endif
  {
# if ( LINTP_USE_INIT_POINTER == STD_ON )
    LinTp_ConfigDataPtr     = ConfigPtr;

    /* #30 Check validity of parameter ConfigPtr (only post-build) */
    if (LinTp_ConfigDataPtr == (P2CONST(LinTp_ConfigType, AUTOMATIC, LINIF_INIT_DATA))NULL_PTR)
    {
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      errorId = LINIF_E_PARAMETER_POINTER;
#  endif
#  if ( LINTP_CONFIGURATION_VARIANT == LINTP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_NULLPTR );
#  endif
    }
#  if ( LINTP_CONFIGURATION_VARIANT == LINTP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    /* #40 Check generator version (only post-build loadable) */
    else if ( LinTp_GetGeneratorCompatibilityVersion() != (uint16) LINTP_GENERATOR_COMPATIBILITY_VERSION )
    {
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_COMPATIBILITYVERSION );
    }
    /* #50 Check magic number of config structure (only post-build loadable) */
    else if ( LinTp_GetFinalMagicNumber() != LINTP_FINAL_MAGIC_NUMBER)
    {
      EcuM_BswErrorHook((uint16) LINIF_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER );
    }
#  endif
    else
# endif
    {
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
      /* #60 Check LinIf and LinTp channel data structures for consistent size */
      if ( LinIf_GetSizeOfChannelData() < LinTp_GetSizeOfCtrl() ) /* COV_LINIF_INV_STATE_ALWAYSFALSE */ /* PRQA S 2742 */ /* MD_LINIF_Rule14.3_2742 */
      {
        /* defensive programming, will never occur in a valid configuration. This check ensures that state remains UNINIT in an invalid configuration.
           Check it once here in initialization to avoid check in main function that linIfChannelIdx has valid size */
        errorId = LINIF_E_CONFIG; /* PRQA S 2880 */ /* MD_LINIF_Rule2.1_2880 */
      }
      else
# endif
      {
        /* ----- Implementation ----------------------------------------------- */
        /* #70 Set LinTp initialization flag to INIT */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
        LinTp_Dev_InitDetect  = LINTP_INIT;
# endif

        /* #80 Initialize all channel dependent transport protocol variables for all channels */
        for ( linIfChannelIdx = 0x00u; linIfChannelIdx < (uint8)LinTp_GetSizeOfCtrl(); linIfChannelIdx++ ) /* FETA_LINIF_UPPER_LIMIT_SIZEOF */
        {
          LinTp_GetCtrl(linIfChannelIdx).LinTp_Timer                      = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */

          LinTp_GetCtrl(linIfChannelIdx).LinTp_RemainingLength            = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelIdx).LinTp_Curr_UL_NSdu_Id            = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */

          LinTp_GetCtrl(linIfChannelIdx).LinTp_PduInfoPtr.SduLength       = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelIdx).LinTp_Curr_Rx_Id                 = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */

          LinTp_GetCtrl(linIfChannelIdx).LinTp_Ch_State                   = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelIdx).LinTp_NAD_Current                = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelIdx).LinTp_Tx_Rx_Buffer_Idx           = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelIdx).LinTp_Tx_Rx_Buffer_Length        = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */
          LinTp_GetCtrl(linIfChannelIdx).LinTp_SN                         = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON ) && ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
          LinTp_GetCtrl(linIfChannelIdx).LinTp_NodeType                   = LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx); /* SBSW_LINIF_TP_CTRL_ARRAY */
# endif

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
          if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) == LinIf_ChannelNodeType_Master )
#  endif
          {
            LinTp_GetCtrl(linIfChannelIdx).LinTp_P2Timer                  = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */
            LinTp_GetCtrl(linIfChannelIdx).LinTp_RespPendingCounter       = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */

#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
            LinTp_GetCtrl(linIfChannelIdx).LinTp_FuncPduInfoPtr.SduLength = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */
            LinTp_GetCtrl(linIfChannelIdx).LinTp_Curr_Func_NSdu_Id        = 0u; /* SBSW_LINIF_TP_CTRL_ARRAY */
            LinTp_GetCtrl(linIfChannelIdx).LinTp_ComModeRestore           = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_ARRAY */
            LinTp_GetCtrl(linIfChannelIdx).LinTp_FuncRequest              = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_CTRL_ARRAY */
#  endif

            LinTp_GetCtrl(linIfChannelIdx).LinTp_PortNotification         = LinTp_PortNotificationIdle; /* SBSW_LINIF_TP_CTRL_ARRAY */
            LinTp_GetCtrl(linIfChannelIdx).LinTp_PduRNotification         = LinTp_PduRNotificationIdle; /* SBSW_LINIF_TP_CTRL_ARRAY */
          }
# endif
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_INIT, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

# if ( LINTP_RUNTIME_MEASUREMENT_SUPPORT == STD_ON )
  Rtm_Stop(RtmConf_RtmMeasurementPoint_LinTp_Init);
# endif
}

# if ( LINTP_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  LinTp_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LINIF_CODE) LinTp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR) versioninfo
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check validity of parameter versioninfo. Note: no uninit check is performed */
  if ( versioninfo == (P2VAR(Std_VersionInfoType, AUTOMATIC, LINIF_APPL_VAR))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set versioninfo parameter with component information */
    versioninfo->vendorID           = (uint16)LINIF_VENDOR_ID; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->moduleID           = (uint16)LINIF_MODULE_ID; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_major_version   = (uint8)LINIF_SW_MAJOR_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_minor_version   = (uint8)LINIF_SW_MINOR_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
    versioninfo->sw_patch_version   = (uint8)LINIF_SW_PATCH_VERSION; /* SBSW_LINIF_VERSIONINFO_PTR */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_GETVERSIONINFO, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
}
# endif /* LINIF_VERSION_INFO_API == STD_ON */


/**********************************************************************************************************************
 *  LinTp_Shutdown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, LINIF_CODE) LinTp_Shutdown
(
  void
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least                                               linIfChannelIdx; /* initialized below before being used in loop */
  uint8                                                     errorId = LINIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
    errorId = LINIF_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    SchM_Enter_LinIf_LINIF_EXCLUSIVE_AREA_1();

    /* clear all LIN TP resources */
    /* #20 Reset all transport protocol variables and connections for all LinIf channels */
    for ( linIfChannelIdx = 0x00u; linIfChannelIdx < (uint8)LinTp_GetSizeOfCtrl(); linIfChannelIdx++ ) /* FETA_LINIF_UPPER_LIMIT_SIZEOF */
    {
      /* according to LINIF357 no reporting done in this routine */
      /* according to AUTOSAR Bugzilla 61446 and 77498, upper layers shall not be notified as they are possibly
         already uninitialized at this point */
      P2VAR(LinTp_ControlType, AUTOMATIC, LINIF_VAR_NOINIT) tpCtrl = LinTp_GetAddrCtrl(linIfChannelIdx);

      tpCtrl->LinTp_Timer              = 0u; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
      tpCtrl->LinTp_Ch_State           = LINTP_CHANNEL_IDLE; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */

# if ( LINIF_NODETYPE_MASTER_SUPPORTED == STD_ON )
#  if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
      if ( LinIf_GetNodeTypeOfChannelConfig(linIfChannelIdx) == LinIf_ChannelNodeType_Master )
#  endif
      {
        tpCtrl->LinTp_NAD_Current      = 0u; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
        tpCtrl->LinTp_P2Timer          = 0u; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
#  if ( LINTP_FUNCTIONAL_REQUEST_SUPPORTED == STD_ON )
        tpCtrl->LinTp_FuncRequest      = LINTP_FUNC_REQUEST_IDLE; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
        tpCtrl->LinTp_ComModeRestore   = LINTP_APPLICATIVE_SCHEDULE; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
#  endif
        tpCtrl->LinTp_PortNotification   = LinTp_PortNotificationIdle; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
        tpCtrl->LinTp_PduRNotification   = LinTp_PduRNotificationIdle; /* SBSW_LINIF_TP_CTRL_PTR_LOCAL */
      }
# endif
    }

    SchM_Exit_LinIf_LINIF_EXCLUSIVE_AREA_1();

    /* #30 Set LinTp initialization flag to UNINIT */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
    LinTp_Dev_InitDetect = LINTP_UNINIT;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_SHUTDOWN, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *
 * END - LinTp Initialization and General Function Interface - Service Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *
 * LinTp Transmission Handling - Service Functions
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LinTp_Transmit()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_Transmit
(
  VAR(PduIdType, AUTOMATIC)                                 LinTpTxSduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA)          LinTpTxInfoPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                            retVal = E_NOT_OK;
  uint8                                                     errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(LinTpTxSduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(LinTpTxInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
# if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
     errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter LinTpTxInfoPtr */
  else if ( LinTpTxInfoPtr == (P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA))NULL_PTR )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
  /* #25 Check validity of parameter LinTpTxInfoPtr->SduLength */
  else if ( LinTpTxInfoPtr->SduLength > LinTp_FrameMaxLength )
  {
    errorId = LINIF_E_PARAMETER;
  }
  /* #30 Check validity of parameter LinTpTxSduId */
#  if ( LINTP_INVALIDHNDOFTXNSDU == STD_ON)
  else if ( (LinTpTxSduId >= LinTp_GetNumberOfLinTpTxNSdu()) || (LinTp_IsInvalidHndOfTxNSdu(LinTpTxSduId)) )
#  else
  else if ( LinTpTxSduId >= LinTp_GetNumberOfLinTpTxNSdu() )
#  endif
  {
    errorId = LINIF_E_PARAMETER;
  }
  /* at this point LinTpTxSduId is valid, so the LinIfChannel belonging to this Pdu is guaranteed to be valid and used by LinTp */
  /* #40 Check validity of parameter LinTpTxInfoPtr->SduDataPtr if meta data handling is supported and used for TxNSdu */
#  if ( LINTP_META_DATA_SUPPORTED == STD_ON )
  else if ( (LinTp_IsMetaDataSupportedOfTxNSdu(LinTpTxSduId)) && (LinTpTxInfoPtr->SduDataPtr == NULL_PTR) )
  {
    errorId = LINIF_E_PARAMETER_POINTER;
  }
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 Process transmission request */
    retVal = LinTp_TxTransmitInternal(LinTpTxSduId, LinTpTxInfoPtr); /* SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL */
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_TRANSMIT, errorId);
  }
# else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}



# if ( LINTP_CANCEL_TRANSMIT_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinTp_CancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelTransmit
(
  VAR(PduIdType, AUTOMATIC)               LinTpTxSduId
)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC)                   errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(LinTpTxSduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
     errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter LinTpTxSduId */
#   if ( LINTP_INVALIDHNDOFTXNSDU == STD_ON)
  else if ( (LinTpTxSduId >= LinTp_GetNumberOfLinTpTxNSdu()) || (LinTp_IsInvalidHndOfTxNSdu(LinTpTxSduId)) )
#   else
  else if ( LinTpTxSduId >= LinTp_GetNumberOfLinTpTxNSdu() )
#   endif
  {
    errorId = LINIF_E_PARAMETER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Function does nothing, just return E_NOT_OK */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_CANCELTRANSMIT, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif

  /* cancellation request is always rejected */
  return E_NOT_OK;
}
# endif /* LINTP_CANCEL_TRANSMIT_SUPPORTED */

/**********************************************************************************************************************
 *
 * END - LinTp Transmission Handling - Service Functions
 *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *
 * LinTp Reception Handling - Service Functions
 *
 *********************************************************************************************************************/

# if ( LINTP_CANCEL_RECEIVE_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinTp_CancelReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelReceive
(
  VAR(PduIdType, AUTOMATIC) LinTpRxSduId
)
{
  /* ----- Local Variables ---------------------------------------------- */
  VAR(uint8, AUTOMATIC)                   errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(LinTpRxSduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
     errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter LinTpRxSduId */
#   if ( LINTP_INVALIDHNDOFRXNSDU == STD_ON )
  else if ( (LinTpRxSduId >= LinTp_GetNumberOfLinTpRxNSdu()) || (LinTp_IsInvalidHndOfRxNSdu(LinTpRxSduId)) )
#   else
  else if ( LinTpRxSduId >= LinTp_GetNumberOfLinTpRxNSdu() )
#   endif
  {
    errorId = LINIF_E_PARAMETER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Function does nothing, just return E_NOT_OK */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_CANCELRECEIVE, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif

  /* cancellation request is always rejected */
  return E_NOT_OK;
}
# endif /* LINTP_CANCEL_RECEIVE_SUPPORTED */


# if ( LINTP_CHANGE_PARAMETER_SUPPORTED == STD_ON )
/**********************************************************************************************************************
 *  LinTp_ChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_ChangeParameter
(
  VAR(PduIdType, AUTOMATIC)                 id,
  VAR(TPParameterType, AUTOMATIC)           parameter,
  VAR(uint16, AUTOMATIC)                    value
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                                     errorId = LINIF_E_NO_ERROR;

  LINIF_DUMMY_STATEMENT(id); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(parameter); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  LINIF_DUMMY_STATEMENT(value); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  /* ----- Development Error Checks ------------------------------------- */
#  if ( LINIF_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check if component is initialized */
  if ( (LinIf_Dev_InitDetect != LINIF_INIT) || (LinTp_Dev_InitDetect != LINTP_INIT) )
  {
     errorId = LINIF_E_UNINIT;
  }
  /* #20 Check validity of parameter id */
#   if ( LINTP_INVALIDHNDOFRXNSDU == STD_ON )
  else if ( (id >= LinTp_GetNumberOfLinTpRxNSdu()) || (LinTp_IsInvalidHndOfRxNSdu(id)) )
#   else
  else if ( id >= LinTp_GetNumberOfLinTpRxNSdu() )
#   endif
  {
    errorId = LINIF_E_PARAMETER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Function does nothing, just return E_NOT_OK */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if ( LINIF_DEV_ERROR_REPORT == STD_ON )
  if ( errorId != LINIF_E_NO_ERROR )
  {
    (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID_DET, LINTP_SID_CHANGEPARAMETER, errorId);
  }
#  else
  LINIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif

  return E_NOT_OK;
}
# endif /* LINTP_CHANGE_PARAMETER_SUPPORTED */

/**********************************************************************************************************************
 *
 * END - LinTp Reception Handling - Service Functions
 *
 *********************************************************************************************************************/

#endif /* LINIF_TP_SUPPORTED == STD_ON */

#define LINIF_STOP_SEC_CODE
#include "LinIf_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* module specific MISRA deviations:

  MD_LINIF_Rule2.1_2880:
    Reason: Statement is only reachable in case of corrupted variable content.
    Risk: None.
    Prevention: None.

  MD_LINIF_Rule2.2_2982_RP:
    Reason: Depending on the configuration, the variable might be set two times to the same value. This is accepted in order to not
            increase control flow and preprocessor complexity.
    Risk: None.
    Prevention: None.

  MD_LINIF_Rule8.13_3673_3678:
     Reason: The usage of this pointer depends on the actual configuration and cannot be declared as 'pointer to const' in every case.
     Risk: Unintentional modification of the object addressed by the pointer.
     Prevention: Code inspection and test of the different variants in the component test.

  MD_LINIF_Rule11.8_0311:
    Reason: The object addressed by the pointer parameter LinSduPtr is not modified and thus is a pointer to type
            'pointer to const'. The SduDataPtr is a member of PduInfoType which is defined by AUTOSAR. The SduDataPtr
            is a 'standard pointer' type, thus the const type qualifier is lost in the assignment.
            The LinSduPtr parameter is provided by LIN driver in LinIf_RxIndication callback, in which the pointer
            has 'standard pointer' type, so it does not point to const data. The intermediate conversion of this
            pointer to type 'pointer to const' is solely to avoid several other MISRA warnings that this pointer is
            passed to a function but is not modified so shall be of type 'pointer to const'.
    Risk: None.
    Prevention: None.

  MD_LINIF_Rule14.3_2742:
    Reason: Consistency check for invalid configuration, can never be covered with valid generation data.
    Risk: None.
    Prevention: None.

*/

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_LINIF_CHANNEL_DATA_ACCESS
    \DESCRIPTION Array write access of LinIf_ChannelData using ComStackLib. The LinIf channel index variable is either
                 a loop variable and ranges from 0 to LinIf_GetSizeOfChannelData() - 1 or the variable is already
                 checked before for validity to be less than LinIf_GetSizeOfChannelData.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_CHANNEL_DATA_PTR_LOCAL
    \DESCRIPTION Pointer write access to channel dependent data.
                 The pointer is retrieved by accessing the array LinIf_ChannelData using an index variable.
                 The index variable is checked before using a runtime check to be less than LinIf_GetSizeOfChannelData.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_MASTER_SLAVE_CHANNEL_DATA_PTR
    \DESCRIPTION Pointer write access to master or slave specific channel data via indirection over
                 LinIf_GetNodeChannelDataIdxOfChannelConfig.
                 The pointer is retrieved using the Get macro of ComStackLib and a channel index that is already checked
                 before to to be less than LinIf_GetSizeOfChannelData. Note that it is ensured via CSL02 that
                 LinIf_GetSizeOfChannelData has the same size than LinIf_GetSizeOfChannelConfig.
    \COUNTERMEASURE \N Qualified use-case CSL02 in combination of CSL03 of ComStackLib.

\ID SBSW_LINIF_PASSED_PTR_ACCESS
    \DESCRIPTION Write access of pointer passed to this function.
    \COUNTERMEASURE \N Caller assures that passed pointer is always valid.

\ID SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR_PDUINFO
    \DESCRIPTION Call of external function with pointer parameter.
                 The passed pduinfo pointer points always to a local variable on the stack, therefore it's always valid.
                 The SduDataPtr points always to LinIf_TxData which is 8 bytes long. The length value is always in range [1,8], see countermeasure.
    \COUNTERMEASURE \S It must be verified that all frame length values are in range [1,8]. SMI-148.

\ID SBSW_LINIF_FCT_CALL_EXTERNAL_WITH_PTR_STACK_VAR
    \DESCRIPTION Call of external function with pointer parameter.
    \COUNTERMEASURE \N The pointer points always to a local variable on the stack, therefore it's always valid.

\ID SBSW_LINIF_VERSIONINFO_PTR
    \DESCRIPTION The function <Msn>_GetVersionInfo writes to the object referenced by parameter versioninfo.
    \COUNTERMEASURE \S The caller must ensure that the pointers passed to the parameters versioninfo references an object
                       of type Std_VersionInfoType. SMI-16.
                    \R A check of versioninfo against NULL_PTR ensures that no null pointer is dereferenced.

\ID SBSW_LINIF_SCHEDULEINFO_PTR
    \DESCRIPTION The function Linf_GetScheduleInfo writes to the object referenced by parameter ScheduleInfo.
    \COUNTERMEASURE \S The caller must ensure that the pointers passed to the parameters ScheduleInfo references an object
                       of type LinIf_ScheduleInfoType. SMI-16.
                    \R A check of ScheduleInfo against NULL_PTR ensures that no null pointer is dereferenced.

\ID SBSW_LINIF_LINPDU_SDUPTR
    \DESCRIPTION Array write access to LIN Pdu data buffer. The pointer SduPtr of structure LinIf_LinPduPtr is set to
                 LinIf Tx data array before array access.
    \COUNTERMEASURE \N The size of this array is always equal to the loop variable used for element access by using the
                       same define LINIF_LINPDU_SIZE. No out of bounds array access can occur.

\ID SBSW_LINIF_SET_CONFIGURED_PID
    \DESCRIPTION Access of LinIf_ConfiguredPIDTable using ComStackLib.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER
    \DESCRIPTION Access of LinIf_ChannelData.LinIf_SchedTabData using ComStackLib.
                 The pointer is retrieved once at the beginning of function LinIf_ScheduleTableHandler by accessing
                 the array LinIf_ChannelData using the index LinIf_LinIfChannel.
                 The variable LinIf_LinIfChannel is checked for validity in LinIf_ChannelMainFunction.
                 The pointer just is passed-through to all local functions from LinIf_ScheduleTableHandler onwards.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_FLUSHALLSCHED_CALL_1
    \DESCRIPTION Function call LinIf_FlushAllSchedules with pointer parameter.
                 The pointer LinIf_ChannelDataPtr itself is valid due to [SBSW_LINIF_CHANNEL_DATA_PTR]. This implies that
                 LinIf_ChannelDataPtr->LinIf_SchedTabData is also valid because LinIf_ChannelData is statically allocated.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_FLUSHALLSCHED_CALL_2
    \DESCRIPTION Function call LinIf_FlushAllSchedules with pointer parameter.
                 The provided pointer to the schedule table data is valid due to [SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER].
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_FCT_CALL_WITH_SCHEDTABPTR
    \DESCRIPTION Function call with pointer parameter to schedule table data.
    \COUNTERMEASURE \N The provided pointer to the schedule table data is valid due to [SBSW_LINIF_SCHEDULE_TABLE_DATA_POINTER].

\ID SBSW_LINIF_SPOR_TXFLAGS_ACCESS_CSL
    \DESCRIPTION Array write access of LinIf_SporTransmitFlags using ComStackLib with an index variable.
                 The index variable is a loop variable and ranges from 0 to LinIf_GetSizeOfSporTransmitFlags() - 1.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_SPOR_TXFLAGS_ACCESS
    \DESCRIPTION Array write access of LinIf_SporTransmitFlags.
    \COUNTERMEASURE \R It is ensured by a runtime check before that the index variable is less than LinIf_GetSizeOfSporTransmitFlags.

\ID SBSW_LINIF_FCT_POINTER_CALL
    \DESCRIPTION Function pointer call via indirection over function table or function pointer variable.
                 The index of the Linif channel is valid due to [SBSW_LINIF_CHANNEL_DATA_ACCESS].
                 The indirection to get actual index into table is not critical due to qualified use-case CSL03 of ComStackLib.
    \COUNTERMEASURE \S It must be verified that only valid function names with matching parameter lists are contained in the
                       function table or function variable. Especially no NULL_PTR or explicit address values are allowed. SMI-388.

\ID SBSW_LINIF_FCT_POINTER_CALL_PDUINFO
    \DESCRIPTION Function pointer call via indirection over function table.
                 The index of the Linif channel is valid due to [SBSW_LINIF_CHANNEL_DATA_ACCESS].
                 The indirection to get actual index into table is not critical due to qualified use-case CSL03 of ComStackLib.
                 The passed pduinfo pointer points always to a local variable on the stack, therefore it's always valid.
                 The SduDataPtr points always to LinIf_TxData which is 8 bytes long.
                 The length value is always in range [1,8], see countermeasure.
    \COUNTERMEASURE \S It must be verified that only valid function names with matching parameter lists are contained in the
                       function table. Especially no NULL_PTR or explicit address values are allowed. SMI-388.
                       It must be verified that all frame length values are in range [1,8]. SMI-148.

\ID SBSW_LINIF_FCT_CALL_PTR_HANDOVER_OR_LOCAL_OR_CSL
    \DESCRIPTION Call of function with pointer parameter. There are three possible pointer origins:
                 a) The pointer was provided by the caller of this function and the pointer is just passed to the called
                    function without accessing it.
                 b) The pointer points to a local stack variable.
                 c) The pointer is retrieved by an address macro of ComStackLib. The passed channel index is already checked
                    for validity in LinIf_ChannelMainFunction.
    \COUNTERMEASURE \N The pointer is valid in all three cases. Case c) is the qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_PIDBUFFER_ARRAY
   \DESCRIPTION Array write access of PidBuffer array.
   \COUNTERMEASURE \R The passed length of the buffer pointed to by PidBuffer is checked before to be match the length
                      of the configured PID table of current channel.

\ID SBSW_LINIF_RX_BUFFER_ARRAY
    \DESCRIPTION Array access to local Rx buffer LinIf_RxMirrorData with size LINIF_LINPDU_SIZE using an index variable.
    \COUNTERMEASURE \R The index cannot exceed the size of the array because the index is limited to range from 0
                       to LINIF_LINPDU_SIZE - 1. Length of LinIf_RxMirrorData is LINIF_LINPDU_SIZE.
                       It is additionally verified with a preprocessor check that LINIF_LINPDU_SIZE has valid size.


\ID SBSW_LINIF_TP_CTRL_ARRAY
    \DESCRIPTION Array write access to LinTp_Ctrl using ComStackLib with an index variable.
                 The index variable is a loop variable and ranges from 0 to LinTp_GetSizeOfCtrl() - 1.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_TP_CTRL_PTR_LOCAL
    \DESCRIPTION Pointer write access to channel dependent Tp data.
                 The pointer is retrieved by accessing the array LinTp_Ctrl using an index variable.
                 The index variable is checked before using a runtime check to be less than LinIf_GetSizeOfChannelData
                 which is equal to LinTp_GetSizeOfCtrl.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_LINIF_TP_PASSED_PTR_ACCESS
    \DESCRIPTION Write access of pointer passed to this function.
    \COUNTERMEASURE \N Caller assures that passed pointer is always valid.

\ID SBSW_LINIF_TP_TRANSMIT_CTRL_PTR
    \DESCRIPTION Pointer write access of to LinTp_Ctrl using ComStackLib. LinTp_Ctrl is accessed via an indirection using
                 index variable LinTpTxSduId. LinTpTxSduId is checked before to be less than LinTp_GetNumberOfLinTpTxNSdu.
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID SBSW_LINIF_TP_RXDATABUFHANDLING_CALL
    \DESCRIPTION Function call LinTp_RxDataBufferHandling_Main with pointer parameters.
    \COUNTERMEASURE \N The first passed pointer is valid due to [SBSW_LINIF_TP_CTRL_ARRAY], [SBSW_LINIF_TP_CTRL_PTR]
                       or [SBSW_LINIF_TP_TRANSMIT_CTRL_PTR].
                       The second parameter was either provided by the caller of this function or is the NULL_PTR.
                       Note that LinTp_RxDataBufferHandling_Main explicitly allows to pass a NULL_PTR as argument to
                       indicate special handling.

\ID SBSW_LINIF_TP_LINPDU_SDUPTR
    \DESCRIPTION Array write access to LIN Pdu data buffer using LinPduPtr->SduPtr.
                 The pointer SduPtr of structure LinPdu is set to LinIf_TxData array directly before array access and
                 LinIf_TxData has size LINIF_LINPDU_SIZE.
    \COUNTERMEASURE \R A preprocessor check ensures that LINIF_LINPDU_SIZE is at least two.
                       The loop variable used for element access uses LINIF_LINPDU_SIZE as border so it never exceeds
                       the size of the array LinPduPtr->SduPtr.

\ID SBSW_LINIF_TP_PDUR_FCT_CALL_START_RX
    \DESCRIPTION Call of function PduR_LinTpStartOfReception with pointer parameters.
    \COUNTERMEASURE \N The first passed pointer is fixed to NULL_PTR to indicate no meta data. Providing a NULL pointer is
                       explicitly allowed by this function.
                       The second passed pointer points to a local stack variable and is always valid.

\ID SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_RX
    \DESCRIPTION Call of function PduR_LinTpCopyRxData with pointer parameters.
    \COUNTERMEASURE \N The pointer parameter was either provided by the caller of this function and the pointer is just
                       passed to the called function. Or the pointer is a local stack variable or points to a local
                       stack variable and is therefore always valid.

\ID SBSW_LINIF_TP_PDUR_FCT_CALL_COPY_TX
    \DESCRIPTION Call of function PduR_LinTpCopyTxData with pointer parameters.
    \COUNTERMEASURE \N The passed pointer for parameter retry is fixed to NULL_PTR to indicate no retry.
                       Providing a NULL pointer for retry is explicitly allowed by this function.
                       The other pointer parameters were either provided by the caller of this function and the pointers
                       are just passed to the called function. Or the pointers are or point to a local stack variables
                       and are always valid.

\ID SBSW_LINIF_TP_CTRL_BUFFER_ARRAY
    \DESCRIPTION Array access to local Tp buffer LinTp_Tx_Rx_Buffer with size LINTP_LOCAL_BUFFER_SIZE via index variable.
    \COUNTERMEASURE \R The index cannot exceed the size of the array because the index is limited to range from 0
                       to LINTP_LOCAL_BUFFER_SIZE - 1. Length of LinTp_Tx_Rx_Buffer is LINTP_LOCAL_BUFFER_SIZE.
                       It is additionally verified with a preprocessor check that LINTP_LOCAL_BUFFER_SIZE has valid size.

\ID SBSW_LINIF_TP_META_DATA_ARRAY_LOCAL
    \DESCRIPTION Array write access to local array variable on the stack.
    \COUNTERMEASURE \N The array has a static size of 1. The index used to access the array has the fixed value 0.

SBSW_JUSTIFICATION_END */


/* START_COVERAGE_JUSTIFICATION

\ID COV_LINIF_MISRA_ELSEBRANCH
  \ACCEPT TX
  \REASON [COV_MSR_MISRA]

\ID COV_LINIF_MISRA_DEFAULTBRANCH
  \ACCEPT X
  \REASON [COV_MSR_MISRA]

\ID COV_LINIF_INV_STATE_ALWAYSFALSE
  \ACCEPT XF
  \REASON [COV_MSR_INV_STATE]

\ID COV_LINIF_SPOR_FRAME_HANDLE_CONFIG
  \ACCEPT TX
  \ACCEPT XF
  \REASON Depending on the configuration (e.g. frame names, order of frames), inside the FrameList data structure no unconditional
          Tx frames follow behind the sporadic unconditional frames. This means the frame handle of all other unconditional frames
          is smaller than that of any sporadic unconditional frames - in this case the loop is entered only once and left due to the
          break statement in the first loop cycle. In case a sproadic frame is actually transmitted, then it is found in this search
          and the while loop reaches never the limit, but is also quit by the break statement.
          In postbuild configurations, sporadic frames support is enabled but zero sporadic frames exist. In this case the condition
          is always false.

\ID COV_LINIF_HANDLE_GAPS_CONFIG
  \ACCEPT TF tf xf
  \REASON In case there are no gaps inside the handle list, then no invalid rows in the data structure exist, therefore the check for
          handlegaps can never be evaluated true.

\ID COV_LINIF_FUNC_CALL_CYCLE_CONFIG
  \ACCEPT TX
  \REASON In a single channel configuration or in a multi channel configuration where each channel uses the same time base, the channel
          function call cycle is 1 (the channel main funcion is called in every LinIf main function cycle). Therefore this statement is
          always true.

\ID COV_LINIF_NODECONFIG_ELSEBRANCH
  \ACCEPT TX
  \ACCEPT XX
  \REASON For TX, see [MSR_COV_MISRA]
          In postbuild configurations, node configuration frames support is enabled but no node configuration frames could exist. In this case
          this state is never reached.

\ID COV_LINIF_NO_SPORFRAME_ON_TESTCHANNEL
  \ACCEPT XF
  \ACCEPT XX
  \ACCEPT TX
  \REASON The support for sporadic frames globally enabled, but there is no sporadic frame configured on the channel under test and it never
          explicitely triggered. In postbuild configurations, this location may be never covered if no sporadic frames exist at all (XX case).

\ID COV_LINIF_TRCV
  \ACCEPT XF
  \REASON In a single channel configuration with transceiver handling enabled, there is no Lin channel without transceiver, so this code cannot be
          covered in such a configuration.

\ID COV_LINIF_SCHEDULECHANGE_POINT_1
  \ACCEPT TF tx tf tf
  \ACCEPT XF xf xx xx
  \REASON This boolean value is either always FALSE or always TRUE for one channel. This configuration parameter supports postbuild.

\ID COV_LINIF_SCHEDULECHANGE_POINT_2
  \ACCEPT XX xx xx xx
  \REASON Previous decision depends on boolean (postbuild) configuration parameter that could be either FALSE or TRUE. If it's FALSE then this decision
          is never reached.

\ID COV_SLAVE_RESP_ERR_SIG
  \ACCEPT XF xf xx xx
  \ACCEPT XF xf xx
  \REASON In configurations with no response error signal handling on any channel, this case can never be covered. As the configuration of the response
          error signal is postbuild capable, this condition is not encapsulated by a preprocessor statement.

\ID COV_SLAVE_J2602_FIND_NEXT_ERROR_BIT_FOR_LOOP
  \ACCEPT TX
  \REASON The loop is never quit due to non-fulfilled condition in for-loop header, but always by the break statement.
          The preceding if-else-statement ensures that the for loop is only entered if at least one bit is set in the
          current J2602 status error bit mask. If a set bit is encountered in the loop, the loop is directly quit using a break statement.

\ID COV_LINTP_SHORT_SRF_SLOT
  \ACCEPT TF tf xf
  \REASON The state can only be LINTP_CHANNEL_BUSY_RX_WAIT_FOR_START at this point if a previous SRF slot is finished in the same cycle as a
          new SRF slot begins and the received SRF aborts the previous diagnostic response connection. The abortion notification to PduR is pending,
          so the buffer request for the new connection is postponed to the next main function cycle.
          This can only happen in configuration where the SRF slot is as long as the transmission duration of the frame in ticks, and this constraint
          is only provided in special configurations.

\ID COV_LINTP_SCHEDCHANGENOTIFY
  \ACCEPT TX
  \ACCEPT XF
  \REASON In a single channel configuration, this boolean value is either always FALSE or always TRUE. This configuration parameter supports postbuild.

\ID COV_LINTP_STRICTNADCHECK_CONDITION
  \ACCEPT XF xf xx xx
  \ACCEPT TF tx tf tf
  \REASON In a single channel configuration, this boolean value is either always FALSE or always TRUE. This configuration parameter supports postbuild.

\ID COV_LINTP_STRICTNADCHECK_UNREACHEDCODE
  \ACCEPT XX
  \ACCEPT TX
  \ACCEPT XF
  \REASON If strict NAD check parameter (postbuild parameter) is TRUE, the NAD is rejected before and this code location
          is never reached in case of an unexpected NAD (NAD discarded). In case it is FALSE it depends on the order of
          RxNSdu and the configured NAD values in the configuration when a match is found.
          As the configuration parameter is postbuild capable, this condition is not encapsulated by a preprocessor statement.

\ID COV_LINTP_STRICTNADCHECK_NSDU_CONDITION
  \ACCEPT XX xx xx xx xx xx
  \ACCEPT XX xx xx xx xx
  \ACCEPT TF tf tf tx tf
  \ACCEPT TF tf tf tx tx
  \ACCEPT TF tf tf tx
  \REASON Case 1: If strict NAD check parameter (postbuild parameter) is TRUE, this location is never reached, see check above.
          Case 2,3,4: In a single channel configuration, last condition is always TRUE.

\ID COV_LINTP_BROADCASTHANDLING
  \ACCEPT XF
  \REASON In postbuild configurations, the support for broadcast requests may be enabled but no broadcast request exists in configuration.

\ID COV_LINTP_NODECONFIGURATION
  \ACCEPT TX
  \REASON The channel state can only be IDLE in this state if a node configuration request is scheduled and an active LinTp connection is therefore closed.
          In configurations without node configuration frames, this state can never happen.

\ID COV_LINTP_FUNCREQUEST_SRF
  \ACCEPT TX
  \REASON This case that a port notification for a functional request is still pending at this location can only be stimulated in configurations which
          supports and contains a functional request and in which schedule change notification is enabled and where the SRF slot is exactly one main
          function cycle longer than the SRF frame duration (special timing case).

Preprocessor coverage justifications

\ID COV_LINIF_COMPATIBILITY
  \REASON [COV_MSR_COMPATIBILITY]

\ID COV_LINIF_CONFIG
  \REASON [COV_MSR_CONFIG]

\ID COV_LINIF_FBL_USECASE
  \ACCEPT TX
  \REASON In special FBL configuration, only diagnostic frames is used but no other frame types. This means that LinIf does not handle any
          frames and header file PduR_LinIf.h does not exist in the configuration. Because this switch has not any other impact except this
          inclusion, it is not tested.

END_COVERAGE_JUSTIFICATION */

/* FETA_JUSTIFICATION_BEGIN

\ID FETA_LINIF_CORRECT_USAGE_OF_INTERFACE
\DESCRIPTION Store received data locally. Upper limit is dependent on content of receive buffer provided by component DrvLin.
\COUNTERMEASURE \S User assures that the passed value is always valid SMI-494148.

\ID FETA_LINIF_UPPER_LIMIT_SIZEOF
\DESCRIPTION Loop with an upper limit defined by a size value that is generated. It is obtained with a trivial data getter, the value is known at compile time.
\COUNTERMEASURE \N ComStackLib assures the value of this qualified use-case CSL01 is valid.

\ID FETA_LINIF_UPPER_LIMIT_ENDINDEX
\DESCRIPTION Loop with an upper limit that is a generated static end index. It is obtained with a trivial data getter, the value is known at compile time.
\COUNTERMEASURE \N ComStackLib assures the value of this qualified use-case CSL03 is valid.

\ID FETA_LINIF_UPPER_LIMIT_LENGTHOF
\DESCRIPTION Loop with a static upper limit defined by a length value that is generated. It is obtained with a trivial data getter, the value is known at compile time.
\COUNTERMEASURE \N ComStackLib assures the value of this qualified use-case CSL03 is valid.

\ID FETA_LINIF_UPPER_LIMIT_IS_GENERATED_CONSTANT
\DESCRIPTION Loop with a static upper limit that is generated. It is obtained with a trivial data getter, the value is known at compile time.
\COUNTERMEASURE \N The upper limit of the loop is a generated constant.

FETA_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: LinIf.c
 *********************************************************************************************************************/
