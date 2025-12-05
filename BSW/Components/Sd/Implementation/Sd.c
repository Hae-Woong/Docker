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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Sd.c
 *        \brief  Service Discovery source file
 *
 *      \details  Vector static code implementation for AUTOSAR Service Discovery (SD) module.
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

#define SD_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 1881 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA S 4304 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA S 4558 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA S 4522, 4424, 4432, 4332, 4342, 4393 EOF */ /* MD_Sd_Enum_4522_4424_4432_4332_4342_4393 */

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use an include guard. */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Sd_Priv.h"
#include "BswM_Sd.h"
#include "vstdlib.h"

#if((SD_DEV_ERROR_REPORT == STD_ON) || (SD_RUNTIME_ERROR_REPORT == STD_ON))
# include "Det.h"
#endif /* (SD_DEV_ERROR_REPORT == STD_ON) || (SD_RUNTIME_ERROR_REPORT == STD_ON) */

#if(SD_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
# include "Rtm.h"
#endif

#if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of Sd header file. */
#if((SD_SW_MAJOR_VERSION != (0x15u)) || (SD_SW_MINOR_VERSION != (0x01u)) || (SD_SW_PATCH_VERSION != (0x00u)))
# error "Vendor specific version numbers of Sd.c and Sd.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if !defined (SD_LOCAL)                                                                                                 /* COV_SD_COMPATIBILITY */
# define SD_LOCAL static
#endif

#if !defined (SD_LOCAL_INLINE)                                                                                          /* COV_SD_COMPATIBILITY */
# define SD_LOCAL_INLINE LOCAL_INLINE
#endif

/* PartitionData array has always size == 1 for every partition. */
# define SD_PARTITION_DATA_IDX 0u

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* Rtm runtime measurement hooks */
#if(SD_RUNTIME_MEASUREMENT_SUPPORT == STD_ON)
# define SdHookBegin_EventServer_Process()                    Rtm_Start(RtmConf_RtmMeasurementPoint_Sd_EventServer_Process)
# define SdHookEnd_EventServer_Process()                      Rtm_Stop(RtmConf_RtmMeasurementPoint_Sd_EventServer_Process)
# define SdHookBegin_RxIndication()                           Rtm_Start(RtmConf_RtmMeasurementPoint_Sd_RxIndication)
# define SdHookEnd_RxIndication()                             Rtm_Stop(RtmConf_RtmMeasurementPoint_Sd_RxIndication)
# define SdHookBegin_State_HandleRunMode()                    Rtm_Start(RtmConf_RtmMeasurementPoint_Sd_State_HandleRunMode)
# define SdHookEnd_State_HandleRunMode()                      Rtm_Stop(RtmConf_RtmMeasurementPoint_Sd_State_HandleRunMode)
# define SdHookBegin_State_Process_ClientSMs()                Rtm_Start(RtmConf_RtmMeasurementPoint_Sd_State_Process__ClientSMs)
# define SdHookEnd_State_Process_ClientSMs()                  Rtm_Stop(RtmConf_RtmMeasurementPoint_Sd_State_Process__ClientSMs)
# define SdHookBegin_State_Process_ServerSMs()                Rtm_Start(RtmConf_RtmMeasurementPoint_Sd_State_Process__ServerSMs)
# define SdHookEnd_State_Process_ServerSMs()                  Rtm_Stop(RtmConf_RtmMeasurementPoint_Sd_State_Process__ServerSMs)
# define SdHookBegin_EventServer_Tx_ProcessPendingMessages()  Rtm_Start(RtmConf_RtmMeasurementPoint_Sd_Tx_ProcessPendingMessages)
# define SdHookEnd_EventServer_Tx_ProcessPendingMessages()    Rtm_Stop(RtmConf_RtmMeasurementPoint_Sd_Tx_ProcessPendingMessages)
#else
# define SdHookBegin_EventServer_Process()
# define SdHookEnd_EventServer_Process()
# define SdHookBegin_RxIndication()
# define SdHookEnd_RxIndication()
# define SdHookBegin_State_HandleRunMode()
# define SdHookEnd_State_HandleRunMode()
# define SdHookBegin_State_Process_ClientSMs()
# define SdHookEnd_State_Process_ClientSMs()
# define SdHookBegin_State_Process_ServerSMs()
# define SdHookEnd_State_Process_ServerSMs()
# define SdHookBegin_EventServer_Tx_ProcessPendingMessages()
# define SdHookEnd_EventServer_Tx_ProcessPendingMessages()
#endif /* SD_RUNTIME_MEASUREMENT_SUPPORT */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
#define SD_START_SEC_VAR_CLEARED_8
#include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Global initialization state of the module. */
SD_LOCAL VAR(Sd_StateType, SD_VAR_CLEARED) Sd_State = SD_UNINIT;

#define SD_STOP_SEC_VAR_CLEARED_8
#include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define SD_START_SEC_VAR_INIT_8
#include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Indication if module is initialized for the first time or if it is a re-initialization. */
SD_LOCAL VAR(boolean, SD_VAR_INIT) Sd_FirstInit = TRUE;

#define SD_STOP_SEC_VAR_INIT_8
#include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define SD_START_SEC_VAR_CLEARED_32
#include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if(SD_GET_RESET_MEASUREMENT_DATA_API == STD_ON)
/* Counter for received invalid SdMessages. */
SD_LOCAL VAR(uint32, SD_VAR_CLEARED) Sd_Measure_InvalidMessageCount = 0u;

/* Counter for received SubscribeNACK entries. */
SD_LOCAL VAR(uint32, SD_VAR_CLEARED) Sd_Measure_SubscribeNackCount = 0u;
#endif /* SD_GET_RESET_MEASUREMENT_DATA_API */

#define SD_STOP_SEC_VAR_CLEARED_32
#include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

P2CONST(Sd_ConfigType, SD_VAR_CLEARED, SD_PBCFG) Sd_ConfigDataPtr = NULL_PTR;

#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Sd_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SD_START_SEC_CODE
#include "Sd_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Sd_UtilClient_SetRequested()
 *********************************************************************************************************************/
/*! \brief          Configures the client as requested.
 *  \details        -
 *  \param[in]      ClientIdx               Index of the client.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_SetRequested(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilClient_SetReleased()
 *********************************************************************************************************************/
/*! \brief          Configures the client as released.
 *  \details        -
 *  \param[in]      ClientIdx               Index of the client.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_SetReleased(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_General()
 *********************************************************************************************************************/
/*! \brief          Initializes all general variable data.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_General(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_Instance()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of the instances.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Instance(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_PartitionData()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of a partition.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_PartitionData(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_SendEntryList()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable send entry data of instance.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of the instance.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_SendEntryList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_Server()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of the servers.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Server(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_Client()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of the clients.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Client(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_ClientCeg()
 *********************************************************************************************************************/
/*! \brief          Initializes all variable data of the consumed eventgroups.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ClientCeg(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_CloseAllSocketsOfInstance()
 *********************************************************************************************************************/
/*! \brief          Closes all used sockets of an SD Instance.
 *  \details        Function is called to re-initialize a SD Instance, all sockets are closed with Abort = TRUE.
 *  \param[in]      InstanceIdx             Index of instance which shall be initialized.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_CloseAllSocketsOfInstance(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_ReInitialize()
 *********************************************************************************************************************/
/*! \brief          Initializes all communication state dependent data.
 *  \details        Re-initializes data if the module state was changed by a call of Sd_Init() or
 *                  Sd_LocalIpAddrAssignmentChg().
 *  \param[in]      InstanceIdx             Index of instance which shall be initialized.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitialize(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_ReInitializeServers()
 *********************************************************************************************************************/
/*! \brief          Initializes the communication configuration of all servers of the given instance.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which shall be initialized.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeServers(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_ReInitializeEventHandler()
 *********************************************************************************************************************/
/*! \brief          Initializes the communication configuration of all event handlers of the given server.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeEventHandler(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_Process()
 *********************************************************************************************************************/
/*! \brief          Executes the server state machine.
 *  \details        Is called by the MainFunction and processes the state of the server.
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_StateServer_Process(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessNotReady()
 *********************************************************************************************************************/
/*! \brief          Handles the server state machine of the down phases.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessNotReady(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessInitialWait()
 *********************************************************************************************************************/
/*! \brief          Handles the server state machine of the initial wait phase.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessInitialWait(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessRepetition()
 *********************************************************************************************************************/
/*! \brief          Handles the server state machine of the repetition phase.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessRepetition(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessMain()
 *********************************************************************************************************************/
/*! \brief          Handles the server state machine of the main phase.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessMain(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessEventHandlerStateMachine()
 *********************************************************************************************************************/
/*! \brief          Handles the event handler state machines of the given server.
 *  \details        -
 *  \param[in]      ServerIdx               Index of the server to execute state machine.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessEventHandlerStateMachine(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_UpdateEventHandlerState()
 *********************************************************************************************************************/
/*! \brief          Updates the state of the event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_UpdateEventHandlerState(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_UpdateEventHandlerClients()
 *********************************************************************************************************************/
/*! \brief          Updates the state of all subscribed clients of an event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_UpdateEventHandlerClients(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerSwitchMulticast()
 *********************************************************************************************************************/
/*! \brief          Switches the event handler from unicast to multicast transmission mode.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerSwitchMulticast(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerSwitchUnicast()
 *********************************************************************************************************************/
/*! \brief          Switches the event handler from multicast to unicast transmission mode.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerSwitchUnicast(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerCheckSubscribed()
 *********************************************************************************************************************/
/*! \brief          Checks if the eventhandler is still requested by remote clients and changes its state accordingly.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerCheckSubscribed(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_DisableRouting()
 *********************************************************************************************************************/
/*! \brief          Disables routing for server events.
 *  \details        Disables all routing groups of the event handler and resets the activation count for all server
 *                  socket connections.
 *  \param[in]      EventHandlerIdx         Index of the event handler.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_DisableRouting(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

#if(SD_GET_RESET_MEASUREMENT_DATA_API == STD_ON)
/**********************************************************************************************************************
 *  Sd_Measure_Init()
 *********************************************************************************************************************/
/*! \brief          Initializes the measurement data counters to zero.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \config         This function is only available if SdGetAndResetMeasurementDataApi is enabled.
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Measure_Init(void);

/**********************************************************************************************************************
 *  Sd_Measure_GetAndReset()
 *********************************************************************************************************************/
/*! \brief          Gets and Resets (if requested) a measurement data counter value.
 *  \details        -
 *  \param[in]      MeasurementIdx          The index to select specific measurement data.
 *                                          [range: SD_MEAS_INVALID_SD_MESSAGE, SD_MEAS_SUBSCRIBE_NACK, SD_MEAS_ALL]
 *  \param[in]      MeasurementResetNeeded  Flag to indicate if the counter needs to be reset.
 *                                          [range: TRUE, FALSE]
 *  \param[out]     MeasurementDataPtr      Buffer where the value of the counter is to be copied into.
 *                                          [range: POINTER may be NULL_PTR]
 *  \pre            -
 *  \return         E_OK                    The operations were successful.
 *  \return         E_NOT_OK                The operations failed.
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \config         This function is only available if SdGetAndResetMeasurementDataApi is enabled.
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Measure_GetAndReset(
  Sd_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SD_APPL_VAR) MeasurementDataPtr);

/**********************************************************************************************************************
 *  Sd_Measure_Inc()
 *********************************************************************************************************************/
/*! \brief          Increments a measurement data counter.
 *  \details        -
 *  \param[in]      MeasurementIdx  The measurement index of the counter type to be incremented.
 *                                  [range: SD_MEAS_INVALID_SD_MESSAGE, SD_MEAS_SUBSCRIBE_NACK]
 *  \param[in]      Count           The count of increments to be made.
 *                                  [range: 0 - 65535]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \config         This function is only available if SdGetAndResetMeasurementDataApi is enabled.
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Measure_Inc(
  Sd_MeasurementIdxType MeasurementIdx,
  uint16 Count);

/**********************************************************************************************************************
 *  Sd_Measure_IncCounterValue()
 *********************************************************************************************************************/
/*! \brief          Increments the provided measurement data counter by a specified count.
 *  \details        -
 *  \param[in]      CounterPtr The pointer to the counter to be incremented.
 *                             [range: pointer should not be null pointer]
 *  \param[in]      Count      The count of increments to be made.
 *                             [range: 0 - 65535]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \config         This function is only available if SdGetAndResetMeasurementDataApi is enabled.
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Measure_IncCounterValue(
  P2VAR(uint32, AUTOMATIC, SD_APPL_VAR) CounterPtr,
  uint16 Count);
#endif /* SD_GET_RESET_MEASUREMENT_DATA_API */

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntry()
 *********************************************************************************************************************/
/*! \brief          Processes the attached server Entry Option.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which received the message.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Index of the entry which shall be processed.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntry(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntryFind()
 *********************************************************************************************************************/
/*! \brief          Reads and processes the attached FindService entry.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntryFind(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribe()
 *********************************************************************************************************************/
/*! \brief          Reads and processes the attached SubscribeEventgroup entry.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntrySubscribe(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_EntryIsStopSubscribe()
 *********************************************************************************************************************/
/*! \brief          Determines whether the given entry is a StopSubscribeEventgroup entry.
 *  \details        This function reads the TTL of the received entry and returns TRUE if the value is "0". The
 *                  function can only be used to differentiate between SubscribeEventgroup and StopSubscribeEventgroup
 *                  entries.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \return         TRUE                    Entry is a StopSubscribeEventgroup entry.
 *  \return         FALSE                   Entry is a SubscribeEventgroup entry.
 *  \pre            The user must ensure that the entry is either SubscribeEventgroup or StopSubscribeEventgroup.
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_RxServer_EntryIsStopSubscribe(
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_SearchEventHandler()
 *********************************************************************************************************************/
/*! \brief          Searches the event handler with the matching ID.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \return         ID of the matching event handler.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfEventHandlerType, SD_CODE) Sd_RxServer_SearchEventHandler(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_CheckConfigOptionByCbk()
 *********************************************************************************************************************/
/*! \brief          Triggers a user callout in order to validate the received configuration options.
 *  \details        -
 *  \param[in]      ServerIdx                   Corresponding server index.
 *                                              [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx                Index of partition.
 *                                              [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      RxPduId                     Receiving PDU-ID.
 *  \param[in]      RxEntryType                 Type of received entry.
 *  \param[in]      RxConfigOptionPtrArrayPtr   Configured configuration option pointer list.
 *  \return         SD_OPTIONS_OK               Message was accepted and shall be processed.
 *  \return         SD_OPTIONS_NOT_OK           Message was not accepted and shall be rejected/ignored.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxServer_CheckConfigOptionByCbk(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  PduIdType RxPduId,
  Sd_EntryType RxEntryType,
  SD_P2CONST(Sd_ConfigOptionStringType) RxConfigOptionPtrArrayPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionArray()
 *********************************************************************************************************************/
/*! \brief          Processes the attached option array.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         SD_OPTIONS_OK           Option array is valid.
 *  \return         SD_OPTIONS_NOT_OK       Invalid option array, this SD message is not valid.
 *  \return         SD_OPTIONS_WRONG_SUBNET Option is not topological correct (wrong subnet).
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxServer_ProcessOptionArray(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint16 IndexOfEntry,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionRun()
 *********************************************************************************************************************/
/*! \brief          Processes the attached server options run.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Start index of the Option.
 *  \param[in]      NrOfOptions             Number of successive attached Options in the run.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx <= Sd_GetSizeOfEventHandler()]
 *                                          [invalid: Sd_GetSizeOfEventHandler()]
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         SD_OPTIONS_OK           Option is valid.
 *  \return         SD_OPTIONS_NOT_OK       Invalid option, this SD message is not valid.
 *  \return         SD_OPTIONS_WRONG_SUBNET Option is not topological correct (wrong subnet).
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxServer_ProcessOptionRun(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  uint8 NrOfOptions,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionSubscribe()
 *********************************************************************************************************************/
/*! \brief          Processes the attached (Stop)SubscribeEventgroup option.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Start index of the Option.
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         SD_OPTIONS_OK           Option is valid.
 *  \return         SD_OPTIONS_NOT_OK       Invalid option, this SD message is not valid.
 *  \return         SD_OPTIONS_WRONG_SUBNET Option is not topological correct (wrong subnet).
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxServer_ProcessOptionSubscribe(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribeInternal()
 *********************************************************************************************************************/
/*! \brief          Completes processing of the received subscription and saves all required elements.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Option is valid.
 *  \return         E_NOT_OK                Invalid Option, this SD message is not valid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessEntrySubscribeInternal(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribeSaveParameters()
 *********************************************************************************************************************/
/*! \brief          Save all mandatory parameters of the received message for further processing.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ClientListIdx           Client index which triggered the event.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[out]     AttachedOptionsPtr      Structure containing all attached options.
 *  \return         E_OK                    Option is valid.
 *  \return         E_NOT_OK                Invalid Option, this SD message is not valid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessEntrySubscribeSaveParameters(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfClientListType ClientListIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_EventServer_Process()
 *********************************************************************************************************************/
/*! \brief          Processes all pending server events.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_Process(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_EventServer_DiscardEvent()
 *********************************************************************************************************************/
/*! \brief          Determines if currently processed RxEvent can be discarded.
 *  \details        If a following entry in the RxEvent queue will revert
 *                  the state of a CEG of a client back to the current state,
 *                  the currently processed entry of the RxEvent queue can be discarded.
 *  \param[in]      InstanceIdx             Corresponding instance index.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *                  CurrentRxEventIdx       Index of RxEvent which is currently being processed.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \return         TRUE                    Current RxEvent can be discarded.
 *  \return         FALSE                   Current RxEvent must not be discarded.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_DiscardEvent(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfRxEventType CurrentRxEventIdx);

/**********************************************************************************************************************
 *  Sd_EventServer_ProcessEvent()
 *********************************************************************************************************************/
/*! \brief          Processes the pending server event.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in,out]  PendingStopSubscribePtr Information if a StopSubscribeEventgroup entry is pending and has to be
 *                                          processed first.
 *  \return         TRUE                    The entry has to be kept for later processing.
 *  \return         FALSE                   The entry is processed and can be removed from the queue.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_ProcessEvent(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(boolean) PendingStopSubscribePtr);

/**********************************************************************************************************************
 *  Sd_StateServer_Update()
 *********************************************************************************************************************/
/*! \brief          Updates state machine of the server and sets timer event for the next main function cycle.
 *  \details        -
 *  \param[in]      ServerIdx               Server index to set the value and timer event.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      NewState                New state of the server service state machine.
 *  \param[in]      ResetTimeToTriggerSend  Indicates if the state machine has to be executed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_Update(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ServerStateMachineType NewState,
  boolean ResetTimeToTriggerSend);

/**********************************************************************************************************************
 *  Sd_UtilServer_UpdateEventHandlerStateMachine()
 *********************************************************************************************************************/
/*! \brief          Updates state machine of the event handler and sets a timer event for the next main function cycle.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Event handler to set the value and timer event.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      NewState                New state of the event handler state machine.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_UpdateEventHandlerStateMachine(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_EventHandlerStateMachineType NewState);

/**********************************************************************************************************************
 *  Sd_UtilServer_UpdateEventHandlerRequestState()
 *********************************************************************************************************************/
/*! \brief          Updates the request state of the event handler.
 *  \details        Updates the request state of the event handler, triggers a state machine execution and informs the
 *                  BswM about the state change.
 *  \param[in]      EventHandlerIdx         Event handler to set the value and timer event.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      NewState                New state of the event handler state machine.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_UpdateEventHandlerRequestState(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_EventHandlerCurrentStateType NewState);

/**********************************************************************************************************************
 *  Sd_RxServer_AddEventToRingBuffer()
 *********************************************************************************************************************/
/*! \brief          Adds an received event to the ring buffer.
 *  \details        -
 *  \param[in]      InstanceIdx             Relevant instance index.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      ClientListIdx           Client index which triggered the event.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      Event                   Event to process.
 *                                          [allowed: SD_SERVER_EVENT_EVENTGROUP_[STOP_]SUBSCRIBE[_REBOOT][_MULTI]]
 *  \return         E_OK                    Event successfully saved.
 *  \return         E_NOT_OK                Not able to save event for further processing.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_AddEventToRingBuffer(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ServerEventgroupEventType Event);

/**********************************************************************************************************************
 *  Sd_EventServer_GetFirstIdx()
 *********************************************************************************************************************/
/*! \brief          Gets index of first queued RxEvent.
 *  \details        The event will NOT be removed from the queue.
 *  \param[in]      InstanceIdx             Corresponding instance index.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[out]     EventIdxPtr             Index of first queued RxEvent.
 *                                          [range: RxEventIdx <= Sd_GetSizeOfRxEvent()]
 *                                          [invalid: Sd_GetSizeOfRxEvent()]
 *  \return         E_OK                    Event read and ready to process.
 *  \return         E_NOT_OK                No pending event to process.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_GetFirstIdx(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_SizeOfRxEventType) EventIdxPtr);

/**********************************************************************************************************************
 *  Sd_EventServer_RemoveFirst()
 *********************************************************************************************************************/
/*! \brief          Removes the first element of the RxEvent queue.
 *  \details        Handles also wrap around of the buffer and removes the saved endpoint information if not used by
 *                  other elements.
 *  \param[in]      InstanceIdx             Corresponding instance index for which the first event should be removed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            Expects the ringbuffer to contain at least one element.
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_RemoveFirst(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
*  Sd_EventServer_GetFollowingIdx()
*********************************************************************************************************************/
/*! \brief          Gets the index of a following entry of an entry in the the RxEvent queue.
 *  \details        -
 *  \param[in]      InstanceIdx             Corresponding instance index.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *                  RxEventIdx              Index of a valid RxEvent queue entry for which the index of the next
 *                                          entry shall be determined.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \return         Index to the next RxEvent. If there is no following event Sd_GetSizeOfRxEvent().
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfRxEventType, SD_CODE) Sd_EventServer_GetFollowingIdx(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfRxEventType RxEventIdx);

/**********************************************************************************************************************
 *  Sd_RxServer_SaveEndpoint()
 *********************************************************************************************************************/
/*! \brief          Searches for an unused slot and saves the address pointed by InAddrPtr.
 *  \details        -
 *  \param[in]      InstanceIdx                 Index of instance which handles this address.
 *                                              [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx                Index of partition.
 *                                              [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      InAddrPtr                   Pointer to address which shall be saved.
 *                                              [constraint: IP address has to be a valid IPv4/IPv6 address]
 *  \param[in]      Type                        Type of the endpoint address.
 *  \param[out]     OutEndpointStorageIdxPtr    Reference to the saved endpoint index.
 *                                              [range: OutEndpointStorageIdx <= Sd_GetSizeOfEndpointStorage()]
 *                                              [invalid: Sd_GetSizeOfEndpointStorage()]
 *  \return         E_OK                        Endpoint address saved.
 *  \return         E_NOT_OK                    Unable to save the endpoint address.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_SaveEndpoint(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_SockAddrContainerType) InAddrPtr,
  Sd_EndpointType Type,
  SD_P2VAR(Sd_SizeOfEndpointStorageType) OutEndpointStorageIdxPtr);

/**********************************************************************************************************************
 *  Sd_EventServer_RemoveEndpoint()
 *********************************************************************************************************************/
/*! \brief          Removes the saved endpoint address.
 *  \details        -
 *  \param[in,out]  EndpointStorageIdx      Index of the saved endpoint to be removed.
 *                                          [range: EndpointStorageIdx <= Sd_GetSizeOfEndpointStorage()]
 *                                          [invalid: Sd_GetSizeOfEndpointStorage()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         TRUE                    Endpoint is not referenced by an additional client.
 *  \return         FALSE                   Endpoint is referenced by another client and still required.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_RemoveEndpoint(
  Sd_SizeOfEndpointStorageType EndpointStorageIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_EventServer_Subscription()
 *********************************************************************************************************************/
/*! \brief          Processes the subscription of a remote client.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_Subscription(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitial()
 *********************************************************************************************************************/
/*! \brief          Processes the initial subscription of a remote client.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    Remote client successfully subscribed.
 *  \return         E_NOT_OK                Not able to subscribe remote client.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitial(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_EventServer_IsTriggerTransmitEnabled()
 *********************************************************************************************************************/
/*! \brief          Checks if the EventHandler associated with the RxEvent needs notification transmission.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         TRUE                    EventHandler is configured for triggered/notification event transmission.
 *  \return         FALSE                   EventHandler is not configured for triggered/notification event transmission.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_IsTriggerTransmitEnabled(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitialTcp()
 *********************************************************************************************************************/
/*! \brief          Configures the remote client for TCP.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    Remote client successfully configured.
 *  \return         E_NOT_OK                Not able to configure remote client.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitialTcp(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitialUdp()
 *********************************************************************************************************************/
/*! \brief          Configures the remote client for UDP.
 *  \details        -
 *  \param[in]      RxEventIdx              Index of the received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    Remote client successfully configured.
 *  \return         E_NOT_OK                Not able to configure remote client.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitialUdp(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_RxServer_AddClientToList()
 *********************************************************************************************************************/
/*! \brief          Adds a client to the client list of the event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      AddressStorageIdx       Remote IP address index of the client.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \return         Index of the saved client.
 *                  [invalid: Sd_GetSizeOfClientList()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfClientListType, SD_CODE) Sd_RxServer_AddClientToList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_SetDown()
 *********************************************************************************************************************/
/*! \brief          Sets the requested state of the server service to down.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server service index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different server.
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_SetDown(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_Configure()
 *********************************************************************************************************************/
/*! \brief          Configures the datapath for the server service and starts the initial wait phase.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server service index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different server.
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_Configure(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_RemoveClientsFromEventHandler()
 *********************************************************************************************************************/
/*! \brief          Removes all clients of the event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_RemoveClientsFromEventHandler(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_RemoveClientFromList()
 *********************************************************************************************************************/
/*! \brief          Removes a client from the client list of the event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ClientListIdx           Client index which should be removed.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_RemoveClientFromList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfClientListType ClientListIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_UnconfigureClientComPaths()
 *********************************************************************************************************************/
/*! \brief          Unconfigures the client communication paths.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ClientListIdx           Client index which should be removed.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_UnconfigureClientComPaths(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfClientListType ClientListIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_ReleaseEventHandlers()
 *********************************************************************************************************************/
/*! \brief          Releases all eventhandlers of the server.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_ReleaseEventHandlers(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_CloseTcpAndPreconfiguredSoCons()
 *********************************************************************************************************************/
/*! \brief          Closes all TCP socket and preconfigured UDP connections of the server.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_CloseTcpAndPreconfiguredSoCons(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_DisableMethodRouting()
 *********************************************************************************************************************/
/*! \brief          Disables the routing of provided methods.
 *  \details        -
 *  \param[in]      ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_DisableMethodRouting(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_LocalIpAddrAssignmentChgReset()
 *********************************************************************************************************************/
/*! \brief          Resets all dynamic server data in the case of a local IP address assignment change.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which has to be reset.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_LocalIpAddrAssignmentChgReset(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_GetSoConActivationMapIdx()
 *********************************************************************************************************************/
/*! \brief          Returns the activation map index for the requested SoConId.
 *  \details        The activation map allows it to handle multiple instances of a remote client.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      SoConId                 Requested socket connection ID.
 *  \return         Found SoConActivationMap index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SizeOfSoConActivationMapType, SD_CODE) Sd_UtilServer_GetSoConActivationMapIdx(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  Sd_UtilServer_IncreaseActivationCount()
 *********************************************************************************************************************/
/*! \brief          Increases the activation count for the requested SoConId.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      SoConId                 Requested socket connection ID.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_IncreaseActivationCount(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  Sd_UtilServer_DecreaseActivationCount()
 *********************************************************************************************************************/
/*! \brief          Decreases the activation count for the requested SoConId.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      SoConId                 Requested socket connection ID.
 *  \return         TRUE                    The activation count was decreased to zero.
 *  \return         FALSE                   The activation count was not decreased to zero.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_UtilServer_DecreaseActivationCount(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  Sd_UtilServer_ResetActivationCount()
 *********************************************************************************************************************/
/*! \brief          Resets the activation count for the requested SoConId.
 *  \details        -
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      SoConId                 Requested socket connection ID.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_ResetActivationCount(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  Sd_RxServer_FindClientInList()
 *********************************************************************************************************************/
/*! \brief          Searches for a client in the client-list of the event handler.
 *  \details        -
 *  \param[in]      EventHandlerIdx             Corresponding event handler index.
 *                                              [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx                Index of partition.
 *                                              [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      AddrStorageIdx              Remote SD Endpoint Idx of client.
 *                                              [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      AttachedOptionsPtr          Structure containing all attached options.
 *                                              [NULL_PTR is allowed if a reboot was detected.]
 *  \param[in]      CreateNewClientIfNotFound   Flag indicating if a new client shall be created if no corresponding
 *                                              client could be found.
 *  \return         Found client index.
 *                  [invalid: >= Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx)]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SizeOfClientListType, SD_CODE) Sd_RxServer_FindClientInList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddrStorageIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr,
  boolean CreateNewClientIfNotFound);

/**********************************************************************************************************************
 *  Sd_RxServer_CheckAttachedEndpointOptions()
 *********************************************************************************************************************/
/*! \brief          Checks the attached endpoint options.
 *  \details        Compares the received endpoint address with the configured one.
 *  \param[in]      EventHandlerIdx         Corresponding event handler index.
 *                                          [range: EventHandlerIdx < Sd_GetSizeOfEventHandler()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ClientListIdx           Corresponding client index.
 *                                          [range: ClientListIdx < Sd_GetSizeOfClientList()]
 *  \param[in]      AttachedOptionsPtr      Structure containing all attached options.
 *  \return         TRUE                    Attached endpoint options are valid.
 *  \return         FALSE                   Attached endpoint options are invalid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_RxServer_CheckAttachedEndpointOptions(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfClientListType ClientListIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_RxServer_CheckAttachedEndpointOption()
 *********************************************************************************************************************/
/*! \brief          Checks the attached UDP or TCP endpoint option.
 *  \details        Checks if the configured remote address is equivalent to the received endpoint address.
 *  \param[in]      EndpointStorageIdx      Index of the saved endpoint.
 *                                          [range: EndpointStorageIdx <= Sd_GetSizeOfEndpointStorage()]
 *                                          [invalid: Sd_GetSizeOfEndpointStorage()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      SoConId                 Configured socket connection ID.
 *  \param[in]      EndpointPtr             UDP or TCP endpoint address of the entry.
 *  \return         TRUE                    Attached endpoint option is valid.
 *  \return         FALSE                   Attached endpoint option is invalid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_RxServer_CheckAttachedEndpointOption(
  Sd_SizeOfEndpointStorageType EndpointStorageIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId,
  SD_P2CONST(Sd_SockAddrContainerType) EndpointPtr);

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Process()
 *********************************************************************************************************************/
/*! \brief          Processes all pending trigger transmit requests.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
  *                                         [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_Process(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Trigger()
 *********************************************************************************************************************/
/*! \brief          Triggers the transmission of initial events.
 *  \details        -
 *  \param[in]      TriggerTransmitIdx      Index of the trigger transmit event.
 *                                          [range: TriggerTransmitIdx < Sd_GetSizeOfTriggerTransmit()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_Trigger(
  Sd_SizeOfTriggerTransmitDynType TriggerTransmitIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Add()
 *********************************************************************************************************************/
/*! \brief          Adds an trigger transmit request to the ring buffer.
 *  \details        The trigger transmit request will be processed during the next main function.
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      RxEventIdx              Event index which caused the trigger transmit.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \return         E_OK                    Information if the event could be saved.
 *  \return         E_NOT_OK                Not able to save trigger transmit request.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_TriggerTransmit_Add(
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfRxEventType RxEventIdx);

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_ReadFirst()
 *********************************************************************************************************************/
/*! \brief          Reads the first event from the trigger transmit ring buffer.
 *  \details        The event will NOT be removed from the buffer.
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[out]     TriggerTransmitIdxPtr   Trigger transmit index.
 *  \return         E_OK                    Event successfully read and ready for processing.
 *  \return         E_NOT_OK                No more pending trigger transmit events.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_TriggerTransmit_ReadFirst(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_SizeOfTriggerTransmitDynType) TriggerTransmitIdxPtr);

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_RemoveFirst()
 *********************************************************************************************************************/
/*! \brief          Removes the first element from the ring buffer.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            Ring buffer has to contain at least one element.
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_RemoveFirst(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_ReInitializeClients()
 *********************************************************************************************************************/
/*! \brief          Initializes the communication configuration of all clients of the given instance.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which shall be initialized.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeClients(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Init_ReInitializeConsumedEventgroups()
 *********************************************************************************************************************/
/*! \brief          Initializes the communication configuration of all consumed eventgroups of the given client.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeConsumedEventgroups(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_Process()
 *********************************************************************************************************************/
/*! \brief          Processes the state of the client service.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_Process(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessStateMachine()
 *********************************************************************************************************************/
/*! \brief          Processes the state machine of the client service.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessStateMachine(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_SetStateReleased()
 *********************************************************************************************************************/
/*! \brief          Update state of client when request state is released.
 *  \details        -
 *  \param[in]      ClientIdx               Client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_SetStateReleased(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_HandleRetrySubscription()
 *********************************************************************************************************************/
/*! \brief          Handle the counter of the retry subscription feature.
 *  \details        Triggers DET runtime error if amount of retries exceeds configured value.
 *  \param[in]      ClientIdx               Client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_HandleRetrySubscription(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_CheckIfRetryActive()
 *********************************************************************************************************************/
/*! \brief          Checks if at least one consumed eventgroup of the client performs subscription retries.
 *  \details        -
 *  \param[in]      ClientIdx               Client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         TRUE                    At least one eventgroup is in retry subscription state.
 *  \return         FALSE                   No eventgroup of the client performs retries.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_CheckIfRetryActive(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessInitial()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the initial state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessInitial(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessNotRequestedNotSeen()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the not requested, not seen state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessNotRequestedNotSeen(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessNotRequestedSeen()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the not requested, seen state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessNotRequestedSeen(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessRequestedNotReady()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the not ready state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessRequestedNotReady(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessSearchingInitial()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the searching initial wait state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessSearchingInitial(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessSearchingRepetition()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the searching repetition state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessSearchingRepetition(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveSearching()
 *********************************************************************************************************************/
/*! \brief          Checks if the searching state can be left and triggers the state change.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveSearching(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveSearchingRcvOffer()
 *********************************************************************************************************************/
/*! \brief          Handles the reception of an OfferService message in the searching phase.
 *  \details        -
 *  \param[in]      ClientIdx               Client index which received the OfferService message.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveSearchingRcvOffer(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessReady()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the ready state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessReady(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessReadyEventReceived()
 *********************************************************************************************************************/
/*! \brief          Processes the received OfferService or StopOfferService message.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessReadyEventReceived(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessStopped()
 *********************************************************************************************************************/
/*! \brief          Handles the client state machine of the stopped state.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to execute state machine.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessStopped(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCeg()
 *********************************************************************************************************************/
/*! \brief          Handles all consumed eventgroup state maches of the client.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCeg(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegReset()
 *********************************************************************************************************************/
/*! \brief          Resets the consumed eventgroup if the client service is not ready anymore.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegReset(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegState()
 *********************************************************************************************************************/
/*! \brief          Handles the consumed eventgroup state machine.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegState(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegUnsubscribed()
 *********************************************************************************************************************/
/*! \brief          Handles the consumed eventgroup state machine while being unsubscribed.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegUnsubscribed(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSent()
 *********************************************************************************************************************/
/*! \brief          Handles the consumed eventgroup state machine while waiting for a response.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSent(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSentRxAck()
 *********************************************************************************************************************/
/*! \brief          Processes the received SubscribeEventgroupAck message.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSentRxAck(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSentRxOffer()
 *********************************************************************************************************************/
/*! \brief          Processes the received OfferService message while waiting for the SubscribeEventgroupAck message.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSentRxOffer(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegSubscribed()
 *********************************************************************************************************************/
/*! \brief          Handles the consumed eventgroup state machine while being Subscribed.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegSubscribed(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_Configure()
 *********************************************************************************************************************/
/*! \brief          Configures the client on service level.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    All socket connections and routing groups updated.
 *  \return         E_NOT_OK                Unable to update all socket connections and routing groups.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_Configure(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_ConfigureMethod()
 *********************************************************************************************************************/
/*! \brief          Configures the socket connection and routing group for the client service method.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in,out]  MethodSoConIdPtr        Socket connection IF of the corresponding method.
 *  \param[in]      ServiceStartSoConId     Socket connection ID used by the service.
 *  \param[in]      ServerEndpointAddrPtr   Server endpoint address.
 *  \return         E_OK                    Socket connection and routing group updated.
 *  \return         E_NOT_OK                Unable to update socket connection and routing group.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_ConfigureMethod(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(SoAd_SoConIdType) MethodSoConIdPtr,
  SoAd_SoConIdType ServiceStartSoConId,
  SD_P2CONST(Sd_SockAddrContainerType) ServerEndpointAddrPtr);

/**********************************************************************************************************************
 *  Sd_StateClient_Unconfigure()
 *********************************************************************************************************************/
/*! \brief          Unconfigures all socket connections and routing groups for the client service.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ResetEndpointInfo       Specifies if unicast as well as multicast endpoint address shall be reset.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_Unconfigure(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean ResetEndpointInfo);

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroup()
 *********************************************************************************************************************/
/*! \brief          Updates all socket connections and routing groups of the eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    All socket connections and routing groups updated.
 *  \return         E_NOT_OK                Unable to update all socket connections and routing groups.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroup(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupTcp()
 *********************************************************************************************************************/
/*! \brief          Updates the TCP configuration of the eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    Socket connection and routing group updated.
 *  \return         E_NOT_OK                Unable to update the socket connection and routing group.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupTcp(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupUdp()
 *********************************************************************************************************************/
/*! \brief          Updates the UDP configuration of the eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    Socket connection and routing group updated.
 *  \return         E_NOT_OK                Unable to update the socket connection and routing group.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupUdp(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

#if(SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_CheckMulticastSoConRemoteAddress()
 *********************************************************************************************************************/
/*! \brief          Checks the configured remote address of the multicast socket.
 *  \details        If the received unicast remote address of the server is different from the configured value, the
 *                  configuration is reset.
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK
 *  \return         E_NOT_OK
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_CheckMulticastSoConRemoteAddress(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);
#endif

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceived()
 *********************************************************************************************************************/
/*! \brief          Updates the configuration based on the received SubscribeEventgroupAck.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    All socket connections and routing groups updated.
 *  \return         E_NOT_OK                Unable to update all socket connections and routing groups.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceived(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceivedInitial()
 *********************************************************************************************************************/
/*! \brief          Handles the initial multicast configuration of the consumed eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    Configured the consumed eventgroup for multicast.
 *  \return         E_NOT_OK                Unable to configure the consumed eventgroup for multicast.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceivedInitial(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceivedRenew()
 *********************************************************************************************************************/
/*! \brief          Renews the multicast configuration of the consumed eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         E_OK                    Renewed the multicast configuration of the consumed eventgroup.
 *  \return         E_NOT_OK                Unable to renew the multicast configuration of the consumed eventgroup.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceivedRenew(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoCon()
 *********************************************************************************************************************/
/*! \brief          Searches for the socket connection with the best local and remote address match.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[out]     MulticastSoConIdPtr     Best matching socket connection ID.
 *  \param[out]     MatchTypePtr            Match types of the local and remote address.
 *  \return         E_OK                    Matching/suitable socket connection found.
 *  \return         E_NOT_OK                No suitable socket connection found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoCon(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_SizeOfMulticastSoConIdType) MulticastSoConIdPtr,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) MatchTypePtr);

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState()
 *********************************************************************************************************************/
/*! \brief          Calculates the state of the given address.
 *  \details        The state describes if port or address is set to a unique value or defines ANY value.
 *  \param[in]      AddrPtr                 Address which is used for state calculation.
 *  \return         addrState               State of the given address.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(uint8, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState(
  SD_P2CONST(Sd_SockAddrContainerType) AddrPtr);

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType()
 *********************************************************************************************************************/
/*! \brief          Calculates the local match type of the given multicast socket connection.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      MulticastSoConIdIdx     Multicast socket connection ID of.
 *                                          [range: MulticastSoConIdx < Sd_GetSizeOfMulticastSoCon()]
 *  \param[out]     LocalMatchTypePtr       Match types of the local address.
 *  \return         E_OK                    Match type calculated.
 *  \return         E_NOT_OK                Not able to calculate match type.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfMulticastSoConIdType MulticastSoConIdIdx,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) LocalMatchTypePtr);

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType()
 *********************************************************************************************************************/
/*! \brief          Calculates the remote match type of the given multicast socket connection.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      MulticastSoConIdIdx     Multicast socket connection ID of.
 *                                          [range: MulticastSoConIdx < Sd_GetSizeOfMulticastSoCon()]
 *  \param[out]     RemoteMatchTypePtr      Match types of the remote IP address.
 *  \return         E_OK                    Match type calculated.
 *  \return         E_NOT_OK                Not able to calculate match type.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfMulticastSoConIdType MulticastSoConIdIdx,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) RemoteMatchTypePtr);

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio()
 *********************************************************************************************************************/
/*! \brief          Calculates the local match priority of the given multicast socket connection.
 *  \details        -
 *  \param[in]      MatchTypePtr            Match types of the local address.
 *  \param[in,out]  LocalMatchPrioPtr       Match priority of the local address.
 *  \param[in,out]  RemoteMatchPrioPtr      Match priority of the remote address.
 *  \return         TRUE                    Found local match.
 *  \return         FALSE                   Local address does not match.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio(
  SD_P2CONST(Sd_LocalRemoteMatchTypeType) MatchTypePtr,
  SD_P2VAR(uint8) LocalMatchPrioPtr,
  SD_P2VAR(uint8) RemoteMatchPrioPtr);

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio()
 *********************************************************************************************************************/
/*! \brief          Calculates the remote match priority of the given multicast socket connection.
 *  \details        -
 *  \param[in]      MatchTypePtr            Match types of the remote address.
 *  \param[in,out]  RemoteMatchPrioPtr      Match priority of the remote address.
 *  \return         TRUE                    Found local match.
 *  \return         FALSE                   Local address does not match.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio(
  SD_P2CONST(Sd_LocalRemoteMatchTypeType) MatchTypePtr,
  SD_P2VAR(uint8) RemoteMatchPrioPtr);

/**********************************************************************************************************************
 *  Sd_StateClient_ReleaseCeg()
 *********************************************************************************************************************/
/*! \brief          Unconfigures the consumed eventgroup and informs the BswM if required.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ReleaseCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_StateClient_Update()
 *********************************************************************************************************************/
/*! \brief          Updates state machine of the client and sets a timer event for the next main function cycle.
 *  \details        -
 *  \param[in]      ClientIdx               Client index to set the value and timer event.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      NewState                New state of the client service state machine.
 *  \param[in]      ResetTimeToTriggerSend  Set TimeToTriggerSend to the invalid value.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_Update(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ClientStateMachineType NewState,
  boolean ResetTimeToTriggerSend);

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateCeg()
 *********************************************************************************************************************/
/*! \brief          Updates state machine of the consumed eventgroup.
 *  \details        Sets a timer event for the next main function cycle.
 *  \param[in]      ConsumedEventgroupIdx   Consumed eventgroup index to set the value and timer event.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      NewState                New state of the consumed eventgroup state machine.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_UpdateCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ConsumedEventGroupStateMachineType NewState);

/**********************************************************************************************************************
 *  Sd_StateClient_UnconfigureCeg()
 *********************************************************************************************************************/
/*! \brief          Unconfigures all socket connections and routing groups of the consumed eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Consumed eventgroup index to unconfigure.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      AbortFlag               Forces abort of socket connections.
 *  \param[in]      ResetEndpointInfo       Specifies if the multicast endpoint address shall be reset.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_UnconfigureCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean AbortFlag,
  boolean ResetEndpointInfo);

/**********************************************************************************************************************
 *  Sd_StateClient_LocalIpAlreadyConfigured()
 *********************************************************************************************************************/
/*! \brief          Check if the requested multicast IP address is already configured at another socket.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      MulticastAddressPtr     Requested multicast IP address.
 *  \return         TRUE                    Matching/suitable socket connection found.
 *  \return         FALSE                   No suitable socket connection found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_LocalIpAlreadyConfigured(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_SockAddrContainerType) MulticastAddressPtr);

/**********************************************************************************************************************
 *  Sd_StateClient_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief          Requests the given address as local IP address.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      SoConId                 Socket connection ID which shall be configured.
 *  \param[in]      MulticastAddrPtr        Multicast address which shall be configured as local address.
 *  \param[in]      PerformRequest          Indicating if the address shall be configured or the request counter incremented.
 *  \return         E_OK                    Request successfully processed.
 *  \return         E_NOT_OK                Not able to configure the local address.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_RequestIpAddrAssignment(
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) MulticastAddrPtr,
  boolean PerformRequest);

/**********************************************************************************************************************
 *  Sd_StateClient_ReleaseIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief          Releases the local IP address
 *  \details        The local address is not released, if it is still required by another client.
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      SoConId                 Socket connection ID which is configured with the local address.
 *  \return         E_OK                    Local address was successfully released.
 *  \return         E_NOT_OK                Not able to release local address.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_ReleaseIpAddrAssignment(
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  Sd_StateClient_EntryListContainsSubscribe()
 *********************************************************************************************************************/
/*! \brief          Checks if a SubscribeEventgroup message is still pending.
 *  \details        -
 *  \param[in,out]  ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_EntryListContainsSubscribe(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntry()
 *********************************************************************************************************************/
/*! \brief          Processes the attached client entry option.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of the instance which received the message.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Index of the entry which shall be processed.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntry(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryOffer()
 *********************************************************************************************************************/
/*! \brief          Processes the received OfferService entry.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryOffer(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryOfferOptionRuns()
 *********************************************************************************************************************/
/*! \brief          Processes the options run of the received OfferService entry.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         SD_OPTIONS_OK           Message was accepted and shall be processed.
 *  \return         SD_OPTIONS_NOT_OK       Message was not accepted and shall be rejected/ignored.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxClient_ProcessEntryOfferOptionRuns(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryAcknowledge()
 *********************************************************************************************************************/
/*! \brief          Processes the attached SubscribeEventgroupAck entry.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      IndexOfEntry            Start index of the Entry-Option.
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryAcknowledge(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_RxClient_CheckConfigOptionByCbk()
 *********************************************************************************************************************/
/*! \brief          Triggers a user callout in order to validate the received configuration options.
 *  \details        If no user callout is configured, the options are accepted.
 *  \param[in]      ClientIdx                   Corresponding client index.
 *                                              [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx                Index of partition.
 *                                              [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      RxPduId                     Receiving PDU-ID.
 *  \param[in]      RxEntryType                 Type of received entry.
 *  \param[in]      RxConfigOptionPtrArrayPtr   Configured configuration option pointer list.
 *  \return         SD_OPTIONS_OK               Message was accepted and shall be processed.
 *  \return         SD_OPTIONS_NOT_OK           Message was not accepted and shall be rejected/ignored.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxClient_CheckConfigOptionByCbk(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  PduIdType RxPduId,
  Sd_EntryType RxEntryType,
  SD_P2CONST(Sd_ConfigOptionStringType) RxConfigOptionPtrArrayPtr);

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryAcknowledgeSave()
 *********************************************************************************************************************/
/*! \brief          Processes the attached SubscribeEventgroupAck entry.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx    Corresponding consumed eventgroup index.
 *                                           [range: ConsumedEventgroupIdx <= Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx             Index of partition.
 *                                           [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      MulticastEndpointAddrPtr Received multicast endpoint.
 *  \param[in]      RxTTL                    TTL of the received acknowledge.
 *  \param[in]      RxSdEp                   SD Endpoint from which Acknowledge has been received.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryAcknowledgeSave(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_SockAddrContainerType) MulticastEndpointAddrPtr,
  uint32 RxTTL,
  Sd_SizeOfAddressStorageType RxSdEp);

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessOptionRun()
 *********************************************************************************************************************/
/*! \brief          Processes the attached client option run.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Start index of the Option.
 *  \param[in]      NrOfOptions             Number of succeeding Options which are attached to the Entry-Option.
 *  \param[in]      EMalformedMsgId         DEM error id for malformed messages.
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx <= Sd_GetSizeOfConsumedEventgroup()]
 *                                          [invalid: Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         SD_OPTIONS_OK           Option array is valid.
 *  \return         SD_OPTIONS_NOT_OK       Invalid option array, this SD message is not valid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxClient_ProcessOptionRun(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  uint8 NrOfOptions,
  Sd_Dem_EventIdType EMalformedMsgId,
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_Util_GetClientServiceId()
 *********************************************************************************************************************/
 /*! \brief         Function returns the ServiceId of a client at position ClientIdx.
 *  \details        -
 *  \param[in]      Idx                     Index of the Sd_Client[] array.
 *  \param[in]      PartitionIdx            Index of partition.
 *  \return         ServiceId of the client at the position ClientIdx.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(uint16, SD_CODE) Sd_Util_GetClientServiceId(
  uint16 Idx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_GetServerServiceId()
 *********************************************************************************************************************/
 /*! \brief         Function returns the ServiceId of a server at position ServerIdx.
 *  \details        -
 *  \param[in]      Idx                     Index of the Sd_Server[] array.
 *  \param[in]      PartitionIdx            Index of partition.
 *  \return         ServiceId of the server at the position ServerIdx.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(uint16, SD_CODE) Sd_Util_GetServerServiceId(
  uint16 Idx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_GetServices()
 *********************************************************************************************************************/
/*! \brief          Searches for all services with the given ServiceId within the given range.
 *  \details        The function ServiceIdAccessFuncPtr has to be valid to access indices of the following
 *                  range: [(*ServiceStartIdxPtr) .. (*ServiceEndIdxPtr)]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ServiceId               Relevant ServiceId.
 *  \param[in]      ServiceIdAccessFuncPtr  Function which can be used to read the ServiceId at a given location.
 *  \param[in,out]  ServiceStartIdxPtr      Start index for relevant entries.
 *  \param[in,out]  ServiceEndIdxPtr        End index for relevant entries.
 *                                          Note: The ServiceEndIdx value defines the first invalid index.
 *                                          [range: 1 <= ServiceEndIdxPtr <= Sd_GetSizeOf...()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \note           The ServiceStartIdx and ServiceEndIdx values share the same logic as the StartIdx and EndIdx values
 *                  generated by ComStackLib. Hence, these values can be used as input parameters.
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_GetServices(
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 ServiceId,
  Sd_ServiceIdGetterFctType ServiceIdAccessFuncPtr,
  SD_P2VAR(uint16) ServiceStartIdxPtr,
  SD_P2VAR(uint16) ServiceEndIdxPtr);

/**********************************************************************************************************************
 *  Sd_Copy()
 *********************************************************************************************************************/
 /*! \brief          Internal MemCpy function.
  *  \details        -
  *  \param[out]     pDst          Pointer to the memory location to copy to, must not be NULL.
  *  \param[in]      pSrc          Pointer to the memory location to copy from, must not be NULL.
  *  \param[in]      nCnt          Number of bytes to copy, pDst must be valid for this amount.
  *  \pre            -
  *  \context        TASK|ISR2
  *  \reentrant      FALSE
  *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Copy(
  SD_P2VAR(void) pDst,
  SD_P2CONST(void) pSrc,
  uint32_least nCnt);

/**********************************************************************************************************************
 *  Sd_Rx_SaveAddr()
 *********************************************************************************************************************/
/*! \brief          Saves the address pointed by InAddrPtr.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which handles this address.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      InAddrPtr               Address which shall be saved.
 *  \return         Index of the saved address.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfAddressStorageType, SD_CODE) Sd_Rx_SaveAddr(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_SockAddrContainerType) InAddrPtr);

/**********************************************************************************************************************
 *  Sd_Rx_IndexOptionArray()
 *********************************************************************************************************************/
/*! \brief          Reads all attached options and generates an index in order to ease the access of individual options.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_IndexOptionArray(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr);

/**********************************************************************************************************************
 *  Sd_Rx_CheckOption()
 *********************************************************************************************************************/
/*! \brief          Checks if the option is feasible.
 *  \details        -
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      OptionIndex             Payload offset of the option.
 *  \param[in]      OptionArrayLength       Length of the options array.
 *  \param[out]     CoveredOptionLengthPtr  Covered length of the option.
 *  \return         E_OK                    Option is valid.
 *  \return         E_NOT_OK                Option is invalid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_CheckOption(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint16 OptionIndex,
  uint32 OptionArrayLength,
  SD_P2VAR(uint16) CoveredOptionLengthPtr);

/**********************************************************************************************************************
 *  Sd_Rx_CheckRemoteReboot()
 *********************************************************************************************************************/
/*! \brief          Checks if the remote service has rebooted.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \param[in]      SdHeaderPtr             Header information.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_CheckRemoteReboot(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2CONST(Sd_HeaderType) SdHeaderPtr);

/**********************************************************************************************************************
 *  Sd_Rx_GetSdEndpointOption()
 *********************************************************************************************************************/
/*! \brief          Checks if an SD endpoint option was attached to the message and extracts it.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[out]     SdEndpointAddressPtr    Pointer to a structure where the SD endpoint address shall be stored.
 *  \return         E_OK                    SD endpoint is valid or no option is attached.
 *  \return         E_NOT_OK                The attached SD endpoint option is invalid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_GetSdEndpointOption(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  SD_P2VAR(Sd_SockAddrContainerType) SdEndpointAddressPtr);

/**********************************************************************************************************************
 *  Sd_Rx_GetEndpointOptionParameters()
 *********************************************************************************************************************/
/*! \brief          Reads the parameters of the received endpoint options.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Index of the Option.
 *  \param[in]      McType                  Specifies if unicast, multicast or any endpoints shall be read.
 *  \param[out]     FamilyTypePtr           Read address family of the option.
 *  \param[out]     L4ProtocolTypePtr       Read L4 protocol type of the option.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Rx_GetEndpointOptionParameters(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  SdMulticastTypeType McType,
  SD_P2VAR(Sd_FamilyType) FamilyTypePtr,
  SD_P2VAR(Sd_L4ProtocolType) L4ProtocolTypePtr);

/**********************************************************************************************************************
 *  Sd_Rx_ReadEndpointOptions()
 *********************************************************************************************************************/
/*! \brief          Reads the received endpoint options.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Index of the Option.
 *  \param[in]      FamilyType              Address family.
 *  \param[in]      L4ProtocolType          L4 protocol type of the option.
 *  \param[in,out]  AttachedOptionsPtr      Structure containing all attached options.
 *  \return         SD_OPTIONS_OK           Endpoint is valid.
 *  \return         SD_OPTIONS_NOT_OK       Invalid Endpoint.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_ReadEndpointOptions(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  Sd_FamilyType FamilyType,
  Sd_L4ProtocolType L4ProtocolType,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_Rx_ReadEndpointOption()
 *********************************************************************************************************************/
/*! \brief          Reads the received endpoint option.
 *  \details        -
 *  \param[in]      FamilyType              Address family.
 *  \param[in]      DataPtr                 Data pointer to the received payload.
 *  \param[in]      DataIndex               Index of the option.
 *  \param[out]     EndpointAddrPtr         Reference where the endpoint address shall be stored.
 *  \return         SD_OPTIONS_OK           Endpoint is valid.
 *  \return         SD_OPTIONS_NOT_OK       Invalid Endpoint.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_ReadEndpointOption(
  Sd_FamilyType FamilyType,
  SD_P2CONST(uint8) DataPtr,
  uint16 DataIndex,
  SD_P2VAR(Sd_SockAddrContainerType) EndpointAddrPtr);

/**********************************************************************************************************************
 *  Sd_Rx_CheckConfigOption()
 **********************************************************************************************************************/
/*! \brief          Checks the received configuration option.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[in]      IndexOfOption           Index of the Option.
 *  \param[in,out]  ConfigOptionPtrArrayPtr Pointer to the Configuration Option Pointer Array.
 *  \return         SD_OPTIONS_OK           Configuration option is valid.
 *  \return         SD_OPTIONS_NOT_OK       Invalid configuration option.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 **********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_CheckConfigOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  SD_P2VAR(Sd_ConfigOptionStringType) ConfigOptionPtrArrayPtr);

/**********************************************************************************************************************
 *  Sd_Rx_FillConfigOptionPtrArray()
 **********************************************************************************************************************/
/*! \brief          Adds information to the configuration option pointer array.
 *  \details        Adds the hostname and capability record configuration options to the configuration
 *                  option pointer array.
 *  \param[in,out]  ConfigOptionPtrArrayPtr   Pointer to the Configuration Option Pointer Array.
 *  \param[in]      InstanceCapabilityIdx     Capability record index of the instance.
 *  \param[in]      ServiceCapabilityIdx      Capability record index of the service.
 *  \param[in]      ServiceCapabilityInvIdx   Invalid capability record index of the service.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 **********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_FillConfigOptionPtrArray(
  SD_P2VAR(Sd_ConfigOptionStringType) ConfigOptionPtrArrayPtr,
  Sd_SizeOfCapabilityRecordType InstanceCapabilityIdx,
  Sd_SizeOfCapabilityRecordType ServiceCapabilityIdx,
  Sd_SizeOfCapabilityRecordType ServiceCapabilityInvIdx);

/**********************************************************************************************************************
 *  Sd_State_HandleRunMode()
 *********************************************************************************************************************/
/*! \brief          Handles the run mode of the given instance.
 *  \details        The RUN_MODE of the instance considers the state of the initialization as well as the IP address
 *                  assignment state.
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_State_HandleRunMode(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_State_Process()
 *********************************************************************************************************************/
/*! \brief          Processes all state machines of clients and servers.
 *  \details        State machines are only executed if the corresponding timer is exceeded.
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_State_Process(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Rx_Process()
 *********************************************************************************************************************/
/*! \brief          Processes received message.
 *  \details        Validates message header, retrieves attached options and processes entries.
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      RxSoConId               Reception socket connection ID.
 *  \param[in]      MessageInfoPtr          Information about the received message.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Rx_Process(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType RxSoConId,
  SD_P2VAR(Sd_RxMessageInfoType) MessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_Rx_ProcessHeader()
 *********************************************************************************************************************/
/*! \brief          Validates the message header and reads the header fields.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PduInfoPtr              Contains the length (SduLength) of the received I-PDU and a pointer to a
 *                                          buffer (SduDataPtr) containing the I-PDU.
 *  \param[out]     SdHeaderPtr             Pointer where to store the header.
 *  \return         E_OK                    Header is valid.
 *  \return         E_NOT_OK                Invalid Header, SD message is not valid.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_ProcessHeader(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  SD_P2VAR(Sd_HeaderType) SdHeaderPtr);

/**********************************************************************************************************************
 *  Sd_Rx_ProcessEntryArray()
 *********************************************************************************************************************/
/*! \brief          Processes all attached entries of the message.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      RxMessageInfoPtr        Information about the received message.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_ProcessEntryArray(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr);

/**********************************************************************************************************************
 *  Sd_HandleReboot()
 *********************************************************************************************************************/
/*! \brief          Processes one pending detected reboot for a remote SD instance.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         TRUE                    Reboot processing is pending, do not process server events and state machines.
 *  \return         FALSE                   Reboot processing is not pending.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_HandleReboot(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_HandleReboot_StopEvents()
 *********************************************************************************************************************/
/*! \brief          Handles the remote reboot.
 *  \details        Searches all client and server services which are affected by the reboot of the
 *                  remote service and handles the reboot.
 *  \param[in]      InstanceIdx             Index of the instance which received the message.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      AddrStorageIdx          Information about the received message.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_HandleReboot_StopEvents(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddrStorageIdx);

/**********************************************************************************************************************
 *  Sd_HandleReboot_SoConsUdp()
 *********************************************************************************************************************/
/*! \brief          Handles the UDP socket connections of the rebooted remote client.
 *  \details        Closes and reopens all UDP socket connections which correspond to the rebooted client.
 *  \param[in]      InstanceIdx             Index of the instance which received the message.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      AddrStorageIdx          Address storage index of the rebooted remote SD Instance.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_HandleReboot_SoConsUdp(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddrStorageIdx);

/**********************************************************************************************************************
 *  Sd_HandleReboot_SoConsTcp()
 *********************************************************************************************************************/
/*! \brief          Handles the TCP socket connections of the rebooted remote client.
 *  \details        Releases remote address of all TCP socket connections which correspond to the rebooted client.
 *  \param[in]      InstanceIdx             Index of the instance which received the message.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      AddrStorageIdx          Address storage index of the rebooted remote SD Instance.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_HandleReboot_SoConsTcp(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddrStorageIdx);

/**********************************************************************************************************************
 *  Sd_Tx_ProcessPendingMessages()
 *********************************************************************************************************************/
/*! \brief          Processes all pending messages which are ready to transmit (TimeToTriggerSend).
 *  \details        If there is a ready message, also all pending messages with the same destination address
 *                  will be transmitted.
 *  \param[in]      InstanceIdx             Index of instance which shall be processed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ForceTransmission       If set to TRUE, the TimeToSend is ignored in order to transmit all pending
 *                                          messages directly.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_ProcessPendingMessages(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean ForceTransmission);

/**********************************************************************************************************************
 *  Sd_Tx_CheckIfMessageReadyForTransmission()
 *********************************************************************************************************************/
/*! \brief          Checks if there is a message which is ready for transmission.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which shall be processed.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ForceSend               If set to TRUE, the TimeToSend is ignored in order to transmit all pending
 *                                          messages directly.
 *  \return         Destination index of the address storage array - if there is a message ready for transmission.
 *  \return         Sd_GetSizeOfAddressStorage() - if there is no message ready for transmission.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SizeOfAddressStorageType, SD_CODE) Sd_Tx_CheckIfMessageReadyForTransmission(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean ForceSend);

/**********************************************************************************************************************
 *  Sd_Tx_CheckIfEntryShallBeSkipped()
 *********************************************************************************************************************/
/*! \brief          Checks if the referenced entry is ready for transmission.
 *  \details        Otherwise the transmission of the entry has to be postponed.
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         SD_SENDENTRY_ACCEPT     Entry is ready for transmission.
 *  \return         SD_SENDENTRY_SKIP       TCP connection is not established, skip Entry.
 *  \return         SD_SENDENTRY_DELETE     Entry is not required anymore, delete it.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Sd_SendEntrySkipType, SD_CODE) Sd_Tx_CheckIfEntryShallBeSkipped(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Tx_CheckAndRestartRetry()
 *********************************************************************************************************************/
 /*! \brief          Resets the retry subscription timer if configured.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_CheckAndRestartRetry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeMessage()
 *********************************************************************************************************************/
/*! \brief          Serializes pending entries to a single SD message and triggers the transmission.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which triggers transmission.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      AddressStorageIdx       Address with ready SendEntries to be serialized.
 *                                          [range: 0 < Sd_GetSizeOfAddressStorage() - 1]
 *  \return         Number of successfully serialized entries.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SizeOfSendEntryType, SD_CODE) Sd_Tx_SerializeMessage(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeHeader()
 *********************************************************************************************************************/
/*! \brief          Serializes the header information of the Sd message.
 *  \details        -
 *  \param[in]      AddressStorageIdx       Address storage index defining the destination to transmit messages.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_SerializeHeader(
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeRelatedOptions()
 *********************************************************************************************************************/
/*! \brief          Serializes all options of the referenced entry to the payload.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[out]     EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \return         TRUE                    Buffer was too small to serialize related options.
 *  \return         FALSE                   All related options were serialized to the payload.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeRelatedOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEntryAndOptions()
 *********************************************************************************************************************/
/*! \brief          Serializes the referenced entry and corresponding options to the payload.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      PayloadEntryIdx         Payload start index to serialize entry to.
 *                                          [range: PayloadEntryIdx < Sd_GetSizeOfPayload() - SD_ENTRY_SIZE]
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \return         TRUE                    Buffer was too small to serialize related options.
 *  \return         FALSE                   All related options were serialized to the payload.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeEntryAndOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 PayloadEntryIdx,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_GetRelevantEndpointOptions()
 *********************************************************************************************************************/
/*! \brief         Gets endpoint options which shall be attached to the SD entry.
 *  \details        -
 *  \param[in]      SendEntryIdx              Index of entry to send.
 *                                            [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PartitionIdx              Index of partition.
 *                                            [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[out]     UdpEndpointPtr            UDP endpoint information which shall be set.
 *  \param[out]     TcpEndpointPtr            TCP endpoint information which shall be set.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_GetRelevantEndpointOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EndpointOptionTxType) UdpEndpointPtr,
  SD_P2VAR(Sd_EndpointOptionTxType) TcpEndpointPtr);

/**********************************************************************************************************************
 *  Sd_Tx_GetRelevantConfigOptions()
 *********************************************************************************************************************/
/*! \brief          Analyzes if a configuration option has to be attached at the message.
 *  \details        -
 *  \param[in]      SendEntryIdx                  Index of entry to send.
 *                                                [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PartitionIdx                  Index of partition.
 *                                                [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[out]     HostnameStartIdxPtr           Start index of hostname.
 *  \param[out]     HostnameEndIdxPtr             End index of hostname.
 *  \param[out]     CapabilityRecordStartIdxPtr   Start index of capability record.
 *  \param[out]     CapabilityRecordEndIdxPtr     End index of capability record.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_GetRelevantConfigOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) HostnameStartIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) HostnameEndIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) CapabilityRecordStartIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) CapabilityRecordEndIdxPtr);

/**********************************************************************************************************************
 *  Sd_Tx_CheckForDuplicateOption()
 *********************************************************************************************************************/
/*! \brief          Checks the payload for duplicated options.
 *  \details        Reads the serialized payload and checks if the option which was serializes after the end of the
 *                  options array can be deduplicated.
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_CheckForDuplicateOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_DeduplicateOption()
 *********************************************************************************************************************/
/*! \brief          Tries to deduplicate the option.
 *  \details        Not all possible combinations of options can be deduplicated, because all relevant options of the
 *                  message have to be referenced by the two option runs.
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in,out]  EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \param[in]      RemainingOption         Indicates if the current option is the last one which has to be processed.
 *  \return         TRUE                    Option was deduplicated.
 *  \return         FALSE                   Algorithm was not able to deduplicate option.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Tx_DeduplicateOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr,
  boolean RemainingOption);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEndpointOptions()
 *********************************************************************************************************************/
/*! \brief          Serializes all required endpoint options to the payload.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in,out]  EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \return         TRUE                    Buffer was too small to serialize related endpoint options.
 *  \return         FALSE                   All related options were serialized to the payload.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeEndpointOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEndpointOption()
 *********************************************************************************************************************/
/*! \brief          Serializes an endpoint option to the payload.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      EndpointOptionPtr       Endpoint Option which shall be serialized.
 *  \param[in]      L4Proto                 Which L4-Protocol (UDP/TCP) has the option.
 *  \param[in]      PayloadOffset           Start offset for the option.
 *  \return         The length of the serialized option.
 *                  [invalid: SD_INV_OPTION_LENGTH if buffer is too small]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(uint16, SD_CODE) Sd_Tx_SerializeEndpointOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_EndpointOptionTxType) EndpointOptionPtr,
  Sd_L4ProtocolType L4Proto,
  uint16 PayloadOffset);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeConfigurationOptions()
 *********************************************************************************************************************/
/*! \brief          Serializes all required configuration options to the payload.
 *  \details        -
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in,out]  EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeConfigurationOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr);

/**********************************************************************************************************************
 *  Sd_Tx_SerializeConfigurationOption()
 *********************************************************************************************************************/
/*! \brief          Serializes a configuration option to the payload.
 *  \details        -
 *  \param[in]      PartitionIdx                Index of partition.
 *                                              [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ConfigurationOptionStartIdx Start index of configuration option.
 *  \param[in]      ConfigurationOptionEndIdx   End index of configuration option.
 *  \param[in]      PayloadOffset               Start offset for the option.
 *  \return         The length of the serialized option.
 *                  [invalid: SD_INV_OPTION_LENGTH if buffer is too small]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(uint16, SD_CODE) Sd_Tx_SerializeConfigurationOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfCapabilityRecordType ConfigurationOptionStartIdx,
  Sd_SizeOfCapabilityRecordType ConfigurationOptionEndIdx,
  uint16 PayloadOffset);

/**********************************************************************************************************************
 *  Sd_Tx_UseNewSerializedOption()
 *********************************************************************************************************************/
/*! \brief          Updates the entry to option mapping in order to reference the new serialized option.
 *  \details        -
 *  \param[in,out]  EntryToOptionMapPtr     Information about the entry to option mapping.
 *  \param[in,out]  OptionArrayInfoPtr      Information about the options array.
 *  \param[in]      NewOptionSize           Size of new serialized option.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_UseNewSerializedOption(
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr,
  uint16 NewOptionSize);

/**********************************************************************************************************************
 *  Sd_UtilClient_CloseAllMulticastSoCons()
 *********************************************************************************************************************/
/*! \brief          Closes all multicast socket connections of the eventgroup.
 *  \details        -
 *  \param[in]      ConsumedEventgroupIdx   Corresponding consumed eventgroup index.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      Abort                   Indicates if the socket connections shall be aborted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_CloseAllMulticastSoCons(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean Abort);

/**********************************************************************************************************************
 *  Sd_UtilClient_ResetDynamicVariables()
 *********************************************************************************************************************/
/*! \brief          Reset all dynamic variables of the given client.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_ResetDynamicVariables(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilClient_LocalIpAddrAssignmentChgReset()
 *********************************************************************************************************************/
/*! \brief          Resets all dynamic client data in the case of a local IP address assignment change.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance which has to be reset.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_LocalIpAddrAssignmentChgReset(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilClient_SetCurrentStateInformBswm()
 *********************************************************************************************************************/
/*! \brief          Updates the current state of the client and informs the BswM.
 *  \details        -
 *  \param[in]      ClientIdx               Corresponding client index.
 *                                          [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      State                   New state of the client.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_SetCurrentStateInformBswm(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ClientServiceCurrentStateType State);

#if ( SD_DEV_ERROR_DETECT == STD_ON )
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
/**********************************************************************************************************************
 *  Sd_Util_CheckApplicationIdValidity()
 *********************************************************************************************************************/
 /*! \brief          Checks the validity of the OS application identifier and the Sd partition index.
  *  \details        -
  *  \param[in]      PartitionIdx            Index of partition.
  *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
  *  \return         E_OK                    Application identifier is valid and matches to the partition index.
  *  \return         E_NOT_OK                Application identifier does not match to partition index.
  *  \pre            -
  *  \context        TASK|ISR2
  *  \reentrant      TRUE
  *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_CheckApplicationIdValidity(
  Sd_PartitionConfigIdxType PartitionIdx);
# endif /* SD_MULTI_PARTITION == STD_ON */
#endif /* SD_DEV_ERROR_DETECT == STD_ON */

/**********************************************************************************************************************
 *  Sd_Util_GetSoConMode()
 *********************************************************************************************************************/
/*! \brief          Returns the current mode of a socket connection.
 *  \details        -
 *  \param[in]      SoConId                 Socket connection ID specifying the socket connection.
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         Mode of the socket connection.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SoConModeType, SD_CODE) Sd_Util_GetSoConMode(
  SoAd_SoConIdType SoConId,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_ResetSessionIds()
 *********************************************************************************************************************/
/*! \brief          Resets all used SessionIDs of the given instance.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to reset SessionIDs.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ResetSessionIds(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_GetEmptySendEntry()
 *********************************************************************************************************************/
/*! \brief          Returns the index of a currently unused send entry in case there is any.
 *  \details        The returned index is the tail index of the free send entry list.
 *  \param[in]      InstanceIdx        Index of the instance.
 *                                     [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx       Index of partition.
 *                                     [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \return         Index of the send entry  - In case there are unused send entries.
 *  \return         Sd_GetSizeOfSendEntry()  - In case there are no unused send entries.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_SizeOfSendEntryType, SD_CODE) Sd_Util_GetEmptySendEntry(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_CommitSendEntry()
 *********************************************************************************************************************/
/*! \brief          Adds a send entry to the send entry list of an address storage.
 *  \details        -
 *  \param[in]      AddressStorageIdx  Destination address storage index.
 *                                     [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      PartitionIdx       Index of partition.
 *                                     [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ReservedEntryIdx   Index of send entry to add.
 *                                     [range: Sd_GetSendEntryStartIdxOfInstance() <= ReservedEntryIdx <
 *                                      Sd_GetSendEntryEndIdxOfInstance()]
 *  \param[in]      AdditionalDelay    Delay which will be added to TimeToSend of the address storage.
 *  \pre            The caller of this function must ensure that the send entry is currently unused.
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_CommitSendEntry(
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfSendEntryType ReservedEntryIdx,
  uint32 AdditionalDelay);

/**********************************************************************************************************************
 *  Sd_Util_ClearSendEntryList()
 *********************************************************************************************************************/
/*! \brief          Removes all entries of all send entry lists of an instance.
 *  \details        -
 *  \param[in]      InstanceIdx  Index of the instance with pending send entries.
 *                               [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx Index of partition.
 *                               [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_ClearSendEntryList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_RemoveSendEntryFromList()
 *********************************************************************************************************************/
/*! \brief          Removes a send entry from the send entry list of an address storage and adds the entry to the free
 *                  send entry list.
 *  \details        -
 *  \param[in]      InstanceIdx        Index of the instance.
 *                                     [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx       Index of partition.
 *                                     [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      AddressStorageIdx  Destination address index.
 *                                     [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      RemoveEntryIdx     Send entry index to remove.
 *                                     [range: Sd_GetSendEntryStartIdxOfInstance() <= ReservedEntryIdx <
 *                                      Sd_GetSendEntryEndIdxOfInstance()]
 *  \return         E_OK       - Send entry was succesfully removed.
 *                  E_NOT_OK   - SendEntryIdx is invalid or all send entry lists of the instance are empty.
 *  \pre            The caller of this function must ensure that the send entry is part of the send entry list of the
 *                  called address storage.
 *  \pre            The caller of this function must ensure that the address storage belongs to the instance.
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Util_RemoveSendEntryFromList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_SizeOfSendEntryType RemoveEntryIdx);

/**********************************************************************************************************************
 *  Sd_Util_CheckSendEntryListFullAndTransmit()
 *********************************************************************************************************************/
/*! \brief          Check if the list of pending send entries is full and trigger a transmission.
 *  \details        -
 *  \param[in]      InstanceIdx        Index of the instance with pending send entries.
 *                                     [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx       Index of partition.
 *                                     [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_CheckSendEntryListFullAndTransmit(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_AddFindEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds a FindService message to the list of send entries.
 *  \details        -
 *  \param[in,out]  ClientIdx          Client index searching for service.
 *                                     [range: ClientIdx < Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx       Index of partition.
 *                                     [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddFindEntryToList(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_AddMulticastOfferEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds an OfferService message via multicast to the list of send entries.
 *  \details        -
 *  \param[in,out]  ServerIdx               Server index offering its availability.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      StopOffer               TRUE  = StopOfferService message.
                                            FALSE = OfferService message.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddMulticastOfferEntryToList(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean StopOffer);

/**********************************************************************************************************************
 *  Sd_Util_AddUnicastOfferEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds an OfferService message via unicast to the list of send entries.
 *  \details        -
 *  \param[in,out]  ServerIdx               Corresponding server index.
 *                                          [range: ServerIdx < Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      SavedAddressStorageIdx  Destination address index.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      TransmissionOffset      Timing offset when the message shall be transmitted.
                                            (Random delay for responses to multicast messages.)
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddUnicastOfferEntryToList(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType SavedAddressStorageIdx,
  uint32 TransmissionOffset);

/**********************************************************************************************************************
 *  Sd_Util_AddSubscribeEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds a SubscribeEventgroup message to the list of send entries.
 *  \details        -
 *  \param[in,out]  ConsumedEventgroupIdx   Consumed eventgroup index which triggers the subscription.
 *                                          [range: ConsumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      Subscribe               TRUE  = SubscribeEventgroup message.
                                            FALSE = StopSubscribeEventgroup message.
 *  \param[in]      TransmissionOffset      Timing offset when the message shall be transmitted.
                                            (Random delay for responses to multicast messages.)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddSubscribeEntryToList(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean Subscribe,
  uint32 TransmissionOffset);

/**********************************************************************************************************************
 *  Sd_Util_AddEventGroupAckEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds a SubscribeEventgroupAck message to the list of send entries.
 *  \details        -
 *  \param[in,out]  InstanceIdx             Index of instance handling the event.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      RxEventIdx              Received event.
 *                                          [range: RxEventIdx < Sd_GetSizeOfRxEvent()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddEventGroupAckEntryToList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfRxEventType RxEventIdx);

/**********************************************************************************************************************
 *  Sd_Util_AddEventGroupNackEntryToList()
 *********************************************************************************************************************/
/*! \brief          Adds a SubscribeEventgroupNack message to the list of send entries.
 *  \details        -
 *  \param[in,out]  InstanceIdx             Index of instance where the NACK-Message should be handled.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      ServerIdx               Server service.
 *                                          [range: ServerIdx <= Sd_GetSizeOfServer()]
 *                                          [invalid: Sd_GetSizeOfServer()]
 *  \param[in]      ServiceId               ServiceId of the rejected SubscribeEventgroup Message.
 *  \param[in]      InstanceId              InstanceId of the rejected SubscribeEventgroup Message.
 *  \param[in]      MajorVersion            MajorVersion of the rejected SubscribeEventgroup Message.
 *  \param[in]      ReservedField           ReservedField of the rejected SubscribeEventgroup Message.
 *  \param[in]      EventHandlerId          EventHandlerId of the rejected SubscribeEventgroup Message.
 *  \param[in]      AddressStorageIdx       Destination address index.
 *                                          [range: AddressStorageIdx < Sd_GetSizeOfAddressStorage()]
 *  \param[in]      ServerTimerIdx          Index of the server timer used for calculation of the additional delay.
 *                                          [range: ServerTimerIdx <= Sd_GetSizeOfServerTimer()]
 *                                          [invalid: Sd_GetSizeOfServerTimer()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddEventGroupNackEntryToList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfServerType ServerIdx,
  uint16 ServiceId,
  uint16 InstanceId,
  uint8 MajorVersion,
  uint16 ReservedField,
  uint16 EventHandlerId,
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_SizeOfServerTimerType ServerTimerIdx);

/**********************************************************************************************************************
 *  Sd_UtilServer_ConfigureEntry()
 *********************************************************************************************************************/
/*! \brief          Initialize all server based parameters of the send entry.
 *  \details        Unused parameters will be initialized with invalid values.
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      ServerIdx               Server service.
 *                                          [range: ServerIdx <= Sd_GetSizeOfServer()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_ConfigureEntry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_UtilClient_ConfigureEntry()
 *********************************************************************************************************************/
/*! \brief          Initialize all client based parameters of the send entry.
 *  \details        Unused parameters will be initialized with invalid values.
 *  \param[in]      SendEntryIdx            Index of entry to send.
 *                                          [range: SendEntryIdx < Sd_GetSizeOfSendEntry()]
 *  \param[in]      ClientIdx               Client service.
 *                                          [range: ClientIdx <= Sd_GetSizeOfClient()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_ConfigureEntry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_CopySockAddr()
 *********************************************************************************************************************/
/*! \brief          Copies socket addr (incl. family, port, IP-addr) from source to target.
 *  \details        -
 *  \param[in,out]  TgtSockPtr              Target socket address.
 *  \param[in]      SrcSockPtr              Source socket address.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_CopySockAddr(
  SD_P2VAR(Sd_SockAddrContainerType) TgtSockPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SrcSockPtr);

/**********************************************************************************************************************
 *  Sd_Util_CompareSockAddrIp()
 *********************************************************************************************************************/
/*! \brief          Checks if IP addresses of the sockets are equal.
 *  \details        -
 *  \param[in]      SockAPtr                Target socket address.
 *  \param[in]      SockBPtr                Source socket address.
 *  \param[in]      AddrState               State of the source socket address.
 *  \return         SD_MATCH_FALSE          IP addresses are not identical.
 *  \return         SD_MATCH_TRUE           IP addresses are identical.
 *  \return         SD_MATCH_ANY            IP addresses match because of ANY state.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \note           The caller has to ensure that SockAPtr and/or SockBPtr point to valid addresses with
 *                  family != SOAD_AF_INVALID.
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_MatchType, SD_CODE) Sd_Util_CompareSockAddrIp(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 AddrState);

/**********************************************************************************************************************
 *  Sd_Util_CompareSockAddrPort()
 *********************************************************************************************************************/
/*! \brief          Checks if the ports of the sockets are equal.
 *  \details        -
 *  \param[in]      SockAPtr                Target socket address.
 *  \param[in]      SockBPtr                Source socket address.
 *  \param[in]      AddrState               State of the source socket address.
 *  \return         SD_MATCH_FALSE          Ports are not identical.
 *  \return         SD_MATCH_TRUE           Ports are identical.
 *  \return         SD_MATCH_ANY            IP addresses match because of ANY state.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \note           The caller has to ensure that SockAPtr and/or SockBPtr point to valid addresses with
 *                  family != SOAD_AF_INVALID.
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_MatchType, SD_CODE) Sd_Util_CompareSockAddrPort(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 AddrState);

/**********************************************************************************************************************
 *  Sd_Util_SockAddrIsEqual()
 *********************************************************************************************************************/
/*! \brief          Checks if IP addresses and ports of the sockets are equal.
 *  \details        -
 *  \param[in]      SockAPtr                Target socket address.
 *  \param[in]      SockBPtr                Source socket address.
 *  \return         TRUE                    IP addresses and ports are equal.
 *  \return         FALSE                   IP addresses and/or ports are not equal.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_SockAddrIsEqual(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr);

/**********************************************************************************************************************
 *  Sd_Rx_CheckSubnets()
 *********************************************************************************************************************/
/*! \brief          Checks if the attached endpoint options are in the same subnet as the local SdInstance.
 *  \details        Endpoints which are not within the configured subnet are ignored.
 *  \param[in]      InstanceIdx             Index of instance to process.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      UdpNeeded               Specifies if UDP Endpoint is needed for SD entity.
 *  \param[in]      TcpNeeded               Specifies if TCP Endpoint is needed for SD entity.
 *  \param[in,out]  AttachedOptionsPtr      Structure containing the attached options which shall be checked.
 *  \return         SD_OPTIONS_OK           Local address could be read.
 *  \return         SD_OPTIONS_NOT_OK       Failed to read the local address.
 *  \return         SD_OPTIONS_WRONG_SUBNET Option is not topological correct (wrong subnet).
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_CheckSubnets(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean UdpNeeded,
  boolean TcpNeeded,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr);

/**********************************************************************************************************************
 *  Sd_Rx_CheckSubnet()
 *********************************************************************************************************************/
/*! \brief          Checks if the endpoint address is within the same subnet as the given socket connection.
 *  \details        -
 *  \param[in]      SoConId                 Index of UDP or TCP socket connection to compare to.
 *  /param[in]      LocalAddrCheckLength    User defined subnet mask length.
 *                                          [range: 0 <= LocalAddrCheckLength <= 128, SD_NO_LOCAL_ADDRESS_CHECK_LENGTH]
 *  \param[in,out]  EndpointAddrPtr         IP address pointer of the received endpoint.
 *  \return         SD_OPTIONS_OK           Option is within the given subnet.
 *  \return         SD_OPTIONS_NOT_OK       Failed to read the local address.
 *  \return         SD_OPTIONS_WRONG_SUBNET Option is not topological correct (wrong subnet).
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_CheckSubnet(
  SoAd_SoConIdType SoConId,
  uint8 LocalAddrCheckLength,
  SD_P2VAR(Sd_SockAddrContainerType) EndpointAddrPtr);

/**********************************************************************************************************************
 *  Sd_Rx_SockAddrWithinSubnet()
 *********************************************************************************************************************/
/*! \brief          Checks if both IP addresses are within the same subnet.
 *  \details        -
 *  \param[in]      SockAPtr                Socket address A.
 *  \param[in]      SockBPtr                Socket address B.
 *  \param[in]      NetmaskCidr             Net mask in CIDR notation.
 *  \return         TRUE                    IP addresses are within the same subnet.
 *  \return         FALSE                   IP addresses are not within the same subnet.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Rx_SockAddrWithinSubnet(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 NetmaskCidr);

/**********************************************************************************************************************
 *  Sd_Util_ReportDemEvent()
 *********************************************************************************************************************/
/*! \brief          Reports the configured DEM event.
 *  \details        -
 *  \param[in]      EventId                 Configured event ID.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ReportDemEvent(
  Sd_Dem_EventIdType EventId);

/**********************************************************************************************************************
 *  Sd_Util_GetRandomNrInRange()
 *********************************************************************************************************************/
/*! \brief          Returns a random value.
 *  \details        The value is within the range: MinValue <= Random <= MaxValue.
 *  \param[in]      MinValue                Minimal allowed value.
 *  \param[in]      MaxValue                Maximal allowed value.
 *  \return         Random number.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(uint32, SD_CODE) Sd_Util_GetRandomNrInRange(
  uint32 MinValue,
  uint32 MaxValue);

/**********************************************************************************************************************
 *  Sd_Util_GetRandomNrInRangeForResponse()
 *********************************************************************************************************************/
/*! \brief          Returns a random value which can be used to delay a response message.
 *  \details        The function uses the information if it is called from InterruptContext or not in order to adjust
 *                  the MinValue and/or the MaxValue of the range, because the system time is only updated in context
 *                  of the main function.
 *  \param[in]      MinValue                Minimal allowed value.
 *                                          [MinValue has to be multiple of SD_MAIN_FCT_CYCLE_TIME_MS]
 *  \param[in]      MaxValue                Maximal allowed value.
 *                                          [MaxValue has to be multiple of SD_MAIN_FCT_CYCLE_TIME_MS]
 *  \param[in]      InterruptContext        Specifies if the function is called from context of message reception or context
 *                                          of MainFunction.
 *  \return         Random number.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(uint32, SD_CODE) Sd_Util_GetRandomNrInRangeForResponse(
  uint32 MinValue,
  uint32 MaxValue,
  boolean InterruptContext);

/**********************************************************************************************************************
 *  Sd_Util_CheckIfTimerExceededInt()
 *********************************************************************************************************************/
/*! \brief          Checks if TimerA exceedes TimerB.
 *  \details        -
 *  \param[in]      TimerA                  First timer to be checked.
 *  \param[in]      TimerB                  Second timer to be checked.
 *  \param[in]      EqualIsExceeded         If set to TRUE, TimerA is also indicated as exceeded if it is equal to
 *                                          TimerB.
 *  \return         TRUE                    TimerA exceedes TimerB.
 *  \return         FALSE                   TimerA does not exceed TimerB.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_CheckIfTimerExceededInt(
  Sd_ExtendedTimeType TimerA,
  Sd_ExtendedTimeType TimerB,
  boolean             EqualIsExceeded);

/**********************************************************************************************************************
 *  Sd_Util_CheckIfTimerExceeded()
 *********************************************************************************************************************/
/*! \brief          Checks if the timer is already exceeded.
 *  \details        -
  *  \param[in]     PartitionIdx            Index of partition.
  *                                         [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      Timer                   Timer to be checked.
 *  \param[in]      EqualIsExceeded         If set to TRUE, the timer is also indicated as exceeded if it is equal to the
 *                                          current time.
 *                                          [EqualIsExceeded == TRUE is not allowed for comparison of TTL values.]
 *  \return         TRUE                    Timer is exceeded.
 *  \return         FALSE                   Timer is not exceeded.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \note           If the timer of TTL values is checked, the EqualIsExceeded flag has to be set to FALSE.
 *********************************************************************************************************************/
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_CheckIfTimerExceeded(
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ExtendedTimeType Timer,
  boolean EqualIsExceeded);

/**********************************************************************************************************************
 *  Sd_Util_CheckAndRescheduleTimer()
 *********************************************************************************************************************/
/*! \brief          Reschules the given timer according to the offset in milliseconds.
 *  \details        The offset in milliseconds is added to the Sd_TimeMsSinceStart timer and then assigned to the
 *                  value of TimerPtr.
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      OffsetMs                Offset in milliseconds.
 *  \param[in,out]  TimerPtr                Timer to be rescheduled.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_CheckAndRescheduleTimer(
  Sd_PartitionConfigIdxType PartitionIdx,
  uint32 OffsetMs,
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr);

/**********************************************************************************************************************
 *  Sd_Util_TriggerStateMachineRun()
 *********************************************************************************************************************/
/*! \brief          Triggers a state machine execution at the next MainFunction.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_TriggerStateMachineRun(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_SetNextStateMachineRun()
 *********************************************************************************************************************/
/*! \brief          Triggers a state machine execution at the point in time given by the timer parameter.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in]      Timer                   Time to trigger next state machine execution.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetNextStateMachineRun(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ExtendedTimeType Timer);

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffsetWithTrigger()
 *********************************************************************************************************************/
/*! \brief          Updates the timer to the actual time plus the given offset.
 *  \details        Function also triggers an execution of all state machines of the given instance if the
 *                  timer expires.
 *  \param[in,out]  TimerPtr                Timer to Update.
 *  \param[in]      OffsetMs                Offset when the timer shall expire. [MSEC]
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \note           This function cannot be used to handle received TTL values because of the limited size of the
 *                  OffsetMs parameter.
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffsetWithTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetMs,
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffsetSecWithTrigger()
 *********************************************************************************************************************/
/*! \brief          Updates the timer to the actual time plus the given offset.
 *  \details        Function also triggers an execution of all state machines of the given instance if the
 *                  timer expires.
 *  \param[in,out]  TimerPtr                Timer to Update.
 *  \param[in]      OffsetSec               Offset when the timer shall expire. [SEC]
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \note           This function should be used to handle received TTL values.
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffsetSecWithTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetSec,
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffset()
 *********************************************************************************************************************/
/*! \brief          Updates the timer to the actual time plus the given offset.
 *  \details        -
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \param[in,out]  TimerPtr                Timer to Update.
 *  \param[in]      OffsetMs                Offset when the timer shall expire.  [MSEC]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \note           This function cannot be used to handle received TTL values because of the limited size of the
 *                  OffsetMs parameter.
 *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffset(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetMs);

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToZero()
 *********************************************************************************************************************/
/*! \brief          Resets the given timer to zero.
 *  \details        -
 *  \param[in,out]  TimerPtr                Timer to reset.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ResetTimerToZero(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr);

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToZeroTrigger()
 *********************************************************************************************************************/
/*! \brief          Resets the given timer to zero.
 *  \details        Function also triggers an execution of all state machines of the given instance.
 *  \param[in,out]  TimerPtr                Timer to Update.
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ResetTimerToZeroTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToInvalid()
 *********************************************************************************************************************/
/*! \brief          Resets the given timer to the invalid value.
 *  \details        -
 *  \param[in,out]  TimerPtr                Timer to reset.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ResetTimerToInvalid(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr);

/**********************************************************************************************************************
 *  Sd_Util_UpdateTimeSinceStart()
 *********************************************************************************************************************/
 /*! \brief          Adds main function cycle time to the TimeMsSinceStart timer.
  *  \details        -
  *  \param[in]      PartitionIdx            Index of partition.
  *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
  *  \pre            -
  *  \context        TASK|ISR2
  *  \reentrant      FALSE
  *********************************************************************************************************************/
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_UpdateTimeSinceStart(
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_DisableRouting()
 *********************************************************************************************************************/
/*! \brief          Calls SoAd to disable a RoutingGroup if routing for this group is not disabled already.
 *  \details        -
 *  \param[in]      RoutingGroupIdx         The index to Sd_RoutingGroup.
 *                                          [range: RoutingGroupIdx < Sd_GetSizeOfRoutingGroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_DisableRouting(
  Sd_SizeOfRoutingGroupType RoutingGroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_DisableSpecificRouting()
 *********************************************************************************************************************/
/*! \brief          Calls SoAd to disable one specific routing of a RoutingGroup.
 *  \details        -
 *  \param[in]      RoutingGroupIdx         The index to Sd_RoutingGroup.
 *                                          [range: RoutingGroupIdx < Sd_GetSizeOfRoutingGroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *                  SoConId                 The SocketConnection Id for which the routing shall be disabled.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_DisableSpecificRouting(
  Sd_SizeOfRoutingGroupType RoutingGroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  Sd_Util_EnableRouting()
 *********************************************************************************************************************/
/*! \brief          Calls SoAd to enable a RoutingGroup if routing for this group is not enabled already.
 *  \details        -
 *  \param[in]      RoutingGroupIdx         The index to Sd_RoutingGroup.
 *                                          [range: RoutingGroupIdx < Sd_GetSizeOfRoutingGroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_EnableRouting(
  Sd_SizeOfRoutingGroupType RoutingGroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_EnableSpecificRouting()
 *********************************************************************************************************************/
/*! \brief          Calls SoAd to enable one specific routing of a RoutingGroup.
 *  \details        -
 *  \param[in]      RoutingGroupIdx         The index to Sd_RoutingGroup.
 *                                          [range: RoutingGroupIdx < Sd_GetSizeOfRoutingGroup()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *                  SoConId                 The SocketConnection Id for which the routing shall be enabled.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_EnableSpecificRouting(
  Sd_SizeOfRoutingGroupType RoutingGroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  Sd_Util_SetAllStateMachineExecutionFlags()
 *********************************************************************************************************************/
/*! \brief          Sets state machine execution flags for all server and client services.
 *  \details        -
 *  \param[in]      InstanceIdx             Index of instance to trigger state machine execution.
 *                                          [range: InstanceIdx < Sd_GetSizeOfInstance()]
 *  \param[in]      PartitionIdx            Index of partition.
 *                                          [range: PartitionIdx < Sd_GetSizeOfPartitionIdentifiers()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_SetAllStateMachineExecutionFlags(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  Sd_Util_Write32Bit()
 *********************************************************************************************************************/
/*! \brief          Write the 32 bit value to the given buffer.
 *  \details        -
 *  \param[in,out]  BufferPtr               Buffer to write to.
 *                                          Ensure that the buffer contains at least four additional elements.
 *  \param[in]      Value                   Value to write to the buffer.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_Write32Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint32 Value);

/**********************************************************************************************************************
 *  Sd_Util_Write24Bit()
 *********************************************************************************************************************/
/*! \brief          Write the 24 bit value to the given buffer. (Lower three bytes of the 32 bit value.)
 *  \details        -
 *  \param[in,out]  BufferPtr               Buffer to write to.
 *                                          Ensure that the buffer contains at least three additional elements.
 *  \param[in]      Value                   Value to write to the buffer.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_Write24Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint32 Value);

/**********************************************************************************************************************
 *  Sd_Util_Write16Bit()
 *********************************************************************************************************************/
/*! \brief          Write the 16 bit value to the given buffer.
 *  \details        -
 *  \param[in,out]  BufferPtr               Buffer to write to.
 *                                          Ensure that the buffer contains at least two additional elements.
 *  \param[in]      Value                   Value to write to the buffer.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_Write16Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint16 Value);

/**********************************************************************************************************************
 *  Sd_Util_Write8Bit()
 *********************************************************************************************************************/
/*! \brief          Write the 8 bit value to the given buffer.
 *  \details        -
 *  \param[in,out]  BufferPtr               Buffer to write to.
 *                                          Ensure that the buffer contains at least one additional elements.
 *  \param[in]      Value                   Value to write to the buffer.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_Write8Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint8 Value);

/**********************************************************************************************************************
 *  Sd_SoAd_GetLocalAddrWithNetmask()
 *********************************************************************************************************************/
/*! \brief          Read the local address and the configured netmask of the socket connection.
 *  \details        If the local address can not be read or the family type is unknown, the family type is set to
 *                  the invalid value.
 *  \param[in]      SoConId                 Socket connection ID.
 *  \param[out]     LocalAddrPtr            Address container for local address.
 *  \param[out]     NetmaskPtr              Pointer to network mask in CIDR notation.
 *  \return         E_OK                    Local address successfully read with valid family type.
 *  \return         E_NOT_OK                Failed to read local address or address has unknown family type.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_SoAd_GetLocalAddrWithNetmask(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) LocalAddrPtr,
  SD_P2VAR(uint8) NetmaskPtr);

/**********************************************************************************************************************
 *  Sd_SoAd_GetLocalAddr()
 *********************************************************************************************************************/
/*! \brief          Read the local address of the socket connection.
 *  \details        If the local address can not be read or the family type is unknown, the family type is set to
 *                  the invalid value.
 *  \param[in]      SoConId                 Socket connection ID.
 *  \param[out]     LocalAddrPtr            Address container for local address.
 *  \return         E_OK                    Local address successfully read with valid family type.
 *  \return         E_NOT_OK                Failed to read local address or address has unknown family type.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_SoAd_GetLocalAddr(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) LocalAddrPtr);

/**********************************************************************************************************************
 *  Sd_SoAd_GetRemoteAddr()
 *********************************************************************************************************************/
/*! \brief          Read the remote address of the socket connection.
 *  \details        If the remote address can not be read or the family type is unknown, the family type is set to
 *                  the invalid value.
 *  \param[in]      SoConId                 Socket connection ID.
 *  \param[out]     IpAddrPtr               Address container for remote address.
 *  \return         E_OK                    Remote address successfully read with valid family type.
 *  \return         E_NOT_OK                Failed to read remote address or address has unknown family type.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_SoAd_GetRemoteAddr(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) IpAddrPtr);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Sd_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SD_CODE) Sd_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set module state to uninitialized. */
  Sd_State = SD_UNINIT;
  Sd_FirstInit = TRUE;

  /* #20 Initialize config pointer. */
  Sd_ConfigDataPtr = NULL_PTR;
}

/**********************************************************************************************************************
 *  Sd_PreInit()
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
FUNC(void, SD_CODE) Sd_PreInit(
  P2CONST(Sd_ConfigType, AUTOMATIC, SD_CONST) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameter. */
  if (ConfigPtr == NULL_PTR)
  {
# if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
    EcuM_BswErrorHook(SD_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = SD_E_PARAM_POINTER;
  }
# if(SD_FINALMAGICNUMBER == STD_ON)
  else if (ConfigPtr->FinalMagicNumberOfPBConfig != SD_FINAL_MAGIC_NUMBER)
  {
#  if(SD_CONFIGURATION_VARIANT == SD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)                                          /* COV_SD_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(SD_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    errorId = SD_E_PARAM_CONFIG;
  }
# endif
  else
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    Sd_PartitionIdentifiersIterType partitionIdentifierIter; /* Not initialized: For-loop counter. */

    /* #20 Initialize configuration pointer. */
    Sd_ConfigDataPtr = ConfigPtr;

    /* #30 Initialize measurement counters. */
#if(SD_GET_RESET_MEASUREMENT_DATA_API == STD_ON)
    Sd_Measure_Init();
#endif

    /* #40 Set all partition specific initialization states to UNINIT. */
    for (partitionIdentifierIter = 0u;
         partitionIdentifierIter < Sd_GetSizeOfPartitionIdentifiers();
         partitionIdentifierIter++)
    {
      const Sd_PartitionConfigIdxType partitionIdx = Sd_GetPCPartitionConfigIdxOfPartitionIdentifiers(
        partitionIdentifierIter);
      Sd_SetInitializedOfPartitionDataDyn(SD_PARTITION_DATA_IDX, SD_STATE_PARTITION_UNINIT, partitionIdx);              /* SBSW_SD_PARTITION_DATA_CSL05 */
    }

    /* #50 Set preinitialized state. */
    Sd_State = SD_PREINIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if (errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_PRE_INIT, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SD_DEV_ERROR_REPORT == STD_ON */
}

/**********************************************************************************************************************
 *  Sd_Init()
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
FUNC(void, SD_CODE) Sd_Init(
  P2CONST(Sd_ConfigType, AUTOMATIC, SD_INIT_DATA) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check preinitialized state. */
  if(Sd_State != SD_PREINIT)
  {
    errorId = SD_E_NO_PREINIT;
  }
  else
#else
  if(Sd_State == SD_PREINIT)
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    Sd_PartitionIdentifiersIterType partitionIdentifierIter; /* Not initialized: For-loop counter. */

    for (partitionIdentifierIter = 0u;
         partitionIdentifierIter < Sd_GetSizeOfPartitionIdentifiers();
         partitionIdentifierIter++)
    {
      const Sd_PartitionConfigIdxType partitionIdx = Sd_GetPCPartitionConfigIdxOfPartitionIdentifiers(
        partitionIdentifierIter);

#if (SD_MULTI_PARTITION == STD_ON)                                                                                      /* COV_SD_QM_FEATURE_MULTI_PARTITION */
      const ApplicationType osApplicationId = (ApplicationType)GetApplicationID();
      if (Sd_GetOsApplicationIdOfPartitionData(SD_PARTITION_DATA_IDX, partitionIdx) == osApplicationId)
#endif /* SD_MULTI_PARTITION == STD_ON */
      {
        /* #20 Initialize general data. */
        Sd_Init_General(partitionIdx);

        /* #30 Initialize partition data. */
        Sd_Init_PartitionData(partitionIdx);

        /* #40 Initialize instance data. */
        Sd_Init_Instance(partitionIdx);

        /* #50 Initialize server data. */
        Sd_Init_Server(partitionIdx);

        /* #60 Initialize client data. */
        Sd_Init_Client(partitionIdx);

        /* #70 Set the state to initialized. */
        Sd_SetInitializedOfPartitionDataDyn(SD_PARTITION_DATA_IDX, SD_STATE_PARTITION_INIT, partitionIdx);              /* SBSW_SD_PARTITION_DATA_CSL05 */
      }
    }
  }
  SD_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #80 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_INIT, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_PostInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SD_CODE) Sd_PostInit(void)
{
  /* ----- Local Variable ----------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check preinitialized state. */
  if (Sd_State != SD_PREINIT)
  {
    errorId = SD_E_NO_PREINIT;
  }
  else
#else
  if (Sd_State == SD_PREINIT)
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check initialization states of all partitions. */
    Sd_PartitionIdentifiersIterType partitionIdentifierIter; /* Not initialized: For-loop counter. */
    for (partitionIdentifierIter = 0u;
         partitionIdentifierIter < Sd_GetSizeOfPartitionIdentifiers();
         partitionIdentifierIter++)
    {
      const Sd_PartitionConfigIdxType partitionIdx = Sd_GetPCPartitionConfigIdxOfPartitionIdentifiers(
        partitionIdentifierIter);

      if (Sd_GetInitializedOfPartitionDataDyn(SD_PARTITION_DATA_IDX, partitionIdx) != SD_STATE_PARTITION_INIT)
      {
        errorId = SD_E_NOT_INITIALIZED;
        break;
      }
    }

    /* #30 Set module state to initialized if all partitions are initialized. */
    if (errorId == SD_E_NO_ERROR)
    {
      Sd_State = SD_INIT;
      Sd_FirstInit = FALSE;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SD_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default error if any occurred. */
  if (errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_POST_INIT, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SD_DEV_ERROR_REPORT == STD_ON */
}

/**********************************************************************************************************************
 *  Sd_MainFunctionPartition()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SD_CODE) Sd_MainFunctionPartition(
  Sd_SizeOfPartitionIdentifiersType PartitionIdentifierIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of runtime parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if (PartitionIdentifierIdx >= Sd_GetSizeOfPartitionIdentifiers())
  {
    errorId = SD_E_INV_ARG;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPCPartitionConfigIdxOfPartitionIdentifiers(PartitionIdentifierIdx)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif /* SD_MULTI_PARTITION == STD_ON */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_PartitionConfigIdxType partitionIdx = Sd_GetPCPartitionConfigIdxOfPartitionIdentifiers(
      PartitionIdentifierIdx);
    Sd_SizeOfInstanceType instanceIdx = 0u;

    /* #20 Update the time since start of the Sd_Init() function. */
    Sd_Util_UpdateTimeSinceStart(partitionIdx);

    /* #30 Iterate over all instances. */
    for (instanceIdx = 0u; instanceIdx < Sd_GetSizeOfInstance(partitionIdx); instanceIdx++)
    {
      /* #300 Handle the run mode of the current instance. */
      SdHookBegin_State_HandleRunMode();
      Sd_State_HandleRunMode(instanceIdx, partitionIdx);
      SdHookEnd_State_HandleRunMode();

      /* #301 Handle reboot, only process pending events and SMs if no reboot is pending. */
      if(!Sd_HandleReboot(instanceIdx, partitionIdx))
      {
        /* #3010 Process pending server events. */
        SdHookBegin_EventServer_Process();
        Sd_EventServer_Process(instanceIdx, partitionIdx);
        SdHookEnd_EventServer_Process();

        /* #3011 Process pending state machine executions. */
        Sd_State_Process(instanceIdx, partitionIdx);
      }

      /* #302 Process pending messages. Serialize message and trigger transmission. */
      SdHookBegin_EventServer_Tx_ProcessPendingMessages();
      Sd_Tx_ProcessPendingMessages(instanceIdx, partitionIdx, FALSE);
      SdHookEnd_EventServer_Tx_ProcessPendingMessages();
    }

    /* #40 Process pending trigger transmit requests. */
    Sd_TriggerTransmit_Process(partitionIdx);
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_MAIN_FUNCTION, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

#if(SD_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Sd_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SD_CODE) Sd_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SD_APPL_VAR) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                     /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(VersionInfoPtr == NULL_PTR)
  {
    errorId = SD_E_PARAM_POINTER;
  }
  else
# endif /* SD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set VersionInfo structure with corresponding macros. */
    VersionInfoPtr->vendorID = (SD_VENDOR_ID);                                                                          /* SBSW_SD_GETVERSION_PARAM */
    VersionInfoPtr->moduleID = (SD_MODULE_ID);                                                                          /* SBSW_SD_GETVERSION_PARAM */
    VersionInfoPtr->sw_major_version = (uint8)(SD_SW_MAJOR_VERSION);                                                    /* SBSW_SD_GETVERSION_PARAM */
    VersionInfoPtr->sw_minor_version = (uint8)(SD_SW_MINOR_VERSION);                                                    /* SBSW_SD_GETVERSION_PARAM */
    VersionInfoPtr->sw_patch_version = (uint8)(SD_SW_PATCH_VERSION);                                                    /* SBSW_SD_GETVERSION_PARAM */
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_GET_VERSION_INFO, errorId);
  }
# else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif /* SD_DEV_ERROR_REPORT == STD_ON */
}
#endif

/**********************************************************************************************************************
 *  Sd_RxIndication()
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
/* PRQA S 6050, 6080 1 */ /* MD_Sd_STCAL_6050, MD_MSR_STMIF */
FUNC(void, SD_CODE) Sd_RxIndication(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SD_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  SdHookBegin_RxIndication();

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if(PduInfoPtr == NULL_PTR)
  {
    errorId = SD_E_PARAM_POINTER;
  }
  else if ((RxPduId >= Sd_GetSizeOfRxPduIdMap()) || (Sd_IsInvalidHndOfRxPduIdMap(RxPduId) == TRUE))
  {
    errorId = SD_E_INV_ID;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificInstancePartitionIdxOfRxPduIdMap(RxPduId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif /* SD_MULTI_PARTITION == STD_ON */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_SizeOfInstanceType instanceIdx = Sd_GetPartitionSpecificInstanceIdxOfRxPduIdMap(RxPduId);
    const Sd_PartitionConfigIdxType partitionIdx = Sd_GetPartitionSpecificInstancePartitionIdxOfRxPduIdMap(RxPduId);
    SoAd_SoConIdType rxSoConId = 0u;
    Sd_RxMessageInfoType messageInfo = { 0u };

    messageInfo.PduInfoPtr = PduInfoPtr;

    /* #20 Check whether message was received by unicast or multicast PDU. */
    if (Sd_GetUnicastRxPduIdOfInstance(instanceIdx, partitionIdx) == RxPduId)
    {
      messageInfo.MulticastMessage = FALSE;
      rxSoConId = Sd_GetUnicastRxSoConIdOfInstance(instanceIdx, partitionIdx);
    }
    else
    {
      messageInfo.MulticastMessage = TRUE;
      rxSoConId = Sd_GetMulticastRxSoConIdOfInstance(instanceIdx, partitionIdx);
    }

    /* #30 Increment RxIndication counter in order to be able to distinguish received packets. */
    /* Overflows are expected. */
    Sd_IncRxIndicationCountOfPartitionDataDyn(SD_PARTITION_DATA_IDX, partitionIdx);                                     /* SBSW_SD_PARTITION_DATA_CSL05 */

    /* #40 Process received message. */
    Sd_Rx_Process(instanceIdx, partitionIdx, rxSoConId, &messageInfo);                                                  /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }
  SdHookEnd_RxIndication();

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #50 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_RX_INDICATION, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_LocalIpAddrAssignmentChg()
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
FUNC(void, SD_CODE) Sd_LocalIpAddrAssignmentChg(
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if((State != SOAD_IPADDR_STATE_ASSIGNED) &&
    (State != SOAD_IPADDR_STATE_ONHOLD) &&
    (State != SOAD_IPADDR_STATE_UNASSIGNED))
  {
    errorId = SD_E_INV_MODE;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificInstancePartitionIdxOfTxSoConIdMap(SoConId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif /* SD_MULTI_PARTITION == STD_ON */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check if SoConId is valid. */
    if ((SoConId < Sd_GetSizeOfTxSoConIdMap()) && (Sd_IsInvalidHndOfTxSoConIdMap(SoConId) == FALSE))
    {
      const Sd_PartitionConfigIdxType partitionIdx =
        Sd_GetPartitionSpecificInstancePartitionIdxOfTxSoConIdMap(SoConId);
      const Sd_SizeOfInstanceType instanceIdx = Sd_GetPartitionSpecificInstanceIdxOfTxSoConIdMap(SoConId);

      /* #30 Save the state of the IP address if it is not ONHOLD. */
      if (State != SOAD_IPADDR_STATE_ONHOLD)
      {
        Sd_SetStateOfInstanceDyn(instanceIdx, (Sd_ServiceInstanceStateType)State, partitionIdx);                          /* SBSW_SD_CSL03_CSL05 */
        Sd_Util_SetAllStateMachineExecutionFlags(instanceIdx, partitionIdx);
        Sd_Util_TriggerStateMachineRun(instanceIdx, partitionIdx);
      }

      if (State == SOAD_IPADDR_STATE_ASSIGNED)
      {
        /* #300 Save the event that a new IP address was assigned.
         *        The event will be processed in the MainFunction. */
        if ((Sd_GetRunModeOfInstanceDyn(instanceIdx, partitionIdx) != SD_RUN_MODE_RUNNING_NO_IP) &&
          (Sd_GetRunModeOfInstanceDyn(instanceIdx, partitionIdx) != SD_RUN_MODE_ONHOLD))
        {
          Sd_SetRunModeOfInstanceDyn(instanceIdx, SD_RUN_MODE_NEW_IP_ASSIGNED, partitionIdx);                             /* SBSW_SD_CSL03_CSL05 */
        }
        else
        {
          Sd_SetRunModeOfInstanceDyn(instanceIdx, SD_RUN_MODE_RUNNING, partitionIdx);                                     /* SBSW_SD_CSL03_CSL05 */
        }
      }
      else if (State == SOAD_IPADDR_STATE_ONHOLD)
      {
        if (Sd_GetRunModeOfInstanceDyn(instanceIdx, partitionIdx) == SD_RUN_MODE_RUNNING)
        {
          Sd_SetRunModeOfInstanceDyn(instanceIdx, SD_RUN_MODE_ONHOLD, partitionIdx);                                      /* SBSW_SD_CSL03_CSL05 */
        }
      }
      else  /* State != SOAD_IPADDR_STATE_ASSIGNED */
      {
        /* #301 Save the event that the local IP address was un-assigned.
         *        The event will be processed in the MainFunction. */
        Sd_SetRunModeOfInstanceDyn(instanceIdx, SD_RUN_MODE_IP_UNASSIGNED, partitionIdx);                                 /* SBSW_SD_CSL03_CSL05 */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_LOCAL_IP_ADDR_ASSIGNMENT_CHG, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_SoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SD_CODE) Sd_SoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if((Mode != SOAD_SOCON_ONLINE) && (Mode != SOAD_SOCON_RECONNECT) && (Mode != SOAD_SOCON_OFFLINE))
  {
    errorId = SD_E_INV_MODE;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificSoConModePartitionIdxOfSoConModeIdMap(SoConId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif /* SD_MULTI_PARTITION == STD_ON */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check if SoConId is valid. */
    if ((SoConId < Sd_GetSizeOfSoConModeIdMap()) && (Sd_IsInvalidHndOfSoConModeIdMap(SoConId) == FALSE))
    {
      const Sd_SizeOfSoConModeDynType soConModeIdx = Sd_GetPartitionSpecificSoConModeIdxOfSoConModeIdMap(SoConId);
      const Sd_PartitionConfigIdxType partitionIdx =
        Sd_GetPartitionSpecificSoConModePartitionIdxOfSoConModeIdMap(SoConId);

      /* #30 Save the current mode of the socket connection. */
      SD_BEGIN_CRITICAL_SECTION();
      Sd_SetSoConModeDyn(soConModeIdx, (Sd_SoConModeType)Mode, partitionIdx);                                             /* SBSW_SD_CSL03_CSL05 */
      SD_END_CRITICAL_SECTION();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SOCONMODE_CHG, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_ServerServiceSetState()
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
FUNC(Std_ReturnType, SD_CODE) Sd_ServerServiceSetState(
  uint16 ServerServiceHandleId,
  Sd_ServerServiceSetStateType ServerServiceState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if((ServerServiceState != SD_SERVER_SERVICE_DOWN) &&
          (ServerServiceState != SD_SERVER_SERVICE_AVAILABLE))
  {
    errorId = SD_E_INV_MODE;
  }
  else if ((ServerServiceHandleId >= Sd_GetSizeOfServerIdMap()) ||
    (Sd_IsInvalidHndOfServerIdMap(ServerServiceHandleId) == TRUE))
  {
    errorId = SD_E_INV_ID;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificServerPartitionIdxOfServerIdMap(ServerServiceHandleId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif /* SD_MULTI_PARTITION == STD_ON */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_SizeOfServerType serverIdx = Sd_GetPartitionSpecificServerIdxOfServerIdMap(ServerServiceHandleId);
    const Sd_PartitionConfigIdxType partitionIdx =
      Sd_GetPartitionSpecificServerPartitionIdxOfServerIdMap(ServerServiceHandleId);

    /* #20 Verify that server is not member of any service group. */
    if(Sd_IsServiceGroupMemberOfServer(serverIdx, partitionIdx) == FALSE)
    {
      const Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(serverIdx, partitionIdx);

      /* #200 Check if the server shall be shut down. */
      if(ServerServiceState == SD_SERVER_SERVICE_DOWN)
      {
        Sd_UtilServer_SetDown(serverIdx, partitionIdx);
      }
      /* #201 Check if server shall be activated. */
      else if (Sd_GetStateSwcOfServerDyn(serverIdx, partitionIdx) == SD_SERVER_SERVICE_DOWN)
      {
        if (Sd_GetStateOfInstanceDyn(instanceIdx, partitionIdx) == SD_INSTANCE_UP_AND_CONFIGURED)
        {
          Sd_UtilServer_Configure(serverIdx, partitionIdx);
        }
        else
        {
          Sd_SetStateSwcOfServerDyn(serverIdx, SD_SERVER_SERVICE_AVAILABLE, partitionIdx);                              /* SBSW_SD_CSL03_CSL05 */
        }
      }
      else
      {
        /* Nothing to do */
      }

      retVal = E_OK;
    }
    else
    {
      errorId = SD_E_INV_ID;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SERVER_SERVICE_SET_STATE, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_ClientServiceSetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SD_CODE) Sd_ClientServiceSetState(
  uint16 ClientServiceHandleId,
  Sd_ClientServiceSetStateType ClientServiceState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if((ClientServiceState != SD_CLIENT_SERVICE_RELEASED) &&
          (ClientServiceState != SD_CLIENT_SERVICE_REQUESTED))
  {
    errorId = SD_E_INV_MODE;
  }
  else if ((ClientServiceHandleId >= Sd_GetSizeOfClientIdMap()) ||
    (Sd_IsInvalidHndOfClientIdMap(ClientServiceHandleId) == TRUE))
  {
    errorId = SD_E_INV_ID;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificClientPartitionIdxOfClientIdMap(ClientServiceHandleId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif /* SD_MULTI_PARTITION == STD_ON */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_SizeOfClientType clientIdx = Sd_GetPartitionSpecificClientIdxOfClientIdMap(ClientServiceHandleId);
    const Sd_PartitionConfigIdxType partitionIdx =
      Sd_GetPartitionSpecificClientPartitionIdxOfClientIdMap(ClientServiceHandleId);

    /* #20 Verify that client is not member of any service group. */
    if(Sd_IsServiceGroupMemberOfClient(clientIdx, partitionIdx) == FALSE)
    {
      /* #30 Check the requested state and forward the request to the corresponding subfunction. */
      if(ClientServiceState == SD_CLIENT_SERVICE_REQUESTED)
      {
        Sd_UtilClient_SetRequested(clientIdx, partitionIdx);
      }
      else
      {
        Sd_UtilClient_SetReleased(clientIdx, partitionIdx);
      }

      retVal = E_OK;
    }
    else
    {
      errorId = SD_E_INV_ID;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_CLIENT_SERVICE_SET_STATE, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_ServerServiceSetInstanceId()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
FUNC(Std_ReturnType, SD_CODE) Sd_ServerServiceSetInstanceId(
  uint16 ServerServiceHandleId,
  uint16 ServerServiceInstanceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if (Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if (ServerServiceInstanceId == SD_ENTRY_WILDCARD_INSTANCE_ID)
  {
    errorId = SD_E_INV_INSTANCE_ID;
  }
  else if ((ServerServiceHandleId >= Sd_GetSizeOfServerIdMap()) ||
    (Sd_IsInvalidHndOfServerIdMap(ServerServiceHandleId) == TRUE))
  {
    errorId = SD_E_INV_ID;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificServerPartitionIdxOfServerIdMap(ServerServiceHandleId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif /* SD_MULTI_PARTITION == STD_ON */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_SizeOfServerType serverIdx = Sd_GetPartitionSpecificServerIdxOfServerIdMap(ServerServiceHandleId);
    const Sd_PartitionConfigIdxType partitionIdx =
      Sd_GetPartitionSpecificServerPartitionIdxOfServerIdMap(ServerServiceHandleId);

    /* #20 Set ServerServiceInstanceId. */
    SD_BEGIN_CRITICAL_SECTION();
    Sd_SetInstanceIdOfServerDyn(serverIdx, ServerServiceInstanceId, partitionIdx);                                      /* SBSW_SD_CSL03_CSL05 */
    SD_END_CRITICAL_SECTION();

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if (errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SERVER_SERVICE_SET_INSTANCE_ID, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_ClientServiceSetInstanceId()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
FUNC(Std_ReturnType, SD_CODE) Sd_ClientServiceSetInstanceId(
  uint16 ClientServiceHandleId,
  uint16 ClientServiceInstanceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if (Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if (ClientServiceInstanceId == SD_ENTRY_WILDCARD_INSTANCE_ID)
  {
    errorId = SD_E_INV_INSTANCE_ID;
  }
  else if ((ClientServiceHandleId >= Sd_GetSizeOfClientIdMap()) ||
    (Sd_IsInvalidHndOfClientIdMap(ClientServiceHandleId) == TRUE))
  {
    errorId = SD_E_INV_ID;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificClientPartitionIdxOfClientIdMap(ClientServiceHandleId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_SizeOfClientType clientIdx = Sd_GetPartitionSpecificClientIdxOfClientIdMap(ClientServiceHandleId);
    const Sd_PartitionConfigIdxType partitionIdx =
      Sd_GetPartitionSpecificClientPartitionIdxOfClientIdMap(ClientServiceHandleId);

    /* #20 Set ClientServiceInstanceId. */
    SD_BEGIN_CRITICAL_SECTION();
    Sd_SetInstanceIdOfClientDyn(clientIdx, ClientServiceInstanceId, partitionIdx);                                      /* SBSW_SD_CSL03_CSL05 */
    SD_END_CRITICAL_SECTION();

    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if (errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_CLIENT_SERVICE_SET_INSTANCE_ID, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_ServiceGroupStart()
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
FUNC(void, SD_CODE) Sd_ServiceGroupStart(
  uint16 ServiceGroupHandleId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if ((ServiceGroupHandleId >= Sd_GetSizeOfServiceGroupIdMap()) ||
    (Sd_IsInvalidHndOfServiceGroupIdMap(ServiceGroupHandleId) == TRUE))
  {
    errorId = SD_E_INV_ID;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificServiceGroupPartitionIdxOfServiceGroupIdMap(ServiceGroupHandleId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_SizeOfServiceGroupType serviceGroupIdx =
      Sd_GetPartitionSpecificServiceGroupIdxOfServiceGroupIdMap(ServiceGroupHandleId);
    const Sd_PartitionConfigIdxType partitionIdx =
      Sd_GetPartitionSpecificServiceGroupPartitionIdxOfServiceGroupIdMap(ServiceGroupHandleId);

    /* #20 Iterate over all members of the service group. */
    Sd_ServiceGroupMemberIterType memberIdx;
    for(memberIdx = Sd_GetServiceGroupMemberStartIdxOfServiceGroup(serviceGroupIdx, partitionIdx);
        memberIdx < Sd_GetServiceGroupMemberEndIdxOfServiceGroup(serviceGroupIdx, partitionIdx);
        memberIdx++)
    {
      const Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfServiceGroupMember(memberIdx, partitionIdx);
      const Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfServiceGroupMember(memberIdx, partitionIdx);

      /* #200 Set state to AVAILABLE in case of server and inc request counter. */
      if(serverIdx != SD_NO_SERVERIDXOFSERVICEGROUPMEMBER)
      {
        Sd_UtilServer_Configure(serverIdx, partitionIdx);
        Sd_IncServiceGroupReqCounterOfServerDyn(serverIdx, partitionIdx);                                               /* SBSW_SD_CSL03_CSL05 */
      }
      /* #201 Set state to REQUESTED in case of client and inc request counter. */
      else
      {
        Sd_UtilClient_SetRequested(clientIdx, partitionIdx);
        Sd_IncServiceGroupReqCounterOfClientDyn(clientIdx, partitionIdx);                                               /* SBSW_SD_CSL03_CSL05 */
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SERVICE_GROUP_START, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_ServiceGroupStop()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, SD_CODE) Sd_ServiceGroupStop(
  uint16 ServiceGroupHandleId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if ((ServiceGroupHandleId >= Sd_GetSizeOfServiceGroupIdMap()) ||
    (Sd_IsInvalidHndOfServiceGroupIdMap(ServiceGroupHandleId) == TRUE))
  {
    errorId = SD_E_INV_ID;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificServiceGroupPartitionIdxOfServiceGroupIdMap(ServiceGroupHandleId)) != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_SizeOfServiceGroupType serviceGroupIdx =
      Sd_GetPartitionSpecificServiceGroupIdxOfServiceGroupIdMap(ServiceGroupHandleId);
    const Sd_PartitionConfigIdxType partitionIdx =
      Sd_GetPartitionSpecificServiceGroupPartitionIdxOfServiceGroupIdMap(ServiceGroupHandleId);

    /* #20 Iterate over all members of the service group. */
    Sd_ServiceGroupMemberIterType memberIdx;

    for(memberIdx = Sd_GetServiceGroupMemberStartIdxOfServiceGroup(serviceGroupIdx, partitionIdx);
        memberIdx < Sd_GetServiceGroupMemberEndIdxOfServiceGroup(serviceGroupIdx, partitionIdx);
        memberIdx++)
    {
      Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfServiceGroupMember(memberIdx, partitionIdx);
      Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfServiceGroupMember(memberIdx, partitionIdx);

      /* #200 Set state to DOWN in case of server and if request counter is zero. */
      if(serverIdx != SD_NO_SERVERIDXOFSERVICEGROUPMEMBER)
      {
        if(Sd_GetServiceGroupReqCounterOfServerDyn(serverIdx, partitionIdx) != 0u)
        {
          Sd_DecServiceGroupReqCounterOfServerDyn(serverIdx, partitionIdx);                                             /* SBSW_SD_CSL03_CSL05 */

          if(Sd_GetServiceGroupReqCounterOfServerDyn(serverIdx, partitionIdx) == 0u)
          {
            Sd_UtilServer_SetDown(serverIdx, partitionIdx);
          }
        }
      }
      /* #201 Set state to RELEASED in case of client and if request counter is zero. */
      else
      {
        if(Sd_GetServiceGroupReqCounterOfClientDyn(clientIdx, partitionIdx) != 0u)
        {
          Sd_DecServiceGroupReqCounterOfClientDyn(clientIdx, partitionIdx);                                             /* SBSW_SD_CSL03_CSL05 */

          if(Sd_GetServiceGroupReqCounterOfClientDyn(clientIdx, partitionIdx) == 0u)
          {
            Sd_UtilClient_SetReleased(clientIdx, partitionIdx);
          }
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SERVICE_GROUP_STOP, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_ConsumedEventGroupSetState()
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
FUNC(Std_ReturnType, SD_CODE) Sd_ConsumedEventGroupSetState(
  uint16 ConsumedEventGroupHandleId,
  Sd_ConsumedEventGroupSetStateType ConsumedEventGroupState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SD_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if(SD_DEV_ERROR_DETECT == STD_ON)                                                                                      /* COV_SD_SAFE_BSW */
  /* #10 Check plausibility of input parameters. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  else if((ConsumedEventGroupState != SD_CONSUMED_EVENTGROUP_RELEASED) &&
          (ConsumedEventGroupState != SD_CONSUMED_EVENTGROUP_REQUESTED))
  {
    errorId = SD_E_INV_MODE;
  }
  else if ((ConsumedEventGroupHandleId >= Sd_GetSizeOfConsumedEventgroupIdMap()) ||
    (Sd_IsInvalidHndOfConsumedEventgroupIdMap(ConsumedEventGroupHandleId) == TRUE))
  {
    errorId = SD_E_INV_ID;
  }
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped partition index. */
  else if (Sd_Util_CheckApplicationIdValidity(
    Sd_GetPartitionSpecificConsumedEventgroupPartitionIdxOfConsumedEventgroupIdMap(ConsumedEventGroupHandleId))
    != E_OK)
  {
    errorId = SD_E_INV_APPLICATION_ID;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx =
      Sd_GetPartitionSpecificConsumedEventgroupIdxOfConsumedEventgroupIdMap(ConsumedEventGroupHandleId);
    const Sd_PartitionConfigIdxType partitionIdx =
      Sd_GetPartitionSpecificConsumedEventgroupPartitionIdxOfConsumedEventgroupIdMap(ConsumedEventGroupHandleId);
    const Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(consumedEventgroupIdx, partitionIdx);

    /* #30 Check if the consumed eventgroup is requested while the corresponding client service instance is not
     *     requested. */
    if((ConsumedEventGroupState == SD_CONSUMED_EVENTGROUP_REQUESTED) &&
      (Sd_GetStateSwcOfClientDyn(clientIdx, partitionIdx) != SD_CLIENT_SERVICE_REQUESTED))
    {
      /* #300 Do not update the state because of the actual state of the client. */
    }
    /* #40 Otherwise, process the consumed eventgroup request. */
    else
    {
      /* #400 Check if the state of the consumed eventgroup was changed to requested. */
      if((ConsumedEventGroupState == SD_CONSUMED_EVENTGROUP_REQUESTED) &&
        (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, partitionIdx) ==
          SD_CONSUMED_EVENTGROUP_RELEASED))
      {
        /* #4000 Trigger a subscription of the consumed eventgroup and reset the retry subscription counter. */
        Sd_SetTriggerSubscriptionOfConsumedEventgroupDyn(consumedEventgroupIdx, TRUE, partitionIdx);                    /* SBSW_SD_CSL03_CSL05 */
        Sd_SetRetryCounterOfClientDyn(clientIdx, 1u, partitionIdx);                                                     /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      }

      /* #401 Update the state. */
      Sd_SetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, ConsumedEventGroupState, partitionIdx);    /* SBSW_SD_CSL03_CSL05 */
      Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, partitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(clientIdx, partitionIdx), partitionIdx);
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #60 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_CONSUMED_EVENTGROUP_SET_STATE, errorId);
  }
#else
  SD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}

#if(SD_GET_RESET_MEASUREMENT_DATA_API == STD_ON)
/**********************************************************************************************************************
 *  Sd_GetAndResetMeasurementData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SD_CODE) Sd_GetAndResetMeasurementData(
  Sd_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SD_APPL_VAR) MeasurementDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_Sd_Rule-2.2_2981 */
  uint8 errorId = SD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(SD_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state and input parameters. */
  /* Do not check 'MeasurementDataPtr' since it can be NULL_PTR. */
  /* MeasurementIdx is checked in called function, check is not duplicated here. */
  if(Sd_State != SD_INIT)
  {
    errorId = SD_E_NOT_INITIALIZED;
  }
  /* PRQA S 2992,2996,2880 4 */ /* MD_Sd_BooleanValueRangeCheck */
  else if((MeasurementResetNeeded != TRUE) && (MeasurementResetNeeded != FALSE))
  {
    errorId = SD_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Try to get and reset measurement data. */
    retVal = Sd_Measure_GetAndReset(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);                        /* SBSW_SD_PASSING_API_POINTER */
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SD_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report default errors if any occurred. */
  if(errorId != SD_E_NO_ERROR)
  {
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_GET_RESET_MEASURE_DATA, errorId);
  }
# else
  SD_DUMMY_STATEMENT(errorId);
# endif

  return retVal;
}
#endif /* SD_GET_RESET_MEASUREMENT_DATA_API */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Sd_UtilClient_SetRequested()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_SetRequested(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is released. */
  if(Sd_GetStateSwcOfClientDyn(ClientIdx, PartitionIdx) == SD_CLIENT_SERVICE_RELEASED)
  {
    /* #100 Check if the remote server was not yet seen. */
    if(TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx, Sd_GetTimerTTLOfClientDyn(ClientIdx, PartitionIdx), TRUE))
    {
      /* #1000 Update the state and reset the TimeToTriggerSend. */
      Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
    }
    /* 200 Otherwise, if it was already seen. */
    else
    {
      /* #2000 Configure the client for consumed methods. */
      if(Sd_StateClient_Configure(ClientIdx, PartitionIdx) == E_OK)
      {
        /* #20000 Enter the main phase and inform the BswM. */
        Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SERVICE_READY, FALSE);
        Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, PartitionIdx, SD_CLIENT_SERVICE_AVAILABLE);
      }
    }
  }

  /* #20 Update the state. */
  Sd_SetStateSwcOfClientDyn(ClientIdx, SD_CLIENT_SERVICE_REQUESTED, PartitionIdx);                                      /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx));                         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_UtilClient_SetReleased()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_SetReleased(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx, PartitionIdx);
  Sd_ConsumedEventgroupIterType cegIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is configured for consumed methods. */
  if((methodIdx != SD_NO_METHODIDXOFCLIENT) &&
    (Sd_IsTcpConfiguredOfMethod(methodIdx, PartitionIdx) || Sd_IsUdpConfiguredOfMethod(methodIdx, PartitionIdx)))
  {
    /* #100 Disable routing of consumed methods. */
    (void)Sd_Util_DisableRouting(
      (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfMethod( methodIdx, PartitionIdx), PartitionIdx);

    if(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
    {
      /* #101 Unconfigure TCP for consumed methods. */
      (void)SoAd_CloseSoCon(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx, PartitionIdx), FALSE);
      Sd_SetMethodTcpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }

    if(Sd_GetMethodUdpSoConIdOfClientDyn(ClientIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
    {
      /* #102 Unconfigure UDP for consumed methods. */
      (void)SoAd_CloseSoCon(Sd_GetMethodUdpSoConIdOfClientDyn(ClientIdx, PartitionIdx), FALSE);
      Sd_SetMethodUdpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }

  /* #20 Process state change for Consumed Eventgroups. */
  for(cegIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx, PartitionIdx);
    cegIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx, PartitionIdx);
    cegIdx++)
  {
    /* #200 Pass the RELEASED state to all eventgroups. */
    Sd_SetServiceModeRequestOfConsumedEventgroupDyn(cegIdx, SD_CONSUMED_EVENTGROUP_RELEASED, PartitionIdx);             /* SBSW_SD_CSL03_CSL05 */

    /* #201 Unconfigure CEG and report state down if subscribed. */
    /*lint -e655 */
    if((Sd_GetStateMachineOfConsumedEventgroupDyn(cegIdx, PartitionIdx) & SD_CONSUMED_EVENTGROUP_STATE_MASK)
      != SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED) /*lint +e655 */
    {
      Sd_StateClient_ReleaseCeg((Sd_SizeOfConsumedEventgroupType)cegIdx, PartitionIdx);
    }
  }

  /* #30 Set state of client to released. */
  Sd_StateClient_SetStateReleased(ClientIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Init_General()
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
 */
/* PRQA S 6010, 6030 1 */ /* MD_Sd_STPTH_6010, MD_MSR_STCYC */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_General(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SoConModeDynIterType soConModeIdx = 0u;
  Sd_RxEventIterType rxEventIdx = 0u;
  Sd_AddressStorageIterType addressStorageIdx = 0u;
  Sd_InstanceIterType instanceIdx = 0u;
  Sd_RoutingGroupDynIterType routingGroupIdx = 0u;
  Sd_CommonInitialDelayIterType commonInitialDelayIdx = 0u;
  Sd_SoConGrpDynIterType soConGrpIdx = 0u;
  Sd_TriggerTransmitDynIterType triggerTransmitIdx = 0u;
  Sd_OptionIndexIterType optionIndexIdx = 0u;
  Sd_PayloadIterType payloadIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the socket connection mode map. */
  for (soConModeIdx = 0u; soConModeIdx < Sd_GetSizeOfSoConModeDyn(PartitionIdx); soConModeIdx++)
  {
    Sd_SetSoConModeDyn(soConModeIdx, SD_SOCON_MODE_OFFLINE, PartitionIdx);                                              /* SBSW_SD_CSL01_CSL05 */
  }

  /* #20 Initialize the RxEvent list. */
  for (rxEventIdx = 0u; rxEventIdx < Sd_GetSizeOfRxEvent(PartitionIdx); rxEventIdx++)
  {
    Sd_SetClientListIdxOfRxEvent(rxEventIdx, Sd_GetSizeOfClientList(PartitionIdx), PartitionIdx);                       /* SBSW_SD_CSL01_CSL05 */
    Sd_SetEventHandlerIdxOfRxEvent(rxEventIdx, Sd_GetSizeOfEventHandler(PartitionIdx), PartitionIdx);                   /* SBSW_SD_CSL01_CSL05 */
    Sd_SetEventOfRxEvent(rxEventIdx, SD_SERVER_EVENT_EVENTGROUP_NONE, PartitionIdx);                                    /* SBSW_SD_CSL01_CSL05 */
  }

  /* #30 Initialize the address storage array. */
  for (addressStorageIdx = 0u; addressStorageIdx < Sd_GetSizeOfAddressStorage(PartitionIdx); addressStorageIdx++)
  {
    /* Session id of 0 is not allowed -> unused/invalid address. */
    Sd_SetSessionIdCounterOfAddressStorage(addressStorageIdx, 0u, PartitionIdx);                                        /* SBSW_SD_CSL01_CSL05 */
    Sd_SetFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK | SD_FLAGS_UNICAST_MASK, PartitionIdx);         /* SBSW_SD_CSL01_CSL05 */
    Sd_SetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, 0u, PartitionIdx);                                      /* SBSW_SD_CSL01_CSL05 */
    Sd_SetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, 0u, PartitionIdx);                                    /* SBSW_SD_CSL01_CSL05 */
    Sd_SetRxUnicastFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK, PartitionIdx);                        /* SBSW_SD_CSL01_CSL05 */
    Sd_SetRxMulticastFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK, PartitionIdx);                      /* SBSW_SD_CSL01_CSL05 */
    Sd_SetSendEntryNrPendingIdxOfAddressStorage(addressStorageIdx, 0u, PartitionIdx);                                   /* SBSW_SD_CSL01_CSL05 */
    Sd_SetSendEntryHeadIdxOfAddressStorage(addressStorageIdx, Sd_GetSizeOfSendEntry(PartitionIdx), PartitionIdx);       /* SBSW_SD_CSL01_CSL05 */
    Sd_SetSendEntryTailIdxOfAddressStorage(addressStorageIdx, Sd_GetSizeOfSendEntry(PartitionIdx), PartitionIdx);       /* SBSW_SD_CSL01_CSL05 */
    Sd_SetRebootDetectedOfAddressStorage(addressStorageIdx, FALSE, PartitionIdx);                                       /* SBSW_SD_CSL01_CSL05 */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToSendOfAddressStorage(addressStorageIdx, PartitionIdx));                 /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT */
  }

  /* #40 Init Local Ports of SoCon Groups. */
  for (soConGrpIdx = 0u; soConGrpIdx < Sd_GetSizeOfSoConGrpDyn(PartitionIdx); soConGrpIdx++)
  {
    Sd_SetLocalPortOfSoConGrpDyn(soConGrpIdx, 0u, PartitionIdx);                                                        /* SBSW_SD_CSL01_CSL05 */
  }

  /* #50 Set the session ID of the SD multicast address. The address will be set during Sd_Init_ReInitialize(). */
  for (instanceIdx = 0u; instanceIdx < Sd_GetSizeOfInstance(PartitionIdx); instanceIdx++)
  {
    Sd_SetSessionIdCounterOfAddressStorage(Sd_GetAddressStorageStartIdxOfInstance(instanceIdx, PartitionIdx),           /* SBSW_SD_CSL03_CSL05 */
      1u, PartitionIdx);
  }

  /* #60 Initialize the status of the SoAd RoutingGroups to unknown. */
  for (routingGroupIdx = 0u; routingGroupIdx < Sd_GetSizeOfRoutingGroupDyn(PartitionIdx); routingGroupIdx++)
  {
    Sd_SetStatusOfRoutingGroupDyn(routingGroupIdx, SD_ROUTINGGROUP_STATUS_UNKNOWN, PartitionIdx);                       /* SBSW_SD_CSL01_CSL05 */
  }

  /* #70 Set all common initial delays to invalid. */
  for (commonInitialDelayIdx = 0u;
       commonInitialDelayIdx < Sd_GetSizeOfCommonInitialDelay(PartitionIdx);
       commonInitialDelayIdx++)
  {
    Sd_SetCommonInitialDelay(commonInitialDelayIdx, SD_INV_TIMER_VALUE_32BIT, PartitionIdx);                            /* SBSW_SD_CSL01_CSL05 */
  }

  /* #80 Initialize trigger transmit list. */
  for (triggerTransmitIdx = 0u;
       triggerTransmitIdx < Sd_GetSizeOfTriggerTransmitDyn(PartitionIdx);
       triggerTransmitIdx++)
  {
    Sd_SetClientListIdxOfTriggerTransmitDyn(triggerTransmitIdx, Sd_GetSizeOfClientList(PartitionIdx),                   /* SBSW_SD_CSL01_CSL05 */
      PartitionIdx); /* Invalid value. */
    Sd_SetEventHandlerIdxOfTriggerTransmitDyn(triggerTransmitIdx, Sd_GetSizeOfEventHandler(PartitionIdx),               /* SBSW_SD_CSL01_CSL05 */
      PartitionIdx); /* Invalid value. */
    Sd_SetStateOfTriggerTransmitDyn(triggerTransmitIdx, SD_TRIGGER_TRANSMIT_STATE_INVALID, PartitionIdx);               /* SBSW_SD_CSL01_CSL05 */
  }

  /* #90 Initialize option index array. */
  for (optionIndexIdx = 0u; optionIndexIdx < Sd_GetSizeOfOptionIndex(PartitionIdx); optionIndexIdx++)
  {
    Sd_SetOptionIndex(optionIndexIdx, 0u, PartitionIdx);                                                                /* SBSW_SD_CSL01_CSL05 */
  }

  /* #100 Initialize payload buffer. */
  for (payloadIdx = 0u; payloadIdx < Sd_GetSizeOfPayload(PartitionIdx); payloadIdx++)
  {
    Sd_SetPayload(payloadIdx, 0u, PartitionIdx);                                                                        /* SBSW_SD_CSL01_CSL05 */
  }
}

/**********************************************************************************************************************
 *  Sd_Init_Instance()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Instance(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all instances. */
  for(instanceIdx = 0u; instanceIdx < Sd_GetSizeOfInstance(PartitionIdx); instanceIdx++)
  {
    /* #20 Trigger an initial run of the state machines. */
    Sd_Util_TriggerStateMachineRun(instanceIdx, PartitionIdx);

    /* #30 Initialize the dynamic data structures. */
    Sd_Util_ResetTimerToZero(Sd_GetAddrNextStateMachineRunOfInstanceDyn(instanceIdx, PartitionIdx));                    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_SAME_SIZE */

    if(Sd_FirstInit == TRUE)
    {
      /* #40 The link state is only changed at the initial run after a reboot. */
      Sd_SetStateOfInstanceDyn(instanceIdx, SD_INSTANCE_DOWN, PartitionIdx);                                            /* SBSW_SD_CSL02_CSL05 */
      Sd_SetRunModeOfInstanceDyn(instanceIdx, SD_RUN_MODE_INITIAL_RUN, PartitionIdx);                                   /* SBSW_SD_CSL02_CSL05 */
    }

    /* #50 Initialize the RX event list. */
    Sd_SetRxEventListFullOfInstanceDyn(instanceIdx, FALSE, PartitionIdx);                                               /* SBSW_SD_CSL02_CSL05 */
    Sd_SetRxEventStartIdxOfInstanceDyn(instanceIdx, Sd_GetRxEventStartIdxOfInstance(instanceIdx, PartitionIdx),         /* SBSW_SD_CSL02_CSL05 */
      PartitionIdx);
    Sd_SetRxEventEndIdxOfInstanceDyn(instanceIdx, Sd_GetRxEventStartIdxOfInstance(instanceIdx, PartitionIdx),           /* SBSW_SD_CSL02_CSL05 */
      PartitionIdx);

    /* #60 Initialize the send entries list. */
    Sd_Init_SendEntryList(instanceIdx, PartitionIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_Init_PartitionData()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_PartitionData(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize component timer. */
  Sd_Util_ResetTimerToZero(Sd_GetAddrTimeMsSinceStartOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx));          /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_PARTITION_DATA */

  /* #20 Initialize partition specific variables. */
  Sd_SetRxIndicationCountOfPartitionDataDyn(SD_PARTITION_DATA_IDX, 0u, PartitionIdx);                                   /* SBSW_SD_PARTITION_DATA_CSL05 */
  Sd_SetTxPathLockedOfPartitionDataDyn(SD_PARTITION_DATA_IDX, FALSE, PartitionIdx);                                     /* SBSW_SD_PARTITION_DATA_CSL05 */
  Sd_SetNrOfOptionsOfPartitionDataDyn(SD_PARTITION_DATA_IDX, 0u, PartitionIdx);                                         /* SBSW_SD_PARTITION_DATA_CSL05 */

  /* #30 Initialize trigger transmit control structure. */
  Sd_SetTriggerTransmitFullOfPartitionDataDyn(SD_PARTITION_DATA_IDX, FALSE, PartitionIdx);                              /* SBSW_SD_PARTITION_DATA_CSL05 */
  Sd_SetTriggerTransmitDynStartIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, 0u, PartitionIdx);                          /* SBSW_SD_PARTITION_DATA_CSL05 */
  Sd_SetTriggerTransmitDynEndIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, 0u, PartitionIdx);                            /* SBSW_SD_PARTITION_DATA_CSL05 */
}

/**********************************************************************************************************************
 *  Sd_Init_SendEntryList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_SendEntryList(
Sd_SizeOfInstanceType InstanceIdx,
Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SendEntryIterType sendEntryIdx;
  Sd_SizeOfSendEntryType invalidIdx = Sd_GetSizeOfSendEntry(PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Init variable send entry data of instance. */
  Sd_SetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, 0u, PartitionIdx);                                              /* SBSW_SD_API_ARGUMENT_IDX */
  Sd_SetSendEntryFreeListHeadIdxOfInstanceDyn(InstanceIdx,                                                              /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_GetSendEntryStartIdxOfInstance(InstanceIdx, PartitionIdx), PartitionIdx);
  Sd_SetSendEntryFreeListTailIdxOfInstanceDyn(InstanceIdx,                                                              /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_GetSendEntryEndIdxOfInstance(InstanceIdx, PartitionIdx) - 1u, PartitionIdx);

  /* #20 Init section of the doubly linked send entries list associated with current instance. */
  for(sendEntryIdx = Sd_GetSendEntryStartIdxOfInstance(InstanceIdx, PartitionIdx);
    sendEntryIdx < Sd_GetSendEntryEndIdxOfInstance(InstanceIdx, PartitionIdx);
    sendEntryIdx++)
  {
    Sd_SetSendEntryPrevIdxOfSendEntry(sendEntryIdx,                                                                     /* SBSW_SD_CSL01_CSL05 */
      (sendEntryIdx == Sd_GetSendEntryStartIdxOfInstance(InstanceIdx, PartitionIdx)) ?
      invalidIdx : (Sd_SizeOfSendEntryType)(sendEntryIdx - (Sd_SendEntryIterType)1u),
      PartitionIdx);
    Sd_SetSendEntryNextIdxOfSendEntry(sendEntryIdx,                                                                     /* SBSW_SD_CSL01_CSL05 */
      (sendEntryIdx == (Sd_GetSendEntryEndIdxOfInstance(InstanceIdx, PartitionIdx) - (Sd_SendEntryIterType)1u)) ?
      invalidIdx : (Sd_SizeOfSendEntryType)(sendEntryIdx + (Sd_SendEntryIterType)1u),
      PartitionIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_Init_Server()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Server(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_ServerIterType serverIdx;
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  Sd_ClientListIterType clientListIdx;
  Sd_SoConActivationMapIterType soConActivationMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the servers. */
  for(serverIdx = 0u; serverIdx < Sd_GetSizeOfServer(PartitionIdx); serverIdx++)
  {
    /* #100 Set the initial state of the server. */
    if(Sd_IsAutoAvailableOfServer(serverIdx, PartitionIdx))
    {
      Sd_SetStateSwcOfServerDyn(serverIdx, SD_SERVER_SERVICE_AVAILABLE, PartitionIdx);                                  /* SBSW_SD_CSL02_CSL05 */
    }
    else
    {
      Sd_SetStateSwcOfServerDyn(serverIdx, SD_SERVER_SERVICE_DOWN, PartitionIdx);                                       /* SBSW_SD_CSL02_CSL05 */
    }

    /* #101 Initialize all dynamic variables. */
    Sd_SetInstanceIdOfServerDyn(serverIdx, Sd_GetInitialInstanceIdOfServer(serverIdx, PartitionIdx), PartitionIdx);     /* SBSW_SD_CSL02_CSL05 */
    Sd_SetStateMachineOfServerDyn(serverIdx, SD_SERVER_STATE_NONE, PartitionIdx);                                       /* SBSW_SD_CSL02_CSL05 */
    Sd_SetServiceGroupReqCounterOfServerDyn(serverIdx, 0u, PartitionIdx);                                               /* SBSW_SD_CSL02_CSL05 */
    Sd_SetStateMachineExecutionRequestedOfServerDyn(serverIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_CSL02_CSL05 */
    Sd_SetInitialOfferRepetitionRunOfServerDyn(serverIdx, 0u, PartitionIdx);                                            /* SBSW_SD_CSL02_CSL05 */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfServerDyn(serverIdx, PartitionIdx));                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_SAME_SIZE */
  }

  /* #20 Initialize the event handlers. */
  for(eventHandlerIdx = 0u; eventHandlerIdx < Sd_GetSizeOfEventHandler(PartitionIdx); eventHandlerIdx++)
  {
    /* #200 Initialize all dynamic variables. */
    Sd_SetStateMachineOfEventHandlerDyn(eventHandlerIdx, SD_EVENT_HANDLER_STATE_NONE, PartitionIdx);                    /* SBSW_SD_CSL02_CSL05 */
    Sd_SetNrOfClientsOfEventHandlerDyn(eventHandlerIdx, 0u, PartitionIdx);                                              /* SBSW_SD_CSL02_CSL05 */
    Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, PartitionIdx, SD_EVENT_HANDLER_RELEASED);
  }

  /* #30 Initialize the client list. */
  for(clientListIdx = 0u; clientListIdx < Sd_GetSizeOfClientList(PartitionIdx); clientListIdx++)
  {
    Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID, PartitionIdx);                  /* SBSW_SD_CSL01_CSL05 */
    Sd_SetNrOfReferencesOfClientList(clientListIdx, 0u, PartitionIdx);                                                  /* SBSW_SD_CSL01_CSL05 */
  }

  /* #40 Initialize the socket connection activation map. */
  for(soConActivationMapIdx = 0u;
      soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap(PartitionIdx);
      soConActivationMapIdx++)
  {
    Sd_SetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, 0u, PartitionIdx);                                 /* SBSW_SD_CSL01_CSL05 */
    Sd_SetSoConIdOfSoConActivationMap(soConActivationMapIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                         /* SBSW_SD_CSL01_CSL05 */
  }
}

/**********************************************************************************************************************
 *  Sd_Init_Client()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_Client(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;
  Sd_RequestCntIterType requestCntIdx;
  Sd_PortSetIterType portSetIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the clients. */
  for(clientIdx = 0u; clientIdx < Sd_GetSizeOfClient(PartitionIdx); clientIdx++)
  {
    /* #100 Inform the BswM if the Sd_Init() is called while being in the initial wait phase. */
    if((Sd_FirstInit == FALSE) &&
       (Sd_GetStateMachineOfClientDyn(clientIdx, PartitionIdx) == SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT))
    {
      Sd_ConsumedEventgroupIterType consumedEventgroupIdx;

      /* #1000 Inform the BswM about the state of the client service and all consumed eventgroups. */
      Sd_UtilClient_SetCurrentStateInformBswm(clientIdx, PartitionIdx, SD_CLIENT_SERVICE_DOWN);

      for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(clientIdx, PartitionIdx);
        consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(clientIdx, PartitionIdx);
        consumedEventgroupIdx++)
      {
        BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx),
          SD_CONSUMED_EVENTGROUP_DOWN);
      }
    }

    /* #101 Set the initial state of the clients. */
    if(Sd_IsAutoRequireOfClient(clientIdx, PartitionIdx))
    {
      Sd_SetStateSwcOfClientDyn(clientIdx, SD_CLIENT_SERVICE_REQUESTED, PartitionIdx);                                  /* SBSW_SD_CSL02_CSL05 */
    }
    else
    {
      Sd_SetStateSwcOfClientDyn(clientIdx, SD_CLIENT_SERVICE_RELEASED, PartitionIdx);                                   /* SBSW_SD_CSL02_CSL05 */
    }

    /* #102 Initialize all dynamic variables. */
    Sd_UtilClient_ResetDynamicVariables(clientIdx, PartitionIdx);
    Sd_SetInstanceIdOfClientDyn(clientIdx, Sd_GetInitialInstanceIdOfClient(clientIdx, PartitionIdx), PartitionIdx);     /* SBSW_SD_CSL02_CSL05 */
    Sd_SetServiceGroupReqCounterOfClientDyn(clientIdx, 0u, PartitionIdx);                                               /* SBSW_SD_CSL02_CSL05 */
  }

  /* #20 Initialize all consumed eventgroups. */
  Sd_Init_ClientCeg(PartitionIdx);

  /* #30 Initialize the Request Count and Port Set flag for dynamic multicast addresses. */
  for(requestCntIdx = 0u; requestCntIdx < Sd_GetSizeOfRequestCnt(PartitionIdx); requestCntIdx++)
  {
    Sd_SetRequestCnt(requestCntIdx, 0u, PartitionIdx);                                                                  /* SBSW_SD_CSL01_CSL05 */
  }

  for(portSetIdx = 0u; portSetIdx < Sd_GetSizeOfPortSet(PartitionIdx); portSetIdx++)
  {
    Sd_SetPortSet(portSetIdx, FALSE, PartitionIdx);                                                                     /* SBSW_SD_CSL01_CSL05 */
  }
}

/**********************************************************************************************************************
 *  Sd_Init_ClientCeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ClientCeg(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_ConsumedEventgroupIterType consumedEventgroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all consumed eventgroups. */
  for(consumedEventgroupIdx = 0u;
      consumedEventgroupIdx < Sd_GetSizeOfConsumedEventgroup(PartitionIdx);
      consumedEventgroupIdx++)
  {
    /* #100 Set the initial state of the consumed eventgroups. */
    if(Sd_IsAutoRequireOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx))
    {
      Sd_SetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_REQUESTED,          /* SBSW_SD_CSL02_CSL05 */
        PartitionIdx);
    }
    else
    {
      Sd_SetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_RELEASED,           /* SBSW_SD_CSL02_CSL05 */
        PartitionIdx);
    }

    /* #101 Initialize all dynamic variables. */
    Sd_SetCurrentStateOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN, PartitionIdx);        /* SBSW_SD_CSL02_CSL05 */
    Sd_SetEventOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE, PartitionIdx);           /* SBSW_SD_CSL02_CSL05 */
    Sd_SetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED,         /* SBSW_SD_CSL02_CSL05 */
      PartitionIdx);
    Sd_Util_ResetTimerToZero(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx));           /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_SAME_SIZE */
    Sd_SetAcknowledgedTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, 0u, PartitionIdx);                              /* SBSW_SD_CSL02_CSL05 */
    Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);             /* SBSW_SD_CSL02_CSL05 */
    Sd_SetUdpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                   /* SBSW_SD_CSL02_CSL05 */
    Sd_SetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                   /* SBSW_SD_CSL02_CSL05 */
    Sd_SetTriggerSubscriptionOfConsumedEventgroupDyn(consumedEventgroupIdx, FALSE, PartitionIdx);                       /* SBSW_SD_CSL02_CSL05 */

    if(Sd_IsMulticastEndpointAddrUsedOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx))
    {
      Sd_MulticastEndpointAddrIdxOfConsumedEventgroupType multicastEndpointAddrIdx =
        Sd_GetMulticastEndpointAddrIdxOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx);

      (Sd_GetAddrMulticastEndpointAddr(multicastEndpointAddrIdx, PartitionIdx))->family = SOAD_AF_INVALID;              /* SBSW_SD_CSL03_CSL05 */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_CloseAllSocketsOfInstance()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_CloseAllSocketsOfInstance(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  Sd_TcpSoConIdIterType tcpSoConIdIdx;
  Sd_UdpSoConIterType udpSoConIdx;

  /* #10 Close the TCP socket connections of the instance. */
  for(tcpSoConIdIdx = Sd_GetTcpSoConIdStartIdxOfInstance(InstanceIdx, PartitionIdx);
      tcpSoConIdIdx < Sd_GetTcpSoConIdEndIdxOfInstance(InstanceIdx, PartitionIdx);
      tcpSoConIdIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetTcpSoConId(tcpSoConIdIdx, PartitionIdx), TRUE);
  }

  /* #20 Close the UDP socket connections of the instance. */
  for(udpSoConIdx = Sd_GetUdpSoConStartIdxOfInstance(InstanceIdx, PartitionIdx);
      udpSoConIdx < Sd_GetUdpSoConEndIdxOfInstance(InstanceIdx, PartitionIdx);
      udpSoConIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetIdOfUdpSoCon(udpSoConIdx, PartitionIdx), TRUE);
  }
}

/**********************************************************************************************************************
 *  Sd_Init_ReInitialize()
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
/* PRQA S 6050 1 */ /* MD_Sd_STCAL_6050 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitialize(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_EndpointStorageIterType endpointStorageIdx;
  Sd_SockAddrContainerType localAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear the list of endpoint entries. */
  for(endpointStorageIdx = 0u; endpointStorageIdx < Sd_GetSizeOfEndpointStorage(PartitionIdx); endpointStorageIdx++)
  {
    Sd_SetNrOfReferencesOfEndpointStorage(endpointStorageIdx, 0u, PartitionIdx);                                        /* SBSW_SD_CSL01_CSL05 */
    Sd_SetTypeOfEndpointStorage(endpointStorageIdx, SD_ENDPOINT_TYPE_INVALID, PartitionIdx);                            /* SBSW_SD_CSL01_CSL05 */
    (Sd_GetAddrAddrOfEndpointStorage(endpointStorageIdx, PartitionIdx))->family = SOAD_AF_INVALID;                      /* SBSW_SD_CSL01_CSL05 */
  }

  /* #20 Get the Sd multicast address. */
  localAddr.family = SOAD_AF_INVALID;
  retVal = Sd_SoAd_GetLocalAddr(Sd_GetMulticastRxSoConIdOfInstance(InstanceIdx, PartitionIdx), &localAddr);             /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    /* #200 Save the multicast address and initialize the SessionID counter. */
    Sd_Util_CopySockAddr(Sd_GetAddrAddrOfAddressStorage(                                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx, PartitionIdx),
      PartitionIdx), &localAddr);
    Sd_Util_ResetSessionIds(InstanceIdx, PartitionIdx);

    /* #201 Set the SoAd remote address of the Rx and Tx socket connections to multicast in order to initialize (open)
     *      the socket. */
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    (void)SoAd_SetRemoteAddr(Sd_GetTxSoConIdOfInstance(InstanceIdx, PartitionIdx),                                      /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_SOCK_ADDR_CAST */
      (SD_P2VAR(SoAd_SockAddrType))&localAddr);
    (void)SoAd_OpenSoCon(Sd_GetTxSoConIdOfInstance(InstanceIdx, PartitionIdx));

    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    (void)SoAd_SetRemoteAddr(Sd_GetMulticastRxSoConIdOfInstance(InstanceIdx, PartitionIdx),                             /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_SOCK_ADDR_CAST */
      (SD_P2VAR(SoAd_SockAddrType))&localAddr);
    (void)SoAd_OpenSoCon(Sd_GetMulticastRxSoConIdOfInstance(InstanceIdx, PartitionIdx));

    /* #202 Initialize all server data. */
    Sd_Init_ReInitializeServers(InstanceIdx, PartitionIdx);

    /* #203 Initialize all client data. */
    Sd_Init_ReInitializeClients(InstanceIdx, PartitionIdx);

    /* #204 Close all sockets of this instance. */
    Sd_CloseAllSocketsOfInstance(InstanceIdx, PartitionIdx);
  }
}

/**********************************************************************************************************************
 *  SERVER FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Sd_Init_ReInitializeServers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeServers(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfServerType serverIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all server to update and process the state machine. */
  for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx, PartitionIdx);
    serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx, PartitionIdx);
    serverIdx++)
  {
    /* #100 Initialize all event handlers of the server. */
    Sd_Init_ReInitializeEventHandler(serverIdx, PartitionIdx);

    /* #101 Disable routing for provided methods. */
    Sd_UtilServer_DisableMethodRouting(serverIdx, PartitionIdx);

    /* #102 Set Flag for executing state machine of server. */
    Sd_SetStateMachineExecutionRequestedOfServerDyn(serverIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
  }
}

/**********************************************************************************************************************
 *  Sd_Init_ReInitializeEventHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeEventHandler(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerType eventHandlerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all event handlers, disable the RoutingGroups and close the socket connections. */
  for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx, PartitionIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx, PartitionIdx);
      eventHandlerIdx++)
  {
    Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx;
    Sd_ClientListIterType clientListIdx;

    /* #100 Reset multicast event handler configuration. */
    eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
    if(eventHandlerMulticastIdx != SD_NO_EVENTHANDLERMULTICASTIDXOFEVENTHANDLER)
    {
      (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerMulticast(
        eventHandlerMulticastIdx, PartitionIdx), PartitionIdx);
      (void)SoAd_CloseSoCon(Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx, PartitionIdx), TRUE);
    }

    /* #101 Disable routing of UDP and TCP event handler and reset activation count. */
    Sd_UtilServer_DisableRouting(eventHandlerIdx, PartitionIdx);

    /* #102 Clear the list of subscribed clients. */
    Sd_SetNrOfClientsOfEventHandlerDyn(eventHandlerIdx, 0u, PartitionIdx);                                              /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
      clientListIdx < Sd_GetClientListEndIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
      clientListIdx++)
    {
      Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID, PartitionIdx);                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_StateServer_Process()
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
/* PRQA S 6050 1 */ /* MD_Sd_STCAL_6050 */
SD_LOCAL FUNC(void, SD_CODE) Sd_StateServer_Process(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle the state machine within the ready block. */
  switch(Sd_GetStateMachineOfServerDyn(ServerIdx, PartitionIdx))
  {
    case SD_SERVER_STATE_NONE:
    case SD_SERVER_STATE_NOT_READY:
      Sd_StateServer_ProcessNotReady(ServerIdx, PartitionIdx);
      break;

    case SD_SERVER_STATE_READY_INITIAL_WAIT:
      Sd_StateServer_ProcessInitialWait(ServerIdx, PartitionIdx);
      break;

    case SD_SERVER_STATE_READY_REPETITION:
      Sd_StateServer_ProcessRepetition(ServerIdx, PartitionIdx);
      break;

    case SD_SERVER_STATE_READY_MAIN_PHASE:
      Sd_StateServer_ProcessMain(ServerIdx, PartitionIdx);
      break;

    default: /* COV_SD_MISRA */
      /* Undefined state. */
      break;
  }

  /* #20 Update state if instance is down. */
  if((Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) == SD_INSTANCE_DOWN) &&
     (Sd_GetStateMachineOfServerDyn(ServerIdx, PartitionIdx) >= SD_SERVER_STATE_READY_INITIAL_WAIT))
  {
    /* #200 Leave the ready-state and reset the TimeToTriggerSend. */
    Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_NOT_READY, TRUE);
  }

  /* #30 Run the state machine, handling all event handlers of the server instance. */
  Sd_StateServer_ProcessEventHandlerStateMachine(ServerIdx, PartitionIdx);

  /* #40 Update the time for the next state machine run. */
  Sd_Util_SetNextStateMachineRun(instanceIdx, PartitionIdx,
    Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx));
}

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessNotReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessNotReady(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if it is the initial run of the server state machine. */
  if(Sd_GetStateMachineOfServerDyn(ServerIdx, PartitionIdx) == SD_SERVER_STATE_NONE)
  {
    /* #100 First run of the state machine. Update state. */
    if((Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) == SD_INSTANCE_UP_AND_CONFIGURED) &&
      (Sd_GetStateSwcOfServerDyn(ServerIdx, PartitionIdx) == SD_SERVER_SERVICE_AVAILABLE))
    {
      Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_READY_INITIAL_WAIT, FALSE);
    }
    else
    {
      Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_NOT_READY, FALSE);
    }
  }
  else /* Sd_GetStateMachineOfServerDyn(ServerIdx) == SD_SERVER_STATE_NOT_READY */
  {
    /* #101 Otherwise, check if the server is requested and the instance is up and configured. */
    if((Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) == SD_INSTANCE_UP_AND_CONFIGURED) &&
      (Sd_GetStateSwcOfServerDyn(ServerIdx, PartitionIdx) == SD_SERVER_SERVICE_AVAILABLE))
    {
      /* #1010 Update the state and reset the TimeToTriggerSend. */
      Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_READY_INITIAL_WAIT, TRUE);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessInitialWait()
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
 */
/* PRQA S 6010, 6030 1 */ /* MD_Sd_STPTH_6010, MD_MSR_STCYC */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessInitialWait(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_CommonInitialDelayIdxOfServerType commonDelayIdx = Sd_GetCommonInitialDelayIdxOfServer(ServerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the current state of the instance is not UP_AND_CONFIGURED. */
  if(Sd_GetStateOfInstanceDyn(Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx), PartitionIdx) !=
       SD_INSTANCE_UP_AND_CONFIGURED)
  {
    /* #100 Instance is not up and configured. Update state. */
    Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_NOT_READY, FALSE);
  }
  /* #20 Otherwise, check if this is the first run of the wait state.*/
  else if(Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx)))
  {
    /* #200 Instance is up and configured and first run of the wait state, configure server. */
    Sd_UtilServer_Configure(ServerIdx, PartitionIdx);
  }
  /* #30 Otherwise this is a succeeding run of the wait state. */
  else
  {
    /* #300 Check if the timer is exceeded. */
    if(TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx,
         Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx), TRUE))
    {
      /* #3000 Update the state and reset the TimeToTriggerSend. */
      Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_READY_REPETITION, TRUE);

      /* #3001 Send an OfferService message. */
      Sd_Util_AddMulticastOfferEntryToList(ServerIdx, PartitionIdx, FALSE);

      /* #3002 Reset common delay. */
      if(commonDelayIdx != SD_NO_COMMONINITIALDELAYIDXOFSERVER)
      {
        Sd_SetCommonInitialDelay(commonDelayIdx, SD_INV_TIMER_VALUE_32BIT, PartitionIdx);                               /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      }
    }

    /* #301 Mark server for execution of state machine. */
    Sd_SetStateMachineExecutionRequestedOfServerDyn(ServerIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessRepetition()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessRepetition(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);
  Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the repetition phase is entered the first time.*/
  if(Sd_GetInitialOfferRepetitionsMaxOfServerTimer(serverTimerIdx, PartitionIdx) == 0u)
  {
    /* #100 Go to the main state if there is no repetition phase configured.
     *      Update the state and reset the TimeToTriggerSend.*/
    Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_READY_MAIN_PHASE, TRUE);
  }
  else if(Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx)))
  {
    /* #200 Otherwise if this is the initial run of the repetition phase.
     *      Reset the repetition counter, initialize the timer and mark server for execution of state machine. */
    const uint32 timerOffset =
      (uint32)Sd_GetInitialOfferRepetitionBaseDelayOfServerTimer(serverTimerIdx, PartitionIdx) -
      SD_MAIN_FCT_CYCLE_TIME_MS;
    Sd_SetInitialOfferRepetitionRunOfServerDyn(ServerIdx, 0u, PartitionIdx);                                            /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx), timerOffset,
      instanceIdx, PartitionIdx);                                                                                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_SetStateMachineExecutionRequestedOfServerDyn(ServerIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
  else
  {
    /* No further state transitions available. */
  }

  /* #30 Succeeding runs of the repetition phase, Timer has expired. */
  if((Sd_Util_CheckIfTimerExceeded(PartitionIdx, Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx), TRUE)
        == TRUE) &&
     (Sd_GetInitialOfferRepetitionsMaxOfServerTimer(serverTimerIdx, PartitionIdx) >
        Sd_GetInitialOfferRepetitionRunOfServerDyn(ServerIdx, PartitionIdx)) &&
     (Sd_GetStateMachineOfServerDyn(ServerIdx, PartitionIdx) == SD_SERVER_STATE_READY_REPETITION))
  {
    /* #300 Send an OfferService message and wait for the next repetition. */
    Sd_Util_AddMulticastOfferEntryToList(ServerIdx, PartitionIdx, FALSE);
    Sd_IncInitialOfferRepetitionRunOfServerDyn(ServerIdx, PartitionIdx);                                                /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    if(Sd_GetInitialOfferRepetitionsMaxOfServerTimer(serverTimerIdx, PartitionIdx) ==
         Sd_GetInitialOfferRepetitionRunOfServerDyn(ServerIdx, PartitionIdx))
    {
      /* #301 Update the state and reset the TimeToTriggerSend. */
      Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_READY_MAIN_PHASE, TRUE);
    }
    else
    {
      /* #302 Wind up the timer and mark server for execution of state machine. */
      const uint32 timerOffset = (uint32)((((uint32)(1u)) << Sd_GetInitialOfferRepetitionRunOfServerDyn(ServerIdx, PartitionIdx)) *
        Sd_GetInitialOfferRepetitionBaseDelayOfServerTimer(serverTimerIdx, PartitionIdx));
      Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx), timerOffset,
        instanceIdx, PartitionIdx);                                                                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_SetStateMachineExecutionRequestedOfServerDyn(ServerIdx, TRUE, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }
  /* #40 Timer has not expired yet, request execution of state machine to wait for expiration.  */
  else
  {
    Sd_SetStateMachineExecutionRequestedOfServerDyn(ServerIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessMain()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessMain(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);
  Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Mark server for execution of state machine. */
  Sd_SetStateMachineExecutionRequestedOfServerDyn(ServerIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Check if the server shall offer the current service instance. */
  if(Sd_GetOfferCyclicDelayOfServerTimer(serverTimerIdx, PartitionIdx) != 0u)
  {
    /* #200 Check if initial run of the main phase. */
    if(Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx)))
    {
      /* #2000 Set timer to send the first offer message after the configured OfferCyclicDelay. */
      Sd_Util_SetTimerOffsetWithTrigger(
        Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx),
        (uint32)Sd_GetOfferCyclicDelayOfServerTimer(serverTimerIdx, PartitionIdx) - SD_MAIN_FCT_CYCLE_TIME_MS,
        instanceIdx, PartitionIdx);                                                                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }

    /* #201 Check if timer is expired. */
    if(TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx,
         Sd_GetTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx), TRUE))
    {
      /* #2010 Send an OfferService message. */
      Sd_Util_AddMulticastOfferEntryToList(ServerIdx, PartitionIdx, FALSE);

      /* #2011 Wind up the timer. */
      Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx),
        Sd_GetOfferCyclicDelayOfServerTimer(serverTimerIdx, PartitionIdx), instanceIdx, PartitionIdx);                  /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }
  }
  else
  {
    /* No offer messages are transmitted for this service. */
  }
}

/**********************************************************************************************************************
 *  Sd_StateServer_ProcessEventHandlerStateMachine()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_ProcessEventHandlerStateMachine(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerType eventHandlerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle all event handlers of the server. */
  for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx, PartitionIdx);
    eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx, PartitionIdx);
    eventHandlerIdx++)
  {
    /* #100 Update the state of the event handler. */
    Sd_StateServer_UpdateEventHandlerState(eventHandlerIdx, PartitionIdx);

    /* #200 Update the state of all subscribed clients. */
    Sd_StateServer_UpdateEventHandlerClients(eventHandlerIdx, PartitionIdx);

    /* #300 Switch to multicast if this functionality is configured and the corresponding amount of subscribed clients is reached. */
    if(Sd_EventHandlerUsesMulticast(eventHandlerIdx, PartitionIdx) &&
      (Sd_GetMulticastThresholdOfEventHandler(eventHandlerIdx, PartitionIdx) <=
         Sd_GetNrOfClientsOfEventHandlerDyn(eventHandlerIdx, PartitionIdx)) &&
      (Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx, PartitionIdx) ==
         SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION))
    {
      Sd_StateServer_EventHandlerSwitchMulticast(eventHandlerIdx, PartitionIdx);
    }

    /* #400 Switch back to unicast if this functionality is configured and the multicast threshold is not reached. */
    if(Sd_EventHandlerUsesUdp(eventHandlerIdx, PartitionIdx) &&
      (Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx, PartitionIdx) ==
         SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION) &&
      (Sd_GetMulticastThresholdOfEventHandler(eventHandlerIdx, PartitionIdx) >
         Sd_GetNrOfClientsOfEventHandlerDyn(eventHandlerIdx, PartitionIdx)))
    {
      Sd_StateServer_EventHandlerSwitchUnicast(eventHandlerIdx, PartitionIdx);
    }

    /* #500 Check if the event handler is still requested. */
    Sd_StateServer_EventHandlerCheckSubscribed(eventHandlerIdx, PartitionIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_StateServer_UpdateEventHandlerState()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_UpdateEventHandlerState(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the event handler is processed for the first time. */
  if(Sd_GetStateMachineOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) == SD_EVENT_HANDLER_STATE_NONE)
  {
    /* #100 First run of the state machine. Update the event handler state based on the server state. */
    if(Sd_GetStateSwcOfServerDyn(serverIdx, PartitionIdx) == SD_SERVER_SERVICE_DOWN)
    {
      Sd_UtilServer_UpdateEventHandlerStateMachine(EventHandlerIdx, PartitionIdx,
        SD_EVENT_HANDLER_STATE_SERVICE_DOWN);
    }
    else
    {
      Sd_UtilServer_UpdateEventHandlerStateMachine(EventHandlerIdx, PartitionIdx,
        SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED);
    }
  }
  /* #20 Otherwise, check if the corresponding server was switched to down state. */
  else if(Sd_GetStateMachineOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) == SD_EVENT_HANDLER_STATE_SERVICE_DOWN)
  {
    /* #200 Set event hander to down if it is currently available. */
    if(Sd_GetStateSwcOfServerDyn(serverIdx, PartitionIdx) == SD_SERVER_SERVICE_AVAILABLE)
    {
      Sd_UtilServer_UpdateEventHandlerStateMachine(EventHandlerIdx, PartitionIdx,
        SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED);
    }
  }
  /* #30 Otherwise, nothing to do. */
  else
  {
    /* All other states are handled within the context of a client. */
  }
}

/**********************************************************************************************************************
 *  Sd_StateServer_UpdateEventHandlerClients()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_UpdateEventHandlerClients(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx =
    Sd_GetEventHandlerMulticastIdxOfEventHandler(EventHandlerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all registered clients of an eventgroup. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
    clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
    clientListIdx++)
  {
    if(Sd_GetClientStateOfClientList(clientListIdx, PartitionIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED)
    {
      /* #100 Check if the client is still subscribed. */
      if(TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx,
           Sd_GetSubscribedUntilOfClientList(clientListIdx, PartitionIdx), FALSE))
      {
        /* #1000 The client has unsubscribed or a timeout occurred. Unconfigure and remove the client. */
        Sd_UtilServer_RemoveClientFromList(EventHandlerIdx, PartitionIdx, clientListIdx);

        /* #1001 Check if the client was subscribed for a multicast eventgroup and was the last subscribed client. */
        if(Sd_EventHandlerUsesMulticast(EventHandlerIdx, PartitionIdx) &&
            (Sd_GetNrOfClientsOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) == 0u))
        {
          /* #10010 Disable the multicast routing of the event handler. */
          (void)Sd_Util_DisableSpecificRouting(
            (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerMulticast(
              eventHandlerMulticastIdx, PartitionIdx),
            PartitionIdx,
            Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx, PartitionIdx));
        }
      }

      /* #200 Update the timer for the next state machine run. */
      Sd_Util_SetNextStateMachineRun(Sd_GetInstanceIdxOfServer(serverIdx, PartitionIdx), PartitionIdx,
        Sd_GetSubscribedUntilOfClientList(clientListIdx, PartitionIdx));
    }
  }
}

/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerSwitchMulticast()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerSwitchMulticast(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx =
    Sd_GetEventHandlerMulticastIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
  SoAd_SoConIdType assignedSoConId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enable the routing for the multicast socket connection. */
  (void)Sd_Util_EnableSpecificRouting(
    (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerMulticast
      (eventHandlerMulticastIdx, PartitionIdx),
    PartitionIdx,
    Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx, PartitionIdx));

  /* #20 Iterate over all registered clients of an eventgroup in order to disable the unicast routing groups. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx++)
  {
    if(Sd_GetClientStateOfClientList(clientListIdx, PartitionIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED)
    {
      assignedSoConId = Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx, PartitionIdx);

      if((assignedSoConId != SOAD_INV_SO_CON_ID) &&
        (Sd_GetMulticastThresholdOfEventHandler(EventHandlerIdx, PartitionIdx) != 1u))
      {
        Sd_UtilServer_ResetActivationCount(EventHandlerIdx, PartitionIdx, assignedSoConId);

        /* #200 Disable the routing for the unicast socket connection. */
        (void)Sd_Util_DisableSpecificRouting(
          (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerUdp(
            Sd_GetEventHandlerUdpIdxOfEventHandler(EventHandlerIdx, PartitionIdx), PartitionIdx),
          PartitionIdx,
          assignedSoConId);
      }
    }
  }

  /* #30 Update the state. */
  Sd_UtilServer_UpdateEventHandlerStateMachine(EventHandlerIdx, PartitionIdx,
    SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION);
}

/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerSwitchUnicast()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerSwitchUnicast(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx =
    Sd_GetEventHandlerMulticastIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
  SoAd_SoConIdType assignedSoConId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all registered clients of an eventgroup in order to enable the unicast routing groups. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx++)
  {
    if(Sd_GetClientStateOfClientList(clientListIdx, PartitionIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED)
    {
      assignedSoConId = Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx, PartitionIdx);

      /* #100 Enable the unicast routing group and increase the activation counter. */
      (void)Sd_Util_EnableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerUdp(Sd_GetEventHandlerUdpIdxOfEventHandler(
          EventHandlerIdx, PartitionIdx), PartitionIdx),
        PartitionIdx,
        assignedSoConId);
      Sd_UtilServer_IncreaseActivationCount(EventHandlerIdx, PartitionIdx, assignedSoConId);
    }
  }

  /* #20 Disable the routing for the multicast socket connection. */
  (void)Sd_Util_DisableSpecificRouting(
    (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerMulticast(
      eventHandlerMulticastIdx, PartitionIdx),
    PartitionIdx,
    Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx, PartitionIdx));

  /* #30 Update the state. */
  Sd_UtilServer_UpdateEventHandlerStateMachine(EventHandlerIdx, PartitionIdx,
    SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION);
}

/**********************************************************************************************************************
 *  Sd_StateServer_EventHandlerCheckSubscribed()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_EventHandlerCheckSubscribed(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the event handler has no subscribed clients, but is still in a subscribed state. */
  if((Sd_GetNrOfClientsOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) == 0u) &&
    ((Sd_GetStateMachineOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) ==
       SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION) ||
    (Sd_GetStateMachineOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) ==
       SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION)))
  {
    /* #100 Check if the event handler was requested. */
    if(Sd_GetRequestStateOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) == SD_EVENT_HANDLER_REQUESTED)
    {
      /* #1000 Set the state of the event handler to released and inform the BswM. */
      Sd_UtilServer_UpdateEventHandlerRequestState(EventHandlerIdx, PartitionIdx, SD_EVENT_HANDLER_RELEASED);
    }

    /* #101 Disable the multicast socket connection routing. */
    if(Sd_GetStateMachineOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) ==
         SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION)
    {
      eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      (void)Sd_Util_DisableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerMulticast(
          Sd_GetEventHandlerMulticastIdxOfEventHandler(EventHandlerIdx, PartitionIdx), PartitionIdx),
        PartitionIdx,
        Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx, PartitionIdx));
    }

    /* #102 Disable routing of UDP and TCP routing groups and reset activation count. */
    Sd_UtilServer_DisableRouting(EventHandlerIdx, PartitionIdx);

    /* #103 Update the state of the event handler. */
    Sd_UtilServer_UpdateEventHandlerStateMachine(EventHandlerIdx, PartitionIdx,
      SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED);
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_DisableRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_DisableRouting(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerTcpType eventHandlerTcpIdx;
  Sd_SizeOfEventHandlerUdpType eventHandlerUdpIdx;
  Sd_SoConActivationMapIterType soConActivationMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Disable routing of TCP event handler. */
  eventHandlerTcpIdx = Sd_GetEventHandlerTcpIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
  if(eventHandlerTcpIdx != SD_NO_EVENTHANDLERTCPIDXOFEVENTHANDLER)
  {
    (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerTcp(
      eventHandlerTcpIdx, PartitionIdx), PartitionIdx);
  }

  /* #20 Disable routing of UDP event handler. */
  eventHandlerUdpIdx = Sd_GetEventHandlerUdpIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
  if(eventHandlerUdpIdx != SD_NO_EVENTHANDLERUDPIDXOFEVENTHANDLER)
  {
    (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerUdp(
      eventHandlerUdpIdx, PartitionIdx), PartitionIdx);
  }

  /* #30 Reset the number of activations. */
  for(soConActivationMapIdx = Sd_GetSoConActivationMapStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      soConActivationMapIdx < Sd_GetSoConActivationMapEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      soConActivationMapIdx++)
  {
    Sd_SetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, 0u, PartitionIdx);                                 /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
  }
}

#if(SD_GET_RESET_MEASUREMENT_DATA_API == STD_ON)
/**********************************************************************************************************************
 *  Sd_Measure_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Measure_Init(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the measurement counters to zero. */
  SD_BEGIN_CRITICAL_SECTION_MEASUREMENT();
  Sd_Measure_InvalidMessageCount = 0u;
  Sd_Measure_SubscribeNackCount = 0u;
  SD_END_CRITICAL_SECTION_MEASUREMENT();
}

/**********************************************************************************************************************
 *  Sd_Measure_GetAndReset()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Measure_GetAndReset(
  Sd_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SD_APPL_VAR) MeasurementDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check requested measurement index. */
  switch (MeasurementIdx)
  {
    case SD_MEAS_INVALID_SD_MESSAGE:
    {
      /* #100 Copy value of InvalidMessageCount to out pointer and reset the counter if requested. */
      SD_BEGIN_CRITICAL_SECTION_MEASUREMENT();
      if (MeasurementDataPtr != NULL_PTR)
      {
        *MeasurementDataPtr = Sd_Measure_InvalidMessageCount;                                                           /* SBSW_SD_POINTER_WRITE */
      }
      if (MeasurementResetNeeded == TRUE)
      {
        Sd_Measure_InvalidMessageCount = 0u;
      }
      SD_END_CRITICAL_SECTION_MEASUREMENT();
      retVal = E_OK;
      break;
    }

    case SD_MEAS_SUBSCRIBE_NACK:
    {
      /* #101 Copy value of SubscribeNackCount to out pointer and reset the counter if requested. */
      SD_BEGIN_CRITICAL_SECTION_MEASUREMENT();
      if (MeasurementDataPtr != NULL_PTR)
      {
        *MeasurementDataPtr = Sd_Measure_SubscribeNackCount;                                                            /* SBSW_SD_POINTER_WRITE */
      }
      if (MeasurementResetNeeded == TRUE)
      {
        Sd_Measure_SubscribeNackCount = 0u;
      }
      SD_END_CRITICAL_SECTION_MEASUREMENT();
      retVal = E_OK;
      break;
    }

    case SD_MEAS_ALL:
    {
      /* #102 For SD_MEAS_ALL, no measurement data can be retrieved, but all measurement data can be reset. */
      if (MeasurementResetNeeded == TRUE)
      {
        Sd_Measure_Init();
      }

      retVal = E_OK;
      break;
    }

    default:
    {
      /* #103 Return E_NOT_OK if the requested measurement index is not supported. */
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Measure_Inc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Measure_Inc(
  Sd_MeasurementIdxType MeasurementIdx,
  uint16 Count)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Increment the counter specified by MeasurementIdx. */
  switch (MeasurementIdx)
  {
  case SD_MEAS_INVALID_SD_MESSAGE:
  {
    Sd_Measure_IncCounterValue(&Sd_Measure_InvalidMessageCount, Count);                                               /* SBSW_SD_VARIABLE_POINTER_FORWARD */
    break;
  }

  case SD_MEAS_SUBSCRIBE_NACK:
  {
    Sd_Measure_IncCounterValue(&Sd_Measure_SubscribeNackCount, Count);                                                /* SBSW_SD_VARIABLE_POINTER_FORWARD */
    break;
  }

  default:
  {
    /* Nothing to do. */
    break;
  }
  }
}

/**********************************************************************************************************************
 *  Sd_Measure_IncCounterValue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Measure_IncCounterValue(
  P2VAR(uint32, AUTOMATIC, SD_APPL_VAR) CounterPtr,
  uint16 Count)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prevent inconsistent reads and writes on the counter. */
  SD_BEGIN_CRITICAL_SECTION_MEASUREMENT();

  /* #20 Increment the counter while observing the maximum value. */
  if (((*CounterPtr) + Count) < SD_MEAS_COUNT_MAX_VALUE)
  {
    *CounterPtr = ((*CounterPtr) + Count);                                                                              /* SBSW_SD_POINTER_WRITE */
  }
  else
  {
    *CounterPtr = SD_MEAS_COUNT_MAX_VALUE;                                                                              /* SBSW_SD_POINTER_WRITE */
  }

  SD_END_CRITICAL_SECTION_MEASUREMENT();
}
#endif /* SD_GET_RESET_MEASUREMENT_DATA_API */

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntry()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntry(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfServerType serverIdx;
  uint16 serverStartIdx, serverEndIdx;
  Sd_EntryType rxEntryType;
  uint16 rxServiceId;
  uint16 rxInstanceId;
  uint8 rxMajorVersion;
  uint16 rxReservedField;
  uint32 rxMinorVersion;
  uint16 rxEventGroupId;
  boolean foundService = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the header fields of the entry. */
  SD_GET_ENTRY_TYPE(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, rxEntryType);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_SERVICEID_OFFSET, rxServiceId);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INSTANCEID_OFFSET, rxInstanceId);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_MAJORVERSION_OFFSET, rxMajorVersion);

  /* #20 Get all services with the corresponding ServiceId */
  serverStartIdx = (uint16)Sd_GetServerStartIdxOfInstance(InstanceIdx, PartitionIdx);
  serverEndIdx = (uint16)Sd_GetServerEndIdxOfInstance(InstanceIdx, PartitionIdx);

  /* #30 Check if the ServiceId is wildcard. */
  if(rxServiceId != SD_ENTRY_WILDCARD_SERVICE_ID)
  {
    /* #300 ServiceId is no wildcard. Search for the matching server(s). */
    Sd_Util_GetServices(PartitionIdx, rxServiceId, Sd_Util_GetServerServiceId, &serverStartIdx, &serverEndIdx);         /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }
  else if(rxEntryType != SD_ENTRY_FIND_SERVICE)
  {
    /* #310 Wildcard ServiceId is only allowed for FindService messages. Do not process entry. */
    serverEndIdx = serverStartIdx;
  }
  else
  {
    /* #320 Received FindService entry with wildcard ServiceId. Process all servers of the instance. */
  }

  /* #40 Differentiate between entry types. */
  if(rxEntryType == SD_ENTRY_FIND_SERVICE)
  {
    /* #400 For FindService entries: Iterate over all matching servers to find the one corresponding to the entry. */
    for(serverIdx = (Sd_SizeOfServerType)serverStartIdx; serverIdx < (Sd_SizeOfServerType)serverEndIdx; serverIdx++)
    {
      SD_GET_UINT32(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY1_MINORVERSION_OFFSET, rxMinorVersion);

      /* #4000 Check if the InstanceId, MajorVersion and MinorVersion matches.
       *       Wildcards are allowed for FindService entries (format type 1). */
      if(((rxInstanceId == Sd_GetInstanceIdOfServerDyn(serverIdx, PartitionIdx)) ||
            (rxInstanceId == SD_ENTRY_WILDCARD_INSTANCE_ID)) &&
         ((rxMajorVersion == Sd_GetMajorVersionOfServer(serverIdx, PartitionIdx)) ||
            (rxMajorVersion == SD_ENTRY_WILDCARD_MAJOR_VERSION)) &&
         ((rxMinorVersion == Sd_GetMinorVersionOfServer(serverIdx, PartitionIdx)) ||
            (rxMinorVersion == SD_ENTRY_WILDCARD_MINOR_VERSION)))
      {
        /* #40000 Process the received FindService message. */
        Sd_RxServer_ProcessEntryFind(serverIdx, PartitionIdx, IndexOfEntry, RxMessageInfoPtr);                           /* SBSW_SD_PASSING_API_POINTER */
      }
    }
  }
  else
  {
    /* #410 For (Stop)SubscribeEventgroup entries: Iterate over all matching servers to find the one corresponding to the entry. */
    for(serverIdx = (Sd_SizeOfServerType)serverStartIdx; serverIdx < (Sd_SizeOfServerType)serverEndIdx; serverIdx++)
    {
      /* #4100 Check if the InstanceId, MajorVersion and MinorVersion matches.
       *       Wildcards are not allowed for (Stop)SubscribeEventgroup entries (format type 2). */
      if((rxInstanceId == Sd_GetInstanceIdOfServerDyn(serverIdx, PartitionIdx)) &&
         (rxMajorVersion == Sd_GetMajorVersionOfServer(serverIdx, PartitionIdx)))
      {
        /* #41000 Found the corresponding service instance. */
        foundService = TRUE;

        /* #41001 Process the received SubscribeEventgroup message. */
        Sd_RxServer_ProcessEntrySubscribe(serverIdx, PartitionIdx, IndexOfEntry, RxMessageInfoPtr);                     /* SBSW_SD_PASSING_API_POINTER */

        if(rxServiceId != SD_SERVICE_ID_OTHERSERV)
        {
          break;
        }
      }
    }

    /* #420 Send a negative acknowledgement if the ServiceId, MajorVersion or InstanceId does not match any
     *      ServerService of the SdInstance and if the entry is not a StopSubscribeEventgroup entry. */
    if((Sd_RxServer_EntryIsStopSubscribe(IndexOfEntry, RxMessageInfoPtr) == FALSE) && (foundService == FALSE))          /* SBSW_SD_PASSING_API_POINTER */
    {
      SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, rxReservedField);
      SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

      Sd_Util_AddEventGroupNackEntryToList(InstanceIdx, PartitionIdx, Sd_GetSizeOfServer(PartitionIdx), rxServiceId,
        rxInstanceId, rxMajorVersion, rxReservedField, rxEventGroupId, RxMessageInfoPtr->AddressStorageIdx,
        Sd_GetSizeOfServerTimer(PartitionIdx));
    }
  }
}

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntryFind()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntryFind(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 2981 1 */ /* MD_Sd_Rule-2.2_2981 */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_NOT_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);
  Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx, PartitionIdx);
  Sd_ConfigOptionStringType rxConfigOptionPtrArray[Sd_GetMaxNrOptionsPerEntry(PartitionIdx) + 1u];
  Sd_AttachedOptionsType attachedOptions = { 0 };
  uint32 rxTTL;
  uint32 randDelay = 0u;
  uint8 index1stOption, index2ndOption;
  uint8 nr1stOptions, nr2ndOptions;

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0314 1 */ /* MD_Sd_Dir-1.1_0314 */
  IpBase_Fill((SD_P2VAR(void))rxConfigOptionPtrArray, 0u, sizeof(rxConfigOptionPtrArray));                              /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_IPBASE_FILL */
  attachedOptions.EndpointAddrUdp.family = SOAD_AF_INVALID;
  attachedOptions.EndpointAddrTcp.family = SOAD_AF_INVALID;
  attachedOptions.ConfigOptionPtrArrayPtr = rxConfigOptionPtrArray;

  /* #10 Read header of FindService entry. */
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX1_OFFSET, index1stOption);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX2_OFFSET, index2ndOption);
  SD_GET_NROFOPT(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, nr1stOptions, nr2ndOptions);
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);

  /* #20 Check if the FindService messages has to be ignored. */
  if((Sd_GetStateSwcOfServerDyn(ServerIdx, PartitionIdx) == SD_SERVER_SERVICE_DOWN) ||
    (Sd_GetStateMachineOfServerDyn(ServerIdx, PartitionIdx) == SD_SERVER_STATE_READY_INITIAL_WAIT)||
    (rxTTL == 0u))
  {
    /* #200 Ignore FindService messages if the server service is not available, during the initial wait phase, or with a TTL == 0 */
  }
  /* #30 Otherwise, process the FindService.*/
  else
  {
    /* #300 Process the corresponding option array(s).
     *      Configuration options are allowed, endpoint options shall be ignored. */

    /* #301 Check if a first option run is available.*/
    if(nr1stOptions != 0u)
    {
      /* #3010 Process first option run. */
      optionsRetVal = Sd_RxServer_ProcessOptionRun(PartitionIdx, RxMessageInfoPtr->PduInfoPtr,
        index1stOption, nr1stOptions, Sd_GetSizeOfEventHandler(PartitionIdx), &attachedOptions);                        /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    }

    /* #400 Check if a second option run is available.*/
    if((nr2ndOptions != 0u) && ((nr1stOptions == 0u) || (optionsRetVal == SD_OPTIONS_OK)))
    {
      /* #4000 Process second option run. */
      optionsRetVal = Sd_RxServer_ProcessOptionRun(PartitionIdx, RxMessageInfoPtr->PduInfoPtr,
        index2ndOption, nr2ndOptions, Sd_GetSizeOfEventHandler(PartitionIdx), &attachedOptions);                        /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    }
    /* #500 Otherwise, check if both option runs are empty.*/
    else if((nr1stOptions == 0u) && (nr2ndOptions == 0u)) /* PRQA S 2995 */ /* MD_Sd_FalsePositive_2995_2996_2853 */
    {
      /* #5000 Find messages without any attached option are also allowed. */
      optionsRetVal = SD_OPTIONS_OK;
    }
    /* #600 Otherwise, nothing to process.*/
    else
    {
      /* #6000 Entry does not contain a second option run. Keep result of first option run. */
    }

    /* #700 Process the find entry. */
    if((optionsRetVal == SD_OPTIONS_OK) &&
       (Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) == SD_INSTANCE_UP_AND_CONFIGURED))
    {
      /* #7000 Check the attached configuration options by a user callout. */
      optionsRetVal = Sd_RxServer_CheckConfigOptionByCbk(
        ServerIdx, PartitionIdx,
        (RxMessageInfoPtr->MulticastMessage == TRUE) ? Sd_GetMulticastRxPduIdOfInstance(instanceIdx, PartitionIdx) :
          Sd_GetUnicastRxPduIdOfInstance(instanceIdx, PartitionIdx),
        SD_ENTRY_FIND_SERVICE,
        rxConfigOptionPtrArray);                                                                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */

      if(optionsRetVal == SD_OPTIONS_OK)
      {
        /* #70000 Check if the FindService message was transmitted by multicast or unicast. */
        if(RxMessageInfoPtr->MulticastMessage == TRUE)
        {
          /* Add a random delay to the response message. */
          randDelay = Sd_Util_GetRandomNrInRangeForResponse(
            Sd_GetRequestResponseMinDelayOfServerTimer(serverTimerIdx, PartitionIdx),
            Sd_GetRequestResponseMaxDelayOfServerTimer(serverTimerIdx, PartitionIdx), TRUE);
        }

        /* #70001 Add a unicast OfferService message to the list of sendentries. */
        Sd_Util_AddUnicastOfferEntryToList(ServerIdx, PartitionIdx, RxMessageInfoPtr->AddressStorageIdx, randDelay);
      }
    }
  }
}

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribe()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxServer_ProcessEntrySubscribe(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  Sd_ConfigOptionStringType rxConfigOptionPtrArray[Sd_GetMaxNrOptionsPerEntry(PartitionIdx) + 1u];
  Sd_AttachedOptionsType attachedOptions = { 0 };
  uint16 rxServiceId, rxInstanceId;
  uint16 rxReservedField;
  uint16 rxEventGroupId;
  uint8 rxMajorVersion;

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0314 1 */ /* MD_Sd_Dir-1.1_0314 */
  IpBase_Fill((SD_P2VAR(void))rxConfigOptionPtrArray, 0u, sizeof(rxConfigOptionPtrArray));                              /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_IPBASE_FILL */
  attachedOptions.EndpointAddrUdp.family = SOAD_AF_INVALID;
  attachedOptions.EndpointAddrTcp.family = SOAD_AF_INVALID;
  attachedOptions.ConfigOptionPtrArrayPtr = rxConfigOptionPtrArray;

  /* #10 Search for the matching event handler to process the SubscribeEventgroup message. */
  eventHandlerIdx = Sd_RxServer_SearchEventHandler(ServerIdx, PartitionIdx, IndexOfEntry, RxMessageInfoPtr);            /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Check if a matching event handler was found and the state of the corresponding server is available. */
  if((eventHandlerIdx < Sd_GetSizeOfEventHandler(PartitionIdx)) &&
     (Sd_GetStateSwcOfServerDyn(ServerIdx, PartitionIdx) == SD_SERVER_SERVICE_AVAILABLE))
  {
    /* #200 Process the attached option array. */
    optionsRetVal = Sd_RxServer_ProcessOptionArray(eventHandlerIdx, PartitionIdx, RxMessageInfoPtr->PduInfoPtr,
      IndexOfEntry, &attachedOptions);                                                                                  /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #201 Check if the options array is valid and all required endpoint options are present. */
    if(optionsRetVal == SD_OPTIONS_OK)
    {
      /* #2010 Check the attached configuration options by a user callout. */
      optionsRetVal = Sd_RxServer_CheckConfigOptionByCbk(
        ServerIdx, PartitionIdx,
        (RxMessageInfoPtr->MulticastMessage == TRUE) ? Sd_GetMulticastRxPduIdOfInstance(instanceIdx, PartitionIdx) :
          Sd_GetUnicastRxPduIdOfInstance(instanceIdx, PartitionIdx),
        SD_ENTRY_SUBSCRIBE_EVENTGROUP,
        rxConfigOptionPtrArray);                                                                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */

      if(optionsRetVal == SD_OPTIONS_OK)
      {
        /* #20100 Save all required elements of the subscription. */
        retVal = Sd_RxServer_ProcessEntrySubscribeInternal(eventHandlerIdx, PartitionIdx, IndexOfEntry,
          RxMessageInfoPtr, &attachedOptions);                                                                          /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }
    }
  }
  /* #30 Otherwise the service instance is not ready or there is no matching event handler. */
  else
  {
    /* #300 Leave retVal at value E_NOT_OK. This will trigger a transmission of a SubscribeEventgroupNack message. */
  }

  /* #40 Send a NACK if we were not able to process the SUBSCRIBE, if no endpoint option was attached or if the
   *     eventgroup was not configured. Entries from another subnet and StopSubscribeEventgroup entries are ignored. */
  /* PRQA S 2996 2 */ /* MD_Sd_FalsePositive_2995_2996_2853 */
  if((Sd_RxServer_EntryIsStopSubscribe(IndexOfEntry, RxMessageInfoPtr) == FALSE) &&                                     /* SBSW_SD_PASSING_API_POINTER */
    ((retVal != E_OK) || (eventHandlerIdx == Sd_GetSizeOfEventHandler(PartitionIdx))) &&
    (optionsRetVal != SD_OPTIONS_WRONG_SUBNET))
  {
    Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetSizeOfServerTimer(PartitionIdx);

    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_SERVICEID_OFFSET, rxServiceId);
    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INSTANCEID_OFFSET, rxInstanceId);
    SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_MAJORVERSION_OFFSET, rxMajorVersion);
    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, rxReservedField);
    SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

    if(RxMessageInfoPtr->MulticastMessage == TRUE)
    {
      /* #400 Add additional delay to NACK message. */
      if(eventHandlerIdx < Sd_GetSizeOfEventHandler(PartitionIdx))
      {
        serverTimerIdx = Sd_GetServerTimerIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
      }
      else
      {
        serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx, PartitionIdx);
      }
    }

    Sd_Util_AddEventGroupNackEntryToList(instanceIdx, PartitionIdx, ServerIdx, rxServiceId, rxInstanceId, rxMajorVersion,
      rxReservedField, rxEventGroupId, RxMessageInfoPtr->AddressStorageIdx, serverTimerIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_RxServer_EntryIsStopSubscribe()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_RxServer_EntryIsStopSubscribe(
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 rxTTL;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the TTL of the received (Stop)SubscribeEventgroup message. */
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);

  /* #20 Return true if TTL is zero. */
  return (boolean)(rxTTL == 0u);
}

/**********************************************************************************************************************
 *  Sd_RxServer_SearchEventHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Sd_SizeOfEventHandlerType, SD_CODE) Sd_RxServer_SearchEventHandler(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  uint16 rxEventGroupId;
  boolean foundEventHandler = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the eventgroup ID from the SD header. */
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

  /* #20 Iterate trough each event handler to find the corresponding one. */
  for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx, PartitionIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx, PartitionIdx);
      eventHandlerIdx++)
  {
    if(Sd_GetEventGroupIdOfEventHandler(eventHandlerIdx, PartitionIdx) == rxEventGroupId)
    {
      /* #200 Found the corresponding event handler. Stop searching. */
      foundEventHandler = TRUE;
      break;
    }
  }

  /* #30 Set the eventHandlerIdx to an invalid value if no matching event handler was found. */
  if(foundEventHandler == FALSE)
  {
    eventHandlerIdx = Sd_GetSizeOfEventHandler(PartitionIdx);
  }

  return eventHandlerIdx;
}

/**********************************************************************************************************************
 *  Sd_RxServer_CheckConfigOptionByCbk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxServer_CheckConfigOptionByCbk(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  PduIdType RxPduId,
  Sd_EntryType RxEntryType,
  SD_P2CONST(Sd_ConfigOptionStringType) RxConfigOptionPtrArrayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the attached configuration options by a user callout. */
  if(Sd_GetCapabilityRecordFunctionCalloutPtrIdxOfServer(ServerIdx, PartitionIdx) !=
       SD_NO_CAPABILITYRECORDFUNCTIONCALLOUTPTRIDXOFSERVER)
  {
    Sd_ConfigOptionStringType configOptionPtrArray[3u] = { NULL_PTR, NULL_PTR, NULL_PTR };

    /* #100 Add configuration options of hostname and capability records to the configuration option pointer array. */
    Sd_Rx_FillConfigOptionPtrArray(configOptionPtrArray,
      Sd_GetCapabilityRecordStartIdxOfInstance(Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx), PartitionIdx),
      Sd_GetCapabilityRecordStartIdxOfServer(ServerIdx, PartitionIdx),
      SD_NO_CAPABILITYRECORDSTARTIDXOFSERVER);                                                                          /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Call application to verify configuration options of received message. */
    if(TRUE != Sd_GetCapabilityRecordFunctionCalloutPtr(
        Sd_GetCapabilityRecordFunctionCalloutPtrIdxOfServer(ServerIdx, PartitionIdx))(
      RxPduId,
      (uint8)RxEntryType,
      Sd_GetServiceIdOfServer(ServerIdx, PartitionIdx),
      Sd_GetInstanceIdOfServerDyn(ServerIdx, PartitionIdx),
      Sd_GetMajorVersionOfServer(ServerIdx, PartitionIdx),
      Sd_GetMinorVersionOfServer(ServerIdx, PartitionIdx),
      RxConfigOptionPtrArrayPtr,
      configOptionPtrArray))                                                                                            /* SBSW_SD_FUNCTION_PTR_CALL */
    {
      optionsRetVal = SD_OPTIONS_NOT_OK;
    }
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionArray()
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
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxServer_ProcessOptionArray(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint16 IndexOfEntry,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_NOT_OK;
  uint8 index1stOption, index2ndOption;
  uint8 nr1stOptions, nr2ndOptions;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the referenced options from the entry. */
  SD_GET_UINT8(PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX1_OFFSET, index1stOption);
  SD_GET_UINT8(PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX2_OFFSET, index2ndOption);
  SD_GET_NROFOPT(PduInfoPtr, IndexOfEntry, nr1stOptions, nr2ndOptions);

  /* #20 Process the corresponding option array(s) and read the data of the attached endpoints. */

  /* #30 Check if a first option run is available.*/
  if(nr1stOptions != 0u)
  {
    /* #300 Process first option run. */
    optionsRetVal = Sd_RxServer_ProcessOptionRun(PartitionIdx, PduInfoPtr, index1stOption, nr1stOptions,
      EventHandlerIdx, AttachedOptionsPtr);                                                                             /* SBSW_SD_PASSING_API_POINTER */
  }

  /* #40 Check if a second option run is available.*/
  if((nr2ndOptions != 0u) && ((nr1stOptions == 0u) || (optionsRetVal == SD_OPTIONS_OK)))
  {
    /* #400 Process second option run. */
    optionsRetVal = Sd_RxServer_ProcessOptionRun(PartitionIdx, PduInfoPtr, index2ndOption, nr2ndOptions,
      EventHandlerIdx, AttachedOptionsPtr);                                                                             /* SBSW_SD_PASSING_API_POINTER */
  }
  /* #50 Otherwise, check if both option runs are empty.*/
  else if((nr1stOptions == 0u) && (nr2ndOptions == 0u)) /* PRQA S 2995 */ /* MD_Sd_FalsePositive_2995_2996_2853 */
  {
    if(Sd_EventHandlerUsesMulticast(EventHandlerIdx, PartitionIdx))
    {
      /* #500 If the EventGroup is configured for multicast, allow the subscription without any attached endpoint option. */
      optionsRetVal = SD_OPTIONS_OK;
    }
  }
  /* #60 Otherwise, nothing to process.*/
  else
  {
    /* #600 Entry does not contain a second option run. Keep result of first run. */
  }

  /* #70 Check if all required endpoint options are present. */
  if((optionsRetVal == SD_OPTIONS_OK) &&
    ((Sd_EventHandlerUsesUdp(EventHandlerIdx, PartitionIdx) &&
       (AttachedOptionsPtr->EndpointAddrUdp.family == SOAD_AF_INVALID)) ||
     (Sd_EventHandlerUsesTcp(EventHandlerIdx, PartitionIdx) &&
       (AttachedOptionsPtr->EndpointAddrTcp.family == SOAD_AF_INVALID))))
  {
    /* #700 Not all required endpoint options are present. Reject message. */
    optionsRetVal = SD_OPTIONS_NOT_OK;
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionRun()
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
/* PRQA S 6060 1 */ /* MD_Sd_STPAR_6060 */
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxServer_ProcessOptionRun(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  uint8 NrOfOptions,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;
  uint8 currentOptionIndex = IndexOfOption;
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Stop processing of the message if the header indexes more options than attached
   *     or if the size of the options array is exceeded. */
  if(Sd_GetNrOfOptionsOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx) <
      ((Sd_NrOfOptionsOfPartitionDataDynType)IndexOfOption + (Sd_NrOfOptionsOfPartitionDataDynType)NrOfOptions))
  {
    optionsRetVal = SD_OPTIONS_NOT_OK;
  }

  /* #20 Process consecutive options. */
  while((optionsRetVal == SD_OPTIONS_OK) && ((IndexOfOption + NrOfOptions) > currentOptionIndex))
  {
    /* #200 Read the option information. */
    SD_GET_OPTION_TYPE(PduInfoPtr, Sd_GetOptionIndex(currentOptionIndex, PartitionIdx), optionType);

    /* #201 Differentiate between service- and eventgroup entry. */
    if(EventHandlerIdx == Sd_GetSizeOfEventHandler(PartitionIdx))
    {
      /* #2010 Received FindService message. */

      if(optionType == SD_OPTION_CONFIGURATION)
      {
        /* #20100 Check if configuration option is valid. */
        optionsRetVal = Sd_Rx_CheckConfigOption(PartitionIdx, PduInfoPtr, currentOptionIndex,
          AttachedOptionsPtr->ConfigOptionPtrArrayPtr);                                                                 /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_ATTACHED_OPTIONS_PTR */
      }
      else
      {
        /* #20101 Endpoint or unknown options are not allowed for FindService entries and shall be ignored. */
      }
    }
    else
    {
      /* #2011 Received (Stop)SubscribeEventgroup message. Trigger processing. */
      optionsRetVal = Sd_RxServer_ProcessOptionSubscribe(EventHandlerIdx, PartitionIdx, PduInfoPtr,
        currentOptionIndex, AttachedOptionsPtr);                                                                        /* SBSW_SD_PASSING_API_POINTER */
    }

    currentOptionIndex++;
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessOptionSubscribe()
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
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxServer_ProcessOptionSubscribe(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the option information. */
  SD_GET_OPTION_TYPE(PduInfoPtr, Sd_GetOptionIndex(IndexOfOption, PartitionIdx), optionType);

  /* #20 Differentiate between configuration option or endpoint option. */
  if(optionType == SD_OPTION_CONFIGURATION)
  {
    /* #200 Configuration option: Check if valid. */
    optionsRetVal = Sd_Rx_CheckConfigOption(PartitionIdx, PduInfoPtr, IndexOfOption,
      AttachedOptionsPtr->ConfigOptionPtrArrayPtr);                                                                     /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_ATTACHED_OPTIONS_PTR */
  }
  else
  {
    Sd_L4ProtocolType optionL4Proto; /* Set by Sd_Rx_GetEndpointOptionParameters() */
    uint16 addrType; /* Set by Sd_Rx_GetEndpointOptionParameters() */

    /* #201 Endpoint option: Read the type values. */
    Sd_Rx_GetEndpointOptionParameters(PartitionIdx, PduInfoPtr, IndexOfOption, SD_MCTYPE_ANY, &addrType, &optionL4Proto); /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #202 Check if a valid endpoint option was attached. */
    if(addrType != SOAD_AF_INVALID)
    {
      /* #2020 Read the attached endpoint option. */
      optionsRetVal = Sd_Rx_ReadEndpointOptions(PartitionIdx, PduInfoPtr, IndexOfOption, addrType, optionL4Proto,
        AttachedOptionsPtr);                                                                                            /* SBSW_SD_PASSING_API_POINTER */

      if(optionsRetVal == SD_OPTIONS_OK)
      {
        boolean checkUdpEp = FALSE;
        Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx, PartitionIdx);

        /* If Unicast UDP endpoint option, check subnet. */
        if((optionL4Proto == SD_L4PROTOCOL_UDP) && ((optionType & SD_OPTION_MULTICAST_MASK) != SD_OPTION_MULTICAST_MASK)) /*lint !e655 */
        {
          checkUdpEp = Sd_IsEventHandlerUdpUsedOfEventHandler(EventHandlerIdx, PartitionIdx);
        }

        /* #20200 Check if the attached endpoint addresses are part of the subnet. */
        optionsRetVal = Sd_Rx_CheckSubnets(Sd_GetInstanceIdxOfServer(serverIdx, PartitionIdx), PartitionIdx, checkUdpEp,
          Sd_IsEventHandlerTcpUsedOfEventHandler(EventHandlerIdx, PartitionIdx) ? TRUE : FALSE,
          AttachedOptionsPtr);                                                                                          /* SBSW_SD_PASSING_API_POINTER */
      }
    }
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribeInternal()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessEntrySubscribeInternal(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfClientListType clientListIdx;
  boolean entryIsStopSubscribe;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the TTL of the received SubscribeEventgroup message to identify a StopSubscribeEventgroup entry. */
  entryIsStopSubscribe = Sd_RxServer_EntryIsStopSubscribe(IndexOfEntry, RxMessageInfoPtr);                              /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Search the client in the list of subscribed clients, or create a new entry if not found.
   *     An unknown client will not be added to the list if it transmitted a StopSubscribeEventgroup entry. */
  clientListIdx = Sd_RxServer_FindClientInList(EventHandlerIdx, PartitionIdx, RxMessageInfoPtr->AddressStorageIdx, AttachedOptionsPtr,
    (entryIsStopSubscribe == TRUE) ? FALSE : TRUE);                                                                     /* SBSW_SD_PASSING_API_POINTER */

  if(clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx))
  {
    /* #200 If the client is valid, save all parameters and endpoints.
     *      The processing will be done in the next main function cycle. */
    retVal = Sd_RxServer_ProcessEntrySubscribeSaveParameters(EventHandlerIdx, PartitionIdx, clientListIdx,
      IndexOfEntry, RxMessageInfoPtr, AttachedOptionsPtr);                                                              /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #201 Check if all required parameters of the event could be saved. */
    if(retVal != E_OK)
    {
      /* #2010 Not able to save all required parameters. */
      if(Sd_GetNrOfReferencesOfClientList(clientListIdx, PartitionIdx) == 0u)
      {
        /* #20100 Remove the client from list. */
        Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID, PartitionIdx);              /* SBSW_SD_CSL01_CSL05 */
      }
    }
  }
  else
  {
    /* #210 Otherwise, the client is not valid. Check if it transmitted a StopSubscribeEventgroup entry. */
    if(entryIsStopSubscribe == TRUE)
    {
      /* #2100 Unknown client transmitted a StopSubscribeEventgroup entry. Ignore this entry. */
      retVal = E_OK;
    }
    else
    {
      /* #2101 Otherwise, the maximum amount of clients is reached. Not able to process this request. */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_RxServer_ProcessEntrySubscribeSaveParameters()
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
/* PRQA S 6060 1 */ /* MD_Sd_STPAR_6060 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_ProcessEntrySubscribeSaveParameters(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfClientListType ClientListIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(serverIdx, PartitionIdx);
  uint32 rxTTL;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the TTL reserved field and counter of the received SubscribeEventgroup message. */
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);
  Sd_SetRequestedTTLOfClientList(ClientListIdx, rxTTL, PartitionIdx);                                                   /* SBSW_SD_API_ARGUMENT_IDX */
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr,                                                                           /* SBSW_SD_API_ARGUMENT_IDX */
    IndexOfEntry + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, *Sd_GetAddrRxReservedOfClientList(ClientListIdx, PartitionIdx));

  SD_BEGIN_CRITICAL_SECTION();

  /* #20 Save all attached endpoints, the processing will be done in the next main function cycle. */
  if(Sd_EventHandlerUsesUdp(EventHandlerIdx, PartitionIdx))
  {
    /* #200 Save the received UDP endpoint. */
    Sd_EndpointStorageUdpIdxOfClientListType endpointStorageIdx; /* Set by Sd_RxServer_SaveEndpoint(). */

    retVal = Sd_RxServer_SaveEndpoint(instanceIdx, PartitionIdx, &AttachedOptionsPtr->EndpointAddrUdp,
      SD_ENDPOINT_TYPE_UDP, &endpointStorageIdx);                                                                      /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    Sd_SetEndpointStorageUdpIdxOfClientList(ClientListIdx, endpointStorageIdx, PartitionIdx);                          /* SBSW_SD_API_ARGUMENT_IDX */
  }

  if(Sd_EventHandlerUsesTcp(EventHandlerIdx, PartitionIdx))
  {
    /* #201 Save the received TCP endpoint. */
    Sd_EndpointStorageTcpIdxOfClientListType endpointStorageIdx; /* Set by Sd_RxServer_SaveEndpoint(). */

    retVal |= Sd_RxServer_SaveEndpoint(instanceIdx, PartitionIdx, &AttachedOptionsPtr->EndpointAddrTcp,
      SD_ENDPOINT_TYPE_TCP, &endpointStorageIdx);                                                                       /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    Sd_SetEndpointStorageTcpIdxOfClientList(ClientListIdx, endpointStorageIdx, PartitionIdx);                           /* SBSW_SD_API_ARGUMENT_IDX */
  }

  /* #30 Check if the endpoints could be saved. */
  if(retVal == E_OK)
  {
    /* #300 Save the received (Stop)SubscribeEventgroup message as event. */
    Sd_ServerEventgroupEventType eventFlag = SD_SERVER_EVENT_EVENTGROUP_NONE;

    if(RxMessageInfoPtr->MulticastMessage == TRUE)
    {
      eventFlag = (Sd_ServerEventgroupEventType)SD_SERVER_EVENT_EVENTGROUP_MULTICAST_FLAG; /*lint !e655 */
    }

    if(rxTTL == 0u)
    {
      /* SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP */
      retVal = Sd_RxServer_AddEventToRingBuffer(instanceIdx, EventHandlerIdx, ClientListIdx, PartitionIdx,
        (Sd_ServerEventgroupEventType)(SD_SERVER_EVENT_EVENTGROUP_STOP_SUBSCRIBE | eventFlag)); /*lint !e655 */
    }
    else
    {
      /* SD_ENTRY_SUBSCRIBE_EVENTGROUP */
      retVal = Sd_RxServer_AddEventToRingBuffer(instanceIdx, EventHandlerIdx, ClientListIdx, PartitionIdx,
        (Sd_ServerEventgroupEventType)(SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE | eventFlag)); /*lint !e655 */
    }
  }

  SD_END_CRITICAL_SECTION();

  Sd_SetStateMachineExecutionRequestedOfServerDyn(serverIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
  Sd_Util_TriggerStateMachineRun(instanceIdx, PartitionIdx);

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_EventServer_Process()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_Process(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfRxEventType rxEventIdx;
  Sd_SizeOfEventHandlerType eventHandlerIdx;
  boolean pendingStopSubscribe = FALSE, keepEntry;
  boolean discardEvent;
  Sd_QueueProcessLimitOfInstanceType entriesToProcessLeft;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine maximum number of entries to process in one cycle. */
  entriesToProcessLeft = Sd_GetQueueProcessLimitOfInstance(InstanceIdx, PartitionIdx);

  /* #20 Process pending events for event handler. */
  while((TRUE == Sd_EventServer_GetFirstIdx(InstanceIdx, PartitionIdx, &rxEventIdx)) && (entriesToProcessLeft > 0u))    /* SBSW_SD_REFERENCE_OF_VARIABLE */
  {
    eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(rxEventIdx, PartitionIdx);

    /* #200 Check that the event handler as well as the remote client are in valid states. */
    if((Sd_GetClientStateOfClientList(Sd_GetClientListIdxOfRxEvent(rxEventIdx, PartitionIdx), PartitionIdx) !=
          SD_EVENT_HANDLER_CLIENT_STATE_INVALID) &&
       ((Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx, PartitionIdx) ==
           SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED) ||
        (Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx, PartitionIdx) ==
           SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION) ||
        (Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx, PartitionIdx) ==
           SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION)))
    {
      /* #2000 Check if event needs to be discarded. */
      discardEvent = Sd_EventServer_DiscardEvent(InstanceIdx, PartitionIdx, rxEventIdx);

      if(discardEvent == FALSE)
      {
        /* #20000 Process the pending event. */
        keepEntry = Sd_EventServer_ProcessEvent(rxEventIdx, PartitionIdx, &pendingStopSubscribe);                       /* SBSW_SD_REFERENCE_OF_VARIABLE */

        if(keepEntry == TRUE)
        {
          /* #20001 Do not remove the entry from the list in order to process it in the next MainFunction cycle.
           *       Stop processing of remaining entries. */
          break;
        }
      }
      /* #3000 If StopSubscribe-Subscribe sequence and StopSubscribe has been discarded,
               check if notifications have to be sent. */
      /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
      else if((Sd_GetEventOfRxEvent(rxEventIdx, PartitionIdx) == SD_SERVER_EVENT_EVENTGROUP_STOP_SUBSCRIBE) &&
              (Sd_EventServer_IsTriggerTransmitEnabled(rxEventIdx, PartitionIdx) == TRUE))
      {
          /* #30000 Notifications needed, save the request in the trigger transmit list. */
          (void)Sd_TriggerTransmit_Add(PartitionIdx, rxEventIdx);
      }
      else
      {
        /* Nothing to do */
      }
    }

    /* #201 Remove the processed event from the buffer. */
    Sd_EventServer_RemoveFirst(InstanceIdx, PartitionIdx);
    entriesToProcessLeft--;
  }
}

/**********************************************************************************************************************
*  Sd_EventServer_DiscardEvent()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_DiscardEvent(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfRxEventType CurrentRxEventIdx)
{
  Sd_ServerEventgroupEventType currentEventType;
  Sd_ServerEventgroupEventType revertEventType;
  Sd_SizeOfRxEventType nextEventIdx;
  boolean discardCurrentEvent = FALSE;

  /* #10 Get event type of current event which is being processed. */
  currentEventType = Sd_GetEventOfRxEvent(CurrentRxEventIdx, PartitionIdx);

  /* #20 Remove multicast flag since it is not relevant for this functionality. */
  currentEventType = (Sd_ServerEventgroupEventType)(currentEventType & (Sd_ServerEventgroupEventType)(~SD_SERVER_EVENT_EVENTGROUP_MULTICAST_FLAG)); /*lint !e655 */

  /* #30 Determine event type which would revert the state of the CEG to the current state. */
  if(currentEventType == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE)
  {
    revertEventType = SD_SERVER_EVENT_EVENTGROUP_STOP_SUBSCRIBE;
  }
  else
  {
    revertEventType = SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE;
  }

  /* #40 Get index of following event in queue. */
  nextEventIdx = Sd_EventServer_GetFollowingIdx(InstanceIdx, PartitionIdx, CurrentRxEventIdx);

  /* #50 Iterate over all following events in the queue. */
  while(nextEventIdx != Sd_GetSizeOfRxEvent(PartitionIdx))
  {
    /* #500 If a following event will nullify the currently processed event, the currently processed event can be discarded. */
    if(((Sd_GetEventOfRxEvent(nextEventIdx, PartitionIdx) & ((Sd_ServerEventgroupEventType)(~SD_SERVER_EVENT_EVENTGROUP_MULTICAST_FLAG))) == revertEventType) && /*lint !e655 */
        (Sd_GetClientListIdxOfRxEvent(nextEventIdx, PartitionIdx) ==
          Sd_GetClientListIdxOfRxEvent(CurrentRxEventIdx, PartitionIdx)))
    {
      discardCurrentEvent = TRUE;
      break;
    }

    nextEventIdx = Sd_EventServer_GetFollowingIdx(InstanceIdx, PartitionIdx, nextEventIdx);
  }

  return discardCurrentEvent;
}

/**********************************************************************************************************************
 *  Sd_EventServer_ProcessEvent()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_ProcessEvent(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(boolean) PendingStopSubscribePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean keepEntry = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  Sd_ServerEventgroupEventType eventType = Sd_GetEventOfRxEvent(RxEventIdx, PartitionIdx);

  /* #10 Check the type of the received entry. */
  if((eventType == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE) ||
    (eventType == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE_MULTI))
  {
    /* #100 SubscribeEventgroup: Check if there was a pending StopSubscribeEventgroup event which has to be processed first. */
    if(*PendingStopSubscribePtr == TRUE)
    {
      /* #1000 There is a pending StopSubscribeEventgroup which has to be handled during the next run of the SoAd_MainFunction().
       *       Resume processing of the subscription in the next MainFunction cycle. */
      keepEntry = TRUE;
    }
    else
    {
      /* #1001 Otherwise, process subscription of remote client. */
      Sd_EventServer_Subscription(RxEventIdx, PartitionIdx);
    }
  }
  else
  {
    /* #101 StopSubscribeEventgroup: Subscription was stopped, remove the client. */
    Sd_UtilServer_RemoveClientFromList(Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx, PartitionIdx), PartitionIdx,
      Sd_GetClientListIdxOfRxEvent(RxEventIdx, PartitionIdx));
    *PendingStopSubscribePtr = TRUE;                                                                                    /* SBSW_SD_POINTER_WRITE */
  }

  return keepEntry;
}

/**********************************************************************************************************************
 *  Sd_StateServer_Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateServer_Update(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ServerStateMachineType NewState,
  boolean ResetTimeToTriggerSend)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the new state of the state machine. */
  Sd_SetStateMachineOfServerDyn(ServerIdx, NewState, PartitionIdx);                                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Mark server service for state machine execution and set a timer event. */
  Sd_SetStateMachineExecutionRequestedOfServerDyn(ServerIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx), PartitionIdx);

  if(ResetTimeToTriggerSend == TRUE)
  {
    /* #200 Reset the TimeToTriggerSend. */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx));                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_UpdateEventHandlerStateMachine()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_UpdateEventHandlerStateMachine(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_EventHandlerStateMachineType NewState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_ServerIdxOfEventHandlerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the new state of the state machine. */
  Sd_SetStateMachineOfEventHandlerDyn(EventHandlerIdx, NewState, PartitionIdx);                                         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Mark server service for state machine execution and set a timer event. */
  Sd_SetStateMachineExecutionRequestedOfServerDyn(serverIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfServer(serverIdx, PartitionIdx), PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_UtilServer_UpdateEventHandlerRequestState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_UpdateEventHandlerRequestState(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_EventHandlerCurrentStateType NewState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_ServerIdxOfEventHandlerType serverIdx = Sd_GetServerIdxOfEventHandler(EventHandlerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the request state to the new value. */
  Sd_SetRequestStateOfEventHandlerDyn(EventHandlerIdx, NewState, PartitionIdx);                                         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Trigger state machine execution for the next MainFunction cycle. */
  Sd_SetStateMachineExecutionRequestedOfServerDyn(serverIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfServer(serverIdx, PartitionIdx), PartitionIdx);

  /* #30 Inform the BswM about the new request state. */
  BswM_Sd_EventHandlerCurrentState(Sd_GetHandleIdOfEventHandler(EventHandlerIdx, PartitionIdx), NewState);
}

/**********************************************************************************************************************
 *  Sd_RxServer_AddEventToRingBuffer()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_AddEventToRingBuffer(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_SizeOfClientListType ClientListIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ServerEventgroupEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType eventSaved = E_NOT_OK;
  Sd_SizeOfRxEventType rxEventIdx;
  Sd_SizeOfEndpointStorageType endpointStorageIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the ring buffer is full. */
  if(!Sd_IsRxEventListFullOfInstanceDyn(InstanceIdx, PartitionIdx))
  {
    SD_BEGIN_CRITICAL_SECTION();
    eventSaved = E_OK;

    /* #100 Save the received event. */
    rxEventIdx = Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx, PartitionIdx);
    Sd_SetEventHandlerIdxOfRxEvent(rxEventIdx, EventHandlerIdx, PartitionIdx);                                          /* SBSW_SD_RXEVENT_IDX */
    Sd_SetClientListIdxOfRxEvent(rxEventIdx, ClientListIdx, PartitionIdx);                                              /* SBSW_SD_RXEVENT_IDX */
    Sd_SetEventOfRxEvent(rxEventIdx, Event, PartitionIdx);                                                              /* SBSW_SD_RXEVENT_IDX */

    /* #101 Update the entry of the client list. */
    Sd_IncNrOfReferencesOfClientList(ClientListIdx, PartitionIdx);                                                      /* SBSW_SD_API_ARGUMENT_IDX */

    endpointStorageIdx = Sd_GetEndpointStorageTcpIdxOfClientList(ClientListIdx, PartitionIdx);
    if(endpointStorageIdx < Sd_GetSizeOfEndpointStorage(PartitionIdx))
    {
      Sd_IncNrOfReferencesOfEndpointStorage(endpointStorageIdx, PartitionIdx);                                          /* SBSW_SD_CSL01_CSL05 */
    }

    endpointStorageIdx = Sd_GetEndpointStorageUdpIdxOfClientList(ClientListIdx, PartitionIdx);
    if(endpointStorageIdx < Sd_GetSizeOfEndpointStorage(PartitionIdx))
    {
      Sd_IncNrOfReferencesOfEndpointStorage(endpointStorageIdx, PartitionIdx);                                          /* SBSW_SD_CSL01_CSL05 */
    }

    /* #102 Increment the ring buffer end index. */
    Sd_IncRxEventEndIdxOfInstanceDyn(InstanceIdx, PartitionIdx);                                                        /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    /* #103 Handle wrap around. */
    if(Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx, PartitionIdx) >=
         Sd_GetRxEventEndIdxOfInstance(InstanceIdx, PartitionIdx))
    {
      Sd_SetRxEventEndIdxOfInstanceDyn(InstanceIdx, Sd_GetRxEventStartIdxOfInstance(InstanceIdx, PartitionIdx),         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
        PartitionIdx);
    }

    /* #104 Check if the ring buffer is full. */
    if(Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx, PartitionIdx) ==
         Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx, PartitionIdx))
    {
      Sd_SetRxEventListFullOfInstanceDyn(InstanceIdx, TRUE, PartitionIdx);                                              /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }

    SD_END_CRITICAL_SECTION();
  }
  else
  {
    /* #105 The ring buffer is full. Not able to save the event. */
  }

  return eventSaved;
}

/**********************************************************************************************************************
 *  Sd_EventServer_GetFirstIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_GetFirstIdx(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_SizeOfRxEventType) EventIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean eventRead = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the ring buffer contains an element. */
  if((Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx, PartitionIdx) !=
        Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx, PartitionIdx)) ||
     Sd_IsRxEventListFullOfInstanceDyn(InstanceIdx, PartitionIdx))
  {
    /* #100 Read the first element from of the buffer. */
    *EventIdxPtr = Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx, PartitionIdx);                                       /* SBSW_SD_POINTER_WRITE */
    eventRead = TRUE;
  }
  else
  {
    /* The buffer contains no elements. */
    *EventIdxPtr = Sd_GetSizeOfRxEvent(PartitionIdx);                                                                   /* SBSW_SD_POINTER_WRITE */
  }

  return eventRead;
}

/**********************************************************************************************************************
 *  Sd_EventServer_RemoveFirst()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_RemoveFirst(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(
    Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx, PartitionIdx), PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Remove the references of the saved client and the endpoints. */

  Sd_DecNrOfReferencesOfClientList(clientListIdx, PartitionIdx);                                                        /* SBSW_SD_RXEVENT_MEMBER */

  if(Sd_GetNrOfReferencesOfClientList(clientListIdx, PartitionIdx) == 0u)
  {
    /* #100 The client is not referenced by another event. Remove it from the list. */
    Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID, PartitionIdx);                  /* SBSW_SD_RXEVENT_MEMBER */
  }

  /* #20 Remove the TCP endpoint. */
  if(Sd_EventServer_RemoveEndpoint(Sd_GetEndpointStorageTcpIdxOfClientList(clientListIdx, PartitionIdx),
       PartitionIdx) == FALSE)
  {
    /* #200 Clear reference because the endpoint is not used by another instance. */
    Sd_SetEndpointStorageTcpIdxOfClientList(clientListIdx, Sd_GetSizeOfEndpointStorage(PartitionIdx), PartitionIdx);    /* SBSW_SD_RXEVENT_MEMBER */
  }

  /* #30 Remove UDP endpoint. */
  if(Sd_EventServer_RemoveEndpoint(Sd_GetEndpointStorageUdpIdxOfClientList(clientListIdx, PartitionIdx),
       PartitionIdx) == FALSE)
  {
    /* #300 Clear reference because the endpoint is not used by another instance. */
    Sd_SetEndpointStorageUdpIdxOfClientList(clientListIdx, Sd_GetSizeOfEndpointStorage(PartitionIdx), PartitionIdx);    /* SBSW_SD_RXEVENT_MEMBER */
  }

  SD_BEGIN_CRITICAL_SECTION();

  /* #40 Remove the first element of the buffer. */
  Sd_IncRxEventStartIdxOfInstanceDyn(InstanceIdx, PartitionIdx);                                                        /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #50 Handle wrap around. */
  if(Sd_GetRxEventStartIdxOfInstanceDyn(InstanceIdx, PartitionIdx) >=
       Sd_GetRxEventEndIdxOfInstance(InstanceIdx, PartitionIdx))
  {
    Sd_SetRxEventStartIdxOfInstanceDyn(InstanceIdx, Sd_GetRxEventStartIdxOfInstance(InstanceIdx, PartitionIdx),         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      PartitionIdx);
  }

  /* #60 Check if the full flag has to be removed. */
  if(Sd_IsRxEventListFullOfInstanceDyn(InstanceIdx, PartitionIdx))
  {
    Sd_SetRxEventListFullOfInstanceDyn(InstanceIdx, FALSE, PartitionIdx);                                               /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_EventServer_GetFollowingIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Sd_SizeOfRxEventType, SD_CODE) Sd_EventServer_GetFollowingIdx(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfRxEventType RxEventIdx)
{
  Sd_SizeOfRxEventType nextRxEventIdx;

  /* #10 Increase queue index and observe wrap around */
  nextRxEventIdx = RxEventIdx + 1u;
  if(nextRxEventIdx == Sd_GetRxEventEndIdxOfInstance(InstanceIdx, PartitionIdx))
  {
    nextRxEventIdx = Sd_GetRxEventStartIdxOfInstance(InstanceIdx, PartitionIdx);
  }

  /* #20 Set to invalid value if there is no next element */
  if(nextRxEventIdx == Sd_GetRxEventEndIdxOfInstanceDyn(InstanceIdx, PartitionIdx))
  {
    nextRxEventIdx = Sd_GetSizeOfRxEvent(PartitionIdx);
  }

  return nextRxEventIdx;
}

/**********************************************************************************************************************
 *  Sd_RxServer_SaveEndpoint()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_RxServer_SaveEndpoint(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_SockAddrContainerType) InAddrPtr,
  Sd_EndpointType Type,
  SD_P2VAR(Sd_SizeOfEndpointStorageType) OutEndpointStorageIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_SizeOfEndpointStorageType endpointStorageIdx;
  boolean saveAddr = TRUE;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Set invalid endpoint index. Will be overwritten if endpoint could be saved. */
  *OutEndpointStorageIdxPtr = Sd_GetSizeOfEndpointStorage(PartitionIdx);                                                /* SBSW_SD_POINTER_WRITE */

  /* #20 Check if the address is already saved. */
  for(endpointStorageIdx = Sd_GetEndpointStorageStartIdxOfInstance(InstanceIdx, PartitionIdx);
    endpointStorageIdx < Sd_GetEndpointStorageEndIdxOfInstance(InstanceIdx, PartitionIdx);
    endpointStorageIdx++)
  {
    /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
    if((Sd_GetNrOfReferencesOfEndpointStorage(endpointStorageIdx, PartitionIdx) != 0u) &&
      (Sd_GetTypeOfEndpointStorage(endpointStorageIdx, PartitionIdx) == Type) &&
      (Sd_Util_SockAddrIsEqual(Sd_GetAddrAddrOfEndpointStorage(endpointStorageIdx, PartitionIdx), InAddrPtr) == TRUE))  /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
    {
      /* #2000 The address is already saved. Save index of address. */
      saveAddr = FALSE;
      retVal = E_OK;
      *OutEndpointStorageIdxPtr = endpointStorageIdx;                                                                   /* SBSW_SD_POINTER_WRITE */
      break;
    }
  }

  if(saveAddr == TRUE)
  {
    /* #300 If the address has to be saved, search for the next free slot. */
    for(endpointStorageIdx = Sd_GetEndpointStorageStartIdxOfInstance(InstanceIdx, PartitionIdx);
      endpointStorageIdx < Sd_GetEndpointStorageEndIdxOfInstance(InstanceIdx, PartitionIdx);
      endpointStorageIdx++)
    {
      if(Sd_GetTypeOfEndpointStorage(endpointStorageIdx, PartitionIdx) == SD_ENDPOINT_TYPE_INVALID)
      {
        /* #3000 Save the address and type of the endpoint. */
        Sd_Util_CopySockAddr(Sd_GetAddrAddrOfEndpointStorage(endpointStorageIdx, PartitionIdx), InAddrPtr);             /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
        Sd_SetTypeOfEndpointStorage(endpointStorageIdx, Type, PartitionIdx);                                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        retVal = E_OK;
        *OutEndpointStorageIdxPtr = endpointStorageIdx;                                                                 /* SBSW_SD_POINTER_WRITE */
        break;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_EventServer_RemoveEndpoint()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_RemoveEndpoint(
  Sd_SizeOfEndpointStorageType EndpointStorageIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean stillReferenced = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the endpoint is valid. */
  if(EndpointStorageIdx < Sd_GetSizeOfEndpointStorage(PartitionIdx))
  {
    /* #100 Decrement number of references. */
    Sd_DecNrOfReferencesOfEndpointStorage(EndpointStorageIdx, PartitionIdx);                                            /* SBSW_SD_CSL01_CSL05 */

    /* #101 Remove endpoint if not used anymore. */
    if(Sd_GetNrOfReferencesOfEndpointStorage(EndpointStorageIdx, PartitionIdx) == 0u)
    {
      (Sd_GetAddrAddrOfEndpointStorage(EndpointStorageIdx, PartitionIdx))->family = SOAD_AF_INVALID;                    /* SBSW_SD_CSL01_CSL05 */
      Sd_SetTypeOfEndpointStorage(EndpointStorageIdx, SD_ENDPOINT_TYPE_INVALID, PartitionIdx);                          /* SBSW_SD_CSL01_CSL05 */
      stillReferenced = FALSE;
    }
  }

  return stillReferenced;
}

/**********************************************************************************************************************
 *  Sd_EventServer_Subscription()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_EventServer_Subscription(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(serverIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is already subscribed. */
  if(Sd_GetClientStateOfClientList(clientListIdx, PartitionIdx) == SD_EVENT_HANDLER_CLIENT_STATE_UNSUBSCRIBED)
  {
    /* #100 The client subscribed for the first time. */
    retVal = Sd_EventServer_SubscriptionInitial(RxEventIdx, PartitionIdx);
  }

  /* #20 Check if the configuration was successful. */
  if(retVal == E_OK)
  {
    /* #200 Subscription was successful.
     *      Update the client configuration. */
    Sd_Util_SetTimerOffsetSecWithTrigger(Sd_GetAddrSubscribedUntilOfClientList(clientListIdx, PartitionIdx),
      Sd_GetRequestedTTLOfClientList(clientListIdx, PartitionIdx),
      Sd_GetInstanceIdxOfServer(serverIdx, PartitionIdx), PartitionIdx);                                                /* SBSW_SD_REFERENCE_RXEVENT_MEMBER */

    /* #201 Send a SubscribeEventgroupAck message and update the state. */
    Sd_Util_AddEventGroupAckEntryToList(instanceIdx, PartitionIdx, RxEventIdx);

    if(Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx, PartitionIdx) == SD_EVENT_HANDLER_STATE_NOT_SUBSCRIBED)
    {
      /* #2010 Set the state of the event handler to requested and inform the BswM. */
      Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, PartitionIdx, SD_EVENT_HANDLER_REQUESTED);
      Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, PartitionIdx,
        SD_EVENT_HANDLER_STATE_UNICAST_SUBSCRIPTION);
    }
  }
  else
  {
    Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetSizeOfServerTimer(PartitionIdx);

    /* #202 Subscription was NOT successful. (Not able to configure all mandatory endpoints.) */

    if(Sd_GetEventOfRxEvent(RxEventIdx, PartitionIdx) == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE_MULTI)
    {
      /* #2020 Add a random delay to the response message. */
      serverTimerIdx = Sd_GetServerTimerIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
    }

    /* #203 Send a SubscribeEventgroupNegativeAcknowledge message. */
    Sd_Util_AddEventGroupNackEntryToList(instanceIdx, PartitionIdx, serverIdx,
      Sd_GetServiceIdOfServer(serverIdx, PartitionIdx),
      Sd_GetInstanceIdOfServerDyn(serverIdx, PartitionIdx),
      Sd_GetMajorVersionOfServer(serverIdx, PartitionIdx),
      Sd_GetRxReservedOfClientList(clientListIdx, PartitionIdx),
      Sd_GetEventGroupIdOfEventHandler(eventHandlerIdx, PartitionIdx),
      Sd_GetAddressStorageIdxOfClientList(clientListIdx, PartitionIdx),
      serverTimerIdx);

    /* #204 Undo the configuration of the client and remove the client from the list. */
    Sd_UtilServer_RemoveClientFromList(eventHandlerIdx, PartitionIdx, clientListIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitial()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitial(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 The event handler uses TCP. Update the configuration. */
  if(Sd_EventHandlerUsesTcp(eventHandlerIdx, PartitionIdx))
  {
    retVal = Sd_EventServer_SubscriptionInitialTcp(RxEventIdx, PartitionIdx);
  }

  /* #20 The event handler uses UDP. Update the configuration. */
  /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
  if(Sd_EventHandlerUsesUdp(eventHandlerIdx, PartitionIdx) && (retVal == E_OK))
  {
    retVal = Sd_EventServer_SubscriptionInitialUdp(RxEventIdx, PartitionIdx);
  }

  /* #30 Check if the event handler is configured for trigger transmit. */
  /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
  if((retVal == E_OK) && (Sd_EventServer_IsTriggerTransmitEnabled(RxEventIdx, PartitionIdx) == TRUE))
  {
    /* #300 The client subscribed the first time, save the request in the trigger transmit list. */
    (void)Sd_TriggerTransmit_Add(PartitionIdx, RxEventIdx);
  }

  if(retVal == E_OK)
  {
    /* #400 The client subscribed successfully. */
    Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED, PartitionIdx);               /* SBSW_SD_RXEVENT_MEMBER */
    Sd_IncNrOfReferencesOfClientList(clientListIdx, PartitionIdx);                                                      /* SBSW_SD_RXEVENT_MEMBER */
    Sd_IncNrOfClientsOfEventHandlerDyn(eventHandlerIdx, PartitionIdx);                                                  /* SBSW_SD_RXEVENT_MEMBER */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_EventServer_IsTriggerTransmitEnabled()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_EventServer_IsTriggerTransmitEnabled(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  boolean retVal = FALSE;

  /* #10 Get Index of TCP and UDP EventHandler associated with this RxEvent. */
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfEventHandlerTcpType eventHandlerTcpIdx =
    Sd_GetEventHandlerTcpIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
  Sd_SizeOfEventHandlerUdpType eventHandlerUdpIdx =
    Sd_GetEventHandlerUdpIdxOfEventHandler(eventHandlerIdx, PartitionIdx);

  /* #20 Check if TCP EventHandler is configured for triggered transmission. */
  if((eventHandlerTcpIdx != SD_NO_EVENTHANDLERTCPIDXOFEVENTHANDLER) &&
    (Sd_GetRoutingGroupTriggerIdxOfEventHandlerTcp(eventHandlerTcpIdx, PartitionIdx) !=
      SD_NO_ROUTINGGROUPTRIGGERIDXOFEVENTHANDLERTCP))
  {
    retVal = TRUE;
  }

  /* #30 Check if UDP EventHandler is configured for triggered transmission. */
  if((eventHandlerUdpIdx != SD_NO_EVENTHANDLERUDPIDXOFEVENTHANDLER) &&
    (Sd_GetRoutingGroupTriggerIdxOfEventHandlerUdp(eventHandlerUdpIdx, PartitionIdx) !=
      SD_NO_ROUTINGGROUPTRIGGERIDXOFEVENTHANDLERUDP))
  {
    retVal = TRUE;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitialTcp()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitialTcp(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
  SoAd_SoConIdType assignedSoConId = SOAD_INV_SO_CON_ID;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client has already established a TCP connection. */
  /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_SetUniqueRemoteAddr(
    Sd_GetTcpSoConId(Sd_GetTcpSoConIdStartIdxOfServer(serverIdx, PartitionIdx), PartitionIdx),
    (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrAddrOfEndpointStorage(
      Sd_GetEndpointStorageTcpIdxOfClientList(clientListIdx, PartitionIdx), PartitionIdx),
    &assignedSoConId);                                                                                                  /* SBSW_SD_REFERENCE_RXEVENT_MEMBER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_SOCK_ADDR_CAST */

  if(retVal == E_OK)
  {
    /* #100 Check if the socket connection is online. */
    if(SD_SOCON_MODE_ONLINE == Sd_Util_GetSoConMode(assignedSoConId, PartitionIdx))
    {
      Sd_SizeOfEventHandlerTcpType eventHandlerTcpIdx =
        Sd_GetEventHandlerTcpIdxOfEventHandler(eventHandlerIdx, PartitionIdx);

      /* #1000 The client TCP IP address is already configured, enable the TCP routing group. */
      retVal = Sd_Util_EnableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerTcp(eventHandlerTcpIdx, PartitionIdx),
        PartitionIdx,
        assignedSoConId);
      Sd_UtilServer_IncreaseActivationCount(eventHandlerIdx, PartitionIdx, assignedSoConId);

      /* #1001 Save the state of the client. */
      Sd_SetAssignedTcpSoConIdOfClientList(clientListIdx, assignedSoConId, PartitionIdx);                               /* SBSW_SD_RXEVENT_MEMBER */
    }
    else
    {
      /* #1002 If socket connection is not online, release remote address. */
      SoAd_ForceReleaseRemoteAddr(assignedSoConId);
      retVal = E_NOT_OK;
    }
  }

  /* #20 Check if the subscription was successful. */
  if(Sd_GetAssignedTcpSoConIdOfClientList(clientListIdx, PartitionIdx) == SOAD_INV_SO_CON_ID)
  {
    /* #200 Reject the subscription. */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_EventServer_SubscriptionInitialUdp()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_EventServer_SubscriptionInitialUdp(
  Sd_SizeOfRxEventType RxEventIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
  SoAd_SoConIdType assignedSoConId = SOAD_INV_SO_CON_ID;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Configure a socket connection with the client endpoint address. */
  /* PRQA S 0310 4 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_SetUniqueRemoteAddr(
    Sd_GetIdOfUdpSoCon(Sd_GetUdpSoConStartIdxOfServer(serverIdx, PartitionIdx), PartitionIdx),
    (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrAddrOfEndpointStorage(
      Sd_GetEndpointStorageUdpIdxOfClientList(clientListIdx, PartitionIdx), PartitionIdx),
    &assignedSoConId);                                                                                                  /* SBSW_SD_REFERENCE_RXEVENT_MEMBER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_SOCK_ADDR_CAST */

  if(retVal == E_OK)
  {
    Sd_SizeOfEventHandlerUdpType eventHandlerUdpIdx =
      Sd_GetEventHandlerUdpIdxOfEventHandler(eventHandlerIdx, PartitionIdx);

    /* #100 Each event handler which requires the socket connection opens it in order to indicate the dependency. */
    retVal = SoAd_OpenSoCon(assignedSoConId);

    /* #101 Enable the unicast routing if the event handler is not in the multicast subscription state. */
    if((Sd_GetStateMachineOfEventHandlerDyn(eventHandlerIdx, PartitionIdx) !=
          SD_EVENT_HANDLER_STATE_MULTICAST_SUBSCRIPTION) &&
       (Sd_GetMulticastThresholdOfEventHandler(eventHandlerIdx, PartitionIdx) != 1u))
    {
      retVal |= Sd_Util_EnableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerUdp(eventHandlerUdpIdx, PartitionIdx),
        PartitionIdx,
        assignedSoConId);
      Sd_UtilServer_IncreaseActivationCount(eventHandlerIdx, PartitionIdx, assignedSoConId);
    }

    Sd_SetAssignedUdpSoConIdOfClientList(clientListIdx, assignedSoConId, PartitionIdx);                                 /* SBSW_SD_RXEVENT_MEMBER */
  }

  /* #20 Check if we were able to add the client to the system. */
  if(Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx, PartitionIdx) == SOAD_INV_SO_CON_ID)
  {
    /* #200 The client address could not be configured, reject the subscription. */
    Sd_Util_ReportDemEvent(Sd_GetEOutOfResIdOfInstance(Sd_GetInstanceIdxOfServer(serverIdx, PartitionIdx),
      PartitionIdx));
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_RxServer_AddClientToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Sd_SizeOfClientListType, SD_CODE) Sd_RxServer_AddClientToList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  boolean foundFreeSlot = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for the next free slot in client list. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx++)
  {
    if(Sd_GetClientStateOfClientList(clientListIdx, PartitionIdx) == SD_EVENT_HANDLER_CLIENT_STATE_INVALID)
    {
      /* #20 Save client information. */
      foundFreeSlot = TRUE;
      Sd_SetAddressStorageIdxOfClientList(clientListIdx, AddressStorageIdx, PartitionIdx);                              /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetAssignedTcpSoConIdOfClientList(clientListIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetAssignedUdpSoConIdOfClientList(clientListIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_UNSUBSCRIBED, PartitionIdx);           /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetNrOfReferencesOfClientList(clientListIdx, 0u, PartitionIdx);                                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetRequestedTTLOfClientList(clientListIdx, 0u, PartitionIdx);                                                  /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetRxReservedOfClientList(clientListIdx, 0u, PartitionIdx);                                                    /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetEndpointStorageTcpIdxOfClientList(clientListIdx, Sd_GetSizeOfEndpointStorage(PartitionIdx), PartitionIdx);  /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_SetEndpointStorageUdpIdxOfClientList(clientListIdx, Sd_GetSizeOfEndpointStorage(PartitionIdx), PartitionIdx);  /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_Util_ResetTimerToZero(Sd_GetAddrSubscribedUntilOfClientList(clientListIdx, PartitionIdx));                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      break;
    }
  }

  if(foundFreeSlot == FALSE)
  {
    /* #30 No free slot found. Return invalid ClientList index. */
    /* The amount of manageable clients corresponds to the amount of configured socket connections for this event handler. */
    clientListIdx = Sd_GetSizeOfClientList(PartitionIdx);
#if(SD_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_ADD_CLIENT_TO_LIST, SD_E_CLIENT_LIST_FULL);
#endif /* SD_DEV_ERROR_REPORT == STD_ON */
  }

  return clientListIdx;
}

/**********************************************************************************************************************
 *  Sd_UtilServer_SetDown()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_SetDown(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if(Sd_GetStateSwcOfServerDyn(ServerIdx, PartitionIdx) == SD_SERVER_SERVICE_AVAILABLE)
  {
    /* #10 Set the state of all associated event handlers. */
    Sd_UtilServer_ReleaseEventHandlers(ServerIdx, PartitionIdx);

    /* #20 Close all TCP socket connections of the server. */
    Sd_UtilServer_CloseTcpAndPreconfiguredSoCons(ServerIdx, PartitionIdx);

    /* #30 Disable the routing for provided methods. */
    Sd_UtilServer_DisableMethodRouting(ServerIdx, PartitionIdx);

    /* #40 Send a StopOfferService if ports for endpoints are valid. */
    if(Sd_ServerTcpLocalPortUnusedOrValid(ServerIdx, PartitionIdx) &&
       Sd_ServerUdpLocalPortUnusedOrValid(ServerIdx, PartitionIdx))
    {
      /* Send a StopOfferService message. */
      Sd_Util_AddMulticastOfferEntryToList(ServerIdx, PartitionIdx, TRUE);
    }

    /* #50 Update the state. */
    Sd_StateServer_Update(ServerIdx, PartitionIdx, SD_SERVER_STATE_NOT_READY, TRUE);
    Sd_SetStateSwcOfServerDyn(ServerIdx, SD_SERVER_SERVICE_DOWN, PartitionIdx);                                         /* SBSW_SD_API_ARGUMENT_IDX */
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_Configure()
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
/* PRQA S 6010 1 */ /* MD_Sd_STPTH_6010 */
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_Configure(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);
  Sd_EventHandlerIterType eventHandlerIdx;
  Sd_TcpSoConIdIterType tcpSoConIdIdx;
  Sd_UdpSoConIterType udpSoConIdx;
  uint32 additionalOffset;
  Sd_SizeOfServerTimerType serverTimerIdx = Sd_GetServerTimerIdxOfServer(ServerIdx, PartitionIdx);
  Sd_CommonInitialDelayIdxOfServerType commonDelayIdx = Sd_GetCommonInitialDelayIdxOfServer(ServerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Open the multicast socket connections. */
  for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx, PartitionIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx, PartitionIdx);
      eventHandlerIdx++)
  {
    Sd_EventHandlerMulticastIterType eventHandlerMulticastIdx;
    eventHandlerMulticastIdx = Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx, PartitionIdx);

    if(eventHandlerMulticastIdx != SD_NO_EVENTHANDLERMULTICASTIDXOFEVENTHANDLER)
    {
      (void)SoAd_OpenSoCon(Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx, PartitionIdx));
    }
  }

  /* #20 Open the TCP socket connections in order to allow incoming connections.
   *      (UDP socket connections will be configured and opened if a client subscribes.) */
  for(tcpSoConIdIdx = Sd_GetTcpSoConIdStartIdxOfServer(ServerIdx, PartitionIdx);
      tcpSoConIdIdx < Sd_GetTcpSoConIdEndIdxOfServer(ServerIdx, PartitionIdx);
      tcpSoConIdIdx++)
  {
    (void)SoAd_OpenSoCon(Sd_GetTcpSoConId(tcpSoConIdIdx, PartitionIdx));
  }

  /* #30 If sockets with preconfigured remote addresses exist, open them to allow provided method calls. */
  for(udpSoConIdx = Sd_GetUdpSoConStartIdxOfServer(ServerIdx, PartitionIdx);
      udpSoConIdx < Sd_GetUdpSoConEndIdxOfServer(ServerIdx, PartitionIdx);
      udpSoConIdx++)
  {
    if(Sd_IsPreconfiguredOfUdpSoCon(udpSoConIdx, PartitionIdx) == TRUE)
    {
      (void)SoAd_OpenSoCon(Sd_GetIdOfUdpSoCon(udpSoConIdx, PartitionIdx));
    }
  }

  /* #40 Enable the routing group for the provided method. */
  if(Sd_GetMethodIdxOfServer(ServerIdx, PartitionIdx) != SD_NO_METHODIDXOFSERVER)
  {
    (void)Sd_Util_EnableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfMethod(
      Sd_GetMethodIdxOfServer(ServerIdx, PartitionIdx), PartitionIdx), PartitionIdx);
  }

  /* #50 Get random delay if a common delay shall not be used or the common delay has not been determined yet. */
  if((commonDelayIdx == SD_NO_COMMONINITIALDELAYIDXOFSERVER) ||
    (Sd_GetCommonInitialDelay(commonDelayIdx, PartitionIdx) == SD_INV_TIMER_VALUE_32BIT))
  {
    additionalOffset = Sd_Util_GetRandomNrInRange(
      Sd_GetInitialOfferDelayMinOfServerTimer(serverTimerIdx, PartitionIdx),
      Sd_GetInitialOfferDelayMaxOfServerTimer(serverTimerIdx, PartitionIdx));

    if(commonDelayIdx != SD_NO_COMMONINITIALDELAYIDXOFSERVER)
    {
      Sd_SetCommonInitialDelay(commonDelayIdx, additionalOffset, PartitionIdx);                                         /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    }
  }
  /* #51 Common delay shall be used and it has been set already, use it. */
  else
  {
    additionalOffset = Sd_GetCommonInitialDelay(commonDelayIdx, PartitionIdx);
  }

  /* #60 Set Timer and mark server for execution of state machine. */
  Sd_SetStateMachineOfServerDyn(ServerIdx, SD_SERVER_STATE_READY_INITIAL_WAIT, PartitionIdx);                           /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_SetStateSwcOfServerDyn(ServerIdx, SD_SERVER_SERVICE_AVAILABLE, PartitionIdx);                                      /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_Util_SetTimerOffsetWithTrigger(
    Sd_GetAddrTimeToTriggerSendOfServerDyn(ServerIdx, PartitionIdx),
    additionalOffset + (SD_MAIN_FCT_CYCLE_TIME_MS * 2u), instanceIdx, PartitionIdx);                                    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  Sd_SetStateMachineExecutionRequestedOfServerDyn(ServerIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
}

/**********************************************************************************************************************
 *  Sd_UtilServer_RemoveClientsFromEventHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_RemoveClientsFromEventHandler(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all clients of the event handler. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx++)
  {
    /* #100 Check if the client is in a valid state. */
    if(Sd_GetClientStateOfClientList(clientListIdx, PartitionIdx) != SD_EVENT_HANDLER_CLIENT_STATE_INVALID)
    {
      /* #1000 Remove the client from the list. */
      Sd_UtilServer_RemoveClientFromList(EventHandlerIdx, PartitionIdx, clientListIdx);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_RemoveClientFromList()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_RemoveClientFromList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfClientListType ClientListIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Undo the configuration of the client communication paths. */
  Sd_UtilServer_UnconfigureClientComPaths(EventHandlerIdx, PartitionIdx, ClientListIdx);

  /* #20 Check if this client is subscribed for the event handler. */
  if(Sd_GetClientStateOfClientList(ClientListIdx, PartitionIdx) == SD_EVENT_HANDLER_CLIENT_STATE_SUBSCRIBED)
  {
    /* #200 Decrement the amount of subscribed clients. */
    if(Sd_GetNrOfClientsOfEventHandlerDyn(EventHandlerIdx, PartitionIdx) != 0u)
    {
      Sd_DecNrOfClientsOfEventHandlerDyn(EventHandlerIdx, PartitionIdx);                                                /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }

    /* #201 Check if the client is referenced from a pending event. */
    if(Sd_GetNrOfReferencesOfClientList(ClientListIdx, PartitionIdx) != 0u)
    {
      /* #2010 Decrement the reference counter. */
      Sd_DecNrOfReferencesOfClientList(ClientListIdx, PartitionIdx);                                                    /* SBSW_SD_API_ARGUMENT_IDX */
    }
  }

  /* #30 Check if the client is still referenced by other events. */
  if(Sd_GetNrOfReferencesOfClientList(ClientListIdx, PartitionIdx) == 0u)
  {
    /* #300 If the client is not referenced by another event. Remove it from the list. */
    Sd_SetClientStateOfClientList(ClientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID, PartitionIdx);                  /* SBSW_SD_API_ARGUMENT_IDX */
  }
  else
  {
    /* #301 Otherwise, if the client is referenced, keep the information in the list. */
    Sd_SetClientStateOfClientList(ClientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_UNSUBSCRIBED, PartitionIdx);             /* SBSW_SD_API_ARGUMENT_IDX */
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_UnconfigureClientComPaths()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_UnconfigureClientComPaths(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfClientListType ClientListIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIdType assignedUdpSoConId, assignedTcpSoConId;

  /* ----- Implementation ----------------------------------------------- */
  assignedTcpSoConId = Sd_GetAssignedTcpSoConIdOfClientList(ClientListIdx, PartitionIdx);
  assignedUdpSoConId = Sd_GetAssignedUdpSoConIdOfClientList(ClientListIdx, PartitionIdx);

  /* #10 Check if the client is configured for TCP. */
  if(assignedTcpSoConId != SOAD_INV_SO_CON_ID)
  {
    /* #100 Decrement the activation counter. */
    if(Sd_UtilServer_DecreaseActivationCount(EventHandlerIdx, PartitionIdx, assignedTcpSoConId) == TRUE)
    {
      /* #1000 Deactivate the routing group if no other instance of the client is active. */
      (void)Sd_Util_DisableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerTcp(
          Sd_GetEventHandlerTcpIdxOfEventHandler(EventHandlerIdx, PartitionIdx), PartitionIdx),
        PartitionIdx,
        assignedTcpSoConId);
    }

    Sd_SetAssignedTcpSoConIdOfClientList(ClientListIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                              /* SBSW_SD_API_ARGUMENT_IDX */
  }

  /* #20 Check if the client is configured for UDP. */
  if(assignedUdpSoConId != SOAD_INV_SO_CON_ID)
  {
    /* #200 Decrement the activation counter. */
    if(Sd_UtilServer_DecreaseActivationCount(EventHandlerIdx, PartitionIdx, assignedUdpSoConId) == TRUE)
    {
      /* #2000 Deactivate the routing group if no other instance of the client is active. */
      (void)Sd_Util_DisableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerUdp(
          Sd_GetEventHandlerUdpIdxOfEventHandler(EventHandlerIdx, PartitionIdx), PartitionIdx),
        PartitionIdx,
        assignedUdpSoConId);
    }

    /* #201 Close the socket connection. (The OpenCount is handled by SoAd). */
    (void)SoAd_CloseSoCon(assignedUdpSoConId, FALSE);
    Sd_SetAssignedUdpSoConIdOfClientList(ClientListIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                              /* SBSW_SD_API_ARGUMENT_IDX */
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_ReleaseEventHandlers()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_ReleaseEventHandlers(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfEventHandlerType eventHandlerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the state of all associated event handlers. */
  for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(ServerIdx, PartitionIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(ServerIdx, PartitionIdx);
      eventHandlerIdx++)
  {
    /* #100 Inform BswM if event handler was requested. */
    if(Sd_GetRequestStateOfEventHandlerDyn(eventHandlerIdx, PartitionIdx) == SD_EVENT_HANDLER_REQUESTED)
    {
      Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, PartitionIdx, SD_EVENT_HANDLER_RELEASED);
    }

    /* #101 Update event handler state machine. */
    Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, PartitionIdx, SD_EVENT_HANDLER_STATE_SERVICE_DOWN);

    /* #102 Remove all clients from the event handler. */
    Sd_UtilServer_RemoveClientsFromEventHandler(eventHandlerIdx, PartitionIdx);

    /* #103 Disable all referenced routing groups and reset the socket connections. */
    if(Sd_IsEventHandlerMulticastUsedOfEventHandler(eventHandlerIdx, PartitionIdx))
    {
      Sd_SizeOfEventHandlerMulticastType eventHandlerMulticastIdx =
        Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
      (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfEventHandlerMulticast(
        eventHandlerMulticastIdx, PartitionIdx), PartitionIdx);
      (void)SoAd_CloseSoCon(Sd_GetSoConIdOfEventHandlerMulticast(eventHandlerMulticastIdx, PartitionIdx), FALSE);
    }

    /* #104 Disable routing of UDP and TCP routing groups and reset activation count. */
    Sd_UtilServer_DisableRouting(eventHandlerIdx, PartitionIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_CloseTcpAndPreconfiguredSoCons()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_CloseTcpAndPreconfiguredSoCons(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_TcpSoConIdIterType tcpSoConIdIdx;
  Sd_UdpSoConIterType udpSoConIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Close all TCP socket connections of the server. */
  for(tcpSoConIdIdx = Sd_GetTcpSoConIdStartIdxOfServer(ServerIdx, PartitionIdx);
      tcpSoConIdIdx < Sd_GetTcpSoConIdEndIdxOfServer(ServerIdx, PartitionIdx);
      tcpSoConIdIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetTcpSoConId(tcpSoConIdIdx, PartitionIdx), FALSE);
  }

  /* #20 Close all UDP socket connections with preconfigured remote address. */
  for(udpSoConIdx = Sd_GetUdpSoConStartIdxOfServer(ServerIdx, PartitionIdx);
      udpSoConIdx < Sd_GetUdpSoConEndIdxOfServer(ServerIdx, PartitionIdx);
      udpSoConIdx++)
  {
    if(Sd_IsPreconfiguredOfUdpSoCon(udpSoConIdx, PartitionIdx) == TRUE)
    {
      (void)SoAd_CloseSoCon(Sd_GetIdOfUdpSoCon(udpSoConIdx, PartitionIdx), FALSE);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_DisableMethodRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_DisableMethodRouting(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the server provides methods. */
  if(Sd_IsMethodUsedOfServer(ServerIdx, PartitionIdx))
  {
    /* #100 Disable the routing group for the server methods. */
    (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfMethod(
      Sd_GetMethodIdxOfServer(ServerIdx, PartitionIdx), PartitionIdx), PartitionIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_LocalIpAddrAssignmentChgReset()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_LocalIpAddrAssignmentChgReset(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfServerType serverIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset state of all server services. */
  for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx, PartitionIdx);
    serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx, PartitionIdx);
    serverIdx++)
  {
    Sd_SizeOfEventHandlerType eventHandlerIdx;

    /* #100 Reset state of all dependent event handler. */
    for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(serverIdx, PartitionIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(serverIdx, PartitionIdx);
      eventHandlerIdx++)
    {
      Sd_SizeOfClientListType clientListIdx;

      /* #1000 Reset dynamic variables. */
      Sd_UtilServer_UpdateEventHandlerStateMachine(eventHandlerIdx, PartitionIdx, SD_EVENT_HANDLER_STATE_NONE);
      Sd_SetNrOfClientsOfEventHandlerDyn(eventHandlerIdx, 0u, PartitionIdx);                                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */

      /* #1001 Reset all clients. */
      for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
          clientListIdx < Sd_GetClientListEndIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
          clientListIdx++)
      {
        Sd_SetClientStateOfClientList(clientListIdx, SD_EVENT_HANDLER_CLIENT_STATE_INVALID, PartitionIdx);              /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      }

      /* #1002 Inform the BswM about the state of all event handlers. */
      Sd_UtilServer_UpdateEventHandlerRequestState(eventHandlerIdx, PartitionIdx, SD_EVENT_HANDLER_RELEASED);
    }

    /* #101 Reset the dynamic variables. */
    Sd_SetInitialOfferRepetitionRunOfServerDyn(serverIdx, 0u, PartitionIdx);                                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfServerDyn(serverIdx, PartitionIdx));                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_GetSoConActivationMapIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_SizeOfSoConActivationMapType, SD_CODE) Sd_UtilServer_GetSoConActivationMapIdx(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSoConActivationMapType iterIdx;
  Sd_SizeOfSoConActivationMapType soConActivationMapIdx = Sd_GetSizeOfSoConActivationMap(PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the socket connection is already saved in the map. */
  for(iterIdx = Sd_GetSoConActivationMapStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      iterIdx < Sd_GetSoConActivationMapEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      iterIdx++)
  {
    if(Sd_GetSoConIdOfSoConActivationMap(iterIdx, PartitionIdx) == SoConId)
    {
      soConActivationMapIdx = iterIdx;
      break;
    }
  }

  /* #20 Requested socket connection was not saved. */
  if(soConActivationMapIdx == Sd_GetSizeOfSoConActivationMap(PartitionIdx))
  {
    /* #200 Search for an unused slot to save the socket connection. */
    for(iterIdx = Sd_GetSoConActivationMapStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      iterIdx < Sd_GetSoConActivationMapEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      iterIdx++)
    {
      /* Check if the entry is unused. */
      if(Sd_GetSoConIdOfSoConActivationMap(iterIdx, PartitionIdx) == SOAD_INV_SO_CON_ID)
      {
        /* #2000 Save the new socket connection. */
        Sd_SetSoConIdOfSoConActivationMap(iterIdx, SoConId, PartitionIdx);                                              /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetNrOfActivationsOfSoConActivationMap(iterIdx, 0u, PartitionIdx);                                           /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        soConActivationMapIdx = iterIdx;
        break;
      }
    }
  }

  return soConActivationMapIdx;
}

/**********************************************************************************************************************
 *  Sd_UtilServer_IncreaseActivationCount()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_IncreaseActivationCount(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSoConActivationMapType soConActivationMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the activation map entry of the SoConId. */
  soConActivationMapIdx = Sd_UtilServer_GetSoConActivationMapIdx(EventHandlerIdx, PartitionIdx, SoConId);

  if(soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap(PartitionIdx))
  {
    /* #100 Increase the activation count. */
    Sd_IncNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, PartitionIdx);                                     /* SBSW_SD_CSL01_CSL05 */
  }
}

/**********************************************************************************************************************
 *  Sd_UtilServer_DecreaseActivationCount()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_UtilServer_DecreaseActivationCount(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSoConActivationMapType soConActivationMapIdx;
  boolean activationCountIsZero = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the activation map entry of the SoConId. */
  soConActivationMapIdx = Sd_UtilServer_GetSoConActivationMapIdx(EventHandlerIdx, PartitionIdx, SoConId);

  if(soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap(PartitionIdx))
  {
    if(Sd_GetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, PartitionIdx) != 0u)
    {
      /* #100 Decrease the activation count. */
      Sd_DecNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, PartitionIdx);                                   /* SBSW_SD_CSL01_CSL05 */

      /* #101 Check if the activation count was decreased to zero. */
      if(Sd_GetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, PartitionIdx) == 0u)
      {
        activationCountIsZero = TRUE;
      }
    }
  }

  return activationCountIsZero;
}

/**********************************************************************************************************************
 *  Sd_UtilServer_ResetActivationCount()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilServer_ResetActivationCount(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSoConActivationMapType soConActivationMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the activation map entry of the SoConId. */
  soConActivationMapIdx = Sd_UtilServer_GetSoConActivationMapIdx(EventHandlerIdx, PartitionIdx, SoConId);

  if(soConActivationMapIdx < Sd_GetSizeOfSoConActivationMap(PartitionIdx))
  {
    /* #100 Reset the activation count. */
    Sd_SetNrOfActivationsOfSoConActivationMap(soConActivationMapIdx, 0u, PartitionIdx);                                 /* SBSW_SD_CSL01_CSL05 */
  }
}

/**********************************************************************************************************************
 *  Sd_RxServer_FindClientInList()
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
SD_LOCAL FUNC(Sd_SizeOfClientListType, SD_CODE) Sd_RxServer_FindClientInList(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddrStorageIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr,
  boolean CreateNewClientIfNotFound)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx;
  boolean matchingClient = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is part of the list. */
  for(clientListIdx = Sd_GetClientListStartIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx < Sd_GetClientListEndIdxOfEventHandler(EventHandlerIdx, PartitionIdx);
      clientListIdx++)
  {
    if((Sd_GetClientStateOfClientList(clientListIdx, PartitionIdx) != SD_EVENT_HANDLER_CLIENT_STATE_INVALID) &&
      (Sd_GetAddressStorageIdxOfClientList(clientListIdx, PartitionIdx) == AddrStorageIdx))
    {
      /* #100 Found the corresponding remote address. Check if a remote reboot was detected. */
      /* AttachedOptionsPtr is NULL_PTR if called from Sd_HandleReboot_StopEvents after reboot was detected. */
      if(AttachedOptionsPtr == NULL_PTR)
      {
        /* #1000 Client has rebooted.
         *       Set the SubscribedUntil time to 0 in order to remove the client in the next main function cycle. */
        Sd_SetStateMachineExecutionRequestedOfServerDyn(                                                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
          Sd_GetServerIdxOfEventHandler(EventHandlerIdx, PartitionIdx), TRUE, PartitionIdx);
        Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrSubscribedUntilOfClientList(clientListIdx, PartitionIdx),
          Sd_GetInstanceIdxOfServer(Sd_GetServerIdxOfEventHandler(EventHandlerIdx, PartitionIdx), PartitionIdx),
          PartitionIdx);                                                                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      }
      else
      {
        /* #1001 No reboot detected. Check if the attached endpoints have changed. */
        matchingClient = Sd_RxServer_CheckAttachedEndpointOptions(EventHandlerIdx, PartitionIdx, clientListIdx,
          AttachedOptionsPtr);                                                                                          /* SBSW_SD_PASSED_API_POINTER_NOT_NULL */

        if(matchingClient == TRUE)
        {
          /* Client matches the attached endpoint options. */
          break;
        }
      }
    }
  }

  if((matchingClient == TRUE) || (CreateNewClientIfNotFound == FALSE))
  {
    /* #200 Matching client was found, or new client shall not be created. */
  }
  else
  {
    /* #201 The client was not found and is not subscribed. Add the client to the list. */
    clientListIdx = Sd_RxServer_AddClientToList(EventHandlerIdx, PartitionIdx, AddrStorageIdx);
  }

  return clientListIdx;
}

/**********************************************************************************************************************
 *  Sd_RxServer_CheckAttachedEndpointOptions()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_RxServer_CheckAttachedEndpointOptions(
  Sd_SizeOfEventHandlerType EventHandlerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfClientListType ClientListIdx,
  SD_P2CONST(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean tcpMatch, udpMatch;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the event handler uses TCP. */
  if(Sd_EventHandlerUsesTcp(EventHandlerIdx, PartitionIdx))
  {
    /* #100 Check the attached TCP endpoint. */
    tcpMatch = Sd_RxServer_CheckAttachedEndpointOption(
      Sd_GetEndpointStorageTcpIdxOfClientList(ClientListIdx, PartitionIdx), PartitionIdx,
      Sd_GetAssignedTcpSoConIdOfClientList(ClientListIdx, PartitionIdx), &AttachedOptionsPtr->EndpointAddrTcp);         /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
  }
  else
  {
    /* #101 Ignore TCP endpoint if event handler is not configured for TCP. */
    tcpMatch = TRUE;
  }

  /* #20 Check if the event handler uses UDP. */
  if(Sd_EventHandlerUsesUdp(EventHandlerIdx, PartitionIdx))
  {
    /* #200 Check the attached UDP endpoint. */
    udpMatch = Sd_RxServer_CheckAttachedEndpointOption(
      Sd_GetEndpointStorageUdpIdxOfClientList(ClientListIdx, PartitionIdx), PartitionIdx,
      Sd_GetAssignedUdpSoConIdOfClientList(ClientListIdx, PartitionIdx), &AttachedOptionsPtr->EndpointAddrUdp);         /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
  }
  else
  {
    /* #201 Ignore UDP endpoint if event handler is not configured for UDP. */
    udpMatch = TRUE;
  }

  return (boolean)((tcpMatch == TRUE) && (udpMatch == TRUE));
}

/**********************************************************************************************************************
 *  Sd_RxServer_CheckAttachedEndpointOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(boolean, SD_CODE) Sd_RxServer_CheckAttachedEndpointOption(
  Sd_SizeOfEndpointStorageType EndpointStorageIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId,
  SD_P2CONST(Sd_SockAddrContainerType) EndpointPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  boolean endpointMatch;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is already configured with a valid socket connection. */
  if(SoConId != SOAD_INV_SO_CON_ID)
  {
    Sd_SockAddrContainerType remoteAddr;

    /* #100 Check the configured remote address against the endpoint information. */
    endpointMatch = FALSE;

    retVal = Sd_SoAd_GetRemoteAddr(SoConId, &remoteAddr);                                                               /* SBSW_SD_REFERENCE_OF_VARIABLE */

    if(retVal == E_OK)
    {
      if(TRUE == Sd_Util_SockAddrIsEqual(&remoteAddr, EndpointPtr))                                                     /* SBSW_SD_REFERENCE_OF_VARIABLE */
      {
        endpointMatch = TRUE;
      }
    }
  }
  else
  {
    /* #101 The client is not (yet) configured, but has a temporary endpoint. Check it. */
    endpointMatch = FALSE;

    if(TRUE == Sd_Util_SockAddrIsEqual(Sd_GetAddrAddrOfEndpointStorage(EndpointStorageIdx, PartitionIdx), EndpointPtr)) /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT */
    {
      endpointMatch = TRUE;
    }
  }

  return endpointMatch;
}

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Process()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_Process(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfTriggerTransmitDynType triggerTransmitIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read all pending trigger transmit requests. */
  while(TRUE == Sd_TriggerTransmit_ReadFirst(PartitionIdx, &triggerTransmitIdx))                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */
  {
    /* #100 Check if the transmit shall be triggered now or during the next main function execution. */
    if(Sd_GetStateOfTriggerTransmitDyn(triggerTransmitIdx, PartitionIdx) == SD_TRIGGER_TRANSMIT_STATE_NOW)
    {
      /* #1000 Trigger transmission of initial events. */
      Sd_TriggerTransmit_Trigger(triggerTransmitIdx, PartitionIdx);

      /* #1001 Remove the trigger transmit request. */
      Sd_TriggerTransmit_RemoveFirst(PartitionIdx);
    }
    else  /* SD_TRIGGER_TRANSMIT_STATE_NOW != Sd_GetStateOfTriggerTransmit(triggerTransmitIdx) */
    {
      /* #1002 Postpone the transmission to the next main function execution. */
      Sd_SetStateOfTriggerTransmitDyn(triggerTransmitIdx, SD_TRIGGER_TRANSMIT_STATE_NOW, PartitionIdx);                 /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
      break;
    }
  }
}

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Trigger()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_Trigger(
  Sd_SizeOfTriggerTransmitDynType TriggerTransmitIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const Sd_SizeOfEventHandlerType eventHandlerIdx =
    Sd_GetEventHandlerIdxOfTriggerTransmitDyn(TriggerTransmitIdx, PartitionIdx);
  const Sd_SizeOfClientListType clientListIdx =
    Sd_GetClientListIdxOfTriggerTransmitDyn(TriggerTransmitIdx, PartitionIdx);
  const SoAd_SoConIdType tcpSoConId = Sd_GetAssignedTcpSoConIdOfClientList(clientListIdx, PartitionIdx);
  const SoAd_SoConIdType udpSoConId = Sd_GetAssignedUdpSoConIdOfClientList(clientListIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the event handler is configured for TCP. */
  if(tcpSoConId != SOAD_INV_SO_CON_ID)
  {
    /* #100 Check if the trigger transmission is configured for TCP. */
    const Sd_SizeOfEventHandlerTcpType eventHandlerTcpIdx =
      Sd_GetEventHandlerTcpIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
    if(Sd_IsRoutingGroupTriggerUsedOfEventHandlerTcp(eventHandlerTcpIdx, PartitionIdx))
    {
      /* #1000 Trigger the initial transmission for TCP. */
      (void)SoAd_IfSpecificRoutingGroupTransmit(Sd_GetRoutingGroup(Sd_GetRoutingGroupTriggerIdxOfEventHandlerTcp(
        eventHandlerTcpIdx, PartitionIdx), PartitionIdx), tcpSoConId);
    }
  }

  /* #20 Check if the event handler is configured for UDP. */
  if(udpSoConId != SOAD_INV_SO_CON_ID)
  {
    /* #200 Check if the trigger transmission is configured for UDP. */
    const Sd_SizeOfEventHandlerUdpType eventHandlerUdpIdx =
      Sd_GetEventHandlerUdpIdxOfEventHandler(eventHandlerIdx, PartitionIdx);
    if(Sd_IsRoutingGroupTriggerUsedOfEventHandlerUdp(eventHandlerUdpIdx, PartitionIdx))
    {
      /* #2000 Trigger the initial transmission for UDP. */
      (void)SoAd_IfSpecificRoutingGroupTransmit(Sd_GetRoutingGroup(Sd_GetRoutingGroupTriggerIdxOfEventHandlerUdp(
        eventHandlerUdpIdx, PartitionIdx), PartitionIdx), udpSoConId);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_Add()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_TriggerTransmit_Add(
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfRxEventType RxEventIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType requestSaved = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that the ring buffer exists and is not full. */
  if(Sd_HasTriggerTransmitDyn(PartitionIdx) &&
      !Sd_IsTriggerTransmitFullOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx))
  {
    /* #100 Save the requested trigger transmit. */
    Sd_SizeOfTriggerTransmitDynType triggerTransmitFreeIdx =
      Sd_GetTriggerTransmitDynEndIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx);
    Sd_SetClientListIdxOfTriggerTransmitDyn(triggerTransmitFreeIdx,                                                     /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
      Sd_GetClientListIdxOfRxEvent(RxEventIdx, PartitionIdx), PartitionIdx);
    Sd_SetEventHandlerIdxOfTriggerTransmitDyn(triggerTransmitFreeIdx,                                                   /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
      Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx, PartitionIdx), PartitionIdx);
    Sd_SetStateOfTriggerTransmitDyn(triggerTransmitFreeIdx, SD_TRIGGER_TRANSMIT_STATE_NEXT_CYCLE, PartitionIdx);        /* SBSW_SD_TRIGGER_TRANSMIT_IDX */

    Sd_IncTriggerTransmitDynEndIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx);                              /* SBSW_SD_PARTITION_DATA_CSL05  */
    requestSaved = E_OK;

    /* #101 Handle wrap around. */
    if(Sd_GetTriggerTransmitDynEndIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx) >=
         Sd_GetSizeOfTriggerTransmitDyn(PartitionIdx))
    {
      Sd_SetTriggerTransmitDynEndIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, 0u, PartitionIdx);                        /* SBSW_SD_PARTITION_DATA_CSL05  */
    }

    /* #102 Check if the ring buffer is full. */
    if(Sd_GetTriggerTransmitDynEndIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx) ==
         Sd_GetTriggerTransmitDynStartIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx))
    {
      Sd_SetTriggerTransmitFullOfPartitionDataDyn(SD_PARTITION_DATA_IDX, TRUE, PartitionIdx);                           /* SBSW_SD_PARTITION_DATA_CSL05  */
    }
  }
  else
  {
    /* #103 The ring buffer does not exist or is full. Not able to save the event. */
  }

  return requestSaved;
}

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_ReadFirst()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_TriggerTransmit_ReadFirst(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_SizeOfTriggerTransmitDynType) TriggerTransmitIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean eventRead = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that the ring buffer exists and contains an element. */
  if(Sd_HasTriggerTransmitDyn(PartitionIdx) &&
     ((Sd_GetTriggerTransmitDynStartIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx) !=
        Sd_GetTriggerTransmitDynEndIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx)) ||
      Sd_IsTriggerTransmitFullOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx)))
  {
    /* #100 Read the first element from of the buffer. */
    *TriggerTransmitIdxPtr = Sd_GetTriggerTransmitDynStartIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx);   /* SBSW_SD_POINTER_WRITE */
    eventRead = TRUE;
  }
  else
  {
    /* #101 The buffer does not exist or contains no elements. */
    *TriggerTransmitIdxPtr = Sd_GetSizeOfTriggerTransmitDyn(PartitionIdx);                                              /* SBSW_SD_POINTER_WRITE */
  }

  return eventRead;
}

/**********************************************************************************************************************
 *  Sd_TriggerTransmit_RemoveFirst()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_TriggerTransmit_RemoveFirst(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 The ring buffer will always contain at least one element. */
  Sd_SetStateOfTriggerTransmitDyn(                                                                                      /* SBSW_SD_TRIGGER_TRANSMIT_IDX */
    Sd_GetTriggerTransmitDynStartIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx),
    SD_TRIGGER_TRANSMIT_STATE_INVALID, PartitionIdx);

  /* #20 Remove the first element of the buffer. */
  Sd_IncTriggerTransmitDynStartIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx);                              /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #30 Handle wrap around. */
  if(Sd_GetTriggerTransmitDynStartIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx) >=
       Sd_GetSizeOfTriggerTransmitDyn(PartitionIdx))
  {
    Sd_SetTriggerTransmitDynStartIdxOfPartitionDataDyn(SD_PARTITION_DATA_IDX, 0u, PartitionIdx);                        /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }

  /* #40 Check if the full flag has to be removed. */
  if(Sd_IsTriggerTransmitFullOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx))
  {
    Sd_SetTriggerTransmitFullOfPartitionDataDyn(SD_PARTITION_DATA_IDX, FALSE, PartitionIdx);                            /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  CLIENT FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Sd_Init_ReInitializeClients()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeClients(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all clients to update and process the state machine. */
  for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx, PartitionIdx);
      clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx, PartitionIdx);
      clientIdx++)
  {
    /* #100 Reinitialize all consumed eventgroups of the client. */
    Sd_Init_ReInitializeConsumedEventgroups(clientIdx, PartitionIdx);

    /* #101 Mark all clients for state machine execution. */
    Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */

    /* #102 Disable routing for method socket connection. */
    if(Sd_IsMethodUsedOfClient(clientIdx, PartitionIdx))
    {
      (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfMethod(
        Sd_GetMethodIdxOfClient(clientIdx, PartitionIdx), PartitionIdx), PartitionIdx);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Init_ReInitializeConsumedEventgroups()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Init_ReInitializeConsumedEventgroups(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all eventgroups of the client, disable the routing and close multicast socket connections. */
  for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx++)
  {
    /* #100 Check if the consumed eventgroup is configured for multicast. */
    if(Sd_IsRoutingGroupMulticastActRefUsedOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx))
    {
      (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupMulticastActRefIdxOfConsumedEventgroup(
        consumedEventgroupIdx, PartitionIdx), PartitionIdx);

      /* #1000 Close all multicast socket connections of the client. */
      Sd_UtilClient_CloseAllMulticastSoCons(consumedEventgroupIdx, PartitionIdx, TRUE);
    }

    /* #101 Check if the consumed eventgroup is configured for TCP. */
    if(Sd_IsRoutingGroupTcpActRefUsedOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx))
    {
      /* #1010 Disable TCP routing. */
      (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupTcpActRefIdxOfConsumedEventgroup(
        consumedEventgroupIdx, PartitionIdx), PartitionIdx);
    }

    /* #102 Check if the consumed eventgroup is configured for UDP. */
    if(Sd_IsRoutingGroupUdpActRefUsedOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx))
    {
      /* #1020 Disable UDP routing. */
      (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupUdpActRefIdxOfConsumedEventgroup(
        consumedEventgroupIdx, PartitionIdx), PartitionIdx);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_Process()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_Sd_STCAL_6050 */
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_Process(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute client state machine. */
  Sd_StateClient_ProcessStateMachine(ClientIdx, PartitionIdx);

  /* #20 Run the state machine for consumed eventgroups of the client instance. */
  Sd_StateClient_ProcessCeg(ClientIdx, PartitionIdx);

  /* #30 Handle the retry subscription functionality. */
  Sd_StateClient_HandleRetrySubscription(ClientIdx, PartitionIdx);

  /* #40 Update the time for the next state machine run. */
  if(!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfClientDyn(ClientIdx, PartitionIdx)))
  {
    Sd_Util_SetNextStateMachineRun(instanceIdx, PartitionIdx, Sd_GetTimerTTLOfClientDyn(ClientIdx, PartitionIdx));
  }

  Sd_Util_SetNextStateMachineRun(instanceIdx, PartitionIdx, Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx));
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessStateMachine()
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
/* PRQA S 6050 1 */ /* MD_Sd_STCAL_6050 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessStateMachine(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute client state machine. */
  switch(Sd_GetStateMachineOfClientDyn(ClientIdx, PartitionIdx))
  {
    case SD_CLIENT_STATE_NONE:
      /* #100 Initial run of the state machine. */
      Sd_StateClient_ProcessInitial(ClientIdx, PartitionIdx);
      break;

    case SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN:
      /* 101 Not requested, not seen state machine. */
      Sd_StateClient_ProcessNotRequestedNotSeen(ClientIdx, PartitionIdx);
      break;

    case SD_CLIENT_STATE_NOT_REQUESTED_SEEN:
      /* 102 Not requested, seen state machine. */
      Sd_StateClient_ProcessNotRequestedSeen(ClientIdx, PartitionIdx);
      break;

    case SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY:
      /* #103 Not ready state machine. */
      Sd_StateClient_ProcessRequestedNotReady(ClientIdx, PartitionIdx);
      break;

    case SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT:
      /* #104 Searching initial wait state machine. */
      Sd_StateClient_ProcessSearchingInitial(ClientIdx, PartitionIdx);
      break;

    case SD_CLIENT_STATE_SEARCHING_REPETITION:
      /* #105 Searching repetition state machine. */
      Sd_StateClient_ProcessSearchingRepetition(ClientIdx, PartitionIdx);
      break;

    case SD_CLIENT_STATE_SERVICE_READY:
      /* #106 Ready state machine. */
      Sd_StateClient_ProcessReady(ClientIdx, PartitionIdx);
      break;

    case SD_CLIENT_STATE_STOPPED:
      /* #107 Stopped state machine. */
      Sd_StateClient_ProcessStopped(ClientIdx, PartitionIdx);
      break;

    default: /* COV_SD_MISRA */
      /* #108 Undefined state. */
      break;
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_SetStateReleased()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_SetStateReleased(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if sm state requires action because client has been released. */
  if((Sd_GetStateMachineOfClientDyn(ClientIdx, PartitionIdx) == SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY) ||
     (Sd_GetStateMachineOfClientDyn(ClientIdx, PartitionIdx) == SD_CLIENT_STATE_SERVICE_READY) ||
     (Sd_GetStateMachineOfClientDyn(ClientIdx, PartitionIdx) == SD_CLIENT_STATE_STOPPED))
  {
    /* #100 Enter the down phase and indicate the state. */
    Sd_SetCurrentStateOfClientDyn(ClientIdx, SD_CLIENT_SERVICE_DOWN, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    if(Sd_GetStateMachineOfClientDyn(ClientIdx, PartitionIdx) == SD_CLIENT_STATE_SERVICE_READY)
    {
      BswM_Sd_ClientServiceCurrentState(Sd_GetHandleIdOfClient(ClientIdx, PartitionIdx), SD_CLIENT_SERVICE_DOWN);
    }

    /* #101 Unconfigure data path. */
    Sd_StateClient_Unconfigure(ClientIdx, PartitionIdx, TRUE);
  }

  /* #20 Reset state of client service. */
  Sd_SetStateSwcOfClientDyn(ClientIdx, SD_CLIENT_SERVICE_RELEASED, PartitionIdx);                                       /* SBSW_SD_CSL03_CSL05 */
  Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN, TRUE);
  Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx);   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
}

/**********************************************************************************************************************
 *  Sd_StateClient_HandleRetrySubscription()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_HandleRetrySubscription(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the retry subscription feature is configured. */
  if(Sd_GetRetryMaxOfInstance(instanceIdx, PartitionIdx) != 0u)
  {
    /* #100 Check if the client is in the main phase and the retry subscription timer is expired. */
    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((Sd_GetStateMachineOfClientDyn(ClientIdx, PartitionIdx) == SD_CLIENT_STATE_SERVICE_READY) &&
      (TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx, Sd_GetRetryTimerOfClientDyn(ClientIdx, PartitionIdx), TRUE)))
    {
      /* #1000 Check if no eventgroups of the client perform subscription retries. */
      if(Sd_StateClient_CheckIfRetryActive(ClientIdx, PartitionIdx) == FALSE)
      {
        /* 10000 Cancel the retry timer. */
        Sd_Util_ResetTimerToInvalid(Sd_GetAddrRetryTimerOfClientDyn(ClientIdx, PartitionIdx));                          /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      }
      /* #1001 Otherwise, check if the maximal amount of retries is not yet reached. */
      else if(Sd_GetRetryCounterOfClientDyn(ClientIdx, PartitionIdx) <=
                Sd_GetRetryMaxOfInstance(instanceIdx, PartitionIdx))
      {
        /* #10010 Increment the retry subscription counter. */
        /* Overflow in case of RetryMax is configured to 0xFF (retry forever) is expected. */
        Sd_IncRetryCounterOfClientDyn(ClientIdx, PartitionIdx);                                                         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      }
      /* #1002 Otherwise, do not trigger additional retries. */
      else
      {
        /* #10020 Invalidate the retry subscription timer and report a DET runtime error. */
        Sd_Util_ResetTimerToInvalid(Sd_GetAddrRetryTimerOfClientDyn(ClientIdx, PartitionIdx));                          /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
#if(SD_RUNTIME_ERROR_REPORT == STD_ON)
        (void)Det_ReportRuntimeError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_HANDLERETRYSUBSCRIPTION, SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED);
#endif /* SD_RUNTIME_ERROR_REPORT == STD_ON */
      }
    }

    /* #101 Update the time for the next state machine run. */
    Sd_Util_SetNextStateMachineRun(instanceIdx, PartitionIdx, Sd_GetRetryTimerOfClientDyn(ClientIdx, PartitionIdx));
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_CheckIfRetryActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_CheckIfRetryActive(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;
  boolean retryActive = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check all eventgroups of the client service instance. */
  for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx++)
  {
    /* #100 Check if the consumed eventgroup performs subscription retries. */
    if((Sd_GetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) ==
          SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT) ||
       (Sd_GetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) ==
          SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER) ||
       (Sd_GetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) ==
          SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_RECONFIGURE))
    {
      retryActive = TRUE;
      break;
    }
  }

  return retryActive;
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessInitial()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessInitial(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initial run of the state machine. */
  if(Sd_GetStateSwcOfClientDyn(ClientIdx, PartitionIdx) != SD_CLIENT_SERVICE_REQUESTED)
  {
    /* #100 Update the state if the client is not requested. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN, FALSE);
  }
  else if((Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) != SD_INSTANCE_UP_AND_CONFIGURED) /* &&
    (Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_REQUESTED) */ )
  {
    /* #101 Update the state if the instance is not ready. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY, FALSE);
  }
  else  /* (Sd_GetStateSwcOfClientDyn(ClientIdx) == SD_CLIENT_SERVICE_REQUESTED) && */
        /* (Sd_GetStateOfInstanceDyn(instanceIdx) == SD_INSTANCE_UP_AND_CONFIGURED) */
  {
    /* #102 Otherwise, update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessNotRequestedNotSeen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessNotRequestedNotSeen(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 OfferService message received for the first time. Update the configuration. */
  if((Sd_GetEventMainOfClientDyn(ClientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    Sd_Util_SetTimerOffsetSecWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx),
      Sd_GetOfferedTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx);                                 /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_NOT_REQUESTED_SEEN, FALSE);

    /* #100 Reset the event. */
    Sd_SetEventMainOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE, PartitionIdx);                                  /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessNotRequestedSeen()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessNotRequestedSeen(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the remote server was already seen. Update the state. */
  /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
  if((Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) == SD_INSTANCE_DOWN) ||
    (TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx, Sd_GetTimerTTLOfClientDyn(ClientIdx, PartitionIdx), FALSE)) ||
    ((Sd_GetEventMainOfClientDyn(ClientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_STOP_OFFER)) /*lint !e655 */
  {
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_NOT_REQUESTED_NOT_SEEN, FALSE);
    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx); /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_SetCurrentStateOfClientDyn(ClientIdx, SD_CLIENT_SERVICE_DOWN, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    /* #100 Reset the event. */
    Sd_SetEventMainOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE, PartitionIdx);                                  /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
  /* #20 Otherwise, if an OfferService was received. */
  else if((Sd_GetEventMainOfClientDyn(ClientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    /* #200 Reset the timer and the event. */
    Sd_Util_SetTimerOffsetSecWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx),
      Sd_GetOfferedTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx);                                 /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_SetEventMainOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE, PartitionIdx);                                  /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
  /* #30 Otherwise, if the client is requested. */
  else if(Sd_GetStateSwcOfClientDyn(ClientIdx, PartitionIdx) == SD_CLIENT_SERVICE_REQUESTED)
  {
    /* #300 Client was set to requested: Configure the client for consumed methods. */
    if(Sd_StateClient_Configure(ClientIdx, PartitionIdx) == E_OK)
    {
      /* #3000 Enter the main phase and inform the BswM. */
      Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SERVICE_READY, FALSE);
      Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, PartitionIdx, SD_CLIENT_SERVICE_AVAILABLE);
    }
  }
  /* #40 Otherwise, do not advance state and set marker for state machine execution to stay in this state. */
  else
  {
    Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessRequestedNotReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessRequestedNotReady(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  if(Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) == SD_INSTANCE_UP_AND_CONFIGURED)
  {
    /* #10 Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessSearchingInitial()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessSearchingInitial(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientTimerType clientTimerIdx = Sd_GetClientTimerIdxOfClient(ClientIdx, PartitionIdx);
  Sd_CommonInitialDelayIdxOfClientType commonDelayIdx = Sd_GetCommonInitialDelayIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check state of the wait phase. */
  if(Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx)))
  {
    uint32 additionalDelay;

    /* #100 Get random delay if a common delay shall not be used or the common delay has not been determined yet. */
    if((commonDelayIdx == SD_NO_COMMONINITIALDELAYIDXOFCLIENT) ||
      (Sd_GetCommonInitialDelay(commonDelayIdx, PartitionIdx) == SD_INV_TIMER_VALUE_32BIT))
    {
      additionalDelay = Sd_Util_GetRandomNrInRange(
        Sd_GetInitialFindDelayMinOfClientTimer(clientTimerIdx, PartitionIdx),
        Sd_GetInitialFindDelayMaxOfClientTimer(clientTimerIdx, PartitionIdx));

      if(commonDelayIdx != SD_NO_COMMONINITIALDELAYIDXOFCLIENT)
      {
        Sd_SetCommonInitialDelay(commonDelayIdx, additionalDelay, PartitionIdx);                                        /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      }
    }
    /* #101 Common delay shall be used and it has been set already, use it. */
    else
    {
      additionalDelay = Sd_GetCommonInitialDelay(commonDelayIdx, PartitionIdx);
    }

    /* #102 Set Timer and mark client for execution of state machine. */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx), additionalDelay,
      Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx);                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
  else if(TRUE ==
    Sd_Util_CheckIfTimerExceeded(PartitionIdx, Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx), TRUE))
  {
    /* #103 Wait phase timer is exceeded. Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SEARCHING_REPETITION, TRUE);
    Sd_SetRepetitionRunOfClientDyn(ClientIdx, 0u, PartitionIdx);                                                        /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    /* #104 Send a FindService message. */
    Sd_Util_AddFindEntryToList(ClientIdx, PartitionIdx);

    /* #105 Reset common delay. */
    if(commonDelayIdx != SD_NO_COMMONINITIALDELAYIDXOFCLIENT)
    {
      Sd_SetCommonInitialDelay(commonDelayIdx, SD_INV_TIMER_VALUE_32BIT, PartitionIdx);                                 /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    }
  }
  else
  {
    /* #106 Wait phase timer is not exceeded. Wait for next execution. */
    Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }

  /* #20 Check conditions to leave the "Search for Service" block. */
  Sd_StateClient_ProcessLeaveSearching(ClientIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessSearchingRepetition()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessSearchingRepetition(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);
  Sd_SizeOfClientTimerType clientTimerIdx = Sd_GetClientTimerIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Mark client for execution of state machine. */
  Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Check if the repetition phase was entered the first time. */
  if((Sd_CheckIfTimerIsInvalid(Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx))) &&
     (Sd_GetInitialFindRepetitionsMaxOfClientTimer(clientTimerIdx, PartitionIdx) > 0u) &&
     (Sd_GetRepetitionRunOfClientDyn(ClientIdx, PartitionIdx) == 0u))
  {
    /* #200 Wind up the timer. */
    Sd_Util_SetTimerOffsetWithTrigger(
      Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx),
      (uint32)Sd_GetInitialFindRepetitionBaseDelayOfClientTimer(clientTimerIdx, PartitionIdx) - SD_MAIN_FCT_CYCLE_TIME_MS,
      instanceIdx, PartitionIdx);                                                                                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  /* #30 Succeeding runs of the repetition phase. */
  if((TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx,
        Sd_GetTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx), TRUE)) &&
     (Sd_GetRepetitionRunOfClientDyn(ClientIdx, PartitionIdx) <
        Sd_GetInitialFindRepetitionsMaxOfClientTimer(clientTimerIdx, PartitionIdx)))
  {
    /* #300 Send a FindService message if timer is expired. */
    Sd_Util_AddFindEntryToList(ClientIdx, PartitionIdx);
    Sd_IncRepetitionRunOfClientDyn(ClientIdx, PartitionIdx);                                                            /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    /* #301 Wind up the timer. */
    Sd_Util_SetTimerOffsetWithTrigger(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx),
      (uint32)((((uint32)(1u)) << Sd_GetRepetitionRunOfClientDyn(ClientIdx, PartitionIdx)) *
        Sd_GetInitialFindRepetitionBaseDelayOfClientTimer(clientTimerIdx, PartitionIdx)),
      instanceIdx, PartitionIdx);                                                                                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
  else if(Sd_GetRepetitionRunOfClientDyn(ClientIdx, PartitionIdx) ==
            Sd_GetInitialFindRepetitionsMaxOfClientTimer(clientTimerIdx, PartitionIdx))
  {
    /* #302 End the searching repetition phase, if the amount of repetitions is reached. */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx));                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_IncRepetitionRunOfClientDyn(ClientIdx, PartitionIdx);                                                            /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
  else
  {
    /* #303 Stay in repetition phase if timer is not yet expired. */
  }

  /* #40 Check conditions to leave the "Search for Service" block. */
  Sd_StateClient_ProcessLeaveSearching(ClientIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveSearching()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveSearching(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the remote server was seen. */
  if(!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfClientDyn(ClientIdx, PartitionIdx)))
  {
    /* #100 Reset the TTL. */
    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx); /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  /* #20 Check if the instance is no more active. */
  if(Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) != SD_INSTANCE_UP_AND_CONFIGURED)
  {
    Sd_ConsumedEventgroupIterType consumedEventgroupIdx;

    /* #200 Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY, TRUE);

    /* #201 Inform the BswM about the change of the client service instance and all associated eventgroups. */
    Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, PartitionIdx, SD_CLIENT_SERVICE_DOWN);

    for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx++)
    {
      BswM_Sd_ConsumedEventGroupCurrentState(
        Sd_GetHandleIdOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx), SD_CONSUMED_EVENTGROUP_DOWN);
    }
  }

  /* #40 Check if an OfferService message was received. */
  if((Sd_GetEventMainOfClientDyn(ClientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == /*lint !e655 */
        SD_CLIENT_EVENT_SERVICE_OFFER)
  {
    /* #400 Process the received OfferService message. */
    Sd_StateClient_ProcessLeaveSearchingRcvOffer(ClientIdx, PartitionIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessLeaveSearchingRcvOffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessLeaveSearchingRcvOffer(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the timer with the offered TTL. */
  Sd_Util_SetTimerOffsetSecWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx),
    Sd_GetOfferedTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx);                                   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

  /* #20 Configure the client according to the received OfferService message. */
  if(Sd_StateClient_Configure(ClientIdx, PartitionIdx) == E_OK)
  {
    /* #200 Enter the main phase and inform the BswM. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SERVICE_READY, TRUE);
    Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, PartitionIdx, SD_CLIENT_SERVICE_AVAILABLE);

    /* #201 The event is not reset because it is needed during processing of the eventgroups. */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessReady()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessReady(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Mark client for execution of state machine. */
  Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Check if an OfferService or StopOfferService message was received. */
  if((Sd_GetEventMainOfClientDyn(ClientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) != SD_CLIENT_EVENT_SERVICE_NONE) /*lint !e655 */
  {
    /* #200 Process the received message. */
    Sd_StateClient_ProcessReadyEventReceived(ClientIdx, PartitionIdx);
  }
  /* #30 Otherwise, check if the TTL timer is expired. */
  else if(TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx, Sd_GetTimerTTLOfClientDyn(ClientIdx, PartitionIdx), FALSE))
  {
    /* #300 The TTL of server expired, unconfigure all connections. */
    Sd_StateClient_Unconfigure(ClientIdx, PartitionIdx, TRUE);

    /* #301 Update the state and reset the TimeToTriggerSend. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SEARCHING_INITIAL_WAIT, TRUE);
    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx),
      Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx);                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

    /* #302 Report the status of the client service. */
    Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, PartitionIdx, SD_CLIENT_SERVICE_DOWN);
  }
  /* #40 Otherwise, nothing to do. */
  else
  {
    /* Wait till the Timer is expired. */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessReadyEventReceived()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessReadyEventReceived(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an OfferService or StopOfferService message was received. */
  if((Sd_GetEventMainOfClientDyn(ClientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    /* #100 Received an OfferService message. Configure the client. */
    if(Sd_StateClient_Configure(ClientIdx, PartitionIdx) == E_OK)
    {
      /* #1000 On success, update the timer. */
      Sd_Util_SetTimerOffsetSecWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx),
        Sd_GetOfferedTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx);                               /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

      /* #1001 The event is not reset because it is needed during processing of the eventgroups. */
    }
    else
    {
      /* #1002 If failed, the server has changed the attached endpoint addresses. */

      /* #1003 Send a FindService message. */
      Sd_Util_AddFindEntryToList(ClientIdx, PartitionIdx);

      /* #1004 Cancel the timer and reset the event. */
      Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_STOPPED, FALSE);
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx),
        instanceIdx, PartitionIdx);                                                                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_SetEventMainOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE, PartitionIdx);                                /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

      /* #1005 Report the status of the client service. */
      Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, PartitionIdx, SD_CLIENT_SERVICE_DOWN);
    }
  }
  else
  {
    /* #101 Received StopOfferService message. Cancel the timer and update state machine. */
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_STOPPED, FALSE);
    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx); /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

    /* #102 Report the status of the client service. */
    Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, PartitionIdx, SD_CLIENT_SERVICE_DOWN);

    Sd_StateClient_Unconfigure(ClientIdx, PartitionIdx, TRUE);

    /* #103 Reset the event. */
    Sd_SetEventMainOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE, PartitionIdx);                                  /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessStopped()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessStopped(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an OfferService message was received. */
  if((Sd_GetEventMainOfClientDyn(ClientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    /* #100 Update the timer. */
    Sd_Util_SetTimerOffsetSecWithTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx),
      Sd_GetOfferedTTLOfClientDyn(ClientIdx, PartitionIdx), instanceIdx, PartitionIdx);                                 /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

    if(Sd_StateClient_Configure(ClientIdx, PartitionIdx) == E_OK)
    {
      /* #1000 Enter the main phase and inform the BswM. */
      Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_SERVICE_READY, FALSE);
      Sd_UtilClient_SetCurrentStateInformBswm(ClientIdx, PartitionIdx, SD_CLIENT_SERVICE_AVAILABLE);
    }
  }

  /* #20 Change the state if the connection is lost. */
  if(Sd_GetStateOfInstanceDyn(instanceIdx, PartitionIdx) != SD_INSTANCE_UP_AND_CONFIGURED)
  {
    Sd_StateClient_Update(ClientIdx, PartitionIdx, SD_CLIENT_STATE_REQUESTED_BUT_NOT_READY, FALSE);
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCeg()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCeg(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle all eventgroups of the client service instance. */
  for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx++)
  {
    /* #100 Check if the service is ready. */
    if(Sd_GetStateMachineOfClientDyn(ClientIdx, PartitionIdx) == SD_CLIENT_STATE_SERVICE_READY)
    {
      /* #1000 Execute the state machine of the consumed eventgroup. */
      Sd_StateClient_ProcessCegState(consumedEventgroupIdx, PartitionIdx);

      /* #1001 Check if the TTL expired or the eventgroup was set from requested to released. */
      /*lint -e655 */
      if(((TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx, Sd_GetTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx), FALSE)) &&
          (!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx))) &&
          ((Sd_GetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) & SD_CONSUMED_EVENTGROUP_STATE_MASK) != SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED)) ||
        (((Sd_GetStateMachineOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) & SD_CONSUMED_EVENTGROUP_STATE_MASK) != SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED) &&
        (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) == SD_CONSUMED_EVENTGROUP_RELEASED)) ||
        (Sd_GetEventOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) == SD_CLIENT_EVENT_EVENTGROUP_NACK)) /*lint +e655 */
      {
        /* #10010 Unconfigure consumed eventgroup. */
        Sd_StateClient_ReleaseCeg(consumedEventgroupIdx, PartitionIdx);
      }
    }
    else
    {
      /* #1002 The service instance is not ready. Reset the state of the consumed eventgroup. */
      Sd_StateClient_ProcessCegReset(consumedEventgroupIdx, PartitionIdx);
    }

    if(!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx)))
    {
      /* #1003 Update the time for the next state machine run. */
      Sd_Util_SetNextStateMachineRun(Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx,
        Sd_GetTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx));
    }
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegReset(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(
    Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx), PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 The service instance is not ready. Reset the state of the eventgroup. Do not trigger the state machine execution. */
  Sd_SetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED,           /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    PartitionIdx);
  Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE, PartitionIdx);             /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Report the state of all consumed eventgroups to BswM if the service was changed to down. */
  if(!Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx)))
  {
    if(Sd_GetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SD_CONSUMED_EVENTGROUP_DOWN)
    {
      BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx),
        SD_CONSUMED_EVENTGROUP_DOWN);
      Sd_SetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN, PartitionIdx);      /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }

    Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx),
      instanceIdx, PartitionIdx);                                                                                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  /* #30 Clear the SoAd configuration of the eventgroup. */
  Sd_StateClient_UnconfigureCeg(ConsumedEventgroupIdx, PartitionIdx, FALSE, TRUE);
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegState()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegState(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute the state machine of the consumed eventgroup based on the current state. */
  switch(Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx))
  {
    case SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED:
    case SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED_AFTER_MULTI_OFFER_NACK:
      /* #100 Execute state machine if in state unsubscribed. */
      Sd_StateClient_ProcessCegUnsubscribed(ConsumedEventgroupIdx, PartitionIdx);
      break;

    case SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT:
    case SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER:
    case SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_RECONFIGURE:
      /* #101 Execute state machine if in state registration sent. */
      Sd_StateClient_ProcessCegRegistrationSent(ConsumedEventgroupIdx, PartitionIdx);
      break;

    case SD_CONSUMED_EVENTGROUP_STATE_SUBSCRIBED:
      /* #102 Execute state machine if in state subscribed. */
      Sd_StateClient_ProcessCegSubscribed(ConsumedEventgroupIdx, PartitionIdx);
      break;

    default: /* COV_SD_MISRA */
      /* #103 Undefined state. */
      break;
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegUnsubscribed()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegUnsubscribed(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx);
  Sd_SizeOfClientTimerType clientTimerIdx =
    Sd_GetClientTimerIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  uint32 randDelay;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the eventgroup is requested */
  /*lint -e655 */
  if((Sd_GetServiceModeRequestOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SD_CONSUMED_EVENTGROUP_REQUESTED) &&
    (((Sd_GetEventMainOfClientDyn(clientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) ||
    (Sd_IsTriggerSubscriptionOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx)))) /*lint +e655 */
  {
    retVal = Sd_StateClient_UpdateEventGroup(ConsumedEventgroupIdx, PartitionIdx);

    if(retVal == E_OK)
    {
      /* #100 Check if the OfferService message was transmitted by multicast or unicast.
       *      Messages received via multicast will be answered with an additional delay. */
      if(Sd_GetEventMainOfClientDyn(clientIdx, PartitionIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_MULTI)
      {
        /* Calculate a random delay after which the response message will be transmitted. */
        randDelay = Sd_Util_GetRandomNrInRangeForResponse(
          Sd_GetRequestResponseMinDelayOfClientTimer(clientTimerIdx, PartitionIdx),
          Sd_GetRequestResponseMaxDelayOfClientTimer(clientTimerIdx, PartitionIdx),
          FALSE);

        /* #1000 Check if the reception of this multicast OfferService entry should trigger the transmission of a
         *       StopSubscribeEventgroup-SubscribeEventgroup sequence. */
        if(Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) ==
             SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED_AFTER_MULTI_OFFER_NACK)
        {
          /* #10000 Add a StopSubscribeEventgroup entry to the TX queue. */
          /* Note: Added with delay to take into account time until SoCon is opened. */
          Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, PartitionIdx, FALSE, randDelay + SD_MAIN_FCT_CYCLE_TIME_MS);
        }

        Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx,
          SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER);
      }
      else
      {
        randDelay = 0u;
        Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx,
          SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT);
      }

      /* #101 Add a SubscribeEventgroup entry to the TX queue. */
      Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, PartitionIdx, TRUE, randDelay + SD_MAIN_FCT_CYCLE_TIME_MS);

      /* #102 Enable the unicast PDU routings in order to prevent the race condition discussed in RFC 68309. */
      if(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
      {
        (void)Sd_Util_EnableSpecificRouting(
          (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupTcpActRefIdxOfConsumedEventgroup(
            ConsumedEventgroupIdx, PartitionIdx),
          PartitionIdx,
          Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx));
      }
      if(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
      {
        (void)Sd_Util_EnableSpecificRouting(
          (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupUdpActRefIdxOfConsumedEventgroup(
            ConsumedEventgroupIdx, PartitionIdx),
          PartitionIdx,
          Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx));
      }

      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx),
        instanceIdx, PartitionIdx);                                                                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_SetTriggerSubscriptionOfConsumedEventgroupDyn(ConsumedEventgroupIdx, FALSE, PartitionIdx);                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSent()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSent(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an acknowledge was received. */
  if(Sd_GetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SD_CLIENT_EVENT_EVENTGROUP_ACK)
  {
    /* #100 Process the acknowledge. */
    Sd_StateClient_ProcessCegRegistrationSentRxAck(ConsumedEventgroupIdx, PartitionIdx);
  }
  /* #20 Otherwise, check if an OfferService was received.*/
  else if((Sd_GetEventMainOfClientDyn(clientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == /*lint !e655 */
    SD_CLIENT_EVENT_SERVICE_OFFER)
  {
    /* #200 Process the OfferService message. */
    if(Sd_StateClient_UpdateEventGroup(ConsumedEventgroupIdx, PartitionIdx) == E_OK)
    {
      Sd_StateClient_ProcessCegRegistrationSentRxOffer(ConsumedEventgroupIdx, PartitionIdx);
    }
  }
  /* #30 Otherwise, check if the subscription has to be retried. */
  /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
  else if((Sd_GetRetryCounterOfClientDyn(clientIdx, PartitionIdx) <=
             Sd_GetRetryMaxOfInstance(instanceIdx, PartitionIdx)) &&
          (Sd_Util_CheckIfTimerExceeded(PartitionIdx, Sd_GetRetryTimerOfClientDyn(clientIdx, PartitionIdx), TRUE) ==
             TRUE))
  {
    /* #300 Retry subscription by transmission of StopSubscribeEventgroup and SubscribeEventgroup message. */
    Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, PartitionIdx, FALSE, 0u);
    Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, PartitionIdx, TRUE, 0u);
  }
  /* #40 Otherwise, no relevant message received and no retry has to be triggered. */
  else
  {
    /* #400 Wait for response from server or till TTL or retry timer expires. */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSentRxAck()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSentRxAck(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 The server acknowledged the subscription, update the socket connections. */
  Std_ReturnType retVal = Sd_StateClient_UpdateEventGroupAckReceived(ConsumedEventgroupIdx, PartitionIdx);

  if(retVal == E_OK)
  {
    /* #100 Inform the BswM if the eventgroup was subscribed for the first time. */
    if(Sd_CheckIfTimerIsZero(Sd_GetTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx)))
    {
      /* Inform the BswM. */
      BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx),
        SD_CONSUMED_EVENTGROUP_AVAILABLE);
      Sd_SetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_AVAILABLE,                /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
        PartitionIdx);
    }

    /* #101 Start the timers, update the state and reset the event. */
    Sd_Util_SetTimerOffsetSecWithTrigger(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx),
      Sd_GetAcknowledgedTTLOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx),
      instanceIdx, PartitionIdx);                                                                                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx, SD_CONSUMED_EVENTGROUP_STATE_SUBSCRIBED);
    Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE, PartitionIdx);           /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
  else
  {
    /* #102 The multicast socket connection could not be configured. */
    if(Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) !=
         SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_RECONFIGURE)
    {
      /* #1020 If the multicast endpoint was changed, the Sd will try to configure it in the next cycle.
       *       The SD_CLIENT_EVENT_EVENTGROUP_ACK event is not removed. */
      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx,
        SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_RECONFIGURE);
    }
    else
    {
      /* #1021 Otherwise, inform the BswM, update the state and remove event. */
      BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx),
        SD_CONSUMED_EVENTGROUP_DOWN);
      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx, SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED);
      Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE, PartitionIdx);         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegRegistrationSentRxOffer()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegRegistrationSentRxOffer(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SizeOfClientTimerType clientTimerIdx =
    Sd_GetClientTimerIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  uint32 randDelay;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the previous subscribe message is already transmitted, or still pending because of a missing TCP connection. */
  if(TRUE == Sd_StateClient_EntryListContainsSubscribe(ConsumedEventgroupIdx, PartitionIdx))
  {
    /* #100 There is already a SubscribeEventgroup message pending.
     *      Do not trigger the StopSubscribeEventgroup - SubscribeEventgroup message. */
  }
  else
  {
    /* #101 Check if the OfferService message was transmitted by multicast or unicast. */
    if(Sd_GetEventMainOfClientDyn(clientIdx, PartitionIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_MULTI)
    {
      /* #1010 Add a random delay to the response message. */
      randDelay = Sd_Util_GetRandomNrInRangeForResponse(
        Sd_GetRequestResponseMinDelayOfClientTimer(clientTimerIdx, PartitionIdx),
        Sd_GetRequestResponseMaxDelayOfClientTimer(clientTimerIdx, PartitionIdx), FALSE);

      if(Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) ==
           SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER)
      {
        /* #10100 Send out a StopSubscribeEventgroup followed by a SubscribeEventgroup message. */
        Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, PartitionIdx, FALSE, randDelay);
      }

      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx,
        SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER);
    }
    else  /* Sd_GetEventMainOfClientDyn(clientIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_UNI */
    {
      /* #1011 The response can be transmitted without additional delay. */
      randDelay = 0u;
      Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx,
        SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT);
    }

    /* #102 Send out a SubscribeEventgroup message. */
    Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, PartitionIdx, TRUE, randDelay);
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_ProcessCegSubscribed()
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
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ProcessCegSubscribed(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SizeOfClientTimerType clientTimerIdx =
    Sd_GetClientTimerIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  uint32 randDelay;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If an OfferService message was received. */
  if((Sd_GetEventMainOfClientDyn(clientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    /* #100 Update the consumed eventgroup based on the OfferService. */
    retVal = Sd_StateClient_UpdateEventGroup(ConsumedEventgroupIdx, PartitionIdx);

    if(retVal == E_OK)
    {
      /* #1000 If the OfferService message was transmitted by multicast. */
      if(Sd_GetEventMainOfClientDyn(clientIdx, PartitionIdx) == SD_CLIENT_EVENT_SERVICE_OFFER_MULTI)
      {
        /* #10000 Calculate a random delay after which the response message will be transmitted. */
        randDelay = Sd_Util_GetRandomNrInRangeForResponse(
          Sd_GetRequestResponseMinDelayOfClientTimer(clientTimerIdx, PartitionIdx),
          Sd_GetRequestResponseMaxDelayOfClientTimer(clientTimerIdx, PartitionIdx), FALSE);

        /* #10001 Update the state of the consumed eventgroup. */
        Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx,
          SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER);
      }
      /* #2000 Otherwise, if the OfferService was transmitted via unicast.*/
      else
      {
        /* #20000 Do not add a random delay to the response message. */
        randDelay = 0u;

        /* #20001 Update the state of the consumed eventgroup. */
        Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx, SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT);
      }

      /* #300 Send a subscribe eventgroup message and init the mechanism waiting for the ACK. */
      Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, PartitionIdx, TRUE, randDelay);
    }
    else
    {
      Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
    }
  }
  /* #40 Otherwise, do not perform any actions. */
  else
  {
    /* Timer is not yet expired. */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_Configure()
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
 */
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_Configure(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);
  Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client configures a consumed method using TCP. */
  if((methodIdx != SD_NO_METHODIDXOFCLIENT) && Sd_IsTcpConfiguredOfMethod(methodIdx, PartitionIdx))
  {
    /* #100 Configure the TCP socket connection. */
    retVal = Sd_StateClient_ConfigureMethod(ClientIdx, PartitionIdx,
      Sd_GetAddrMethodTcpSoConIdOfClientDyn(ClientIdx, PartitionIdx),
      Sd_GetTcpSoConId(Sd_GetTcpSoConIdStartIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx),
      Sd_GetAddrServerTcpEndpointAddrOfClientDyn(ClientIdx, PartitionIdx));                                             /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  /* #20 Check if the client configures a consumed method using UDP. */
  if((methodIdx != SD_NO_METHODIDXOFCLIENT) && Sd_IsUdpConfiguredOfMethod(methodIdx, PartitionIdx))
  {
    /* #200 Configure the UDP socket connection. */
    retVal = Sd_StateClient_ConfigureMethod(ClientIdx, PartitionIdx,
      Sd_GetAddrMethodUdpSoConIdOfClientDyn(ClientIdx, PartitionIdx),
      Sd_GetIdOfUdpSoCon(Sd_GetUdpSoConStartIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx),
      Sd_GetAddrServerUdpEndpointAddrOfClientDyn(ClientIdx, PartitionIdx));                                             /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  /* #30 Check if the consumed method configuration has failed. */
  if(retVal != E_OK)
  {
    /* #300 Unconfigure the client and trigger execution of MainFunction in order to perform retry. */
    Sd_StateClient_Unconfigure(ClientIdx, PartitionIdx, FALSE);
    Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    Sd_Util_TriggerStateMachineRun(instanceIdx, PartitionIdx);
  }
  /* #40 Otherwise, check if the client is configured for subscription retry. */
  else if((Sd_GetOfferedTTLOfClientDyn(ClientIdx, PartitionIdx) == SD_TTL_INFINITE) &&
          (Sd_GetRetryMaxOfInstance(instanceIdx, PartitionIdx) != 0u))
  {
    /* #400 Reset the retry subscription counter. */
    Sd_SetRetryCounterOfClientDyn(ClientIdx, 1u, PartitionIdx);                                                         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
  /* #50 Otherwise, do not use the retry subscription functionality. */
  else
  {
    /* #500 Reset retry subscription counter and timer values. */
    Sd_SetRetryCounterOfClientDyn(ClientIdx, 0u, PartitionIdx);                                                         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrRetryTimerOfClientDyn(ClientIdx, PartitionIdx));                              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_ConfigureMethod()
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
/* PRQA S 6050 1 */ /* MD_Sd_STCAL_6050 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_ConfigureMethod(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(SoAd_SoConIdType) MethodSoConIdPtr,
  SoAd_SoConIdType ServiceStartSoConId,
  SD_P2CONST(Sd_SockAddrContainerType) ServerEndpointAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx, PartitionIdx);
  Sd_SockAddrContainerType remoteAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client is already configured for consumed methods. */
  if(*MethodSoConIdPtr == SOAD_INV_SO_CON_ID)
  {
    /* #100 Configure a socket connection with the corresponding remote address. */
    /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
    /* PRQA S 0311 2 */ /* MD_Sd_11.5_0311 */
    retVal = SoAd_SetUniqueRemoteAddr(ServiceStartSoConId,
      (SD_P2VAR(SoAd_SockAddrType))ServerEndpointAddrPtr, MethodSoConIdPtr);                                            /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_SOCK_ADDR_CAST */ /* SBSW_SD_CAST_TO_VAR */

    if((retVal == E_OK) && (*MethodSoConIdPtr != SOAD_INV_SO_CON_ID))
    {
      /* #1000 Each entity which requires the socket connection opens it in order to indicate the dependency. */
      retVal = SoAd_OpenSoCon(*MethodSoConIdPtr);
      retVal |= Sd_Util_EnableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfMethod(methodIdx, PartitionIdx),
        PartitionIdx,
        *MethodSoConIdPtr);
    }
    else
    {
      *MethodSoConIdPtr = SOAD_INV_SO_CON_ID;                                                                           /* SBSW_SD_POINTER_WRITE */
    }
  }
  else
  {
    /* #101 The socket connection is already configured. Check if the server endpoint address has changed. */

    retVal = Sd_SoAd_GetRemoteAddr(*MethodSoConIdPtr, &remoteAddr);                                                     /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 1 */ /* MD_Sd_12.4_3415 */
    if((retVal != E_OK) || (FALSE == Sd_Util_SockAddrIsEqual(ServerEndpointAddrPtr, &remoteAddr)))                      /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #1010 The attached endpoint has changed. Close the currently configured connection and mark the service as not available. */
      (void)SoAd_CloseSoCon(*MethodSoConIdPtr, FALSE);
      (void)Sd_Util_DisableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfMethod(methodIdx, PartitionIdx),
        PartitionIdx,
        *MethodSoConIdPtr);
      *MethodSoConIdPtr = SOAD_INV_SO_CON_ID;                                                                           /* SBSW_SD_POINTER_WRITE */
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx),
        Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx);                                              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_Unconfigure()
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
/* PRQA S 6010 1 */ /* MD_Sd_STPTH_6010 */
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_Unconfigure(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean ResetEndpointInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean abortFlag = FALSE, tmpResetEndpointInfo = ResetEndpointInfo;
  Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;
  Sd_SizeOfMethodType methodIdx = Sd_GetMethodIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();
  /* #10 Check if an Offer entry was received since remote server has been requested for unconfiguration. */
  if((Sd_GetEventRxOfClientDyn(ClientIdx, PartitionIdx) & SD_CLIENT_EVENT_TYPE_MASK) == SD_CLIENT_EVENT_SERVICE_OFFER) /*lint !e655 */
  {
    const Sd_SizeOfAddressStorageType addrStorageIdx = Sd_GetAddressStorageServerIdxOfClientDyn(ClientIdx, PartitionIdx);

    /* #100 Endpoint addresses of remote server were overwritten in rx context, do not invalidate! */
    tmpResetEndpointInfo = FALSE;

    /* #101 Abort existing connections if remote server has rebooted. */
    if(Sd_IsRebootDetectedOfAddressStorage(addrStorageIdx, PartitionIdx))
    {
      abortFlag = TRUE;
    }
  }

  /* #20 Remove the saved endpoint addresses if required. */
  if(tmpResetEndpointInfo == TRUE)
  {
    (Sd_GetAddrServerTcpEndpointAddrOfClientDyn(ClientIdx, PartitionIdx))->family = SOAD_AF_INVALID;                    /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    (Sd_GetAddrServerUdpEndpointAddrOfClientDyn(ClientIdx, PartitionIdx))->family = SOAD_AF_INVALID;                    /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }
  SD_END_CRITICAL_SECTION();

  /* #30 Reset retry subscription counter and timer values. */
  Sd_SetRetryCounterOfClientDyn(ClientIdx, 0u, PartitionIdx);                                                           /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_Util_ResetTimerToInvalid(Sd_GetAddrRetryTimerOfClientDyn(ClientIdx, PartitionIdx));                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

  /* #40 Iterate over all eventgroups, disable the routing groups and close the socket connections. */
  for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx++)
  {
    Sd_StateClient_UnconfigureCeg(consumedEventgroupIdx, PartitionIdx, abortFlag, tmpResetEndpointInfo);
  }

  /* #50 Disable the SoAd routing groups for methods and close the socket connections. */
  if((methodIdx != SD_NO_METHODIDXOFCLIENT) &&
    ((Sd_IsTcpConfiguredOfMethod(methodIdx, PartitionIdx)) || (Sd_IsUdpConfiguredOfMethod(methodIdx, PartitionIdx))))
  {
    (void)Sd_Util_DisableRouting(
      (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupActRefIdxOfMethod(methodIdx, PartitionIdx), PartitionIdx);

    if(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
    {
      /* #500 Unconfigure TCP for consumed methods. */
      (void)SoAd_CloseSoCon(Sd_GetMethodTcpSoConIdOfClientDyn(ClientIdx, PartitionIdx), abortFlag);
      Sd_SetMethodTcpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }

    if(Sd_GetMethodUdpSoConIdOfClientDyn(ClientIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
    {
      /* #501 Unconfigure UDP for consumed methods. */
      (void)SoAd_CloseSoCon(Sd_GetMethodUdpSoConIdOfClientDyn(ClientIdx, PartitionIdx), abortFlag);
      Sd_SetMethodUdpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroup()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroup(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the consumed eventgroup is configured for TCP. */
  if(Sd_ConsumedEventGroupUsesTcp(ConsumedEventgroupIdx, PartitionIdx))
  {
    /* #100 Update the TCP configuration. */
    retVal = Sd_StateClient_UpdateEventGroupTcp(ConsumedEventgroupIdx, PartitionIdx);
  }

  /* #20 Check if the consumed eventgroup is configured for UDP. */
  if(Sd_ConsumedEventGroupUsesUdp(ConsumedEventgroupIdx, PartitionIdx))
  {
    /* #200 Update the UDP configuration. */
    retVal |= Sd_StateClient_UpdateEventGroupUdp(ConsumedEventgroupIdx, PartitionIdx);
  }

#if(SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON == STD_ON)
  /* #30 Check the configured remote address of the multicast socket connection if the consumed eventgroup is not
   *     configured for UDP unicast, but multicast only. */
  if((!Sd_ConsumedEventGroupUsesUdp(ConsumedEventgroupIdx, PartitionIdx)) &&
        Sd_ConsumedEventGroupUsesMulticast(ConsumedEventgroupIdx, PartitionIdx))
  {
    retVal |= Sd_StateClient_CheckMulticastSoConRemoteAddress(ConsumedEventgroupIdx, PartitionIdx);
  }
#endif

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupTcp()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupTcp(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  SoAd_SoConIdType soConId;
  SoAd_SoConIdType assignedSoConId = SOAD_INV_SO_CON_ID;
  Sd_SockAddrContainerType remoteAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the endpoint address has changed. */
  soConId = Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx);
  if(soConId != SOAD_INV_SO_CON_ID)
  {
    retVal = Sd_SoAd_GetRemoteAddr(soConId, &remoteAddr);                                                               /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) && (Sd_Util_SockAddrIsEqual(Sd_GetAddrServerTcpEndpointAddrOfClientDyn(clientIdx, PartitionIdx),
       &remoteAddr) == FALSE))                                                                                          /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #1000 The attached endpoint has changed. Close the currently configured connection and mark the service as not available. */
      (void)SoAd_CloseSoCon(soConId, FALSE);
      Sd_SetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                 /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(clientIdx, PartitionIdx),
        Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx), PartitionIdx);                                              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
      retVal = E_NOT_OK;
    }
  }
  else
  {
    /* #101 Otherwise, try to establish a connection to the server. */
    /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_SetUniqueRemoteAddr(
      Sd_GetTcpSoConId(Sd_GetTcpSoConIdStartIdxOfClient(clientIdx, PartitionIdx), PartitionIdx),
      (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrServerTcpEndpointAddrOfClientDyn(clientIdx, PartitionIdx),
      &assignedSoConId);                                                                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */ /* SBSW_SD_SOCK_ADDR_CAST */

    if(retVal == E_OK)
    {
      /* #1010 The requested IP address could be set or is already set. */

      /* #1011 Each EventGroup which requires the socket connection opens it in order to indicate the dependency. */
      retVal = SoAd_OpenSoCon(assignedSoConId);

      /* #1012 Save the socket connection ID which is used for this EventGroup. */
      Sd_SetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, assignedSoConId, PartitionIdx);                    /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }

  /* #20 Check if the server endpoint address could be saved. */
  if(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SOAD_INV_SO_CON_ID)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupUdp()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupUdp(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  SoAd_SoConIdType soConId;
  SoAd_SoConIdType assignedSoConId = SOAD_INV_SO_CON_ID;
  Sd_SockAddrContainerType remoteAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the endpoint address has changed. */
  soConId = Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx);
  if(soConId != SOAD_INV_SO_CON_ID)
  {
    retVal = Sd_SoAd_GetRemoteAddr(soConId, &remoteAddr);                                                               /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&(Sd_Util_SockAddrIsEqual(Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx, PartitionIdx),
        &remoteAddr) == FALSE))                                                                                         /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #1000 The attached endpoint has changed.
       *       Close the currently configured connection and mark the service as not available. */
      (void)SoAd_CloseSoCon(soConId, FALSE);
      Sd_SetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                 /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(clientIdx, PartitionIdx),
        Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx), PartitionIdx);                                              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      retVal = E_NOT_OK;
    }
  }
  else
  {
    /* #101 Otherwise, check if a socket connection is already configured with the correct server IP address or configure a new one. */
    /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
    retVal = SoAd_SetUniqueRemoteAddr(
      Sd_GetIdOfUdpSoCon(Sd_GetUdpSoConStartIdxOfClient(clientIdx, PartitionIdx), PartitionIdx),
      (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx, PartitionIdx),
      &assignedSoConId);                                                                                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */ /* SBSW_SD_SOCK_ADDR_CAST */

    if(retVal == E_OK)
    {
      /* #1010 The requested IP address could be set or is already set. */

      /* #1011 Each EventGroup which requires the socket connection opens it in order to indicate the dependency. */
      retVal = SoAd_OpenSoCon(assignedSoConId);

      /* #1012 Save the socket connection ID which is used for this EventGroup. */
      Sd_SetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, assignedSoConId, PartitionIdx);                    /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }

  /* #20 Check if the server endpoint address could be saved. */
  if(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SOAD_INV_SO_CON_ID)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

#if(SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON == STD_ON)
/**********************************************************************************************************************
 *  Sd_StateClient_CheckMulticastSoConRemoteAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_CheckMulticastSoConRemoteAddress(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  SoAd_SoConIdType soConId = Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SockAddrContainerType soadAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the multicast socket connection is configured. */
  if(soConId != SOAD_INV_SO_CON_ID)
  {
    /* #100 Check if the offered UDP unicast endpoint has changed. */
    const Sd_SockAddrContainerType* storedAddrPtr = Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx, PartitionIdx);  /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
    retVal = Sd_SoAd_GetRemoteAddr(soConId, &soadAddr);                                                                   /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* PRQA S 3415 1 */ /* MD_Sd_12.4_3415 */
    if((retVal != E_OK) || (Sd_Util_SockAddrIsEqual(storedAddrPtr, &soadAddr) == FALSE))                                  /* SBSW_SD_REFERENCE_OF_VARIABLE */
    {
      /* #1000 The remote endpoint address has changed, reset the configuration. */
      (void)Sd_StateClient_ReleaseIpAddrAssignment(PartitionIdx, soConId);
      (void)Sd_Util_DisableSpecificRouting(
        (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupMulticastActRefIdxOfConsumedEventgroup(
          ConsumedEventgroupIdx, PartitionIdx),
        PartitionIdx,
        soConId);
      (void)SoAd_CloseSoCon(soConId, FALSE);
      Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);           /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      Sd_Util_ResetTimerToZeroTrigger(Sd_GetAddrTimerTTLOfClientDyn(clientIdx, PartitionIdx),
        Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx), PartitionIdx);                                              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

      retVal = E_NOT_OK;
    }
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceived()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceived(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the multicast configuration. */
  if(Sd_ConsumedEventGroupUsesMulticast(ConsumedEventgroupIdx, PartitionIdx))
  {
    /* #100 Check if the client is already configured for multicast reception. */
    if(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
    {
      /* #1000 If yes: Check the the new endpoint option matches the current configuration. */
      retVal = Sd_StateClient_UpdateEventGroupAckReceivedRenew(ConsumedEventgroupIdx, PartitionIdx);
    }
    else
    {
      /* #1001 Otherwise, configure consumed eventgroup for multicast. */
      retVal = Sd_StateClient_UpdateEventGroupAckReceivedInitial(ConsumedEventgroupIdx, PartitionIdx);
    }

    if(retVal == E_OK)
    {
      /* #1002 Enable the multicast PDU routing. */
      if(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
      {
        retVal = Sd_Util_EnableSpecificRouting(
          (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupMulticastActRefIdxOfConsumedEventgroup(
            ConsumedEventgroupIdx, PartitionIdx),
          PartitionIdx,
          Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx));
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceivedInitial()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceivedInitial(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfMulticastSoConIdType multicastSoConIdIdx;
  Sd_LocalRemoteMatchTypeType matchType;
  SD_P2VAR(Sd_SockAddrContainerType) multicastEndpointAddrPtr = Sd_GetAddrMulticastEndpointAddr(
    Sd_GetMulticastEndpointAddrIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx), PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check all available socket connections and find the one which matches best to the server. */
  retVal = Sd_StateClient_FindBestMatchLocalRemoteSoCon(ConsumedEventgroupIdx, PartitionIdx, &multicastSoConIdIdx,
    &matchType);                                                                                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    /* #100 Configure the local address according to the server multicast endpoint. */
    if((matchType.LocalIpMatchType != SD_MATCH_TRUE) || (matchType.LocalPortMatchType != SD_MATCH_TRUE))
    {
      /* #1000 Check if the local IP address is already configured. */
      /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
      if((matchType.LocalIpMatchType != SD_MATCH_TRUE) &&
        (TRUE == Sd_StateClient_LocalIpAlreadyConfigured(PartitionIdx, multicastEndpointAddrPtr)))                      /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
      {
        /* #10000 The requested multicast IP address is already configured at another socket. */
        retVal = E_NOT_OK;
      }
      else
      {
        /* #10001 Otherwise, request the local address according to the endpoint. */
        retVal = Sd_StateClient_RequestIpAddrAssignment(PartitionIdx,
          Sd_GetMulticastSoConId(multicastSoConIdIdx, PartitionIdx), multicastEndpointAddrPtr, TRUE);                   /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
      }
    }
    else
    {
      /* #1001 The local address is already configured. Increment the request counter. */
      retVal = Sd_StateClient_RequestIpAddrAssignment(PartitionIdx,
        Sd_GetMulticastSoConId(multicastSoConIdIdx, PartitionIdx), multicastEndpointAddrPtr, FALSE);                    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
    }

    if(retVal == E_OK)
    {
#if(SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON == STD_ON)
      /* #1002 Check if the remote address has to be configured. */
      if((matchType.RemoteIpMatchType != SD_MATCH_TRUE) || (matchType.RemotePortMatchType != SD_MATCH_TRUE))
      {
        Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);

        /* #10020 Configure the remote address according to the server unicast endpoint. */
        /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
        retVal = SoAd_SetRemoteAddr(Sd_GetMulticastSoConId(multicastSoConIdIdx, PartitionIdx),
          (SD_P2VAR(SoAd_SockAddrType))Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx, PartitionIdx));            /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */ /* SBSW_SD_SOCK_ADDR_CAST */
      }
#endif

      /* #1003 Open the socket connection. Each EventGroup which requires the socket connection opens it in order to indicate the dependency. */
      /* PRQA S 2986 1 */ /* MD_Sd_Rule-2.2_2986 */
      retVal |= SoAd_OpenSoCon(Sd_GetMulticastSoConId(multicastSoConIdIdx, PartitionIdx));

      /* #1004 Save the socket connection ID which is used for this EventGroup. */
      Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx,                                              /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
        Sd_GetMulticastSoConId(multicastSoConIdIdx, PartitionIdx), PartitionIdx);
    }
  }

  /* #20 Check if the server endpoint address could be saved. */
  if(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SOAD_INV_SO_CON_ID)
  {
    retVal = E_NOT_OK;

    /* #200 Accept the acknowledge if no multicast endpoint was transmitted, but the unicast configuration was successful. */
    if((multicastEndpointAddrPtr->family == SOAD_AF_INVALID) &&
      (Sd_ConsumedEventGroupUsesUdp(ConsumedEventgroupIdx, PartitionIdx) &&
      (Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)))
    {
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateEventGroupAckReceivedRenew()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_UpdateEventGroupAckReceivedRenew(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  SoAd_SoConIdType soConId = Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  Sd_SockAddrContainerType localAddr;

  /* #10 Get the local address. */
  retVal = Sd_SoAd_GetLocalAddr(soConId, &localAddr);                                                                   /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #20 Check if the local endpoint address has changed. */
  /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
  if((retVal == E_OK) &&
     (FALSE == Sd_Util_SockAddrIsEqual(Sd_GetAddrMulticastEndpointAddr(
       Sd_GetMulticastEndpointAddrIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx), PartitionIdx),
       &localAddr)))                                                                                                    /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */
  {
    /* #200 The attached endpoint has changed. Unconfigure the local address. */
    (void)Sd_StateClient_ReleaseIpAddrAssignment(PartitionIdx, soConId);

    (void)Sd_Util_DisableSpecificRouting(
      (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupMulticastActRefIdxOfConsumedEventgroup(
        ConsumedEventgroupIdx, PartitionIdx),
      PartitionIdx,
      soConId);
    (void)SoAd_CloseSoCon(soConId, FALSE);
    Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);             /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoCon()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoCon(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_SizeOfMulticastSoConIdType) MulticastSoConIdPtr,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) MatchTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfMulticastSoConIdType tmpMulticastSoConIdIdx, bestMulticastSoConIdIdx =
    Sd_GetSizeOfMulticastSoConId(PartitionIdx);
  uint8 localMatchPrio = 0u, remoteMatchPrio = 0u;
  Sd_LocalRemoteMatchTypeType bestMatchType;

  /* ----- Implementation ----------------------------------------------- */
  MatchTypePtr->LocalIpMatchType = SD_MATCH_FALSE;                                                                      /* SBSW_SD_POINTER_WRITE */
  MatchTypePtr->LocalPortMatchType = SD_MATCH_FALSE;                                                                    /* SBSW_SD_POINTER_WRITE */
  MatchTypePtr->RemoteIpMatchType = SD_MATCH_FALSE;                                                                     /* SBSW_SD_POINTER_WRITE */
  MatchTypePtr->RemotePortMatchType = SD_MATCH_FALSE;                                                                   /* SBSW_SD_POINTER_WRITE */
  bestMatchType = *MatchTypePtr;

  /* #10 Calculate the match priority for all configured multicast socket connections. */
  for(tmpMulticastSoConIdIdx = Sd_GetMulticastSoConIdStartIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
    tmpMulticastSoConIdIdx < Sd_GetMulticastSoConIdEndIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
    tmpMulticastSoConIdIdx++)
  {
    /* #100 Calculate the match type of the local address. */
    retVal = Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType(
      ConsumedEventgroupIdx, PartitionIdx, tmpMulticastSoConIdIdx, MatchTypePtr);                                       /* SBSW_SD_PASSING_API_POINTER */

    /* #101 Calculate the match type of the remote address. */
    retVal |= Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType(
      ConsumedEventgroupIdx, PartitionIdx, tmpMulticastSoConIdIdx, MatchTypePtr);                                       /* SBSW_SD_PASSING_API_POINTER */

    /* #102 Check if the socket connection can be (re)configured to the expected values. */
    if((retVal == E_OK) &&
      (MatchTypePtr->LocalIpMatchType != SD_MATCH_FALSE) && (MatchTypePtr->LocalPortMatchType != SD_MATCH_FALSE) &&
      (MatchTypePtr->RemoteIpMatchType != SD_MATCH_FALSE) && (MatchTypePtr->RemotePortMatchType != SD_MATCH_FALSE))
    {
      /* #1020 The socket connection can be (re)configured. Check the priority of the local and remote match. */
      /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
      if((Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio(MatchTypePtr, &localMatchPrio, &remoteMatchPrio) == TRUE) &&
        (Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio(MatchTypePtr, &remoteMatchPrio) == TRUE))      /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      {
        /* #10200 Found better matching socket connection. */
        bestMulticastSoConIdIdx = tmpMulticastSoConIdIdx;
        bestMatchType = *MatchTypePtr;
      }
    }
  }

  /* #20 Check if a feasible socket connection was found. */
  if((localMatchPrio != 0u) && (remoteMatchPrio != 0u))
  {
    /* #200 Found a feasible socket connection, update values. */
    *MulticastSoConIdPtr = bestMulticastSoConIdIdx;                                                                     /* SBSW_SD_POINTER_WRITE */
    *MatchTypePtr = bestMatchType;                                                                                      /* SBSW_SD_POINTER_WRITE */
    retVal = E_OK;
  }
  else
  {
    /* #201 No socket connection found which matches the given IP address. */
    *MulticastSoConIdPtr = Sd_GetSizeOfMulticastSoConId(PartitionIdx);                                                  /* SBSW_SD_POINTER_WRITE */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(uint8, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState(
  SD_P2CONST(Sd_SockAddrContainerType) AddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 addrState;

  /* #10 Calculate the state of the port. */
  if(AddrPtr->port == SOAD_PORT_ANY)
  {
    addrState = SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_PORT;
  }
  else
  {
    addrState = SOAD_SOCON_IP_SET_PORT_SET & SOAD_SOCON_MASK_PORT;
  }

  /* #20 Calculate the state of the address. */
  if(AddrPtr->family == SOAD_AF_INET)
  {
    if(AddrPtr->data[0u] == SOAD_IPADDR_ANY)
    {
      addrState = (uint8)(addrState | (SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_IPADDR));
    }
    else
    {
      addrState = (uint8)(addrState | (SOAD_SOCON_IP_SET_PORT_SET & SOAD_SOCON_MASK_IPADDR));
    }
  }
  else /* AddrPtr->family == SOAD_AF_INET6) */
  {
    if((AddrPtr->data[0u] == SOAD_IPADDR_ANY) && (AddrPtr->data[1u] == SOAD_IPADDR_ANY) &&
      (AddrPtr->data[2u] == SOAD_IPADDR_ANY) && (AddrPtr->data[3u] == SOAD_IPADDR_ANY))
    {
      addrState = (uint8)(addrState | (SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_IPADDR));
    }
    else
    {
      addrState = (uint8)(addrState | (SOAD_SOCON_IP_SET_PORT_SET & SOAD_SOCON_MASK_IPADDR));
    }
  }

  return addrState;
}

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchType(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfMulticastSoConIdType MulticastSoConIdIdx,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) LocalMatchTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SockAddrContainerType localAddr;
  uint8 localAddrState;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the configured local IP address. */
  retVal = Sd_SoAd_GetLocalAddr(Sd_GetMulticastSoConId(MulticastSoConIdIdx, PartitionIdx), &localAddr);                 /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    SD_P2CONST(Sd_SockAddrContainerType) multicastEndpointAddrPtr = Sd_GetAddrMulticastEndpointAddr(
      Sd_GetMulticastEndpointAddrIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx), PartitionIdx);

    /* #100 Calculate the state of the local address. */
    localAddrState = Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState(&localAddr);                             /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Compare the local address with the configured value and calculate a match type. */
    LocalMatchTypePtr->LocalIpMatchType = Sd_Util_CompareSockAddrIp(
      multicastEndpointAddrPtr, &localAddr, localAddrState);                                                            /* SBSW_SD_POINTER_WRITE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    LocalMatchTypePtr->LocalPortMatchType = Sd_Util_CompareSockAddrPort(
      multicastEndpointAddrPtr, &localAddr, localAddrState);                                                            /* SBSW_SD_POINTER_WRITE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchType(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfMulticastSoConIdType MulticastSoConIdIdx,
  SD_P2VAR(Sd_LocalRemoteMatchTypeType) RemoteMatchTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SockAddrContainerType remoteAddr;
  uint8 remoteAddrState;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the configured remote IP address. */
  retVal = Sd_SoAd_GetRemoteAddr(Sd_GetMulticastSoConId(MulticastSoConIdIdx, PartitionIdx), &remoteAddr);               /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(retVal == E_OK)
  {
    /* #100 Calculate the state of the remote address. */
    remoteAddrState = Sd_StateClient_FindBestMatchLocalRemoteSoConCalcAddrState(&remoteAddr);                           /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Compare the remote address with the configured value and calculate a match type. */
    RemoteMatchTypePtr->RemoteIpMatchType = Sd_Util_CompareSockAddrIp(
      Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx, PartitionIdx), &remoteAddr, remoteAddrState);               /* SBSW_SD_POINTER_WRITE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    RemoteMatchTypePtr->RemotePortMatchType = Sd_Util_CompareSockAddrPort(
      Sd_GetAddrServerUdpEndpointAddrOfClientDyn(clientIdx, PartitionIdx), &remoteAddr, remoteAddrState);               /* SBSW_SD_POINTER_WRITE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcLocalMatchPrio(
  SD_P2CONST(Sd_LocalRemoteMatchTypeType) MatchTypePtr,
  SD_P2VAR(uint8) LocalMatchPrioPtr,
  SD_P2VAR(uint8) RemoteMatchPrioPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean localMatch = FALSE;
  uint8 initialLocalMatchPrio = *LocalMatchPrioPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the match priority for the local IP address. */
  if((MatchTypePtr->LocalIpMatchType == SD_MATCH_TRUE) && (MatchTypePtr->LocalPortMatchType == SD_MATCH_TRUE))
  {
    *LocalMatchPrioPtr = 4u;                                                                                            /* SBSW_SD_POINTER_WRITE */
    localMatch = TRUE;
  }
  else if((MatchTypePtr->LocalIpMatchType == SD_MATCH_TRUE) && (MatchTypePtr->LocalPortMatchType == SD_MATCH_ANY))
  {
    if(*LocalMatchPrioPtr <= 3u)
    {
      *LocalMatchPrioPtr = 3u;                                                                                          /* SBSW_SD_POINTER_WRITE */
      localMatch = TRUE;
    }
  }
  else if((MatchTypePtr->LocalIpMatchType == SD_MATCH_ANY) && (MatchTypePtr->LocalPortMatchType == SD_MATCH_TRUE))
  {
    if(*LocalMatchPrioPtr <= 2u)
    {
      *LocalMatchPrioPtr = 2u;                                                                                          /* SBSW_SD_POINTER_WRITE */
      localMatch = TRUE;
    }
  }
  else if((MatchTypePtr->LocalIpMatchType == SD_MATCH_ANY) && (MatchTypePtr->LocalPortMatchType == SD_MATCH_ANY))
  {
    if(*LocalMatchPrioPtr <= 1u)
    {
      *LocalMatchPrioPtr = 1u;                                                                                          /* SBSW_SD_POINTER_WRITE */
      localMatch = TRUE;
    }
  }
  else
  {
    /* The socket connections do not match. */
  }

  /* #20 Check if the priority of the local match could be improved. */
  if(*LocalMatchPrioPtr > initialLocalMatchPrio)
  {
    /* #200 Reset the remote priority in order to calculate it again for the new socket connection. */
    *RemoteMatchPrioPtr = 0u;                                                                                           /* SBSW_SD_POINTER_WRITE */
  }

  return localMatch;
}

/**********************************************************************************************************************
 *  Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_FindBestMatchLocalRemoteSoConCalcRemoteMatchPrio(
  SD_P2CONST(Sd_LocalRemoteMatchTypeType) MatchTypePtr,
  SD_P2VAR(uint8) RemoteMatchPrioPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean betterMatchFound = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the match priority for the remote IP address. */
  if((MatchTypePtr->RemoteIpMatchType == SD_MATCH_TRUE) && (MatchTypePtr->RemotePortMatchType == SD_MATCH_TRUE))
  {
    *RemoteMatchPrioPtr = 4u;                                                                                           /* SBSW_SD_POINTER_WRITE */
    betterMatchFound = TRUE;
  }
  else if((MatchTypePtr->RemoteIpMatchType == SD_MATCH_TRUE) && (MatchTypePtr->RemotePortMatchType == SD_MATCH_ANY))
  {
    if(*RemoteMatchPrioPtr < 3u)
    {
      *RemoteMatchPrioPtr = 3u;                                                                                         /* SBSW_SD_POINTER_WRITE */
      betterMatchFound = TRUE;
    }
  }
  else if((MatchTypePtr->RemoteIpMatchType == SD_MATCH_ANY) && (MatchTypePtr->RemotePortMatchType == SD_MATCH_TRUE))
  {
    if(*RemoteMatchPrioPtr < 2u)
    {
      *RemoteMatchPrioPtr = 2u;                                                                                         /* SBSW_SD_POINTER_WRITE */
      betterMatchFound = TRUE;
    }
  }
  else if((MatchTypePtr->RemoteIpMatchType == SD_MATCH_ANY) && (MatchTypePtr->RemotePortMatchType == SD_MATCH_ANY))
  {
    if(*RemoteMatchPrioPtr < 1u)
    {
      *RemoteMatchPrioPtr = 1u;                                                                                         /* SBSW_SD_POINTER_WRITE */
      betterMatchFound = TRUE;
    }
  }
  else
  {
    /* The socket connections do not match. */
  }

  return betterMatchFound;
}

/**********************************************************************************************************************
 *  Sd_StateClient_ReleaseCeg()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_ReleaseCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx);
  Sd_ConsumedEventGroupStateMachineType consumedEventgroupState = SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Unconfigure consumed eventgroup. */
  Sd_StateClient_UnconfigureCeg(ConsumedEventgroupIdx, PartitionIdx, FALSE, TRUE);

  /* #20 Inform BswM about state change of consumed eventgroup. */
  BswM_Sd_ConsumedEventGroupCurrentState(Sd_GetHandleIdOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx),
    SD_CONSUMED_EVENTGROUP_DOWN);

  /* #30 Check if the consumed eventgroup was released by BswM. */
  /*lint -e655 */
  if(((Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) & SD_CONSUMED_EVENTGROUP_STATE_MASK) != SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED) &&
    (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SD_CONSUMED_EVENTGROUP_RELEASED)) /*lint +e655 */
  {
    /* #300 The eventgroup was set from requested to released, send a StopSubscribe message. */
    Sd_Util_AddSubscribeEntryToList(ConsumedEventgroupIdx, PartitionIdx, FALSE, 0u);
  }
  /* #40 Otherwise, inform the BswM about the state change. */
  else
  {
    /* #400 Check if the consumed eventgroup is released because of the reception of a SubscribeEventgroupNack entry. */
    if(Sd_GetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SD_CLIENT_EVENT_EVENTGROUP_NACK)
    {
      /* #4000 Report DEM if a NACK was received. */
      Sd_Util_ReportDemEvent(Sd_GetENackRecvIdOfInstance(instanceIdx, PartitionIdx));
      Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE, PartitionIdx);         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

      /* #4001 Check if the next received multicast OfferService should lead to the transmission of a
       *       StopSubscribeEventgroup-SubscribeEventgroup sequence. */
      if(Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) ==
           SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT_MULTI_OFFER)
      {
        consumedEventgroupState = SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED_AFTER_MULTI_OFFER_NACK;
      }
    }
    /* #401 Otherwise, the timer of the consumed eventgroup expired. */
    else
    {
      /* #4010 Trigger a subscription. */
      Sd_SetTriggerSubscriptionOfConsumedEventgroupDyn(ConsumedEventgroupIdx, TRUE, PartitionIdx);                      /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }

  /* #50 Reset the state of the eventgroup. */
  Sd_StateClient_UpdateCeg(ConsumedEventgroupIdx, PartitionIdx, consumedEventgroupState);
  Sd_SetCurrentStateOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN, PartitionIdx);          /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
}

/**********************************************************************************************************************
 *  Sd_StateClient_Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_Update(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ClientStateMachineType NewState,
  boolean ResetTimeToTriggerSend)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the new state of the state machine. */
  Sd_SetStateMachineOfClientDyn(ClientIdx, NewState, PartitionIdx);                                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Set a timer event and mark state machine of client for execution. */
  Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx);

  if(ResetTimeToTriggerSend == TRUE)
  {
    /* #200 Reset the TimeToTriggerSend. */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx));                       /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_UpdateCeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_StateClient_UpdateCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ConsumedEventGroupStateMachineType NewState)
{
  Sd_ClientIdxOfConsumedEventgroupType clientIdx =
    Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the new state of the state machine. */
  Sd_SetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, NewState, PartitionIdx);                             /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Set a timer event and mark state machine of client for execution. */
  Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx), PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_StateClient_UnconfigureCeg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_StateClient_UnconfigureCeg(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean AbortFlag,
  boolean ResetEndpointInfo)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Unconfigure TCP configuration. */
  if(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
  {
    (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupTcpActRefIdxOfConsumedEventgroup(
      ConsumedEventgroupIdx, PartitionIdx), PartitionIdx);
    (void)SoAd_CloseSoCon(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx), AbortFlag);
    Sd_SetTcpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }

  /* #20 Unconfigure UDP configuration. */
  if(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
  {
    (void)Sd_Util_DisableRouting((Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupUdpActRefIdxOfConsumedEventgroup(
      ConsumedEventgroupIdx, PartitionIdx), PartitionIdx);
    (void)SoAd_CloseSoCon(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx), AbortFlag);
    Sd_SetUdpSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }

  /* #30 Unconfigure multicast configuration. */
  if(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID)
  {
    (void)Sd_Util_DisableRouting( (Sd_SizeOfRoutingGroupType)Sd_GetRoutingGroupMulticastActRefIdxOfConsumedEventgroup(
      ConsumedEventgroupIdx, PartitionIdx), PartitionIdx);
    (void)Sd_StateClient_ReleaseIpAddrAssignment(PartitionIdx,
      Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx));
    (void)SoAd_CloseSoCon(Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx),
#if(SD_SET_REM_ADDR_OF_CLIENT_RX_MULTICAST_SOCON == STD_ON)
      AbortFlag);
#else
      FALSE);
#endif
    Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);             /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }

  if(Sd_IsMulticastEndpointAddrUsedOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx) &&
      (ResetEndpointInfo == TRUE))
  {
    (Sd_GetAddrMulticastEndpointAddr(
      Sd_GetMulticastEndpointAddrIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx), PartitionIdx))
      ->family = SOAD_AF_INVALID;                                                                                       /* SBSW_SD_API_ARGUMENT_IDX */
  }
}

/**********************************************************************************************************************
 *  Sd_StateClient_LocalIpAlreadyConfigured()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_LocalIpAlreadyConfigured(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_SockAddrContainerType) MulticastAddressPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_IpAddrRequestIterType ipAddrRequestIdx = 0u;
  Std_ReturnType retVal;
  Sd_SockAddrContainerType localAddr;
  boolean localIpConfigured = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for(ipAddrRequestIdx = 0u; ipAddrRequestIdx < Sd_GetSizeOfIpAddrRequest(PartitionIdx); ipAddrRequestIdx++)
  {
    /* #10 Get the configured local IP address. */
    retVal = Sd_SoAd_GetLocalAddr(Sd_GetSoConIdOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx), &localAddr);            /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #20 Check if the local address is already configured. */
    /* PRQA S 3415 2 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&
      (Sd_Util_CompareSockAddrIp(&localAddr, MulticastAddressPtr, SOAD_SOCON_IP_SET_PORT_SET) == SD_MATCH_TRUE))        /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_PASSING_API_POINTER */
    {
      /* #200 The multicast IP address is already configured. */
      localIpConfigured = TRUE;
      break;
    }
  }

  return localIpConfigured;
}

/**********************************************************************************************************************
 *  Sd_StateClient_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_RequestIpAddrAssignment(
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) MulticastAddrPtr,
  boolean PerformRequest)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_IpAddrRequestIterType ipAddrRequestIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the corresponding local IP address request count. */
  for (ipAddrRequestIdx = 0u; ipAddrRequestIdx < Sd_GetSizeOfIpAddrRequest(PartitionIdx); ipAddrRequestIdx++)
  {
    if(Sd_GetSoConIdOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx) == SoConId)
    {
      retVal = E_OK;

      /* #100 Check if the IP address is already requested. */
      if(((!Sd_IsPortSet(Sd_GetPortSetIdxOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx), PartitionIdx)) ||
        (Sd_GetRequestCnt(Sd_GetRequestCntIdxOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx), PartitionIdx) == 0u)) &&
        (PerformRequest == TRUE))
      {
        /* #1000 The multicast address and port have to be requested. */
        Sd_SockAddrContainerType defaultRouter;

        /* Create an unspecified address for the default router and net mask. */
        uint8 netmaskCidr = 0u;
        defaultRouter.family = MulticastAddrPtr->family;
        defaultRouter.data[0u] = 0u;                                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        defaultRouter.data[1u] = 0u;                                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        defaultRouter.data[2u] = 0u;                                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        defaultRouter.data[3u] = 0u;                                                                                    /* SBSW_SD_IP_ADDR_SIZE */
        defaultRouter.port = 0u;

        /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
        retVal = SoAd_RequestIpAddrAssignment(SoConId, SOAD_IPADDR_ASSIGNMENT_STATIC,
          (SD_P2VAR(SoAd_SockAddrType))MulticastAddrPtr, netmaskCidr, (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);     /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_SOCK_ADDR_CAST */
      }

      if(retVal == E_OK)
      {
        /* #1001 Increment the request counter. */
        Sd_IncRequestCnt(Sd_GetRequestCntIdxOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx), PartitionIdx);             /* SBSW_SD_CSL03_CSL05 */
        Sd_SetPortSet(Sd_GetPortSetIdxOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx), TRUE, PartitionIdx);             /* SBSW_SD_CSL03_CSL05 */
      }

      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_ReleaseIpAddrAssignment()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_StateClient_ReleaseIpAddrAssignment(
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Sd_IpAddrRequestIterType ipAddrRequestIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for the request count of the given socket connection index. */
  for (ipAddrRequestIdx = 0u; ipAddrRequestIdx < Sd_GetSizeOfIpAddrRequest(PartitionIdx); ipAddrRequestIdx++)
  {
    if(Sd_GetSoConIdOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx) == SoConId)
    {
      retVal = E_OK;

      /* #100 Check if the IP address is also assigned by another client. */
      if(Sd_GetRequestCnt(Sd_GetRequestCntIdxOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx), PartitionIdx) == 1u)
      {
        if(!Sd_IsStaticIpConfigurationOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx))
        {
          /* #1000 The multicast address is only requested by a single client. Release the request. */
          retVal = SoAd_ReleaseIpAddrAssignment(SoConId);
        }
      }

      /* #101 Reset the port set configuration. */
      if(!Sd_IsStaticPortConfigurationOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx))
      {
        Sd_SetPortSet(Sd_GetPortSetIdxOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx), FALSE, PartitionIdx);            /* SBSW_SD_CSL03_CSL05 */
      }

      /* #102 Decrement the request counter. (Even if the SoAd_ReleaseIpAddrAssignment fails). */
      Sd_DecRequestCnt(Sd_GetRequestCntIdxOfIpAddrRequest(ipAddrRequestIdx, PartitionIdx), PartitionIdx);               /* SBSW_SD_CSL03_CSL05 */

      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_StateClient_EntryListContainsSubscribe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_StateClient_EntryListContainsSubscribe(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_AddressStorageIterType addressStorageIdx;
  Sd_SizeOfSendEntryType sendEntryIdx;
  boolean containsSubscribe = FALSE;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(
    Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx), PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all address storages of the given instance. */
  for(addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(instanceIdx, PartitionIdx);
      addressStorageIdx < Sd_GetAddressStorageEndIdxOfInstance(instanceIdx, PartitionIdx);
      addressStorageIdx++)
  {
    /* #100 Iterate over all send entries of the address storage list. */
    sendEntryIdx = Sd_GetSendEntryHeadIdxOfAddressStorage(addressStorageIdx, PartitionIdx);
    while(sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx))
    {
      /* #1000 If send entry is a Subscribe-Entry for the given ConsumedEventGroup, set return value as TRUE and
               break iterations. */
      if((Sd_GetEntryTypeOfSendEntry(sendEntryIdx, PartitionIdx) == SD_ENTRY_SUBSCRIBE_EVENTGROUP) &&
        (ConsumedEventgroupIdx == Sd_GetConsumedEventgroupIdxOfSendEntry(sendEntryIdx, PartitionIdx)))
      {
        containsSubscribe = TRUE;
        break;
      }
      sendEntryIdx = Sd_GetSendEntryNextIdxOfSendEntry(sendEntryIdx, PartitionIdx);
    }
    if(containsSubscribe == TRUE)
    {
      break;
    }
  }

  return containsSubscribe;
}

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntry()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntry(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;
  uint16 clientStartIdx, clientEndIdx;
  Sd_EntryType rxEntryType;
  uint16 rxServiceId;
  uint16 rxInstanceId;
  uint8 rxMajorVersion;
  uint32 rxMinorVersion;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the header fields of the entry. */
  SD_GET_ENTRY_TYPE(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry /* + SD_ENTRY_TYPE_OFFSET */, rxEntryType);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_SERVICEID_OFFSET, rxServiceId);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INSTANCEID_OFFSET, rxInstanceId);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_MAJORVERSION_OFFSET, rxMajorVersion);

  /* #20 Get all services with the corresponding ServiceId */
  clientStartIdx = (uint16)Sd_GetClientStartIdxOfInstance(InstanceIdx, PartitionIdx);
  clientEndIdx = (uint16)Sd_GetClientEndIdxOfInstance(InstanceIdx, PartitionIdx);

  /* #30 Search for the matching client(s). */
  Sd_Util_GetServices(PartitionIdx, rxServiceId, Sd_Util_GetClientServiceId, &clientStartIdx, &clientEndIdx);           /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #40 Iterate over all matching clients to find the one corresponding to the entry. */
  for(clientIdx = (Sd_SizeOfClientType)clientStartIdx; clientIdx < (Sd_SizeOfClientType)clientEndIdx; clientIdx++)
  {
    /* #400 Check if the InstanceId and MajorVersion match. */
    if((rxInstanceId == Sd_GetInstanceIdOfClientDyn(clientIdx, PartitionIdx)) &&
      (rxMajorVersion == Sd_GetMajorVersionOfClient(clientIdx, PartitionIdx)))
    {
      /* #4000 Differentiate between entry format type 1 or 2 */
      if(rxEntryType == SD_ENTRY_OFFER_SERVICE)
      {
        /* #40000 Entry format type 1 - OfferService.
         *        Process the entry even if the service is not available. */
        SD_GET_UINT32(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY1_MINORVERSION_OFFSET, rxMinorVersion);

        /* #40001 Check the minor version fits to the configuration. */
        if(((Sd_GetFindBehaviorOfClient(clientIdx, PartitionIdx) == SD_FIND_BEHAVIOR_EXACT_OR_ANY_MINOR_VERSION) &&
           ((rxMinorVersion == Sd_GetMinorVersionOfClient(clientIdx, PartitionIdx)) || Sd_IsMinorVersionWildcardOfClient(clientIdx, PartitionIdx))) ||
          ((Sd_GetFindBehaviorOfClient(clientIdx, PartitionIdx) == SD_FIND_BEHAVIOR_MINIMUM_MINOR_VERSION) &&
          (rxMinorVersion >= Sd_GetMinorVersionOfClient(clientIdx, PartitionIdx))))
        {
          /* #400010 Process the received OfferService entry. */
          Sd_RxClient_ProcessEntryOffer(clientIdx, PartitionIdx, IndexOfEntry, RxMessageInfoPtr);                       /* SBSW_SD_PASSING_API_POINTER */
        }
      }
      else
      {
        /* #40002 Entry format type 2 - SubscribeEventgroup(N)Ack.
         *        Process the acknowledge only if no reboot of the remote server was detected. */
        if(!Sd_IsRebootDetectedOfAddressStorage(RxMessageInfoPtr->AddressStorageIdx, PartitionIdx))
        {
          Sd_RxClient_ProcessEntryAcknowledge(clientIdx, PartitionIdx, IndexOfEntry, RxMessageInfoPtr);                 /* SBSW_SD_PASSING_API_POINTER */
        }
      }
    }
  }
}

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryOffer()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryOffer(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);
  Sd_AttachedOptionsType attachedOptions = { 0 };
  Sd_ConfigOptionStringType rxConfigOptionPtrArray[Sd_GetMaxNrOptionsPerEntry(PartitionIdx) + 1u];

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0314 1 */ /* MD_Sd_Dir-1.1_0314 */
  IpBase_Fill((SD_P2VAR(void))rxConfigOptionPtrArray, 0u, sizeof(rxConfigOptionPtrArray));                              /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_IPBASE_FILL */
  attachedOptions.EndpointAddrUdp.family = SOAD_AF_INVALID;
  attachedOptions.EndpointAddrTcp.family = SOAD_AF_INVALID;
  attachedOptions.ConfigOptionPtrArrayPtr = rxConfigOptionPtrArray;

  /* #10 Process the endpoints of the OfferService message. */
  optionsRetVal = Sd_RxClient_ProcessEntryOfferOptionRuns(ClientIdx, PartitionIdx, IndexOfEntry, RxMessageInfoPtr,
    &attachedOptions);                                                                                                  /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

  if(optionsRetVal == SD_OPTIONS_OK)
  {
    /* #200 Check if the attached endpoint addresses are part of the subnet. */
    optionsRetVal = Sd_Rx_CheckSubnets(instanceIdx, PartitionIdx,
      Sd_IsUdpSoConUsedOfClient(ClientIdx, PartitionIdx) ? TRUE : FALSE,
      Sd_IsTcpSoConIdUsedOfClient(ClientIdx, PartitionIdx) ? TRUE : FALSE,
      &attachedOptions);                                                                                                /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }

  if(optionsRetVal == SD_OPTIONS_OK)
  {
    /* #300 Check the attached configuration options by a user callout. */
    optionsRetVal = Sd_RxClient_CheckConfigOptionByCbk(
      ClientIdx, PartitionIdx,
      (RxMessageInfoPtr->MulticastMessage == TRUE) ?
        Sd_GetMulticastRxPduIdOfInstance(instanceIdx, PartitionIdx) :
        Sd_GetUnicastRxPduIdOfInstance(instanceIdx, PartitionIdx),
      SD_ENTRY_OFFER_SERVICE,
      rxConfigOptionPtrArray);                                                                                          /* SBSW_SD_REFERENCE_OF_VARIABLE */
  }

  if(optionsRetVal == SD_OPTIONS_OK)
  {
    /* #400 Save the address of the server. */
    Sd_SetAddressStorageServerIdxOfClientDyn(ClientIdx, RxMessageInfoPtr->AddressStorageIdx, PartitionIdx);             /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    /* #401 Check the TTL of the received entry. */
    if(Sd_GetOfferedTTLOfClientDyn(ClientIdx, PartitionIdx) == 0u)
    {
      /* #4010 Received SD_ENTRY_STOP_OFFER_SERVICE message. Save the event. */
      Sd_SetEventRxOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_STOP_OFFER, PartitionIdx);                            /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      Sd_Util_TriggerStateMachineRun(instanceIdx, PartitionIdx);
    }
    else
    {
      /* #4011 Received SD_ENTRY_OFFER_SERVICE message. */

      /* #4012 Save the attached endpoint options. */
      Sd_Util_CopySockAddr(Sd_GetAddrServerUdpEndpointAddrOfClientDyn(ClientIdx, PartitionIdx),
        &attachedOptions.EndpointAddrUdp);                                                                              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      Sd_Util_CopySockAddr(Sd_GetAddrServerTcpEndpointAddrOfClientDyn(ClientIdx, PartitionIdx),
        &attachedOptions.EndpointAddrTcp);                                                                              /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

      /* #4013 Save the received OfferService message as event. */
      Sd_SetEventRxOfClientDyn(ClientIdx, (RxMessageInfoPtr->MulticastMessage == TRUE) ?                              /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
        SD_CLIENT_EVENT_SERVICE_OFFER_MULTI : SD_CLIENT_EVENT_SERVICE_OFFER_UNI, PartitionIdx);

      Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      Sd_Util_TriggerStateMachineRun(instanceIdx, PartitionIdx);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryOfferOptionRuns()
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
SD_LOCAL_INLINE FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxClient_ProcessEntryOfferOptionRuns(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_NOT_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);
  uint32 rxTTL;
  uint8 index1stOption, index2ndOption;
  uint8 nr1stOptions, nr2ndOptions;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Parse OfferService entry. */
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX1_OFFSET, index1stOption);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX2_OFFSET, index2ndOption);
  SD_GET_NROFOPT(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, nr1stOptions, nr2ndOptions);
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);

  /* #20 Save the offered TTL of the server. */
  Sd_SetOfferedTTLOfClientDyn(ClientIdx, rxTTL, PartitionIdx);                                                                        /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #30 Process the corresponding option array.
   *     Check if all attached endpoint options are valid and there exist no conflicts. */

   /* #40 Check if a first option run is available.*/
  if(nr1stOptions != 0u)
  {
    /* #400 Process first option run. */
    optionsRetVal = Sd_RxClient_ProcessOptionRun(PartitionIdx, RxMessageInfoPtr->PduInfoPtr, index1stOption,
      nr1stOptions, Sd_GetEMalformedMsgIdOfInstance(instanceIdx, PartitionIdx),
      Sd_GetSizeOfConsumedEventgroup(PartitionIdx), AttachedOptionsPtr);                                                /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_PASSING_API_POINTER */
  }

  /* #50 Check if a second option run is available.*/
  if((nr2ndOptions != 0u) && ((nr1stOptions == 0u) || (optionsRetVal == SD_OPTIONS_OK)))
  {
    /* #500 Process second option run. */
    optionsRetVal = Sd_RxClient_ProcessOptionRun(PartitionIdx, RxMessageInfoPtr->PduInfoPtr, index2ndOption,
      nr2ndOptions, Sd_GetEMalformedMsgIdOfInstance(instanceIdx, PartitionIdx),
      Sd_GetSizeOfConsumedEventgroup(PartitionIdx), AttachedOptionsPtr);                                                /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_PASSING_API_POINTER */
  }

  /* #60 Otherwise, check if both option runs are empty and the entry is a StopOfferService entry.*/
  else if((nr1stOptions == 0u) && (nr2ndOptions == 0u) && (rxTTL == 0u)) /* PRQA S 2995 */ /* MD_Sd_FalsePositive_2995_2996_2853 */
  {
    /* #600 In case no first or second option run exists, only SD_ENTRY_STOP_OFFER_SERVICE messages are allowed. */
    optionsRetVal = SD_OPTIONS_OK;
  }

  /* #70 Otherwise, nothing to process.*/
  else
  {
    /* #700 Entry does not contain a second option run. Keep result of first option run. */
  }

  /* #80 Check if all required endpoint options are present. */
  if((optionsRetVal == SD_OPTIONS_OK) &&
    ((Sd_IsUdpSoConUsedOfClient(ClientIdx, PartitionIdx) && (AttachedOptionsPtr->EndpointAddrUdp.family == SOAD_AF_INVALID)) ||
    (Sd_IsTcpSoConIdUsedOfClient(ClientIdx, PartitionIdx) && (AttachedOptionsPtr->EndpointAddrTcp.family == SOAD_AF_INVALID))))
  {
    /* #800 Not all required endpoint options are present. Reject message. */
    optionsRetVal = SD_OPTIONS_NOT_OK;
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryAcknowledge()
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
 */
/* PRQA S 6010 1 */ /* MD_Sd_STPTH_6010 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryAcknowledge(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 IndexOfEntry,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 2981 1 */ /* MD_Sd_Rule-2.2_2981 */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_NOT_OK;
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);
  Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;
  Sd_ConfigOptionStringType rxConfigOptionPtrArray[Sd_GetMaxNrOptionsPerEntry(PartitionIdx) + 1u];
  Sd_AttachedOptionsType attachedOptions = { 0 };
  uint32 rxTTL;
  uint16 rxEventGroupId;
  uint8 index1stOption, index2ndOption;
  uint8 nr1stOptions, nr2ndOptions;

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 0314 1 */ /* MD_Sd_Dir-1.1_0314 */
  IpBase_Fill((SD_P2VAR(void))rxConfigOptionPtrArray, 0u, sizeof(rxConfigOptionPtrArray));                              /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_IPBASE_FILL */
  attachedOptions.EndpointAddrUdp.family = SOAD_AF_INVALID;
  attachedOptions.EndpointAddrTcp.family = SOAD_AF_INVALID;
  attachedOptions.ConfigOptionPtrArrayPtr = rxConfigOptionPtrArray;

  /* #10 Parse SubscribeEventgroupAck entry. */
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX1_OFFSET, index1stOption);
  SD_GET_UINT8(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_INDEX2_OFFSET, index2ndOption);
  SD_GET_NROFOPT(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry, nr1stOptions, nr2ndOptions);
  SD_GET_UINT24(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY_TTL_OFFSET, rxTTL);
  SD_GET_UINT16(RxMessageInfoPtr->PduInfoPtr, IndexOfEntry + SD_ENTRY2_EVENTGROUPID_OFFSET, rxEventGroupId);

  /* #20 Iterate through each eventgroup to find the corresponding one. */
  for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(ClientIdx, PartitionIdx);
      consumedEventgroupIdx++)
  {
    if((rxEventGroupId == Sd_GetEventGroupIdOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx)) &&
       (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) ==
         SD_CONSUMED_EVENTGROUP_REQUESTED))
    {
      /* #200 Found the corresponding eventgroup, update the acknowledged TTL. */
      Sd_SetAcknowledgedTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, rxTTL, PartitionIdx);                         /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */

      /* #201 Process the corresponding option array.
       *      Check if all attached endpoint options are valid and there exist no conflicts. */

      /* #202 Check if a first option run is available.*/
      if(nr1stOptions != 0u)
      {
        /* #2020 Process first option run. */
        optionsRetVal = Sd_RxClient_ProcessOptionRun(PartitionIdx, RxMessageInfoPtr->PduInfoPtr,
          index1stOption, nr1stOptions, Sd_GetEMalformedMsgIdOfInstance(instanceIdx, PartitionIdx),
          consumedEventgroupIdx, &attachedOptions);                                                                     /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }

      /* #300 Check if a second option run is available.*/
      if((nr2ndOptions != 0u) && ((nr1stOptions == 0u) || (optionsRetVal == SD_OPTIONS_OK)))
      {
        /* #3000 Process second option run. */
        optionsRetVal = Sd_RxClient_ProcessOptionRun(PartitionIdx, RxMessageInfoPtr->PduInfoPtr,
          index2ndOption, nr2ndOptions, Sd_GetEMalformedMsgIdOfInstance(instanceIdx, PartitionIdx),
          consumedEventgroupIdx, &attachedOptions);                                                                     /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }
      /* #400 Otherwise, check if both option runs are empty.*/
      else if((nr1stOptions == 0u) && (nr2ndOptions == 0u)) /* PRQA S 2995 */ /* MD_Sd_FalsePositive_2995_2996_2853 */
      {
        /* #4000 In case no first or second option run exists, only SD_ENTRY_SUBSCRIBE_EVENTGROUP_(N)ACK messages are allowed. */
        optionsRetVal = SD_OPTIONS_OK;
      }
      /* #500 Otherwise, nothing to process.*/
      else
      {
        /* #5000 No second option run present. Keep result of first option run. */
      }

      /* #600 Check if all required endpoint options are present. */
      if((optionsRetVal == SD_OPTIONS_OK) &&
        (attachedOptions.EndpointAddrUdp.family == SOAD_AF_INVALID) &&
        Sd_ConsumedEventGroupUsesMulticast(consumedEventgroupIdx, PartitionIdx) &&
        (!Sd_ConsumedEventGroupUsesUdp(consumedEventgroupIdx, PartitionIdx)))
      {
        /* #6000 Missing multicast endpoint is not allowed for pure multicast consumed eventgroups. Ignore message. */
        optionsRetVal = SD_OPTIONS_NOT_OK;
      }

      if(optionsRetVal == SD_OPTIONS_OK)
      {
        /* #7000 Check the attached configuration options by a user callout. */
        optionsRetVal = Sd_RxClient_CheckConfigOptionByCbk(
          ClientIdx, PartitionIdx,
          (RxMessageInfoPtr->MulticastMessage == TRUE) ?
          Sd_GetMulticastRxPduIdOfInstance(instanceIdx, PartitionIdx) :
          Sd_GetUnicastRxPduIdOfInstance(instanceIdx, PartitionIdx),
          SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK,
          rxConfigOptionPtrArray);                                                                                      /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }

      if(optionsRetVal == SD_OPTIONS_OK)
      {
        /* #8000 The attached options are valid. Save the received event. */
        Sd_RxClient_ProcessEntryAcknowledgeSave(consumedEventgroupIdx, PartitionIdx, &attachedOptions.EndpointAddrUdp,
          rxTTL, RxMessageInfoPtr->AddressStorageIdx);                                                                 /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }

#if(SD_GET_RESET_MEASUREMENT_DATA_API == STD_ON)
      /* #900 Increment measurement data counter for SubscribeNack entries. */
      if(rxTTL == 0u)
      {
        Sd_Measure_Inc(SD_MEAS_SUBSCRIBE_NACK, 1u);
      }
#endif

      /* #1000 Found the corresponding eventgroup, stop searching. */
      break;
    }
  }
}

/**********************************************************************************************************************
 *  Sd_RxClient_CheckConfigOptionByCbk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxClient_CheckConfigOptionByCbk(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  PduIdType RxPduId,
  Sd_EntryType RxEntryType,
  SD_P2CONST(Sd_ConfigOptionStringType) RxConfigOptionPtrArrayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the client contains a configured user callout for checking capability records. */
  if(Sd_GetCapabilityRecordFunctionCalloutPtrIdxOfClient(ClientIdx, PartitionIdx) !=
       SD_NO_CAPABILITYRECORDFUNCTIONCALLOUTPTRIDXOFCLIENT)
  {
    Sd_ConfigOptionStringType configOptionPtrArray[3u] = { NULL_PTR, NULL_PTR, NULL_PTR };

    /* #100 Add configuration options of hostname and capability records to the configuration option pointer array. */
    Sd_Rx_FillConfigOptionPtrArray(configOptionPtrArray,
      Sd_GetCapabilityRecordStartIdxOfInstance(Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx), PartitionIdx),
      Sd_GetCapabilityRecordStartIdxOfClient(ClientIdx, PartitionIdx),
      SD_NO_CAPABILITYRECORDSTARTIDXOFCLIENT);                                                                          /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Call application to verify configuration options of received message. */
    if(TRUE != Sd_GetCapabilityRecordFunctionCalloutPtr(
        Sd_GetCapabilityRecordFunctionCalloutPtrIdxOfClient(ClientIdx, PartitionIdx))(
      RxPduId,
      (uint8)RxEntryType,
      Sd_GetServiceIdOfClient(ClientIdx, PartitionIdx),
      Sd_GetInstanceIdOfClientDyn(ClientIdx, PartitionIdx),
      Sd_GetMajorVersionOfClient(ClientIdx, PartitionIdx),
      Sd_GetMinorVersionOfClient(ClientIdx, PartitionIdx),
      RxConfigOptionPtrArrayPtr,
      configOptionPtrArray))                                                                                            /* SBSW_SD_FUNCTION_PTR_CALL */
    {
      optionsRetVal = SD_OPTIONS_NOT_OK;
    }
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessEntryAcknowledgeSave()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_RxClient_ProcessEntryAcknowledgeSave(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_SockAddrContainerType) MulticastEndpointAddrPtr,
  uint32 RxTTL,
  Sd_SizeOfAddressStorageType RxSdEp)
{
  /* ----- Local Variables ---------------------------------------------- */
  const Sd_RxIndicationCountOfPartitionDataDynType rxIndicationCount =
    Sd_GetRxIndicationCountOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx);
  const Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If TTL is zero, NACK has been received.. */
  if(RxTTL == 0u)
  {
    /* #100 Received SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK. Save the event. */
    Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NACK, PartitionIdx);           /* SBSW_SD_API_ARGUMENT_IDX */

    /* #101 Save the RxIndicationCount of the current frame. */
    Sd_SetRxIndicationCountOfConsumedEventgroupDyn(ConsumedEventgroupIdx, rxIndicationCount, PartitionIdx);             /* SBSW_SD_API_ARGUMENT_IDX */
  }
  /* #20 Else ACK has been received.. */
  else
  {
    /* #200 Check if there is no pending event,
     *      or if the pending event is a SubscribeEventgroupNack entry received in the same message. */
    if((Sd_GetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SD_CLIENT_EVENT_EVENTGROUP_NONE) ||
      ((Sd_GetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == SD_CLIENT_EVENT_EVENTGROUP_NACK) &&
      (Sd_GetRxIndicationCountOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx) == rxIndicationCount)))
    {
      Sd_SizeOfAddressStorageType serverSdEp = Sd_GetAddressStorageServerIdxOfClientDyn(clientIdx, PartitionIdx);
      Sd_ConsumedEventGroupStateMachineType consumedEventgroupState =
        Sd_GetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx, PartitionIdx);

      /* #2000 Save the attached endpoint option. */
      if(Sd_IsMulticastEndpointAddrUsedOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx))
      {
        Sd_Util_CopySockAddr(
          Sd_GetAddrMulticastEndpointAddr(Sd_GetMulticastEndpointAddrIdxOfConsumedEventgroup(
            ConsumedEventgroupIdx, PartitionIdx), PartitionIdx), MulticastEndpointAddrPtr);                                                                                        /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
      }

      /* #2001 Check if the consumed eventgroup expects an ACK message
               and if the remote SD endpoint of the Ack matches to the Offer. */
      if(((consumedEventgroupState & SD_CONSUMED_EVENTGROUP_STATE_MASK) != SD_CONSUMED_EVENTGROUP_STATE_UNSUBSCRIBED) && /*lint !e655 */
         (serverSdEp == RxSdEp))
      {
        /* #20010 Save the event. */
        Sd_SetEventOfConsumedEventgroupDyn(ConsumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_ACK, PartitionIdx);        /* SBSW_SD_API_ARGUMENT_IDX */

        /* #20020 Change the state of the consumed eventgroup to REGISTRATION_SENT.
         *        This will trigger processing of the ACK in the next MainFunction. */
        Sd_SetStateMachineOfConsumedEventgroupDyn(ConsumedEventgroupIdx,                                                /* SBSW_SD_API_ARGUMENT_IDX */
          SD_CONSUMED_EVENTGROUP_STATE_REGISTRATION_SENT, PartitionIdx);
      }
    }
  }

  /* #30 Trigger execution of state machines. */
  Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
  Sd_Util_TriggerStateMachineRun(Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx), PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_RxClient_ProcessOptionRun()
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
/* PRQA S 6060 1 */ /* MD_Sd_STPAR_6060 */
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_RxClient_ProcessOptionRun(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  uint8 NrOfOptions,
  Sd_Dem_EventIdType EMalformedMsgId,
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;
  uint8 currentOption = IndexOfOption;
  Sd_OptionType optionType;
  Sd_L4ProtocolType optionL4Proto = SD_L4PROTOCOL_NONE;
  uint16 addrType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Stop processing of the message if the header indexes more options than attached. */
  if(Sd_GetNrOfOptionsOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx) <
    ((Sd_NrOfOptionsOfPartitionDataDynType)IndexOfOption + (Sd_NrOfOptionsOfPartitionDataDynType)NrOfOptions))
  {
    optionsRetVal = SD_OPTIONS_NOT_OK;
  }

  /* #20 Process consecutive options. */
  while((optionsRetVal == SD_OPTIONS_OK) && ((IndexOfOption + NrOfOptions) > currentOption))
  {
    addrType = SOAD_AF_INVALID;

    /* #200 Read the option information. */
    SD_GET_OPTION_TYPE(PduInfoPtr, Sd_GetOptionIndex(currentOption, PartitionIdx), optionType);

    /* #201 Differentiate between configuration and endpoint option. */
    if(optionType == SD_OPTION_CONFIGURATION)
    {
      /* #2010 Configuration option: Check if the option is valid. */
      optionsRetVal = Sd_Rx_CheckConfigOption(PartitionIdx, PduInfoPtr, currentOption,
        AttachedOptionsPtr->ConfigOptionPtrArrayPtr); /*lint !e655 */                                                   /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_ATTACHED_OPTIONS_PTR */
    }
    else
    {
      const SdMulticastTypeType McType =
        ((ConsumedEventgroupIdx == Sd_GetSizeOfConsumedEventgroup(PartitionIdx)) ? SD_MCTYPE_UNI : SD_MCTYPE_MULTI);

      /* #2011 Endpoint option: Read the type values of the options.
       *       Multicast endpoint options are only allowed for SubscribeEventgroup(N)Ack messages. */
      Sd_Rx_GetEndpointOptionParameters(PartitionIdx, PduInfoPtr, currentOption, McType, &addrType, &optionL4Proto);    /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
    }

    /* #202 Read the endpoint options. */
    optionsRetVal |= Sd_Rx_ReadEndpointOptions(PartitionIdx, PduInfoPtr, currentOption, addrType, optionL4Proto,
      AttachedOptionsPtr); /*lint !e655 */                                                                              /* SBSW_SD_PASSING_API_POINTER */

    currentOption++;
  }

  if(optionsRetVal != SD_OPTIONS_OK)
  {
    /* #203 Report an error if the endpoint configuration was not correct. */
    Sd_Util_ReportDemEvent(EMalformedMsgId);
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  SHARED FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  Sd_Util_GetClientServiceId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(uint16, SD_CODE) Sd_Util_GetClientServiceId(
  uint16 Idx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* #10 Return the ServiceId of the client. */
  return Sd_GetServiceIdOfClient(Idx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Util_GetServerServiceId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(uint16, SD_CODE) Sd_Util_GetServerServiceId(
  uint16 Idx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* #10 Return the ServiceId of the server. */
  return Sd_GetServiceIdOfServer(Idx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Util_GetServices()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_GetServices(
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 ServiceId,
  Sd_ServiceIdGetterFctType ServiceIdAccessFuncPtr,
  SD_P2VAR(uint16) ServiceStartIdxPtr,
  SD_P2VAR(uint16) ServiceEndIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 low, high, initialEndIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Save range to search for service. */
  low = *ServiceStartIdxPtr;
  high = (uint16)((*ServiceEndIdxPtr) - 1u);
  initialEndIdx = *ServiceEndIdxPtr;

  /* #20 Invalidate the start and end idx values. */
  *ServiceEndIdxPtr = *ServiceStartIdxPtr;                                                                              /* SBSW_SD_POINTER_WRITE */

  /* #30 Search for the first service with the given ServiceId. */
  while(low < high)
  {
    uint16 mid = low + ((uint16)(high - low) / 2u);
    uint16 midServiceId = ServiceIdAccessFuncPtr(mid, PartitionIdx);                                                    /* SBSW_SD_FUNCTION_PTR_CALL_STATIC */

    if(midServiceId < ServiceId)
    {
      /* Increase the lower limit. */
      low = mid + 1u;
    }
    else if(midServiceId > ServiceId)
    {
      /* Reduce the upper limit. */
      high = (mid != 0u) ? (uint16)(mid - 1u) : 0u;
    }
    else
    {
      /* Found the correct ServiceId. */

      if(low != mid)
      {
        /* Search for the first occurrence. */
        high = mid;
      }
      else
      {
        /* ServiceId matches and full range is scanned. */
        high = mid;
        break;
      }
    }
  }

  /* #40 Check if a matching service was found. */
  if(low == high)
  {
    uint16 tmpIdx = low;

    *ServiceStartIdxPtr = low;                                                                                          /* SBSW_SD_POINTER_WRITE */
    *ServiceEndIdxPtr = low;                                                                                            /* SBSW_SD_POINTER_WRITE */

    /* #400 Check how many matching services exist and update the EndIdx. */
    /* PRQA S 3415 1 */ /* MD_Sd_12.4_3415 */
    while((*ServiceEndIdxPtr < initialEndIdx) && (ServiceId == ServiceIdAccessFuncPtr(tmpIdx, PartitionIdx)))           /* SBSW_SD_FUNCTION_PTR_CALL_STATIC */
    {
      tmpIdx++;
      (*ServiceEndIdxPtr)++;                                                                                            /* SBSW_SD_POINTER_WRITE */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Copy()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Copy(
  SD_P2VAR(void) pDst,
  SD_P2CONST(void) pSrc,
  uint32_least nCnt)
{
  /* PRQA S 0316 2 */ /* MD_Sd_Dir-1.1_0316_copy */
  SD_P2VAR(uint8) pDst8 = (SD_P2VAR(uint8)) pDst;
  SD_P2CONST(uint8) pSrc8 = (SD_P2CONST(uint8)) pSrc;
  uint32_least i;

  /* #10 Mem copy byte by byte. */
  for (i = 0; i < nCnt; i++)
  {
    pDst8[i] = pSrc8[i];                                                                                                /* SBSW_SD_ACCESS_LOOP */
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_SaveAddr()
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
SD_LOCAL_INLINE FUNC(Sd_SizeOfAddressStorageType, SD_CODE) Sd_Rx_SaveAddr(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_SockAddrContainerType) InAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfAddressStorageType addressStorageIdx, savedAddressStorageIdx = Sd_GetSizeOfAddressStorage(PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the address is already saved. */
  for(addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx, PartitionIdx);
    addressStorageIdx < Sd_GetAddressStorageEndIdxOfInstance(InstanceIdx, PartitionIdx);
    addressStorageIdx++)
  {
    if((Sd_Util_SockAddrIsEqual(Sd_GetAddrAddrOfAddressStorage(addressStorageIdx, PartitionIdx), InAddrPtr) == TRUE) &&
      (Sd_GetSessionIdCounterOfAddressStorage(addressStorageIdx, PartitionIdx) != 0u))                                  /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */ /* SBSW_SD_PASSING_API_POINTER */
    {
      /* #100 The address is already saved. Save index of the address. */
      savedAddressStorageIdx = addressStorageIdx;
      break;
    }
  }

  /* #20 Check if the address has to be saved. */
  if(savedAddressStorageIdx == Sd_GetSizeOfAddressStorage(PartitionIdx))
  {
    /* #200 Search for the next free slot and save the address. */
    for(addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx, PartitionIdx);
        addressStorageIdx < Sd_GetAddressStorageEndIdxOfInstance(InstanceIdx, PartitionIdx);
        addressStorageIdx++)
    {
      if(Sd_GetSessionIdCounterOfAddressStorage(addressStorageIdx, PartitionIdx) == 0u)
      {
        /* #2000 Save the address and set the session counter to 1. */
        Sd_Util_CopySockAddr(Sd_GetAddrAddrOfAddressStorage(addressStorageIdx, PartitionIdx), InAddrPtr);               /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */ /* SBSW_SD_PASSING_API_POINTER */
        Sd_SetSessionIdCounterOfAddressStorage(addressStorageIdx, 1u, PartitionIdx);                                    /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, 0u, PartitionIdx);                                  /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, 0u, PartitionIdx);                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetRxUnicastFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK, PartitionIdx);                    /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetRxMulticastFlagsOfAddressStorage(addressStorageIdx, SD_FLAGS_REBOOT_MASK, PartitionIdx);                  /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        savedAddressStorageIdx = addressStorageIdx;
        break;
      }
    }
  }

  if(savedAddressStorageIdx == Sd_GetSizeOfAddressStorage(PartitionIdx))
  {
    /* #201 Not able to save the additional address.
     *      The amount of manageable destination addresses is configured in Sd/SdConfig/SdInstance/SdMaxNrDestAddr. */
#if(SD_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_RX_INDICATION, SD_E_ADDR_LIST_FULL);
#endif
  }

  return savedAddressStorageIdx;
}

/**********************************************************************************************************************
 *  Sd_Rx_IndexOptionArray()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_IndexOptionArray(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionIndexIterType optionIndexIter = 0u;
  Std_ReturnType retVal = E_OK;
  uint32 lengthEntriesArray, lengthOfOptionsArray;
  uint16 lengthOfOption, tmpIndex, startIndexOptionsArray;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear the option index. */
  Sd_SetNrOfOptionsOfPartitionDataDyn(SD_PARTITION_DATA_IDX, 0u, PartitionIdx);                                         /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
  for (optionIndexIter = 0u; optionIndexIter < Sd_GetSizeOfOptionIndex(PartitionIdx); optionIndexIter++)
  {
    Sd_SetOptionIndex(optionIndexIter, 0u, PartitionIdx);                                                               /* SBSW_SD_CSL03_CSL05 */
  }

  /* #20 read length of the entries and options array. */
  SD_GET_UINT32(PduInfoPtr, SD_HEADER_LENGTH, lengthEntriesArray);
  tmpIndex = (uint16)(SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + lengthEntriesArray);

  SD_GET_UINT32(PduInfoPtr, tmpIndex, lengthOfOptionsArray);
  tmpIndex += SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE;

  startIndexOptionsArray = tmpIndex;

  /* #30 Iterate through all options and generate an index. */
  while((tmpIndex < (startIndexOptionsArray + lengthOfOptionsArray)) && (retVal == E_OK))
  {
    /* #300 Read the option and check if it is valid. */
    retVal = Sd_Rx_CheckOption(PduInfoPtr, tmpIndex, lengthOfOptionsArray, &lengthOfOption);                            /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    if(retVal == E_OK)
    {
      /* #3000 The option is valid. Check if it could be saved in the OptionIndex. */
      const uint16 nrOptions = Sd_GetNrOfOptionsOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx);
      if(nrOptions < Sd_GetSizeOfOptionIndex(PartitionIdx))
      {
        /* #30000 Add the option to the index. */
        Sd_SetOptionIndex(nrOptions, tmpIndex, PartitionIdx);                                                          /* SBSW_SD_CSL01_CSL05 */
        Sd_IncNrOfOptionsOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx);                                      /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
        tmpIndex  +=  SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE + lengthOfOption;
      }
      else
      {
        /* #30001 The Sd_OptionIndex is full. Stop reading further options. */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* #3001 The option is not valid. Stop processing. */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_CheckOption()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_CheckOption(
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint16 OptionIndex,
  uint32 OptionArrayLength,
  SD_P2VAR(uint16) CoveredOptionLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the length and the type field are part of the payload. */
  if(PduInfoPtr->SduLength >= (OptionIndex + SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE))
  {
    /* #100 Read the length and the type of the option. */
    SD_GET_UINT16(PduInfoPtr, OptionIndex, *CoveredOptionLengthPtr);                                                    /* SBSW_SD_POINTER_WRITE */
    SD_GET_OPTION_TYPE(PduInfoPtr, OptionIndex, optionType);

    /* #101 Check if the length of the option is feasible. */
    if((uint16)(SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE + *CoveredOptionLengthPtr) <= OptionArrayLength)
    {
      /* #1010 Check the length of IPv4 endpoint or multicast endpoint options. */
      if(((optionType == SD_OPTION_IPV4_ENDPOINT) ||
        (optionType == SD_OPTION_IPV4_MULTICAST) ||
        (optionType == SD_OPTION_IPV4_SD_ENDPOINT)) &&
        (*CoveredOptionLengthPtr != SD_OPTION_IPV4_COVERED_LENGTH))
      {
        retVal = E_NOT_OK;
      }

      /* #1011 Check the length of IPv6 endpoint or multicast endpoint options. */
      if(((optionType == SD_OPTION_IPV6_ENDPOINT) ||
        (optionType == SD_OPTION_IPV6_MULTICAST) ||
        (optionType == SD_OPTION_IPV6_SD_ENDPOINT)) &&
        (*CoveredOptionLengthPtr != SD_OPTION_IPV6_COVERED_LENGTH))
      {
        retVal = E_NOT_OK;
      }
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  else
  {
    *CoveredOptionLengthPtr = 0u;                                                                                       /* SBSW_SD_POINTER_WRITE */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_CheckRemoteReboot()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_CheckRemoteReboot(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr,
  SD_P2CONST(Sd_HeaderType) SdHeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfAddressStorageType addressStorageIdx = RxMessageInfoPtr->AddressStorageIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the message was received via unicast or multicast. */
  if(RxMessageInfoPtr->MulticastMessage == TRUE)
  {
    /* #100 Multicast: Check if the device has rebooted. */
    if(((Sd_GetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, PartitionIdx) >= SdHeaderPtr->SessionID) ||
      ((Sd_GetRxMulticastFlagsOfAddressStorage(addressStorageIdx, PartitionIdx) & SD_FLAGS_REBOOT_MASK) == 0u)) &&
      ((SdHeaderPtr->Flags & SD_FLAGS_REBOOT_MASK) != 0u))
    {
      /* #1000 The remote device has rebooted. */
      Sd_SetRebootDetectedOfAddressStorage(addressStorageIdx, TRUE, PartitionIdx);                                      /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */

      /* #1001 Reset the unicast Session ID in order to detect the reboot only once. */
      Sd_SetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, 0u, PartitionIdx);                                    /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
    }

    /* #101 Update the Session ID and Flags field for multicast messages. */
    Sd_SetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, SdHeaderPtr->SessionID, PartitionIdx);                /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
    Sd_SetRxMulticastFlagsOfAddressStorage(addressStorageIdx, SdHeaderPtr->Flags, PartitionIdx);                        /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
  }
  else
  {
    /* #102 Unicast: Check if the device has rebooted. */
    if(((Sd_GetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, PartitionIdx) >= SdHeaderPtr->SessionID) ||
      ((Sd_GetRxUnicastFlagsOfAddressStorage(addressStorageIdx, PartitionIdx) & SD_FLAGS_REBOOT_MASK) == 0u)) &&
      ((SdHeaderPtr->Flags & SD_FLAGS_REBOOT_MASK) != 0u))
    {
      /* #1020 The remote device has rebooted. */
      Sd_SetRebootDetectedOfAddressStorage(addressStorageIdx, TRUE, PartitionIdx);                                      /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */

      /* #1021 Reset the multicast Session ID in order to detect the reboot only once. */
      Sd_SetRxMulticastSessionIdOfAddressStorage(addressStorageIdx, 0u, PartitionIdx);                                  /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
    }

    /* #103 Update the Session ID and Flags field for unicast messages. */
    Sd_SetRxUnicastSessionIdOfAddressStorage(addressStorageIdx, SdHeaderPtr->SessionID, PartitionIdx);                  /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
    Sd_SetRxUnicastFlagsOfAddressStorage(addressStorageIdx, SdHeaderPtr->Flags, PartitionIdx);                          /* SBSW_SD_RX_ADDRESS_STORAGE_IDX */
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_GetSdEndpointOption()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_GetSdEndpointOption(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  SD_P2VAR(Sd_SockAddrContainerType) SdEndpointAddressPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;
  boolean sdEndpointOptionPresent = FALSE;
  Sd_OptionIndexIterType optionIndexIter = 0u;
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  SdEndpointAddressPtr->family = SOAD_AF_INVALID;                                                                       /* SBSW_SD_POINTER_WRITE */

  /* #10 Check if an SD endpoint option is attached and save it. */
  while(optionIndexIter < Sd_GetNrOfOptionsOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx))
  {
    /* #100 Get the option type. */
    const uint16 optionIndex = Sd_GetOptionIndex(optionIndexIter, PartitionIdx);
    SD_GET_OPTION_TYPE(PduInfoPtr, optionIndex, optionType);

    if(optionType == SD_OPTION_IPV4_SD_ENDPOINT)
    {
      /* #1000 Read the address transported in the IPv4 SD endpoint option. */
      optionsRetVal = Sd_Rx_ReadEndpointOption(SOAD_AF_INET, PduInfoPtr->SduDataPtr, optionIndex,
        SdEndpointAddressPtr);                                                                                          /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_PASSING_API_POINTER */

      /* #1001 Only the first SD endpoint option shall be processed. (All further SD endpoint options shall be ignored.) */
      sdEndpointOptionPresent = TRUE;
    }
    else if(optionType == SD_OPTION_IPV6_SD_ENDPOINT)
    {
      /* #1002 Read the address transported in the IPv6 SD endpoint option. */
      optionsRetVal = Sd_Rx_ReadEndpointOption(SOAD_AF_INET6, PduInfoPtr->SduDataPtr, optionIndex,
        SdEndpointAddressPtr);                                                                                          /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_PASSING_API_POINTER */

      /* #1003 Only the first SD endpoint option shall be processed. (All further SD endpoint options shall be ignored.) */
      sdEndpointOptionPresent = TRUE;
    }
    else
    {
      /* #101 No IPv4 or IPv6 SD endpoint option. Keep searching. */
    }

    /* #102 In case that a valid SD endpoint option was found, check if IP address is part of the configured subnet. */
    if((sdEndpointOptionPresent == TRUE) && (optionsRetVal == SD_OPTIONS_OK))
    {
      optionsRetVal = Sd_Rx_CheckSubnet(Sd_GetUnicastRxSoConIdOfInstance(InstanceIdx, PartitionIdx),
        Sd_GetLocalAddressCheckLengthOfInstance(InstanceIdx, PartitionIdx), SdEndpointAddressPtr);                      /* SBSW_SD_PASSING_API_POINTER */

      if(optionsRetVal != SD_OPTIONS_OK)
      {
        retVal = E_NOT_OK;
      }

      break;
    }

    optionIndexIter++;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_GetEndpointOptionParameters()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_Sd_STPAR_6060 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Rx_GetEndpointOptionParameters(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  SdMulticastTypeType McType,
  SD_P2VAR(Sd_FamilyType) FamilyTypePtr,
  SD_P2VAR(Sd_L4ProtocolType) L4ProtocolTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint16 optionIndex = Sd_GetOptionIndex(IndexOfOption, PartitionIdx);
  Sd_OptionType optionType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set parameters to read to invalid and get the option type. */
  *L4ProtocolTypePtr = SD_L4PROTOCOL_NONE;                                                                              /* SBSW_SD_POINTER_WRITE */
  *FamilyTypePtr = SOAD_AF_INVALID;                                                                                     /* SBSW_SD_POINTER_WRITE */
  SD_GET_OPTION_TYPE(PduInfoPtr, optionIndex, optionType);

  /* #20 If option is IPv4 and MulticastType shall be considered, read L4Proto and AddrFamily. */
  if(((optionType == SD_OPTION_IPV4_MULTICAST) && ((McType == SD_MCTYPE_MULTI) || (McType == SD_MCTYPE_ANY))) ||
    ((optionType == SD_OPTION_IPV4_ENDPOINT) && ((McType == SD_MCTYPE_UNI) || (McType == SD_MCTYPE_ANY))))
  {
    SD_GET_L4PROTOCOL_IPV4_TYPE(PduInfoPtr, optionIndex, *L4ProtocolTypePtr);                                           /* SBSW_SD_POINTER_WRITE */
    *FamilyTypePtr = SOAD_AF_INET;                                                                                      /* SBSW_SD_POINTER_WRITE */
  }
  /* #21 Else if option is IPv6 and MulticastType shall be considered, read L4Proto and AddrFamily. */
  else if(((optionType == SD_OPTION_IPV6_MULTICAST) && ((McType == SD_MCTYPE_MULTI) || (McType == SD_MCTYPE_ANY))) ||
    ((optionType == SD_OPTION_IPV6_ENDPOINT) && ((McType == SD_MCTYPE_UNI) || (McType == SD_MCTYPE_ANY))))
  {
    SD_GET_L4PROTOCOL_IPV6_TYPE(PduInfoPtr, optionIndex, *L4ProtocolTypePtr);                                           /* SBSW_SD_POINTER_WRITE */
    *FamilyTypePtr = SOAD_AF_INET6;                                                                                     /* SBSW_SD_POINTER_WRITE */
  }
  else
  {
    /* Ignore unknown and unexpected (McType) options. */
  }

  /* #30 If TCP Multicast Endpoint (invalid), invalidate AddrFamily. */
  if((*L4ProtocolTypePtr == SD_L4PROTOCOL_TCP) &&
     ((optionType & SD_OPTION_MULTICAST_MASK) == SD_OPTION_MULTICAST_MASK)) /*lint !e655 */
  {
    *FamilyTypePtr = SOAD_AF_INVALID;                                                                                   /* SBSW_SD_POINTER_WRITE */
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_ReadEndpointOptions()
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
/* PRQA S 6060 1 */ /* MD_Sd_STPAR_6060 */
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_ReadEndpointOptions(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  Sd_FamilyType FamilyType,
  Sd_L4ProtocolType L4ProtocolType,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an endpoint option was attached */
  if(FamilyType != SOAD_AF_INVALID)
  {
    /* #100 Differentiate between UDP and TCP endpoints. */
    if((L4ProtocolType == SD_L4PROTOCOL_UDP) && (AttachedOptionsPtr->EndpointAddrUdp.family == SOAD_AF_INVALID))
    {
      /* #1000 Read the UDP address transported in the endpoint. */
      optionsRetVal = Sd_Rx_ReadEndpointOption(FamilyType, PduInfoPtr->SduDataPtr,
        Sd_GetOptionIndex(IndexOfOption, PartitionIdx), &AttachedOptionsPtr->EndpointAddrUdp);                         /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
    }
    else if((L4ProtocolType == SD_L4PROTOCOL_TCP) && (AttachedOptionsPtr->EndpointAddrTcp.family == SOAD_AF_INVALID))
    {
      /* #1001 Read the TCP address transported in the endpoint. */
      optionsRetVal = Sd_Rx_ReadEndpointOption(FamilyType, PduInfoPtr->SduDataPtr,
        Sd_GetOptionIndex(IndexOfOption, PartitionIdx), &AttachedOptionsPtr->EndpointAddrTcp);                         /* SBSW_SD_PDU_INFO_PTR */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
    }
    else
    {
      /* #1002 Reject subscription because of an undefined protocol type or because the entry references multiple endpoint options of the same type. */
      optionsRetVal = SD_OPTIONS_NOT_OK;
    }
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_ReadEndpointOption()
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
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_ReadEndpointOption(
  Sd_FamilyType FamilyType,
  SD_P2CONST(uint8) DataPtr,
  uint16 DataIndex,
  SD_P2VAR(Sd_SockAddrContainerType) EndpointAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;
  EndpointAddrPtr->family = FamilyType;                                                                                 /* SBSW_SD_POINTER_WRITE */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Data is saved in Network Byte Order. */

  if(FamilyType == SOAD_AF_INET)
  {
    /* #100 Save IPv4 endpoint option. */
    /* PRQA S 0314 1 */ /* MD_Sd_Dir-1.1_0314_copy */
    Sd_Copy((SD_P2VAR(void)) EndpointAddrPtr->data, (SD_P2CONST(void)) &DataPtr[DataIndex + SD_OPTION_IPV4_ADDRESS_OFFSET], SD_OPTION_IPV4_ADDRESS_SIZE);   /* SBSW_SD_SOCK_ADDR_SIZE */ /* SBSW_SD_RX_MESSAGE_SIZE */

    if(EndpointAddrPtr->data[0u] == 0u)
    {
      /* #1000 Mark the endpoint address as invalid if no definite address is defined. */
      EndpointAddrPtr->family = SOAD_AF_INVALID;                                                                        /* SBSW_SD_POINTER_WRITE */
      optionsRetVal = SD_OPTIONS_NOT_OK;
    }

    /* PRQA S 0314 2 */ /* MD_Sd_Dir-1.1_0314_copy */
    Sd_Copy((SD_P2VAR(void)) &EndpointAddrPtr->port,                                                                    /* SBSW_SD_SOCK_ADDR_SIZE */ /* SBSW_SD_RX_MESSAGE_SIZE */
      (SD_P2CONST(void)) &DataPtr[DataIndex + SD_OPTION_IPV4_PORTNUMBER_OFFSET], SD_OPTION_IPV4_PORTNUMBER_SIZE);
  }
  else /* SOAD_AF_INET6 == FamilyType. */
  {
    /* #101 Save IPv6 endpoint option. */
    /* PRQA S 0314 1 */ /* MD_Sd_Dir-1.1_0314_copy */
    Sd_Copy((SD_P2VAR(void)) EndpointAddrPtr->data, (SD_P2CONST(void)) &DataPtr[DataIndex + SD_OPTION_IPV6_ADDRESS_OFFSET], SD_OPTION_IPV6_ADDRESS_SIZE);   /* SBSW_SD_SOCK_ADDR_SIZE */ /* SBSW_SD_RX_MESSAGE_SIZE */

    if((EndpointAddrPtr->data[0u] | EndpointAddrPtr->data[1u] | EndpointAddrPtr->data[2u] | EndpointAddrPtr->data[3u]) == 0u)
    {
      /* #1010 Mark the endpoint address as invalid if no definite address is defined. */
      EndpointAddrPtr->family = SOAD_AF_INVALID;                                                                        /* SBSW_SD_POINTER_WRITE */
      optionsRetVal = SD_OPTIONS_NOT_OK;
    }

    /* PRQA S 0314 2 */ /* MD_Sd_Dir-1.1_0314_copy */
    Sd_Copy((SD_P2VAR(void)) &EndpointAddrPtr->port,                                                                    /* SBSW_SD_SOCK_ADDR_SIZE */ /* SBSW_SD_RX_MESSAGE_SIZE */
      (SD_P2CONST(void)) &DataPtr[DataIndex + SD_OPTION_IPV6_PORTNUMBER_OFFSET], SD_OPTION_IPV6_PORTNUMBER_SIZE);
  }

  if(EndpointAddrPtr->port == SD_TCPIP_PORT_ANY)
  {
    /* #102 Mark the endpoint address as invalid if no definite port is defined. */
    EndpointAddrPtr->family = SOAD_AF_INVALID;                                                                          /* SBSW_SD_POINTER_WRITE */
    optionsRetVal = SD_OPTIONS_NOT_OK;
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_CheckConfigOption()
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
 */
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_CheckConfigOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  uint8 IndexOfOption,
  SD_P2VAR(Sd_ConfigOptionStringType) ConfigOptionPtrArrayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint16 optionIndex = Sd_GetOptionIndex(IndexOfOption, PartitionIdx);
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;
  boolean correctTermination = FALSE;
  uint8 itemLength;
  uint16 lengthOfOption, reservedIndex, tmpIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get configuration option header. */
  SD_GET_UINT16(PduInfoPtr, optionIndex, lengthOfOption);

  /* #20 Skip Reserved Field. */
  reservedIndex = optionIndex + SD_OPTION_LENGTH_SIZE + SD_OPTION_TYPE_SIZE;
  tmpIndex = SD_OPTION_RESERVED_SIZE;

  /* #30 Check configuration string. */
  while((lengthOfOption > tmpIndex) && (optionsRetVal == SD_OPTIONS_OK))
  {
    /* #300 Get length of capability string. */
    SD_GET_UINT8(PduInfoPtr, reservedIndex + tmpIndex, itemLength);
    tmpIndex++;

    /* #301 Check if the current byte defines the termination of the capability string. */
    if(itemLength == 0u)
    {
      /* #3010 Termination of capability string. Exit loop. */
      correctTermination = TRUE;
      break;
    }

    /* #302 Check if the length of the item exceeds the length of the entire option. */
    if(lengthOfOption >= (itemLength + tmpIndex))
    {
      tmpIndex += itemLength;
    }
    else
    {
      optionsRetVal = SD_OPTIONS_NOT_OK;
    }
  }

  /* #40 Check if the option was terminated correctly. */
  if(correctTermination == TRUE)
  {
    Sd_SizeOfOptionIndexType optionIdx;

    /* #400 Save reference to the configuration string in the list of configuration options. */
    for(optionIdx = 0u; optionIdx < Sd_GetMaxNrOptionsPerEntry(PartitionIdx); optionIdx++)
    {
      if(ConfigOptionPtrArrayPtr[optionIdx] == NULL_PTR)
      {
        ConfigOptionPtrArrayPtr[optionIdx] = &PduInfoPtr->SduDataPtr[reservedIndex + SD_OPTION_RESERVED_SIZE];          /* SBSW_SD_CONFIG_OPTION_PTR_ARRAY_SIZE */
        break;
      }
    }

    /* #401 Check if the number of attached options exceeds number defined in BSWMD configuration parameter SdMaxNrOptionsPerMsg. */
    if(optionIdx == Sd_GetMaxNrOptionsPerEntry(PartitionIdx))
    {
      optionsRetVal = SD_OPTIONS_NOT_OK;
    }
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_FillConfigOptionPtrArray()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_FillConfigOptionPtrArray(
  SD_P2VAR(Sd_ConfigOptionStringType) ConfigOptionPtrArrayPtr,
  Sd_SizeOfCapabilityRecordType InstanceCapabilityIdx,
  Sd_SizeOfCapabilityRecordType ServiceCapabilityIdx,
  Sd_SizeOfCapabilityRecordType ServiceCapabilityInvIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 configOptionIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add hostname configuration option if configured. */
  if(InstanceCapabilityIdx != SD_NO_CAPABILITYRECORDSTARTIDXOFINSTANCE)
  {
    ConfigOptionPtrArrayPtr[configOptionIdx] = Sd_GetAddrCapabilityRecord(InstanceCapabilityIdx);                       /* SBSW_SD_FIXED_SIZE_OF_CONFIGOPTIONPTRARRAY */
    configOptionIdx++;
  }

  /* #20 Add configuration option of service level if configured. */
  if(ServiceCapabilityIdx != ServiceCapabilityInvIdx)
  {
    ConfigOptionPtrArrayPtr[configOptionIdx] = Sd_GetAddrCapabilityRecord(ServiceCapabilityIdx);                        /* SBSW_SD_FIXED_SIZE_OF_CONFIGOPTIONPTRARRAY */
  }
}

/**********************************************************************************************************************
 *  Sd_State_HandleRunMode()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_State_HandleRunMode(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the run mode of the current instance. */
  switch(Sd_GetRunModeOfInstanceDyn(InstanceIdx, PartitionIdx))
  {
    case SD_RUN_MODE_INITIAL_RUN:
    case SD_RUN_MODE_NEW_IP_ASSIGNED:
      /* #100 The module was re-initialized by a call of Sd_Init() or Sd_LocalIpAddrAssignmentChg(). */
      Sd_Init_ReInitialize(InstanceIdx, PartitionIdx);

      if(Sd_GetRunModeOfInstanceDyn(InstanceIdx, PartitionIdx) == SD_RUN_MODE_INITIAL_RUN)
      {
        Sd_SetRunModeOfInstanceDyn(InstanceIdx, SD_RUN_MODE_RUNNING_NO_IP, PartitionIdx);                               /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      }
      else
      {
        Sd_SetRunModeOfInstanceDyn(InstanceIdx, SD_RUN_MODE_RUNNING, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      }
      break;

    case SD_RUN_MODE_IP_UNASSIGNED:
      /* #101 The IP assignment of the local SD address was removed. */

      /* #102 Reset the state of all client and server services. */
      Sd_UtilClient_LocalIpAddrAssignmentChgReset(InstanceIdx, PartitionIdx);
      Sd_UtilServer_LocalIpAddrAssignmentChgReset(InstanceIdx, PartitionIdx);

      /* #103 Clear the list of pending send entries. */
      Sd_Util_ClearSendEntryList(InstanceIdx, PartitionIdx);

      /* #104 Update the run mode of the instance. */
      Sd_SetRunModeOfInstanceDyn(InstanceIdx, SD_RUN_MODE_WAIT_IP_ASSIGNMENT, PartitionIdx);                            /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      break;

    default:
      /* #105 The instance is in state SD_RUN_MODE_RUNNING, SD_RUN_MODE_RUNNING_NO_IP or SD_RUN_MODE_WAIT_IP_ASSIGNMENT. Nothing to do. */
      break;
  }
}

/**********************************************************************************************************************
 *  Sd_State_Process()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_State_Process(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the state machines have to be triggered because of a pending timer event. */
  if(TRUE == Sd_Util_CheckIfTimerExceeded(PartitionIdx,
       Sd_GetNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx), TRUE))
  {
    Sd_SizeOfServerType serverIdx;
    Sd_SizeOfClientType clientIdx;

    /* #100 Reset the timer. */
    Sd_Util_ResetTimerToInvalid(Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx));                 /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */

    /* #101 Iterate over all server state machines of the instance. */
    SdHookBegin_State_Process_ServerSMs();
    for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx, PartitionIdx);
        serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx, PartitionIdx);
        serverIdx++)
    {
      /* #1010 Only execute state machine if requested for server. */
      SD_BEGIN_CRITICAL_SECTION();
      if(Sd_IsStateMachineExecutionRequestedOfServerDyn(serverIdx, PartitionIdx))
      {
        Sd_SetStateMachineExecutionRequestedOfServerDyn(serverIdx, FALSE, PartitionIdx);                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        SD_END_CRITICAL_SECTION();
        Sd_StateServer_Process(serverIdx, PartitionIdx);
      }
      else
      {
        SD_END_CRITICAL_SECTION();
      }
    }
    SdHookEnd_State_Process_ServerSMs();

    /* #102 Iterate over all client state machines of the instance. */
    SdHookBegin_State_Process_ClientSMs();
    for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx, PartitionIdx);
        clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx, PartitionIdx);
        clientIdx++)
    {
      SD_BEGIN_CRITICAL_SECTION();
      /* #1020 Only execute state machine if requested for client. */
      if(Sd_IsStateMachineExecutionRequestedOfClientDyn(clientIdx, PartitionIdx))
      {
        Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, FALSE, PartitionIdx);                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        /* Copy received event to client. */
        Sd_SetEventMainOfClientDyn(clientIdx, Sd_GetEventRxOfClientDyn(clientIdx, PartitionIdx), PartitionIdx);         /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_SetEventRxOfClientDyn(clientIdx, SD_CLIENT_EVENT_SERVICE_NONE, PartitionIdx);                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        SD_END_CRITICAL_SECTION();
        Sd_StateClient_Process(clientIdx, PartitionIdx);
      }
      else
      {
        SD_END_CRITICAL_SECTION();
      }
    }
    SdHookEnd_State_Process_ClientSMs();
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_Process()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_Rx_Process(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType RxSoConId,
  SD_P2VAR(Sd_RxMessageInfoType) MessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_HeaderType sdHeader = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read and validate message header. */
  if (Sd_Rx_ProcessHeader(InstanceIdx, PartitionIdx, MessageInfoPtr->PduInfoPtr, &sdHeader) == E_OK)                    /* SBSW_SD_PUBLIC_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */
  {
    Std_ReturnType retVal = E_NOT_OK;
    Sd_SockAddrContainerType remoteAddr = { 0 };

    /* #20 Generate an index of all attached options to be able to address them directly. */
    Sd_Rx_IndexOptionArray(PartitionIdx, MessageInfoPtr->PduInfoPtr);                                                   /* SBSW_SD_PUBLIC_API_POINTER */

    /* #30 Check if an SD endpoint option was attached and save it. */
    retVal = Sd_Rx_GetSdEndpointOption(InstanceIdx, PartitionIdx, MessageInfoPtr->PduInfoPtr, &remoteAddr);             /* SBSW_SD_PUBLIC_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

    if (remoteAddr.family == SOAD_AF_INVALID)
    {
      /* #300 No valid SD endpoint was attached, use the remote address of the sender. */
      /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
      retVal |= SoAd_GetRcvRemoteAddr(RxSoConId, (SD_P2VAR(SoAd_SockAddrType))(&remoteAddr));                           /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_SOCK_ADDR_CAST */
    }

    if (retVal == E_OK)
    {
      /* #40 Try to save the remote address. */
      MessageInfoPtr->AddressStorageIdx = Sd_Rx_SaveAddr(InstanceIdx, PartitionIdx, &remoteAddr);                       /* SBSW_SD_PUBLIC_API_POINTER */  /* SBSW_SD_REFERENCE_OF_VARIABLE */

      if (MessageInfoPtr->AddressStorageIdx < Sd_GetSizeOfAddressStorage(PartitionIdx))
      {
        /* #400 Check if the remote service has rebooted. */
        Sd_Rx_CheckRemoteReboot(PartitionIdx, MessageInfoPtr, &sdHeader);                                               /* SBSW_SD_REFERENCE_OF_VARIABLE */

        /* #401 Process attached entries. */
        Sd_Rx_ProcessEntryArray(InstanceIdx, PartitionIdx, MessageInfoPtr);                                             /* SBSW_SD_REFERENCE_OF_VARIABLE */
      }
      else
      {
        /* #402 Remote address could not be saved (and a DET error was reported). Discard the received message. */
      }
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Rx_ProcessHeader()
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
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Rx_ProcessHeader(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(PduInfoType) PduInfoPtr,
  SD_P2VAR(Sd_HeaderType) SdHeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* PRQA S 2981 1 */ /* MD_Sd_Rule-2.2_2981 */
  uint32 lengthEntriesArray, lengthOptionsArray= 0u;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Drop message if it does not include a complete SD header as well as the length field for entries and options. */
  if(PduInfoPtr->SduLength >= (SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE))
  {
    retVal = E_OK;

    /* #100 Read the header, message ID and Length field of the SD header will be handled by the SoAd. */
    SD_GET_UINT16(PduInfoPtr, SD_HEADER_CLIENTID_OFFSET, SdHeaderPtr->ClientID);                                        /* SBSW_SD_POINTER_WRITE */
    SD_GET_UINT16(PduInfoPtr, SD_HEADER_SESSIONID_OFFSET, SdHeaderPtr->SessionID);                                      /* SBSW_SD_POINTER_WRITE */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_PROTOCOLVERSION_OFFSET, SdHeaderPtr->ProtocolVersion);                           /* SBSW_SD_POINTER_WRITE */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_INTERFACEVERSION_OFFSET, SdHeaderPtr->InterfaceVersion);                         /* SBSW_SD_POINTER_WRITE */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_MESSAGETYPE_OFFSET, SdHeaderPtr->MessageType);                                   /* SBSW_SD_POINTER_WRITE */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_RETURNCODE_OFFSET, SdHeaderPtr->ReturnCode);                                     /* SBSW_SD_POINTER_WRITE */
    SD_GET_UINT8(PduInfoPtr, SD_HEADER_FLAGS_OFFSET, SdHeaderPtr->Flags);                                               /* SBSW_SD_POINTER_WRITE */
    SD_GET_UINT24(PduInfoPtr, SD_HEADER_RESERVED_OFFSET, SdHeaderPtr->Reserved);                                        /* SBSW_SD_POINTER_WRITE */

    /* #101 Check the predefined fields of the header. */
    if((SdHeaderPtr->ProtocolVersion != SD_HEADER_PROTOCOL_VERSION) ||
      (SdHeaderPtr->InterfaceVersion != SD_HEADER_INTERFACE_VERSION) ||
      (SdHeaderPtr->MessageType != SD_HEADER_MESSAGE_TYPE) ||
      (SdHeaderPtr->ReturnCode != SD_HEADER_RETURN_CODE) ||
      (SdHeaderPtr->ClientID != SD_HEADER_CLIENT_ID))
    {
      /* #1010 The header is not compliant with the statically defined values. */
      retVal = E_NOT_OK;
    }
    else if(0u == (SdHeaderPtr->Flags & SD_FLAGS_UNICAST_MASK))
    {
      /* #1011 If unicast flag is not set, ignore message. */
      retVal = E_NOT_OK;
    }
    else
    {
      /* #1012 Predefined header fields are ok. Process the received SD payload data. */
      SD_GET_UINT32(PduInfoPtr, SD_HEADER_LENGTH, lengthEntriesArray);

      /* #1013 Check if the entries array fits into the payload. */
      if(((uint32)PduInfoPtr->SduLength - SD_HEADER_LENGTH - SD_HEADER_LENGTHOFENTRIESARRAY_SIZE -
          SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE) >= lengthEntriesArray)
      {
        SD_GET_UINT32(PduInfoPtr, SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE +
          lengthEntriesArray, lengthOptionsArray);
      }
      else
      {
        /* #10130 The entries array exceeds the payload. */
        retVal = E_NOT_OK;
      }

      /* #1014 Check if the payload can contain all mentioned entry and option arrays. */
      if((retVal == E_OK) &&
        (PduInfoPtr->SduLength >= (SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + lengthEntriesArray +
        SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE + lengthOptionsArray)))
      {
        /* #10140 Header fields of message are valid. */
        retVal = E_OK;
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }

  if(retVal != E_OK)
  {
    /* #102 The message is malformed, Report Dem event and increase invalid SdMessage counter. */
    Sd_Util_ReportDemEvent(Sd_GetEMalformedMsgIdOfInstance(InstanceIdx, PartitionIdx));
#if(SD_GET_RESET_MEASUREMENT_DATA_API == STD_ON)
    Sd_Measure_Inc(SD_MEAS_INVALID_SD_MESSAGE, 1u);
#endif /* SD_GET_RESET_MEASUREMENT_DATA_API */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_ProcessEntryArray()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Rx_ProcessEntryArray(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_RxMessageInfoType) RxMessageInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 indexEntry;
  uint32 lengthEntriesArray;

  /* ----- Implementation ----------------------------------------------- */
  SD_GET_UINT32(RxMessageInfoPtr->PduInfoPtr, SD_HEADER_LENGTH, lengthEntriesArray);
  indexEntry = SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE;

  /* #10 Iterate through all entries of the array. */
  while(((uint32)indexEntry + SD_ENTRY_SIZE) <= (SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + lengthEntriesArray))
  {
    /* #100 Process the current entry and all associated options. */
    Sd_EntryType rxEntryType;
    SD_GET_ENTRY_TYPE(RxMessageInfoPtr->PduInfoPtr, indexEntry, rxEntryType);

    /* #101 Check if the entry has to be processed by a server or client service. */
    if((rxEntryType == SD_ENTRY_FIND_SERVICE) || (rxEntryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP))
    {
      /* #1010 Entry has to be processed by a server. */
      Sd_RxServer_ProcessEntry(InstanceIdx, PartitionIdx, indexEntry, RxMessageInfoPtr);                                /* SBSW_SD_PASSING_API_POINTER */
    }
    else if((rxEntryType == SD_ENTRY_OFFER_SERVICE) || (rxEntryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK))
    {
      /* #1011 Entry has to be processed by a client. */
      Sd_RxClient_ProcessEntry(InstanceIdx, PartitionIdx, indexEntry, RxMessageInfoPtr);                                /* SBSW_SD_PASSING_API_POINTER */
    }
    else
    {
      /* #1012 Ignore entry with unknown type. */
    }

    /* #102 The length of all entry arrays (format type 1 and format type 2) is 16 bytes. */
    indexEntry += SD_ENTRY_SIZE;
  }
}

/**********************************************************************************************************************
 *  Sd_HandleReboot()
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
SD_LOCAL FUNC(boolean, SD_CODE) Sd_HandleReboot(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  boolean rebootPending = FALSE;
  Sd_SizeOfAddressStorageType addrStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx, PartitionIdx);

  /* #10 Iterate over remote SD Instances. */
  while((addrStorageIdx < Sd_GetAddressStorageEndIdxOfInstance(InstanceIdx, PartitionIdx)) &&
        (!rebootPending))
  {
    /* #100 If reboot has been detected for remote instance. */
    if(Sd_IsRebootDetectedOfAddressStorage(addrStorageIdx, PartitionIdx))
    {
      /* #1000 Process SoCons which are used for rebooted remote SD Instance. */
      Sd_HandleReboot_SoConsUdp(InstanceIdx, PartitionIdx, addrStorageIdx);
      Sd_HandleReboot_SoConsTcp(InstanceIdx, PartitionIdx, addrStorageIdx);

      /* #1001 Generate and process StopOffer/StopSubscribe events for services of rebooted remote SD Instance. */
      Sd_HandleReboot_StopEvents(InstanceIdx, PartitionIdx, addrStorageIdx);

      /* #1002 Clear flag of detected reboot and set return value to TRUE. */
      Sd_SetRebootDetectedOfAddressStorage(addrStorageIdx, FALSE, PartitionIdx);                                        /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      rebootPending = TRUE;
    }
    else
    {
      /* Nothing to do, keep return value FALSE. */
    }

    addrStorageIdx++;
  }

  return rebootPending;
}

/**********************************************************************************************************************
 *  Sd_HandleReboot_StopEvents()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_HandleReboot_StopEvents(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddrStorageIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;
  Sd_SizeOfServerType serverIdx;
  Sd_SizeOfEventHandlerType eventHandlerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all clients of the Sd Instance. */
  for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx, PartitionIdx);
    clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx, PartitionIdx);
    clientIdx++)
  {
    /* #100 Check if the client is connected to the rebooted remote Sd Instance. */
    if(AddrStorageIdx == Sd_GetAddressStorageServerIdxOfClientDyn(clientIdx, PartitionIdx))
    {
      /* #1000 Save reboot of remote server as if a StopOfferService entry was received. */
      Sd_SetEventMainOfClientDyn(clientIdx, SD_CLIENT_EVENT_SERVICE_STOP_OFFER, PartitionIdx);                        /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      Sd_StateClient_Process(clientIdx, PartitionIdx);
    }
  }

  /* #20 Iterate over all servers of the Sd Instance. */
  for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx, PartitionIdx);
    serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx, PartitionIdx);
    serverIdx++)
  {
    /* #200 Iterate over all event handlers of the server. */
    for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(serverIdx, PartitionIdx);
      eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(serverIdx, PartitionIdx);
      eventHandlerIdx++)
    {
      /* #2000 Set the TTL of all subscribed clients of rebooted eventhandler to 0 (= StopSubscribe). */
      (void)Sd_RxServer_FindClientInList(eventHandlerIdx, PartitionIdx, AddrStorageIdx, NULL_PTR, FALSE);             /* SBSW_SD_FUNCTION_CALL_NULL_PTR */
    }

    /* #201 If SM execution is requested (clients found), run SM to unconfigure clients. */
    if(Sd_IsStateMachineExecutionRequestedOfServerDyn(serverIdx, PartitionIdx))
    {
      Sd_StateServer_Process(serverIdx, PartitionIdx);
      Sd_SetStateMachineExecutionRequestedOfServerDyn(serverIdx, FALSE, PartitionIdx);                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_HandleReboot_SoConsTcp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_HandleReboot_SoConsTcp(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddrStorageIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_TcpSoConIdIterType tcpSoConIdIdx;
  Sd_SockAddrContainerType tmpAddr;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all TCP socket connections. */
  for(tcpSoConIdIdx = Sd_GetTcpSoConIdStartIdxOfInstance(InstanceIdx, PartitionIdx);
      tcpSoConIdIdx < Sd_GetTcpSoConIdEndIdxOfInstance(InstanceIdx, PartitionIdx);
      tcpSoConIdIdx++)
  {
    const SoAd_SoConIdType soConId = Sd_GetTcpSoConId(tcpSoConIdIdx, PartitionIdx);

    /* #100 Get Remote Address of TCP socket connection. */
    retVal = Sd_SoAd_GetRemoteAddr(soConId, &tmpAddr);                                                                /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Check if the saved remote address is equivalent to the remote address of the rebooted service. */
    /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&
      (SD_MATCH_TRUE == Sd_Util_CompareSockAddrIp(                                                                    /* SBSW_SD_REFERENCE_OF_VARIABLE */
        Sd_GetAddrAddrOfAddressStorage(AddrStorageIdx, PartitionIdx), &tmpAddr, 0u)))
    {
      /* #1010 Socket connection was used for the rebooted remote client, release remote address. */
      SoAd_ForceReleaseRemoteAddr(soConId);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_HandleReboot_SoConsUdp()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_HandleReboot_SoConsUdp(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddrStorageIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_UdpSoConIterType udpSoConIdx;
  Sd_SockAddrContainerType tmpAddr;
  boolean abortParameter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all UDP socket connections. */
  for(udpSoConIdx = Sd_GetUdpSoConStartIdxOfInstance(InstanceIdx, PartitionIdx);
      udpSoConIdx < Sd_GetUdpSoConEndIdxOfInstance(InstanceIdx, PartitionIdx);
      udpSoConIdx++)
  {
    const SoAd_SoConIdType soConId = Sd_GetIdOfUdpSoCon(udpSoConIdx, PartitionIdx);

    /* #100 Get Remote Address of socket connection. */
    retVal = Sd_SoAd_GetRemoteAddr(soConId, &tmpAddr);                                                                /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #101 Check if the saved remote address is equivalent to the remote address of the rebooted service. */
    /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
    if((retVal == E_OK) &&
      (SD_MATCH_TRUE == Sd_Util_CompareSockAddrIp(                                                                    /* SBSW_SD_REFERENCE_OF_VARIABLE */
        Sd_GetAddrAddrOfAddressStorage(AddrStorageIdx, PartitionIdx), &tmpAddr, 0u)))
    {
      /* #1010 Do not abort if remote address of socket is preconfigured. */
      if(Sd_IsPreconfiguredOfUdpSoCon(udpSoConIdx, PartitionIdx) == TRUE)
      {
        abortParameter = FALSE;
      }
      else
      {
        abortParameter = TRUE;
      }

      /* #1011 Socket connection was used for the rebooted remote client, close and reopen it. */
      (void)SoAd_CloseSoCon(soConId, abortParameter);
      (void)SoAd_OpenSoCon(soConId);
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_ProcessPendingMessages()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_ProcessPendingMessages(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean ForceTransmission)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean tmpForceTransmission = ForceTransmission;
  Sd_SizeOfSendEntryType iteration;
  Sd_SizeOfAddressStorageType addressStorageIdx;
  Sd_SizeOfSendEntryType sentEntries;

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();
  /* #10 Skip transmission if TxPath is locked by other context. */
  if((Sd_IsTxPathLockedOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx) == FALSE) &&
     (Sd_GetRunModeOfInstanceDyn(InstanceIdx, PartitionIdx) == SD_RUN_MODE_RUNNING))
  {
    Sd_SetTxPathLockedOfPartitionDataDyn(SD_PARTITION_DATA_IDX, TRUE, PartitionIdx);                                    /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */

    /* #100 Iterate until all messages are transmitted, but maximally SendEntryNrPendingIdx-times. */
    iteration = Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, PartitionIdx);

    /* #101 Check if the list with pending send entries is full. */
    if(Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, PartitionIdx) >=
         Sd_GetSendEntryCapacityOfInstance(InstanceIdx, PartitionIdx))
    {
      /* #1010 The send entry list is full, force transmission. */
      tmpForceTransmission = TRUE;
    }

    SD_END_CRITICAL_SECTION();

    /* #200 Serialize SdMessages until there are no entries left to send. */
    while((iteration != 0u) && (Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, PartitionIdx) != 0u))
    {
      iteration--;

      /* #201 Get address storage index of next entry which is ready for transmission. */
      addressStorageIdx = Sd_Tx_CheckIfMessageReadyForTransmission(InstanceIdx, PartitionIdx, tmpForceTransmission);

      /* #202 Check if entries for a destination are ready for transmission. */
      if(addressStorageIdx < Sd_GetSizeOfAddressStorage(PartitionIdx))
      {
        sentEntries = 1u;
        /* #2020 Generate one SdMessage and transmit it if there are pending send entries and the instance is up. */
        while((sentEntries != 0u) &&
              (Sd_GetStateOfInstanceDyn(InstanceIdx, PartitionIdx) == SD_INSTANCE_UP_AND_CONFIGURED))
        {
          sentEntries = Sd_Tx_SerializeMessage(InstanceIdx, PartitionIdx, addressStorageIdx);
        }
      }
    }

    SD_BEGIN_CRITICAL_SECTION();
    Sd_SetTxPathLockedOfPartitionDataDyn(SD_PARTITION_DATA_IDX, FALSE, PartitionIdx);                                   /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
    SD_END_CRITICAL_SECTION();
  }
  else
  {
    SD_END_CRITICAL_SECTION();
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_CheckIfMessageReadyForTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Sd_SizeOfAddressStorageType, SD_CODE) Sd_Tx_CheckIfMessageReadyForTransmission(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean ForceSend)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_AddressStorageIterType addressStorageIter;
  Sd_SizeOfSendEntryType invalidIdx = Sd_GetSizeOfSendEntry(PartitionIdx);
  Sd_SizeOfAddressStorageType addressStorageIdx = Sd_GetSizeOfAddressStorage(PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();
  /* #10 Check if send entries are added to any address storage list of instance. */
  if(Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, PartitionIdx) > 0u)
  {
    /* #100 Iterate over all address storages. */
    for(addressStorageIter = Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx, PartitionIdx);
        addressStorageIter < Sd_GetAddressStorageEndIdxOfInstance(InstanceIdx, PartitionIdx);
        addressStorageIter++)
    {
      /* #1000 Check if address storage contains any send entries. */
      if(Sd_GetSendEntryHeadIdxOfAddressStorage(addressStorageIter, PartitionIdx) < invalidIdx)
      {
        /* #10000 In case the timer expired or transmission is enforced, return the AddressStorageIdx. */
        if((Sd_Util_CheckIfTimerExceeded(PartitionIdx,
              Sd_GetTimeToSendOfAddressStorage(addressStorageIter, PartitionIdx), TRUE) == TRUE) ||
           (ForceSend == TRUE))
        {
          addressStorageIdx = (Sd_SizeOfAddressStorageType)addressStorageIter;
          break;
        }
      }
    }
  }
  SD_END_CRITICAL_SECTION();

  return addressStorageIdx;
}

/**********************************************************************************************************************
 *  Sd_Tx_CheckIfEntryShallBeSkipped()
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
 */
SD_LOCAL_INLINE FUNC(Sd_SendEntrySkipType, SD_CODE) Sd_Tx_CheckIfEntryShallBeSkipped(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SendEntrySkipType skipEntry = SD_SENDENTRY_ACCEPT;
  Sd_SizeOfSendEntryType entryType = Sd_GetEntryTypeOfSendEntry(SendEntryIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the selected entry is a Subscribe entry. */
  if(entryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP)
  {
    Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx =
      Sd_GetConsumedEventgroupIdxOfSendEntry(SendEntryIdx, PartitionIdx);
    Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx);

    /* #100 Check if the TCP connection is established. */
    /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
    if(Sd_ConsumedEventGroupUsesTcp(consumedEventgroupIdx, PartitionIdx) &&
      (Sd_GetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID) &&
      (Sd_Util_GetSoConMode(Sd_GetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx),
         PartitionIdx) != SD_SOCON_MODE_ONLINE))
    {
      /* #1000 The TCP connection is not established. Skip the transmission of this message. */
      skipEntry = SD_SENDENTRY_SKIP;
    }

    /* #101 Check if the UDP connection is established. */
    /* PRQA S 3415 3 */ /* MD_Sd_12.4_3415 */
    if(Sd_ConsumedEventGroupUsesUdp(consumedEventgroupIdx, PartitionIdx) &&
      (Sd_GetUdpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) != SOAD_INV_SO_CON_ID) &&
      (Sd_Util_GetSoConMode(Sd_GetUdpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx),
         PartitionIdx) != SD_SOCON_MODE_ONLINE))
    {
      /* #1010 The UDP connection is not established. Skip the transmission of this message. */
      skipEntry = SD_SENDENTRY_SKIP;
    }

    /* #102 Check if the client service and/or eventgroup is still requested. */
    if(((Sd_GetStateSwcOfClientDyn(clientIdx, PartitionIdx) == SD_CLIENT_SERVICE_RELEASED) ||
      (Sd_GetServiceModeRequestOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx) == SD_CONSUMED_EVENTGROUP_RELEASED) ||
      (Sd_GetCurrentStateOfClientDyn(clientIdx, PartitionIdx) == SD_CLIENT_SERVICE_DOWN)))
    {
      /* #1020 The eventgroup is not requested or not available anymore. Remove the message. */
      skipEntry = SD_SENDENTRY_DELETE;
    }
  }
  /* #20 Check if the entry corresponds to a server which is not available. */
  else if(((entryType == SD_ENTRY_OFFER_SERVICE) ||
    (entryType == SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK)) &&
    (Sd_GetStateSwcOfServerDyn(Sd_GetServerIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx) == SD_SERVER_SERVICE_DOWN))
  {
    /* #200 The server is not available anymore. Remove the message. */
    skipEntry = SD_SENDENTRY_DELETE;
  }
  else
  {
    /* #300 Otherwise, the entry shall not be skipped. */
  }

  return skipEntry;
}

/**********************************************************************************************************************
 *  Sd_Tx_CheckAndRestartRetry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_CheckAndRestartRetry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfSendEntry(SendEntryIdx, PartitionIdx);

  /* #10 Check if the entry is a subscription configured with retry functionality. */
  if((Sd_GetEntryTypeOfSendEntry(SendEntryIdx, PartitionIdx) == SD_ENTRY_SUBSCRIBE_EVENTGROUP) &&
    (Sd_GetOfferedTTLOfClientDyn(Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx) == SD_TTL_INFINITE) &&
    (Sd_GetRetryMaxOfInstance(instanceIdx, PartitionIdx) > 0u))
  {
    /* #100 Set the retry subscription timer for the time of the next retry. */
    Sd_Util_SetTimerOffsetWithTrigger(
      Sd_GetAddrRetryTimerOfClientDyn(Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx),            /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      Sd_GetRetryDelayOfInstance(instanceIdx, PartitionIdx), instanceIdx, PartitionIdx);
    Sd_SetStateMachineExecutionRequestedOfClientDyn(Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx), TRUE,       /* SBSW_SD_CSL03_CSL05 */
      PartitionIdx);
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeMessage()
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
 *
 *
 */
/* PRQA S 6010, 6030, 6050 1 */ /* MD_Sd_STPTH_6010, MD_MSR_STCYC, MD_Sd_STCAL_6050 */
SD_LOCAL FUNC(Sd_SizeOfSendEntryType, SD_CODE) Sd_Tx_SerializeMessage(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType numSerializedEntries = 0u;
  Std_ReturnType retValSoad;
  boolean continueIter = TRUE;
  boolean entrySkipped = FALSE;
  boolean bufferTooSmall = FALSE;
  uint16 sizeEntriesArray;
  PduInfoType pduInfo;
  Sd_SizeOfSendEntryType sendEntryIdx;
  Sd_SizeOfSendEntryType maxNumberOfEntriesToSerialize;
  uint32 sizeOfOptionsArray;
  uint16 payloadEntryIdx = 0u;   /* PRQA S 2981 */ /* MD_Sd_Rule-2.2_2981 */
  Sd_OptionArrayInfoType optionArrayInfo;
  uint16 optionArrayEndIdxPrev;
  Sd_SendEntrySkipType skipEntry;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine a reasonable maximum number of entries to be serialized depending on the max payload. */
  /* NOTE: Sd_GetPayloadLengthOfPartition() is enforced to be >=48 by the generator. */
  maxNumberOfEntriesToSerialize = (Sd_SizeOfSendEntryType)((Sd_GetSizeOfPayload(PartitionIdx) -
    (Sd_GetSizeOfPayload(PartitionIdx) / 4u)) / SD_ENTRY_SIZE);

  /* #20 Correct if number of entries in queue is smaller than maximum value. Calculate reserved space for EntriesArray. */
  if(maxNumberOfEntriesToSerialize > Sd_GetSendEntryNrPendingIdxOfAddressStorage(AddressStorageIdx, PartitionIdx))
  {
    maxNumberOfEntriesToSerialize = Sd_GetSendEntryNrPendingIdxOfAddressStorage(AddressStorageIdx, PartitionIdx);
  }
  sizeEntriesArray = (uint16)maxNumberOfEntriesToSerialize * SD_ENTRY_SIZE;

  /* #30 Get next entry to send and set offset to entry and option array. */
  sendEntryIdx = Sd_GetSendEntryHeadIdxOfAddressStorage(AddressStorageIdx, PartitionIdx);
  payloadEntryIdx = SD_HEADER_LENGTH + SD_HEADER_LENGTHOFENTRIESARRAY_SIZE;
  optionArrayInfo.ArrayStartIdx = payloadEntryIdx + sizeEntriesArray + SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE;
  optionArrayInfo.ArrayEndIdx = optionArrayInfo.ArrayStartIdx;
  optionArrayEndIdxPrev = optionArrayInfo.ArrayEndIdx;
  optionArrayInfo.FreeOptionIndex = 0u;

  /* #40 Iterate over ready to send entries and serialize entries as well as options. */
  while((sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx)) &&
        (numSerializedEntries < maxNumberOfEntriesToSerialize) &&
        ((payloadEntryIdx + SD_ENTRY_SIZE) < (uint16)Sd_GetSizeOfPayload(PartitionIdx)) &&
        (continueIter == TRUE))
  {
    skipEntry = Sd_Tx_CheckIfEntryShallBeSkipped(sendEntryIdx, PartitionIdx);

    if(skipEntry == SD_SENDENTRY_ACCEPT)
    {
      /* Check if retry timer has to be set */
      Sd_Tx_CheckAndRestartRetry(sendEntryIdx, PartitionIdx);

      /* #400 Serialize the entry with all corresponding options. */
      bufferTooSmall = Sd_Tx_SerializeEntryAndOptions(sendEntryIdx, PartitionIdx, payloadEntryIdx, &optionArrayInfo);   /* SBSW_SD_REFERENCE_OF_VARIABLE */

      /* #401 Stop processing if TxBuffer is too small for next entry. */
      if(bufferTooSmall == TRUE)
      {
        continueIter = FALSE;
      }
      else
      {
        /* #402 Remove sent entry from queue and get next entry ready to send. */
        Sd_SizeOfSendEntryType sendEntryNext = Sd_GetSendEntryNextIdxOfSendEntry(sendEntryIdx, PartitionIdx);
        payloadEntryIdx += SD_ENTRY_SIZE;
        numSerializedEntries++;
        optionArrayEndIdxPrev = optionArrayInfo.ArrayEndIdx;
        (void)Sd_Util_RemoveSendEntryFromList(InstanceIdx, PartitionIdx, AddressStorageIdx, sendEntryIdx);
        sendEntryIdx = sendEntryNext;
      }
    }
    else if(skipEntry == SD_SENDENTRY_SKIP)
    {
      sendEntryIdx = Sd_GetSendEntryNextIdxOfSendEntry(sendEntryIdx, PartitionIdx);

      if (entrySkipped == FALSE)
      {
        Sd_Util_SetTimerOffset(PartitionIdx,
          Sd_GetAddrTimeToSendOfAddressStorage(AddressStorageIdx, PartitionIdx), SD_MAIN_FCT_CYCLE_TIME_MS);            /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
        entrySkipped = TRUE;
      }
    }
    else
    {
      Sd_SizeOfSendEntryType sendEntryNext = Sd_GetSendEntryNextIdxOfSendEntry(sendEntryIdx, PartitionIdx);
      (void)Sd_Util_RemoveSendEntryFromList(InstanceIdx, PartitionIdx, AddressStorageIdx, sendEntryIdx);
      sendEntryIdx = sendEntryNext;
    }
  }

  /* #50 Check if at least one entry has been serialized completely. */
  if((numSerializedEntries != 0u) && ((bufferTooSmall == TRUE) || (numSerializedEntries != maxNumberOfEntriesToSerialize)))
  {
    /* #500 Calculate length of gap between Entries and OptionArray. */
    uint16 unusedEntryArrayLength = ((uint16)maxNumberOfEntriesToSerialize - (uint16)numSerializedEntries) *
      SD_ENTRY_SIZE;

    /* #501 Copy Options Array after EntriesArray so already serialized entries can be sent.  */
    /* PRQA S 0314 2 */ /* MD_Sd_Dir-1.1_0314_copy */
    Sd_Copy(                                                                                                            /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
      (SD_P2VAR(void)) Sd_GetAddrPayload(optionArrayInfo.ArrayStartIdx - unusedEntryArrayLength, PartitionIdx),
      (SD_P2CONST(void)) Sd_GetAddrPayload(optionArrayInfo.ArrayStartIdx, PartitionIdx),
      ((uint32_least)optionArrayEndIdxPrev - (uint32_least)optionArrayInfo.ArrayStartIdx));

    /* #502 Correct size of entries and start/end address of Options Array. */
    sizeEntriesArray -= unusedEntryArrayLength;
    optionArrayInfo.ArrayStartIdx -= unusedEntryArrayLength;
    optionArrayInfo.ArrayEndIdx = optionArrayEndIdxPrev - unusedEntryArrayLength;
    bufferTooSmall = FALSE;
  }

  /* #60 Continue if payload contains usable data. */
  if((numSerializedEntries != 0u) && (bufferTooSmall == FALSE)) /* PRQA S 2995 */ /* MD_Sd_FalsePositive_2995_2996_2853 */
  {
    /* #600 Write the header of the SD message. */
    Sd_Tx_SerializeHeader(AddressStorageIdx, PartitionIdx);

    /* #601 Write the length of the entries array field. */
    Sd_Util_Write32Bit(Sd_GetAddrPayload(SD_HEADER_LENGTH, PartitionIdx), sizeEntriesArray);                            /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */

    /* #602 Set the length of the options array. */
    sizeOfOptionsArray = (uint32)optionArrayInfo.ArrayEndIdx - (uint32)optionArrayInfo.ArrayStartIdx;
    Sd_Util_Write32Bit(Sd_GetAddrPayload(                                                                               /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
        (Sd_SizeOfPayloadType)optionArrayInfo.ArrayStartIdx - SD_HEADER_LENGTHOFENTRIESARRAY_SIZE, PartitionIdx),
      sizeOfOptionsArray);

    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    retValSoad = SoAd_SetRemoteAddr(Sd_GetTxSoConIdOfInstance(InstanceIdx, PartitionIdx),
      (SD_P2VAR(SoAd_SockAddrType))(Sd_GetAddrAddrOfAddressStorage(AddressStorageIdx, PartitionIdx)));                  /* SBSW_SD_TX_ADDRESS_STORAGE_IDX */ /* SBSW_SD_SOCK_ADDR_CAST */

    /* #603 Send the message. (Return value of SoAd_IfTransmit() is ignored.) */
    if(retValSoad == E_OK)
    {
      pduInfo.SduDataPtr = Sd_GetAddrPayload(0u, PartitionIdx);
      pduInfo.SduLength =  (PduLengthType)optionArrayInfo.ArrayEndIdx;

      (void)SoAd_IfTransmit(Sd_GetTxPduIdOfInstance(InstanceIdx, PartitionIdx), &pduInfo);                              /* SBSW_SD_REFERENCE_OF_VARIABLE */
    }

    /* #604 Increment SessionId for each transmitted message. */
    if(Sd_GetSessionIdCounterOfAddressStorage(AddressStorageIdx, PartitionIdx) == 0xFFFFu)
    {
      /* #6040 The SessionId should start with the value 1. */
      Sd_SetSessionIdCounterOfAddressStorage(AddressStorageIdx, 1u, PartitionIdx);                                      /* SBSW_SD_TX_ADDRESS_STORAGE_IDX */
      /* #6041 Reset the reboot flag if the SessionId wraps around. */
      Sd_SetFlagsOfAddressStorage(AddressStorageIdx,                                                                    /* SBSW_SD_TX_ADDRESS_STORAGE_IDX */
        (uint8)(Sd_GetFlagsOfAddressStorage(AddressStorageIdx, PartitionIdx) & (~SD_FLAGS_REBOOT_MASK)),
        PartitionIdx);
    }
    else
    {
      Sd_IncSessionIdCounterOfAddressStorage(AddressStorageIdx, PartitionIdx);                                          /* SBSW_SD_TX_ADDRESS_STORAGE_IDX */
    }
  }
  else if((numSerializedEntries == 0u) && (bufferTooSmall == FALSE)) /* PRQA S 2995 */ /* MD_Sd_FalsePositive_2995_2996_2853 */
  {
    /* No more entries to send */
  }
  /* #70 The configured Sd_Payload buffer is too small to contain the first entry which shall be serialized. */
  /* Buffer size is configured in Sd/SdConfig/SdInstance/SdInstanceTxPdu/SdTxPduRef or Sd/SdGeneral/SdTxBufferSize. */
  else
  {
#if(SD_DEV_ERROR_REPORT == STD_ON)
    /* #700 Report error. */
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_SERIALIZE_PENDING_MESSAGES, SD_E_TX_BUFFER_FULL);
#endif
    /* #701 Remove Entry and set return value to one to avoid that entry which cannot be serialized blocks the queue. */
    (void)Sd_Util_RemoveSendEntryFromList(InstanceIdx, PartitionIdx, AddressStorageIdx, sendEntryIdx);
    numSerializedEntries = 1u;
  }

  return numSerializedEntries;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_SerializeHeader(
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Fill the header of the SD message. */
  /* #20 The Session ID of the header is used as a counter.
   *     All unicast as well as multicast messages have their own counter. */
  Sd_Util_Write16Bit(Sd_GetAddrPayload(SD_HEADER_CLIENTID_OFFSET, PartitionIdx), SD_HEADER_CLIENTID);                   /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write16Bit(Sd_GetAddrPayload(SD_HEADER_SESSIONID_OFFSET, PartitionIdx),
    Sd_GetSessionIdCounterOfAddressStorage(AddressStorageIdx, PartitionIdx));                                           /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_PROTOCOLVERSION_OFFSET, PartitionIdx), SD_HEADER_PROTOCOLVERSION);      /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_INTERFACEVERSION_OFFSET, PartitionIdx), SD_HEADER_INTERFACEVERSION);    /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_MESSAGETYPE_OFFSET, PartitionIdx), SD_HEADER_MESSAGETYPE);              /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_RETURNCODE_OFFSET, PartitionIdx), SD_HEADER_RETURNCODE);                /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write8Bit(Sd_GetAddrPayload(SD_HEADER_FLAGS_OFFSET, PartitionIdx),
    Sd_GetFlagsOfAddressStorage(AddressStorageIdx, PartitionIdx));                                                      /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
  Sd_Util_Write24Bit(Sd_GetAddrPayload(SD_HEADER_RESERVED_OFFSET, PartitionIdx), SD_HEADER_RESERVED);                   /* SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES */
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeRelatedOptions()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeRelatedOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean bufferTooSmall;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the control structures which map the options. */
  EntryToOptionMapPtr->Index1st = SD_INV_OPTION_INDEX;                                                                  /* SBSW_SD_POINTER_WRITE */
  EntryToOptionMapPtr->Index2nd = SD_INV_OPTION_INDEX;                                                                  /* SBSW_SD_POINTER_WRITE */
  EntryToOptionMapPtr->Nr1stOptions = 0u;                                                                               /* SBSW_SD_POINTER_WRITE */
  EntryToOptionMapPtr->Nr2ndOptions = 0u;                                                                               /* SBSW_SD_POINTER_WRITE */

  /* #20 Serialize all endpoint options into the message. */
  bufferTooSmall = Sd_Tx_SerializeEndpointOptions(SendEntryIdx, PartitionIdx, EntryToOptionMapPtr, OptionArrayInfoPtr); /* SBSW_SD_PASSING_API_POINTER */

  if(bufferTooSmall == FALSE)
  {
    /* #200 Serialize all configuration options into the message. */
    bufferTooSmall = Sd_Tx_SerializeConfigurationOptions(SendEntryIdx, PartitionIdx, EntryToOptionMapPtr,
      OptionArrayInfoPtr);                                                                                              /* SBSW_SD_PASSING_API_POINTER */
  }

  if(EntryToOptionMapPtr->Index1st == SD_INV_OPTION_INDEX)
  {
    /* #201 1st option run not used, clear all values. */
    EntryToOptionMapPtr->Index1st = 0u;                                                                                 /* SBSW_SD_POINTER_WRITE */
    EntryToOptionMapPtr->Nr1stOptions = 0u;                                                                             /* SBSW_SD_POINTER_WRITE */
  }

  if(EntryToOptionMapPtr->Index2nd == SD_INV_OPTION_INDEX)
  {
    /* #202 2nd option run not used, clear all values. */
    EntryToOptionMapPtr->Index2nd = 0u;                                                                                 /* SBSW_SD_POINTER_WRITE */
    EntryToOptionMapPtr->Nr2ndOptions = 0u;                                                                             /* SBSW_SD_POINTER_WRITE */
  }

  return bufferTooSmall;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEntryAndOptions()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeEntryAndOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  uint16 PayloadEntryIdx,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_EntryType entryType;
  boolean bufferTooSmall;
  uint32 usedTtl;
  Sd_EntryToOptionMapType entryToOptionMap;

  /* ----- Implementation ----------------------------------------------- */
  entryType = Sd_GetEntryTypeOfSendEntry(SendEntryIdx, PartitionIdx);

  /* #10 Serialize first all related options of the entry. */
  bufferTooSmall = Sd_Tx_SerializeRelatedOptions(SendEntryIdx, PartitionIdx, &entryToOptionMap, OptionArrayInfoPtr);    /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Check if all options fit to the payload. */
  if(bufferTooSmall == FALSE)
  {
    /* #200 Serialize the entry itself. */
    /* Fill the type field. The first bit of the Sd_EntryType is only used internally to differentiate STOP/NACK entries. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx /* + SD_ENTRY_TYPE_OFFSET */, PartitionIdx),
      (uint8)(entryType & SD_ENTRY_TYPE_MASK)); /*lint !e655 */                                                         /* SBSW_SD_TX_SERIALIZE_ENTRY */

    /* Write the option index and number of options. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_INDEX1_OFFSET, PartitionIdx),
      entryToOptionMap.Index1st);                                                                                       /* SBSW_SD_TX_SERIALIZE_ENTRY */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_INDEX2_OFFSET, PartitionIdx),
      entryToOptionMap.Index2nd);                                                                                       /* SBSW_SD_TX_SERIALIZE_ENTRY */
    Sd_Util_Write8Bit(Sd_GetAddrPayload( PayloadEntryIdx + SD_ENTRY_NROFOPTS_OFFSET, PartitionIdx),
      (uint8)(entryToOptionMap.Nr1stOptions << 4u) | entryToOptionMap.Nr2ndOptions);                                    /* SBSW_SD_TX_SERIALIZE_ENTRY */

    /* Write Service and Instance IDs. */
    Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_SERVICEID_OFFSET, PartitionIdx),
      Sd_GetServiceIdOfSendEntry(SendEntryIdx, PartitionIdx));                                                          /* SBSW_SD_TX_SERIALIZE_ENTRY */
    Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_INSTANCEID_OFFSET, PartitionIdx),
      Sd_GetInstanceIdOfSendEntry(SendEntryIdx, PartitionIdx));                                                         /* SBSW_SD_TX_SERIALIZE_ENTRY */

    /* Determine the correct TTL. */
    switch(entryType)
    {
      case SD_ENTRY_FIND_SERVICE:
        usedTtl = Sd_GetTTLOfClientTimer(Sd_GetClientTimerIdxOfClient(
          Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx), PartitionIdx);
        break;

      case SD_ENTRY_SUBSCRIBE_EVENTGROUP:
        usedTtl = Sd_GetTTLOfClientTimer(Sd_GetClientTimerIdxOfConsumedEventgroup(
          Sd_GetConsumedEventgroupIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx), PartitionIdx);
        break;

      case SD_ENTRY_OFFER_SERVICE:
        usedTtl = Sd_GetTTLOfServerTimer(Sd_GetServerTimerIdxOfServer(
          Sd_GetServerIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx), PartitionIdx);
        break;

      case SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK:
        usedTtl = Sd_GetRequestedTTLOfClientList(
          Sd_GetClientListIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx);
        break;

      default:
        /* STOP/NACK entry with TTL = 0 */
        usedTtl = 0u;
        break;
    }

    /* Write Major Version. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY_MAJORVERSION_OFFSET, PartitionIdx),
      Sd_GetMajorVersionOfSendEntry(SendEntryIdx, PartitionIdx));                                                       /* SBSW_SD_TX_SERIALIZE_ENTRY */

    /* Write TTL . */
    Sd_Util_Write24Bit(Sd_GetAddrPayload(PayloadEntryIdx +  SD_ENTRY_TTL_OFFSET, PartitionIdx), usedTtl);               /* SBSW_SD_TX_SERIALIZE_ENTRY */

    /* Write the Minor Version for send entries of format type 1 or the eventgroup ID for a format type 2 entry. */
    switch(entryType)
    {
      case SD_ENTRY_FIND_SERVICE:
        if(Sd_GetFindBehaviorOfClient(Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx) ==
             SD_FIND_BEHAVIOR_MINIMUM_MINOR_VERSION)
        {
          Sd_Util_Write32Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY1_MINORVERSION_OFFSET, PartitionIdx),
            SD_ENTRY_WILDCARD_MINOR_VERSION);                                                                           /* SBSW_SD_TX_SERIALIZE_ENTRY */
        }
        else
        {
          Sd_Util_Write32Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY1_MINORVERSION_OFFSET, PartitionIdx),
            Sd_GetMinorVersionOfClient(Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx));          /* SBSW_SD_TX_SERIALIZE_ENTRY */
        }
        break;

      case SD_ENTRY_SUBSCRIBE_EVENTGROUP:
      case SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP:
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, PartitionIdx), 0u);    /* SBSW_SD_TX_SERIALIZE_ENTRY */
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY2_EVENTGROUPID_OFFSET, PartitionIdx),
          Sd_GetEventGroupIdOfConsumedEventgroup(Sd_GetConsumedEventgroupIdxOfSendEntry(SendEntryIdx, PartitionIdx),
            PartitionIdx));                                                                                             /* SBSW_SD_TX_SERIALIZE_ENTRY */
        break;

      case SD_ENTRY_OFFER_SERVICE:
      case SD_ENTRY_STOP_OFFER_SERVICE:
        Sd_Util_Write32Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY1_MINORVERSION_OFFSET, PartitionIdx),
          Sd_GetMinorVersionOfServer(Sd_GetServerIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx));            /* SBSW_SD_TX_SERIALIZE_ENTRY */
        break;

      case SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK:
      case SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK:
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY2_RESERVEDCOUNTER_OFFSET, PartitionIdx),
          Sd_GetReservedFieldOfSendEntry(SendEntryIdx, PartitionIdx));                                                  /* SBSW_SD_TX_SERIALIZE_ENTRY */
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadEntryIdx + SD_ENTRY2_EVENTGROUPID_OFFSET, PartitionIdx),
          Sd_GetEventHandlerIdOfSendEntry(SendEntryIdx, PartitionIdx));                                                 /* SBSW_SD_TX_SERIALIZE_ENTRY */
        break;

      default: /* COV_SD_INV_STATE */
        /* Nothing to do. */
        break;
    }
  }

  return bufferTooSmall;
}

/**********************************************************************************************************************
 *  Sd_Tx_GetRelevantEndpointOptions()
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
 *
 */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_GetRelevantEndpointOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EndpointOptionTxType) UdpEndpointPtr,
  SD_P2VAR(Sd_EndpointOptionTxType) TcpEndpointPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_EntryType entryType = Sd_GetEntryTypeOfSendEntry(SendEntryIdx, PartitionIdx);
  Sd_SockAddrContainerType* udpEpAddrPtr = &(UdpEndpointPtr->EpAddr);
  Sd_SockAddrContainerType* tcpEpAddrPtr = &(TcpEndpointPtr->EpAddr);

  /* ----- Implementation ----------------------------------------------- */
  udpEpAddrPtr->family = SOAD_AF_INVALID;                                                                 /* SBSW_SD_POINTER_WRITE */
  tcpEpAddrPtr->family = SOAD_AF_INVALID;                                                                 /* SBSW_SD_POINTER_WRITE */
  UdpEndpointPtr->Multicast = FALSE;                                                                      /* SBSW_SD_POINTER_WRITE */
  TcpEndpointPtr->Multicast = FALSE;                                                                      /* SBSW_SD_POINTER_WRITE */

  /* #10 Check which type of entry shall be transmitted. */
  switch(entryType)
  {
  case SD_ENTRY_FIND_SERVICE:
  case SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK:
    /* #100 FindService and SubscribeEventgroupNack entries do not allow any endpoint options. */
    break;

  case SD_ENTRY_OFFER_SERVICE:
  case SD_ENTRY_STOP_OFFER_SERVICE:
  {
    const Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfSendEntry(SendEntryIdx, PartitionIdx);

    /* #101 (Stop)OfferService entries require unicast endpoint options.
     *      Check if the service is configured for UDP and save the address. */
    if(Sd_GetUdpSoConStartIdxOfServer(serverIdx, PartitionIdx) != SD_NO_UDPSOCONSTARTIDXOFSERVER)
    {
      const Sd_SizeOfSoConGrpDynType soConGrpIdx = Sd_GetSoConGrpDynUdpIdxOfServer(serverIdx, PartitionIdx);

      (void)Sd_SoAd_GetLocalAddr(Sd_GetIdOfUdpSoCon(Sd_GetUdpSoConStartIdxOfServer(serverIdx, PartitionIdx),
        PartitionIdx), udpEpAddrPtr);                                                                     /* SBSW_SD_PASSING_API_POINTER */

      /* #1010 Local port could have already been reset, read back for StopOffer. */
      if(entryType == SD_ENTRY_STOP_OFFER_SERVICE)
      {
        udpEpAddrPtr->port = Sd_GetLocalPortOfSoConGrpDyn(soConGrpIdx, PartitionIdx);                     /* SBSW_SD_POINTER_WRITE */
      }
      /* #1011 Store local port at time of sending Offer. */
      else
      {
        Sd_SetLocalPortOfSoConGrpDyn(soConGrpIdx, udpEpAddrPtr->port, PartitionIdx);                      /* SBSW_SD_CSL03_CSL05 */
      }
    }

    if(udpEpAddrPtr->family == SOAD_AF_INVALID)
    {
      Sd_SizeOfEventHandlerType eventHandlerIdx;

      /* #1012 Check if the service contains an event handler which is configured for multicast. */
      for(eventHandlerIdx = Sd_GetEventHandlerStartIdxOfServer(serverIdx, PartitionIdx);
          eventHandlerIdx < Sd_GetEventHandlerEndIdxOfServer(serverIdx, PartitionIdx);
          eventHandlerIdx++)
      {
        if(Sd_EventHandlerUsesMulticast(eventHandlerIdx, PartitionIdx))
        {
          (void)Sd_SoAd_GetLocalAddr(Sd_GetSoConIdOfEventHandlerMulticast(
            Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx, PartitionIdx), PartitionIdx),
            udpEpAddrPtr);                                                                                /* SBSW_SD_PASSING_API_POINTER */

          break;
        }
      }
    }

    /* #102 Check if the service is configured for TCP and save the address. */
    if(Sd_GetTcpSoConIdStartIdxOfServer(serverIdx, PartitionIdx) != SD_NO_TCPSOCONIDSTARTIDXOFSERVER)
    {
      const Sd_SizeOfSoConGrpDynType soConGrpIdx = Sd_GetSoConGrpDynTcpIdxOfServer(serverIdx, PartitionIdx);

      (void)Sd_SoAd_GetLocalAddr( Sd_GetTcpSoConId(
        Sd_GetTcpSoConIdStartIdxOfServer(serverIdx, PartitionIdx), PartitionIdx), tcpEpAddrPtr);          /* SBSW_SD_PASSING_API_POINTER */

      /* #1020 Local port could have already been reset, read back for StopOffer. */
      if(entryType == SD_ENTRY_STOP_OFFER_SERVICE)
      {
        tcpEpAddrPtr->port = Sd_GetLocalPortOfSoConGrpDyn(soConGrpIdx, PartitionIdx);                     /* SBSW_SD_POINTER_WRITE */
      }
      /* #1021 Store local port at time of sending Offer. */
      else
      {
        Sd_SetLocalPortOfSoConGrpDyn(soConGrpIdx, tcpEpAddrPtr->port, PartitionIdx);                      /* SBSW_SD_CSL03_CSL05 */
      }
    }
  }
  break;

  case SD_ENTRY_SUBSCRIBE_EVENTGROUP:
  case SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP:
  {
    const Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx =
      Sd_GetConsumedEventgroupIdxOfSendEntry(SendEntryIdx, PartitionIdx);
    const Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx);

    /* #103 (Stop)SubscribeEventgroup entries allow unicast or multicast endpoint options.
     *      Check if the service is configured for UDP Unicast or UDP ClientMulticast and save the address. */
    if(Sd_ConsumedEventGroupUsesUdp(consumedEventgroupIdx, PartitionIdx) == TRUE)
    {
      const Sd_SizeOfSoConGrpDynType soConGrpIdx = Sd_GetSoConGrpDynUdpIdxOfClient(clientIdx, PartitionIdx);

      (void)Sd_SoAd_GetLocalAddr(Sd_GetIdOfUdpSoCon(
        Sd_GetUdpSoConStartIdxOfClient(clientIdx, PartitionIdx), PartitionIdx), udpEpAddrPtr);            /* SBSW_SD_PASSING_API_POINTER */

      if(Sd_IsUdpClientMulticastOfClient(clientIdx, PartitionIdx))
      {
        UdpEndpointPtr->Multicast = TRUE;                                                                 /* SBSW_SD_POINTER_WRITE */
      }

      /* #1030 Local port could have already been reset, read back for StopSubscribe. */
      if(entryType == SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP)
      {
        udpEpAddrPtr->port = Sd_GetLocalPortOfSoConGrpDyn(soConGrpIdx, PartitionIdx);                     /* SBSW_SD_POINTER_WRITE */
      }
      /* #1031 Store local port at time of sending Subscribe. */
      else
      {
        Sd_SetLocalPortOfSoConGrpDyn(soConGrpIdx, udpEpAddrPtr->port, PartitionIdx);                      /* SBSW_SD_CSL03_CSL05 */
      }
    }

    /* #104 Check if the service is configured for TCP and save the address. */
    if(Sd_ConsumedEventGroupUsesTcp(consumedEventgroupIdx, PartitionIdx) == TRUE)
    {
      const Sd_SizeOfSoConGrpDynType soConGrpIdx = Sd_GetSoConGrpDynTcpIdxOfClient(clientIdx, PartitionIdx);

      (void)Sd_SoAd_GetLocalAddr(Sd_GetTcpSoConId(
        Sd_GetTcpSoConIdStartIdxOfClient(clientIdx, PartitionIdx), PartitionIdx), tcpEpAddrPtr);          /* SBSW_SD_PASSING_API_POINTER */

      /* #1040 Local port could have already been reset, read back for StopSubscribe. */
      if(entryType == SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP)
      {
        tcpEpAddrPtr->port = Sd_GetLocalPortOfSoConGrpDyn(soConGrpIdx, PartitionIdx);                     /* SBSW_SD_POINTER_WRITE */
      }
      /* #1041 Store local port at time of sending Subscribe. */
      else
      {
        Sd_SetLocalPortOfSoConGrpDyn(soConGrpIdx, tcpEpAddrPtr->port, PartitionIdx);                      /* SBSW_SD_CSL03_CSL05 */
      }
    }
  }
  break;

  case SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK:
  {
    Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfSendEntry(SendEntryIdx, PartitionIdx);

    /* #105 SubscribeEventgroupAck entries allow multicast endpoint options.
     *      Check if the event handler is configured for multicast. */
    if(Sd_EventHandlerUsesMulticast(eventHandlerIdx, PartitionIdx))
    {
      /* #1050 Get the attached multicast endpoint. */
      (void)Sd_SoAd_GetRemoteAddr(Sd_GetSoConIdOfEventHandlerMulticast(
        Sd_GetEventHandlerMulticastIdxOfEventHandler(eventHandlerIdx, PartitionIdx), PartitionIdx),
        udpEpAddrPtr);                                                                                    /* SBSW_SD_PASSING_API_POINTER */
      UdpEndpointPtr->Multicast = TRUE;                                                                   /* SBSW_SD_POINTER_WRITE */
    }
  }
  break;

  default: /* COV_SD_INV_STATE */
    /* #106 Undefined EntryType. */
    break;
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_GetRelevantConfigOptions()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_Sd_STPAR_6060 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_GetRelevantConfigOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) HostnameStartIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) HostnameEndIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) CapabilityRecordStartIdxPtr,
  SD_P2VAR(Sd_SizeOfCapabilityRecordType) CapabilityRecordEndIdxPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize start and end values. */
  *HostnameStartIdxPtr = SD_NO_CAPABILITYRECORDSTARTIDXOFINSTANCE;                                                      /* SBSW_SD_POINTER_WRITE */
  *HostnameEndIdxPtr = SD_NO_CAPABILITYRECORDENDIDXOFINSTANCE;                                                          /* SBSW_SD_POINTER_WRITE */
  *CapabilityRecordStartIdxPtr = SD_NO_CAPABILITYRECORDSTARTIDXOFSERVER;                                                /* SBSW_SD_POINTER_WRITE */
  *CapabilityRecordEndIdxPtr = SD_NO_CAPABILITYRECORDENDIDXOFSERVER;                                                    /* SBSW_SD_POINTER_WRITE */

  /* #20 Check if the entry might contain configuration options. */
  if((Sd_GetEntryTypeOfSendEntry(SendEntryIdx, PartitionIdx) == SD_ENTRY_FIND_SERVICE) ||
    (Sd_GetEntryTypeOfSendEntry(SendEntryIdx, PartitionIdx) == SD_ENTRY_OFFER_SERVICE) ||
    (Sd_GetEntryTypeOfSendEntry(SendEntryIdx, PartitionIdx) == SD_ENTRY_STOP_OFFER_SERVICE) ||
    (Sd_GetServiceIdOfSendEntry(SendEntryIdx, PartitionIdx) == SD_SERVICE_ID_OTHERSERV))
  {
    /* #200 Update the hostname start and end indexes. */
    *HostnameStartIdxPtr = Sd_GetCapabilityRecordStartIdxOfInstance(                                                    /* SBSW_SD_POINTER_WRITE */
      Sd_GetInstanceIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx);
    *HostnameEndIdxPtr = Sd_GetCapabilityRecordEndIdxOfInstance(                                                        /* SBSW_SD_POINTER_WRITE */
      Sd_GetInstanceIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx);

    /* #201 Update the capability record start and end indexes. */
    if(Sd_GetServerIdxOfSendEntry(SendEntryIdx, PartitionIdx) < Sd_GetSizeOfServer(PartitionIdx))
    {
      *CapabilityRecordStartIdxPtr = Sd_GetCapabilityRecordStartIdxOfServer(                                            /* SBSW_SD_POINTER_WRITE */
        Sd_GetServerIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx);
      *CapabilityRecordEndIdxPtr = Sd_GetCapabilityRecordEndIdxOfServer(                                                /* SBSW_SD_POINTER_WRITE */
        Sd_GetServerIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx);
    }
    else if(Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx) < Sd_GetSizeOfClient(PartitionIdx))
    {
      *CapabilityRecordStartIdxPtr = Sd_GetCapabilityRecordStartIdxOfClient(                                            /* SBSW_SD_POINTER_WRITE */
        Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx);
      *CapabilityRecordEndIdxPtr = Sd_GetCapabilityRecordEndIdxOfClient(                                                /* SBSW_SD_POINTER_WRITE */
        Sd_GetClientIdxOfSendEntry(SendEntryIdx, PartitionIdx), PartitionIdx);
    }
    else
    {
      /* No associated ServiceInstance, no capability record exists. */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_CheckForDuplicateOption()
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
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Tx_CheckForDuplicateOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isDuplicateOption = FALSE;
  uint16 tmpPayloadIdx = OptionArrayInfoPtr->ArrayStartIdx;
  uint16 optionOffset;
  uint16 optionLength;
  uint8 lastDeduplicateAbleOptionIndex = SD_INV_OPTION_INDEX;

  /* ----- Implementation ----------------------------------------------- */
  OptionArrayInfoPtr->DeduplicatedOptionIndex = SD_INV_OPTION_INDEX;                                                    /* SBSW_SD_POINTER_WRITE */

  /* #10 Check if the payload contains the new option which was serialized after the end of the options array. */
  while((tmpPayloadIdx < OptionArrayInfoPtr->ArrayEndIdx) &&
        ((tmpPayloadIdx + SD_OPTION_LENGTH_SIZE ) < (uint16)Sd_GetSizeOfPayload(PartitionIdx)))
  {
    /* #100 Read length of next option. */
    IPBASE_GET_UINT16(Sd_GetAddrPayload(tmpPayloadIdx, PartitionIdx), 0u, optionLength);                                /* SBSW_SD_TX_PAYLOAD_ACCESS_OPTION_LEN_FIELD */

    /* Consider entire option. */
    optionLength += SD_OPTION_NOT_COVERED_FIELDS;

    optionOffset = 0u;
    OptionArrayInfoPtr->DeduplicatedOptionIndex++;                                                                      /* SBSW_SD_POINTER_WRITE */
    isDuplicateOption = TRUE;

    /* #101 Compare all bytes of the already serialized option. */
    while(optionOffset < optionLength)
    {
      /* #1010 Check if the current option is equivalent to the option that has been serialized already. */
      if(Sd_GetPayload(tmpPayloadIdx + optionOffset, PartitionIdx) !=
         Sd_GetPayload(OptionArrayInfoPtr->ArrayEndIdx + optionOffset, PartitionIdx))
      {
        /* #10100 Option does not match and cannot be used for deduplication. Skip option. */
        isDuplicateOption = FALSE;
        break;
      }

      optionOffset++;
    }

    if(isDuplicateOption == TRUE)
    {
      /* #1011 Save option for possible deduplication. Repeat searching in order to find the last matching option. */
      lastDeduplicateAbleOptionIndex = OptionArrayInfoPtr->DeduplicatedOptionIndex;
    }

    tmpPayloadIdx += optionLength;
  }

  /* #20 Check if the last serialized option can be reused for deduplication. */
  if(isDuplicateOption == TRUE)
  {
    /* #200 Return the last option. The struct member DeduplicatedOptionIndex is already set to the correct value. */
  }
  /* #30 Otherwise, if another option can be used for deduplication. */
  else if(lastDeduplicateAbleOptionIndex != SD_INV_OPTION_INDEX)
  {
    /* #300 Return the last matching option. */
    OptionArrayInfoPtr->DeduplicatedOptionIndex = lastDeduplicateAbleOptionIndex;                                       /* SBSW_SD_POINTER_WRITE */
  }
  /* #40 Otherwise, no matching option was found.*/
  else
  {
    /* #400 Set the DeduplicatedOptionIndex to invalid. */
    OptionArrayInfoPtr->DeduplicatedOptionIndex = SD_INV_OPTION_INDEX;                                                  /* SBSW_SD_POINTER_WRITE */
  }
}

/**********************************************************************************************************************
 *  Sd_Tx_DeduplicateOption()
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
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Tx_DeduplicateOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr,
  boolean RemainingOption)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean ableToDeduplicateOption = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the new option was already serialized to the payload. */
  Sd_Tx_CheckForDuplicateOption(PartitionIdx, OptionArrayInfoPtr);                                                      /* SBSW_SD_PASSING_API_POINTER */

  if(OptionArrayInfoPtr->DeduplicatedOptionIndex != SD_INV_OPTION_INDEX)
  {
    ableToDeduplicateOption = TRUE;

    /* #100 Option was already serialized to the payload. Check if it can be deduplicated. */

    if(EntryToOptionMapPtr->Nr1stOptions == 0u)
    {
      /* #1000 Use first option run to index the option. */
      EntryToOptionMapPtr->Index1st = OptionArrayInfoPtr->DeduplicatedOptionIndex;                                      /* SBSW_SD_POINTER_WRITE */
      EntryToOptionMapPtr->Nr1stOptions++;                                                                              /* SBSW_SD_POINTER_WRITE */
    }
    else if((EntryToOptionMapPtr->Index1st + EntryToOptionMapPtr->Nr1stOptions) == OptionArrayInfoPtr->DeduplicatedOptionIndex)
    {
      /* #1001 Extend first option run at the end to index new option. */
      EntryToOptionMapPtr->Nr1stOptions++;                                                                              /* SBSW_SD_POINTER_WRITE */
    }
    else if((EntryToOptionMapPtr->Index1st - 1u) == OptionArrayInfoPtr->DeduplicatedOptionIndex)
    {
      /* #1002 Extend first option run at the beginning to index new option. */
      EntryToOptionMapPtr->Index1st--;                                                                                  /* SBSW_SD_POINTER_WRITE */
      EntryToOptionMapPtr->Nr1stOptions++;                                                                              /* SBSW_SD_POINTER_WRITE */
    }
    else if((RemainingOption == FALSE) && (EntryToOptionMapPtr->Nr2ndOptions == 0u))
    {
      /* #1003 Use second option run to index the option because there are no more pending options. */
      EntryToOptionMapPtr->Index2nd = OptionArrayInfoPtr->DeduplicatedOptionIndex;                                      /* SBSW_SD_POINTER_WRITE */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_POINTER_WRITE */
    }
    else if(OptionArrayInfoPtr->FreeOptionIndex == (OptionArrayInfoPtr->DeduplicatedOptionIndex + 1u))
    {
      /* #1004 Use second option run to index the option because the found option is the last one in the array. */
      EntryToOptionMapPtr->Index2nd = OptionArrayInfoPtr->DeduplicatedOptionIndex;                                      /* SBSW_SD_POINTER_WRITE */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_POINTER_WRITE */
    }
    else if(OptionArrayInfoPtr->FreeOptionIndex == (EntryToOptionMapPtr->Index1st + EntryToOptionMapPtr->Nr1stOptions))
    {
      /* #1005 Use second option run to index the option because the last option of the array is indexed by the current entry. */
      EntryToOptionMapPtr->Index2nd = OptionArrayInfoPtr->DeduplicatedOptionIndex;                                      /* SBSW_SD_POINTER_WRITE */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_POINTER_WRITE */
    }
    else if((EntryToOptionMapPtr->Nr2ndOptions != 0u) && ((EntryToOptionMapPtr->Index2nd + EntryToOptionMapPtr->Nr2ndOptions) == OptionArrayInfoPtr->DeduplicatedOptionIndex))
    {
      /* #1006 Extend second option run at the end to index new option. */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_POINTER_WRITE */
    }
    else if((EntryToOptionMapPtr->Nr2ndOptions != 0u) && ((EntryToOptionMapPtr->Index2nd - 1u) == OptionArrayInfoPtr->DeduplicatedOptionIndex))
    {
      /* #1007 Extend second option run at the beginning to index new option. */
      EntryToOptionMapPtr->Index2nd--;                                                                                  /* SBSW_SD_POINTER_WRITE */
      EntryToOptionMapPtr->Nr2ndOptions++;                                                                              /* SBSW_SD_POINTER_WRITE */
    }
    else
    {
      /* #1008 Option can not be deduplicated. */
      ableToDeduplicateOption = FALSE;
      OptionArrayInfoPtr->DeduplicatedOptionIndex = SD_INV_OPTION_INDEX;                                                /* SBSW_SD_POINTER_WRITE */
    }
  }

  return ableToDeduplicateOption;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEndpointOptions()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeEndpointOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_EndpointOptionTxType updEndpoint, tcpEndpoint;
  Sd_SizeOfCapabilityRecordType hostnameStartIdx, hostnameEndIdx;
  Sd_SizeOfCapabilityRecordType capabilityRecordStartIdx, capabilityRecordEndIdx;
  boolean bufferTooSmall = FALSE, configOptionPresent = FALSE;
  uint16 additionalOptionSize;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get relevant configuration options. */
  Sd_Tx_GetRelevantConfigOptions(SendEntryIdx, PartitionIdx, &hostnameStartIdx, &hostnameEndIdx,
    &capabilityRecordStartIdx, &capabilityRecordEndIdx);                                                                /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #20 Check if a configuration option has to be added. */
  if((hostnameStartIdx != hostnameEndIdx) || (capabilityRecordStartIdx != capabilityRecordEndIdx))
  {
    configOptionPresent = TRUE;
  }

  /* #30 Get relevant endpoint options. */
  Sd_Tx_GetRelevantEndpointOptions(SendEntryIdx, PartitionIdx, &updEndpoint, &tcpEndpoint);           /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #40 Serialize UDP endpoint option. */
  if(updEndpoint.EpAddr.family != SOAD_AF_INVALID)
  {
    /* #400 Serialize option to the end of the options array. */
    additionalOptionSize = Sd_Tx_SerializeEndpointOption(PartitionIdx, &updEndpoint, SD_L4PROTOCOL_UDP,
      OptionArrayInfoPtr->ArrayEndIdx);                                                                                 /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #401 Check if the option could be serialized. */
    if(additionalOptionSize != SD_INV_OPTION_LENGTH)
    {
      /* #4010 Check if the option was already serialized to the payload and can be deduplicated. */
      if(FALSE == Sd_Tx_DeduplicateOption(PartitionIdx, EntryToOptionMapPtr, OptionArrayInfoPtr, TRUE))                 /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #40100 Use the option, which was already serialized to the end of the payload. */
        Sd_Tx_UseNewSerializedOption(EntryToOptionMapPtr, OptionArrayInfoPtr, additionalOptionSize);                    /* SBSW_SD_PASSING_API_POINTER */
      }
    }
    else
    {
      bufferTooSmall = TRUE;
    }
  }

  /* #50 Serialize TCP endpoint option. */
  if((tcpEndpoint.EpAddr.family != SOAD_AF_INVALID) && (bufferTooSmall == FALSE))
  {
    /* #500 Serialize option to the end of the payload. */
    additionalOptionSize = Sd_Tx_SerializeEndpointOption(PartitionIdx, &tcpEndpoint, SD_L4PROTOCOL_TCP,
      OptionArrayInfoPtr->ArrayEndIdx);                                                                                 /* SBSW_SD_REFERENCE_OF_VARIABLE */

    /* #501 Check if the option could be serialized. */
    if(additionalOptionSize != SD_INV_OPTION_LENGTH)
    {
      /* #5010 Check if option was already serialized to the payload and can be deduplicated. */
      if(FALSE == Sd_Tx_DeduplicateOption(PartitionIdx, EntryToOptionMapPtr, OptionArrayInfoPtr, configOptionPresent))  /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #50100 Use option which was serialized to the end of the payload. */
        Sd_Tx_UseNewSerializedOption(EntryToOptionMapPtr, OptionArrayInfoPtr, additionalOptionSize);                    /* SBSW_SD_PASSING_API_POINTER */
      }
    }
    else
    {
      bufferTooSmall = TRUE;
    }
  }

  return bufferTooSmall;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeEndpointOption()
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
SD_LOCAL FUNC(uint16, SD_CODE) Sd_Tx_SerializeEndpointOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2CONST(Sd_EndpointOptionTxType) EndpointOptionPtr,
  Sd_L4ProtocolType L4Proto,
  uint16 PayloadOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionType optionType = SD_OPTION_IPV4_ENDPOINT;
  SD_P2CONST(Sd_SockAddrContainerType) LocalIpAddrPtr = &(EndpointOptionPtr->EpAddr);
  uint16 optionLength;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check family type of option. */
  if(LocalIpAddrPtr->family == SOAD_AF_INET6)
  {
    optionType = SD_OPTION_IPV6_ENDPOINT;
  }

  /* #20 Check if a multicast endpoint has to be added. */
  if(EndpointOptionPtr->Multicast)
  {
    optionType = (Sd_OptionType)(optionType | SD_OPTION_MULTICAST_MASK); /*lint !e655 */
  }

  /* #30 Differentiate the option type. */
  switch(optionType)
  {
    /* #300 For IPv4 endpoint.*/
    case SD_OPTION_IPV4_ENDPOINT:
    case SD_OPTION_IPV4_MULTICAST:
      /* #3000 Check if the endpoint size exceeds the size of the payload buffer. */
      if((PayloadOffset + SD_OPTION_IPV4_SIZE) >= (uint16)Sd_GetSizeOfPayload(PartitionIdx))
      {
        /* #30000 Stop processing of the endpoint option. */
        optionLength = SD_INV_OPTION_LENGTH;
      }
      /* #3001 Otherwise, serialize all fields of the IPv4 endpoint option. */
      else
      {
        /* Set the length field. */
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadOffset /* + SD_OPTION_LENGTH_OFFSET */, PartitionIdx),
          SD_OPTION_IPV4_COVERED_LENGTH);                                                                               /* SBSW_SD_TX_SERIALIZE_OPTION */
        optionLength = SD_OPTION_IPV4_SIZE;

        /* Set the type field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_TYPE_OFFSET, PartitionIdx),
          (uint8)optionType);                                                                                           /* SBSW_SD_TX_SERIALIZE_OPTION */

        /* Set the first reserved field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload( PayloadOffset + SD_OPTION_RESERVED_OFFSET, PartitionIdx), 0u);             /* SBSW_SD_TX_SERIALIZE_OPTION */

        /* Set the IP address. */
        /* PRQA S 0314 2 */ /* MD_Sd_Dir-1.1_0314_copy */
        Sd_Copy((SD_P2VAR(void)) Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV4_ADDRESS_OFFSET, PartitionIdx),        /* SBSW_SD_TX_SERIALIZE_OPTION */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
          (SD_P2CONST(void)) LocalIpAddrPtr->data, SD_OPTION_IPV4_ADDRESS_SIZE);

        /* Set the second reserved field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV4_RESERVED_OFFSET, PartitionIdx), 0u);         /* SBSW_SD_TX_SERIALIZE_OPTION */

        /* Set the L4-Proto field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV4_L4PROTO_OFFSET, PartitionIdx),
          (uint8)L4Proto);                                                                                              /* SBSW_SD_TX_SERIALIZE_OPTION */

        /* Set the port number. */
        /* PRQA S 0314 2 */ /* MD_Sd_Dir-1.1_0314_copy */
        Sd_Copy((SD_P2VAR(void)) Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV4_PORTNUMBER_OFFSET, PartitionIdx),     /* SBSW_SD_TX_SERIALIZE_OPTION */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
          (SD_P2CONST(void)) &LocalIpAddrPtr->port, SD_OPTION_IPV4_PORTNUMBER_SIZE);
      }
      break;

    /* #301 For IPv6 endpoint.*/
    case SD_OPTION_IPV6_ENDPOINT:
    case SD_OPTION_IPV6_MULTICAST:
      /* #3010 Check if the endpoint size exceeds the size of the payload buffer. */
      if((PayloadOffset + SD_OPTION_IPV6_SIZE) >= (uint16)Sd_GetSizeOfPayload(PartitionIdx))
      {
        /* #30100 Stop processing of the endpoint option. */
        optionLength = SD_INV_OPTION_LENGTH;
      }
      /* #3011 Otherwise, serialize all fields of the IPv6 endpoint option. */
      else
      {
        /* Set the length field. */
        Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadOffset /* + SD_OPTION_LENGTH_OFFSET */, PartitionIdx),
          SD_OPTION_IPV6_COVERED_LENGTH);                                                                               /* SBSW_SD_TX_SERIALIZE_OPTION */
        optionLength = SD_OPTION_IPV6_SIZE;

        /* Set the type field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_TYPE_OFFSET, PartitionIdx),
          (uint8)optionType);                                                                                           /* SBSW_SD_TX_SERIALIZE_OPTION */

        /* Set the first reserved field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_RESERVED_OFFSET, PartitionIdx), 0u);              /* SBSW_SD_TX_SERIALIZE_OPTION */

        /* Set the IP address. */
        /* PRQA S 0314 2 */ /* MD_Sd_Dir-1.1_0314_copy */
        Sd_Copy((SD_P2VAR(void)) Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV6_ADDRESS_OFFSET, PartitionIdx),        /* SBSW_SD_TX_SERIALIZE_OPTION */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
          (SD_P2CONST(void)) LocalIpAddrPtr->data, SD_OPTION_IPV6_ADDRESS_SIZE);

        /* Set the second reserved field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV6_RESERVED_OFFSET, PartitionIdx), 0u);         /* SBSW_SD_TX_SERIALIZE_OPTION */

        /* Set the L4-Proto field. */
        Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV6_L4PROTO_OFFSET, PartitionIdx),
          (uint8)L4Proto);                                                                                              /* SBSW_SD_TX_SERIALIZE_OPTION */

        /* Set the port number. */
        /* PRQA S 0314 2 */ /* MD_Sd_Dir-1.1_0314_copy */
        Sd_Copy((SD_P2VAR(void)) Sd_GetAddrPayload(PayloadOffset + SD_OPTION_IPV6_PORTNUMBER_OFFSET, PartitionIdx),     /* SBSW_SD_TX_SERIALIZE_OPTION */ /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
          (SD_P2CONST(void)) &LocalIpAddrPtr->port, SD_OPTION_IPV6_PORTNUMBER_SIZE);
      }
      break;

    /* #302 Undefined OptionType. */
    default: /* COV_SD_INV_STATE */
      /* #3020 Do not process the option. */
      optionLength = 0u;
      break;
  }

  return optionLength;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeConfigurationOptions()
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
SD_LOCAL_INLINE FUNC(boolean, SD_CODE) Sd_Tx_SerializeConfigurationOptions(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfCapabilityRecordType hostnameStartIdx, hostnameEndIdx;
  Sd_SizeOfCapabilityRecordType capabilityRecordStartIdx, capabilityRecordEndIdx;
  boolean bufferTooSmall = FALSE;
  uint16 additionalOptionSize;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get relevant configuration options. */
  Sd_Tx_GetRelevantConfigOptions(SendEntryIdx, PartitionIdx, &hostnameStartIdx, &hostnameEndIdx,
    &capabilityRecordStartIdx, &capabilityRecordEndIdx);                                                                /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #20 Check if hostname has to be added as configuration option. */
  if(hostnameStartIdx != hostnameEndIdx)
  {
    /* #200 Build and attach the configuration option. */
    additionalOptionSize = Sd_Tx_SerializeConfigurationOption(PartitionIdx, hostnameStartIdx, hostnameEndIdx,
      OptionArrayInfoPtr->ArrayEndIdx);

    /* #201 Check if the option could be serialized. */
    if(additionalOptionSize != SD_INV_OPTION_LENGTH)
    {
      /* #2010 Check if option was already serialized to the payload and can be deduplicated. */
      if(FALSE == Sd_Tx_DeduplicateOption(PartitionIdx, EntryToOptionMapPtr, OptionArrayInfoPtr,
        (boolean)(capabilityRecordStartIdx != capabilityRecordEndIdx)))                                                 /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #20100 Use option which was serialized to the end of the payload. */
        Sd_Tx_UseNewSerializedOption(EntryToOptionMapPtr, OptionArrayInfoPtr, additionalOptionSize);                    /* SBSW_SD_PASSING_API_POINTER */
      }
    }
    else
    {
      bufferTooSmall = TRUE;
    }
  }

  /* #30 Check if capability records have to be added as configuration option. */
  if(capabilityRecordStartIdx != capabilityRecordEndIdx)
  {
    /* #300 Build and attach the configuration option. */
    additionalOptionSize = Sd_Tx_SerializeConfigurationOption(PartitionIdx, capabilityRecordStartIdx,
      capabilityRecordEndIdx, OptionArrayInfoPtr->ArrayEndIdx);

    /* #301 Check if the option could be serialized. */
    if(additionalOptionSize != SD_INV_OPTION_LENGTH)
    {
      /* #3010 Check if option was already serialized to the payload and can be deduplicated. */
      if(FALSE == Sd_Tx_DeduplicateOption(PartitionIdx, EntryToOptionMapPtr, OptionArrayInfoPtr, FALSE))                /* SBSW_SD_PASSING_API_POINTER */
      {
        /* #30100 Use option which was serialized to the end of the payload. */
        Sd_Tx_UseNewSerializedOption(EntryToOptionMapPtr, OptionArrayInfoPtr, additionalOptionSize);                    /* SBSW_SD_PASSING_API_POINTER */
      }
    }
    else
    {
      bufferTooSmall = TRUE;
    }
  }

  return bufferTooSmall;
}

/**********************************************************************************************************************
 *  Sd_Tx_SerializeConfigurationOption()
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
SD_LOCAL FUNC(uint16, SD_CODE) Sd_Tx_SerializeConfigurationOption(
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfCapabilityRecordType ConfigurationOptionStartIdx,
  Sd_SizeOfCapabilityRecordType ConfigurationOptionEndIdx,
  uint16 PayloadOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfCapabilityRecordType configurationOptionLength = ConfigurationOptionEndIdx - ConfigurationOptionStartIdx;
  uint16 writtenBytes;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the configuration option fits into the buffer. */
  if((PayloadOffset + SD_OPTION_CONFIG_FIXED_HEADER_SIZE + (uint16)configurationOptionLength) <=
      (uint16)Sd_GetSizeOfPayload(PartitionIdx))
  {
    /* #100 Set the type field. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_TYPE_OFFSET, PartitionIdx),
      SD_OPTION_CONFIGURATION);                                                                                         /* SBSW_SD_TX_SERIALIZE_OPTION */

    /* #101 Set the reserved field. */
    Sd_Util_Write8Bit(Sd_GetAddrPayload(PayloadOffset + SD_OPTION_RESERVED_OFFSET, PartitionIdx), 0u);                  /* SBSW_SD_TX_SERIALIZE_OPTION */

    /* #102 Add the configuration option. */
    /* PRQA S 0314 2 */ /* MD_Sd_Dir-1.1_0314_copy */
    Sd_Copy((SD_P2VAR(void)) Sd_GetAddrPayload(PayloadOffset + SD_OPTION_CONFIG_STRING_OFFSET, PartitionIdx),           /* SBSW_SD_TX_SERIALIZE_OPTION */ /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION */
      (SD_P2CONST(void)) Sd_GetAddrCapabilityRecord(ConfigurationOptionStartIdx), configurationOptionLength);

    /* #103 Set the length of the capability record. */
    writtenBytes = (uint16)(SD_OPTION_CONFIG_STRING_OFFSET + (uint16)configurationOptionLength);
    Sd_Util_Write16Bit(Sd_GetAddrPayload(PayloadOffset /* + SD_OPTION_LENGTH_OFFSET */, PartitionIdx),
      (uint16)(writtenBytes - SD_OPTION_NOT_COVERED_FIELDS));                                                           /* SBSW_SD_TX_SERIALIZE_OPTION */
  }
  /* #20 Otherwise, buffer too small. */
  else
  {
    /* #200 Invalidate the length information of the written bytes. */
    writtenBytes = SD_INV_OPTION_LENGTH;
  }

  return writtenBytes;
}

/**********************************************************************************************************************
 *  Sd_Tx_UseNewSerializedOption()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_Tx_UseNewSerializedOption(
  SD_P2VAR(Sd_EntryToOptionMapType) EntryToOptionMapPtr,
  SD_P2VAR(Sd_OptionArrayInfoType) OptionArrayInfoPtr,
  uint16 NewOptionSize)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Use option which was serialized to the end of the payload.
   *     Check if the option can be referenced by the first or the second option run. */

  if(EntryToOptionMapPtr->Nr1stOptions == 0u)
  {
    /* #100 Use first option run to index the option. */
    EntryToOptionMapPtr->Index1st = OptionArrayInfoPtr->FreeOptionIndex;                                                /* SBSW_SD_POINTER_WRITE */
    EntryToOptionMapPtr->Nr1stOptions++;                                                                                /* SBSW_SD_POINTER_WRITE */
  }
  else if(OptionArrayInfoPtr->FreeOptionIndex == (EntryToOptionMapPtr->Index1st + EntryToOptionMapPtr->Nr1stOptions))
  {
    /* #101 Extend first option run at the end to index new option. */
    EntryToOptionMapPtr->Nr1stOptions++;                                                                                /* SBSW_SD_POINTER_WRITE */
  }
  else if(EntryToOptionMapPtr->Nr2ndOptions == 0u)
  {
    /* #102 Use second option run to index the option. */
    EntryToOptionMapPtr->Index2nd = OptionArrayInfoPtr->FreeOptionIndex;                                                /* SBSW_SD_POINTER_WRITE */
    EntryToOptionMapPtr->Nr2ndOptions++;                                                                                /* SBSW_SD_POINTER_WRITE */
  }
  else /* (OptionArrayInfoPtr->FreeOptionIndex == (EntryToOptionMapPtr->Index2nd + EntryToOptionMapPtr->Nr2ndOptions) */
  {
    /* #103 Extend second option run at the end to index new option. */
    EntryToOptionMapPtr->Nr2ndOptions++;                                                                                /* SBSW_SD_POINTER_WRITE */
  }

  OptionArrayInfoPtr->FreeOptionIndex++;                                                                                /* SBSW_SD_POINTER_WRITE */
  OptionArrayInfoPtr->ArrayEndIdx += NewOptionSize;                                                                     /* SBSW_SD_POINTER_WRITE */
}

/**********************************************************************************************************************
 *  Sd_UtilClient_CloseAllMulticastSoCons()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_CloseAllMulticastSoCons(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_MulticastSoConIdIterType multicastSoConIdIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Close all multicast socket connections of the client. */
  for(multicastSoConIdIdx = Sd_GetMulticastSoConIdStartIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
      multicastSoConIdIdx < Sd_GetMulticastSoConIdEndIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
      multicastSoConIdIdx++)
  {
    (void)SoAd_CloseSoCon(Sd_GetMulticastSoConId(multicastSoConIdIdx, PartitionIdx), Abort);
  }
}

/**********************************************************************************************************************
 *  Sd_UtilClient_ResetDynamicVariables()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_ResetDynamicVariables(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset dynamic variables. */
  Sd_SetStateMachineOfClientDyn(ClientIdx, SD_CLIENT_STATE_NONE, PartitionIdx);                                         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_SetStateMachineExecutionRequestedOfClientDyn(ClientIdx, TRUE, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_SetEventRxOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE, PartitionIdx);                                      /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_SetEventMainOfClientDyn(ClientIdx, SD_CLIENT_EVENT_SERVICE_NONE, PartitionIdx);                                    /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_SetAddressStorageServerIdxOfClientDyn(ClientIdx, Sd_GetSizeOfAddressStorage(PartitionIdx), PartitionIdx);          /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  (Sd_GetAddrServerTcpEndpointAddrOfClientDyn(ClientIdx, PartitionIdx))->family = SOAD_AF_INVALID;                      /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  (Sd_GetAddrServerUdpEndpointAddrOfClientDyn(ClientIdx, PartitionIdx))->family = SOAD_AF_INVALID;                      /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_SetMethodUdpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_SetMethodTcpSoConIdOfClientDyn(ClientIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_SetOfferedTTLOfClientDyn(ClientIdx, 0u, PartitionIdx);                                                             /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_Util_ResetTimerToZero(Sd_GetAddrTimerTTLOfClientDyn(ClientIdx, PartitionIdx));                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToTriggerSendOfClientDyn(ClientIdx, PartitionIdx));                         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  Sd_SetRepetitionRunOfClientDyn(ClientIdx, 0u, PartitionIdx);                                                          /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  Sd_Util_ResetTimerToInvalid(Sd_GetAddrRetryTimerOfClientDyn(ClientIdx, PartitionIdx));                                /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
  Sd_SetRetryCounterOfClientDyn(ClientIdx, 0u, PartitionIdx);                                                           /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
}

/**********************************************************************************************************************
 *  Sd_UtilClient_LocalIpAddrAssignmentChgReset()
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
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_LocalIpAddrAssignmentChgReset(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset dynamic variables of all clients with and all consumed event groups. */
  for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx, PartitionIdx);
      clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx, PartitionIdx);
      clientIdx++)
  {
    Sd_SizeOfConsumedEventgroupType consumedEventgroupIdx;

    for(consumedEventgroupIdx = Sd_GetConsumedEventgroupStartIdxOfClient(clientIdx, PartitionIdx);
      consumedEventgroupIdx < Sd_GetConsumedEventgroupEndIdxOfClient(clientIdx, PartitionIdx);
      consumedEventgroupIdx++)
    {
      /* #1000 Release the multicast IP address of the consumed eventgroup. */
      (void)Sd_StateClient_ReleaseIpAddrAssignment(PartitionIdx,
        Sd_GetMulticastSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx));

      /* #1001 Reset dynamic variables of the consumed eventgroup. */
      Sd_SetEventOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CLIENT_EVENT_EVENTGROUP_NONE, PartitionIdx);         /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      Sd_SetAcknowledgedTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, 0u, PartitionIdx);                            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      Sd_Util_ResetTimerToZero(Sd_GetAddrTimerTTLOfConsumedEventgroupDyn(consumedEventgroupIdx, PartitionIdx));         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE */

      Sd_SetTcpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                 /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      Sd_SetUdpSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);                 /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
      Sd_SetMulticastSoConIdOfConsumedEventgroupDyn(consumedEventgroupIdx, SOAD_INV_SO_CON_ID, PartitionIdx);           /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */

      if(Sd_IsMulticastEndpointAddrUsedOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx))
      {
        Sd_MulticastEndpointAddrIdxOfConsumedEventgroupType multicastEndpointAddrIdx =
          Sd_GetMulticastEndpointAddrIdxOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx);

        (Sd_GetAddrMulticastEndpointAddr(multicastEndpointAddrIdx, PartitionIdx))->family = SOAD_AF_INVALID;            /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
      }

      /* #1002 Inform the BswM about the state of the consumed eventgroups. */
      BswM_Sd_ConsumedEventGroupCurrentState(
        Sd_GetHandleIdOfConsumedEventgroup(consumedEventgroupIdx, PartitionIdx), SD_CONSUMED_EVENTGROUP_DOWN);
      Sd_SetCurrentStateOfConsumedEventgroupDyn(consumedEventgroupIdx, SD_CONSUMED_EVENTGROUP_DOWN, PartitionIdx);      /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE */
    }

    /* #200 Reset dynamic variables of the client. */
    Sd_UtilClient_ResetDynamicVariables(clientIdx, PartitionIdx);

    /* #201 Inform the BswM about the state of the client service instance. */
    Sd_UtilClient_SetCurrentStateInformBswm(clientIdx, PartitionIdx, SD_CLIENT_SERVICE_DOWN);
  }
}

/**********************************************************************************************************************
 *  Sd_UtilClient_SetCurrentStateInformBswm()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_UtilClient_SetCurrentStateInformBswm(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ClientServiceCurrentStateType State)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the current state of the client. */
  Sd_SetCurrentStateOfClientDyn(ClientIdx, State, PartitionIdx);                                                        /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Inform the BswM about the state of the client service instance. */
  BswM_Sd_ClientServiceCurrentState(Sd_GetHandleIdOfClient(ClientIdx, PartitionIdx), State);
}

#if ( SD_DEV_ERROR_DETECT == STD_ON )
# if ( SD_MULTI_PARTITION == STD_ON )                                                                                   /* COV_SD_QM_FEATURE_MULTI_PARTITION */
/**********************************************************************************************************************
 *  Sd_Util_CheckApplicationIdValidity()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_CheckApplicationIdValidity(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const ApplicationType osApplicationId = (ApplicationType)GetApplicationID();
  Std_ReturnType  retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the OS application identifier contains a valid value. */
  if (osApplicationId >= INVALID_OSAPPLICATION)
  {
    retVal = E_NOT_OK;
  }
  /* #20 Check if the mapped OS application ID of the partition does not match to the OS application ID otherwise. */
  else if (Sd_GetOsApplicationIdOfPartitionData(SD_PARTITION_DATA_IDX, PartitionIdx) != osApplicationId)
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
}
# endif /* SD_MULTI_PARTITION == STD_ON */
#endif /* SD_DEV_ERROR_DETECT == STD_ON */

/**********************************************************************************************************************
 *  Sd_Util_GetSoConMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL FUNC(Sd_SoConModeType, SD_CODE) Sd_Util_GetSoConMode(
  SoAd_SoConIdType SoConId,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SoConModeType tmpSoConMode = SD_SOCON_MODE_OFFLINE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return socket connection mode if socket connection Id is valid. */
  if((SoConId < Sd_GetSizeOfSoConModeIdMap()) && (Sd_IsInvalidHndOfSoConModeIdMap(SoConId) == FALSE))
  {
    const Sd_SizeOfSoConModeDynType soConModeIdx = Sd_GetPartitionSpecificSoConModeIdxOfSoConModeIdMap(SoConId);
    const Sd_PartitionConfigIdxType soConPartitionIdx =
      Sd_GetPartitionSpecificSoConModePartitionIdxOfSoConModeIdMap(SoConId);
    if (PartitionIdx == soConPartitionIdx)
    {
      SD_BEGIN_CRITICAL_SECTION();
      tmpSoConMode = Sd_GetSoConModeDyn(soConModeIdx, PartitionIdx);
      SD_END_CRITICAL_SECTION();
    }
  }
  return tmpSoConMode;
}

/**********************************************************************************************************************
 *  Sd_Util_ResetSessionIds()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ResetSessionIds(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfAddressStorageType addressStorageIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over the address storage. */
  for(addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx, PartitionIdx);
      addressStorageIdx < Sd_GetAddressStorageEndIdxOfInstance(InstanceIdx, PartitionIdx);
      addressStorageIdx++)
  {
    /* #100 Check if the address is in use. */
    if(Sd_GetSessionIdCounterOfAddressStorage(addressStorageIdx, PartitionIdx) != 0u)
    {
      /* #1000 Reset the SessionID of this address. */
      Sd_SetSessionIdCounterOfAddressStorage(addressStorageIdx, 1u, PartitionIdx);                                      /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
    }
  }
}

/**********************************************************************************************************************
 *  Sd_Util_GetEmptySendEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_SizeOfSendEntryType, SD_CODE) Sd_Util_GetEmptySendEntry(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType emptyEntryIdxPrev;
  Sd_SizeOfSendEntryType invalidEntryIdx = Sd_GetSizeOfSendEntry(PartitionIdx);
  Sd_SizeOfSendEntryType emptyEntryIdx = invalidEntryIdx;

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Check if free send entries list has any entry. */
  if(Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, PartitionIdx) <
       Sd_GetSendEntryCapacityOfInstance(InstanceIdx, PartitionIdx))
  {
    /* #100 Remove tail entry from free send entry list. */
    emptyEntryIdx = Sd_GetSendEntryFreeListTailIdxOfInstanceDyn(InstanceIdx, PartitionIdx);
    emptyEntryIdxPrev = Sd_GetSendEntryPrevIdxOfSendEntry(emptyEntryIdx, PartitionIdx);

    /* If removing last element from free list */
    if(emptyEntryIdxPrev == invalidEntryIdx)
    {
      Sd_SetSendEntryFreeListHeadIdxOfInstanceDyn(InstanceIdx, invalidEntryIdx, PartitionIdx);                          /* SBSW_SD_API_ARGUMENT_IDX */
    }
    else
    {
      Sd_SetSendEntryNextIdxOfSendEntry(emptyEntryIdxPrev, invalidEntryIdx, PartitionIdx);                              /* SBSW_SD_SEND_ENTRY_IDX_INDIRECTION */
    }

    /* #101 Update TailIdx and pending counter of free send entry list. */
    Sd_SetSendEntryFreeListTailIdxOfInstanceDyn(InstanceIdx, emptyEntryIdxPrev, PartitionIdx);                          /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_IncSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, PartitionIdx);                                                /* SBSW_SD_API_ARGUMENT_IDX */
    SD_END_CRITICAL_SECTION();
  }
  else
  {
    SD_END_CRITICAL_SECTION();

    /* #20 If there are no free send entries, report DET error. */
#if(SD_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportError(SD_MODULE_ID, SD_INSTANCE_ID, SD_SID_GET_EMPTY_SENDENTRY, SD_E_FREE_SEND_ENTRY_LIST_EMPTY);
#endif
  }

  return emptyEntryIdx;
}

/**********************************************************************************************************************
 *  Sd_Util_CommitSendEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_CommitSendEntry(
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfSendEntryType ReservedEntryIdx,
  uint32 AdditionalDelay)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType invalidEntryIdx = Sd_GetSizeOfSendEntry(PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Check if entry index is valid. */
  if(ReservedEntryIdx < invalidEntryIdx)
  {
    /* #100 Add send entry to send entry list of address storage. */
    /* If send entry list of address storage (used list) is empty */
    if(Sd_GetSendEntryHeadIdxOfAddressStorage(AddressStorageIdx, PartitionIdx) == invalidEntryIdx)
    {
      Sd_SetSendEntryHeadIdxOfAddressStorage(AddressStorageIdx, ReservedEntryIdx, PartitionIdx);                        /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
      Sd_SetSendEntryPrevIdxOfSendEntry(ReservedEntryIdx, invalidEntryIdx, PartitionIdx);                               /* SBSW_SD_SEND_ENTRY_IDX */
    }
    else
    {
      Sd_SetSendEntryPrevIdxOfSendEntry(ReservedEntryIdx,                                                               /* SBSW_SD_SEND_ENTRY_IDX */
        Sd_GetSendEntryTailIdxOfAddressStorage(AddressStorageIdx, PartitionIdx), PartitionIdx);
      Sd_SetSendEntryNextIdxOfSendEntry(                                                                                /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
        Sd_GetSendEntryTailIdxOfAddressStorage(AddressStorageIdx, PartitionIdx), ReservedEntryIdx, PartitionIdx);
    }
    Sd_SetSendEntryNextIdxOfSendEntry(ReservedEntryIdx, invalidEntryIdx, PartitionIdx);                                 /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetSendEntryTailIdxOfAddressStorage(AddressStorageIdx, ReservedEntryIdx, PartitionIdx);                          /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    /* #101 Update send entry pending counter of address storage. */
    Sd_IncSendEntryNrPendingIdxOfAddressStorage(AddressStorageIdx, PartitionIdx);                                       /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

    /* 102 Update TimeToSend of address storage. */
    if(AdditionalDelay == 0u)
    {
      Sd_Util_SetTimerOffset(PartitionIdx,
        Sd_GetAddrTimeToSendOfAddressStorage(AddressStorageIdx, PartitionIdx), 0u);                                     /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }
    else
    {
      Sd_Util_CheckAndRescheduleTimer(PartitionIdx, AdditionalDelay,
        Sd_GetAddrTimeToSendOfAddressStorage(AddressStorageIdx, PartitionIdx));                                         /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE */
    }
  }

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_ClearSendEntryList()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_ClearSendEntryList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfAddressStorageType addressStorageIdx;
  Sd_SizeOfSendEntryType sendEntryIdx;

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();
  /* #10 Iterate over all address storages. */
  for(addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(InstanceIdx, PartitionIdx);
      addressStorageIdx < Sd_GetAddressStorageEndIdxOfInstance(InstanceIdx, PartitionIdx);
      addressStorageIdx++)
  {
    /* #100 Check that send entry list of address storage is not empty. */
    sendEntryIdx = Sd_GetSendEntryHeadIdxOfAddressStorage(addressStorageIdx, PartitionIdx);
    if(sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx))
    {
      /* #1000 Remove all entries from send entry list of address storage. */
      while(Sd_Util_RemoveSendEntryFromList(InstanceIdx, PartitionIdx, addressStorageIdx, sendEntryIdx) == E_OK)
      {
        sendEntryIdx = Sd_GetSendEntryHeadIdxOfAddressStorage(addressStorageIdx, PartitionIdx);
      }
    }
  }

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_RemoveSendEntryFromList()
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
SD_LOCAL FUNC(Std_ReturnType, SD_CODE) Sd_Util_RemoveSendEntryFromList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_SizeOfSendEntryType RemoveEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType result = E_NOT_OK;
  Sd_SizeOfSendEntryType invalidEntryIdx = Sd_GetSizeOfSendEntry(PartitionIdx);
  Sd_SendEntryNrPendingIdxOfAddressStorageType sendEntriesPending;

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();
  sendEntriesPending = Sd_GetSendEntryNrPendingIdxOfAddressStorage(AddressStorageIdx, PartitionIdx);

  /* #10 Check if send entry index is valid and send entry list contains at least one entry. */
  if((RemoveEntryIdx < invalidEntryIdx) &&
     (sendEntriesPending > 0u))
  {
    Sd_SizeOfSendEntryType removeEntryIdxNext = Sd_GetSendEntryNextIdxOfSendEntry(RemoveEntryIdx, PartitionIdx);
    Sd_SizeOfSendEntryType removeEntryIdxPrev = Sd_GetSendEntryPrevIdxOfSendEntry(RemoveEntryIdx, PartitionIdx);

    /* #100 Remove send entry from send entry list of address storage. */
    /* Deleting from head of used list? */
    if(removeEntryIdxPrev == invalidEntryIdx)
    {
      Sd_SetSendEntryHeadIdxOfAddressStorage(AddressStorageIdx, removeEntryIdxNext, PartitionIdx);                      /* SBSW_SD_API_ARGUMENT_IDX */
    }
    else
    {
      Sd_SetSendEntryNextIdxOfSendEntry(removeEntryIdxPrev, removeEntryIdxNext, PartitionIdx);                          /* SBSW_SD_SEND_ENTRY_IDX_INDIRECTION */
    }
    /* Deleting from tail of used list? */
    if(removeEntryIdxNext == invalidEntryIdx)
    {
      Sd_SetSendEntryTailIdxOfAddressStorage(AddressStorageIdx, removeEntryIdxPrev, PartitionIdx);                      /* SBSW_SD_API_ARGUMENT_IDX */
    }
    else
    {
      Sd_SetSendEntryPrevIdxOfSendEntry(removeEntryIdxNext, removeEntryIdxPrev, PartitionIdx);                          /* SBSW_SD_SEND_ENTRY_IDX_INDIRECTION */
    }

    /* #101 Add send entry to free send entry list. */
    /* If free send entry list is empty */
    if(Sd_GetSendEntryFreeListTailIdxOfInstanceDyn(InstanceIdx, PartitionIdx) == invalidEntryIdx)
    {
      Sd_SetSendEntryFreeListHeadIdxOfInstanceDyn(InstanceIdx, RemoveEntryIdx, PartitionIdx);                           /* SBSW_SD_API_ARGUMENT_IDX */
      Sd_SetSendEntryNextIdxOfSendEntry(RemoveEntryIdx, invalidEntryIdx, PartitionIdx);                                 /* SBSW_SD_SEND_ENTRY_IDX */
      Sd_SetSendEntryPrevIdxOfSendEntry(RemoveEntryIdx, invalidEntryIdx, PartitionIdx);                                 /* SBSW_SD_SEND_ENTRY_IDX */
    }
    else
    {
      Sd_SetSendEntryNextIdxOfSendEntry(Sd_GetSendEntryFreeListTailIdxOfInstanceDyn(InstanceIdx, PartitionIdx),         /* SBSW_SD_SEND_ENTRY_IDX_INDIRECTION */
        RemoveEntryIdx, PartitionIdx);
      Sd_SetSendEntryPrevIdxOfSendEntry(RemoveEntryIdx,                                                                 /* SBSW_SD_SEND_ENTRY_IDX */
        Sd_GetSendEntryFreeListTailIdxOfInstanceDyn(InstanceIdx, PartitionIdx), PartitionIdx);
    }
    Sd_SetSendEntryFreeListTailIdxOfInstanceDyn(InstanceIdx, RemoveEntryIdx, PartitionIdx);                             /* SBSW_SD_API_ARGUMENT_IDX */

    /* #102 Update send entry pending counters of address storage and instance. */
    sendEntriesPending--;
    Sd_SetSendEntryNrPendingIdxOfAddressStorage(AddressStorageIdx, sendEntriesPending, PartitionIdx);                   /* SBSW_SD_API_ARGUMENT_IDX */
    Sd_DecSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, PartitionIdx);                                                /* SBSW_SD_API_ARGUMENT_IDX */

    /* #103 Reset TimeToSend if list is empty now. */
    if(sendEntriesPending == 0u)
    {
      Sd_Util_ResetTimerToInvalid(Sd_GetAddrTimeToSendOfAddressStorage(AddressStorageIdx, PartitionIdx));               /* SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT */
    }

    result = E_OK;
  }

  SD_END_CRITICAL_SECTION();

  return result;
}

/**********************************************************************************************************************
 *  Sd_Util_CheckSendEntryListFullAndTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_CheckSendEntryListFullAndTransmit(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the list with pending send entries is full. */
  if(Sd_GetSendEntryNrPendingIdxOfInstanceDyn(InstanceIdx, PartitionIdx) >=
       Sd_GetSendEntryCapacityOfInstance(InstanceIdx, PartitionIdx))
  {
    /* #100 The send entry list is full, trigger a transmission. */
    Sd_Tx_ProcessPendingMessages(InstanceIdx, PartitionIdx, TRUE);
  }
}

/**********************************************************************************************************************
 *  Sd_Util_AddFindEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddFindEntryToList(
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);
  Sd_SizeOfAddressStorageType addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(instanceIdx, PartitionIdx);
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(instanceIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* Currently the specification does not support sending FindService entries using unicast. */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx))
  {
    /* #100 Save all information relevant for SD_ENTRY_FIND_SERVICE entry. */
    Sd_UtilClient_ConfigureEntry(sendEntryIdx, ClientIdx, PartitionIdx);
    Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_FIND_SERVICE, PartitionIdx);                                      /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_Util_CommitSendEntry(addressStorageIdx, PartitionIdx, sendEntryIdx, 0u);
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(instanceIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Util_AddMulticastOfferEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddMulticastOfferEntryToList(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean StopOffer)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);
  Sd_SizeOfAddressStorageType addressStorageIdx = Sd_GetAddressStorageStartIdxOfInstance(instanceIdx, PartitionIdx);
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(instanceIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx))
  {
    /* #100 Save all information relevant for multicast SD_ENTRY_(STOP)_OFFER_SERVICE entry. */
    Sd_UtilServer_ConfigureEntry(sendEntryIdx, ServerIdx, PartitionIdx);

    if(StopOffer == FALSE)
    {
      Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_OFFER_SERVICE, PartitionIdx);                                   /* SBSW_SD_SEND_ENTRY_IDX */
    }
    else
    {
      Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_STOP_OFFER_SERVICE, PartitionIdx);                              /* SBSW_SD_SEND_ENTRY_IDX */
    }
    Sd_Util_CommitSendEntry(addressStorageIdx, PartitionIdx, sendEntryIdx, 0u);
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(instanceIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Util_AddUnicastOfferEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddUnicastOfferEntryToList(
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfAddressStorageType SavedAddressStorageIdx,
  uint32 TransmissionOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(instanceIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx))
  {
    /* #100 Save all information relevant for unicast SD_ENTRY_OFFER_SERVICE entry. */
    Sd_UtilServer_ConfigureEntry(sendEntryIdx, ServerIdx, PartitionIdx);
    Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_OFFER_SERVICE, PartitionIdx);                                     /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_Util_CommitSendEntry(SavedAddressStorageIdx, PartitionIdx, sendEntryIdx, TransmissionOffset);
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(instanceIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Util_AddSubscribeEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddSubscribeEntryToList(
  Sd_SizeOfConsumedEventgroupType ConsumedEventgroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean Subscribe,
  uint32 TransmissionOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientType clientIdx = Sd_GetClientIdxOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx);
  Sd_SizeOfAddressStorageType addressStorageIdx = Sd_GetAddressStorageServerIdxOfClientDyn(clientIdx, PartitionIdx);
  Sd_SizeOfInstanceType instanceIdx = Sd_GetInstanceIdxOfClient(clientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(instanceIdx, PartitionIdx);

  if(sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx))
  {
    /* #100 Save all information relevant for SD_ENTRY_(STOP)_SUBSCRIBE_EVENTGROUP entry. */
    Sd_UtilClient_ConfigureEntry(sendEntryIdx, clientIdx, PartitionIdx);
    if(Subscribe == TRUE)
    {
      Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_SUBSCRIBE_EVENTGROUP, PartitionIdx);                            /* SBSW_SD_SEND_ENTRY_IDX */
    }
    else
    {
      Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_STOP_SUBSCRIBE_EVENTGROUP, PartitionIdx);                       /* SBSW_SD_SEND_ENTRY_IDX */
    }
    Sd_SetConsumedEventgroupIdxOfSendEntry(sendEntryIdx, ConsumedEventgroupIdx, PartitionIdx);                          /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetEventHandlerIdOfSendEntry(sendEntryIdx,                                                                       /* SBSW_SD_SEND_ENTRY_IDX */
      Sd_GetEventGroupIdOfConsumedEventgroup(ConsumedEventgroupIdx, PartitionIdx), PartitionIdx);

    Sd_Util_CommitSendEntry(addressStorageIdx, PartitionIdx, sendEntryIdx, TransmissionOffset);
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(instanceIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Util_AddEventGroupAckEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddEventGroupAckEntryToList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfRxEventType RxEventIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfClientListType clientListIdx = Sd_GetClientListIdxOfRxEvent(RxEventIdx, PartitionIdx);
  Sd_SizeOfAddressStorageType addressStorageIdx = Sd_GetAddressStorageIdxOfClientList(clientListIdx, PartitionIdx);
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(InstanceIdx, PartitionIdx);
  uint32 additionalDelay = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx))
  {
    /* #100 Save all information relevant for SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK entry. */
    Sd_SizeOfEventHandlerType eventHandlerIdx = Sd_GetEventHandlerIdxOfRxEvent(RxEventIdx, PartitionIdx);
    Sd_SizeOfServerType serverIdx = Sd_GetServerIdxOfEventHandler(eventHandlerIdx, PartitionIdx);

    Sd_UtilServer_ConfigureEntry(sendEntryIdx, serverIdx, PartitionIdx);

    /* Update the entry list. */
    Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_SUBSCRIBE_EVENTGROUP_ACK, PartitionIdx);                          /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetEventHandlerIdxOfSendEntry(sendEntryIdx, eventHandlerIdx, PartitionIdx);                                      /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetClientListIdxOfSendEntry(sendEntryIdx, clientListIdx, PartitionIdx);                                          /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetEventHandlerIdOfSendEntry(sendEntryIdx,                                                                       /* SBSW_SD_SEND_ENTRY_IDX */
      Sd_GetEventGroupIdOfEventHandler(eventHandlerIdx, PartitionIdx), PartitionIdx);
    Sd_SetReservedFieldOfSendEntry(sendEntryIdx,                                                                        /* SBSW_SD_SEND_ENTRY_IDX */
      Sd_GetRxReservedOfClientList(clientListIdx, PartitionIdx), PartitionIdx);

    if(Sd_GetEventOfRxEvent(RxEventIdx, PartitionIdx) == SD_SERVER_EVENT_EVENTGROUP_SUBSCRIBE_MULTI)
    {
      additionalDelay = Sd_Util_GetRandomNrInRangeForResponse(
        Sd_GetRequestResponseMinDelayOfServerTimer(
          Sd_GetServerTimerIdxOfEventHandler(eventHandlerIdx, PartitionIdx), PartitionIdx),
        Sd_GetRequestResponseMaxDelayOfServerTimer(
          Sd_GetServerTimerIdxOfEventHandler(eventHandlerIdx, PartitionIdx), PartitionIdx),
        FALSE);
    }

    Sd_Util_CommitSendEntry(addressStorageIdx, PartitionIdx, sendEntryIdx, additionalDelay);
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(InstanceIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Util_AddEventGroupNackEntryToList()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_Sd_STPAR_6060 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_AddEventGroupNackEntryToList(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_SizeOfServerType ServerIdx,
  uint16 ServiceId,
  uint16 InstanceId,
  uint8 MajorVersion,
  uint16 ReservedField,
  uint16 EventHandlerId,
  Sd_SizeOfAddressStorageType AddressStorageIdx,
  Sd_SizeOfServerTimerType ServerTimerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfSendEntryType sendEntryIdx = Sd_Util_GetEmptySendEntry(InstanceIdx, PartitionIdx);
  uint32 additionalDelay = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the send entry list contains a free slot. */
  if(sendEntryIdx < Sd_GetSizeOfSendEntry(PartitionIdx))
  {
    /* #100 Save all information relevant for SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK entry. */
    Sd_SetEntryTypeOfSendEntry(sendEntryIdx, SD_ENTRY_SUBSCRIBE_EVENTGROUP_NACK, PartitionIdx);                         /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetInstanceIdxOfSendEntry(sendEntryIdx, InstanceIdx, PartitionIdx);                                              /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetClientIdxOfSendEntry(sendEntryIdx, Sd_GetSizeOfClient(PartitionIdx), PartitionIdx);             /* UNUSED */  /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetConsumedEventgroupIdxOfSendEntry(sendEntryIdx, Sd_GetSizeOfConsumedEventgroup(PartitionIdx),    /* UNUSED */  /* SBSW_SD_SEND_ENTRY_IDX */
      PartitionIdx);
    Sd_SetServerIdxOfSendEntry(sendEntryIdx, ServerIdx, PartitionIdx);                                                  /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetEventHandlerIdxOfSendEntry(sendEntryIdx, Sd_GetSizeOfEventHandler(PartitionIdx), PartitionIdx); /* UNUSED */  /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetClientListIdxOfSendEntry(sendEntryIdx, Sd_GetSizeOfClientList(PartitionIdx), PartitionIdx);     /* UNUSED */  /* SBSW_SD_SEND_ENTRY_IDX */

    Sd_SetServiceIdOfSendEntry(sendEntryIdx, ServiceId, PartitionIdx);                                                  /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetInstanceIdOfSendEntry(sendEntryIdx, InstanceId, PartitionIdx);                                                /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetMajorVersionOfSendEntry(sendEntryIdx, MajorVersion, PartitionIdx);                                            /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetEventHandlerIdOfSendEntry(sendEntryIdx, EventHandlerId, PartitionIdx);                                        /* SBSW_SD_SEND_ENTRY_IDX */
    Sd_SetReservedFieldOfSendEntry(sendEntryIdx, ReservedField, PartitionIdx);                                          /* SBSW_SD_SEND_ENTRY_IDX */

    if(ServerTimerIdx != Sd_GetSizeOfServerTimer(PartitionIdx))
    {
      additionalDelay = Sd_Util_GetRandomNrInRangeForResponse(
        Sd_GetRequestResponseMinDelayOfServerTimer(ServerTimerIdx, PartitionIdx),
        Sd_GetRequestResponseMaxDelayOfServerTimer(ServerTimerIdx, PartitionIdx), TRUE);
    }

    Sd_Util_CommitSendEntry(AddressStorageIdx, PartitionIdx, sendEntryIdx, additionalDelay);
  }

  /* #20 Check if the list of send entries is full and trigger a transmission. */
  Sd_Util_CheckSendEntryListFullAndTransmit(InstanceIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_UtilServer_ConfigureEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilServer_ConfigureEntry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfServerType ServerIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_InstanceIdxOfServerType instanceIdx = Sd_GetInstanceIdxOfServer(ServerIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all used parameters of the send entry element. */
  Sd_SetInstanceIdxOfSendEntry(SendEntryIdx, instanceIdx, PartitionIdx);                                                /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetServerIdxOfSendEntry(SendEntryIdx, ServerIdx, PartitionIdx);                                                    /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetServiceIdOfSendEntry(SendEntryIdx, Sd_GetServiceIdOfServer(ServerIdx, PartitionIdx), PartitionIdx);             /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetInstanceIdOfSendEntry(SendEntryIdx, Sd_GetInstanceIdOfServerDyn(ServerIdx, PartitionIdx), PartitionIdx);        /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetMajorVersionOfSendEntry(SendEntryIdx, Sd_GetMajorVersionOfServer(ServerIdx, PartitionIdx), PartitionIdx);       /* SBSW_SD_SEND_ENTRY_IDX */

  /* #20 Initialize all unused parameters of the send entry element with invalid values. */
  Sd_SetClientIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfClient(PartitionIdx), PartitionIdx);             /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetConsumedEventgroupIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfConsumedEventgroup(PartitionIdx),    /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
    PartitionIdx);

  Sd_SetEventHandlerIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfEventHandler(PartitionIdx), PartitionIdx); /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetClientListIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfClientList(PartitionIdx), PartitionIdx);     /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetEventHandlerIdOfSendEntry(SendEntryIdx, 0u, PartitionIdx);                                      /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetReservedFieldOfSendEntry(SendEntryIdx, 0u, PartitionIdx);                                       /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
}

/**********************************************************************************************************************
 *  Sd_UtilClient_ConfigureEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_UtilClient_ConfigureEntry(
  Sd_SizeOfSendEntryType SendEntryIdx,
  Sd_SizeOfClientType ClientIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_InstanceIdxOfClientType instanceIdx = Sd_GetInstanceIdxOfClient(ClientIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all used parameters of the send entry element. */
  Sd_SetInstanceIdxOfSendEntry(SendEntryIdx, instanceIdx, PartitionIdx);                                                /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetClientIdxOfSendEntry(SendEntryIdx, ClientIdx, PartitionIdx);                                                    /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetServiceIdOfSendEntry(SendEntryIdx, Sd_GetServiceIdOfClient(ClientIdx, PartitionIdx), PartitionIdx);             /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetInstanceIdOfSendEntry(SendEntryIdx, Sd_GetInstanceIdOfClientDyn(ClientIdx, PartitionIdx), PartitionIdx);        /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetMajorVersionOfSendEntry(SendEntryIdx, Sd_GetMajorVersionOfClient(ClientIdx, PartitionIdx), PartitionIdx);       /* SBSW_SD_SEND_ENTRY_IDX */

  /* #20 Initialize all unused parameters of the send entry element with invalid values. */
  Sd_SetServerIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfServer(PartitionIdx), PartitionIdx);             /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetEventHandlerIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfEventHandler(PartitionIdx), PartitionIdx); /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetClientListIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfClientList(PartitionIdx), PartitionIdx);     /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */

  Sd_SetConsumedEventgroupIdxOfSendEntry(SendEntryIdx, Sd_GetSizeOfConsumedEventgroup(PartitionIdx),    /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
    PartitionIdx);

  Sd_SetEventHandlerIdOfSendEntry(SendEntryIdx, 0u, PartitionIdx);                                      /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
  Sd_SetReservedFieldOfSendEntry(SendEntryIdx, 0u, PartitionIdx);                                       /* UNUSED */    /* SBSW_SD_SEND_ENTRY_IDX */
}

/**********************************************************************************************************************
 *  Sd_Util_CopySockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_CopySockAddr(
  SD_P2VAR(Sd_SockAddrContainerType) TgtSockPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SrcSockPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy the socket address. */
  /* PRQA S 0310 1 */ /* MD_Sd_11.4_0310 */
  retVal = IpBase_CopySockAddr((SD_P2VAR(IpBase_SockAddrType))TgtSockPtr, (SD_P2CONST(IpBase_SockAddrType))SrcSockPtr); /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_SOCK_ADDR_CAST */

  if(retVal != E_OK)
  {
    /* #100 Source address is invalid, set the target address as invalid. */
    TgtSockPtr->family = SOAD_AF_INVALID;                                                                               /* SBSW_SD_POINTER_WRITE */
  }
}

/**********************************************************************************************************************
 *  Sd_Util_CompareSockAddrIp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_MatchType, SD_CODE) Sd_Util_CompareSockAddrIp(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 AddrState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_MatchType matchType = SD_MATCH_FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check family type of both socket addresses. */
  if(SockAPtr->family == SockBPtr->family)
  {
    /* #100 Check if the address state defines ANY possible IP address. */
    if((AddrState & SOAD_SOCON_MASK_IPADDR & SOAD_SOCON_MASK_ANY) != 0u)
    {
      matchType = SD_MATCH_ANY;
    }
    else
    {
      /* #1000 Otherwise, check if the IP addresses are equal. */
      /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
      if(TRUE == IpBase_SockIpAddrIsEqual(
        (SD_P2CONST(IpBase_SockAddrType))SockAPtr, (SD_P2CONST(IpBase_SockAddrType))SockBPtr))                          /* SBSW_SD_PASSING_API_POINTER */
      {
        matchType = SD_MATCH_TRUE;
      }
    }
  }

  return matchType;
}

/**********************************************************************************************************************
 *  Sd_Util_CompareSockAddrPort()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_MatchType, SD_CODE) Sd_Util_CompareSockAddrPort(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 AddrState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_MatchType matchType = SD_MATCH_FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check family type of both socket addresses. */
  if(SockAPtr->family == SockBPtr->family)
  {
    /* #100 Check if the address state defines ANY possible port. */
    if(((AddrState & SOAD_SOCON_MASK_PORT) & SOAD_SOCON_MASK_ANY) > 0u)
    {
      matchType = SD_MATCH_ANY;
    }
    else
    {
      /* #1000 Otherwise, check if the ports are equal. */
      /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
      if(TRUE == IpBase_SockPortIsEqual(
        (SD_P2CONST(IpBase_SockAddrType))SockAPtr, (SD_P2CONST(IpBase_SockAddrType))SockBPtr))                          /* SBSW_SD_PASSING_API_POINTER */
      {
        matchType = SD_MATCH_TRUE;
      }
    }
  }

  return matchType;
}

/**********************************************************************************************************************
 *  Sd_Util_SockAddrIsEqual()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_SockAddrIsEqual(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean equal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check family and the IP addresses. */
  /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
  if(TRUE == IpBase_SockIpAddrIsEqual(
    (SD_P2CONST(IpBase_SockAddrType))SockAPtr, (SD_P2CONST(IpBase_SockAddrType))SockBPtr))                              /* SBSW_SD_PASSING_API_POINTER */
  {
    /* #100 Check the Ports. */
    /* PRQA S 0310 2 */ /* MD_Sd_11.4_0310 */
    if(TRUE == IpBase_SockPortIsEqual(
      (SD_P2CONST(IpBase_SockAddrType))SockAPtr, (SD_P2CONST(IpBase_SockAddrType))SockBPtr))                            /* SBSW_SD_PASSING_API_POINTER */
    {
      equal = TRUE;
    }
  }

  return equal;
}

/**********************************************************************************************************************
 *  Sd_Rx_CheckSubnets()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_CheckSubnets(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  boolean UdpNeeded,
  boolean TcpNeeded,
  SD_P2VAR(Sd_AttachedOptionsType) AttachedOptionsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_OK;
  uint8 LocalAddrCheckLength = Sd_GetLocalAddressCheckLengthOfInstance(InstanceIdx, PartitionIdx);
  SoAd_SoConIdType localSdInstSoConId = Sd_GetUnicastRxSoConIdOfInstance(InstanceIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the service is configured for UDP and a UDP endpoint option was received. */
  if(UdpNeeded && (AttachedOptionsPtr->EndpointAddrUdp.family != SOAD_AF_INVALID))
  {
    /* #100 Check if the attached UDP endpoint address is part of the subnet. */
    optionsRetVal = Sd_Rx_CheckSubnet(localSdInstSoConId, LocalAddrCheckLength, &AttachedOptionsPtr->EndpointAddrUdp);  /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
  }

  /* #20 Check if the service is configured for TCP and a TCP endpoint option was received. */
  if((optionsRetVal == SD_OPTIONS_OK) &&
     TcpNeeded &&
     (AttachedOptionsPtr->EndpointAddrTcp.family != SOAD_AF_INVALID))
  {
    /* #200 Check if the attached TCP endpoint address is part of the subnet. */
    optionsRetVal = Sd_Rx_CheckSubnet(localSdInstSoConId, LocalAddrCheckLength, &AttachedOptionsPtr->EndpointAddrTcp);  /* SBSW_SD_REFERENCE_OF_POINTER_ELEMENT */
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_CheckSubnet()
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
SD_LOCAL FUNC(Sd_OptionsReturnType, SD_CODE) Sd_Rx_CheckSubnet(
  SoAd_SoConIdType SoConId,
  uint8 LocalAddrCheckLength,
  SD_P2VAR(Sd_SockAddrContainerType) EndpointAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_OptionsReturnType optionsRetVal = SD_OPTIONS_NOT_OK;
  Sd_SockAddrContainerType localAddr;
  uint8 netmaskCidr = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the local IP address and the subnet mask. */
  if(Sd_SoAd_GetLocalAddrWithNetmask(SoConId, &localAddr, &netmaskCidr) == E_OK)                                        /* SBSW_SD_REFERENCE_OF_VARIABLE */
  {
    /* #100 Check if configured subnet mask shall be used. */
    if(LocalAddrCheckLength != SD_NO_LOCAL_ADDRESS_CHECK_LENGTH)
    {
      netmaskCidr = LocalAddrCheckLength;
    }

    /* #101 Check if the endpoint address is part of the subnet. */
    if(Sd_Rx_SockAddrWithinSubnet(&localAddr, EndpointAddrPtr, netmaskCidr) == TRUE)                                    /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_PASSING_API_POINTER */
    {
      optionsRetVal = SD_OPTIONS_OK;
    }
    else
    {
      /* #1010 The endpoint references an IP address outside the configured subnet, do not consider this endpoint. */
      EndpointAddrPtr->family = SOAD_AF_INVALID;                                                                        /* SBSW_SD_POINTER_WRITE */
      optionsRetVal = SD_OPTIONS_WRONG_SUBNET;
    }
  }
  /* #20 Failed to read the local address. Consider option as invalid. */
  else
  {
    EndpointAddrPtr->family = SOAD_AF_INVALID;                                                                          /* SBSW_SD_POINTER_WRITE */
  }

  return optionsRetVal;
}

/**********************************************************************************************************************
 *  Sd_Rx_SockAddrWithinSubnet()
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
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Rx_SockAddrWithinSubnet(
  SD_P2CONST(Sd_SockAddrContainerType) SockAPtr,
  SD_P2CONST(Sd_SockAddrContainerType) SockBPtr,
  uint8 NetmaskCidr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean sameSubnet = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the family type of both addresses are identical. */
  if(SockAPtr->family == SockBPtr->family)
  {
    /* #100 All IP Addresses are accepted if netmask is zero. */
    if(NetmaskCidr == 0u)
    {
      sameSubnet = TRUE;
    }
    else
    {
      /* #1000 If family is IPv4, the netmask can be in the range [1 .. 32]. */
      if((SockAPtr->family == SOAD_AF_INET) && (NetmaskCidr <= 32u))
      {
        uint32 addrA, addrB;

        /* #10000 Generate a copy of the IP addresses in network byte order. */
        addrA = IPBASE_NTOH32(SockAPtr->data[0u]);
        addrB = IPBASE_NTOH32(SockBPtr->data[0u]);

        /* #10001 Shift to remove the variable parts of the IP address. */
        addrA >>= (32u - NetmaskCidr);
        addrB >>= (32u - NetmaskCidr);

        /* #10002 Compare the remaining par of the addresses. */
        if(addrA == addrB)
        {
          sameSubnet = TRUE;
        }
      }
      /* #1001 If family is IPv6, the netmask can be in the range [1 .. 128]. */
      else if((SockAPtr->family == SOAD_AF_INET6) && (NetmaskCidr <= 128u))
      {
        uint32 addrA[4u], addrB[4u];
        uint8_least i;
        uint8 wordsNetmask = ((NetmaskCidr + 31u) / 32u);
        uint8 bitsToClear = 32u - (NetmaskCidr % 32u);

        /* #10010 Generate a copy of the IP addresses in network byte order. */
        addrA[0u] = IPBASE_NTOH32(SockAPtr->data[0u]);                                                                  /* SBSW_SD_IP_ADDR_SIZE */
        addrA[1u] = IPBASE_NTOH32(SockAPtr->data[1u]);                                                                  /* SBSW_SD_IP_ADDR_SIZE */
        addrA[2u] = IPBASE_NTOH32(SockAPtr->data[2u]);                                                                  /* SBSW_SD_IP_ADDR_SIZE */
        addrA[3u] = IPBASE_NTOH32(SockAPtr->data[3u]);                                                                  /* SBSW_SD_IP_ADDR_SIZE */
        addrB[0u] = IPBASE_NTOH32(SockBPtr->data[0u]);                                                                  /* SBSW_SD_IP_ADDR_SIZE */
        addrB[1u] = IPBASE_NTOH32(SockBPtr->data[1u]);                                                                  /* SBSW_SD_IP_ADDR_SIZE */
        addrB[2u] = IPBASE_NTOH32(SockBPtr->data[2u]);                                                                  /* SBSW_SD_IP_ADDR_SIZE */
        addrB[3u] = IPBASE_NTOH32(SockBPtr->data[3u]);                                                                  /* SBSW_SD_IP_ADDR_SIZE */

        /* #10011 Clear the subnet bits of the IP addresses in the word which contains both netmask and subnet part. */
        if(bitsToClear != 32u)
        {
          addrA[wordsNetmask - 1u] >>= bitsToClear;                                                                     /* SBSW_SD_IP_ADDR_SIZE */
          addrB[wordsNetmask - 1u] >>= bitsToClear;                                                                     /* SBSW_SD_IP_ADDR_SIZE */
        }

        /* #10012 Check if the netmask part of both IP addresses is the same. */
        sameSubnet = TRUE;
        for(i = 0u; i < wordsNetmask; i++)
        {
          if(addrA[i] != addrB[i])
          {
            sameSubnet = FALSE;
            break;
          }
        }
      }
      /* #1002 The family is undefined or the netmask is out of range. */
      else
      {
        /* Nothing to do */
      }
    }
  }

  return sameSubnet;
}

/**********************************************************************************************************************
 *  Sd_Util_ReportDemEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ReportDemEvent(
  Sd_Dem_EventIdType EventId)
{
  /* ----- Implementation ----------------------------------------------- */
#if(SD_DEM_EVENTS_CONFIGURED == STD_ON)
  /* #10 Check if the DEM event is configured. */
  if(EventId != 0u)
  {
    /* #100 Report DEM event. */
    Dem_ReportErrorStatus(EventId, DEM_EVENT_STATUS_FAILED);
  }
#else
  SD_DUMMY_STATEMENT(EventId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/**********************************************************************************************************************
 *  Sd_Util_GetRandomNrInRange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(uint32, SD_CODE) Sd_Util_GetRandomNrInRange(
  uint32 MinValue,
  uint32 MaxValue)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 randNo;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a random number has to be calculated */
  if(MinValue == MaxValue)
  {
    /* #100 No need to calculate a random number. */
    randNo = MinValue;
  }
  else
  {
    /* #101 Calculate a random number in the range:  MinValue <= RandInRange <= MaxValue. */
    randNo = (Sd_ExternalRandomNumberFct() % ((MaxValue + 1u) - MinValue)) + MinValue;
  }

  return randNo;
}

/**********************************************************************************************************************
 *  Sd_Util_GetRandomNrInRangeForResponse()
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
SD_LOCAL FUNC(uint32, SD_CODE) Sd_Util_GetRandomNrInRangeForResponse(
  uint32 MinValue,
  uint32 MaxValue,
  boolean InterruptContext)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 tmpMinValue = MinValue, tmpMaxValue = MaxValue;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the context the function is called from. */
  if(InterruptContext == TRUE)
  {
    /* #100 Response is generated in context of message transmission. */

    if((MinValue != 0u) && (MinValue != MaxValue))
    {
      /* #1000 Increase the lower bound in order to consider the elapsed time between the last MainFunction and the RxIndication. */
      tmpMinValue += SD_MAIN_FCT_CYCLE_TIME_MS;
    }
  }
  else
  {
    /* #101 Response is generated in context of MainFunction. */

    if(MaxValue != 0u)
    {
      /* #1010 Decrease the upper bound in order to consider the elapsed time between the last MainFunction and the RxIndication. */
      if(MinValue == MaxValue)
      {
        tmpMinValue -= SD_MAIN_FCT_CYCLE_TIME_MS;
      }

      tmpMaxValue -= SD_MAIN_FCT_CYCLE_TIME_MS;
    }
  }

  return Sd_Util_GetRandomNrInRange(tmpMinValue, tmpMaxValue);
}

/**********************************************************************************************************************
 *  Sd_Util_CheckIfTimerExceededInt()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
  */
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_CheckIfTimerExceededInt(
  Sd_ExtendedTimeType TimerA,
  Sd_ExtendedTimeType TimerB,
  boolean             EqualIsExceeded)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean exceeded = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Check if the seconds are exceeded. */
  if(TimerA.Seconds > TimerB.Seconds)
  {
    exceeded = TRUE;
  }
  else if(TimerA.Seconds == TimerB.Seconds)
  {
    /* #100 Seconds are identical. Check milliseconds. */
    if(TimerA.Milliseconds > TimerB.Milliseconds)
    {
      exceeded = TRUE;
    }
    else if((EqualIsExceeded == TRUE) && ((TimerA.Milliseconds) == (TimerB.Milliseconds)))
    {
      exceeded = TRUE;
    }
    else
    {
      /* #1000 Milliseconds are not exceeded. */
    }
  }
  else
  {
    /* #101 Seconds are not exceeded. */
  }

  SD_END_CRITICAL_SECTION();

  return exceeded;
}

/**********************************************************************************************************************
 *  Sd_Util_CheckIfTimerExceeded()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(boolean, SD_CODE) Sd_Util_CheckIfTimerExceeded(
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ExtendedTimeType Timer,
  boolean EqualIsExceeded)
{
  /* ----- Implementation ----------------------------------------------- */
  return Sd_Util_CheckIfTimerExceededInt(Sd_GetTimeMsSinceStartOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx),
    Timer, EqualIsExceeded);
}

/**********************************************************************************************************************
 *  Sd_Util_CheckAndRescheduleTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_CheckAndRescheduleTimer(
  Sd_PartitionConfigIdxType PartitionIdx,
  uint32 OffsetMs,
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_ExtendedTimeType tempTimer;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add offset to current time. */
  Sd_Util_SetTimerOffset(PartitionIdx, &tempTimer, OffsetMs);                                                           /* SBSW_SD_REFERENCE_OF_VARIABLE */

  /* #20 If requested time is earlier, reschedule timer to earlier point in time. */
  if(Sd_Util_CheckIfTimerExceededInt(*TimerPtr, tempTimer, FALSE) == TRUE)
  {
    *TimerPtr = tempTimer;                                                                                              /* SBSW_SD_POINTER_WRITE */
  }
}

/**********************************************************************************************************************
 *  Sd_Util_TriggerStateMachineRun()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_TriggerStateMachineRun(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Set next state machine execution time to 0 in order to trigger execution in the next MainFunction. */
  (Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx))->Seconds = 0u;                                /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  (Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx))->Milliseconds = 0u;                           /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_SetNextStateMachineRun()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetNextStateMachineRun(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  Sd_ExtendedTimeType Timer)
{
  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Check if the next state machine execution is scheduled later than the given time.
   *     If so, it has to be re-scheduled to an earlier point in time. */
  if((Sd_GetNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx).Seconds > Timer.Seconds) ||
    ((Sd_GetNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx).Seconds == Timer.Seconds) &&
    (Sd_GetNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx).Milliseconds > Timer.Milliseconds)))
  {
    /* #100 Set next state machine execution time to the given time. */
    (Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx))->Seconds      = Timer.Seconds;              /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    (Sd_GetAddrNextStateMachineRunOfInstanceDyn(InstanceIdx, PartitionIdx))->Milliseconds = Timer.Milliseconds;         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
  }

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffsetWithTrigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffsetWithTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetMs,
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the timer. */
  Sd_Util_SetTimerOffset(PartitionIdx, TimerPtr, OffsetMs);                                                             /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Trigger state machine execution based on the given offset. */
  Sd_Util_SetNextStateMachineRun(InstanceIdx, PartitionIdx, *TimerPtr);
}

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffsetSecWithTrigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffsetSecWithTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetSec,
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Check if the TTL shall be considered valid till the next reboot. */
  if(OffsetSec == SD_TTL_INFINITE)
  {
    /* #200 Set the timer value to infinite. This timer will never expire. */
    TimerPtr->Seconds = SD_TIMER_SEC_INFINITE;                                                                          /* SBSW_SD_POINTER_WRITE */
    TimerPtr->Milliseconds = SD_TIMER_MSEC_INFINITE;                                                                    /* SBSW_SD_POINTER_WRITE */
  }
  else
  {
    /* #300 Otherwise, update the timer with the given offset. */
    TimerPtr->Seconds =
      Sd_GetTimeMsSinceStartOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx).Seconds + (OffsetSec);              /* SBSW_SD_POINTER_WRITE */
    TimerPtr->Milliseconds =
      Sd_GetTimeMsSinceStartOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx).Milliseconds;                       /* SBSW_SD_POINTER_WRITE */

    /* #301 Trigger state machine execution based on the given offset. */
    Sd_Util_SetNextStateMachineRun(InstanceIdx, PartitionIdx, *TimerPtr);
  }

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_SetTimerOffset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_SetTimerOffset(
  Sd_PartitionConfigIdxType PartitionIdx,
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  uint32 OffsetMs)
{
  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Update the timer with the given offset.
   *     The timer value is saved in separated variables for seconds and milliseconds. */
  TimerPtr->Seconds      = Sd_GetTimeMsSinceStartOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx).Seconds +      /* SBSW_SD_POINTER_WRITE */
    (OffsetMs / 1000u);
  TimerPtr->Milliseconds = Sd_GetTimeMsSinceStartOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx).Milliseconds + /* SBSW_SD_POINTER_WRITE */
    (uint16)(OffsetMs % 1000u);

  /* #20 Handle the case if the amount of milliseconds exceeds one second. */
  TimerPtr->Seconds      = TimerPtr->Seconds                + ((uint32)TimerPtr->Milliseconds / 1000u);                 /* SBSW_SD_POINTER_WRITE */
  TimerPtr->Milliseconds = TimerPtr->Milliseconds           % 1000u;                                                    /* SBSW_SD_POINTER_WRITE */

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToZero()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ResetTimerToZero(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Reset the timer. */
  TimerPtr->Seconds = 0u;                                                                                               /* SBSW_SD_POINTER_WRITE */
  TimerPtr->Milliseconds = 0u;                                                                                          /* SBSW_SD_POINTER_WRITE */

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToZeroTrigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ResetTimerToZeroTrigger(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr,
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the timer. */
  Sd_Util_ResetTimerToZero(TimerPtr);                                                                                   /* SBSW_SD_PASSING_API_POINTER */

  /* #20 Trigger state machine execution. */
  Sd_Util_TriggerStateMachineRun(InstanceIdx, PartitionIdx);
}

/**********************************************************************************************************************
 *  Sd_Util_ResetTimerToInvalid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_ResetTimerToInvalid(
  SD_P2VAR(Sd_ExtendedTimeType) TimerPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Set the timer values to invalid. */
  TimerPtr->Seconds = SD_INV_TIMER_VALUE_32BIT;                                                                         /* SBSW_SD_POINTER_WRITE */
  TimerPtr->Milliseconds = SD_INV_TIMER_VALUE_16BIT;                                                                    /* SBSW_SD_POINTER_WRITE */

  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_UpdateTimeSinceStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL FUNC(void, SD_CODE) Sd_Util_UpdateTimeSinceStart(
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_ExtendedTimeType timeMsSinceStartTmp = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  SD_BEGIN_CRITICAL_SECTION();

  /* #10 Get TimeMsSinceStart timer and add main function cycle time. */
  timeMsSinceStartTmp = Sd_GetTimeMsSinceStartOfPartitionDataDyn(SD_PARTITION_DATA_IDX, PartitionIdx);
  timeMsSinceStartTmp.Milliseconds += (uint16)SD_MAIN_FCT_CYCLE_TIME_MS;

  /* #20 Handle possible milliseconds overflow. */
  timeMsSinceStartTmp.Seconds += (uint32)timeMsSinceStartTmp.Milliseconds / 1000u;
  timeMsSinceStartTmp.Milliseconds = timeMsSinceStartTmp.Milliseconds % 1000u;

  Sd_SetTimeMsSinceStartOfPartitionDataDyn(SD_PARTITION_DATA_IDX, timeMsSinceStartTmp, PartitionIdx);                   /* SBSW_SD_PARTITION_DATA_CSL05 */
  SD_END_CRITICAL_SECTION();
}

/**********************************************************************************************************************
 *  Sd_Util_DisableRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_DisableRouting(
  Sd_SizeOfRoutingGroupType RoutingGroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if RoutingGroup is already disabled. */
  if(Sd_GetStatusOfRoutingGroupDyn(RoutingGroupIdx, PartitionIdx) != SD_ROUTINGGROUP_STATUS_DISABLED)
  {
    /* #100 Disable Routing for RoutingGroup if RoutingGroup is not already disabled. */
    retVal = SoAd_DisableRouting(Sd_GetRoutingGroup(RoutingGroupIdx, PartitionIdx));

    /* #101 Update internal status of RoutingGroup if Routing could be disabled. */
    if(retVal == E_OK)
    {
      Sd_SetStatusOfRoutingGroupDyn(RoutingGroupIdx, SD_ROUTINGGROUP_STATUS_DISABLED, PartitionIdx);                    /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Util_DisableSpecificRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_DisableSpecificRouting(
  Sd_SizeOfRoutingGroupType RoutingGroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set internal status of RoutingGroup to unknown because only a specific routing is disabled. */
  Sd_SetStatusOfRoutingGroupDyn(RoutingGroupIdx, SD_ROUTINGGROUP_STATUS_UNKNOWN, PartitionIdx);                         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Disable specific routing and return result of operation. */
  return SoAd_DisableSpecificRouting(Sd_GetRoutingGroup(RoutingGroupIdx, PartitionIdx), SoConId);
}

/**********************************************************************************************************************
 *  Sd_Util_EnableRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_EnableRouting(
  Sd_SizeOfRoutingGroupType RoutingGroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if RoutingGroup is already enabled. */
  if(Sd_GetStatusOfRoutingGroupDyn(RoutingGroupIdx, PartitionIdx) != SD_ROUTINGGROUP_STATUS_ENABLED)
  {
    /* #100 Enable Routing for RoutingGroup if RoutingGroup is not already enabled. */
    retVal = SoAd_EnableRouting(Sd_GetRoutingGroup(RoutingGroupIdx, PartitionIdx));

    /* #101 Update internal status of RoutingGroup if Routing could be enabled. */
    if(retVal == E_OK)
    {
      Sd_SetStatusOfRoutingGroupDyn(RoutingGroupIdx, SD_ROUTINGGROUP_STATUS_ENABLED, PartitionIdx);                     /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_Util_EnableSpecificRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_Util_EnableSpecificRouting(
  Sd_SizeOfRoutingGroupType RoutingGroupIdx,
  Sd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConIdType SoConId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set internal status of RoutingGroup to unknown because only a specific routing is enabled. */
  Sd_SetStatusOfRoutingGroupDyn(RoutingGroupIdx, SD_ROUTINGGROUP_STATUS_UNKNOWN, PartitionIdx);                         /* SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE */

  /* #20 Enable specific routing and return result of operation. */
  return SoAd_EnableSpecificRouting(Sd_GetRoutingGroup(RoutingGroupIdx, PartitionIdx), SoConId);
}

/**********************************************************************************************************************
 *  Sd_Util_SetAllStateMachineExecutionFlags()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_SetAllStateMachineExecutionFlags(
  Sd_SizeOfInstanceType InstanceIdx,
  Sd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Sd_SizeOfServerType serverIdx;
  Sd_SizeOfClientType clientIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set all state machine execution flags for the server services. */
  for(serverIdx = Sd_GetServerStartIdxOfInstance(InstanceIdx, PartitionIdx);
      serverIdx < Sd_GetServerEndIdxOfInstance(InstanceIdx, PartitionIdx);
      serverIdx++)
  {
    Sd_SetStateMachineExecutionRequestedOfServerDyn(serverIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
  }

  /* #20 Set all state machine execution flags for the client services. */
  for(clientIdx = Sd_GetClientStartIdxOfInstance(InstanceIdx, PartitionIdx);
      clientIdx < Sd_GetClientEndIdxOfInstance(InstanceIdx, PartitionIdx);
      clientIdx++)
  {
    Sd_SetStateMachineExecutionRequestedOfClientDyn(clientIdx, TRUE, PartitionIdx);                                     /* SBSW_SD_API_ARGUMENT_IDX_INDIRECTION */
  }
}

/**********************************************************************************************************************
 *  Sd_Util_Write32Bit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_Write32Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint32 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write next 4 bytes to the array. */
  BufferPtr[0u] = (uint8)((uint32_least)(Value) >> 24u);                                                                /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[1u] = (uint8)((uint32_least)(Value) >> 16u);                                                                /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[2u] = (uint8)((uint32_least)(Value) >>  8u);                                                                /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[3u] = (uint8)((uint32_least)(Value));                                                                       /* SBSW_SD_WRITE_PAYLOAD */
}

/**********************************************************************************************************************
 *  Sd_Util_Write24Bit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_Write24Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint32 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write next 3 bytes to the array. */
  BufferPtr[0u] = (uint8)((uint32_least)(Value) >> 16u);                                                                /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[1u] = (uint8)((uint32_least)(Value) >>  8u);                                                                /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[2u] = (uint8)((uint32_least)(Value));                                                                       /* SBSW_SD_WRITE_PAYLOAD */
}

/**********************************************************************************************************************
 *  Sd_Util_Write16Bit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_Write16Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint16 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write next 2 bytes to the array. */
  BufferPtr[0u] = (uint8)((uint16_least)(Value) >>  8u);                                                                /* SBSW_SD_WRITE_PAYLOAD */
  BufferPtr[1u] = (uint8)((uint16_least)(Value));                                                                       /* SBSW_SD_WRITE_PAYLOAD */
}

/**********************************************************************************************************************
 *  Sd_Util_Write8Bit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(void, SD_CODE) Sd_Util_Write8Bit(
  SD_P2VAR(uint8) BufferPtr,
  uint8 Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Write next byte to the array. */
  BufferPtr[0u] = (uint8)(Value);                                                                                       /* SBSW_SD_WRITE_PAYLOAD */
}

/**********************************************************************************************************************
 *  Sd_SoAd_GetLocalAddrWithNetmask()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_SoAd_GetLocalAddrWithNetmask(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) LocalAddrPtr,
  SD_P2VAR(uint8) NetmaskPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  Sd_SockAddrContainerType defaultRouter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request the local address from the SoAd module. */
  /* PRQA S 0310 5 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_GetLocalAddr(
    SoConId,
    (SD_P2VAR(SoAd_SockAddrType))LocalAddrPtr,
    NetmaskPtr,
    (SD_P2VAR(SoAd_SockAddrType))&defaultRouter);                                                                       /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */ /* SBSW_SD_SOCK_ADDR_CAST */

  /* #20 Check if the request was successful and the family type is known. */
  if((retVal != E_OK) ||
    ((LocalAddrPtr->family != SOAD_AF_INET) && (LocalAddrPtr->family != SOAD_AF_INET6)))
  {
    /* #200 Invalidate the family type and return E_NOT_OK because the request failed. */
    LocalAddrPtr->family = SOAD_AF_INVALID;                                                                             /* SBSW_SD_POINTER_WRITE */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_SoAd_GetLocalAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_SoAd_GetLocalAddr(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) LocalAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 netmask;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request the local address. */
  retVal = Sd_SoAd_GetLocalAddrWithNetmask(SoConId, LocalAddrPtr, &netmask);                                            /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_REFERENCE_OF_VARIABLE */

  return retVal;
}

/**********************************************************************************************************************
 *  Sd_SoAd_GetRemoteAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SD_LOCAL_INLINE FUNC(Std_ReturnType, SD_CODE) Sd_SoAd_GetRemoteAddr(
  SoAd_SoConIdType SoConId,
  SD_P2VAR(Sd_SockAddrContainerType) IpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the remote address from the SoAd module. */
  /* PRQA S 0310 3 */ /* MD_Sd_11.4_0310 */
  retVal = SoAd_GetRemoteAddr(
    SoConId,
    (SD_P2VAR(SoAd_SockAddrType))IpAddrPtr);                                                                            /* SBSW_SD_PASSING_API_POINTER */ /* SBSW_SD_SOCK_ADDR_CAST */

  /* #20 Check if the returned family type is known. */
  if((retVal != E_OK) ||
    ((IpAddrPtr->family != SOAD_AF_INET) && (IpAddrPtr->family != SOAD_AF_INET6)))
  {
    /* #200 Invalidate the family type and return E_NOT_OK because the request failed. */
    IpAddrPtr->family = SOAD_AF_INVALID;                                                                                /* SBSW_SD_POINTER_WRITE */
    retVal = E_NOT_OK;
  }

  return retVal;
}

#define SD_STOP_SEC_CODE
#include "Sd_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:

  MD_Sd_11.4_0310:
    Reason:     Pointer casts are standard proceeding in TcpIp address data handling. (Rule 0310, 3305)
    Risk:       None
    Prevention: Covered by code review.

  MD_Sd_11.5_0311:
    Reason:     Function requires variable input parameter instead of const input parameter. (Rule 0311)
    Risk:       Changes made inside the function are not realized
    Prevention: Covered by code review.

  MD_Sd_12.4_3415:
    Reason:     Function does not change any value and has therefore no side effects. (Rule 3415)
    Risk:       Changes made inside the function are not realized
    Prevention: Covered by code review.

  MD_Sd_Rule-2.2_2981:
    Reason:     Redundant initialization. The value of this object is never used before being modified.
    Risk:       None
    Prevention: No prevention necessary.

  MD_Sd_Rule-2.2_2986:
    Reason:     Redundant operation. The value of the result is always that of the right-hand operand.
    Risk:       None
    Prevention: No prevention necessary.

  MD_Sd_Dir-1.1_0314:
    Reason:     The memset functions from the vstdlib have a void pointer as a function parameter. (See MD_MSR_VStdLibCopy for copy functions.)
    Risk:       No risk, because the underlying uint8 pointer type is known.
    Prevention: No prevention necessary.

  MD_Sd_Dir-1.1_0314_copy:
    Reason:     The Sd_Copy function have a void pointer as a function parameter.
    Risk:       No risk, because the underlying uint8 pointer type is known.
    Prevention: No prevention necessary.

  MD_Sd_Dir-1.1_0316_copy:
    Reason:     The Sd_Copy function have a void pointer as a function parameter and the pointer is internally cast to uint8 for bytewise copy.
    Risk:       No risk, because the pointer length is known and passed to the function.
    Prevention: No prevention necessary.

  MD_Sd_Enum_4522_4424_4432_4332_4342_4393:
    Reason:     Enumerations are not used in a type safe way.
                This applies for the following rules:
                  Rule-10.1_4522: An expression of 'essentially enum' type is being used as the right-hand operand of this bitwise operator (|).
                  Rule-10.3_4424: An expression of 'essentially enum' type is being converted to unsigned type on assignment.
                  Rule-10.3_4432: An expression of 'essentially signed' type is being converted to enum type on assignment.
                  Rule-10.5_4332: An expression of 'essentially signed' type is being cast to enum type.
                  Rule-10.5_4342: An expression of 'essentially unsigned' type is being cast to enum type.
                  Rule-10.8_4393: A composite expression of 'essentially signed' type is being cast to a different type category.
    Risk:       The type safety of enumeration data types is not ensured, which may lead to unexpected behavior.
    Prevention: Covered by code review.

  MD_Sd_FalsePositive_2995_2996_2853:
    Reason:     Tooling detects errors that do not exist:
                Rule-2.2_2995:  The result of this logical operation is always 'true'.
                Rule-2.2_2996:  The result of this logical operation is always 'false'.
                Rule-3.1_2853:  Suspicious: Implicit conversion to a signed integer type of insufficient size.
    Risk:       Potential issue may be not detected.
    Prevention: Covered by code review.

  MD_Sd_BooleanValueRangeCheck:
      Reason:     The value range size of boolean may be greater than 2. Therefore, the value range is checked for
                  invalid values. In configurations with a value range of 2 causes this check "always true/false"
                  conditions and unreachable code:
                  Rule-2.12_2880: This code is unreachable.
                  Rule-2.2_2996:  The result of this logical operation is always 'false'.
                  Rule-14.3_2992: The value of this 'if' controlling expression is always 'false'.
      Risk:       None.
      Prevention: None.

  MD_Sd_STPTH_6010:
    Reason:     Path count too high, function is hard to understand.
    Risk:       Maintainability and testability might become too complex.
    Prevention: Covered by code review.

  MD_Sd_STCAL_6050:
    Reason:     Deeper nesting of functions increases call stack usage and runtime.
    Risk:       Understandability and testability might become too complex due to fan-out to many functions.
    Prevention: Covered by code review.

  MD_Sd_STPAR_6060:
    Reason:     Function handles specific task in the overall component behavior. This requires multiple input
                parameters.
    Risk:       Stack usage and runtime too high for target uC.
    Prevention: Not applicable.
*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_SD_CSL01_CSL05
    \DESCRIPTION      Access of partition dependent array element with index generated by ComStackLib.
    \COUNTERMEASURE   \N  Qualified use-case CSL01 of ComStackLib. The PartitionIdx has been derived in advance by a
                          qualified use-case of CSL05.

  \ID SBSW_SD_CSL02_CSL05
    \DESCRIPTION      Access of partition dependent array element with index checked against size of different array.
                      (Arrays Xyz[] and XyzDyn[] depend on ComStackLib size relation.)
    \COUNTERMEASURE   \N  Qualified use-case CSL02 of ComStackLib. The PartitionIdx has been derived in advance by a
                          qualified use-case of CSL05.

  \ID SBSW_SD_CSL03_CSL05
    \DESCRIPTION      Access of array element with index modelled by partition dependent indirection.
    \COUNTERMEASURE   \N  Qualified use-case CSL03 of ComStackLib. The PartitionIdx has been derived in advance by a
                          qualified use-case of CSL05.

  \ID SBSW_SD_GETVERSION_PARAM
    \DESCRIPTION      The Sd_GetVersionInfo writes to the 'versioninfo' pointer parameter. It is checked against NULL,
                      but this does not guarantee that the referenced memory is valid.
    \COUNTERMEASURE   \N  Pointers passed to public Sd APIs point to a valid memory range.
                          A general countermeasure is included in the safety manual.

  \ID SBSW_SD_PUBLIC_API_POINTER
    \DESCRIPTION      The function forwards the input pointer. It is checked against NULL_PTR,
                      but this does not guarantee that the referenced memory is valid.
    \COUNTERMEASURE   \N  Pointers passed to public Sd APIs point to a valid memory range.
                          A general countermeasure is included in the safety manual.

  \ID SBSW_SD_PASSING_API_POINTER
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  Pointer was passed as function argument and is used to call sub-function. The validity of the
                          pointer is ensured by the caller. If not otherwise specified, each pointer has to point to
                          valid data != NULL_PTR.

  \ID SBSW_SD_PASSED_API_POINTER_NOT_NULL
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \R  The pointer is checked against NULL_PTR.

  \ID SBSW_SD_FUNCTION_CALL_NULL_PTR
    \DESCRIPTION      NULL_PTR is passed as function argument.
    \COUNTERMEASURE   \N  NULL_PTR is an allowed value to identify special use-case.
                          The pointer value is checked against NULL_PTR within the called function.

  \ID SBSW_SD_REFERENCE_OF_VARIABLE
    \DESCRIPTION      Pointer to variable is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing a variable respectively a struct member of a variable
                          by the address operator.

  \ID SBSW_SD_REFERENCE_OF_POINTER_ELEMENT
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing a member of a struct passed as reference function
                          argument. The validity of the function argument is ensured by the caller.

  \ID SBSW_SD_PDU_INFO_PTR
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  The PduInfoPtr is a pointer which is passed via the public API Sd_RxIndication().
                          It is checked against NULL_PTR and saved in the Sd_RxMessageInfoType structure.
                          The pointed structure contains a member SduDataPtr which is also assumed to be valid.
                          Pointers passed to public Sd APIs point to a valid memory range.
                          A general countermeasure is included in the safety manual.

  \ID SBSW_SD_RX_ADDRESS_STORAGE_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The AddressStorageIdx index is determined during reception of a message within
                          Sd_Rx_SaveAddr(). The index is obtained by qualified use-case CSL01 of ComStackLib within
                          Sd_Rx_Process().The PartitionIdx has been derived in advance by a qualified use-case
                          CSL05.

  \ID SBSW_SD_SEND_ENTRY_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The index is determined by the Sd_Util_GetEmptySendEntry() function.
                          The valid range of the index is ensured by this function and defined at the function
                          declaration. The PartitionIdx has been derived in advance by a qualified use-case
                          CSL05.

  \ID SBSW_SD_SEND_ENTRY_IDX_INDIRECTION
    \DESCRIPTION      Access of array element with index modelled by indirection.
    \COUNTERMEASURE   \N  The index is determined by an indirection from an element of the send entry list. The
                          validity of the indirection is ensured by the return value of the
                          Sd_Util_GetEmptySendEntry() function. The PartitionIdx has been derived in advance by a
                          qualified use-case CSL05.

  \ID SBSW_SD_CONFIG_OPTION_PTR_ARRAY_SIZE
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The ConfigOptionPtrArray array has a size of (Sd_GetMaxNrOptionsPerEntry() + 1).
                          The write access of element with index is checked with qualified use-case CSL01 of
                          ComStackLib.

  \ID SBSW_SD_FIXED_SIZE_OF_CONFIGOPTIONPTRARRAY
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The ConfigOptionPtrArrayPtr passed as function argument points to an array of fixed size 3.
                          Within the function Sd_Rx_FillConfigOptionPtrArray(), it is ensures that maximally the
                          indexes 0 and 1 of the array are modified inside the function.

  \ID SBSW_SD_ATTACHED_OPTIONS_PTR
    \DESCRIPTION      Pointer is passed as function argument.
    \COUNTERMEASURE   \N  The ConfigOptionPtrArrayPtr element of the Sd_AttachedOptionsType structure points to a local
                          array of size (Sd_GetMaxNrOptionsPerEntry() + 1).
                          The assignment is done directly after creation of the structure.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element which is obtained by qualified
                          use-case CSL01 of ComStackLib. The PartitionIdx has been derived in advance by a
                          qualified use-case CSL05.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_SAME_SIZE
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element which is obtained by same size Relation.
                          (Qualified use-case CSL02 of ComStackLib.) The PartitionIdx has been derived in advance by a
                          qualified use-case CSL05.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element with index given as function argument.
                          The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_SAME_SIZE
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element with index.
                          The index is obtained by same size relation of function argument.
                          (Qualified use-case CSL02 of ComStackLib).
                          The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function. The PartitionIdx has been derived in advance by a
                          qualified use-case CSL05.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is obtained by referencing an array element with index.
                          The index is obtained by indirection of function argument.
                          (Qualified use-case CSL03 of ComStackLib).
                          The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function. The PartitionIdx has been derived in advance by a
                          qualified use-case CSL05.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_OUTER_INDEX_INDIRECTION_SAME_SIZE
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element by index.
                          The index is obtained by indirection and same size relation of function argument.
                          (Qualified use-cases CSL03 and CSL02 of ComStackLib). The valid range of the function argument
                          is defined at the function declaration and has to be ensured by the calling function.
                          The PartitionIdx has been derived in advance by a qualified use-case CSL05.

  \ID SBSW_SD_API_ARGUMENT_IDX
    \DESCRIPTION      Access of array element with index given as function argument is not checked for validity.
    \COUNTERMEASURE   \N  The valid range of the function argument is defined at the function declaration and has to be
                          ensured by the calling function. The PartitionIdx has been derived in advance by a
                          qualified use-case CSL05.

  \ID SBSW_SD_API_ARGUMENT_IDX_SAME_SIZE
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  Index is obtained by same size relation (qualified use-case CSL02 of ComStackLib) of function
                          argument. The valid range of the function argument is defined at the function declaration and
                          has to be ensured by the calling function. The PartitionIdx has been derived in advance by a
                          qualified use-case CSL05.

  \ID SBSW_SD_API_ARGUMENT_IDX_INDIRECTION
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  Index is obtained by indirection (qualified use-case CSL03 of ComStackLib) of function
                          argument. The valid range of the function argument is defined at the function declaration and
                          has to be ensured by the calling function. The PartitionIdx has been derived in advance by a
                          qualified use-case CSL05.
                          If the indirection is not valid in case of an optional 0:1 indirection (value of index is SD_NO_*),
                          the index must not be used.

  \ID SBSW_SD_API_ARGUMENT_IDX_INDIRECTION_SAME_SIZE
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  Index is obtained by indirection (qualified use-case CSL03 of ComStackLib) and same size
                          relation (qualified use-case CSL02 of ComStackLib) of function argument. The valid range of
                          the function argument is either defined at the function declaration and has to be ensured by the
                          calling function or is checked within the function. The PartitionIdx has been derived in
                          advance by a qualified use-case CSL05.

  \ID SBSW_SD_FUNCTION_PTR_CALL
    \DESCRIPTION      Calling a function pointer without checking the reference.
    \COUNTERMEASURE   \R  It is checked, that the corresponding service has a function pointer configured.
                          The index is obtained by indirection of function argument.
                          (Qualified use-case CSL03 of ComStackLib). The valid range of the function argument is defined
                          at the function declaration and has to be ensured by the calling function.The PartitionIdx has
                          been derived in advance by a qualified use-case CSL05.

  \ID SBSW_SD_FUNCTION_PTR_CALL_STATIC
    \DESCRIPTION      Calling a function pointer without checking the reference.
    \COUNTERMEASURE   \N  The function pointer is passed as function argument. Its correctness has to be ensured by the
                          calling function.

  \ID SBSW_SD_PARTITION_DATA_CSL05
    \DESCRIPTION      Access of partition dependent array element with constant index "0".
    \COUNTERMEASURE   \N  PartitionDataDyn array exists once per partition, but has only one element. It is therefore
                          valid to access this array with a constant index of "0". The PartitionIdx has been derived
                          in advance by a qualified use-case of CSL05.

  \ID SBSW_SD_REFERENCE_OF_ARRAY_ELEMENT_PARTITION_DATA
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element which of the PartitionDataDyn array.
                          This array exists once per partition, but has only one element. It is therefore valid to
                          access this array with a constant index of "0". The PartitionIdx has been derived in
                          advance by a qualified use-case of CSL05.

  \ID SBSW_SD_IP_ADDR_SIZE
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  IPv6 addresses are saved in an array with fixed size of 4.
                          The used indices are either constant and smaller as the array size,
                          or the index is determined by subtracting 1 from 'wordsNetmask',
                          which is ensured to be in the range 1-4 by design and verified by review.

  \ID SBSW_SD_RXEVENT_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  All RxEvents are saved in a ring buffer in the Sd_RxEvent[] array.
                          The value RxEventEndIdxOfInstanceDyn is an index into a RAM data structure which is used to
                          implement a ring buffer. The value is initialized before used and checked to stay in correct
                          range after any update. The check for correctness is done via modelled relation according to
                          CSL03. The PartitionIdx has been derived in advance by a qualified use-case of CSL05.

  \ID SBSW_SD_RXEVENT_MEMBER
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The index is taken from ring buffer element. Ring buffer implementation assures that only
                          elements are read which have been written with correct values before. The index to read from
                          the ring buffer is initialized before usage and checked to stay in correct range with any
                          change, similar to the write index as stated in SBSW_SD_RXEVENT_IDX. Correctness of the index
                          values written to the ring buffer element is assured by CSL01, CSL02 or CSL03. All parts of a
                          ring buffer entry are written at once. The PartitionIdx has been derived in advance by a
                          qualified use-case of CSL05.

  \ID SBSW_SD_REFERENCE_RXEVENT_MEMBER
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing an array element by index.
                          The index is obtained by using a member of the RxEvent structure which is qualified by
                          SBSW_SD_RXEVENT_MEMBER. The PartitionIdx has been derived in advance by a qualified use-case
                          of CSL05.

  \ID SBSW_SD_REFERENCE_MESSAGE_SIZE_WITH_ENTRIES
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  The length of the partition specific payload array is enforced to be >=48Bytes by the
                          Generator, which is sufficient for the entire header (SD_HEADER_LENGTH), the maximum
                          length of all SD entries which shall be serialized
                          (maxNumberOfEntriesToSerialize * SD_ENTRY_SIZE) and the corresponding length fields
                          (SD_HEADER_LENGTHOFENTRIESARRAY_SIZE + SD_HEADER_LENGTHOFOPTIONSARRAY_SIZE).
                          maxNumberOfEntriesToSerialize is determined at the beginning of Sd_Tx_SerializeMessage()
                          function using the length of the payload array. This ensures that the access is always valid.
                          The PartitionIdx has been derived in advance by a qualified use-case of CSL05.

  \ID SBSW_SD_TX_SERIALIZE_ENTRY
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  PayloadEntryIdx + SD_ENTRY_SIZE is smaller than the length of the partition specific payload
                          array. This must be checked by the caller of Sd_Tx_SerializeEntryAndOptions(). The length
                          check ensures that the access is always valid. The PartitionIdx has been derived in advance
                          by a qualified use-case of CSL05.

  \ID SBSW_SD_TX_PAYLOAD_ACCESS_OPTION_LEN_FIELD
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  The partition specific payload array is accessed to retrieve the option length field.
                          tmpPayloadIdx + SD_OPTION_LENGTH_SIZE is checked to be smaller than the length of the payload
                          array. The length check ensures that the access is always valid. The PartitionIdx has been
                          derived in advance by a qualified use-case of CSL05.

  \ID SBSW_SD_TX_SERIALIZE_OPTION
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  The partition specific payload array is accessed to retrieve the serialize option fields.
                          It is verified for each option that it will not exceed the payload array by
                          checking against the length of array. The length check ensures that the access is always valid.
                          The PartitionIdx has been derived in advance by a qualified use-case of CSL05.

  \ID SBSW_SD_SOCK_ADDR_SIZE
    \DESCRIPTION      Pointer to struct element is passed as function argument.
    \COUNTERMEASURE   \N  Pointer is generated by referencing a member of a struct passed as function argument. The
                          validity of the function argument is ensured by the caller.
                          The size of the pointed struct members ensure the following requirements:
                          - sizeof(data) >= max(SD_OPTION_IPV4_ADDRESS_SIZE, SD_OPTION_IPV6_ADDRESS_SIZE)
                          - sizeof(port) >= max(SD_OPTION_IPV4_PORTNUMBER_SIZE, SD_OPTION_IPV6_PORTNUMBER_SIZE)

  \ID SBSW_SD_RX_MESSAGE_SIZE
    \DESCRIPTION      Pointer to array element is passed as function argument.
    \COUNTERMEASURE   \R  The size of each received message is checked to be valid by Sd_Rx_ProcessHeader() function.
                          These checks ensure that the given entries and options array lengths are valid.
                          Hence, the payload pointer point to valid bytes of the received message.

  \ID SBSW_SD_WRITE_PAYLOAD
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The Sd_Util_Write<8|16|24|32>Bit() functions access the element given by the pointer as well
                          as the succeeding element(s). The validity of the pointer and the minimal amount of valid
                          remaining elements is defined at the function description and has to be satisfied by the
                          caller.

  \ID SBSW_SD_TX_ADDRESS_STORAGE_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  Each valid send entry references its destination address (AddressStorageIdx).
                          This index is always set to an existing value if a send entry is saved
                          (Sd_Util_Add<Xyz>EntryToList()). The PartitionIdx has been derived in advance by a qualified
                          use-case of CSL05.

  \ID SBSW_SD_TRIGGER_TRANSMIT_IDX
    \DESCRIPTION      Access of array element with index is not checked for validity.
    \COUNTERMEASURE   \N  The list of trigger transmit requests is managed in a ring-buffer with start and end indices
                          and a full-flag. The list is modified by Sd_TriggerTransmit_<Xyz>() functions. There it is
                          ensured that the corresponding indices are always valid. The PartitionIdx has been derived
                          in advance by a qualified use-case of CSL05.

  \ID SBSW_SD_SOCK_ADDR_CAST
    \DESCRIPTION      Type cast of pointer may change the amount of memory the called function writes to.
    \COUNTERMEASURE   \N  The SockAddr structures for IPv4 and IPv6 addresses have a different structure and size.
                          In order to allow passing both address types via the same API to functions, the pointer to the
                          structure is type casted to a generic type (*_SockAddrType) which contains only the domain
                          information of the address and results in a typecast to a smaller structure.
                          The called function has to interpret the domain information and will type cast to the
                          corresponding type.
                          The size of the casted structure (Sd_SockAddrContainerType) is sufficient to store both
                          address types.

  \ID SBSW_SD_IPBASE_FILL
    \DESCRIPTION      Type cast of pointer may change the amount of memory the called function writes to.
    \COUNTERMEASURE   \N  The size of the corresponding element as well as the offset are given as additional function
                          arguments. Thereby the offset is set to 0 and the size calculated by sizeof() operator.

  \ID SBSW_SD_CAST_TO_VAR
    \DESCRIPTION      Pointer to const data is casted to pointer to variable data.
    \COUNTERMEASURE   \N  The AUTOSAR 4.2.1 API SoAd_SetUniqueRemoteAddr() is defined with var "RemoteAddrPtr" pointer,
                          although the pointed data will not be modified within this function.

  \ID SBSW_SD_POINTER_WRITE
    \DESCRIPTION      Write access to pointer passed as function parameter.
    \COUNTERMEASURE   \N The validity of the memory location pointed to must be ensured by the caller.
                         This includes struct members in case the pointer points to a struct type.
                         If not explicitely stated in the function's description, the pointer must not be NULL.

  \ID SBSW_SD_POINTER_WRITE_LOCAL
    \DESCRIPTION      Write access to local pointer.
    \COUNTERMEASURE   \N Local pointer is initialized to null and conditionally set to a valid memory location in the same function.
                         Before writing to the location pointed to, the pointer is checked that it is not null.

  \ID SBSW_SD_VARIABLE_POINTER_FORWARD
    \DESCRIPTION      The function forwards pointer to a local/global variable which could be used to write to memory
                      location of the variable.
    \COUNTERMEASURE   \N Local/global variables are located in valid memory locations. Therefore, address of variable
                         can be passed as function parameter.

  \ID SBSW_SD_ACCESS_LOOP
    \DESCRIPTION Linear memory access using a for loop.
    \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. The for loop ensures that
                        the loop index is always smaller than nCnt. Therefore only valid memory is accessed.

SBSW_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

  \ID COV_SD_COMPATIBILITY
    \ACCEPT   TX
    \ACCEPT   XF
    \REASON   [COV_MSR_COMPATIBILITY]

  \ID COV_SD_VAR_DEPENDENT_SWITCH
    \ACCEPT TX
    \REASON The nested switch depends on a previous switch.

  \ID COV_SD_MISRA
    \ACCEPT XX
    \REASON [MSR_COV_MISRA]

  \ID COV_SD_INV_STATE
    \ACCEPT XX
    \REASON [COV_MSR_INV_STATE]

  \ID COV_SD_SAFE_BSW
    \ACCEPT TX
    \REASON In SafeBSW use-case SD_DEV_ERROR_DETECT has to be configured to STD_ON.

  \ID COV_SD_QM_FEATURE_MULTI_PARTITION
    \ACCEPT XF
    \REASON This code is part of a QM-Feature and has to be deactivated for SafeBSW.

END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: Sd.c
 *********************************************************************************************************************/
