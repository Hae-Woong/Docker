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
/*!         \file     ComM_Nm.c
 *          \unit     Nm
 *          \brief    Communication Manager ASR4
 *
 *          \details  Callback API services of the Autosar Communication Manager for the Network Management unit
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

#define CCL_ASR_COMM_NM_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include "ComM_Private_Nm.h"
#include "ComM_Channel_StateTransitions.h"
#include "ComM_Channel_StatusAndState.h"
#include "ComM_PNC_General.h"
#include "ComM_PNC_SignalProcessing.h"
#include "ComM_ModeInhibition.h"
#include "SchM_ComM.h"

#if (COMM_NMSUPPORTOFCHANNEL== STD_ON)
# include "Nm.h"
#endif

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
 *  ROM DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ComM_Nm_ServiceHandler
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC( Std_ReturnType, COMM_CODE ) ComM_Nm_ServiceHandler( uint8 serviceIndex, CONST( NetworkHandleType, AUTOMATIC ) channel, CONST( PNCHandleType, AUTOMATIC ) pncId )
{
  /* ----- Local Variable ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call the requested Nm service. */
  switch ( serviceIndex )
  {
    case NM_NETWORKREQUEST:
#if (COMM_NM_VARIANT_FULL_PRESENT == STD_ON)
      retVal = Nm_NetworkRequest( channel );
#endif
    break;

    case NM_NETWORKRELEASE:
#if (COMM_NM_VARIANT_FULL_PRESENT == STD_ON)
     retVal = Nm_NetworkRelease( channel );
#endif
    break;

    case NM_PASSIVESTARTUP:
#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
     retVal = Nm_PassiveStartUp( channel );
#endif
    break;

    case NM_REQUESTSYNCHRONIZEDPNCSHUTDOWN:
#if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON )
     retVal = Nm_RequestSynchronizedPncShutdown( channel, pncId );
#endif
    break;

    default: /* COV_COMM_MISRA */
    break;
  }

  COMM_DUMMY_STATEMENT( serviceIndex ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( pncId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return retVal;
}

/**********************************************************************************************************************
 *  CALLBACK FUNCTIONS (AUTOSAR NM)
 *********************************************************************************************************************/

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/*********************************************************************************************************************
 *  ComM_Nm_ProcessNetworkStartIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_ProcessNetworkStartIndication( NetworkHandleType Channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward the indication to the passive wake-up processing. */
  ComM_ProcessPassiveWakeUp( Channel, partitionIdx );
}

/*********************************************************************************************************************
 *  ComM_Nm_ProcessNetworkMode()
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
FUNC( void, COMM_CODE ) ComM_Nm_ProcessNetworkMode( NetworkHandleType Channel )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasSilentSupportOfChannel() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
#if ( COMM_MULTIPARTITION == STD_ON )
    ComM_StateType activeComMode = COMM_NO_COM_NO_PENDING_REQUEST;
    ComM_StateType busComModeReq = COMM_NO_COM_NO_PENDING_REQUEST;
    NetworkHandleType channelIdx;
#endif

    /* #10 If Prepare Bus-Sleep Mode is supported and Network Mode is notified while ComM is in SI_COM and Nm
      *     has notified Prepare Bus Sleep Mode before it, immediately enter the target wake-up state of the channel
      *     (Network Requested or Ready Sleep) and trigger BusSM to enter FULL_COM to ensure that Nm message
      *     can be sent as first message on the bus. */

#if (COMM_MULTIPARTITION == STD_ON)

    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

    /* Check if an event occurred on a satellite which has changed the active com mode since last master sync. */
    if ( ComM_GetActiveComModeSyncCounter( Channel, partitionIdx ) == (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
    {
      activeComMode = ComM_GetActiveComMode( Channel, partitionIdx );
    }
    else
    {
      activeComMode = ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX );
    }

    if ( ComM_GetBusComModeReqSyncCounter( Channel, partitionIdx ) == (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
    {
      busComModeReq = ComM_GetBusComModeReq( Channel, partitionIdx );
    }
    else
    {
      busComModeReq = ComM_GetBusComModeReq( Channel, COMM_MASTER_PARTITION_IDX );
    }

    if ( ComM_HasSynchronousWakeUp() )
    {
      if ( (busComModeReq & COMM_FULL_COMMUNICATION) == 0u )
      {
        for ( channelIdx = 0u; channelIdx < ComM_GetSizeOfChannel(); channelIdx++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          if ( ComM_GetEcuMWakeUpIndicationSyncCounter( channelIdx, ComM_GetPartitionConfigIdxOfChannel( channelIdx ) ) == (uint8)(ComM_GetEcuMWakeUpIndicationSyncCounter( channelIdx, COMM_MASTER_PARTITION_IDX ) + 1u) )
          {
            /* Set busComModeReq to WakeUpState to ensure that following condition is not entered. */
            busComModeReq = ComM_GetWakeupStateOfChannel( Channel );
            break;
          }
        }
      }
    }

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();

    if ( (activeComMode == COMM_SILENT_COM) && (busComModeReq == COMM_SILENT_COM) )
#else
    if ( (ComM_GetActiveComMode( Channel, partitionIdx ) == COMM_SILENT_COM) && (ComM_GetBusComModeReq( Channel, partitionIdx ) == COMM_SILENT_COM) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
#endif
    {
      /* ESCAN00066282 Note: ComM_ActiveComMode is set first to ensure that condition is not re-entered. */
      ComM_SetActiveComMode( Channel, ComM_GetWakeupStateOfChannel( Channel ), partitionIdx );
#if (COMM_MULTIPARTITION == STD_ON)
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2(); /* Critical section will be left inside the following transition function. */

        /* Increment sync counter for ComM_ActiveComMode by 1 (relative to the corresponding Master-Counter) */
        ComM_SetActiveComModeSyncCounter( Channel, (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      }
#endif
      ComM_ExecuteTransitionFct( Channel, COMM_SILENT_COM, ComM_GetWakeupStateOfChannel( Channel ), partitionIdx );

      /* SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2() -> done in transition function (ComM_TF_NoCom_NetReq() or ComM_TF_NoCom_FullReadySleep()). */
    }
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_Nm_ProcessPrepareBusSleepMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_ProcessPrepareBusSleepMode( NetworkHandleType Channel )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasSilentSupportOfChannel() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
#if ( COMM_MULTIPARTITION == STD_ON )
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();
#endif
    /* #10 Initiate the state transition to SI_COM. */
    ComM_SetBusComModeReq( Channel, COMM_SILENT_COM, partitionIdx );
    ComM_SetActiveComMode( Channel, COMM_SILENT_COM, partitionIdx );

#if (COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      /* Increment sync counter for ComM_BusComModeReq and ComM_ActiveComModeReq by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      ComM_SetActiveComModeSyncCounter( Channel, (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
    }

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
#endif

    ComM_ExecuteTransitionFct( Channel, COMM_FULL_COM_READY_SLEEP, COMM_SILENT_COM, partitionIdx );
  }
}

/*********************************************************************************************************************
 *  ComM_Nm_ProcessBusSleepMode()
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
FUNC( void, COMM_CODE ) ComM_Nm_ProcessBusSleepMode( NetworkHandleType Channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );


  /* ----- Implementation ----------------------------------------------- */
  if ( (ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX ) != COMM_FULL_COM_NETWORK_REQUESTED) && (ComM_GetBusTypeOfChannel( Channel ) == COMM_BUS_TYPE_FR) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    /* #10 Store the status that shutdown on a FlexRay channel has started (this is to counteract the so-called 'clique problem' on FlexRay). */
    ComM_SetFrShutdown( Channel, TRUE, partitionIdx );
  }

#if ( COMM_MULTIPARTITION == STD_ON)
  if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
  {
    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2(); /* Critical Section will either be left directly below, or in TF_SiCom_NoCom. */

    ComM_SetBusComModeReq( Channel, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );
    /* Increment the sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
    ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
    ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
  }
  else
#endif
  {
    ComM_SetBusComModeReq( Channel, COMM_NO_COM_NO_PENDING_REQUEST, COMM_MASTER_PARTITION_IDX );
  }

  /* #20 There are two use cases how Nm calls the function which can be identified through ComM_ActiveComMode: */
  if ( ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_NETWORK_REQUESTED )
  {
    /* #30 ComM_Nm_BusSleepMode() is called in context of Nm_NetworkRelease() while ComM performs the state transition COMM_FULL_COM_NETWORK_REQUESTED to COMM_FULL_COM_READY_SLEEP.
      *     This is only possible if LinNm without coordination (ESCAN00061388, ESCAN00072736) or J1939Nm (ESCAN00071329) is configured on the channel.
      *     Do nothing here and perform the transition to NoCom in the next ComM_MainFunction().
      *     This check is also a safety mechanism against potentially wrong Nm behavior:
      *         It ensures that ComM remains in state COMM_FULL_COM_NETWORK_REQUESTED and does not interrupt the established communication in case of a wrong call of ComM_Nm_BusSleepMode(). */

#if ( COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
#endif
  }
  else
  {
    /* #40 ComM_Nm_BusSleepMode() is called from the main function of a Nm module, this is the normal case. ComM shall perform the transition to NoCom immediately. */
    ComM_ExecuteTransitionFct( Channel, COMM_SILENT_COM, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );

    /* SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2() -> is done in ComM_TF_SiCom_NoCom(). */
  }

  /* #50 If FrSM reached NO_COM state, set the FlexRay shutdown status as finished. */
  if ( ComM_GetBusTypeOfChannel( Channel ) == COMM_BUS_TYPE_FR )
  {
    if ( (ComM_GetBusSmState( Channel, partitionIdx ) == COMM_NO_COMMUNICATION) && (ComM_IsFrShutdown( Channel, partitionIdx )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
    {
      ComM_SetFrShutdown( Channel, FALSE, partitionIdx );
    }
  }
}

/*********************************************************************************************************************
 *  ComM_Nm_ProcessRestartIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_ProcessRestartIndication( NetworkHandleType Channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward the indication to the passive wake-up processing. */
  ComM_ProcessPassiveWakeUp( Channel, partitionIdx );
}
#endif

#if ( COMM_PNC_SUPPORT == STD_ON )
/*********************************************************************************************************************
 *  ComM_Nm_ProcessStateChangeNotification()
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
FUNC( void, COMM_CODE ) ComM_Nm_ProcessStateChangeNotification( CONST( NetworkHandleType, AUTOMATIC ) Channel, CONST( Nm_StateType, AUTOMATIC ) NmPreviousState, CONST( Nm_StateType, AUTOMATIC ) NmCurrentState )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_IsPncLimitationUsedOfChannelPb( Channel ) )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncIterType pnc;
    ComM_PncChannelMappingEndIdxOfPncPbType pncChIdx;
    ComM_SizeOfPncSignalType signalIndex;
    boolean sendSignal = FALSE;
    ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
# if ( COMM_MULTIPARTITION == STD_ON )
    boolean isSignalStoredPncBitSet;
# endif

    if ( NmCurrentState == NM_STATE_REPEAT_MESSAGE )
    {
      /* #10 If Nm entered NM_STATE_REPEAT_MESSAGE state, set the status bit accordingly. */
      if ( ComM_GetPncToChannelRoutingState( Channel, COMM_MASTER_PARTITION_IDX ) == COMM_PNCROUTING_LIMITED )
      {
        /* #100 If PNC routing is limited on the channel, immediately send PNC bits = 1 for all PNC's connected to the channel
        *      and being in state PNC_REQUESTED.
        */
        for ( signalIndex = 0u; signalIndex < ComM_GetSizeOfPncSignal(); signalIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          if ( (ComM_GetTypeOfPncSignal( signalIndex ) == COMM_EIRA_TX_TYPEOFPNCSIGNAL) && (ComM_GetChannelIdxOfPncSignal( signalIndex ) == Channel) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
          {
            for ( pnc = 0u; pnc < ComM_GetSizeOfPnc(); pnc++ ) /* FETA_COMM_CSL_USE_CASE */
            {
              for ( pncChIdx = ComM_GetPncChannelMappingStartIdxOfPncPb( pnc ); pncChIdx < ComM_GetPncChannelMappingEndIdxOfPncPb( pnc ); pncChIdx++ ) /* FETA_COMM_CSL_USE_CASE */
              {
# if ( COMM_MULTIPARTITION == STD_ON )
                SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
                if ( ComM_GetPncSignalValuesSyncCounter( (uint8)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pnc )), partitionIdx ) ==  /* PRQA S 2985 */ /* MD_ComM_2985 */
                    (ComM_GetPncSignalValuesSyncCounter( (uint8)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pnc )), COMM_MASTER_PARTITION_IDX ) + 1u) ) /* PRQA S 2985 */ /* MD_ComM_2985 */
                {
                  isSignalStoredPncBitSet = (boolean)ComM_IsSignalStoredPncBitSet( signalIndex, pnc, partitionIdx ); /* PRQA S 2985 */ /* MD_ComM_2985 */
                }
                else
                {
                  isSignalStoredPncBitSet = (boolean)ComM_IsSignalStoredPncBitSet( signalIndex, pnc, COMM_MASTER_PARTITION_IDX ); /* PRQA S 2985 */ /* MD_ComM_2985 */
                }
                SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();

                if ( (isSignalStoredPncBitSet == FALSE) &&
# else
                if ( (!ComM_IsSignalStoredPncBitSet( signalIndex, pnc, COMM_MASTER_PARTITION_IDX )) && /* PRQA S 2985 */ /* MD_ComM_2985 */
# endif
                      (ComM_GetPncChannelMapping( pncChIdx ) == Channel) && (ComM_PncGetGwChannelState( Channel, ComM_GetActivePncComMode( pnc, COMM_MASTER_PARTITION_IDX ) ) == COMM_FULL_COM_NETWORK_REQUESTED) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
                {
                  /* #110 Attaching the channel: PNC Routing Limitation becomes temporarily disabled, send PNC bit = 1 if
                    *      1) PNC state is COMM_PNC_REQUESTED (the highest activation), then send bit = 1 on each channel or
                    *      2) PNC state is COMM_PNC_REQUESTED_PASSIVE (lower activation), then send bit = 1 on ACTIVE channels only.
                    */
                  ComM_PncSetBitInSignal( signalIndex, pnc, partitionIdx );
                  sendSignal = TRUE;
                }
              }
            }

            if ( sendSignal == TRUE )
            {
              /* Com Signal can be send directly (no sync needed), since the state change notification is always called in the partition context of the corresponding channel. */
              (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( signalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ), partitionIdx ) ); /* PRQA S 0315 */ /* MD_ComM_0315 */
            }
            break; /* There is only one EIRA_TX signal per channel */
          }
        }
      }

      /* Example sequence: Routing Limitation is enabled on a channel and a PNC is already in PNC_REQUESTED.
        * Channel user requests FULL_COM, channel enters COMM_NETWORK_REQUESTED and Nm enters NM_STATE_REPEAT_MESSAGE.
        * Then the user requests NO_COM while Nm is still in NM_STATE_REPEAT_MESSAGE. In this case
        * ComM_PncToChannelRoutingState[Channel] shall be COMM_PNCROUTING_NM_REPEAT_MESSAGE and PNC bit=1 shall be
        * sent as long as Nm is in Repeat Message state.
        */
# if (COMM_MULTIPARTITION == STD_ON)
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
        ComM_SetPncToChannelRoutingState( Channel, ComM_GetPncToChannelRoutingState( Channel, COMM_MASTER_PARTITION_IDX ) | COMM_PNCROUTING_NM_REPEAT_MESSAGE, partitionIdx ); /* Increment sync counter for ComM_PncToChannelRoutingState by 1 (relative to corresponding Master-Counter) */
        ComM_SetPncToChannelRoutingStateSyncCounter( Channel, (uint8)(ComM_GetPncToChannelRoutingStateSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
      }
      else
# endif
      {
        ComM_SetPncToChannelRoutingState( Channel, ComM_GetPncToChannelRoutingState( Channel, partitionIdx ) | COMM_PNCROUTING_NM_REPEAT_MESSAGE, partitionIdx );
      }
    }
    else if ( NmPreviousState == NM_STATE_REPEAT_MESSAGE )
    {
      /* #20 If Nm has left NM_STATE_REPEAT_MESSAGE, clear the status bit. If PNC routing status changes to COMM_PNCROUTING_LIMITED,
        *     ComM will set PNC bits to 0 for all PNC's connected to the channel and being in state PNC_REQUESTED in the next ComM_MainFunction().
        */
# if (COMM_MULTIPARTITION == STD_ON)
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
        ComM_SetPncToChannelRoutingState( Channel, ComM_GetPncToChannelRoutingState( Channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NM_REPEAT_MESSAGE_CLEAR, partitionIdx ); /* Increment sync counter for ComM_PncToChannelRoutingState by 1 (relative to corresponding Master-Counter) */
        ComM_SetPncToChannelRoutingStateSyncCounter( Channel, (uint8)(ComM_GetPncToChannelRoutingStateSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
      }
      else
# endif
      {
        ComM_SetPncToChannelRoutingState( Channel, ComM_GetPncToChannelRoutingState( Channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NM_REPEAT_MESSAGE_CLEAR, partitionIdx );
      }
    }
    else
    {
      /* nothing to do */
    }
  }
}

# if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON )
/*********************************************************************************************************************
 *  ComM_Nm_ProcessForwardSynchronizedPncShutdown()
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
FUNC( void, COMM_CODE ) ComM_Nm_ProcessForwardSynchronizedPncShutdown( CONST( NetworkHandleType, AUTOMATIC ) Channel, CONSTP2CONST( uint8, AUTOMATIC, COMM_APPL_VAR ) PncBitVectorPtr )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasIntermediatePncCoordinatorOfPnc() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncIterType pncIndex;
    ComM_PncChannelMappingEndIdxOfPncPbType pncChIdx;
# if (COMM_MULTIPARTITION == STD_ON)
    ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
# endif

    /* #10 If the ECU is in the role of an intermediate PNC coordinator, check if the affected PNCs are qualified for a forward synchronized PNC shutdown indication request.
      *     1) The affect PNC's bit is set to '1' in the PNC bit vector and
      *     2) The indicated channel is assigned to this PNC and has attribute GW type set to COMM_GATEWAY_TYPE_PASSIVE
      */
    if ( ComM_GetGwTypeOfChannel( Channel ) == COMM_GATEWAY_TYPE_PASSIVE )
    {
      for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        if ( (PncBitVectorPtr[ComM_GetSignalByteIndexOfPncPb( pncIndex )] & ComM_GetSetMaskOfPnc( pncIndex )) > 0u )
        {
          for ( pncChIdx = ComM_GetPncChannelMappingStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
                pncChIdx < ComM_GetPncChannelMappingEndIdxOfPncPb( pncIndex ); pncChIdx++ )
          {
            if ( (ComM_GetPncChannelMapping( pncChIdx ) == Channel) )
            {
              /* #20 Store the request for the synchronized PNC shutdown. Forward the synchronized PNC shutdown request towards the Nm on all active channels as soon as the affected PNC is released.*/
# if (COMM_MULTIPARTITION == STD_ON)
              if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
              {
                /* Increment the sync counter for ComM_ForwardNmSyncPncShutdownIndicated by 1 (relative to the corresponding Master-Counter) */
                ComM_SetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, (uint8)(ComM_GetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
                ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
              }
              else
# endif
              {
                ComM_SetForwardSynchronizedPncShutdownIndicated( pncIndex, TRUE, COMM_MASTER_PARTITION_IDX );
              }
              break;
            }
          }
        }
      }
    }
  }
}
# endif
#endif

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
