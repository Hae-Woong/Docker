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
/*!         \file     ComM_Dcm.c
 *          \unit     DCM
 *          \brief    Communication Manager ASR4
 *
 *          \details  Callback API services of the Autosar Communication Manager for the DCM unit.
 *
 *********************************************************************************************************************/

#define CCL_ASR_COMM_DCM_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_Private_Dcm.h"
#include "SchM_ComM.h"
#include "ComM.h"

#if (COMM_DCM_INDICATION == STD_ON)
# include "Dcm_Cbk.h"
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_DCM_NotificationHandling()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_DCM_NotificationHandling( NetworkHandleType Channel, P2CONST( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----- Implementation ----------------------------------------------- */

  /* #10 Notifies Dcm about the new mode. */
#if (COMM_DCM_INDICATION == STD_ON)
  switch ( *ComMode )
  {
    case COMM_FULL_COMMUNICATION:
      Dcm_ComM_FullComModeEntered( Channel );
    break;

    case COMM_SILENT_COMMUNICATION:
      Dcm_ComM_SilentComModeEntered( Channel );
    break;

    case COMM_NO_COMMUNICATION:
      Dcm_ComM_NoComModeEntered( Channel );
    break;
    default: /* COV_COMM_MISRA */
    break;
  }
#endif

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT_CONST( *ComMode ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

#if (COMM_DCM_INDICATION == STD_ON)
/*********************************************************************************************************************
 *  ComM_DCM_ProcessActiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( uint8, COMM_CODE ) ComM_DCM_ProcessActiveDiagnostic( NetworkHandleType Channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

    /* ----- Implementation ----------------------------------------------- */
    if ( ComM_HasKeepAwakeChannels() )
    {
      /* ----- Local Variables ---------------------------------------------- */
      boolean isPassiveOrLinSlaveOfChannel;

      /* #10 Check that Nm Variant of the channel is not PASSIVE or LINSLAVE. */
      isPassiveOrLinSlaveOfChannel = (boolean)ComM_IsPassiveOrLinSlaveOfChannel( Channel );
      if ( isPassiveOrLinSlaveOfChannel == TRUE )
      {
        errorId = COMM_E_DIAGNOSTIC_NOT_SUPPORTED; /* ESCAN00072762 */
      }
      else
      {
        /* #20 Store the active status of the diagnostic session on the channel. */
# if (COMM_MULTIPARTITION == STD_ON)
        ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
        if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
        {
          SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
          ComM_SetDcmRequestActive( Channel, TRUE, partitionIdx );
          /* Increment the sync counter for ComM_DcmRequestActive by 1 (relative to the corresponding Master-Counter) */
          ComM_SetDcmRequestActiveSyncCounter( Channel, (uint8)(ComM_GetDcmRequestActiveSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
          ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
          SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
        }
        else
# endif
        {
          ComM_SetDcmRequestActive( Channel, TRUE, COMM_MASTER_PARTITION_IDX );
        }
      }
    }
    else
    {
      errorId = COMM_E_DIAGNOSTIC_NOT_SUPPORTED; /* ESCAN00072762 */
    }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return errorId;
}

/*********************************************************************************************************************
 *  ComM_DCM_InactiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( uint8, COMM_CODE ) ComM_DCM_ProcessInactiveDiagnostic( NetworkHandleType Channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  if ( ComM_HasKeepAwakeChannels() )
  {
    /* ----- Local Variables ---------------------------------------------- */
    boolean isPassiveOrLinSlaveOfChannel;

    /* #10 Check that Nm Variant of the channel is not PASSIVE or LINSLAVE. */
    isPassiveOrLinSlaveOfChannel = (boolean)ComM_IsPassiveOrLinSlaveOfChannel( Channel );
    if ( isPassiveOrLinSlaveOfChannel == TRUE )
    {
      errorId = COMM_E_DIAGNOSTIC_NOT_SUPPORTED; /* ESCAN00072762 */
    }
    else
    {
      /* #20 Store the inactive status of the diagnostic session on the channel. */
      ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
# if (COMM_MULTIPARTITION == STD_ON)
      if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
      {
        SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_3();
        ComM_SetDcmRequestActive( Channel, FALSE, partitionIdx );
        /* Increment the sync counter for ComM_DcmRequestActive by 1 (relative to the corresponding Master-Counter) */
        ComM_SetDcmRequestActiveSyncCounter( Channel, (uint8)(ComM_GetDcmRequestActiveSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        ComM_SetGlobalPartitionSyncCounter( Channel, (uint8)(ComM_GetGlobalPartitionSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u), partitionIdx );
        SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_3();
      }
      else
# endif
      {
        ComM_SetDcmRequestActive( Channel, FALSE, partitionIdx );
      }
    }
  }
  else
  {
    errorId = COMM_E_DIAGNOSTIC_NOT_SUPPORTED; /* ESCAN00072762 */
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  return errorId;
}

/**********************************************************************************************************************
 *  ComM_DCM_GetActiveRequestStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_DCM_GetActiveRequestStatus( NetworkHandleType Channel, P2VAR( boolean, AUTOMATIC, COMM_APPL_VAR ) Status )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store TRUE if Dcm indicated active diagnostic. Store FALSE otherwise. */
  *Status = FALSE;
  if ( ComM_HasKeepAwakeChannels() )
  {
    /* ----- Local Variable ---------------------------------------------- */
    ComM_PartitionConfigIdxOfChannelType partitionIdx = COMM_MASTER_PARTITION_IDX;

# if ( COMM_MULTIPARTITION == STD_ON )
    if ( ComM_GetDcmRequestActiveSyncCounter( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) ) == (uint8)(ComM_GetDcmRequestActiveSyncCounter( Channel, COMM_MASTER_PARTITION_IDX ) + 1u) )
    {
      partitionIdx = ComM_GetPartitionConfigIdxOfChannel( Channel );
    }
# endif
    if ( ComM_IsDcmRequestActive( Channel, partitionIdx ) )
    {
      *Status = TRUE;
    }
  }

  COMM_DUMMY_STATEMENT( Channel ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}
#endif

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
