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
/*!         \file     ComM_MainFunction.c
 *          \unit     MainFunction
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the MainFunction unit.
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

#define CCL_ASR_COMM_MAINFUNCTION_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM.h"
#include "ComM_PNC_General.h"
#include "ComM_Channel_StatusAndState.h"
#include "ComM_Channel_StateTransitions.h"
#include "ComM_Private_Nm.h"
#include "ComM_ErrorReporting.h"
#include "ComM_Util.h"
#include "SchM_ComM.h"
#include "ComM_MainFunction.h"

#if (COMM_MULTIPARTITION == STD_ON)
# include "ComM_MultiPartitionSynchronization.h"
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
 * LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ComM_MainFunction_ProcessCycle()
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
 *
 */
 /* PRQA S 6010 1 */ /* MD_MSR_STPTH */
FUNC( void, COMM_CODE ) ComM_MainFunction_ProcessCycle( NetworkHandleType Channel )
{
  /* ----- Implementation ----------------------------------------------- */
#if (COMM_MULTIPARTITION == STD_ON)
  if ( ComM_IsMainFunctionNeedsSyncOfChannel( Channel ) )
  {
    ComM_UpdateDataFromSatellites();
  }
#endif

  /* If the channel is de-activated in the current variant, the main function shall do nothing
    * besides handling of PN prepare sleep timer. */
  if ( !ComM_IsInvalidHndOfChannel( Channel ) )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_StateType  highestComModeReq = COMM_NO_COM_NO_PENDING_REQUEST;     /* The highest ComMode request */
    boolean  internalRequest = FALSE;

    SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();

    ComM_ProcessSynchronousWakeUp();

    /* #10 Get the highest requested communication mode and perform a state transition accordingly. */
    highestComModeReq = ComM_CalculateHighestComMode( Channel, &internalRequest );

    /* #100 If Extended RAM Check functionality is used and communication needs to be started on the channel,
      *     limit the highest requested mode according to the RAM Check result. */
#if (COMM_EXTENDED_RAM_CHECK == STD_ON)
    if ( (ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX ) <= COMM_NO_COM_REQUEST_PENDING) && (highestComModeReq > COMM_NO_COM_REQUEST_PENDING) )
    {
      highestComModeReq = ComM_ChannelGetMaxStateAfterExtRamCheck( Channel, highestComModeReq );
    }
#endif

    ComM_ChannelStateTransition( Channel, highestComModeReq );

    /* #20 If User Mode Notification is enabled and BusSM indicated a state change on the channel or the last notification failed,
      *     notify the mode change of channel users via RTE Mode Switch Interface.
      *     Note: do it outside of the exclusive area to avoid that calls to RTE extend the interrupt lock time. */
    if ( ComM_User_HasModeNotificationOfChannel() )
    {
      if ( ComM_GetStateChange( Channel, COMM_MASTER_PARTITION_IDX ) != COMM_NO_STATE_CHANGE )
      {
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();
        ComM_StateChangeNotification( Channel );
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();
      }
    }

    /* #30 If Notification of Channel Requesters is enabled, update the data via RTE Sender-Receiver Interface if applicable.
      *     Note: do it outside of the exclusive area to avoid that calls to RTE extend the interrupt lock time. */
    if ( ComM_HasFullCommRequestNotif() )
    {
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();
      ComM_CurrentChannelRequestUpdate( Channel );
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();
    }

    /* #40 If Minimum FULL_COM Mode or Nm Light timer are enabled, handle the timers. */
    ComM_Channel_HandleTimers( Channel, internalRequest );

    SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();
  } /* ComM_IsInvalidHndOfChannel( Channel ) */

    /* #50 If Partial Networking is enabled and the current channel id is 0:
      *     1) handle the optional PNC Prepare Sleep Timer. Note: the timer value is same for all PNCs, it is calculated based on the cycle time of channel 0.
      *     2) if PNC Extended Functionality is enabled, handle PNCs without channels and perform state transitions if applicable. */
    /* #60 If Partial Networking is enabled and the current channel is enabled, handle PNCs with channels and perform state transitions if applicable. */
#if (COMM_PNC_SUPPORT == STD_ON)
  SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1();

  if ( Channel == 0u )
  {
    ComM_HandlePncPrepareSleepTimer();
    ComM_PncModeArbitrationWithoutChannels();
  }

  /* If the channel is de-activated in the current variant, the main function shall do nothing
  * besides handling of PN prepare sleep timer. */
  if ( !ComM_IsInvalidHndOfChannel( Channel ) )
  {
    ComM_PncModeArbitration( Channel );
  }

  SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1();

  /* #70 If User Mode Notification is enabled, notify the mode change of PNC users via RTE Mode Switch Port. */
  if ( ComM_IsNotifyPncStateChange( COMM_MASTER_PARTITION_IDX ) )
  {
    /* PNC users: a PNC changed the state from FULL_COM to NO_COM or vice versa or the last mode notification failed (ESCAN00067386) */
    if ( ComM_PncStateChangeNotification() == E_OK )
    {
      /* All PNC users have been successfully notified about new mode, reset the flag.
      * Otherwise, if at least one user mode notification failed, ComM will try again in the next task (ESCAN00067423) */
      ComM_SetNotifyPncStateChange( FALSE, COMM_MASTER_PARTITION_IDX );
    }
  }
#endif
}


#if ( COMM_MULTIPARTITION == STD_ON )
/**********************************************************************************************************************
 *  ComM_MainFunction_ProcessSatelliteCycle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_MainFunction_ProcessSatelliteCycle( NetworkHandleType Channel, CONST( ComM_PartitionConfigIdxOfChannelType, AUTOMATIC ) partitionIdx )
{
  /* ----- Implementation ----------------------------------------------- */
  /* If the channel is de-activated in the current variant, the satellite main function shall do nothing. */
  if ( !ComM_IsInvalidHndOfChannel( Channel ) )
  {
    /* ----- Local Variables ---------------------------------------------- */
    ComM_ModeType forwardingMode = COMM_NO_COMMUNICATION;
# if ( COMM_PNC_SUPPORT == STD_ON )
    P2CONST( ComM_PncSignalValuesType, AUTOMATIC, COMM_VAR_NO_INIT ) pncSignalValuesPtr;
# endif

    /* #10 Check forwarding-flags on each partition and forward calls to corresponding BusNm/SM on the current partition, if needed. */

    /* Check BusSmRequestComMode-Flag */
    if ( ComM_GetForwardRequestBusSmModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) == (uint8)(ComM_GetForwardRequestBusSmModeSyncCounter( Channel, partitionIdx ) + 1u) )
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
      ComM_SetForwardRequestBusSmModeSyncCounter( Channel, (uint8)ComM_GetForwardRequestBusSmModeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ), partitionIdx );
      forwardingMode = ComM_GetForwardingBusSmMode( Channel, COMM_MASTER_PARTITION_IDX );
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();
      if ( ComM_GetRequestBusSMModeApiOfChannel( Channel )(Channel, forwardingMode) != E_OK )
      {
        ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_MAINFUNCTION_SATELLITE, COMM_E_ERROR_IN_PROV_SERVICE );
      }
    }

    if ( ComM_HasFullNmTypeOfChannel() )
    {
      /* Check NetworkRequest-Flag */
      ComM_CheckSyncCounterAndForwardFunction( ComM_GetAddrForwardNmNetworkRequestSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ),
                                               ComM_GetAddrForwardNmNetworkRequestSyncCounter( Channel, partitionIdx ), ComM_Nm_ServiceHandler, NM_NETWORKREQUEST, Channel, COMM_PNC_ID_NOT_USED );

      /* Check NetworkRelease-Flag */
      ComM_CheckSyncCounterAndForwardFunction( ComM_GetAddrForwardNmNetworkReleaseSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ),
                                               ComM_GetAddrForwardNmNetworkReleaseSyncCounter( Channel, partitionIdx ), ComM_Nm_ServiceHandler, NM_NETWORKRELEASE, Channel, COMM_PNC_ID_NOT_USED );
    }

    /* Check PassiveStartUp-Flag */
    ComM_CheckSyncCounterAndForwardFunction( ComM_GetAddrForwardNmPassiveStartUpSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ),
                                             ComM_GetAddrForwardNmPassiveStartUpSyncCounter( Channel, partitionIdx ), ComM_Nm_ServiceHandler, NM_PASSIVESTARTUP, Channel, COMM_PNC_ID_NOT_USED );

# if ( COMM_PNC_SUPPORT == STD_ON )
    /* Check ComSendSignal-Flag */
    if ( ComM_GetForwardComSendSignalSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) == (uint8)(ComM_GetForwardComSendSignalSyncCounter( Channel, partitionIdx ) + 1u) )
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_5();
      ComM_SetForwardComSendSignalSyncCounter( Channel, (uint8)ComM_GetForwardComSendSignalSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ), partitionIdx );
      pncSignalValuesPtr = ComM_GetAddrPncSignalValues( ComM_GetPncSignalValuesStartIdxOfPncSignal( ComM_GetForwardingComSendSignalIndex( Channel, COMM_MASTER_PARTITION_IDX ) ), COMM_MASTER_PARTITION_IDX );
      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_5();

      (void)Com_SendSignal( ComM_GetSymbolicNameValueOfEiraTxSignalIndirection( ComM_GetForwardingComSendSignalIndex( Channel, COMM_MASTER_PARTITION_IDX ) ), pncSignalValuesPtr ); /* PRQA S 0315 */ /* MD_ComM_0315 */
    }
# endif
  }
}
#endif

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
