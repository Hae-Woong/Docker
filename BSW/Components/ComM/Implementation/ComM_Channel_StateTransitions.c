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
/*!         \file  ComM_Channel_StateTransitions.c
 *          \unit     Channel_StateTransitions
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the Channel_StateTransitions unit.
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

#define CCL_ASR_COMM_CHANNEL_STATETRANSITIONS_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM.h"
#include "ComM_Channel_StateTransitions.h"
#include "ComM_ModeInhibition.h"
#include "ComM_Private_Nm.h"
#include "ComM_ErrorReporting.h"
#include "ComM_Util.h"
#include "SchM_ComM.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* type definition of the ComM transition function call table */
typedef P2FUNC(void, COMM_CODE, ComM_TransitionFunctionType) (NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES OF STATE TRANSITION FUNCTIONS (TF)
 *********************************************************************************************************************/

#define COMM_START_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
  ComM_TF_NoCom_NetReq()
 *********************************************************************************************************************/
/*! \brief      Performs the state transition from No Communication to Network Requested for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-47933, SPEC-5081, SPEC-47933, SPEC-5456, SPEC-5414
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_NoCom_NetReq( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_TF_NoCom_FullReadySleep()
 *********************************************************************************************************************/
/*! \brief      Performs the state transition from No Communication to Ready Sleep for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5081, SPEC-5361, SPEC-5487, SPEC-5488
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_NoCom_FullReadySleep( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_TF_Full_SiCom()
 *********************************************************************************************************************/
/*! \brief      Performs the state transition from Full to Silent Communication for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5097
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_Full_SiCom( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_TF_ReadyS_NetReq()
 *********************************************************************************************************************/
/*! \brief      Performs the state transition from Ready Sleep to Network Requested for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5478
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_ReadyS_NetReq( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_TF_SiCom_NoCom()
 *********************************************************************************************************************/
/*! \brief      Performs the state transition from Full or Silent to No Communication for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      CREQ-116030
 *  \trace      SPEC-5083, SPEC-5370
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_SiCom_NoCom( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_TF_NetReq_ReadyS()
 *********************************************************************************************************************/
/*! \brief      Performs the state transition from Network Requested to Ready Sleep for the given channel
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-5477, SPEC-5086
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_NetReq_ReadyS( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_TF_No_Action()
 *********************************************************************************************************************/
/*! \brief      Special state transition function for transitions that do not require any actions, needed as a placeholder
 *              in the state transition table.
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_No_Action( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_TF_No_Transition()
 *********************************************************************************************************************/
/*! \brief      Special state transition function for unsupported transitions to report development errors
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_No_Transition( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );


/**********************************************************************************************************************
  ComM_ForwardBusSMModeRequest()
 *********************************************************************************************************************/
/*! \brief      Notifies the channel's new mode to its BusSM.
 *  \details    -
 *  \param[in]  Channel                 local channel handle
 *  \param[in]  ComMode                 Communication mode which shall be forwarded to the BusSM
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_ForwardBusSMModeRequest( NetworkHandleType Channel, VAR( ComM_ModeType, AUTOMATIC ) ComMode, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  ROM DATA
 *********************************************************************************************************************/

#define COMM_START_SEC_CONST_8
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/*! ComM transition table [requested][active], the element COMM_MAX_NUMBER_OF_STATES means that no transition is defined for the active/requested combination. */
/* PRQA S 3218 1 */ /* MD_ComM_3218 */
COMM_LOCAL CONST( ComM_StateType, COMM_CONST ) ComM_TransitionTable[COMM_MAX_NUMBER_OF_STATES][COMM_MAX_NUMBER_OF_STATES] =    /* [row] [column] */
{/* COMM_NO_COM_NO_PENDING_REQUEST |COMM_NO_COM_REQUEST_PENDING     |COMM_FULL_COM_NETWORK_REQUESTED |COMM_FULL_COM_READY_SLEEP       |COMM_SILENT_COM                   requested mode: */
  {
    COMM_MAX_NUMBER_OF_STATES,      COMM_NO_COM_NO_PENDING_REQUEST,  COMM_FULL_COM_READY_SLEEP,       COMM_SILENT_COM,                 COMM_NO_COM_NO_PENDING_REQUEST  /* COMM_NO_COM_NO_PENDING_REQUEST */
  },
  {
    COMM_NO_COM_REQUEST_PENDING,    COMM_MAX_NUMBER_OF_STATES,       COMM_MAX_NUMBER_OF_STATES,       COMM_MAX_NUMBER_OF_STATES,       COMM_MAX_NUMBER_OF_STATES       /* COMM_NO_COM_REQUEST_PENDING */
  },
  {
    COMM_NO_COM_REQUEST_PENDING,    COMM_FULL_COM_NETWORK_REQUESTED, COMM_MAX_NUMBER_OF_STATES,       COMM_FULL_COM_NETWORK_REQUESTED, COMM_FULL_COM_NETWORK_REQUESTED /* COMM_FULL_COM_NETWORK_REQUESTED */
  },
  {
    COMM_NO_COM_REQUEST_PENDING,    COMM_FULL_COM_READY_SLEEP,       COMM_FULL_COM_READY_SLEEP,       COMM_MAX_NUMBER_OF_STATES,       COMM_FULL_COM_READY_SLEEP       /* COMM_FULL_COM_READY_SLEEP */
  },
  {
    COMM_MAX_NUMBER_OF_STATES,      COMM_MAX_NUMBER_OF_STATES,       COMM_FULL_COM_READY_SLEEP,       COMM_SILENT_COM,                 COMM_MAX_NUMBER_OF_STATES       /* COMM_SILENT_COM */
  }
};

#define COMM_STOP_SEC_CONST_8
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"


#define COMM_START_SEC_CONST_UNSPECIFIED
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/*! ComM transition action table [requested][active], the element ComM_TF_No_Action means that no transition is defined for the active/requested combination. */
/* PRQA S 3218 1 */ /* MD_ComM_3218 */
COMM_LOCAL CONST( ComM_TransitionFunctionType, COMM_CONST ) ComM_TransitionFctTable[COMM_MAX_NUMBER_OF_STATES][COMM_MAX_NUMBER_OF_STATES] =
{/* COMM_NO_COM_NO_PENDING_REQUEST |COMM_NO_COM_REQUEST_PENDING  |COMM_FULL_COM_NETWORK_REQUESTED |COMM_FULL_COM_READY_SLEEP |COMM_SILENT_COM                  requested mode: */
  {
    ComM_TF_No_Transition,          ComM_TF_No_Action,            ComM_TF_NetReq_ReadyS,           ComM_TF_Full_SiCom,        ComM_TF_SiCom_NoCom           /* COMM_NO_COM_NO_PENDING_REQUEST */
  },
  {
    ComM_TF_No_Action,              ComM_TF_No_Transition,        ComM_TF_No_Transition,           ComM_TF_No_Transition,     ComM_TF_No_Transition         /* COMM_NO_COM_REQUEST_PENDING */
  },
  {
    ComM_TF_No_Action,              ComM_TF_NoCom_NetReq,         ComM_TF_No_Transition,           ComM_TF_ReadyS_NetReq,     ComM_TF_NoCom_NetReq          /* COMM_FULL_COM_NETWORK_REQUESTED */
  },
  {
    ComM_TF_No_Action,              ComM_TF_NoCom_FullReadySleep, ComM_TF_NetReq_ReadyS,           ComM_TF_No_Transition,     ComM_TF_NoCom_FullReadySleep  /* COMM_FULL_COM_READY_SLEEP */
  },
  {
    ComM_TF_No_Transition,          ComM_TF_No_Transition,        ComM_TF_NetReq_ReadyS,           ComM_TF_Full_SiCom,        ComM_TF_No_Transition         /* COMM_SILENT_COM */
  }
};

#define COMM_STOP_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"


/*********************************************************************************************************************
 *  ComM_ForwardBusSMModeRequest()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_ForwardBusSMModeRequest( NetworkHandleType Channel, VAR( ComM_ModeType, AUTOMATIC ) ComMode, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
#if (COMM_MULTIPARTITION == STD_ON)
  /* #10 In case of MultiPartition, if the channel is assigned to a satellite partition, set the sync counter to be processed in the next ComM_MainFunction_Satellite(). */
  if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != partitionIdx )
  {
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
    /* Channel is not assigned to current partition: Transition function is called from master main function, channel is not assigned to master partition. */
    ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX );
    ComM_SetForwardingBusSmMode( Channel, ComMode, COMM_MASTER_PARTITION_IDX );
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
  }
  else
#endif
  {
    /* #20 In single partition use case, forward the new mode directly to the BusSM. */
    /* Channel is assigned to current partition: Transition function is called either from master main function (-> current channel is assigned to master partition)
     * or from interrupt context on satellite partition (-> current channel is assigned to current satellite partition). */
    if ( ComM_GetRequestBusSMModeApiOfChannel( Channel )(Channel, ComMode) != E_OK )
    {
      ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_MAINFUNCTION, COMM_E_ERROR_IN_PROV_SERVICE );
    }
  }

  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_TF_NoCom_NetReq()
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
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_NoCom_NetReq( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  if ( ComM_HasKeepAwakeChannels() )
  {
    if ( partitionIdx < ComM_GetSizeOfPartitionIdentifiers() )
    {
      switch ( ComM_GetNmTypeOfChannel( Channel ) )
      {
        /* #10 Nm variant NONE: Once communication requested, ComM shall not trigger shutdown on the channel. */
        case COMM_NONE_NMTYPEOFCHANNEL:
          ComM_SetBusComModeReq( Channel, COMM_FULL_COM_NETWORK_REQUESTED, partitionIdx );

          /* For channels with NmType NONE, this transition function can only be called from the mater partition.
           * No further check for the partiton index and no sync counter increase needed here. */
        break;

        /* #20 Nm variant LIGHT: Cancel the LIGHT SILENT timer (ComM_NmLightTimer variable is re-used). */
        case COMM_LIGHT_NMTYPEOFCHANNEL:
          if ( ComM_HasNmLightSilentDurationOfChannel() )
          {
            ComM_SetNmLightTimer( Channel, 0, partitionIdx );
          }

          /* #30 Nm variant LIGHT: Start the timer for ComMTMinFullComModeDuration. */
          ComM_SetMinFullComModeTimer( Channel, ComM_GetMinFullComTimeOfChannel( Channel ), partitionIdx );
        break;

          /* #40 Nm variant FULL: Notify Nm about network request. */
        case COMM_FULL_NMTYPEOFCHANNEL:

          ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), partitionIdx ); /* ESCAN00106564 */
          /* #50 Nm variant FULL: Start the timer for ComMTMinFullComModeDuration. */
          /* ESCAN00081330 */
          if ( ComM_HasMinFullComTimeOfChannel() )
          {
#if ( COMM_MULTIPARTITION == STD_ON )
            if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
            {
              /* Increment the sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
              ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
              ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

              /* Leave the critical section. It was entered previously in ComM_Nm_NetworkMode(). */
              SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();

              /* Increment the sync counter for ComM_MinFullComModeTimer by 1 (relative to the corresponding Master-Counter) */
              ComM_SetMinFullComModeTimerSyncCounter( Channel, (uint8)(ComM_GetMinFullComModeTimerSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
            }
            else
#endif
            {
              ComM_SetMinFullComModeTimer( Channel, ComM_GetMinFullComTimeOfChannel( Channel ), partitionIdx );
            }
          }

#if (COMM_MULTIPARTITION == STD_ON)
          if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != partitionIdx )
          {
            ComM_SetForwardNmNetworkRequestSyncCounter( Channel, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX );
          }
          else
#endif
          {
            (void)ComM_Nm_ServiceHandler( NM_NETWORKREQUEST, Channel, COMM_PNC_ID_NOT_USED );
          }
        break;

        default: /* COV_COMM_MISRA */
          break;
      }

      /* #60 Request COMM_FULL_COMMUNICATION from the state manager. */
      ComM_ForwardBusSMModeRequest( Channel, COMM_FULL_COMMUNICATION, partitionIdx );
    }
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_TF_NoCom_FullReadySleep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_NoCom_FullReadySleep( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* #10 If Nm is supported on the channel, request FULL_COM from BusSM and notify Nm about passive start-up. */
  if ( ComM_IsNmSupportOfChannel( Channel ) )
  {
    ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), partitionIdx );

#if ( COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

      /* Leave critical section. It was entered previously in ComM_Nm_NetworkMode(). */
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
#endif

    ComM_ForwardBusSMModeRequest( Channel, COMM_FULL_COMMUNICATION, partitionIdx );

    /* ComM shall call Nm_PassiveStartUp() when entering Ready Sleep for robustness reasons and ignore E_NOT_OK return value ESCAN00068132 */
#if (COMM_MULTIPARTITION == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != partitionIdx )
    {
      ComM_SetForwardNmPassiveStartUpSyncCounter( Channel, (uint8)(ComM_GetForwardNmPassiveStartUpSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), COMM_MASTER_PARTITION_IDX );
    }
    else
#endif
    {
      (void)ComM_Nm_ServiceHandler( NM_PASSIVESTARTUP, Channel, COMM_PNC_ID_NOT_USED );
    }
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

/* Note: the function cannot be called for channels with Nm Variant LIGHT because Minimum Full Com Duration
 * always exists for LIGHT and this results that channels enters Network Requested state and not Ready Sleep. */
}

/*********************************************************************************************************************
 *  ComM_TF_Full_SiCom()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_Full_SiCom( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  if ( ComM_IsSilentSupportOfChannel( Channel ) )
  {
    if ( ComM_GetNmLightSilentDurationOfChannel( Channel ) > 0x00u )
    {
      /* #10 If Nm Variant is LIGHT and the LIGHT SILENT timer is configured, start the timer. */
      ComM_SetNmLightTimer( Channel, ComM_GetNmLightSilentDurationOfChannel( Channel ), partitionIdx );
    }

    /* #20 Request COMM_SILENT_COMMUNICATION from BusSM. Silent mode is only available on CAN (except J1939Nm) or ETH with Nm Variants FULL or PASSIVE (ESCAN00069043) or LIGHT SILENT timer.
     *     Note: EthSM will ignore the mode request, but ComM shall trigger it for the sake of consistency because UdpNm indicates Prepare Bus Sleep Mode. */
#if (COMM_MULTIPARTITION == STD_ON)
    if ( (ComM_HasNmLightSilentDurationOfChannel()) && (ComM_GetPartitionConfigIdxOfChannel( Channel ) != partitionIdx) )
    {
      ComM_ForwardBusSMModeRequest( Channel, COMM_SILENT_COMMUNICATION, partitionIdx );
    }
    else
#endif
    {
      ComM_ForwardBusSMModeRequest( Channel, COMM_SILENT_COMMUNICATION, partitionIdx );
    }
  }

  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_TF_ReadyS_NetReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_ReadyS_NetReq( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{

  /* #10 If Nm Type is LIGHT, cancel the light timer. */
  if ( ComM_HasNmLightDurationOfChannel() )
  {
    ComM_SetNmLightTimer( Channel, 0u, partitionIdx );
  }

  /* #20 If Nm Type is FULL, notify Nm about network requested. */
  if ( ComM_GetNmTypeOfChannel( Channel ) == COMM_FULL_NMTYPEOFCHANNEL )
  {
#if (COMM_MULTIPARTITION == STD_ON)
    if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != partitionIdx )
    {
      ComM_SetForwardNmNetworkRequestSyncCounter( Channel, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), partitionIdx );
    }
    else
#endif
    {
      (void)ComM_Nm_ServiceHandler( NM_NETWORKREQUEST, Channel, COMM_PNC_ID_NOT_USED );
    }
  }

  /* #30 If Nm Type is LINSLAVE, notify LinSM and request FULL_COMMUNICATION. */
  if ( ComM_GetNmTypeOfChannel( Channel ) == COMM_LINSLAVE_NMTYPEOFCHANNEL )
  {
    ComM_ForwardBusSMModeRequest( Channel, COMM_FULL_COMMUNICATION, partitionIdx );
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#if (COMM_MULTIPARTITION == STD_ON)
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
}

/*********************************************************************************************************************
 *  ComM_TF_SiCom_NoCom()
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
 /* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_SiCom_NoCom( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set channel mode to NO_COM before requesting NO_COM from the BusSM to ensure accepting bus wake-ups. */
  if ( partitionIdx < ComM_GetSizeOfPartitionIdentifiers() )
  {
    ComM_SetActiveComMode( Channel, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx ); /* ESCAN00091475 */

#if (COMM_MULTIPARTITION == STD_ON)
    if ( ComM_IsNmSupportOfChannel( Channel ) )
    {
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        /* Increment the sync counter for ComM_ActiveComMode by 1 (relative to the corresponding Master-Counter) */
        ComM_SetActiveComModeSyncCounter( Channel, (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

        /* Leave the critical section. It was entered previously in BusSleepMode indication from Nm. */
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
      }
    }
#endif

    switch ( ComM_GetNmTypeOfChannel( Channel ) )
    {
      /* #20 If the channel is of Nm variant LINSLAVE, set Bus Sleep Mode indicated Flag to FALSE. The Flag is handled in the next ComM main function. */
    case COMM_LINSLAVE_NMTYPEOFCHANNEL:
      ComM_SetBusSleepModeIndicated( Channel, FALSE, partitionIdx );
      break;
      /* #30 If the channel is of Nm variant FULL, PASSIVE or LIGHT, request NO_COM from BusSM. */
    case COMM_FULL_NMTYPEOFCHANNEL:
    case COMM_PASSIVE_NMTYPEOFCHANNEL:
    case COMM_LIGHT_NMTYPEOFCHANNEL:
      ComM_ForwardBusSMModeRequest( Channel, COMM_NO_COMMUNICATION, partitionIdx );
      break;
    default: /* COV_COMM_MISRA */
      break;
    }

    /* #40 If the channel is limited to NO_COM, clear all FULL_COM requests for channel and PNC users. 
           In case of MultiPartition, update the data from the satellite partitions to prevent from having unexpected wakeUps.*/
    if ( ComM_NoComLimitationActive( Channel ) == TRUE )
    {
      ComM_UserByteMaskStartIdxOfUserType userByteMaskIndex;
      ComM_UserIterType user;
      ComM_UserReqFullComIdxOfUserByteMaskType userReqFullComIdx;
#if (COMM_PNC_SUPPORT == STD_ON)
      ComM_UserReqPncFullComEndIdxOfPncPbType userReqPncFullComIndex;
      ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;
#endif
#if (COMM_MULTIPARTITION == STD_ON)
      ComM_PCPartitionConfigIdxOfPartitionIdentifiersType userPartitionIdx;
#endif

      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

      for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
      {
#if (COMM_MULTIPARTITION == STD_ON)
        userPartitionIdx = (ComM_PCPartitionConfigIdxOfPartitionIdentifiersType)ComM_GetPartitionConfigIdxOfUser( user );
#endif
        for ( userByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); userByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); userByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          if ( ComM_GetChannelOfUserByteMask( userByteMaskIndex ) == Channel )
          {
            userReqFullComIdx = ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex );
#if (COMM_MULTIPARTITION == STD_ON)
            if ( userPartitionIdx != COMM_MASTER_PARTITION_IDX )
            {
              /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
              ComM_SetUserReqFullCom( userReqFullComIdx, COMM_NO_REQUESTS, userPartitionIdx );
              ComM_SetForwardUserReqSyncCounter( user, (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u), userPartitionIdx );
              ComM_SetForwardingUserReqMode( user, COMM_NO_COMMUNICATION, userPartitionIdx );
            }
            else
#endif
            {
              /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
              ComM_SetUserReqFullCom( userReqFullComIdx, COMM_NO_REQUESTS, COMM_MASTER_PARTITION_IDX );
            }
          }
        }

#if (COMM_PNC_SUPPORT == STD_ON)
        if ( ComM_HasPncChannelMapping() )
        {
          for ( pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb( Channel ); /* FETA_COMM_CSL_USE_CASE */
            pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb( Channel );
            pncPbIndIter++ )
          {
            for ( userReqPncFullComIndex = ComM_GetUserReqPncFullComStartIdxOfPncPb( ComM_GetPncPbInd( pncPbIndIter ) ); /* FETA_COMM_CSL_USE_CASE */
              userReqPncFullComIndex < ComM_GetUserReqPncFullComEndIdxOfPncPb( ComM_GetPncPbInd( pncPbIndIter ) );
              userReqPncFullComIndex++ )
            {
#if (COMM_MULTIPARTITION == STD_ON)
              if ( userPartitionIdx != COMM_MASTER_PARTITION_IDX )
              {
                /* @ assert userReqPncFullComIndex < ComM_GetSizeOfUserReqPncFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQPNCFULLCOM_IDX */
                ComM_SetUserReqPncFullCom( userReqPncFullComIndex, COMM_NO_REQUESTS, userPartitionIdx );
              }
              else
#endif
              {
                /* @ assert userReqPncFullComIndex < ComM_GetSizeOfUserReqPncFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQPNCFULLCOM_IDX */
                ComM_SetUserReqPncFullCom( userReqPncFullComIndex, COMM_NO_REQUESTS, COMM_MASTER_PARTITION_IDX );
              }
            }
          }
        }
#endif /* COMM_PNC_SUPPORT == STD_ON */
      }
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
    }
  }
}

/*********************************************************************************************************************
 *  ComM_TF_NetReq_ReadyS()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_NetReq_ReadyS( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  if ( ComM_HasKeepAwakeChannels() )
  {
    /* #10 If Nm Light timer is configured, start it. It is ensured by generator that Nm Type of the channel is LIGHT. */
    if ( ComM_GetNmLightDurationOfChannel( Channel ) > 0u )
    {
      ComM_SetNmLightTimer( Channel, ComM_GetNmLightDurationOfChannel( Channel ), partitionIdx );
    }

    /* #20 If Nm Type is FULL, notify Nm about network release. */
    if ( ComM_GetNmTypeOfChannel( Channel ) == COMM_FULL_NMTYPEOFCHANNEL )
    {
#if (COMM_MULTIPARTITION == STD_ON)
      if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) != partitionIdx )
      {
        ComM_SetForwardNmNetworkReleaseSyncCounter( Channel, (uint8)(ComM_GetForwardNmNetworkReleaseSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) + 1u), partitionIdx );
      }
      else
#endif
      {
        (void)ComM_Nm_ServiceHandler( NM_NETWORKRELEASE, Channel, COMM_PNC_ID_NOT_USED );
      }
    }

    /* #30 If Nm Type is LINSLAVE, notify LinSm and directly request NO_COMMUNICATION. */
    if ( ComM_GetNmTypeOfChannel( Channel ) == COMM_LINSLAVE_NMTYPEOFCHANNEL )
    {
      ComM_ForwardBusSMModeRequest( Channel, COMM_NO_COMMUNICATION, partitionIdx );
    }
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_TF_No_Action()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_No_Action( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* #10 There is no action for this sub-state transition. Use cases:
   * COMM_NO_COM_REQUEST_PENDING to COMM_NO_COM_NO_PENDING_REQUEST (communication was never allowed and user/DCM request is canceled)
   * COMM_NO_COM_NO_PENDING_REQUEST to COMM_NO_COM_REQUEST_PENDING (communication is requested and ComM waits for allowed notification)
   */
  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_TF_No_Transition()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_TF_No_Transition( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* #10 This is an invalid state transition, report a Det error. */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_MAINFUNCTION, COMM_E_NOSUPPORTED_MODECHANGE );

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ComM_ChannelStateTransition()
 **********************************************************************************************************************/
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
FUNC( void, COMM_CODE ) ComM_ChannelStateTransition( NetworkHandleType channel, ComM_StateType highestComModeReq )
{
  ComM_StateType  calculatedComModeReq;  /* the calculated ComM mode */
  ComM_StateType  localhighestComModeReq = highestComModeReq;
  do /* FETA_COMM_CHANNEL_STATE_TRANSITION */
  {
    /* #10 Check if a timer is running for Nm Variants FULL or LIGHT. Do not perform any state transitions if
     *    1) Nm Light timer is running and there is no FULL_COM request by users or DCM (applicable in COMM_FULL_COM_READY_SLEEP or COMM_SILENT_COM) or
     *    2) Min FULL_COM mode timer is running.
     */
    if ( ((ComM_GetNmLightTimer( channel, COMM_MASTER_PARTITION_IDX ) > 0x00u) && (localhighestComModeReq == COMM_NO_COM_NO_PENDING_REQUEST)) ||
         (ComM_GetMinFullComModeTimer( channel, COMM_MASTER_PARTITION_IDX ) > 0x00u) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
    {
      localhighestComModeReq = ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX );
    }
    else
    {
      /* #20 If no timer is running, calculate the next state transition step and perform as many transitions as needed
       *     to reach the target state, which is defined by the highest requested communication mode. */
      calculatedComModeReq = ComM_TransitionTable[localhighestComModeReq][ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX )];
      if ( (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) < COMM_MAX_NUMBER_OF_STATES) && (calculatedComModeReq < COMM_MAX_NUMBER_OF_STATES) )
      {
        if ( (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) == COMM_NO_COM_REQUEST_PENDING) && (calculatedComModeReq > COMM_NO_COM_REQUEST_PENDING) )
        {
          /* #30 If the channel is in COMM_NO_COM_REQUEST_PENDING state and there is a pending request to start communication,
           *     check the Communication Allowed flag. If Communication Allowed is FALSE, stay in COMM_NO_COM_REQUEST_PENDING state
           *     and try again in the next task if request is still pending. */
          if ( !ComM_IsComAllowed( channel, COMM_MASTER_PARTITION_IDX ) )
          {
            localhighestComModeReq = ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX );
          }
        }

        if ( localhighestComModeReq != ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) )
        {
          ComM_ExecuteTransitionFct( channel, ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ), calculatedComModeReq, COMM_MASTER_PARTITION_IDX);
          /* #40 If the current channel is Nm variant LINSLAVE and current state is COMM_FULL_COM_READY_SLEEP, but the Bus Sleep Mode indication is still pending,
           *     the channel remains in state COMM_FULL_COM_READY_SLEEP. */
          if ( !(ComM_IsBusSleepModeIndicated( channel, COMM_MASTER_PARTITION_IDX )) && (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_READY_SLEEP) && /* PRQA S 3415 */ /* MD_ComM_3415 */
            (calculatedComModeReq == COMM_SILENT_COM) && (ComM_GetNmTypeOfChannel( channel ) == COMM_LINSLAVE_NMTYPEOFCHANNEL) /* PRQA S 3415 */ /* MD_ComM_3415 */
               )
          {
            /* Set localhighestComModeReq to state COMM_FULL_COM_READY_SLEEP in order to remain in state Ready Sleep and to prevent endless while loop. */
            localhighestComModeReq = COMM_FULL_COM_READY_SLEEP;
          }
          else
          {
            ComM_SetActiveComMode( channel, calculatedComModeReq, COMM_MASTER_PARTITION_IDX );
          }
        }
      }
      else
      {
        localhighestComModeReq = ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX );
      }
    }
  } while ( ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) != localhighestComModeReq );
}

/**********************************************************************************************************************
 *  ComM_ExecuteTransitionFct()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_ExecuteTransitionFct( NetworkHandleType channel, ComM_StateType sourceState, ComM_StateType targetState, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* #10 Perform the channel state transition from sourceState to targetState. */
  ComM_TransitionFctTable[targetState][sourceState]( channel, partitionIdx );
}

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
