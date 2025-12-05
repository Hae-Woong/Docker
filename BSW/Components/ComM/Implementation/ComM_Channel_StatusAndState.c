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
/*!         \file    ComM_Channel_StatusAndState.c
 *          \unit    Channel_StatusAndState
 *          \brief   Communication Manager ASR4
 *
 *         \details  Implementation of the MICROSAR Communication Manager module for the Channel_StatusAndState unit.
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

#define CCL_ASR_COMM_CHANNEL_STATUSANDSTATE_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_Channel_StatusAndState.h"
#include "ComM_PNC_General.h"
#include "ComM_ModeInhibition.h"
#include "ComM_Private_Nm.h"
#include "SchM_ComM.h"
#if (COMM_MULTIPARTITION == STD_ON)
# include "ComM_MultiPartitionSynchronization.h"
#endif

#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
# include "CanSM.h"
# include "CanSM_ComM.h"
#endif

#include "ComM_Util.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
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
 ComM_IsChannelUserRequestingChannel()
 *********************************************************************************************************************/
/*! \brief      Determines if a channel user is requesting the channel.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \param[in]  partitionIdx            identification of the internal partition index
 *  \return     TRUE                    a channel user requests FULL_COM or a PNC user requests FULL_COM (if PNC coordination is disabled).
 *  \return     FALSE                   otherwise.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsChannelUserRequestingChannel( NetworkHandleType channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

#if (COMM_MULTIPARTITION == STD_ON)
/**********************************************************************************************************************
  ComM_IsUserRequestingChannelOnSatellite()
 **********************************************************************************************************************/
 /*! \brief      Determines if a user on a satellite partition is requesting the provided channel.
 *  \details    -
 *  \param[in]  channel                 local channel handle
 *  \return     TRUE                    a user on a specific partition index is requesting FULL_COM for the given channel
 *  \return     FALSE                   otherwise.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserRequestingChannelOnSatellite( NetworkHandleType channel );

# if(COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
  ComM_IsUserRequestingPncOnSatellite()
 **********************************************************************************************************************/
 /*! \brief      Determines if a user on a satellite partition is requesting the provided PNC.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \return     TRUE                    a user on a specific partition index is requesting FULL_COM for the given PNC
 *  \return     FALSE                   otherwise.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserRequestingPncOnSatellite( ComM_PncIterType pncIndex );
# endif
#endif

/**********************************************************************************************************************
 ComM_ChannelIsRequestPending()
 *********************************************************************************************************************/
/*! \brief      Determines if a pending communication request exists on the channel. The function checks all communication
 *              requests that can occur in NO_COM mode: user requests and passive wake-ups. EIRA/ERA and Dcm requests are
 *              not relevant because they can occur only if communication is on.
 *  \details    The function is used by EcuM fixed and by BswM to evaluate the channel state before a sleep/shutdown.
 *  \param[in]  channel                 local channel handle
 *  \return     TRUE                    a passive wake-up occurred or a channel or PNC is requesting FULL_COM
 *  \return     FALSE                   otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_ChannelIsRequestPending( NetworkHandleType channel );

/**********************************************************************************************************************
 *  ComM_IsChannelUserRequestingChannel()
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
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsChannelUserRequestingChannel( NetworkHandleType channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isUserRequesting = FALSE;
  ComM_UserReqFullComEndIdxOfChannelPbType userReqFullComIndex;
#if ( COMM_PNC_SUPPORT == STD_ON)
  ComM_UserByteMaskEndIdxOfUserType userByteMaskIndex;
  ComM_SizeOfUserType user;
#if (COMM_MULTIPARTITION == STD_ON)
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType userPartitionIdx;
#endif
#endif

  /* #10 PNC Coordinator is enabled and at least one channel or PNC user is requesting FULL_COM:
   *     Return TRUE if the channel has no PNCs assigned, otherwise iterate over channel users to check if one of them is requesting FULL_COM.
   *     Note that PNC users are handled separately because of dependency to the Gateway Type of channel. */

   /* #20 PNC Coordinator is disabled and at least one channel or PNC user is requesting FULL_COM:
    *     Return TRUE, because there is no dependency to the Gateway Type of channel. */

  for ( userReqFullComIndex = ComM_GetUserReqFullComStartIdxOfChannelPb( channel ); userReqFullComIndex < ComM_GetUserReqFullComEndIdxOfChannelPb( channel ); userReqFullComIndex++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    if ( (ComM_GetUserReqFullCom( userReqFullComIndex, partitionIdx ) != COMM_NO_REQUESTS) && (isUserRequesting == FALSE) )
    {
      /* At least one user assigned to this channel requests FULL_COM. The 2nd condition is because there is no downgrade of highest request. */
#if ( COMM_PNC_SUPPORT == STD_ON)
      if ( ComM_HasCoordinatedOfPnc() )
      {
        /* there are some channels without PNCs */
        if ( !ComM_IsPncPbIndUsedOfChannelPb( channel ) ) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
        {
          isUserRequesting = TRUE;
        }
        else
        {
          for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            if ( !ComM_IsPncUserOfUser( user ) )
            {
              for ( userByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); userByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); userByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 0771 */ /* MD_ComM_0771 */
              {
                if ( ComM_GetChannelOfUserByteMask( userByteMaskIndex ) == channel )
                {
#if (COMM_MULTIPARTITION == STD_ON)
                  userPartitionIdx = (ComM_PCPartitionConfigIdxOfPartitionIdentifiersType)ComM_GetPartitionConfigIdxOfUser( user );
                  if ( ComM_GetForwardUserReqSyncCounter( user, userPartitionIdx ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
                  {
                    if ( (ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), userPartitionIdx ) &
                      ComM_GetSetMaskOfUserByteMask( userByteMaskIndex )) != COMM_NO_REQUESTS )
                    {
                      isUserRequesting = TRUE;
                      break;
                    }
                  }
                  else
#endif
                  {
                    if ( (ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), COMM_MASTER_PARTITION_IDX ) &
                      ComM_GetSetMaskOfUserByteMask( userByteMaskIndex )) != COMM_NO_REQUESTS )
                    {
                      isUserRequesting = TRUE;
                      break;
                    }
                  }
                }
              }
            }
          }
        }
      }
      else
#endif
      {
        isUserRequesting = TRUE;
      }
    }
  }

  return isUserRequesting;
}

#if (COMM_MULTIPARTITION == STD_ON)
/**********************************************************************************************************************
 *  ComM_IsUserRequestingChannelOnSatellite()
 *********************************************************************************************************************/
 /*!
 *
 * Internal comment removed.
 *
 *
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserRequestingChannelOnSatellite( NetworkHandleType channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isRequestPending = FALSE;
  ComM_SizeOfUserType user;
  ComM_UserByteMaskStartIdxOfUserType userByteMaskIndex;
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType userPartitionIdx;

  /* ----- Implementation ---------------------------------------------- */
  for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    for ( userByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); userByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); userByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 0771 */ /* MD_ComM_0771 */
    {
      /* #10 Check if a user request event occurred on a satellite partition for the provided channel and return the pending request status accordingly. */
      userPartitionIdx = (ComM_PCPartitionConfigIdxOfPartitionIdentifiersType)ComM_GetPartitionConfigIdxOfUser( user );
      if ( ComM_GetChannelOfUserByteMask( userByteMaskIndex ) == channel )
      {
        if ( ComM_GetForwardUserReqSyncCounter( user, userPartitionIdx ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
        {
          if ( ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), userPartitionIdx ) != COMM_NO_REQUESTS )
          {
            isRequestPending = TRUE;
            break;
          }
        }
        else
        {
          if ( ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( userByteMaskIndex ), COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS )
          {
            isRequestPending = TRUE;
            break;
          }
        }
      }
    }
    if ( isRequestPending == TRUE )
    {
      break;
    }
  }

  COMM_DUMMY_STATEMENT_CONST( channel );

  return isRequestPending;
}

# if( COMM_PNC_SUPPORT == STD_ON )
/**********************************************************************************************************************
 *  ComM_IsUserRequestingPncOnSatellite()
 *********************************************************************************************************************/
 /*!
 *
 * Internal comment removed.
 *
 *
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_IsUserRequestingPncOnSatellite( ComM_PncIterType pncIndex )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_UserReqPncFullComEndIdxOfPncPbType userReqPncFullComIndex;
  ComM_SizeOfUserType user;
  boolean isRequestPending = FALSE;
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType userPartitionIdx;

  /* ----- Implementation ---------------------------------------------- */
  for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
  {
    for ( userReqPncFullComIndex = ComM_GetUserReqPncFullComStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 0771 */ /* MD_ComM_0771 */
      userReqPncFullComIndex < ComM_GetUserReqPncFullComEndIdxOfPncPb( pncIndex );
      userReqPncFullComIndex++ )
    {
      if ( userReqPncFullComIndex == ComM_GetUserReqPncFullComIdxOfUserPncByteMask( ComM_GetUserPncByteMaskStartIdxOfUser( user ) ) )
      {
        /* #10 Check if a user request event occurred on a satellite partition for the provided PNC and return the pending request status accordingly. */
        userPartitionIdx = (ComM_PCPartitionConfigIdxOfPartitionIdentifiersType)ComM_GetPartitionConfigIdxOfUser( user );
        if ( ComM_GetForwardUserReqSyncCounter( user, userPartitionIdx ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
        {
          if ( ComM_GetUserReqPncFullCom( userReqPncFullComIndex, userPartitionIdx ) != COMM_NO_REQUESTS )
          {
            isRequestPending = TRUE;
            break;
          }
        }
        else
        {
          if ( ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS )
          {
            isRequestPending = TRUE;
            break;
          }
        }
      }
    }
    if ( isRequestPending == TRUE )
    {
      break;
    }
  }

  COMM_DUMMY_STATEMENT_CONST( pncIndex );

  return isRequestPending;
}
# endif
#endif

/**********************************************************************************************************************
 *  ComM_ChannelIsRequestPending()
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
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_ChannelIsRequestPending( NetworkHandleType channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isRequestPending = FALSE;
#if(COMM_MULTIPARTITION == STD_OFF)
  ComM_UserReqFullComEndIdxOfChannelPbType userReqFullComIndex;
#endif
  ComM_StateType busComModeReqFrShutdown = COMM_NO_COM_NO_PENDING_REQUEST;
  ComM_StateType busComModeReq;
#if( COMM_PNC_SUPPORT == STD_ON )
  ComM_PncIterType pncIndex;
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PCPartitionConfigIdxOfPartitionIdentifiersType partitionIdx;
  ComM_SizeOfUserType user;
# else
  ComM_UserReqPncFullComEndIdxOfPncPbType userReqPncFullComIndex;
# endif
#endif

  /* #10 If a passive wake-up occurred on the channel, return a pending request. */
#if ( COMM_MULTIPARTITION == STD_ON )
  ComM_IsRequestPendingOnSatellite( channel, &busComModeReq, &busComModeReqFrShutdown );
#else
  if ( ComM_HasBusTypeFr() )
  {
    busComModeReqFrShutdown = ComM_GetBusComModeReqFrShutdown( channel, COMM_MASTER_PARTITION_IDX );
  }
  busComModeReq = ComM_GetBusComModeReq( channel, COMM_MASTER_PARTITION_IDX );
#endif
  if ( busComModeReq != COMM_NO_COM_NO_PENDING_REQUEST )
  {
    isRequestPending = TRUE;
  }
  /* #20 If a passive wake-up occurred on a FlexRay channel during shutdown, return a pending request. */
  else if ( busComModeReqFrShutdown != COMM_NO_COM_NO_PENDING_REQUEST )
  {
    isRequestPending = TRUE;
  }
  /* #30 If at least one user is requesting the channel and the channel is not limited, return a pending request. */
  else
  {
    if ( ComM_HasKeepAwakeChannels() )
    {
      if ( ComM_ChannelIsLimited( channel ) == FALSE )
      {
#if ( COMM_MULTIPARTITION == STD_ON )
        isRequestPending = ComM_IsUserRequestingChannelOnSatellite( channel );
#else
        for ( userReqFullComIndex = ComM_GetUserReqFullComStartIdxOfChannelPb( channel ); /* FETA_COMM_CSL_USE_CASE */
              userReqFullComIndex < ComM_GetUserReqFullComEndIdxOfChannelPb( channel );
              userReqFullComIndex++ )
        {
          if ( ComM_GetUserReqFullCom( userReqFullComIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS )
          {
            isRequestPending = TRUE;
            break;
          }
        }
#endif
      }

#if( COMM_PNC_SUPPORT == STD_ON )
      /* If PNC Routing Limitation is enabled, reject the request if no channel user requests FULL_COM and the channel has limited routing */
      if ( (ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX )) && (isRequestPending == TRUE) )
      {
        boolean isUserRequesting = FALSE;
# if ( COMM_MULTIPARTITION == STD_ON )
        for ( user = 0u; user < ComM_GetSizeOfUser(); user++ ) /* FETA_COMM_CSL_USE_CASE */
        {
          partitionIdx = (ComM_PCPartitionConfigIdxOfPartitionIdentifiersType)ComM_GetPartitionConfigIdxOfUser( user );
          if ( ComM_GetForwardUserReqSyncCounter( user, partitionIdx ) == (uint8)(ComM_GetForwardUserReqSyncCounter( user, COMM_MASTER_PARTITION_IDX ) + 1u) )
          {
            isUserRequesting = ComM_IsChannelUserRequestingChannel( channel, partitionIdx );
          }
          else
          {
            isUserRequesting = ComM_IsChannelUserRequestingChannel( channel, COMM_MASTER_PARTITION_IDX );
          }
        }
# else
        isUserRequesting = ComM_IsChannelUserRequestingChannel( channel, COMM_MASTER_PARTITION_IDX );
#endif

        if ( (isUserRequesting == FALSE) && (ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) == COMM_PNCROUTING_LIMITED) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
        {
          isRequestPending = FALSE;
        }
      }
#endif
    }
  }

  /* #40 If Extended PNC Functionality is enabled, a PNC may have no channels assigned. If a user is requesting such a PNC,
   *     return a pending request for each channel to ensure ECU staying in RUN mode. */
#if( COMM_PNC_SUPPORT == STD_ON )
  if ( ComM_HasPncWithoutChannels() )
  {
    for ( pncIndex = 0u; /* FETA_COMM_CSL_USE_CASE */
      (pncIndex < ComM_GetSizeOfPncPb()) && (isRequestPending == FALSE); /* break the loop as soon as a pending request is detected */
      pncIndex++ )
    {
      /* The condition is true if PNC has no channels */
      if ( (!ComM_HasPncChannelMapping()) || (ComM_GetPncChannelMappingStartIdxOfPncPb( pncIndex ) == COMM_NO_PNCCHANNELMAPPINGSTARTIDXOFPNCPB) )
      {
# if (COMM_MULTIPARTITION == STD_ON)
        isRequestPending = ComM_IsUserRequestingPncOnSatellite( pncIndex );
# else
        for ( userReqPncFullComIndex = ComM_GetUserReqPncFullComStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
          userReqPncFullComIndex < ComM_GetUserReqPncFullComEndIdxOfPncPb( pncIndex );
          userReqPncFullComIndex++ )
        {
          if ( ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS )
          {
            isRequestPending = TRUE;
            break;
          }
        }
# endif
      }
    }
  }
#endif

  return isRequestPending;
}


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ComM_Channel_GetMinFullComModeTimerStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Channel_GetMinFullComModeTimerStatus( NetworkHandleType Channel, P2VAR( boolean, AUTOMATIC, COMM_APPL_VAR ) Status )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store TRUE if MinFullComModeTimer is running. Store FALSE otherwise. */
  if ( ComM_GetMinFullComModeTimer( Channel, COMM_MASTER_PARTITION_IDX ) > 0x00u )
  {
    *Status = TRUE;
  }
  else
  {
    *Status = FALSE;
  }
}

/**********************************************************************************************************************
 * ComM_Channel_GetState()
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
FUNC( void , COMM_CODE ) ComM_Channel_GetState( NetworkHandleType Channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) State )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionConfigIdxOfChannelType partitionIdx = COMM_MASTER_PARTITION_IDX;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store the current active state of the channel by default. */
#if ( COMM_MULTIPARTITION == STD_ON )
  if ( ComM_GetActiveComModeSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) == (uint8)(ComM_GetActiveComModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
  {
    partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
  }
#endif
  *State = ComM_GetActiveComMode( Channel, partitionIdx );
  /* #20 If the active state is COMM_NO_COM_NO_PENDING_REQUEST and there is a pending request (external or internal),
    *     the current state depends on Extended RAM Check functionality:
    *     1) if Extended RAM Check functionality is used, the ComM channel is allowed to enter COMM_NO_COM_REQUEST_PENDING state depending on the check result.
    *     2) if Extended RAM Check functionality is not used, store COMM_NO_COM_REQUEST_PENDING.
    *     Note: COMM_NO_COM_REQUEST_PENDING ensures that the request can be considered and ECU stays in RUN mode. */
  if ( ComM_GetActiveComMode( Channel, partitionIdx ) == COMM_NO_COM_NO_PENDING_REQUEST )
  {
    if ( ComM_ChannelIsRequestPending( Channel ) == TRUE )
    {
#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
      *State = ComM_ChannelGetMaxStateAfterExtRamCheck( Channel, COMM_NO_COM_REQUEST_PENDING );
#else
      *State = COMM_NO_COM_REQUEST_PENDING;
#endif
    }
  }
}


#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
/**********************************************************************************************************************
 *  ComM_ChannelGetMaxStateAfterExtRamCheck()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( ComM_StateType, COMM_CODE ) ComM_ChannelGetMaxStateAfterExtRamCheck( NetworkHandleType channel, ComM_StateType targetState )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_StateType maxState = targetState;

  /* #10 Provide the maximum communication state on CAN channels according to the result of Extended RAM Check:
   *     COMM_NO_COM_NO_PENDING_REQUEST if check result is Disable,
   *     COMM_NO_COM_REQUEST_PENDING if check result is Disable Repeat,
   *     the targetState (no changes) otherwise. */
  if ( ComM_GetBusTypeOfChannel( channel ) == COMM_BUS_TYPE_CAN )
  {
    Std_ReturnType ramCheckStatus = CanSM_RamCheckStatus( channel );

    if ( ramCheckStatus == CANSM_DISABLE_COMMUNICATION )
    {
      maxState = COMM_NO_COM_NO_PENDING_REQUEST;
    }
    else if ( ramCheckStatus == CANSM_DISABLE_COMMUNICATION_REPEAT )
    {
      maxState = COMM_NO_COM_REQUEST_PENDING;
    }
    else
    {
      /* no changes */
    }
  }

  return maxState;
}
#endif

/**********************************************************************************************************************
 *  ComM_Channel_HandleTimers()
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
 */
FUNC( void, COMM_CODE ) ComM_Channel_HandleTimers( NetworkHandleType channel, boolean internalRequest )
{
  /* #10 If Minimum FULL_COM mode timer is running, decrement the timer.
   *     Cancel the timer in case an internal request (ComM user or Dcm) exists and the channel has Nm Variant LIGHT. */
  if ( ComM_GetMinFullComModeTimer( channel, COMM_MASTER_PARTITION_IDX ) > 0x00u )
  {
    ComM_DecMinFullComModeTimer( channel, COMM_MASTER_PARTITION_IDX );

    if ( ComM_GetNmTypeOfChannel( channel ) == COMM_LIGHT_NMTYPEOFCHANNEL )
    {
      if ( internalRequest == TRUE )
      {
        ComM_SetMinFullComModeTimer( channel, 0x00, COMM_MASTER_PARTITION_IDX );
      }
    }

    /* #20 If Minimum FULL_COM mode timer is expired, the communication is not required anymore and channel can leave Network Requested state.
     *     If Nm Variant is FULL the next state is Ready Sleep where ComM waits for a shutdown trigger from Nm.
     *     If Nm Variant is LIGHT the target state is NO_COM, the transition depends on configuration of Nm Light and Nm Light Silent timers. */
    if ( ComM_GetMinFullComModeTimer( channel, COMM_MASTER_PARTITION_IDX ) == 0x00u )
    {
      /* Set the default state, it's accepted to overwrite it later (if Nm Type is FULL) to keep the code cleaner */
      ComM_SetBusComModeReq( channel, COMM_NO_COM_NO_PENDING_REQUEST, COMM_MASTER_PARTITION_IDX );

      if ( ComM_GetNmTypeOfChannel( channel ) == COMM_FULL_NMTYPEOFCHANNEL )
      {
        ComM_SetBusComModeReq( channel, COMM_FULL_COM_READY_SLEEP, COMM_MASTER_PARTITION_IDX );
      }
    }
  }

  /* #30 If Nm Light or Nm Light Silent timer is running, decrement the timer.
   *     If the timer is expired set the target state is NO_COM. Note that the variable ComM_NmLightTimer is
   *     shared by Nm Light and Nm Light Silent timers. If Nm Light timer is expired, the target state is NO_COM but
   *     the next state will be SI_COM in case Nm Light Silent timer is configured. */
  if ( ComM_GetNmLightTimer( channel, COMM_MASTER_PARTITION_IDX ) > 0x00u )
  {
    ComM_DecNmLightTimer( channel, COMM_MASTER_PARTITION_IDX );

    if ( ComM_GetNmLightTimer( channel, COMM_MASTER_PARTITION_IDX ) == 0x00u )
    {
      ComM_SetBusComModeReq( channel, COMM_NO_COM_NO_PENDING_REQUEST, COMM_MASTER_PARTITION_IDX );
    }
  }

  COMM_DUMMY_STATEMENT( internalRequest ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_CurrentChannelRequestUpdate()
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
FUNC( void, COMM_CODE ) ComM_CurrentChannelRequestUpdate( NetworkHandleType channel )
{
  if ( ComM_HasFullCommRequestNotif() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_SizeOfFullComRequestersType currentNumOfRequesters; /* Number of users requesting the channel */
    ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
    ComM_UserIterType userIndex;

    /* #10 Initialize the list with the default user handle. */
    for ( currentNumOfRequesters = 0u; currentNumOfRequesters < ComM_GetSizeOfFullComRequesters( COMM_MASTER_PARTITION_IDX ); currentNumOfRequesters++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      ComM_SetFullComRequesters( currentNumOfRequesters, 0xFFu, COMM_MASTER_PARTITION_IDX );
    }

    /* #20 If communication inhibition is active all FULL_COM user requests for this channel are rejected. */
    if ( ComM_ChannelIsLimited( channel ) == FALSE )
    {
      currentNumOfRequesters = 0u;
      /* #30 Otherwise store handles of users that are mapped to the current channel and requesting FULL_COM. */
      for ( userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
#if ( COMM_PNC_SUPPORT == STD_ON )
        if ( !ComM_IsPncUserOfUser( userIndex ) )
#endif
        {
          for ( iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( userIndex ); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( userIndex ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            /* This user is mapped to the current channel and a FULL_COM request exists */
            if ( (ComM_GetChannelOfUserByteMask( iByteMaskIndex ) == channel) &&
              ((ComM_GetUserReqFullCom( ComM_GetUserReqFullComIdxOfUserByteMask( iByteMaskIndex ), COMM_MASTER_PARTITION_IDX ) & ComM_GetSetMaskOfUserByteMask( iByteMaskIndex )) != COMM_NO_REQUESTS) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
            {
              if ( currentNumOfRequesters < ComM_GetSizeOfFullComRequesters( COMM_MASTER_PARTITION_IDX ) )
              {
                /* Store the current user index to the list of FullCom requesters */
                ComM_SetFullComRequesters( currentNumOfRequesters, (ComM_FullComRequestersType)userIndex, COMM_MASTER_PARTITION_IDX );
                currentNumOfRequesters++;
              }
            }
          }
        }
      }
    }

    /* #40 Trigger notification via Sender-Receiver interface. */
    /* Note: The function below will evaluate ComM_FullComRequesters[] and if detected a change, it will notify RTE via
     * the optional sender-receiver interface ComM_CurrentChannelRequest */
    ComM_CurrentChannelRequestNotification( channel, currentNumOfRequesters );
  }

  COMM_DUMMY_STATEMENT( channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_StateChangeNotification()
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
 */
 /* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
FUNC( void, COMM_CODE ) ComM_StateChangeNotification( NetworkHandleType channel )
{
#if( COMM_USERMODENOTIFUNC == STD_ON )
  if ( !ComM_HasUserModeNotificationOnlyForPncUsers() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_SizeOfUserType userIndex;
    ComM_ModeType ComM_CalcMode = COMM_NO_COMMUNICATION;
    Std_ReturnType retValue = RTE_E_OK;
    Std_ReturnType retValueCumulated = RTE_E_OK;
    ComM_UserByteMaskEndIdxOfUserType iByteMaskIndex;
    boolean bNotify = TRUE;

    /* #10 Iterate over users that have configured User Mode (state change) notification and are mapped to channels (not PNC). */
    for ( userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      ComM_UserIterType user = (ComM_UserIterType)userIndex;
      if ( ComM_GetUserModeNotiFunc( user ) != NULL_PTR )
      {
# if ( COMM_PNC_SUPPORT == STD_ON )
        if ( !ComM_IsPncUserOfUser( user ) )
# endif
        {
          /* #100 In single-channel configurations notify the BusSM mode to the users */
          ComM_CalcMode = ComM_GetBusSmState( channel, COMM_MASTER_PARTITION_IDX );

          if ( ComM_GetSizeOfChannel() > 1u ) /* ComM_GetSizeOfChannel represents the superset number of channels over all variants. */
          {
            /* #110 In multi-channel configurations only notify a new user mode if
            *     1) the user is mapped to the current channel where the BusSM mode has changed and
            *     2) the lowest mode of all channels mapped to the user has changed since the last mode notification. */
            bNotify = FALSE;

            for ( iByteMaskIndex = ComM_GetUserByteMaskStartIdxOfUser( user ); iByteMaskIndex < ComM_GetUserByteMaskEndIdxOfUser( user ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
            {
              if ( ComM_GetChannelOfUserByteMask( iByteMaskIndex ) == channel )
              {
                bNotify = TRUE; /* PRQA S 2982 */ /* MD_ComM_2982 */
              }

              if ( ComM_GetBusSmState( ComM_GetChannelOfUserByteMask( iByteMaskIndex ), COMM_MASTER_PARTITION_IDX ) < ComM_CalcMode )
              {
                ComM_CalcMode = ComM_GetBusSmState( ComM_GetChannelOfUserByteMask( iByteMaskIndex ), COMM_MASTER_PARTITION_IDX );
              }
            }
          }

          if ( bNotify == TRUE )
          {
            if ( ComM_CalcMode != ComM_GetLastStateChange( user, COMM_MASTER_PARTITION_IDX ) )
            {
              if ( (ComM_SizeOfUserModeNotiFuncType)userIndex < ComM_GetSizeOfUserModeNotiFunc( ) )
              {
                /* #120 If a new mode is available for the current user notify it using the Mode Switch Interface (sender-receiver) 'ComM_CurrentMode' */
                switch ( ComM_CalcMode )
                {
                case COMM_FULL_COMMUNICATION:
                  retValue = ComM_GetUserModeNotiFunc( user )(RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION); /* VCA_COMM_UserModeNotiFunc_PointerCall */
                  break;
                case COMM_SILENT_COMMUNICATION:
                  retValue = ComM_GetUserModeNotiFunc( user )(RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION); /* VCA_COMM_UserModeNotiFunc_PointerCall */
                  break;
                case COMM_NO_COMMUNICATION:
                  retValue = ComM_GetUserModeNotiFunc( user )(RTE_MODE_ComMMode_COMM_NO_COMMUNICATION); /* VCA_COMM_UserModeNotiFunc_PointerCall */
                  break;

                default: /* COV_COMM_MISRA */
                  break;
                }
              }

              if ( retValue == RTE_E_OK )
              {
                /* #130 If notification to the current user was successful, update the mode to ensure that the same one will not be notified again. */
                ComM_SetLastStateChange( user, ComM_CalcMode, COMM_MASTER_PARTITION_IDX ); /*lint !e661 ComM_LastStateChange[] has a size relation to ComM_User[] */
              }
              else
              {
                /* #140 If at least one user notification was not successful, the state change on this channel will be processed again in the next task. */
                retValueCumulated = E_NOT_OK;
              }
            }
          }
        }
      }
    }

    if ( retValueCumulated == RTE_E_OK )
    {
      /* #20 If all users were notified successfully, mark processing of this state change as completed. */
      ComM_SetStateChange( channel, COMM_NO_STATE_CHANGE, COMM_MASTER_PARTITION_IDX );
    }
  }
#endif
  COMM_DUMMY_STATEMENT( channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_ProcessPassiveWakeUp()
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
FUNC( void, COMM_CODE ) ComM_ProcessPassiveWakeUp( NetworkHandleType channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  if ( ComM_HasSynchronousWakeUp() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    uint8 channelIndex;

    /* #10 If Synchronous Wake-up is enabled, notify passive wake-up to all ComM channels being in NO_COM or SI_COM state.
     *     This is done by setting the bus-side requested state to FULL_COM (Network Requested or Ready Sleep). */
#if (COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();
      ComM_SetNmStartUpIndicationSyncCounter( channel, (uint8)(ComM_GetNmStartUpIndicationSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      /* @ assert partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); */ /* VCA_COMM_PARTITION_IDX */
      /* @ assert channel < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
      ComM_SetGlobalPartitionSyncCounter( channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
    else
#endif
    {
      ComM_SetNmStartUpIndicated( 0u, TRUE, COMM_MASTER_PARTITION_IDX );

      for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        if ( ComM_IsFrShutdown( channelIndex, ComM_GetPartitionConfigIdxOfChannel( channelIndex ) ) )
        {
          /* #100 If communication shutdown is ongoing on a FlexRay channel, delay the notification until FrSM indicates NO_COM
           *      because shutdown on FlexRay shall not be interrupted.
           *      Note: the ongoing shutdown means that the channel is not in FULL_COM anymore, no need to check. */
          ComM_SetBusComModeReqFrShutdown( channelIndex, ComM_GetWakeupStateOfChannel( channelIndex ), partitionIdx );
        }
        else
        {
          if ( (ComM_GetActiveComMode( channelIndex, partitionIdx ) & COMM_FULL_COMMUNICATION) == 0u )
          {
            ComM_SetBusComModeReq( channelIndex, ComM_GetWakeupStateOfChannel( channelIndex ), partitionIdx );
          }
        }
      }

      /* #20 If Synchronous Wake-up is enabled, activate all PNCs at least for the duration of the PNC Prepare Sleep Timer. */
#if (COMM_PNC_SUPPORT == STD_ON)
      ComM_ActivateAllPncs();
#endif
    }
  }
  else /* SynchronousWakeUp is disabled */
  {
    /* #30 If Synchronous Wake-up is disabled, notify passive wake-up to the channel by setting the bus-side
     *     requested state to FULL_COM (Network Requested or Ready Sleep). Ignore the wake-up in FULL_COM mode. */
    if ( ComM_IsFrShutdown( channel, partitionIdx ) )
    {
      /* #300 If communication shutdown is ongoing on FlexRay channel, delay the notification until FrSM indicates NO_COM
       *      because shutdown on FlexRay shall not be interrupted. */
#if ( COMM_MULTIPARTITION == STD_ON)
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

        ComM_SetBusComModeReqFrShutdown( channel, ComM_GetWakeupStateOfChannel( channel ), partitionIdx );
        /* Increment sync counter for ComM_BusComModeReqFrShutdown by 1 (relative to the corresponding Master-Counter) */
        ComM_SetBusComModeReqFrShutdownSyncCounter( channel, (uint8)(ComM_GetBusComModeReqFrShutdownSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
      }
      else
#endif
      {
        ComM_SetBusComModeReqFrShutdown( channel, ComM_GetWakeupStateOfChannel( channel ), partitionIdx );
      }
    }
    else
    {
      if ( (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u ) /* ESCAN00091475 */
      {
#if ( COMM_MULTIPARTITION == STD_ON)
        if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();
          /* @ assert partitionIdx < ComM_GetSizeOfPartitionIdentifiers(); */ /* VCA_COMM_PARTITION_IDX */
          /* @ assert channel < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
          ComM_SetBusComModeReq( channel, ComM_GetWakeupStateOfChannel( channel ), partitionIdx );

          /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
          ComM_SetBusComModeReqSyncCounter( channel, (uint8)(ComM_GetBusComModeReqSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
          ComM_SetGlobalPartitionSyncCounter( channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
        }
        else
#endif
        {
          /* @ assert channel < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
          ComM_SetBusComModeReq( channel, ComM_GetWakeupStateOfChannel( channel ), partitionIdx );
        }
      }
    }
  } /* SynchronousWakeUp */

  COMM_DUMMY_STATEMENT( channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}


/**********************************************************************************************************************
 *  ComM_CalculateHighestComMode()
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
 *
 *
 *
 */
 /* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
FUNC( ComM_StateType, COMM_CODE ) ComM_CalculateHighestComMode( NetworkHandleType channel, boolean *internalRequest )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_StateType lHighestRequest = COMM_NO_COM_NO_PENDING_REQUEST;
  *internalRequest = FALSE;

  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

  if ( ComM_HasKeepAwakeChannels() )
  {
    /* #10 The highest requested mode is COMM_FULL_COM_NETWORK_REQUESTED if the following conditions are true:
     *   Communication shutdown on FlexRay is finished. (This is to counteract the so-called 'clique problem' on FlexRay) and:
     *     1) If Communication Inhibition is not active and at least one user mapped to the channel directly or via PNC requests FULL_COM or
     *     2) Dcm has requested communication (it does not take Communication Inhibition into account). */

    /* ComM postpones the execution of communication requests until FrSM indicates entering NO_COM mode.  */
    if ( !ComM_IsFrShutdown( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) )
    {
      if ( ComM_ChannelIsLimited( channel ) == FALSE )
      {
        if ( ComM_IsChannelUserRequestingChannel( channel, COMM_MASTER_PARTITION_IDX ) == TRUE )
        {
          lHighestRequest = COMM_FULL_COM_NETWORK_REQUESTED;
          *internalRequest = TRUE;
          /* #100 If Nm variant LINSLAVE is present and a user is requesting the channel, the BusSleepMode flag is cleared for the corresponding channel. */
          /* No runtime check for linslave channel needed, because variable is initialized with FALSE for each channel and only set to TRUE for linslave channels */
          if ( ComM_HasLinslaveNmTypeOfChannel() )
          {
            ComM_SetBusSleepModeIndicated( channel, FALSE, COMM_MASTER_PARTITION_IDX );
          }
        }
        else if ( ComM_GetNmTypeOfChannel( channel ) == COMM_LINSLAVE_NMTYPEOFCHANNEL )
        {
          /* #110 If no user is requesting the channel and current channel is Nm variant LINSLAVE and the active mode is COMM_FULL_COM_NETWORK_REQUESTED,
           *      the highest requested mode is COMM_FULL_COM_READY_SLEEP and the externally requested state is set to COMM_NO_COM_NO_PENDING_REQUEST.
           *      (This ensures that the channel remains in state COMM_FULL_COM_READY_SLEEP after a passive wake-up event and does not transition to COMM_FULL_COM_NETWORK_REQUEST again.) */
          if ( ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_NETWORK_REQUESTED )
          {
            lHighestRequest = COMM_FULL_COM_READY_SLEEP;
            ComM_SetBusComModeReq( channel, COMM_NO_COM_NO_PENDING_REQUEST, COMM_MASTER_PARTITION_IDX );
          }
        }
        else
        {
          /* Intentionally left empty.*/
        }
      }

      if ( ComM_IsDcmRequestActive( channel, COMM_MASTER_PARTITION_IDX ) )
      {
        lHighestRequest = COMM_FULL_COM_NETWORK_REQUESTED;
        *internalRequest = TRUE;
      }
    }
  }

#if (COMM_PNC_SUPPORT == STD_ON)
  /* #20 If PN feature is enabled and the channel is not requested by a user or Dcm, the requested mode is the highest one of
   *     the mode resulting from the Gateway Type of the channel and the highest activation of PNCs mapped to it or
   *     COMM_FULL_COM_READY_SLEEP if at least one PNC mapped to the channel is in PNC_FULL_COMMUNICATION state. */
  if ( ComM_IsPncPbIndUsedOfChannelPb( channel ) ) /* Checks if the channel is mapped to a PNC */
  {
    if ( ComM_HasCoordinatedOfPnc() )
    {
      if ( lHighestRequest == COMM_NO_COM_NO_PENDING_REQUEST )
      {
        lHighestRequest = ComM_PncGetGwChannelRequest( channel );
        if ( lHighestRequest == COMM_FULL_COM_NETWORK_REQUESTED )
        {
          *internalRequest = TRUE;
        }
      }

      if ( (ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX )) && (lHighestRequest == COMM_FULL_COM_NETWORK_REQUESTED) )
      {
        /* Enter the state COMM_PNCROUTING_NOT_LIMITED because channel is kept awake by a ComM user. */
        ComM_SetPncToChannelRoutingState( channel, ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) | COMM_PNCROUTING_NOT_LIMITED, COMM_MASTER_PARTITION_IDX );
      }
      else
      {
        /* nothing to do */
      }
    }

    /* This to consider all PNCs including not coordinated ones */
    if ( (!ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX )) || (ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) != COMM_PNCROUTING_LIMITED) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
    {
      lHighestRequest = ComM_PncInFullComWakesUpTheChannel( channel, lHighestRequest );
    }
  }
#endif

  /* #30 The requested mode shall be at least as high as requested via bus.
   *     Bus communication requests shall always be granted according to the 'highest win' strategy. */
  if ( (lHighestRequest == COMM_NO_COM_NO_PENDING_REQUEST) || (ComM_GetBusComModeReq( channel, COMM_MASTER_PARTITION_IDX ) == COMM_FULL_COM_NETWORK_REQUESTED) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    lHighestRequest = ComM_GetBusComModeReq( channel, COMM_MASTER_PARTITION_IDX );
  }

  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

  return lHighestRequest;
}

/**********************************************************************************************************************
 *  ComM_ProcessSynchronousWakeUp()
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
 */
 /* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC( void, COMM_CODE ) ComM_ProcessSynchronousWakeUp( void )
{
  if ( ComM_HasSynchronousWakeUp() && ComM_HasNmSupportOfChannel() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    NetworkHandleType channel;

    /* ----- Implementation ----------------------------------------------- */
    /* ESCAN00108491: Ensure that all channels, which are not yet in FULL_COMMUNICATION, are woken up by the synchronous passive wakeup event.
     *                A wakeup event shall be handled with higher priority than a shutdown event, i.e. a wakeup event shall not be overwritten by
     *                a sleep command, e.g. ComM_Nm_BusSleepMode(), happening at the same time. */

    /* #10 Check if an EcuM WakeUp - Indication occurred. */
    if ( ComM_IsEcuMWakeUpIndicated( 0u, COMM_MASTER_PARTITION_IDX ) )
    {
      /* #100 Wake up every channel which is not in FULL_COMMUNICATION yet by setting the bus-site requested communication mode to the wakeup state of the channel. */
      for ( channel = 0u; channel < ComM_GetSizeOfChannel(); channel++ ) /* FETA_COMM_CSL_USE_CASE */
      { /* PRQA S 3415 2 */ /* MD_ComM_3415 */
        if ( ((ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u)
             && (ComM_GetBusComModeReq( channel, COMM_MASTER_PARTITION_IDX ) != ComM_GetWakeupStateOfChannel( channel )) )
        {
          ComM_SetBusComModeReq( channel, ComM_GetWakeupStateOfChannel( channel ), COMM_MASTER_PARTITION_IDX );

          /* #110 If NM Gateway Extension is enabled notify Nm_PassiveStartUp() for each channel with Nm. */
          if ( ComM_HasNmGwExt() )
          {
            if ( ComM_IsNmSupportOfChannel( channel ) )
            {
              (void)ComM_Nm_ServiceHandler( NM_PASSIVESTARTUP, channel, COMM_PNC_ID_NOT_USED );
            }
          }
        }
      }

      ComM_SetEcuMWakeUpIndicated( 0u, FALSE, COMM_MASTER_PARTITION_IDX );
    }

    /* #20 Check if an Nm WakeUp-Indication occurred. */
    if ( ComM_IsNmStartUpIndicated( 0u, COMM_MASTER_PARTITION_IDX ) )
    {
      /* #200 Wake up every channel which is not yet in FULL_COMMUNICATION by setting the bus-site requested communication mode to the wakeup state of the channel. */
      for ( channel = 0u; channel < ComM_GetSizeOfChannel(); channel++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        if ( ComM_IsFrShutdown( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) )
        {
          /* #210 If communication shutdown is ongoing on a FlexRay channel, delay the notification until FrSM indicates NO_COM
           *      because shutdown on FlexRay shall not be interrupted.
           *      Note: the ongoing shutdown means that the channel is not in FULL_COM anymore, no need to check. */
          ComM_SetBusComModeReqFrShutdown( channel, ComM_GetWakeupStateOfChannel( channel ), COMM_MASTER_PARTITION_IDX );
        }
        else
        {
          if ( (ComM_GetActiveComMode( channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
          {
            ComM_SetBusComModeReq( channel, ComM_GetWakeupStateOfChannel( channel ), COMM_MASTER_PARTITION_IDX );
          }
        }
      }

      ComM_SetNmStartUpIndicated( 0u, FALSE, COMM_MASTER_PARTITION_IDX );
    }
  }
}

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
