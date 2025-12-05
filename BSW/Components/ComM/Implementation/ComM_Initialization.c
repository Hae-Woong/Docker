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
/*!         \file     ComM_Initialization.c
 *          \unit     Initialization
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the Initialization unit.
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

#define CCL_ASR_COMM_INITIALIZATION_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM.h"
#include "ComM_Initialization.h"
#include "ComM_ModeInhibition.h"
#include "ComM_Util.h"
#include "ComM_NvM.h"

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

# define COMM_START_SEC_VAR_INIT_8
  /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"
/*! Pre-Initialization state of ComM */
/* PRQA S 3218 1 */ /* MD_ComM_3218 */
COMM_LOCAL VAR( ComM_PreInitializedType, COMM_VAR_CLEARED )   ComM_PreInitialized = FALSE;
# define COMM_STOP_SEC_VAR_INIT_8
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
# include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

#if (COMM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
  ComM_InitCheckGeneratedData()
 *********************************************************************************************************************/
/*! \brief      Checks generated data for correctness.
 *  \details    -
 *  \param[in]  partitionIdx                identification of the internal partition index
 *  \return     COMM_E_NO_ERROR             check is successful
 *  \return     COMM_E_WRONG_GENERATED_DATA otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( uint8, COMM_CODE ) ComM_InitCheckGeneratedData( CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );
#endif

#if (COMM_MULTIPARTITION == STD_ON)
/**********************************************************************************************************************
  ComM_InitMasterAndSatelliteChannelSyncCounter()
 ********************************************************************************************************************/
/*! \brief       Initializes the channel related synchronization counters
 *  \details     -
 *  \param[in]   partitionIdx            identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE for the different partitions
 *  \synchronous TRUE
 *  \note        Called by ComM_Init()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_InitMasterAndSatelliteChannelSyncCounter( CONST( NetworkHandleType, AUTOMATIC ) channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );
#endif

#if ( COMM_PNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
  ComM_PncInit()
 *********************************************************************************************************************/
/*! \brief      Initialization of PNC related data.
 *  \details    -
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-19845, SPEC-19781
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_PncInit( CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );
#endif

#if ( COMM_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
  ComM_SetPreInitializedStatus
 *********************************************************************************************************************/
 /*! \brief      Sets the Pre-Initialitation status of ComM
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SetPreInitializedStatus( ComM_PreInitializedType status );
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if ( COMM_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
  ComM_SetPreInitializedStatus
 *********************************************************************************************************************/
 /*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SetPreInitializedStatus( ComM_PreInitializedType status )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the Pre-Initialized status. */
  ComM_PreInitialized = status;
}
#endif

#if (COMM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  ComM_InitCheckGeneratedData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
COMM_LOCAL_INLINE FUNC( uint8, COMM_CODE ) ComM_InitCheckGeneratedData( CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_ChannelIterType channelIndex;
  ComM_ChannelIterType sizeOfChannel = ComM_GetSizeOfChannel();
# if (COMM_PNC_SUPPORT)
  ComM_PncIterType sizeOfPnc = ComM_GetSizeOfPnc();
# endif
# if ( COMM_MULTIPARTITION == STD_ON )
#  if ( COMM_PNC_SUPPORT == STD_ON )
  ComM_PncSignalValuesIterType sizeOfPncSignalValues = ComM_GetSizeOfPncSignalValues( partitionIdx );
#  endif
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the generated data and return COMM_E_NO_ERROR if successful.Otherwise return COMM_E_WRONG_GENERATED_DATA. */
  if ( sizeOfChannel != ComM_GetSizeOfChannelPb() ) /*lint !e506 */
  {
    errorId = COMM_E_WRONG_GENERATED_DATA;
  }
# if (COMM_PNC_SUPPORT)
  else if ( sizeOfPnc != ComM_GetSizeOfPncPb() ) /*lint !e506 */
  {
    errorId = COMM_E_WRONG_GENERATED_DATA;
  }
# endif
# if ( COMM_MULTIPARTITION == STD_ON )
#  if ( COMM_PNC_SUPPORT == STD_ON )
  else if ( sizeOfPncSignalValues != ComM_GetSizeOfPncSignalValuesSyncCounter( partitionIdx ) ) /*lint !e506 */
  {
    errorId = COMM_E_WRONG_GENERATED_DATA;
  }
#  endif
# endif
  else
  {
    for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 0771 */ /* MD_ComM_0771 */
    {
      uint8 maxNumberOfStates = ComM_GetWakeupStateOfChannel( channelIndex );
      /* Check the target wake-up state of the channel because it is used as index in ComM_TransitionFctTable */
      if ( maxNumberOfStates >= COMM_MAX_NUMBER_OF_STATES )
      {
        errorId = COMM_E_WRONG_GENERATED_DATA;
        break;
      }

      if ( (ComM_GetManagingChannelIdOfChannel( channelIndex ) != COMM_NO_MANAGINGCHANNELIDOFCHANNEL) && (ComM_GetManagingChannelIdOfChannel( channelIndex ) >= sizeOfChannel) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
      {
        errorId = COMM_E_WRONG_GENERATED_DATA;
        break;
      }
    }
  }

  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return errorId;
}
#endif

#if (COMM_MULTIPARTITION == STD_ON)
/**********************************************************************************************************************
 *  ComM_InitMasterAndSatelliteChannelSyncCounter()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_InitMasterAndSatelliteChannelSyncCounter( CONST( NetworkHandleType, AUTOMATIC ) channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all channel related synchronization counters.
   *  - #100 Initialize the master counters to zero.
   *  - #110 Initialize the satellite counters to zero.
   *  - #120 Initialize the forwarding synchronization counters to zero.
  */
  ComM_SetGlobalPartitionSyncCounter( channel, 0u, partitionIdx );

  ComM_SetBusSmStateSyncCounter( channel, 0u, partitionIdx );

  ComM_SetForwardNmPassiveStartUpSyncCounter( channel, 0u, partitionIdx );
  ComM_SetForwardRequestBusSmModeSyncCounter( channel, 0u, partitionIdx );
  ComM_SetForwardingBusSmMode( channel, 0u, partitionIdx );

  ComM_SetActiveComModeSyncCounter( channel, 0u, partitionIdx );

  ComM_SetBusComModeReqSyncCounter( channel, 0u, partitionIdx );

  ComM_SetBusSleepModeIndicatedSyncCounter( channel, 0u, partitionIdx );

  ComM_SetBusComModeReqFrShutdownSyncCounter( channel, 0u, partitionIdx );

  ComM_SetMinFullComModeTimerSyncCounter( channel, 0u, partitionIdx );

  ComM_SetNmLightTimerSyncCounter( channel, 0u, partitionIdx );

  ComM_SetInitiateResetSyncCounter( 0u, partitionIdx );

  ComM_SetDcmRequestActiveSyncCounter( channel, 0u, partitionIdx );

  ComM_SetStateChangeSyncCounter( channel, 0u, partitionIdx );

  ComM_SetForwardNmNetworkReleaseSyncCounter( channel, 0u, partitionIdx );
  ComM_SetForwardNmNetworkRequestSyncCounter( channel, 0u, partitionIdx );

# if ( COMM_PNC_SUPPORT == STD_ON )
  ComM_SetForwardComSendSignalSyncCounter( channel, 0u, partitionIdx );
  ComM_SetForwardingComSendSignalIndex( channel, 0u, partitionIdx );
# endif

  ComM_SetNmStartUpIndicationSyncCounter( channel, 0u, partitionIdx );
  ComM_SetEcuMWakeUpIndicationSyncCounter( channel, 0u, partitionIdx );
}
#endif

#if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_PncInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
 /* PRQA S 6010 1 */ /* MD_MSR_STPTH */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_PncInit( CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_UserReqPncFullComIterType userReqPncFullComIndex;
  ComM_PncIterType pncIndex;
  ComM_PncSignalValuesIterType pncSignalValueIndex;
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PncPartitionIndirectionIterType partitionConfig;
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all PNC related data. */
  for ( userReqPncFullComIndex = 0u; userReqPncFullComIndex < ComM_GetSizeOfUserReqPncFullCom( partitionIdx ); userReqPncFullComIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SetUserReqPncFullCom( userReqPncFullComIndex, COMM_NO_REQUESTS, partitionIdx );
  }

  for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SetPncPSleepTimer( pncIndex, 0x00u, partitionIdx );
    ComM_SetActivePncComMode( pncIndex, COMM_PNC_NO_COMMUNICATION, partitionIdx );
    ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_NO_COMMUNICATION, partitionIdx );
# if (COMM_MULTIPARTITION == STD_ON)
    ComM_SetBusPncComModeReqSyncCounter( pncIndex, 0u, partitionIdx );
    if ( partitionIdx == COMM_MASTER_PARTITION_IDX )
    {
      for ( partitionConfig = 0u; partitionConfig < ComM_GetSizeOfPncPartitionIndirection(); partitionConfig++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        if ( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionConfig ) + pncIndex) < ComM_GetBusPncComModeReqMasterSyncCounterEndIdxOfPncPartitionIndirection( partitionConfig ) )
        {
          ComM_SetBusPncComModeReqMasterSyncCounter( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionConfig ) + pncIndex), 0u, COMM_MASTER_PARTITION_IDX );
        }
      }
    }
# endif

    ComM_SetForwardSynchronizedPncShutdownIndicated( pncIndex, FALSE, partitionIdx );
# if (COMM_MULTIPARTITION == STD_ON)
    ComM_SetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, 0u, partitionIdx );
# endif
  }

  /* #20 Clear the stored signal values (EIRA_RX, ERA_RX, EIRA_TX) */
  for ( pncSignalValueIndex = 0u; pncSignalValueIndex < ComM_GetSizeOfPncSignalValues( partitionIdx ); pncSignalValueIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SetPncSignalValues( pncSignalValueIndex, 0x00, partitionIdx );
# if (COMM_MULTIPARTITION == STD_ON)
    ComM_SetPncSignalValuesSyncCounter( pncSignalValueIndex, 0u, partitionIdx );
# endif
  }

  if ( ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX ) )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_ChannelIterType channelIndex;
    ComM_PncLimitationIterType pncLimitationIndex;

    /* #30 If PNC Routing Limitation is enabled, set all channels status to not limited. */
    for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      ComM_SetPncToChannelRoutingState( channelIndex, COMM_PNCROUTING_NOT_LIMITED, partitionIdx );
# if (COMM_MULTIPARTITION == STD_ON)
      ComM_SetPncToChannelRoutingStateSyncCounter( channelIndex, 0u, partitionIdx );
# endif
    }

    for ( pncLimitationIndex = 0u; pncLimitationIndex < ComM_GetSizeOfPncLimitation( partitionIdx ); pncLimitationIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      ComM_SetPncLimitation( pncLimitationIndex, (uint8)0x00u, partitionIdx );
    }
  }

  ComM_SetNotifyPncStateChange( FALSE, partitionIdx );
}
#endif


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_Initialization_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Initialization_InitMemory( void )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the pre-initialization status of ComM to FALSE. */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  ComM_SetPreInitializedStatus( FALSE );
#endif
}

/**********************************************************************************************************************
 *  ComM_Initialization_PreInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Initialization_PreInit( void )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType partitionIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all partitions and set the InitStatus. */
  for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SetInitStatus( COMM_UNINIT, partitionIdx );
  }

  /* #20 Set the post-initialization status of ComM to FALSE. */
#if ( COMM_MULTIPARTITION == STD_ON )
  if ( ComM_GetSizeOfPostInitialized( COMM_MASTER_PARTITION_IDX ) == 1u )
#endif
  {
    ComM_SetPostInitialized( 0u, FALSE, COMM_MASTER_PARTITION_IDX );
  }

#if ( COMM_DEV_ERROR_DETECT == STD_ON )
  ComM_SetPreInitializedStatus( TRUE );
#endif
}

/***********************************************************************************************************************
 *  ComM_Initialization_Init()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
FUNC( uint8, COMM_CODE ) ComM_Initialization_Init( CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
  uint8 ComM_ChannelIndex;
  ComM_SizeOfUserReqFullComType ComM_ByteIndex;
  ComM_InhibitionPtrType ComMInhibition = ComM_GetAddrInhibition( partitionIdx );
  ComM_UserIterType userIndex;
  boolean isNvMDataRestored = FALSE;


  /* ----- Implementation ----------------------------------------------- */
  /* #10 Restore the NvM data. */
  if ( ComM_NvM_GetErrorStatus( &isNvMDataRestored ) != E_OK )
  {
    errorId = COMM_E_ERROR_IN_PROV_SERVICE;
  }

  if ( isNvMDataRestored == FALSE ) /* The last NvM read request has failed, initialize with configured data. */
  {
    ComMInhibition->ComM_ECUGroupClassification = ComM_ECUGroupClassInit;  /* initialize the ECU group classification with the configured value */
    ComMInhibition->ComM_InhibitCnt             = 0x00u;                   /* reset the inhibition counter */
  }

  /* #20 Initialize the ComM variables. */
  if ( ComM_HasFullCommRequestNotif() )
  {
    ComM_SizeOfFullComRequestersType currentNumOfRequesters;
    ComM_CurrentChannelRequestInit();

    for ( currentNumOfRequesters = 0u; currentNumOfRequesters < ComM_GetSizeOfFullComRequesters( partitionIdx ); currentNumOfRequesters++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      ComM_SetFullComRequesters( currentNumOfRequesters, 0xFFu, partitionIdx );
    }
  }

  if ( ComM_HasUserModeNotiFunc() )
  {
#if (COMM_MULTIPARTITION == STD_ON )
    if ( partitionIdx == COMM_MASTER_PARTITION_IDX )
#endif
    {
      for ( userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        ComM_SetLastStateChange( userIndex, COMM_NO_COMMUNICATION, partitionIdx );
      }
    }
  }

  for ( ComM_ChannelIndex = 0u; ComM_ChannelIndex < ComM_GetSizeOfChannel(); ComM_ChannelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SetBusComModeReq( ComM_ChannelIndex, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );

    ComM_SetBusSleepModeIndicated( ComM_ChannelIndex, FALSE, partitionIdx );

    ComM_SetBusComModeReqFrShutdown( ComM_ChannelIndex, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );
    ComM_SetFrShutdown( ComM_ChannelIndex, FALSE, partitionIdx );

    ComM_SetActiveComMode( ComM_ChannelIndex, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );
    ComM_SetBusSmState( ComM_ChannelIndex, COMM_NO_COMMUNICATION, partitionIdx );

    ComM_SetMinFullComModeTimer( ComM_ChannelIndex, 0x00u, partitionIdx );

    ComM_SetNmLightTimer( ComM_ChannelIndex, 0x00u, partitionIdx );

    ComM_SetComAllowed( ComM_ChannelIndex, FALSE, partitionIdx );

#if (COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx == COMM_MASTER_PARTITION_IDX )
#endif
    {
      ComM_SetResetRequired( ComM_ChannelIndex, FALSE, partitionIdx );
    }

    if ( ComM_HasDcmIndication() && ComM_HasKeepAwakeChannels() )
    {
      ComM_SetDcmRequestActive( ComM_ChannelIndex, FALSE, partitionIdx );
    }

    if ( ComM_HasUserModeNotiFunc() && !(ComM_HasUserModeNotificationOnlyForPncUsers()) )
    {
      ComM_SetStateChange( ComM_ChannelIndex, COMM_NO_STATE_CHANGE, partitionIdx );
    }

    if ( isNvMDataRestored == TRUE )
    {
      /* Successfully restored non-volatile data, ComM_InhibitionStatus[] now contains the restored status of Wake-up Inhibition.
       * Now add the configured value of ComMNoCom (No Com Mode Limitation) which is not stored non-volatile. */
      ComMInhibition->ComM_InhibitionStatus[ComM_ChannelIndex] |= (uint8)(ComM_GetInhibitionInitValueOfChannel( ComM_ChannelIndex ) & COMM_WAKEUP_INHIBITION_CLEAR_MASK); /* PRQA S 2985 */ /* MD_ComM_2985 */
    }
    else
    {
      /* Mode Limitation and Wake-up Inhibition are disabled or couldn't restore non-volatile data, initialize with configured values. */
      ComMInhibition->ComM_InhibitionStatus[ComM_ChannelIndex] = ComM_GetInhibitionInitValueOfChannel( ComM_ChannelIndex );
    }

#if ( COMM_MULTIPARTITION == STD_ON )
    ComM_InitMasterAndSatelliteChannelSyncCounter( ComM_ChannelIndex, partitionIdx );
#endif

    ComM_SetSimulatedSmMode( ComM_ChannelIndex, COMM_NO_COMMUNICATION, partitionIdx );
  }

  for ( ComM_ByteIndex = 0u; ComM_ByteIndex < ComM_GetSizeOfUserReqFullCom( partitionIdx ); ComM_ByteIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SetUserReqFullCom( ComM_ByteIndex, COMM_NO_REQUESTS, partitionIdx );
  }

#if ( COMM_MULTIPARTITION == STD_ON )
  for ( userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SetForwardUserReqSyncCounter( userIndex, 0u, partitionIdx );
    ComM_SetForwardingUserReqMode( userIndex, 0u, partitionIdx );
  }
#endif

  ComM_SetNvMStoreReq( FALSE, partitionIdx );

#if (COMM_MULTIPARTITION == STD_ON)
  if ( partitionIdx == COMM_MASTER_PARTITION_IDX )
#endif
  {
    ComM_SetEcuMWakeUpIndicated( 0u, FALSE, COMM_MASTER_PARTITION_IDX );
    ComM_SetNmStartUpIndicated( 0u, FALSE, COMM_MASTER_PARTITION_IDX );
  }

#if (COMM_PNC_SUPPORT)
  ComM_PncInit( partitionIdx );
#endif

  /* #30 Check the generated data and if successful set the initialization status to initialized. */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  if ( ComM_InitCheckGeneratedData( partitionIdx ) == COMM_E_NO_ERROR )
#endif
  {
    ComM_SetInitStatus( COMM_INIT, partitionIdx );
  }

  return errorId;
}

/**********************************************************************************************************************
 *  ComM_Initialization_PostInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( uint8, COMM_CODE ) ComM_Initialization_PostInit( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType partitionIdx;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
#if ( COMM_MULTIPARTITION == STD_ON )
  if ( ComM_GetSizeOfPostInitialized( COMM_MASTER_PARTITION_IDX ) == 1u )
#endif
  {
    ComM_SetPostInitialized( 0u, FALSE, COMM_MASTER_PARTITION_IDX );
    for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( ComM_GetInitStatus( partitionIdx ) != COMM_INIT )
      {
        errorId = COMM_E_NOT_INITED;
        break;
      }
    }

    /*#10 If ComM is initialized, set the PostInitialized status.*/
    if ( errorId == COMM_E_NO_ERROR )
    {
      ComM_SetPostInitialized( 0u, TRUE, COMM_MASTER_PARTITION_IDX );
    }
  }

  return errorId;
}

/*********************************************************************************************************************
 *  ComM_Initialization_DeInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( uint8, COMM_CODE ) ComM_Initialization_DeInit( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_ChannelIterType channelIndex;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger the storage of non-volatile values. */
  if ( ComM_NvM_StoreRequest() != E_OK )
  {
    errorId = COMM_E_ERROR_IN_PROV_SERVICE;
  }

  /* #20 Iterate over the channels and check if all are in state COMM_NO_COM_NO_PENDING_REQUEST.
  *     If so, set the initialization status to COMM_UNINIT. */
  for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    if ( ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_COM_NO_PENDING_REQUEST )
    {
      break; /* De-Initialization impossible */
    }
  }

  if ( channelIndex == ComM_GetSizeOfChannel() ) /* All channels are in state COMM_NO_COM_NO_PENDING_REQUEST because break above was not reached */
  {
#if ( COMM_MULTIPARTITION == STD_ON )
    if ( ComM_GetSizeOfPostInitialized( COMM_MASTER_PARTITION_IDX ) == 1u )
#endif
    {
      ComM_SetPostInitialized( 0u, FALSE, COMM_MASTER_PARTITION_IDX );
    }
    ComM_SetInitStatus( COMM_UNINIT, COMM_MASTER_PARTITION_IDX );
#if (COMM_MULTIPARTITION == STD_OFF)
    ComM_InitMemory();
#endif
  }

  return errorId;
}

/*********************************************************************************************************************
 *  ComM_Initialization_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_Initialization_GetStatus( P2VAR( ComM_InitStatusType, AUTOMATIC, COMM_APPL_VAR ) Status )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store the initialization state of ComM. */
#if ( COMM_USE_INIT_POINTER == STD_ON )
  if ( ComM_ConfigPtr == NULL_PTR )
  {
    *Status = COMM_UNINIT;
  }
  else
#endif
  {
    *Status = (ComM_Initialization_IsPostInitialized() == TRUE) ? COMM_INIT : COMM_UNINIT;
  }

  return  E_OK;
}

/**********************************************************************************************************************
 *  ComM_Initialization_IsPostInitialized()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( ComM_PostInitializedType, COMM_CODE ) ComM_Initialization_IsPostInitialized( void )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the module is Post-initialized. */
  return ComM_IsPostInitialized( 0u, COMM_MASTER_PARTITION_IDX );
}

/**********************************************************************************************************************
 *  ComM_Initialization_IsPreInitialized()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( ComM_PreInitializedType, COMM_CODE ) ComM_Initialization_IsPreInitialized( void )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the module is Pre-initialized. */
  return ComM_PreInitialized;
}

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
