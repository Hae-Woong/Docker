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
/*!         \file     ComM_MultiPartitionSynchronization.c
 *          \unit     MultiPartitionSynchronization
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the MultiPartitionSynchronization unit.
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

#define CCL_ASR_COMM_MULTIPARTITIONSYNCHRONIZATION_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include "ComM_MultiPartitionSynchronization.h"
#include "ComM_ModeInhibition.h"
#include "BswM_ComM.h"
#include "ComM_PNC_General.h"
#include "ComM_Util.h"
#include "SchM_ComM.h"

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

#if (COMM_MULTIPARTITION == STD_ON)
/**********************************************************************************************************************
  ComM_UpdateMasterSyncCounterAndStates()
 *********************************************************************************************************************/
/*! \brief          Updates the master sync counter and the corresponding new state, if available.
 *  \details        Checks if the state has changed on the satellite partition by comparing the master sync counter to the
 *                  satellite sync counter. If the state has changed, update the master sync counter and copy the new state
 *                  to the master partition.
 *  \param[in]      satSyncCounterPtr      Pointer to the satellite sync counter for this variable
 *  \param[in,out]  masterSyncCounterPtr   Pointer to the master sync counter for this variable
 *  \param[in]      satStatePtr            Pointer to the satellite state value of this variable
 *  \param[in,out]  masterStatePtr         Pointer to the master state value of this variable
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \note           Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateMasterSyncCounterAndStates( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satSyncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterSyncCounterPtr,
                                                                                 volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satStatePtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterStatePtr );

/**********************************************************************************************************************
  ComM_UpdateMasterSyncCounterAndTimers()
 *********************************************************************************************************************/
/*! \brief          Updates the master sync counter and the corresponding new timer value, if available.
 *  \details        Checks if the state has changed on the satellite partition by comparing the master sync counter to the
 *                  satellite sync counter. If the state has changed, update the master sync counter and set the timer on
 *                  the master partition to the value given by satState.
 *  \param[in]      satSyncCounterPtr      Pointer to the satellite sync counter for this variable
 *  \param[in,out]  masterSyncCounterPtr   Pointer to the master sync counter for this variable
 *  \param[in]      satTimer               The satellite state value of this variable
 *  \param[in,out]  masterTimerPtr         Pointer to the master state value of this variable
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \note           Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateMasterSyncCounterAndTimers( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satSyncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterSyncCounterPtr,
                                                                                 CONST( ComM_MinFullComTimeOfChannelType, AUTOMATIC ) satTimer, volatile P2VAR( ComM_MinFullComModeTimerType, AUTOMATIC, COMM_APPL_VAR ) masterTimerPtr );

/**********************************************************************************************************************
  ComM_SyncUserFullComRequests()
 *********************************************************************************************************************/
 /*! \brief      Synchronizes the channel and PNC user requests.
 *  \details     Checks every satellite partition if a user request was performed.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncUserFullComRequests( void );

/**********************************************************************************************************************
  ComM_SyncBusComModeRequests()
 *********************************************************************************************************************/
/*! \brief          Synchronizes the bus-site requested communication mode of all partitions.
 *  \details        Checks every satellite partition for a new bus-site requested communication mode.
 *  \param[in]      channel                            local channel handle
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \note           Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncBusComModeRequests( CONST( NetworkHandleType, AUTOMATIC ) channel, P2CONST( ComM_StateType, AUTOMATIC, COMM_CONST ) lHighestCalculatedBusComMode );

/**********************************************************************************************************************
  ComM_SyncSynchronousWakeUpIndications()
 *********************************************************************************************************************/
/*! \brief       Synchronizes the synchronous wakeup indications for all partitions.
 *  \details     Checks if a passive wakeup event occurred due to Nm (RestartIndication or NetworkStartIndication) or EcuM
 *               and wakes up all channels accordingly which are not yet in FULL_COMMUNICATION.
 *  \param[in]   channel                     local channel handle
 *  \param[in]   partitionIdx                identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncSynchronousWakeUpIndications( CONST( NetworkHandleType, AUTOMATIC ) channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) lHighestCalculatedBusComMode, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

# if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
  ComM_SyncBusPncComModeRequests()
 *********************************************************************************************************************/
/*! \brief       Synchronizes the bus-site requested PNC communication mode of all partitions.
 *  \details     Checks every satellite partition for a new bus-site requested PNC communication mode.
 *  \param[in]   partitionIdx                identification of the internal partition index
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by ComM_MainFunction()
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncBusPncComModeRequests( CONST( ComM_PCPartitionConfigIdxOfPartitionIdentifiersType, AUTOMATIC ) partitionIdx );
# endif
#endif


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (COMM_MULTIPARTITION == STD_ON)
/**********************************************************************************************************************
 *  ComM_UpdateMasterSyncCounterAndStates()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateMasterSyncCounterAndStates( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satSyncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterSyncCounterPtr,
                                                                                 volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satStatePtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterStatePtr )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if on any partition the synchronization counter was incremented by 1 compared to the counter on the master partition. */
  if ( *satSyncCounterPtr == (uint8)(*masterSyncCounterPtr + 1u) ) /* PRQA S 0404 */ /* MD_ComM_0404 */
  {
    /* #100 Increment the synchronization counter on the master partition and update the state on the master partition
     *     with the value of the satellite partition. */
    *masterStatePtr = *satStatePtr;
    *masterSyncCounterPtr = *satSyncCounterPtr;
  }
}

/**********************************************************************************************************************
 *  ComM_UpdateMasterSyncCounterAndTimers()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_UpdateMasterSyncCounterAndTimers( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) satSyncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) masterSyncCounterPtr,
                                                                                 CONST( ComM_MinFullComTimeOfChannelType, AUTOMATIC ) satTimer, volatile P2VAR( ComM_MinFullComModeTimerType, AUTOMATIC, COMM_APPL_VAR ) masterTimerPtr )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_Channel_HasNmTypeFullAndMinFullComTime() )
  {
    /* #10 Check if on any partition the synchronization counter was incremented by 1 compared to the counter on the master partition. */
    if ( *satSyncCounterPtr == (uint8)(*masterSyncCounterPtr + 1u) ) /* PRQA S 0404 */ /* MD_ComM_0404 */
    {
      /* #100 Increment the synchronization counter on the master partition and update the state on the master partition
       *     with the value of the satellite partition. */
      *masterTimerPtr = satTimer;
      *masterSyncCounterPtr = *satSyncCounterPtr;
    }
  }

  COMM_DUMMY_STATEMENT_CONST( satSyncCounterPtr ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( satTimer ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( masterSyncCounterPtr ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( masterTimerPtr ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_SyncUserFullComRequests()
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
 */
 /* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncUserFullComRequests( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_UserIterType user;
  ComM_UserReqFullComType userReqFullCom;
  ComM_UserReqFullComIdxOfUserByteMaskType userReqFullComIdx;
  ComM_UserByteMaskEndIdxOfUserType iByteMaskUserIndex;
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType userPartitionIdx;
#if (COMM_PNC_SUPPORT == STD_ON)
  ComM_UserPncByteMaskEndIdxOfUserType iByteMaskPncIndex;
  ComM_UserReqPncFullComType userReqPncFullCom;
  ComM_UserReqPncFullComEndIdxOfPncPbType userReqPncFullComIndex;
#endif

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

  for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    /* #10 Check if on any partition the synchronization counter was incremented by 1 compared to the counter on the master partition. */
    userPartitionIdx = (ComM_PCPartitionConfigIdxOfPartitionIdentifiersType)ComM_GetPartitionConfigIdxOfUser( user );
    if ( ComM_GetForwardUserReqSyncCounter( user, userPartitionIdx ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
    {
      /* #20 Update the state on the master partition with the value of the satellite partition.
       *     The userReqFullCom data is calculated from the master partition based on the state which is indicated on the satellite partition.
       *     The userReqFullCom data on the satellite remains as it is later on requested by ComM_GetRequestedComMode and ComM_GetState.
       */
      if ( ComM_GetForwardingUserReqMode( user, userPartitionIdx ) == COMM_FULL_COMMUNICATION )
      {
        for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          userReqFullComIdx = ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex );
          userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( userReqFullComIdx, COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfUserByteMask( iByteMaskUserIndex ));

          /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
          ComM_SetUserReqFullCom( userReqFullComIdx, userReqFullCom, COMM_MASTER_PARTITION_IDX );
        }
#if (COMM_PNC_SUPPORT == STD_ON)
        if ( ComM_HasPncPb() && ComM_IsPncUserOfUser( user ) )
        {
          for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( user ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( user ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            userReqPncFullComIndex = ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex );
            userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfUserPncByteMask( iByteMaskPncIndex ));
              
            ComM_SetUserReqPncFullCom( userReqPncFullComIndex, userReqPncFullCom, COMM_MASTER_PARTITION_IDX );
          }
        }
#endif
      }
      else
      {
        for ( iByteMaskUserIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); iByteMaskUserIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); iByteMaskUserIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          userReqFullComIdx = ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskUserIndex );
          userReqFullCom = (ComM_UserReqFullComType)(ComM_GetUserReqFullCom( userReqFullComIdx, COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserByteMask( iByteMaskUserIndex ));

          /* @ assert userReqFullComIdx < ComM_GetSizeOfUserReqFullCom( COMM_MASTER_PARTITION_IDX ); */ /* VCA_COMM_USERREQFULLCOM_IDX */
          ComM_SetUserReqFullCom( userReqFullComIdx, userReqFullCom, COMM_MASTER_PARTITION_IDX );
        }
#if (COMM_PNC_SUPPORT == STD_ON)
        if ( ComM_HasPncPb() && ComM_IsPncUserOfUser( user ) )
        {
          for ( iByteMaskPncIndex = ComM_GetUserPncByteMaskStartIdxOfUser( user ); iByteMaskPncIndex < ComM_GetUserPncByteMaskEndIdxOfUser( user ); iByteMaskPncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            userReqPncFullComIndex = ComM_GetUserReqPncFullComIdxOfUserPncByteMask( iByteMaskPncIndex );
            userReqPncFullCom = (ComM_UserReqPncFullComType)(ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserPncByteMask( iByteMaskPncIndex ));
              
            ComM_SetUserReqPncFullCom( userReqPncFullComIndex, userReqPncFullCom, COMM_MASTER_PARTITION_IDX );
          }
        }
#endif
      }
      /* #30 Increment the synchronization counter on the master partition. */
      ComM_SetForwardUserReqSyncCounter( user, (ComM_GetForwardUserReqSyncCounter( user, userPartitionIdx )), COMM_MASTER_PARTITION_IDX );
    }
  }
  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
}

/**********************************************************************************************************************
 *  ComM_SyncBusComModeRequests()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncBusComModeRequests( CONST( NetworkHandleType, AUTOMATIC ) channel, P2CONST( ComM_StateType, AUTOMATIC, COMM_CONST ) lHighestCalculatedBusComMode )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( channel );

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the bus-site requested communication mode for this channel has changed on a satellite partition: */
  if ( ComM_GetBusComModeReqSyncCounter( channel, partitionIdx ) == (uint8)(ComM_GetBusComModeReqSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
  {
    if ( !ComM_Channel_HasNmSupportOrBusFrOrLinslave() )
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();
    }

    /* #20 It has to be ensured that a wakeup event is not overwritten by a shutdown event. This means, if the channel was woken up on a satellite
     *     (bus requested FULL_COM, i.e. WakeupStateOfChannel), the wakeup state shall not be overwritten. */
    if ( lHighestCalculatedBusComMode[channel] != ComM_GetWakeupStateOfChannel( channel ) )
    {
      /* #30 Update the sync counter and the requested bus communication mode on the master partition. */
      ComM_SetBusComModeReq( channel, ComM_GetBusComModeReq( channel, partitionIdx ), COMM_MASTER_PARTITION_IDX );
      if ( ComM_GetManagingChannelIdOfChannel( channel ) != COMM_NO_MANAGINGCHANNELIDOFCHANNEL )
      {
        if ( (ComM_GetActiveComMode( ComM_GetManagingChannelIdOfChannel( channel ), COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
        {
          ComM_SetBusComModeReq( ComM_GetManagingChannelIdOfChannel( channel ), ComM_GetWakeupStateOfChannel( ComM_GetManagingChannelIdOfChannel( channel ) ), COMM_MASTER_PARTITION_IDX );
        }
      }
    }
    ComM_SetBusComModeReqSyncCounter( channel, (ComM_GetBusComModeReqSyncCounter( channel, partitionIdx )), COMM_MASTER_PARTITION_IDX );

    if ( !ComM_Channel_HasNmSupportOrBusFrOrLinslave() )
    {
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
  }

  COMM_DUMMY_STATEMENT_CONST( lHighestCalculatedBusComMode ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_SyncSynchronousWakeUpIndications()
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
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncSynchronousWakeUpIndications( CONST( NetworkHandleType, AUTOMATIC ) channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) lHighestCalculatedBusComMode, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 syncWakeUpChannelIterator;

  /* ----- Implementation ----------------------------------------------- */
  /* Update ComM_EcuMWakeUpIndicationSyncCounter */
  /* #10 Check if an EcuM WakeUp-Indication occurred on a satellite partition. */
  if ( ComM_GetEcuMWakeUpIndicationSyncCounter( channel, partitionIdx ) == (uint8)(ComM_GetEcuMWakeUpIndicationSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
  {
    /* #100 Wake up every channel which is not in FULL_COMMUNICATION yet by setting the bus-site requested communication mode to the wakeup state of the channel. */
    for ( syncWakeUpChannelIterator = 0u; syncWakeUpChannelIterator < ComM_GetSizeOfChannel(); syncWakeUpChannelIterator++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( (ComM_GetActiveComMode( syncWakeUpChannelIterator, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
      {
        ComM_SetBusComModeReq( syncWakeUpChannelIterator, ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator ), COMM_MASTER_PARTITION_IDX );
      }
      lHighestCalculatedBusComMode[syncWakeUpChannelIterator] = ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator );
    }
    ComM_SetEcuMWakeUpIndicationSyncCounter( channel, ComM_GetEcuMWakeUpIndicationSyncCounter( channel, partitionIdx ), COMM_MASTER_PARTITION_IDX );

    /* #110 If PNC Support is enabled, activate all PNCs at least for the duration of the PNC Prepare Sleep Timer. */
#if (COMM_PNC_SUPPORT == STD_ON)
    ComM_ActivateAllPncs();
#endif
  }

  /* Update ComM_NmStartUpIndicationSyncCounter */
  /* #20 Check if an Nm WakeUp-Indication occurred on a satellite partition. */
  if ( ComM_GetNmStartUpIndicationSyncCounter( channel, partitionIdx ) == (uint8)(ComM_GetNmStartUpIndicationSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
  {
    /* #200 Wake up every channel which is not yet in FULL_COMMUNICATION by setting the bus-site requested communication mode to the wakeup state of the channel. */
    for ( syncWakeUpChannelIterator = 0u; syncWakeUpChannelIterator < ComM_GetSizeOfChannel(); syncWakeUpChannelIterator++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( ComM_IsFrShutdown( syncWakeUpChannelIterator, ComM_GetPartitionConfigIdxOfChannel( syncWakeUpChannelIterator ) ) )
      {
        /* #210 If communication shutdown is ongoing on a FlexRay channel, delay the notification until FrSM indicates NO_COM
         *      because shutdown on FlexRay shall not be interrupted.
         *      Note: the ongoing shutdown means that the channel is not in FULL_COM anymore, no need to check. */
        ComM_SetBusComModeReqFrShutdown( syncWakeUpChannelIterator, ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator ), COMM_MASTER_PARTITION_IDX );
      }
      else

      {
        if ( (ComM_GetActiveComMode( syncWakeUpChannelIterator, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
        {
          ComM_SetBusComModeReq( syncWakeUpChannelIterator, ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator ), COMM_MASTER_PARTITION_IDX );
        }
        lHighestCalculatedBusComMode[syncWakeUpChannelIterator] = ComM_GetWakeupStateOfChannel( syncWakeUpChannelIterator );
      }
    }
    ComM_SetNmStartUpIndicationSyncCounter( channel, ComM_GetNmStartUpIndicationSyncCounter( channel, partitionIdx ), COMM_MASTER_PARTITION_IDX );

    /* #22 If PNC Support is enabled, activate all PNCs at least for the duration of the PNC Prepare Sleep Timer. */
#if (COMM_PNC_SUPPORT == STD_ON)
    ComM_ActivateAllPncs();
#endif
  }

  COMM_DUMMY_STATEMENT( *lHighestCalculatedBusComMode ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

# if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  ComM_SyncBusPncComModeRequests()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 /* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SyncBusPncComModeRequests( CONST( ComM_PCPartitionConfigIdxOfPartitionIdentifiersType, AUTOMATIC ) partitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PncIterType pncIndex;
  ComM_PncSignalIterType signalIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the bus-site requested communication mode for a PNC has changed on a satellite partition: */
  for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    if ( ComM_GetBusPncComModeReqSyncCounter( pncIndex, partitionIdx ) == (uint8)(ComM_GetBusPncComModeReqMasterSyncCounter( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionIdx ) + pncIndex), COMM_MASTER_PARTITION_IDX ) + 1u) )
    {
      /* #20 If the bus-site requested communication mode has changed on a satellite partition, set the bus-site requested communication mode on the master partition accordingly. */
      if ( ComM_GetBusPncComModeReq( pncIndex, partitionIdx ) == COMM_PNC_NO_COMMUNICATION )
      {
        /* If only one EIRA RX (bus type), no need to loop over signals */
        uint8 sigValuesArb = 0u; /* PRQA S 2983 */ /* MD_ComM_2983 */
        if ( ComM_GetPncEiraRxCount( COMM_MASTER_PARTITION_IDX ) > 1u )
        {
          sigValuesArb = 0u;
          for ( signalIndex = 0u; signalIndex < ComM_GetSizeOfPncSignal(); signalIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            if ( ComM_GetTypeOfPncSignal( signalIndex ) == COMM_EIRA_RX_TYPEOFPNCSIGNAL )
            {
              sigValuesArb |= ComM_GetPncSignalValues( (uint8)(ComM_GetPncSignalValuesStartIdxOfPncSignal( signalIndex ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), COMM_MASTER_PARTITION_IDX ); /* PRQA S 2985 */ /* MD_ComM_2985 */
            }
          }
        }
        if ( (ComM_GetPncEiraRxCount( COMM_MASTER_PARTITION_IDX ) == 1u) || ((sigValuesArb & ComM_GetSetMaskOfPnc( pncIndex )) == 0u) )
        {
          ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX );
        }
      }
      else
      {
        ComM_SetBusPncComModeReq( pncIndex, ComM_GetBusPncComModeReq( pncIndex, partitionIdx ), COMM_MASTER_PARTITION_IDX );
      }

      /* #30 Update the sync counter for ComM_BusPncComModeReq. */
      if ( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionIdx ) + pncIndex) < ComM_GetBusPncComModeReqMasterSyncCounterEndIdxOfPncPartitionIndirection( partitionIdx ) )
      {
        ComM_SetBusPncComModeReqMasterSyncCounter( (ComM_GetBusPncComModeReqMasterSyncCounterStartIdxOfPncPartitionIndirection( partitionIdx ) + pncIndex), (ComM_GetBusPncComModeReqSyncCounter( pncIndex, partitionIdx )), COMM_MASTER_PARTITION_IDX );
      }
    }
  }
}
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  ComM_CheckSyncCounterAndForwardFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_CheckSyncCounterAndForwardFunction( volatile P2CONST( uint8, AUTOMATIC, COMM_CONST ) syncCounterPtr, volatile P2VAR( uint8, AUTOMATIC, COMM_APPL_VAR ) syncCounterCurrPartPtr,
                                                                 ComM_ForwardingNmServicePtrType forwardingFunction, uint8 serviceIndex, CONST( NetworkHandleType, AUTOMATIC ) networkHandle, CONST(PNCHandleType, AUTOMATIC) pncId )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasNmSupportOfChannel() )
  {
    /* #10 Check if on any partition the synchronization counter was incremented by 1 compared to the counter on the
     *     current partition (i.e. on the partition the channel belongs to). */
    if ( *syncCounterPtr == (uint8)(*syncCounterCurrPartPtr + 1u) ) /* PRQA S 0404 */ /* MD_ComM_0404 */
    {
      /* #20 Increment synchronization counter on current partition and forward call to the BusNm. */
      (*syncCounterCurrPartPtr) = (*syncCounterCurrPartPtr) + 1u;
      (void)forwardingFunction( serviceIndex, networkHandle, pncId );
    }
  }

  COMM_DUMMY_STATEMENT_CONST( syncCounterPtr ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( syncCounterCurrPartPtr ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( serviceIndex ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( networkHandle ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( pncId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_UpdateDataFromSatellites()
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
 */
 /* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
FUNC( void, COMM_CODE ) ComM_UpdateDataFromSatellites( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType partitionIdx;
  uint8 channel;
  boolean initiateReset = FALSE;
# if (COMM_PNC_SUPPORT == STD_ON)
  ComM_PncSignalValuesEndIdxOfPncSignalType signalByteIndex;
  ComM_PncIterType pncIndex;
# endif
  ComM_StateType lHighestCalculatedBusComMode[ComM_GetSizeOfChannelOfPCPartitionConfig()] = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each channel, check if an event occured on the channel related partition since last synchronization. */
  for ( channel = 0u; channel < ComM_GetSizeOfChannel(); channel++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel( channel );

    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      if ( ComM_GetGlobalPartitionSyncCounter( channel, partitionIdx ) == (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
      {
        ComM_SetGlobalPartitionSyncCounter( channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, partitionIdx ) + 1u), COMM_MASTER_PARTITION_IDX );

        /* ------- Channel Variables ---------------------------------------------- */
        /* #20 Synchronize data for all channel related variables:
         *     If on any satellite partition the synchronization counter of the corresponding variable was incremented
         *     by 1 (compared to the corresponding counter on the master partition), update the data.*/

        if ( ComM_Channel_HasNmSupportOrBusFrOrSynchronousWakeUp() )
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();
        }

        /* Update ComM_ActiveComMode */
        if ( ComM_HasNmSupportOfChannel() )
        {
          ComM_UpdateMasterSyncCounterAndStates( ComM_GetAddrActiveComModeSyncCounter( channel, partitionIdx ), ComM_GetAddrActiveComModeSyncCounter( channel, COMM_MASTER_PARTITION_IDX ),
                                                 ComM_GetAddrActiveComMode( channel, partitionIdx ), ComM_GetAddrActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) );
        }

        /* Update ComM_BusComModeReqFrShutdown */
        if ( ComM_HasBusTypeFr() )
        {
          ComM_UpdateMasterSyncCounterAndStates( ComM_GetAddrBusComModeReqFrShutdownSyncCounter( channel, partitionIdx ), ComM_GetAddrBusComModeReqFrShutdownSyncCounter( channel, COMM_MASTER_PARTITION_IDX ),
                                                 ComM_GetAddrBusComModeReqFrShutdown( channel, partitionIdx ), ComM_GetAddrBusComModeReqFrShutdown( channel, COMM_MASTER_PARTITION_IDX ) );
        }

        /* Update ComM_BusComModeReq */
        ComM_SyncBusComModeRequests( channel, lHighestCalculatedBusComMode );
        ComM_SyncSynchronousWakeUpIndications( channel, lHighestCalculatedBusComMode, partitionIdx );

        if ( ComM_Channel_HasNmSupportOrBusFrOrSynchronousWakeUp() )
        {
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
        }

        /* Update ComM_StateChange */
        if ( ComM_User_HasModeNotificationOfChannel() )
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
          ComM_UpdateMasterSyncCounterAndStates( ComM_GetAddrStateChangeSyncCounter( channel, partitionIdx ), ComM_GetAddrStateChangeSyncCounter( channel, COMM_MASTER_PARTITION_IDX ),
                                                 ComM_GetAddrStateChange( channel, partitionIdx ), ComM_GetAddrStateChange( channel, COMM_MASTER_PARTITION_IDX ) );
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
        }

        /* Update ComM_MinFullComModeTimer */
        ComM_UpdateMasterSyncCounterAndTimers( ComM_GetAddrMinFullComModeTimerSyncCounter( channel, partitionIdx ), ComM_GetAddrMinFullComModeTimerSyncCounter( channel, COMM_MASTER_PARTITION_IDX ),
                                               ComM_GetMinFullComTimeOfChannel( channel ), ComM_GetAddrMinFullComModeTimer( channel, COMM_MASTER_PARTITION_IDX ) );

        /* Update ComM_NmLightTimer */
        /* No sync needed. ComM_NmLightTimer is only accessed on master partition */

        /* Update ComM_DcmRequestActive */
        if ( ComM_Channel_HasDcmIndicationAndKeepAwakeChannels() )
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
          ComM_UpdateMasterSyncCounterAndStates( ComM_GetAddrDcmRequestActiveSyncCounter( channel, partitionIdx ), ComM_GetAddrDcmRequestActiveSyncCounter( channel, COMM_MASTER_PARTITION_IDX ),
                                                 (volatile uint8*)ComM_GetAddrDcmRequestActive( channel, partitionIdx ), (volatile uint8*)ComM_GetAddrDcmRequestActive( channel, COMM_MASTER_PARTITION_IDX ) );
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
        }

        /* Update ComM_BusSleepModeIndicated */
        if ( ComM_HasLinslaveNmTypeOfChannel() )
        {
          if ( ComM_GetBusSleepModeIndicatedSyncCounter( channel, partitionIdx ) == (uint8)(ComM_GetBusSleepModeIndicatedSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
          {
            /* No Spinlock needed, since in case of a new event on a satellite the UserReqFullCom is always set to NO_COM. */
            ComM_SetBusSleepModeIndicated( channel, TRUE, COMM_MASTER_PARTITION_IDX );
            ComM_SetBusSleepModeIndicatedSyncCounter( channel, (ComM_GetBusSleepModeIndicatedSyncCounter( channel, partitionIdx )), COMM_MASTER_PARTITION_IDX );
          }
        }

        /* Update ComM_FrShutdown */
        /* No sync needed. ComM_FrShutdown is only written on satellite partition. */

        /* Update ComM_LastStateChange */
        /* No sync needed. ComM_LastStateChange is only accessed on master partition. */

        /* Update ComM_NvMStoreReq */
        /* No sync needed. Current restriction only allows access on master partition. */

        /* Update ComM_CommunicationAllowed */
        /* No sync needed. Current restriction only allows access on master partition. */

        /* Update ComM_Inhibition */
        /* No sync needed. Current restriction only allows write access on master partition */

        /* Update ComM_FullComRequesters */
        /* No sync needed. Current restriction only allows write access on master partition */


        /* ------- Partial Network Variables ------------------------------------- */
        /* #30 Synchronize data for all partial network related variables:
         *     If on any satellite partition the synchronization counter of the corresponding variable was incremented
         *     by 1 (compared to the corresponding counter on the master partition), update the data.*/

# if (COMM_PNC_SUPPORT == STD_ON)
        /* Update ComM_PncToChannelRoutingState */
        if ( ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX ) )
        {
          if ( ComM_GetPncToChannelRoutingStateSyncCounter( channel, partitionIdx ) == (uint8)(ComM_GetPncToChannelRoutingStateSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
          {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
            if ( (ComM_GetPncToChannelRoutingState( channel, partitionIdx ) & COMM_PNCROUTING_NM_REPEAT_MESSAGE) != 0x00u )
            {
              ComM_SetPncToChannelRoutingState( channel, ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) | COMM_PNCROUTING_NM_REPEAT_MESSAGE, COMM_MASTER_PARTITION_IDX );
            }
            else
            {
              ComM_SetPncToChannelRoutingState( channel, ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NM_REPEAT_MESSAGE_CLEAR, COMM_MASTER_PARTITION_IDX );
            }
            ComM_SetPncToChannelRoutingStateSyncCounter( channel, (ComM_GetPncToChannelRoutingStateSyncCounter( channel, partitionIdx )), COMM_MASTER_PARTITION_IDX );
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
          }
        }

        /* Update ComM_ForwardSyncPncShutdown. */
        if ( ComM_HasIntermediatePncCoordinatorOfPnc() )
        {
          if ( ComM_GetGwTypeOfChannel( channel ) == COMM_GATEWAY_TYPE_PASSIVE )
          {
            for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
            {
              /* If the Forward Synchronized PNC Shutdown indication has changed on a satellite partition, set the Forward Synchronized PNC Shutdown indication on the master partition accordingly. */
              if ( ComM_GetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, partitionIdx ) == (uint8)(ComM_GetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, COMM_MASTER_PARTITION_IDX ) + 1u) )
              {
                ComM_SetForwardSynchronizedPncShutdownIndicated( pncIndex, TRUE, COMM_MASTER_PARTITION_IDX );
                ComM_SetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, (ComM_GetForwardSyncPncShutdownIndicatedSyncCounter( pncIndex, partitionIdx )), COMM_MASTER_PARTITION_IDX );
              }
            }
          }
        }

        /* Update ComM_NotifyPncStateChange */
        /* No sync needed. ComM_NotifyPncStateChange is only accessed on master partition. */

        /* Update ComM_ActivePncComMode */
        /* No sync needed. ComM_ActivePncComMode is only written on master partition. */

        /* Update ComM_PncPSleepTimer */
        /* No sync needed. ComM_PncPSleepTimer is only accessed on master partition. */

# endif
      } /* END: ComM_GlobalPartitionSyncCounter != (ComM_GlobalPartitionSyncCounter + 1) */

        /* Update ComM_BusSmState */
        /* Sync is only needed for BswM mode notification to be done on master partition. ComM_BusSmState is only written on satellite partition. */
      if ( ComM_GetBusSmState( channel, COMM_MASTER_PARTITION_IDX ) != ComM_GetBusSmState( channel, partitionIdx ) )
      {
        /* No Spinlock needed, since value is directly read from the satellite partition (no synchronization needed). */
        ComM_SetBusSmState( channel, ComM_GetBusSmState( channel, partitionIdx ), COMM_MASTER_PARTITION_IDX );
        BswM_ComM_CurrentMode( channel, ComM_GetBusSmState( channel, COMM_MASTER_PARTITION_IDX ) );
      }

    } /* END: partitionIdx != COMM_MASTER_PARTITION_IDX */
  } /* END: for-loop */

  /* Update ComM_UserReqFullCom and ComM_UserReqPncFullCom */
  ComM_SyncUserFullComRequests();

  for ( partitionIdx = 0u; partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); partitionIdx++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
# if (COMM_PNC_SUPPORT == STD_ON)
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_4();
      /* Update ComM_PncSignalValues */
      for ( signalByteIndex = 0u; signalByteIndex < ComM_GetSizeOfPncSignalValues( COMM_MASTER_PARTITION_IDX ); signalByteIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        if ( ComM_GetPncSignalValuesSyncCounter( signalByteIndex, partitionIdx ) == (uint8)(ComM_GetPncSignalValuesSyncCounter( signalByteIndex, COMM_MASTER_PARTITION_IDX ) + 1u) )
        {
          ComM_SetPncSignalValues( signalByteIndex, ComM_GetPncSignalValues( signalByteIndex, partitionIdx ), COMM_MASTER_PARTITION_IDX );
          ComM_SetPncSignalValuesSyncCounter( signalByteIndex, ComM_GetPncSignalValuesSyncCounter( signalByteIndex, partitionIdx ), COMM_MASTER_PARTITION_IDX );
        }
      }

      /* Update ComM_BusPncComModeReq */
      ComM_SyncBusPncComModeRequests( partitionIdx );
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_4();
# endif

      /* Update ComM_ResetRequired */
      if ( ComM_HasResetAfterForcingNoCommOfChannel() )
      {
        if ( ComM_GetInitiateResetSyncCounter( partitionIdx ) == (uint8)(ComM_GetInitiateResetSyncCounter( COMM_MASTER_PARTITION_IDX ) + 1u) ) /*lint !e564 */
        {
          /* No Spinlock needed, since in case of a new event on a satellite only the sync counter is updated and a local flag is set. */
          ComM_SetInitiateResetSyncCounter( (ComM_GetInitiateResetSyncCounter( partitionIdx )), COMM_MASTER_PARTITION_IDX );
          initiateReset = TRUE;
        }
      }
    }
  }

  if ( ComM_HasResetAfterForcingNoCommOfChannel() )
  {
    if ( initiateReset == TRUE )
    {
      for ( channel = 0u; channel < ComM_GetSizeOfChannel(); channel++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        /* Reset the flag in case init function is not called due to BswM_ComM_InitiateReset(). */
        ComM_SetResetRequired( channel, FALSE, COMM_MASTER_PARTITION_IDX );
      }
      BswM_ComM_InitiateReset();
    }
  }

  COMM_DUMMY_STATEMENT( initiateReset ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_IsRequestPendingOnSatellite()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_IsRequestPendingOnSatellite( CONST( NetworkHandleType, AUTOMATIC ) channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) busComModeReq
                                                                          , P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) busComModeReqFrShutdown)
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( channel );

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a wakeup event occurred on a satellite partition and set the value of the provided busComModeReq and busComModeReqFrShutdown accordingly. */
  if ( ComM_HasBusTypeFr() )
  {
    if ( ComM_GetBusComModeReqFrShutdownSyncCounter( channel, partitionIdx ) == (uint8)(ComM_GetBusComModeReqFrShutdownSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
    {
      *busComModeReqFrShutdown = ComM_GetBusComModeReqFrShutdown( channel, partitionIdx );
    }
    else
    {
      *busComModeReqFrShutdown = ComM_GetBusComModeReqFrShutdown( channel, COMM_MASTER_PARTITION_IDX );
    }
  }

  if ( (ComM_Channel_HasNmSupportOrBusFrOrLinslave() || !ComM_HasSynchronousWakeUp()) &&
       (ComM_GetBusComModeReqSyncCounter( channel, partitionIdx ) == (uint8)(ComM_GetBusComModeReqSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u)) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    *busComModeReq = ComM_GetBusComModeReq( channel, partitionIdx );
  }
  else
  {
    *busComModeReq = ComM_GetBusComModeReq( channel, COMM_MASTER_PARTITION_IDX );
  }

  if ( ComM_HasSynchronousWakeUp() )
  {
    NetworkHandleType channelIterator;

    for ( channelIterator = 0u; channelIterator < ComM_GetSizeOfChannel(); channelIterator++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( (ComM_GetEcuMWakeUpIndicationSyncCounter( channelIterator, ComM_GetPartitionConfigIdxOfChannel( channelIterator ) ) == (uint8)(ComM_GetEcuMWakeUpIndicationSyncCounter( channelIterator, COMM_MASTER_PARTITION_IDX ) + 1u)) ||
           (ComM_GetNmStartUpIndicationSyncCounter( channelIterator, ComM_GetPartitionConfigIdxOfChannel( channelIterator ) ) == (uint8)(ComM_GetNmStartUpIndicationSyncCounter( channelIterator, COMM_MASTER_PARTITION_IDX ) + 1u)) /* PRQA S 3415 */ /* MD_ComM_3415 */
           )
      {
        *busComModeReq = ComM_GetWakeupStateOfChannel( channel );
        break;
      }
    }
  }

  COMM_DUMMY_STATEMENT( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( busComModeReqFrShutdown ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}
#endif /* COMM_MULTIPARTITION == STD_ON */


#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
