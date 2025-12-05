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
/*!         \file     ComM_ModeInhibition.c
 *          \unit     ModeInhibition
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the ModeInhibition unit.
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

#define CCL_ASR_COMM_MODEINHIBITION_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_ModeInhibition.h"
#include "BswM_ComM.h"
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

/**********************************************************************************************************************
  ComM_PreventWakeUpActive()
 *********************************************************************************************************************/
/*! \brief      Determines whether Prevent Wake-up is active on the channel.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \return     TRUE                    Prevent Wake-up is active on the channel
 *  \return     FALSE                   otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *  \trace      SPEC-5140, SPEC-5131
 *********************************************************************************************************************/
COMM_LOCAL FUNC( boolean, COMM_CODE ) ComM_PreventWakeUpActive( NetworkHandleType channel );

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_PreventWakeUpActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
COMM_LOCAL FUNC( boolean, COMM_CODE ) ComM_PreventWakeUpActive( NetworkHandleType channel )
{
  /* The function is used in conditions, which are not always evaluated because of 'short-circuit evaluation'.
  * Therefore the function is not allowed to change the module' state i.e. write global variables.
  */

  /* ----- Local Variable ---------------------------------------------- */
  boolean retValue = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return TRUE if Prevent Wake-up
   *     1) is allowed by ECU Group Classification and
   *     2) is active on the channel and
   *     3) the channel is not in FULL_COM mode. */
  

  if ( (ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX )->ComM_ECUGroupClassification & COMM_WAKEUP_INHIBITION_MASK) == COMM_WAKEUP_INHIBITION_MASK )
  {
    /* check if prevent wake up is active */
    if ( (ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX )->ComM_InhibitionStatus[channel] & COMM_WAKEUP_INHIBITION_MASK) == COMM_WAKEUP_INHIBITION_MASK )
    {
      if ( (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
      {
        /* The inhibition shall not get active in FULL_COM mode */
        retValue = TRUE;
      }
    }
  }

  return retValue;
}


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ComM_ModeInhibition_GetInhibitionStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_ModeInhibition_GetInhibitionStatus( NetworkHandleType Channel, P2VAR( ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_VAR ) Status )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store the inhibition status of the channel. */
  *Status = ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX )->ComM_InhibitionStatus[Channel];
}

/*********************************************************************************************************************
 *  ComM_ModeInhibition_PreventWakeUp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_ModeInhibition_PreventWakeUp( NetworkHandleType Channel, boolean Status )
{
  /* ----- Local Variable ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasWakeupInhibition() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_InhibitionPtrType ComMInhibition = ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX );

    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();
    if ( Status == TRUE )
    {
      /* #10 Only accept the request if ECU Group Classification supports Prevent Wake-up. */
      /* ESCAN00068902 */
      if ( (ComMInhibition->ComM_ECUGroupClassification & COMM_WAKEUP_INHIBITION_MASK) == COMM_WAKEUP_INHIBITION_MASK )
      {
        ComMInhibition->ComM_InhibitionStatus[Channel] |= COMM_WAKEUP_INHIBITION_MASK;
        ComM_SetNvMStoreReq( TRUE, COMM_MASTER_PARTITION_IDX );
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* #20 Always accept releasing the Prevent Wake-up. */
      ComMInhibition->ComM_InhibitionStatus[Channel] &= COMM_WAKEUP_INHIBITION_CLEAR_MASK;
      ComM_SetNvMStoreReq( TRUE, COMM_MASTER_PARTITION_IDX );
    }

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
  }
  else
  {
    /* #30 Reject the request if the feature Prevent Wake-up is disabled. */
    /* ESCAN00068896 */
    retVal = E_NOT_OK;
  }

  COMM_DUMMY_STATEMENT( Status );  /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_ModeInhibition_LimitChannelToNoComMode()
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
FUNC( Std_ReturnType, COMM_CODE ) ComM_ModeInhibition_LimitChannelToNoComMode( NetworkHandleType Channel, boolean Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the feature Mode Limitation to NO_COM is enabled and
  *      at least one channel has Nm Type other than PASSIVE, process the request. */
  if ( ComM_HasModeLimitation() && ComM_HasKeepAwakeChannels() ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_InhibitionPtrType ComMInhibition = ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX );

    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

    if ( Status == TRUE )
    {
      /* #20 Accept a request for Mode Limitation to NO_COM if
       *     1) the channel has Nm Type other than NONE (because ComM does not shutdown communication on NONE) and
       *     2) ECU Group Classification supports Mode Limitation NO_COM. */
      /* ESCAN00068896 */ /* ESCAN00106344 */
      if ( ComM_GetNmTypeOfChannel( Channel ) != COMM_NONE_NMTYPEOFCHANNEL )
      {
        if ( ((ComMInhibition->ComM_ECUGroupClassification) & (COMM_MODE_LIMITATION_MASK)) == COMM_MODE_LIMITATION_MASK )
        {
          ComMInhibition->ComM_InhibitionStatus[Channel] |= COMM_MODE_LIMITATION_MASK;
          retVal = E_OK;
          if ( ComM_HasResetAfterForcingNoCommOfChannel() )
          {
            if ( ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_NETWORK_REQUESTED )
            {
              ComM_SetResetRequired( Channel, TRUE, COMM_MASTER_PARTITION_IDX );
            }
          }
        }
      }
    }
    else
    {
      /* #30 Accept releasing the Mode Limitation to NO_COM if the channel has Nm Type other than PASSIVE. */
      /* Note: a possible use case for NONE is when limitation has been set initially in the configuration. */
      if ( ComM_GetNmTypeOfChannel( Channel ) != COMM_PASSIVE_NMTYPEOFCHANNEL )
      {
        ComMInhibition->ComM_InhibitionStatus[Channel] &= COMM_MODE_LIMITATION_CLEAR_MASK;
        retVal = E_OK;
        if ( ComM_HasResetAfterForcingNoCommOfChannel() )
        {
          ComM_SetResetRequired( Channel, FALSE, COMM_MASTER_PARTITION_IDX );
        }
      }
    }
    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();
  }
  /* #40 Reject the request if the feature Mode Limitation to NO_COM is disabled or all channels have Nm Type PASSIVE. */
  /* ESCAN00068896 */

  COMM_DUMMY_STATEMENT( Status ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_ModeInhibition_LimitECUToNoComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_ModeInhibition_LimitECUToNoComMode( boolean Status )
{
  /* ----- Local Variable ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the feature Mode Limitation to NO_COM is enabled and
  *     at least one channel has Nm Type other than PASSIVE and at least one channel has Nm Type other than NONE, request mode limitation for each channel. */
  if ( ComM_Channel_HasModeLimitationAndKeepAwakeAndNoNoneNmType() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    NetworkHandleType ComM_ChannelIndex;

    retVal = E_OK;
    for ( ComM_ChannelIndex = 0u; ComM_ChannelIndex < ComM_GetSizeOfChannel(); ComM_ChannelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( !ComM_IsInvalidHndOfChannel( ComM_ChannelIndex ) )
      {
        if ( ComM_LimitChannelToNoComMode( ComM_ChannelIndex, Status ) != E_OK )
        {
          /* #20 If the request failed for at least one channel, the overall status shall be E_NOT_OK as well. */
          /* ESCAN00068896 */
          retVal = E_NOT_OK;
        }
      }
    }
  }
  else
  {
    /* #30 Reject the request if the feature Mode Limitation to NO_COM is disabled or all channels have Nm Type PASSIVE or all channels have Nm Type NONE. */
    /* (ESCAN00068896) */
    retVal = E_NOT_OK;
  }

  COMM_DUMMY_STATEMENT( Status ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_ReadInhibitCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_ModeInhibition_ReadInhibitCounter( P2VAR( uint16, AUTOMATIC, COMM_APPL_VAR ) CounterValue )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store the inhibition counter. */
  *CounterValue = ComM_GetInhibition( COMM_MASTER_PARTITION_IDX ).ComM_InhibitCnt;
}

/*********************************************************************************************************************
 *  ComM_ModeInhibition_ResetInhibitCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_ModeInhibition_ResetInhibitCounter( void )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_InhibitionPtrType ComMInhibition = ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX );

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the inhibition counter to 0 and mark NvM data to be stored if NvM support is enabled. */
  ComMInhibition->ComM_InhibitCnt = (uint16)0x00;
  ComM_SetNvMStoreReq( TRUE, COMM_MASTER_PARTITION_IDX );
}


/*********************************************************************************************************************
 *  ComM_ModeInhibition_SetECUGroupClassification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_ModeInhibition_SetECUGroupClassification( ComM_InhibitionStatusType Status )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_InhibitionPtrType ComMInhibition = ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX );

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set ECU group classification and mark NvM data to be stored if NvM is supported. */
  ComMInhibition->ComM_ECUGroupClassification = Status;
  ComM_SetNvMStoreReq( TRUE, COMM_MASTER_PARTITION_IDX );
}

/*********************************************************************************************************************
 *  ComM_NoComLimitationActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( boolean, COMM_CODE ) ComM_NoComLimitationActive( NetworkHandleType channel )
{
  /* The function is used in conditions, which are not always evaluated because of 'short-circuit evaluation'.
   * Therefore the function is not allowed to change the module' state i.e. write global variables. */

  /* ----- Local Variable ---------------------------------------------- */
  boolean retValue = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return TRUE if Mode Limitation to NO_COM
   *     1) is active because of NmType PASSIVE or
   *     2) is allowed by ECU Group Classification and
   *     3) is active on the channel */

  if ( ComM_GetNmTypeOfChannel( channel ) == COMM_PASSIVE_NMTYPEOFCHANNEL )
  {
    retValue = TRUE;
  }
  else
  {
    if ( (ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX )->ComM_ECUGroupClassification & COMM_MODE_LIMITATION_MASK) == COMM_MODE_LIMITATION_MASK )
    {
      if ( (ComM_GetAddrInhibition( COMM_MASTER_PARTITION_IDX )->ComM_InhibitionStatus[channel] & COMM_MODE_LIMITATION_MASK) == COMM_MODE_LIMITATION_MASK )
      {
        retValue = TRUE;
      }
    }
  }

  return retValue;
}

/**********************************************************************************************************************
 *  ComM_ChannelIsLimited()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( boolean, COMM_CODE ) ComM_ChannelIsLimited( NetworkHandleType channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  boolean retValue = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return TRUE if Mode Limitation or Prevent Wake-up is active on the channel.
   *     Otherwise return FALSE. */
  if ( (ComM_NoComLimitationActive( channel ) == TRUE) || (ComM_PreventWakeUpActive( channel ) == TRUE) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    retValue = TRUE;
  }

  return retValue;
}

/**********************************************************************************************************************
 *  ComM_ChannelResetAfterForcingNoComm()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 /* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
FUNC( void, COMM_CODE ) ComM_ChannelResetAfterForcingNoComm( CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  if ( ComM_HasResetAfterForcingNoCommOfChannel() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_SizeOfChannelType channelIndex;
    boolean allChannelsAreInNoComm = TRUE;
    boolean aChannelNeedsResetAfterForcingNoComm = FALSE;
#if ( COMM_MULTIPARTITION == STD_ON )
    ComM_StateType activeComMode;
#endif

    /* ----- Implementation ----------------------------------------------- */
    for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( ComM_GetNmTypeOfChannel( channelIndex ) != COMM_NONE_NMTYPEOFCHANNEL )
      {
        /* #10 Check for each channel except for NM variant NONE: BusSm indicated NO_COM and the channel is in NO_COM (ignore possible passive wake-ups). */
#if ( COMM_MULTIPARTITION == STD_ON )
        activeComMode = ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX );
        if ( ComM_HasNmSupportOfChannel() )
        {
          /* Check if an event occurred on a satellite which has changed the active com mode since last master sync. */
          /* No spinlock needed, since for any event on the satellite partition the channel state is written BEFORE the sync counter.
          * The value is always updated in the beginning of the master main function. In the worst case - where the value on the satellite
          * has changed but the sync counter is not yet increased - this value is read from the master partition. It is not possible that a
          * satellite event sets the ActiveComMode to a FULL_COM state, i.e. if the ActiveComMode is already NO_COM on the master, the satellite
          * cannot change to higher than NO_COM.
          */
          if ( ComM_GetActiveComModeSyncCounter( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) ) == (uint8)(ComM_GetActiveComModeSyncCounter( channelIndex, COMM_MASTER_PARTITION_IDX ) + 1u) )
          {
            activeComMode = ComM_GetActiveComMode( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) );
          }
        }
        if ( (ComM_GetBusSmState( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) ) != COMM_NO_COMMUNICATION) || (activeComMode > COMM_NO_COM_REQUEST_PENDING) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
#else
        if ( (ComM_GetBusSmState( channelIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_COMMUNICATION) || (ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX ) > COMM_NO_COM_REQUEST_PENDING) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
#endif
        {
          allChannelsAreInNoComm = FALSE;
        }

        /* #20 Check that at least one channel has an active Mode Limitation and has Reset After Forcing NoCom enabled. */
        if ( ComM_IsResetAfterForcingNoCommOfChannel( channelIndex ) ) /* Needed only for channels with Nm PASSIVE */
        {
          /* #200 The reset shall not occur in case the mode limitation is not actually activated (was not forced from COMM_FULL_COMM_NETWORK_REQUESTED to COMM_FULL_COMM_READY_SLEEP)*/
          if ( (ComM_NoComLimitationActive( channelIndex ) == TRUE) && (ComM_IsResetRequired( channelIndex, COMM_MASTER_PARTITION_IDX )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
          {
            aChannelNeedsResetAfterForcingNoComm = TRUE;
          }
        }
      }
    }

    /* #30 If the conditions above are true, call BswM_ComM_InitiateReset(). */
    if ( allChannelsAreInNoComm == TRUE )
    {
#if ( COMM_MULTIPARTITION == STD_ON )
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        if ( aChannelNeedsResetAfterForcingNoComm == TRUE )
        {
          /* Increment sync counter on current satellite partition only. BswM notification about InitiateReset and Synchronization to other channels is done in next master main function. */
          ComM_SetInitiateResetSyncCounter( (uint8)(ComM_GetInitiateResetSyncCounter( COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        }
      }
      else
#endif
      {
        for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          ComM_SetResetRequired( channelIndex, FALSE, partitionIdx );
        }
        if ( aChannelNeedsResetAfterForcingNoComm == TRUE )
        {
#if (COMM_RESETAFTERFORCINGNOCOMMOFCHANNEL == STD_ON)
          BswM_ComM_InitiateReset();
#endif
        }
      }
    }
  }

  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}


#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
