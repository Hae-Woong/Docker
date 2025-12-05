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
/*!         \file     ComM_EcuMBswM.c
 *          \unit     EcuMBswM
 *          \brief    Communication Manager ASR4
 *
 *          \details  Callback API services of the Autosar Communication Manager for the EcuMBswM unit.
 *
 *********************************************************************************************************************/

#define CCL_ASR_COMM_ECUMBSWM_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_Private_EcuMBswM.h"
#include "ComM_PNC_General.h"
#include "ComM_Private_Nm.h"
#include "SchM_ComM.h"

 /**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

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

#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  CALLBACK FUNCTIONS (AUTOSAR EcuM)
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_EcuM_ProcessWakeUpIndication()
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC( void, COMM_CODE ) ComM_EcuM_ProcessWakeUpIndication( NetworkHandleType Channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel ); /* PRQA S 2983 */ /* MD_ComM_2983 */

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasSynchronousWakeUp() )
  {
    uint8 ComM_ChannelIndex;

    /* #10 If Synchronous Wake-up is enabled, notify passive wake-up to all ComM channels being in NO_COM or SI_COM state.
    *     This is done by setting the bus-side requested state to FULL_COM (Network Requested or Ready Sleep). */
#if ( COMM_MULTIPARTITION == STD_ON )
    if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
    {
      SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

      ComM_SetEcuMWakeUpIndicationSyncCounter( Channel, (uint8)(ComM_GetEcuMWakeUpIndicationSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
      ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

      SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
    }
    else
#endif
    {
      ComM_SetEcuMWakeUpIndicated( 0u, TRUE, COMM_MASTER_PARTITION_IDX );

      for ( ComM_ChannelIndex = 0u; ComM_ChannelIndex < ComM_GetSizeOfChannel(); ComM_ChannelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        if ( (ComM_GetActiveComMode( ComM_ChannelIndex, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
        {
          ComM_SetBusComModeReq( ComM_ChannelIndex, ComM_GetWakeupStateOfChannel( ComM_ChannelIndex ), COMM_MASTER_PARTITION_IDX );

          /* #100 If NM Gateway Extension is enabled notify Nm_PassiveStartUp() for each channel with Nm. */
          if ( ComM_HasNmGwExt() )
          {
            if ( ComM_IsNmSupportOfChannel( ComM_ChannelIndex ) )
            {
              (void)ComM_Nm_ServiceHandler( NM_PASSIVESTARTUP, ComM_ChannelIndex, COMM_PNC_ID_NOT_USED );
            }
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
    /* The ActiveComMode can always be checked in the master partition since no wakeup indication from EcuM can be called when the channels (i.e. the Nm) is already shutting down. */
    if ( (ComM_GetActiveComMode( Channel, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u ) /* ESCAN00091475 */
    {

#if ( COMM_MULTIPARTITION == STD_ON )
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_2();

        ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), partitionIdx );
        /* Increment the sync counter for ComM_BusComModeReq by 1 (relative to the corresponding Master-Counter) */
        ComM_SetBusComModeReqSyncCounter( Channel, (uint8)(ComM_GetBusComModeReqSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );

        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_2();
      }
      else
#endif
      {
        ComM_SetBusComModeReq( Channel, ComM_GetWakeupStateOfChannel( Channel ), partitionIdx );
      }

      /* #300 If NM Gateway Extension is enabled notify Nm_PassiveStartUp(). */
      if ( ComM_HasNmGwExt() )
      {
        if ( ComM_IsNmSupportOfChannel( Channel ) )
        {
          (void)ComM_Nm_ServiceHandler( NM_PASSIVESTARTUP, Channel, COMM_PNC_ID_NOT_USED );
        }
      }
    }
    /* #310 If the indicated channel is a managed channel, notify the passive wake-up also to the corresponding managing channel
            by setting the bus-side requested state to FULL_COM. Ignore the wake-up in FULL_COM mode. */
    if ( ComM_GetManagingChannelIdOfChannel( Channel ) != COMM_NO_MANAGINGCHANNELIDOFCHANNEL )
    {
      if ( (ComM_GetActiveComMode( ComM_GetManagingChannelIdOfChannel( Channel ), COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
      {
#if ( COMM_MULTIPARTITION == STD_ON )
        if ( partitionIdx == COMM_MASTER_PARTITION_IDX )
#endif
        {
          ComM_SetBusComModeReq( ComM_GetManagingChannelIdOfChannel( Channel ), ComM_GetWakeupStateOfChannel( ComM_GetManagingChannelIdOfChannel( Channel ) ), partitionIdx );
        }

        if ( ComM_HasNmGwExt() )
        {
          (void)ComM_Nm_ServiceHandler( NM_PASSIVESTARTUP, ComM_GetManagingChannelIdOfChannel( Channel ), COMM_PNC_ID_NOT_USED );
        }
      }
    }
  } /* SynchronousWakeUp */

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

#if (COMM_PNC_SUPPORT == STD_ON)
/*********************************************************************************************************************
 *  ComM_EcuM_ProcessPNCWakeUpIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
 /* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC( Std_ReturnType, COMM_CODE ) ComM_EcuM_ProcessPNCWakeUpIndication( PNCHandleType PNCid )
{
  /* ----- Local Variable ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ( (ComM_HasWakeupEnabledOfPnc()) && (ComM_HasPncPSleepTimer( COMM_MASTER_PARTITION_IDX )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    if ( ComM_HasPncPb() )
    {
      /* ----- Local Variables ---------------------------------------------- */
      ComM_PncIterType pncIndex;
      ComM_PncChannelMappingEndIdxOfPncPbType pncChannelMappingIndex;
      ComM_ChannelIterType channelIndex;
      ComM_SizeOfPncType numOfPnc = ComM_GetSizeOfPnc();

      /* Get the internal PNC index from the given PNC identifier */
      for ( pncIndex = 0u; pncIndex < numOfPnc; pncIndex++ ) /* FETA_COMM_CSL_USE_CASE */
      {
        /* #10 Check parameter 'PNCid': it must be a valid PNC identifier. */
        if ( PNCid == ComM_GetPncIdOfPnc( pncIndex ) )
        {
          break;
        }
      }

      if ( pncIndex < numOfPnc )
      {
        /* #20 Wakeup the PNC if it is in NO_COM state and no wake-up occurred before. */
        if ( (ComM_GetActivePncComMode( pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION) &&
             (ComM_GetBusPncComModeReq( pncIndex, COMM_MASTER_PARTITION_IDX ) == COMM_PNC_NO_COMMUNICATION) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
        {
          ComM_SetBusPncComModeReq( pncIndex, COMM_PNC_PREPARE_SLEEP, COMM_MASTER_PARTITION_IDX );
        }

        /* #30 Propagate the wake-up to all channels that are mapped to the PNC and are in NO_COM or SI_COM state. */
        for ( pncChannelMappingIndex = ComM_GetPncChannelMappingStartIdxOfPncPb( pncIndex ); /* FETA_COMM_CSL_USE_CASE */
              pncChannelMappingIndex < ComM_GetPncChannelMappingEndIdxOfPncPb( pncIndex );
              pncChannelMappingIndex++ )
        {
          channelIndex = ComM_GetPncChannelMapping( pncChannelMappingIndex );

          if ( (ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
          {
            /* @ assert channelIndex < ComM_GetSizeOfChannel(); */ /* VCA_COMM_CHANNEL_CSL03 */
            ComM_SetBusComModeReq( channelIndex, ComM_GetWakeupStateOfChannel( channelIndex ), COMM_MASTER_PARTITION_IDX );
          }
        }

        /* #40 If Synchronous Wake-up is enabled, propagate the wake-up to all channels that are in NO_COM or SI_COM state. */
        if ( ComM_HasSynchronousWakeUp() )
        {
          for ( channelIndex = 0u; channelIndex < ComM_GetSizeOfChannel(); channelIndex++ ) /* FETA_COMM_CSL_USE_CASE */
          {
            if ( (ComM_GetActiveComMode( channelIndex, COMM_MASTER_PARTITION_IDX ) & COMM_FULL_COMMUNICATION) == 0u )
            {
              ComM_SetBusComModeReq( channelIndex, ComM_GetWakeupStateOfChannel( channelIndex ), COMM_MASTER_PARTITION_IDX );
            }
          }
        }

        retVal = E_OK;
      }
    }
  }

  return retVal;
}
#endif

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
