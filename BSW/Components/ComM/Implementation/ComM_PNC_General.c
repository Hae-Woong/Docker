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
/*!         \file     ComM_PNC_General.c
 *          \unit     PNC_General
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the PNC_General unit.
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

#define CCL_ASR_COMM_PNC_GENERAL_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include "ComM_PNC_General.h"
#include "ComM_PNC_SignalProcessing.h"
#include "ComM_ModeInhibition.h"
#include "ComM_Private_Nm.h"
#include "ComM_ErrorReporting.h"
#include "ComM_Util.h"
#include "BswM_ComM.h"
#include "SchM_ComM.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
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

#if (COMM_PNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
  ComM_PncRoutingLimitationHandleChannels()
 *********************************************************************************************************************/
/*! \brief      Special handling (attaching and detaching) of channels while PNC is in state PNC_REQUESTED.
 *  \details    If the feature 'PNC To Channel Routing Limitation' is activated, channels can be attached or detached
 *              while PNC is in state PNC_REQUESTED by de-activating or activating the limitation. Here we ensure that:
 *              1) if the channel has been attached to PNC, EIRA with PNC bit = 1 is sent out or
 *              2) if the channel has been detached from PNC, EIRA with PNC bit = 0 is sent out on the channel.
 *              Note: attaching when entering NM_REPEAT_MESSAGE state is handled in ComM_Nm_StateChangeNotification().
 *              Note: function ComM_PncModeSendSignal() cannot be used because it handles all PNC channels.
 *  \param[in]  InternalPncId           local PNC handle
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_PncRoutingLimitationHandleChannels( ComM_PncIterType InternalPncId );

/**********************************************************************************************************************
  ComM_CalculateHighestPNComMode()
 *********************************************************************************************************************/
/*! \brief      Calculates the highest communication mode request for the PNC.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \param[in]  pncCanBeRequested       The parameter is only used if PNC Routing Limitation feature is active
 *                                      TRUE: PNC is allowed to enter PNC_REQUESTED state, Routing Limitation is disabled on at least one channel
 *                                      FALSE: otherwise, ComM inhibits all FULL_COM requests to the PNC
 *  \return     Highest requested PNC mode
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-2818028, SPEC-2818290, SPEC-19872, SPEC-19786, SPEC-19866, SPEC-19806, SPEC-19857, SPEC-2818042,
 *  \trace      SPEC-19838, SPEC-19785, SPEC-19853
 *********************************************************************************************************************/
COMM_LOCAL FUNC( ComM_PncModeType, COMM_CODE ) ComM_CalculateHighestPNComMode( ComM_PncIterType pncIndex, boolean pncCanBeRequested );

/**********************************************************************************************************************
  ComM_PncClearWakeup()
 *********************************************************************************************************************/
/*! \brief      Clears the PNC wakeup flag.
 *  \details    -
 *  \param[in]  ComM_CurrentPn        Current PNC
 *  \param[in]  partitionIdx          identification of the internal partition index
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_PncClearWakeup( ComM_PncIterType ComM_CurrentPn, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx );

/**********************************************************************************************************************
  ComM_PncIsOnlySystemUserRequesting()
 *********************************************************************************************************************/
/*! \brief      Determines whether only the system user is requesting the PNC (via ERA) or regular PNC user.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \param[in]  byteIndex               byte index of UserReqPncFullCom
 *  \return     TRUE                    if in the given request byte only the system user request is stored and no other PNC user request.
 *  \return     FALSE                   if at least one PNC user (not the system user) stored the request in the given byte.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_PncIsOnlySystemUserRequesting( ComM_PncIterType pncIndex, ComM_UserReqPncFullComIterType byteIndex );

/**********************************************************************************************************************
  ComM_PncRoutingLimitationSetState()
 *********************************************************************************************************************/
/*! \brief      Updates the PNC Routing Limitation state depending on input parameters highestPNComModeReq and pncRequestIsPending
 *  \details    -
 *  \param[in]  channel              -   local channel handle
 *  \param[in]  highestPNComModeReq  -   the highest requested mode of Pncs mapped to the channel
 *  \param[in]  pncRequestIsPending  -   TRUE - at least one FULL_COM request for a PNC mapped to the channel exists
 *                                       FALSE - otherwise
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_PncRoutingLimitationSetState( CONST( NetworkHandleType, AUTOMATIC ) channel, CONST( ComM_PncModeType, AUTOMATIC ) highestPNComModeReq, CONST( boolean, AUTOMATIC ) pncRequestIsPending );

/**********************************************************************************************************************
  ComM_PncStateTransition()
 *********************************************************************************************************************/
/*! \brief      Performs state transitions to reach the target requested PNC state.
 *  \details    -
 *  \param[in]  ComM_CurrentPn          local PNC handle
 *  \param[in]  highestComModeReq       the target requested PNC state
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      SPEC-19784, SPEC-19803, SPEC-19878, SPEC-19865, SPEC-19815
 *********************************************************************************************************************/
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_PncStateTransition( ComM_PncIterType ComM_CurrentPn, ComM_PncModeType highestComModeReq );

# if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON )
/**********************************************************************************************************************
  ComM_SynchronizedPncShutdownHandling()
 *********************************************************************************************************************/
/*! \brief      Determines if a Synchronized PNC Shutdown is possible while the affected PNC transitions to PNC_READY_SLEEP.
 *  \details    -
 *  \param[in]  pncIndex                local PNC handle
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SynchronizedPncShutdownHandling( ComM_PncIterType ComM_CurrentPn );
# endif
#endif


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (COMM_PNC_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  ComM_PncRoutingLimitationHandleChannels()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_PncRoutingLimitationHandleChannels( ComM_PncIterType InternalPncId )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PncSignalIndEndIdxOfPncPbType ComM_PncSignalMappingIndex;
  ComM_SizeOfPncSignalType ComM_SignalIndex;
  ComM_SizeOfChannelType ComM_ChannelIndex;

  /* ----- Implementation ----------------------------------------------- */
  for ( ComM_PncSignalMappingIndex = ComM_GetPncSignalIndStartIdxOfPncPb( InternalPncId ); ComM_PncSignalMappingIndex < ComM_GetPncSignalIndEndIdxOfPncPb( InternalPncId ); ComM_PncSignalMappingIndex++ )  /* PRQA S 2994 */ /* MD_ComM_2994 */ /* FETA_COMM_CSL_USE_CASE */
  {
    ComM_SignalIndex = ComM_GetPncSignalInd( ComM_PncSignalMappingIndex );
    if ( ComM_GetTypeOfPncSignal( ComM_SignalIndex ) == COMM_EIRA_TX_TYPEOFPNCSIGNAL )
    {
      ComM_ChannelIndex = ComM_GetChannelIdxOfPncSignal( ComM_SignalIndex );

      /* #10 Attaching: PNC routing is not limited and PNC bit = 0 was sent on the channel, send PNC bit = 1 now. */
      /* PRQA S 3415 2 */ /* MD_ComM_3415 */
      if ( ((ComM_GetPncToChannelRoutingState( ComM_ChannelIndex, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NOT_LIMITED) != 0x00u) &&
            (ComM_IsSignalStoredPncBitSet( ComM_SignalIndex, InternalPncId, COMM_MASTER_PARTITION_IDX ) == FALSE) )
      {
        if ( (ComM_GetGwTypeOfChannel( ComM_ChannelIndex ) == COMM_GATEWAY_TYPE_PASSIVE) && (ComM_GetActivePncComMode( InternalPncId, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_REQUESTED_PASSIVE) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
        {
          /* PNC is requested via ERAp=1 from PASSIVE channel, do not keep the channel awake and do not mirror the request back */
        }
        else
        {
          ComM_PncSetBitInSignal( ComM_SignalIndex, InternalPncId, COMM_MASTER_PARTITION_IDX );
# if (COMM_MULTIPARTITION == STD_ON)
          if ( ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) != COMM_MASTER_PARTITION_IDX )
          {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
            /* @ assert ComM_ChannelIndex < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
            ComM_SetForwardComSendSignalSyncCounter( ComM_ChannelIndex, (uint8)(ComM_GetForwardComSendSignalSyncCounter( ComM_ChannelIndex, ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) ) + 1u), COMM_MASTER_PARTITION_IDX );
            ComM_SetForwardingComSendSignalIndex( ComM_ChannelIndex, ComM_SignalIndex, COMM_MASTER_PARTITION_IDX );
            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
          }
          else
# endif
          {
            (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( ComM_SignalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( ComM_SignalIndex ), COMM_MASTER_PARTITION_IDX ) ); /* PRQA S 0315 */ /* MD_ComM_0315 */
          }

          ComM_PncNmRequestHandling( ComM_ChannelIndex, InternalPncId );
        }
      }
      /* #20 Detaching: PNC routing is limited and PNC bit = 1 was sent on the channel, send PNC bit = 0 now. Valid for any GW type. */
      /* PRQA S 3415, 2985 2 */ /* MD_ComM_3415, MD_ComM_2985 */
      else if ( (ComM_GetPncToChannelRoutingState( ComM_ChannelIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNCROUTING_LIMITED) &&
                (ComM_IsSignalStoredPncBitSet( ComM_SignalIndex, InternalPncId, COMM_MASTER_PARTITION_IDX ) == TRUE) )
      {
        ComM_PncClearBitInSignal( ComM_SignalIndex, InternalPncId, COMM_MASTER_PARTITION_IDX );
# if (COMM_MULTIPARTITION == STD_ON)
        if ( ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) != COMM_MASTER_PARTITION_IDX )
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
          /* @ assert ComM_ChannelIndex < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
          ComM_SetForwardComSendSignalSyncCounter( ComM_ChannelIndex, (uint8)(ComM_GetForwardComSendSignalSyncCounter( ComM_ChannelIndex, ComM_GetPartitionConfigIdxOfChannel( ComM_ChannelIndex ) ) + 1u), COMM_MASTER_PARTITION_IDX );
          ComM_SetForwardingComSendSignalIndex( ComM_ChannelIndex, ComM_SignalIndex, COMM_MASTER_PARTITION_IDX );
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
        }
        else
# endif
        {
          (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( ComM_SignalIndex ), ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( ComM_SignalIndex ), COMM_MASTER_PARTITION_IDX ) ); /* PRQA S 0315 */ /* MD_ComM_0315 */
        }
      }
      else
      {
        /* nothing to do */
      }
    }
  }

  COMM_DUMMY_STATEMENT( InternalPncId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}


/**********************************************************************************************************************
 *  ComM_CalculateHighestPNComMode()
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
 /* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
COMM_LOCAL FUNC( ComM_PncModeType, COMM_CODE ) ComM_CalculateHighestPNComMode( ComM_PncIterType pncIndex, boolean pncCanBeRequested )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PncModeType lHighestRequest = COMM_PNC_NO_COMMUNICATION;
  ComM_UserReqPncFullComEndIdxOfPncPbType  userReqPncFullComIndex;
  ComM_PncChannelMappingEndIdxOfPncPbType pncChMappingIndex;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_0();

  /* The guard below is needed to inhibit FULL_COM request to a PNC if all its channels have
  * Routing Limitation state COMM_PNCROUTING_LIMITED (pncCanBeRequested == FALSE). */
  if ( pncCanBeRequested == TRUE )
  {
    /* #10 Iterate through PNC user request bytes in ComM_UserReqPncFullCom[] for the given PN.
    *     If at least one user request is detected, set the requested mode to
    *     COMM_PNC_REQUESTED_PASSIVE if only system user is requesting and Passive Gw Type is supported or
    *     COMM_PNC_REQUESTED otherwise. */

    for ( userReqPncFullComIndex = ComM_GetUserReqPncFullComStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
          userReqPncFullComIndex < ComM_GetUserReqPncFullComEndIdxOfPncPb( pncIndex );
          userReqPncFullComIndex++ )
    {
      if ( ComM_GetUserReqPncFullCom( userReqPncFullComIndex, COMM_MASTER_PARTITION_IDX ) != COMM_NO_REQUESTS )
      {
        lHighestRequest = COMM_PNC_REQUESTED_PASSIVE; /* PRQA S 2982 */ /* MD_ComM_2982 */
        if ( ComM_PncIsOnlySystemUserRequesting( pncIndex, userReqPncFullComIndex ) == FALSE )
        {
          lHighestRequest = COMM_PNC_REQUESTED;
          break; /* Highest requested mode found */
        }
      }
    }

    /* #20 If Passive Gw Type is supported and PNC activation is received via ERA from an Active channel,
    *     set the requested mode to COMM_PNC_REQUESTED. */
    if ( lHighestRequest == COMM_PNC_REQUESTED_PASSIVE )
    {
      if ( ComM_PncIsEraReceivedOnActiveChannel( pncIndex ) == TRUE )
      {
        lHighestRequest = COMM_PNC_REQUESTED;
      }
    }

    /* #30 If Mode Limitation NO_COM or Prevent Wake-up is active on at least one PNC channel, ComM shall inhibit FULL_COM requests. */
    if ( ComM_HasPncChannelMapping() && ComM_Channel_HasModeLimitationOrWakeUpInhibition() ) /* PRQA S 3415 */ /* MD_ComM_3415 */
    {
      if ( ComM_HasKeepAwakeChannels() )
      {
        for ( pncChMappingIndex = ComM_GetPncChannelMappingStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
              pncChMappingIndex < ComM_GetPncChannelMappingEndIdxOfPncPb( pncIndex );
              pncChMappingIndex++ )
        {
          if ( ComM_ChannelIsLimited( ComM_GetPncChannelMapping( pncChMappingIndex ) ) == TRUE )
          {
            lHighestRequest = COMM_PNC_NO_COMMUNICATION;
            break;
          }
        }
      }
      else
      {
        lHighestRequest = COMM_PNC_NO_COMMUNICATION;
      }
    }
  }

  /* Do not allow PNC activation if pncCanBeRequested == FALSE, but always allow PNC de-activation */
  if ( (pncCanBeRequested == TRUE) || (ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) != COMM_PNC_NO_COMMUNICATION) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    /* #40 The requested mode shall be at least as requested via bus (EIRA). */
    if ( ComM_GetBusPncComModeReq( pncIndex, COMM_MASTER_PARTITION_IDX ) > lHighestRequest )
    {
      lHighestRequest = ComM_GetBusPncComModeReq( pncIndex, COMM_MASTER_PARTITION_IDX );
    }
    else if ( (ComM_GetPncPSleepTimer( pncIndex, COMM_MASTER_PARTITION_IDX ) > 0u) && (lHighestRequest <= COMM_PNC_PREPARE_SLEEP) )
    {
      lHighestRequest = COMM_PNC_PREPARE_SLEEP;
    }
    else
    {
      /* Nothing to do. Statement is needed for MISRA-C:2004 compliance (Rule 14.10). */
    }
  }

  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_0();

  return lHighestRequest;
}


/**********************************************************************************************************************
 *  ComM_PncClearWakeup()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_PncClearWakeup( ComM_PncIterType ComM_CurrentPn, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( (ComM_HasPncPSleepTimer( COMM_MASTER_PARTITION_IDX )) && (ComM_Pnc_HasSynchronousWakeUpOrPncWakeupEnabled()) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    /* #10 Clear the pnc wakeup flag when a wakeup has occurred before. */
    if ( ComM_GetBusPncComModeReq( ComM_CurrentPn, partitionIdx ) == COMM_PNC_PREPARE_SLEEP )
    {
      ComM_SetBusPncComModeReq( ComM_CurrentPn, COMM_PNC_NO_COMMUNICATION, partitionIdx );
    }
  }

  COMM_DUMMY_STATEMENT( ComM_CurrentPn ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( partitionIdx ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}


/**********************************************************************************************************************
 *  ComM_PncIsOnlySystemUserRequesting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( boolean, COMM_CODE ) ComM_PncIsOnlySystemUserRequesting( ComM_PncIterType pncIndex, ComM_UserReqPncFullComIterType byteIndex )
{
  /* ----- Local Variable ---------------------------------------------- */
  boolean retValue = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the provided byteIndex corresponds to the system user, check whether a request from another PNC user (not the system user)
  *      is stored in this byte. Return TRUE if only a request of the system user is stored in the request byte.
  *      Note: if PNC is synchronized, there is exactly one system user and one byte index where the system user' request is stored. */
  if ( ComM_HasPncGwTypePassive() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_UserPncByteMaskStartIdxOfUserType uSysUserByteMaskIndex = 0u;

    if ( ComM_IsCoordinatedOfPnc( pncIndex ) )
    {
      uSysUserByteMaskIndex = ComM_GetUserPncByteMaskStartIdxOfUser( (ComM_UserIterType)ComM_GetSystemUserOfPnc( pncIndex ) );
      if ( byteIndex == ComM_GetUserReqPncFullComIdxOfUserPncByteMask( uSysUserByteMaskIndex ) )
      {
        if ( ((ComM_GetUserReqPncFullCom( byteIndex, COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfUserPncByteMask( uSysUserByteMaskIndex )) == COMM_NO_REQUESTS) )
        {
          retValue = TRUE;
        }
      }
    }
  }

  COMM_DUMMY_STATEMENT( pncIndex ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( byteIndex ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return retValue;
}


/**********************************************************************************************************************
 *  ComM_PncRoutingLimitationSetState()
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
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_PncRoutingLimitationSetState( CONST( NetworkHandleType, AUTOMATIC ) channel, CONST( ComM_PncModeType, AUTOMATIC ) highestPNComModeReq, CONST( boolean, AUTOMATIC ) pncRequestIsPending )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX ) )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;

    if ( highestPNComModeReq >= COMM_PNC_REQUESTED )
    {
      /* #10 Enter the Routing Limitation state COMM_PNCROUTING_NOT_LIMITED if
      *     1) ERA=1 is received on the channel or
      *     2) ComM user requests FULL_COM for a PNC, which is not limited on the channel. */
      ComM_SetPncToChannelRoutingState( channel, ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) | COMM_PNCROUTING_NOT_LIMITED, COMM_MASTER_PARTITION_IDX );
    }
    else if ( pncRequestIsPending == TRUE )
    {
      /* #20 Exit the Routing Limitation state COMM_PNCROUTING_NOT_LIMITED if
      *     1) All ComM users assigned to the channel request NO_COM and
      *     2) ERA=0 is received on the channel and
      *     3) All PNCs being in state PNC_REQUESTED are limited on the channel.
      *     Clear the COMM_PNCROUTING_NOT_LIMITED flag, note that Nm can be in state NM_STATE_REPEAT_MESSAGE. */
      ComM_SetPncToChannelRoutingState( channel, ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NOT_LIMITED_CLEAR, COMM_MASTER_PARTITION_IDX );
    }
    else
    {
      /* #30 Otherwise and if at least one PNC on the channel is limited, exit the Routing Limitation state COMM_PNCROUTING_NOT_LIMITED. */
      /* This is needed e.g. for the following scenario: a PNC is limited on all channels, requesting the PNC via user shall not activate it. */
      if ( ComM_HasPncChannelMapping() )
      {
        for ( pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb( channel ); /* FETA_COMM_CSL_USE_CASE */
          pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb( channel );
          pncPbIndIter++ )
        {
          /*lint -e{666} */
          if ( ComM_IsPncLimitation( channel, ComM_GetPncPbInd( pncPbIndIter ), COMM_MASTER_PARTITION_IDX ) )
          {
            ComM_SetPncToChannelRoutingState( channel, ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NOT_LIMITED_CLEAR, COMM_MASTER_PARTITION_IDX );
            break;
          }
        }
      }
    }
  }

  COMM_DUMMY_STATEMENT_CONST( channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( highestPNComModeReq ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( pncRequestIsPending ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}


/***********************************************************************************************************************
 *  ComM_PncStateTransition()
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
 /* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
COMM_LOCAL FUNC( void, COMM_CODE ) ComM_PncStateTransition( ComM_PncIterType ComM_CurrentPn, ComM_PncModeType highestComModeReq )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PncModeType localhighestComModeReq = highestComModeReq;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the highest requested PNC mode differs from the currently active PNC mode, perform as many
  * state transitions as needed to reach the highest requested PNC mode.
  *     1) Send COM signal if entering or leaving PNC Requested state,
  *     2) Set the new PNC state,
  *     3) Notify BswM about the new PNC state (BusSM states of channels are not considered),
  *     4) Handle PNC Prepare Sleep Timer if applicable,
  *     5) Store a flag to notify PNC users about the new state if entering or leaving PNC Full Communication mode. */

  do /* FETA_COMM_PNC_STATE_TRANSITION */
  {
    /* #20 PNC shall stay in Prepare Sleep state if PNC Prepare Sleep timer is running. */
    if ( (ComM_GetPncPSleepTimer( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ) > 0u) && (localhighestComModeReq <= COMM_PNC_PREPARE_SLEEP) )
    {
      localhighestComModeReq = COMM_PNC_PREPARE_SLEEP;
    }

    if ( localhighestComModeReq != ComM_GetActivePncComMode( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ) )
    {
      switch ( ComM_GetActivePncComMode( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ) )
      {
        /* #30 State transition from COMM_PNC_NO_COMMUNICATION state. */
      case COMM_PNC_NO_COMMUNICATION:
        switch ( localhighestComModeReq )
        {
          /* COMM_PNC_NO_COMMUNICATION -> COMM_PNC_REQUESTED */
        case COMM_PNC_REQUESTED:
          ComM_PncModeSendSignal( ComM_CurrentPn, TRUE, COMM_SEND_ALL_SIG );
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_REQUESTED, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_REQUESTED );
          if ( ComM_HasUserModeNotiFunc() )
          {
            ComM_SetNotifyPncStateChange( TRUE, COMM_MASTER_PARTITION_IDX );
          }
          ComM_PncClearWakeup( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX );
          break;

          /* COMM_PNC_NO_COMMUNICATION -> COMM_PNC_REQUESTED_PASSIVE */
        case COMM_PNC_REQUESTED_PASSIVE:
          ComM_PncModeSendSignal( ComM_CurrentPn, TRUE, COMM_SEND_ACTIVE_SIG_ONLY );
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_REQUESTED_PASSIVE, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_REQUESTED );
          if ( ComM_HasUserModeNotiFunc() )
          {
            ComM_SetNotifyPncStateChange( TRUE, COMM_MASTER_PARTITION_IDX );
          }
          ComM_PncClearWakeup( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX );
          break;

          /* COMM_PNC_NO_COMMUNICATION -> COMM_PNC_READY_SLEEP */
        case COMM_PNC_READY_SLEEP:
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_READY_SLEEP, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_READY_SLEEP );
          if ( ComM_HasUserModeNotiFunc() )
          {
            ComM_SetNotifyPncStateChange( TRUE, COMM_MASTER_PARTITION_IDX );
          }
          break;

          /* COMM_PNC_NO_COMMUNICATION -> COMM_PNC_PREPARE_SLEEP */
        case COMM_PNC_PREPARE_SLEEP:
          /* Note: it is ensured that transition is only performed if COMM_PNC_PS_TIMER > 0 and
          * (COMM_SYNCHRONOUS_WAKE_UP == STD_ON or COMM_WAKEUPENABLEDOFPNC == STD_ON) */
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_PREPARE_SLEEP, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_PREPARE_SLEEP );
          ComM_SetPncPSleepTimer( ComM_CurrentPn, COMM_PNC_PS_TIMER, COMM_MASTER_PARTITION_IDX );
          if ( ComM_HasUserModeNotiFunc() )
          {
            ComM_SetNotifyPncStateChange( TRUE, COMM_MASTER_PARTITION_IDX );
          }
          break;

        default: /* COV_COMM_MISRA */
          break;
        }
        break;

        /* #40 State transition from COMM_PNC_PREPARE_SLEEP state. */
      case COMM_PNC_PREPARE_SLEEP:
        switch ( localhighestComModeReq )
        {
          /* COMM_PNC_PREPARE_SLEEP -> COMM_PNC_REQUESTED */
        case COMM_PNC_REQUESTED:
          ComM_PncModeSendSignal( ComM_CurrentPn, TRUE, COMM_SEND_ALL_SIG );
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_REQUESTED, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_REQUESTED );
          break;

          /* COMM_PNC_PREPARE_SLEEP -> COMM_PNC_REQUESTED_PASSIVE */
        case COMM_PNC_REQUESTED_PASSIVE:
          ComM_PncModeSendSignal( ComM_CurrentPn, TRUE, COMM_SEND_ACTIVE_SIG_ONLY );
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_REQUESTED_PASSIVE, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_REQUESTED );
          break;

          /* COMM_PNC_PREPARE_SLEEP -> COMM_PNC_READY_SLEEP */
        case COMM_PNC_READY_SLEEP:
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_READY_SLEEP, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_READY_SLEEP );
          break;

          /* COMM_PNC_PREPARE_SLEEP -> COMM_PNC_NO_COMMUNICATION */
        case COMM_PNC_NO_COMMUNICATION:
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_NO_COMMUNICATION );
          if ( ComM_HasUserModeNotiFunc() )
          {
            ComM_SetNotifyPncStateChange( TRUE, COMM_MASTER_PARTITION_IDX );
          }
          break;

        default: /* COV_COMM_MISRA */
          break;
        }

        /* When leaving COMM_PNC_PREPARE_SLEEP state, always stop the timer and reset ComM_BusPncComModeReq[]
        if no EIRA = 1 signal received for the PNC. Needed e.g. in case PNC is not considered by Nm filter mask.
        Possible scenario: Synchronous Wake-up, FULL_COM request, NO_COM request. In this case highestComModeReq
        shall be COMM_PNC_NO_COMMUNICATION and PNC shall enter COMM_PNC_PREPARE_SLEEP. (ESCAN00077600). */

        /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
        ComM_SetPncPSleepTimer( ComM_CurrentPn, 0, COMM_MASTER_PARTITION_IDX );
        ComM_PncClearWakeup( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX );
        break;

        /* #50 State transition from COMM_PNC_READY_SLEEP state. */
      case COMM_PNC_READY_SLEEP:
        switch ( localhighestComModeReq )
        {
          /* COMM_PNC_READY_SLEEP -> COMM_PNC_REQUESTED */
        case COMM_PNC_REQUESTED:
          ComM_PncModeSendSignal( ComM_CurrentPn, TRUE, COMM_SEND_ALL_SIG );
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_REQUESTED, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_REQUESTED );
          break;

          /* COMM_PNC_READY_SLEEP -> COMM_PNC_REQUESTED_PASSIVE, if COMM_PNC_ACTIVE_COORD == STD_ON */
        case COMM_PNC_REQUESTED_PASSIVE:
          ComM_PncModeSendSignal( ComM_CurrentPn, TRUE, COMM_SEND_ACTIVE_SIG_ONLY );
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_REQUESTED_PASSIVE, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_REQUESTED );
          break;

        case COMM_PNC_NO_COMMUNICATION:
        case COMM_PNC_PREPARE_SLEEP:
          /* Always indicate COMM_PNC_PREPARE_SLEEP to BswM, even if the state is omitted in case COMM_PNC_PS_TIMER == 0. */
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_PREPARE_SLEEP );
          if ( !ComM_HasPncPSleepTimer( COMM_MASTER_PARTITION_IDX ) )
          {
            /* Special case if COMM_PNC_PREPARE_SLEEP == 0: COMM_PNC_READY_SLEEP -> COMM_PNC_NO_COMMUNICATION (ESCAN00067357) */
            /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
            ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX );
            BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_NO_COMMUNICATION );
            if ( ComM_HasUserModeNotiFunc() )
            {
              ComM_SetNotifyPncStateChange( TRUE, COMM_MASTER_PARTITION_IDX );
            }
          }
          else /* COMM_PNC_PS_TIMER > 0 */
          {
            /* COMM_PNC_READY_SLEEP -> COMM_PNC_PREPARE_SLEEP */
            /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
            ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_PREPARE_SLEEP, COMM_MASTER_PARTITION_IDX );
            ComM_SetPncPSleepTimer( ComM_CurrentPn, COMM_PNC_PS_TIMER, COMM_MASTER_PARTITION_IDX );
          } /* COMM_PNC_PS_TIMER */
          break;

        default: /* COV_COMM_MISRA */
          break;
        }
        break;

        /* #60 State transition from COMM_PNC_REQUESTED_PASSIVE state. */
      case COMM_PNC_REQUESTED_PASSIVE:
        if ( localhighestComModeReq == COMM_PNC_REQUESTED )
        {
          /* COMM_PNC_REQUESTED_PASSIVE -> COMM_PNC_REQUESTED, if COMM_PNC_ACTIVE_COORD == STD_ON */
          ComM_PncModeSendSignal( ComM_CurrentPn, TRUE, COMM_SEND_PASSIVE_SIG_ONLY );
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_REQUESTED, COMM_MASTER_PARTITION_IDX );
        }
        else
        {
          /* COMM_PNC_REQUESTED_PASSIVE -> COMM_PNC_READY_SLEEP, if COMM_PNC_ACTIVE_COORD == STD_ON.
          Note: there are only two possible transitions from COMM_PNC_REQUESTED_PASSIVE state, therefore using if..else is ok */
          ComM_PncModeSendSignal( ComM_CurrentPn, FALSE, COMM_SEND_ACTIVE_SIG_ONLY );
# if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON )
          ComM_SynchronizedPncShutdownHandling( ComM_CurrentPn );
# endif
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_READY_SLEEP, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_READY_SLEEP );
        }
        break;

        /* #70 State transition from COMM_PNC_REQUESTED state. */
      case COMM_PNC_REQUESTED:
        if ( localhighestComModeReq == COMM_PNC_REQUESTED_PASSIVE )
        {
          /* COMM_PNC_REQUESTED -> COMM_PNC_REQUESTED_PASSIVE, if COMM_PNC_ACTIVE_COORD == STD_ON */
          ComM_PncModeSendSignal( ComM_CurrentPn, FALSE, COMM_SEND_PASSIVE_SIG_ONLY );
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_REQUESTED_PASSIVE, COMM_MASTER_PARTITION_IDX );
        }
        else
        {
          /* COMM_PNC_REQUESTED -> COMM_PNC_READY_SLEEP
          Note: there are only two possible transitions from COMM_PNC_REQUESTED state, therefore using if..else is ok */
          ComM_PncModeSendSignal( ComM_CurrentPn, FALSE, COMM_SEND_ALL_SIG );
# if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON )
          ComM_SynchronizedPncShutdownHandling( ComM_CurrentPn );
# endif
          /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
          ComM_SetActivePncComMode( ComM_CurrentPn, COMM_PNC_READY_SLEEP, COMM_MASTER_PARTITION_IDX );
          BswM_ComM_CurrentPNCMode( ComM_GetPncIdOfPnc( ComM_CurrentPn ), COMM_PNC_READY_SLEEP );
        }
        break;

      default: /* COV_COMM_MISRA */
        break;
      }
    }
  } while ( localhighestComModeReq != ComM_GetActivePncComMode( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ) );
}

# if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON )
/***********************************************************************************************************************
 *  ComM_SynchronizedPncShutdownHandling()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
COMM_LOCAL_INLINE FUNC( void, COMM_CODE ) ComM_SynchronizedPncShutdownHandling( ComM_PncIterType ComM_CurrentPn )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PncChannelMappingEndIdxOfPncPbType pncChIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine if a Synchronized PNC Shutdown should be forwarded towards the Nm for the provided PNC, if an ECU is
  *      1) in the role of an intermediate PNC Coordinator
  *      2) in the role of a Top-Level PNC Coordinator */

  if ( ComM_IsIntermediatePncCoordinatorOfPnc( ComM_CurrentPn ) )
  {
    if ( ComM_IsForwardSynchronizedPncShutdownIndicated( ComM_CurrentPn, COMM_MASTER_PARTITION_IDX ) )
    {
      for ( pncChIdx = ComM_GetPncChannelMappingStartIdxOfPncPb( ComM_CurrentPn ); /* FETA_COMM_CSL_USE_CASE */
            pncChIdx < ComM_GetPncChannelMappingEndIdxOfPncPb( ComM_CurrentPn ); pncChIdx++ )
      {
        if ( ComM_GetGwTypeOfChannel( ComM_GetPncChannelMapping( pncChIdx ) ) == COMM_GATEWAY_TYPE_ACTIVE )
        {
          (void)ComM_Nm_ServiceHandler( NM_REQUESTSYNCHRONIZEDPNCSHUTDOWN, ComM_GetPncChannelMapping( pncChIdx ), ComM_GetPncIdOfPnc( ComM_CurrentPn ) );
        }
      }
      /* @ assert ComM_CurrentPn < ComM_GetSizeOfPnc(); */ /* VCA_COMM_PNC_IDX */
      ComM_SetForwardSynchronizedPncShutdownIndicated( ComM_CurrentPn, FALSE, COMM_MASTER_PARTITION_IDX );
    }
  }
  else if ( ComM_IsTopLevelPncCoordinatorOfPnc( ComM_CurrentPn ) )
  {
    for ( pncChIdx = ComM_GetPncChannelMappingStartIdxOfPncPb( ComM_CurrentPn ); /* FETA_COMM_CSL_USE_CASE */
          pncChIdx < ComM_GetPncChannelMappingEndIdxOfPncPb( ComM_CurrentPn ); pncChIdx++ )
    {
      (void)ComM_Nm_ServiceHandler( NM_REQUESTSYNCHRONIZEDPNCSHUTDOWN, ComM_GetPncChannelMapping( pncChIdx ), ComM_GetPncIdOfPnc( ComM_CurrentPn ) );
    }
  }
  else
  {
    /* Intentionally left empty.*/
  }
}
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_Pnc_LimitPncToChannelRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_Pnc_LimitPncToChannelRouting( PNCHandleType Pnc, NetworkHandleType Channel, boolean Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX ) )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncIterType pncIndex;
    ComM_SizeOfPncLimitationType index = 0u;
    ComM_PncLimitationType value = 0u;

    /* Get the internal PNC index from the given PNC identifier */
    for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( Pnc == ComM_GetPncIdOfPnc( pncIndex ) )
      {
        break;
      }
    }

    /* #10 Check parameter 'Pnc': it must be a valid PNC identifier. */
    if ( pncIndex < ComM_GetSizeOfPnc() )
    {
      /* #20 If parameter checks are successful, store the PNC to Channel Routing Limitation status. */
      if ( Status == TRUE )
      {
        /* Limitation is activated */
        value = (ComM_PncLimitationType)(ComM_GetPncLimitation( (ComM_SizeOfPncLimitationType)(ComM_GetPncLimitationStartIdxOfChannelPb( Channel ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), COMM_MASTER_PARTITION_IDX ) | ComM_GetSetMaskOfPnc( pncIndex ));
      }
      else
      {
        /* Limitation is de-activated */
        value = (ComM_PncLimitationType)(ComM_GetPncLimitation( (ComM_SizeOfPncLimitationType)(ComM_GetPncLimitationStartIdxOfChannelPb( Channel ) + ComM_GetSignalByteIndexOfPncPb( pncIndex )), COMM_MASTER_PARTITION_IDX ) & ComM_GetClearMaskOfPnc( pncIndex ));
      }

      index = (ComM_SizeOfPncLimitationType)(ComM_GetPncLimitationStartIdxOfChannelPb( Channel ) + ComM_GetSignalByteIndexOfPncPb( pncIndex ));

      if ( index < ComM_GetSizeOfPncLimitation( COMM_MASTER_PARTITION_IDX ) )
      {
        ComM_SetPncLimitation( index, value, COMM_MASTER_PARTITION_IDX );
        retVal = E_OK;
      }
      else
      {
        errorId = COMM_E_WRONG_GENERATED_DATA;
      }
    }
    else
    {
      errorId = COMM_E_WRONG_PARAMETERS;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_LIMITPNCTOCHANNELROUTING, errorId );

  return retVal;
}


/**********************************************************************************************************************
 *  ComM_PncGetGwChannelRequest()
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
FUNC( ComM_StateType, COMM_CODE ) ComM_PncGetGwChannelRequest( NetworkHandleType channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PncIterType pncIndex;
  ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;
  ComM_StateType gwChannelComMode;
  ComM_PncModeType tmpPNComModeReq;
  ComM_PncModeType highestPNComModeReq = COMM_PNC_NO_COMMUNICATION;
  boolean eraRequestReceivedOnChannel = FALSE;
  boolean pncRequestIsPending = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if ( (ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX )) && (ComM_PncIsEraRequestReceivedOnChannel( channel ) == TRUE) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    /* Routing to this channel is not limited because an ERA signal containing a PNC request is received on the channel */
    eraRequestReceivedOnChannel = TRUE;
  }

  /* #10 Iterate over PNCs mapped to the channel and calculate the highest requested mode of the PNCs.
  *     Break the iteration once the highest possible mode (COMM_PNC_REQUESTED) is detected. */
  if ( ComM_HasPncChannelMapping() )
  {
    for ( pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb( channel ); /* FETA_COMM_CSL_USE_CASE */
        (pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb( channel )) && (highestPNComModeReq != COMM_PNC_REQUESTED);
         pncPbIndIter++ )
    {
      pncIndex = (ComM_PncIterType)ComM_GetPncPbInd( pncPbIndIter );

      /* Found a PN assigned to the current channel. Calculate the highest PN requested mode of all PNs on the channel */
      tmpPNComModeReq = ComM_CalculateHighestPNComMode( pncIndex, TRUE ); /* the 2nd parameter is not evaluated */
      if ( tmpPNComModeReq >= COMM_PNC_REQUESTED )
      {
        if ( ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX ) )
        {
          pncRequestIsPending = TRUE;
          /* PNC mode may control the channel mode only if Routing Limitation state of the channel is COMM_PNCROUTING_NOT_LIMITED,
          * i.e. if routing of this PNC is not limited on the channel or limitation is de-activated due to reception of ERA=1. Otherwise ignore the PNC mode.
          * Note: PNC mode does not control the channel mode if limitation is de-activated because Nm is in state NM_REPEAT_MESSAGE or
          * if PNC is woken up by EIRA=1 (not ERA). */
          if ( (!ComM_IsPncLimitation( channel, pncIndex, COMM_MASTER_PARTITION_IDX )) || (eraRequestReceivedOnChannel == TRUE) ) /* PRQA S 3415*/ /* MD_ComM_3415 */
          {
            highestPNComModeReq = tmpPNComModeReq;
          }
        }
        else
        {
          highestPNComModeReq = tmpPNComModeReq;
        }
      }
    }
  }

  /* #20 Get the requested channel state depending on the highest requested mode of the PNCs and the Gateway Type of the channel. */
  gwChannelComMode = ComM_PncGetGwChannelState( channel, highestPNComModeReq );

  /* #30 If PNC Routing Limitation is enabled, update the limitation state of the channel. */
  ComM_PncRoutingLimitationSetState( channel, highestPNComModeReq, pncRequestIsPending );

  COMM_DUMMY_STATEMENT( eraRequestReceivedOnChannel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return gwChannelComMode;
}


/**********************************************************************************************************************
 *  ComM_PncGetGwChannelState()
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
FUNC( ComM_StateType, COMM_CODE ) ComM_PncGetGwChannelState( NetworkHandleType channel, ComM_PncModeType highestPNComModeReq )
{
  /* The function is used in conditions, which are not always evaluated because of 'short-circuit evaluation'.
  * Therefore the function is not allowed to change the module' state i.e. write global variables.
  */

  /* ----- Local Variables ---------------------------------------------- */
  ComM_StateType gwChannelComMode = COMM_NO_COM_NO_PENDING_REQUEST;
  ComM_GwTypeOfChannelType gwTypeOfChannel = ComM_GetGwTypeOfChannel( channel );

  /* ----- Implementation ----------------------------------------------- */
  /* #10 The requested channel state depends on the highest requested mode of the PNCs and the Gateway Type of the channel:
  *      COMM_FULL_COM_NETWORK_REQUESTED if at least one PNC on the channel:
  *      1) is requested internally by a PNC user or externally via ERA that is originated from a channel with Gateway Type ACTIVE.
  *      2) is requested externally via ERA that is originated from a channel with Gateway Type PASSIVE and the current channel has Gateway Type ACTIVE.
  *      3) is requested externally via ERA that is originated from a channel with Gateway Type PASSIVE and the current channel is a managed channel.
  *      4) is requested externally via ERA that is originated from a channel with Gateway Type PASSIVE and the current channel has Gateway Type PASSIVE, and the PNC-0-Vector Avoidance feature is disabled.
  *     The requested state of the channel is COMM_FULL_COM_READY_SLEEP if at least one PNC on the channel is requested externally via ERA that
  *     is originated from a channel with Gateway Type PASSIVE, and the current channel has Gateway Type PASSIVE and the PNC-0-Vector Avoidance feature is enabled.
  *     The requested state of the channel is COMM_NO_COM_NO_PENDING_REQUEST if none of the above applies. */
  if ( highestPNComModeReq == COMM_PNC_REQUESTED )
  {
    gwChannelComMode = COMM_FULL_COM_NETWORK_REQUESTED;
  }
  else if ( highestPNComModeReq == COMM_PNC_REQUESTED_PASSIVE )
  {
    if ( gwTypeOfChannel == COMM_GATEWAY_TYPE_ACTIVE )
    {
      /* Channels with GW type ACTIVE shall be requested actively. */
      gwChannelComMode = COMM_FULL_COM_NETWORK_REQUESTED;
    }
    /* Note: A GW type check is not needed as a managed channel always has GW type NONE. */
    else if ( ComM_GetManagingChannelIdOfChannel( channel ) != COMM_NO_MANAGINGCHANNELIDOFCHANNEL )
    {
      /* Managed Channels with GW type NONE shall be requested actively. */
      gwChannelComMode = COMM_FULL_COM_NETWORK_REQUESTED;
    }
    else
    {
      /* If PNC-0-Vector Avoidance feature is enabled
      * Channels with GW type PASSIVE shall not be requested actively. Note: no check for Gw type needed because it is ensured
      * per validation rule that a coordinated channel always has GW type ACTIVE or PASSIVE and never has Gw type NONE.
      * Managed channels always have GW type NONE and do not contribute to the coordination of the PNC. */
      if ( ComM_HasPnc0VectorAvoidance() )
      {
        gwChannelComMode = COMM_FULL_COM_READY_SLEEP;
      }
      else
      {
        /* Cannels with GW type PASSIVE shall be requested actively if the feature PNC-0-Vector Avoidance is not enabled. */
        gwChannelComMode = COMM_FULL_COM_NETWORK_REQUESTED;
      }
    }
  }
  else
  {
    gwChannelComMode = COMM_NO_COM_NO_PENDING_REQUEST;
  }

  return gwChannelComMode;
}


/**********************************************************************************************************************
 *  ComM_PncInFullComWakesUpTheChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( ComM_StateType, COMM_CODE ) ComM_PncInFullComWakesUpTheChannel( NetworkHandleType channel, ComM_StateType highestRequest )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_StateType localhighestRequest = highestRequest;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return COMM_FULL_COM_READY_SLEEP if current highest request is COMM_NO_COM_NO_PENDING_REQUEST and
  *   at least one PNC on this channel is in PNC_FULL_COMMUNICATION mode.
  *   Otherwise return the current highest request (no changes). */

  /* Note: if COMM_FULL_COM_READY_SLEEP ComM ensures that the channel is at least in COMM_FULL_COM_READY_SLEEP state. */
  /* ESCAN00066290: There is no downgrade of highest request. If detected COMM_FULL_COM_NETWORK_REQUESTED, then no need to check PNCs anymore */
  if ( (localhighestRequest == COMM_NO_COM_NO_PENDING_REQUEST) && (ComM_HasPncChannelMapping()))
  {
    ComM_PncIterType pncIndex;
    ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;

    for ( pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb( channel ); /* FETA_COMM_CSL_USE_CASE */
          pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb( channel );
          pncPbIndIter++ )
    {
      pncIndex = (ComM_PncIterType)ComM_GetPncPbInd( pncPbIndIter );

      if ( ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) != COMM_PNC_NO_COMMUNICATION )
      {
        localhighestRequest = COMM_FULL_COM_READY_SLEEP;
        break;
      }
    }
  }

  COMM_DUMMY_STATEMENT( channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return localhighestRequest;
}


/***********************************************************************************************************************
 *  ComM_HandlePncPrepareSleepTimer()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_HandlePncPrepareSleepTimer( void )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( (ComM_HasPncPSleepTimer( COMM_MASTER_PARTITION_IDX )) && (ComM_HasPncPb()) )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_PncIterType pncIndex;

    /* #10 For each PNC check the PNC Prepare Sleep Timer. If the timer is started, decrement it. If the timer is expired
    *     and PNC is not requested, set the bus requested mode to COMM_PNC_NO_COMMUNICATION to allow PNC to enter COMM_PNC_NO_COMMUNICATION. */
    for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( ComM_GetPncPSleepTimer( pncIndex, COMM_MASTER_PARTITION_IDX ) > 0u ) /* PNC state is always COMM_PNC_PREPARE_SLEEP in this case, no need to check it. */
      {
        ComM_DecPncPSleepTimer( pncIndex, COMM_MASTER_PARTITION_IDX );
        if ( (ComM_GetBusPncComModeReq( pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_PREPARE_SLEEP) && (ComM_GetPncPSleepTimer( pncIndex, COMM_MASTER_PARTITION_IDX ) == 0u) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
        {
          ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_NO_COMMUNICATION, COMM_MASTER_PARTITION_IDX );
        }
      }
    }
  }
}


/***********************************************************************************************************************
 *  ComM_PncModeArbitration()
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
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC( void, COMM_CODE ) ComM_PncModeArbitration( NetworkHandleType channel )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasPncChannelMapping() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncChannelMappingEndIdxOfPncPbType pncChIndex;
    boolean allCoordinatedChannelsAreInFullCom;
    boolean pncCanBeRequested;
    ComM_PncModeType highestComModeReq;
    ComM_PncPbIndEndIdxOfChannelPbType pncPbIndIter;
    ComM_PncIterType pncIndex;

    for ( pncPbIndIter = ComM_GetPncPbIndStartIdxOfChannelPb( channel ); /* FETA_COMM_CSL_USE_CASE */
      pncPbIndIter < ComM_GetPncPbIndEndIdxOfChannelPb( channel );
      pncPbIndIter++ )
    {
      /* For each PNC, the boolean value for allCoordinatedChannelsAreInFullCom and pncCanBeRequested must be recalculated. */
      pncIndex = (ComM_PncIterType)ComM_GetPncPbInd( pncPbIndIter );
      allCoordinatedChannelsAreInFullCom = TRUE;
      pncCanBeRequested = TRUE; /* PRQA S 2982 */ /* MD_ComM_2982 */
      highestComModeReq = COMM_PNC_NO_COMMUNICATION; /* PRQA S 2982 */ /* MD_ComM_2982 */
      if ( ComM_HasCoordinatedOfPnc() )
      {
        pncCanBeRequested = FALSE; /* PRQA S 2982 */ /* MD_ComM_2982 */

        /* #10 If a PNC has channels, a potential state change of the PNC can be performed in context of current channel if
        *     1) the PNC is mapped to the current channel and
        *     2) all coordinated PNC channels are in FULL_COM mode in case PNC Gateway functionality is enabled.
        *     If the feature PNC to Channel Routing Limitation is enabled, only channels without limitation are considered.
        *     E.g. channels with limitation can be in NO_COM mode but PNC can change its state nevertheless. */
        for ( pncChIndex = ComM_GetPncChannelMappingStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
          pncChIndex < ComM_GetPncChannelMappingEndIdxOfPncPb( pncIndex );
          pncChIndex++ )
        {
          /* Example: assume PNC is on CAN1 and CAN2 and routing is limited on CAN1. If PNC is requested, it changes to PNC_REQUESTED
          * as soon as CAN2 is in FULL_COM mode. The mode of CAN1 is not evaluated. */
          /* PRQA S 3415 2 */ /* MD_ComM_3415 */
          if ( (!ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX )) ||
            (ComM_GetPncToChannelRoutingState( ComM_GetPncChannelMapping( pncChIndex ), COMM_MASTER_PARTITION_IDX ) != COMM_PNCROUTING_LIMITED)
            )
          {
            /* Note: found a channel without Routing Limitation, it will be passed to ComM_CalculateHighestPNComMode() later */
            pncCanBeRequested = TRUE;
            if ( (ComM_GetActiveComMode( ComM_GetPncChannelMapping( pncChIndex ), COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
            {
              allCoordinatedChannelsAreInFullCom = FALSE;
            }
          }
        }

        /* #20 If a PNC can perform a state transition in the context of the current channel task, calculate the highest requested mode
        *     of the PNC and perform the corresponding state transition. */
        highestComModeReq = COMM_PNC_NO_COMMUNICATION;
      }
      if ( allCoordinatedChannelsAreInFullCom == TRUE )
      {
        /* Note: highestComModeReq shall be COMM_PNC_NO_COMMUNICATION (default) as long as at least one channel is not in FULL_COMM yet.
        * This is to ensure that a state transition (activation) of a coordinated PNC is performed AFTER all assigned channels have entered FULL_COMM. */
        highestComModeReq = ComM_CalculateHighestPNComMode( pncIndex, pncCanBeRequested );
      }

      if ( highestComModeReq != ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) )
      {
        ComM_PncStateTransition( pncIndex, highestComModeReq );
      }
      else if ( (ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX )) && (ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) > COMM_PNC_READY_SLEEP) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
      {
        ComM_PncRoutingLimitationHandleChannels( pncIndex );
      }
      else
      {
        /* nothing to do */
      }
    } /* Iteration over PNCs */
  }

  COMM_DUMMY_STATEMENT( channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}


/***********************************************************************************************************************
 *  ComM_PncModeArbitrationWithoutChannels()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_PncModeArbitrationWithoutChannels( void )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasPncWithoutChannels() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_PncIterType pncIndex;
    ComM_PncModeType highestComModeReq = COMM_PNC_NO_COMMUNICATION;

    /* #10 If Extended PNC Functionality is enabled, a PNC may have no channels assigned. A potential state change of the PNC
    *     is always to be performed in context of Channel 0.
    *     Note: The function is called for Channel 0 even if it is disabled in the current variant. However, it is not necessary
    *     to check ComM_IsEnabledOfChannel, because no PNC will be assigned to such a disabled channel. */

    for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( !ComM_IsPncChannelMappingUsedOfPncPb( pncIndex ) )
      {
        highestComModeReq = ComM_CalculateHighestPNComMode( pncIndex, TRUE ); /* 2nd parameter is not used */

        if ( highestComModeReq != ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) )
        {
          ComM_PncStateTransition( pncIndex, highestComModeReq );
        }
      }
    }
  }
}


/**********************************************************************************************************************
 *  ComM_PncStateChangeNotification()
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
 */
 /* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC( Std_ReturnType, COMM_CODE ) ComM_PncStateChangeNotification( void )
{
  /* ----- Implementation ----------------------------------------------- */

  /* ----- Local Variable ---------------------------------------------- */
  Std_ReturnType retValueOverall = E_OK;

  if ( ComM_HasUserModeNotiFunc() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_UserIterType userIndex;
    ComM_ModeType lowestUserMode;
    ComM_UserPncByteMaskEndIdxOfUserType iByteMaskIndex;
    ComM_PncIterType pncIndex;
    Std_ReturnType retValueUser = RTE_E_OK;

    /* #10 Iterate over users that have configured User Mode (state change) notification and are mapped to PNC. */
    for ( userIndex = 0u; userIndex < ComM_GetSizeOfUser(); userIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( ComM_GetUserModeNotiFunc( userIndex ) != NULL_PTR )
      {
        if ( ComM_IsPncUserOfUser( userIndex ) )
        {
          /* #20 Calculate the lowest mode of all PNCs assigned to the current user. If at least one PNC
          *     is in COMM_PNC_NO_COMMUNICATION mode, the lowest mode is COMM_NO_COMMUNICATION. */
          lowestUserMode = COMM_FULL_COMMUNICATION;

          for ( iByteMaskIndex = ComM_GetUserPncByteMaskStartIdxOfUser( userIndex ); iByteMaskIndex < ComM_GetUserPncByteMaskEndIdxOfUser( userIndex ); iByteMaskIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            for ( pncIndex = 0u; (lowestUserMode == COMM_FULL_COMMUNICATION) && (pncIndex < ComM_GetSizeOfPnc()); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */ /* PRQA S 3415 */ /* MD_ComM_3415 */
            {
              if ( ComM_GetPncIdOfPnc( pncIndex ) == ComM_GetPncIDOfUserPncByteMask( iByteMaskIndex ) )
              {
                if ( ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION )
                {
                  lowestUserMode = COMM_NO_COMMUNICATION;
                }
              }
            }
          }

          /* #30 If a new mode is available for the current user, notify it using the Mode Switch Interface (sender-receiver) 'ComM_CurrentMode'. */
          if ( lowestUserMode != ComM_GetLastStateChange( userIndex, COMM_MASTER_PARTITION_IDX ) )
          {
            switch ( lowestUserMode )
            {
            case COMM_FULL_COMMUNICATION:
              retValueUser = ComM_GetUserModeNotiFunc( userIndex )(RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION);
              break;
            case COMM_NO_COMMUNICATION:
              retValueUser = ComM_GetUserModeNotiFunc( userIndex )(RTE_MODE_ComMMode_COMM_NO_COMMUNICATION);
              break;

            default: /* COV_COMM_MISRA */
              break;
            }

            /* #40 If notification to the current user was successful, update the mode to ensure that the same one will not be notified again.
            *     Otherwise the notification will be processed again in the next task. */
            if ( retValueUser == RTE_E_OK )
            {
              ComM_SetLastStateChange( userIndex, lowestUserMode, COMM_MASTER_PARTITION_IDX ); /*lint !e661 ComM_LastStateChange[] has a size relation to ComM_User[] */
            }
            else
            {
              retValueOverall = E_NOT_OK;
            }
          }
        }
      }
    }
  }

  return retValueOverall;
}


/***********************************************************************************************************************
 *  ComM_ActivateAllPncs()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_ActivateAllPncs( void )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( (ComM_HasSynchronousWakeUp()) && (ComM_HasPncPSleepTimer( COMM_MASTER_PARTITION_IDX )) && (ComM_HasPncPb()) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_PncIterType pncIndex;

    /* #10 For each PNC check the current state. If the PNC is not yet in FULL_COMMUNICATION, activate the PNC by setting
    *     the bus-site requested communication mode to COMM_PNC_PREPARE_SLEEP. */
    for ( pncIndex = 0u; pncIndex < ComM_GetSizeOfPnc(); pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
    {
      if ( (ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION) && (ComM_GetBusPncComModeReq( pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
      {
        ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_PREPARE_SLEEP, COMM_MASTER_PARTITION_IDX );
      }
    }
  }
}


/***********************************************************************************************************************
 *  ComM_PncNmRequestHandling()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC( void, COMM_CODE ) ComM_PncNmRequestHandling( NetworkHandleType channel, ComM_PncIterType PncHandle )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_IsPncNmRequestOfChannel( channel ) )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_PncChannelMappingStartIdxOfPncPbType pncChannelMappingIndex;

    /*-#10 Call Nm_NetworkRequest() if PNC is activated and the parameter 'PNC Nm Request' is enabled on the channel.
    *     1) In case PNC Extended Functionality is enabled, only call Nm_NetworkRequest() if PNC is mapped to the channel.
    *     2) In case PNC Routing Limitation is enabled, only call Nm_NetworkRequest() if Routing is not limited on the channel.*/
    if ( ComM_HasPncExtendedFunctionality() )
    {
      for ( pncChannelMappingIndex = ComM_GetPncChannelMappingStartIdxOfPncPb( PncHandle ); /* FETA_COMM_CSL_USE_CASE */
            pncChannelMappingIndex < ComM_GetPncChannelMappingEndIdxOfPncPb( PncHandle );
            pncChannelMappingIndex++ )
      {
        if ( ComM_GetPncChannelMapping( pncChannelMappingIndex ) == channel )
        {
#   if (COMM_MULTIPARTITION == STD_ON)
          if ( ComM_GetPartitionConfigIdxOfChannel( channel ) != COMM_MASTER_PARTITION_IDX )
          {
            /* @ assert channel < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
            ComM_SetForwardNmNetworkRequestSyncCounter( channel, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) + 1u), COMM_MASTER_PARTITION_IDX );
          }
          else
#   endif
          {
            (void)ComM_Nm_ServiceHandler( NM_NETWORKREQUEST, channel, COMM_PNC_ID_NOT_USED );
          }
          break;
        }
      }
    }
    else
    {
      /* Note: In normal PNC functionality, additional PNC signals without a PNC - Channel assignment are only allowed for preparing
      * for a PB-L usecase. In this case, the additional signals are automatically filtered in the generator, so we don't need
      * to verify the PNC - Channel assignment. The generator guarantees that PNC Signals exist only if there is a corresponding
      * PNC - Channel assignment. */
      /* This is to prevent calling Nm_NetworkRequest() if Limitation is de-activated temporarily because Nm is in state NM_STATE_REPEAT_MESSAGE. */
      if ( (!ComM_HasPncToChannelRoutingState( COMM_MASTER_PARTITION_IDX )) || ((ComM_GetPncToChannelRoutingState( channel, COMM_MASTER_PARTITION_IDX ) & COMM_PNCROUTING_NOT_LIMITED) != 0x00u) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
      {
# if (COMM_MULTIPARTITION == STD_ON)
        if ( ComM_GetPartitionConfigIdxOfChannel( channel ) != COMM_MASTER_PARTITION_IDX )
        {
          /* @ assert channel < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
          ComM_SetForwardNmNetworkRequestSyncCounter( channel, (uint8)(ComM_GetForwardNmNetworkRequestSyncCounter( channel, ComM_GetPartitionConfigIdxOfChannel( channel ) ) + 1u), COMM_MASTER_PARTITION_IDX );
        }
        else
# endif
        {
          (void)ComM_Nm_ServiceHandler( NM_NETWORKREQUEST, channel, COMM_PNC_ID_NOT_USED );
        }
      }
    }
  }

  COMM_DUMMY_STATEMENT( PncHandle ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}
#endif


#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
