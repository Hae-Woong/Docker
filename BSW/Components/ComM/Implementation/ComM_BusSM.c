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
/*!         \file   ComM_BusSM.c
 *          \unit   BusSM
 *          \brief  Communication Manager ASR4
 *
 *         \details  Callback API services of the Communication Manager for the Bus state manager unit.
 *
 *********************************************************************************************************************/

#define CCL_ASR_COMM_BUSSM_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_Private_BusSM.h"
#include "ComM_ModeInhibition.h"
#include "ComM_Private_Dcm.h"
#include "BswM_ComM.h"
#include "ComM_Util.h"
#include "SchM_ComM.h"

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
 *  CALLBACK FUNCTIONS (AUTOSAR BusSM)
 **********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_BusSM_ProcessModeIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 3673, 6080 1 */ /* MD_ComM_3673, MD_MSR_STMIF */
FUNC( void, COMM_CODE ) ComM_BusSM_ProcessModeIndication( NetworkHandleType Channel, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_GetBusSmState( Channel, partitionIdx ) != *ComMode )
  {
    /* #10 If BusSM indicated a new mode, store it and propagate it to BswM. */
    /* ESCAN00065236 */
    ComM_SetBusSmState( Channel, *ComMode, partitionIdx );
#if ( COMM_MULTIPARTITION == STD_ON )
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      /* BswM notification is synchronized to Master-Partition */
    }
    else
#endif
    {
      BswM_ComM_CurrentMode( Channel, *ComMode );
    }

    /* #20 If a passive wake-up occurred during shutdown of FR, restore the target requested mode to start communication in the next main function. */
    if ( ComM_GetBusTypeOfChannel( Channel ) == COMM_BUS_TYPE_FR )
    {
      if ( *ComMode == COMM_NO_COMMUNICATION )
      {
        ComM_SetFrShutdown( Channel, FALSE, partitionIdx );

        /* ESCAN00081330: ComM_BusComModeReqFrShutdown can be COMM_FULL_COM_NETWORK_REQUESTED or COMM_FULL_COM_READY_SLEEP depending on Min Full Com Mode Duration. */
        if ( (ComM_GetBusComModeReqFrShutdown( Channel, partitionIdx ) != COMM_NO_COM_NO_PENDING_REQUEST)
#if ( COMM_MULTIPARTITION == STD_ON )
             || (ComM_GetBusComModeReqFrShutdown( Channel, COMM_MASTER_PARTITION_IDX ) != COMM_NO_COM_NO_PENDING_REQUEST) /* PRQA S 3415 */ /* MD_ComM_3415 */
#endif
              )
        {
#if (COMM_MULTIPARTITION == STD_ON)
          if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
          {
            SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

            ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), partitionIdx );
            ComM_SetBusComModeReqFrShutdown( Channel, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );
            ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
            ComM_SetBusComModeReqFrShutdownSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqFrShutdownSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
            ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

            SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
          }
          else
#endif
          {
            ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), partitionIdx );
            ComM_SetBusComModeReqFrShutdown( Channel, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );
          }
        }
      }
    }

    /* #30 Store the new mode to handle User Mode Notification in the next main function. */
    if ( ComM_User_HasModeNotificationOfChannel() )
    {
#if ( COMM_MULTIPARTITION == STD_ON )
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
        ComM_SetStateChange( Channel, *ComMode, partitionIdx );
                                                                                                /* Increment sync counter by 1 (relative to Master-Counter). */
        ComM_SetStateChangeSyncCounter( Channel, (uint8)(ComM_GetStateChangeSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
      }
      else
#endif
      {
        ComM_SetStateChange( Channel, *ComMode, partitionIdx );
      }
    }

    ComM_DCM_NotificationHandling( Channel , ComMode );

    /* #40 If NO_COM is indicated and Reset after Forcing NO_COM is enabled, check if a reset can be performed. */
    if ( *ComMode == COMM_NO_COMMUNICATION )
    {
      ComM_ChannelResetAfterForcingNoComm( partitionIdx );
    }
  }
}

/*********************************************************************************************************************
 *  ComM_BusSM_ProcessBusSleepMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_BusSM_ProcessBusSleepMode( NetworkHandleType Channel )
{
  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasLinslaveNmTypeOfChannel() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );

    if ( (ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) != 0u )
    {
      /* #10 If the current channel is in FULL_COMMUNICATION mode, store the Bus Sleep Mode indicated by the BusSM. */
      ComM_SetBusSleepModeIndicated( Channel, TRUE, partitionIdx );
#if (COMM_MULTIPARTITION == STD_ON)
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        /* Increment sync counter for ComM_BusSleepModeIndicated by 1 (relative to the corresponding Master-Counter) */
        ComM_SetBusSleepModeIndicatedSyncCounter( Channel, (uint8)(ComM_GetBusSleepModeIndicatedSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      }
#endif
    }

    /* #20 Set externally requested ComMode to COMM_NO_COM_NO_PENDING_REQUEST. */
#if ( COMM_MULTIPARTITION == STD_ON)
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

      ComM_SetBusComModeReq( Channel, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );
      /* Increment sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
      ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
    else
#endif
    {
      ComM_SetBusComModeReq( Channel, COMM_NO_COM_NO_PENDING_REQUEST, partitionIdx );
    }
  }
}

/***********************************************************************************************************************
 *  ComM_InternalBusSmModeIndication()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_InternalBusSmModeIndication( NetworkHandleType Channel, VAR( ComM_ModeType, AUTOMATIC ) ComMode )
{
  if ( ComM_HasSimulatedSmMode( COMM_MASTER_PARTITION_IDX ) )
  {
    /* #10 Request the communication mode from the simulated BusSM (i.e. for channels with COMM_BUS_TYPE_INTERNAL). */
    ComM_SetSimulatedSmMode( Channel, ComMode, COMM_MASTER_PARTITION_IDX );
    ComM_BusSM_ModeIndication( Channel, &ComMode ); /* PRQA S 1339 */ /* MD_ComM_1339 */
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( ComMode ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return E_OK;
}

/***********************************************************************************************************************
 *  ComM_InternalGetCurrentBusSmMode()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_InternalGetCurrentBusSmMode( NetworkHandleType Channel, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  if ( ComM_HasSimulatedSmMode( COMM_MASTER_PARTITION_IDX ) )
  {
    /* #10 Query the communication mode from the simulated BusSM (i.e. for channels with COMM_BUS_TYPE_INTERNAL). */
    *ComMode = ComM_GetSimulatedSmMode( Channel, COMM_MASTER_PARTITION_IDX );
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( ComMode ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return E_OK;
}

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
