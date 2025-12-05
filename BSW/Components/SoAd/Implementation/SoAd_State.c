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
/*!        \file  SoAd_State.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component State.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd.h"
#include "SoAd_Priv.h"
#include "SoAd_Cbk.h"
#include "SoAd_EventQueue.h"
#include "SoAd_TimeoutList.h"
#include "SoAd_SoCon.h"
#include "SoAd_RouteGrp.h"
#include "SoAd_Util.h"
#include "SoAd_Tx.h"
#include "SoAd_Rx.h"
#include "SoAd_Anomaly.h"
#include "SoAd_State_Int.h"
#include "SoAd_LocalAddr_Int.h"
#include "SoAd_BestMatch_Int.h"

#if ( SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE == SOAD_CONFIGURATION_VARIANT )
# include "EcuM_Error.h"
#endif /* SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE == SOAD_CONFIGURATION_VARIANT */

#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (SOAD_LOCAL) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL static
#endif /* !defined (SOAD_LOCAL) */

#if !defined (SOAD_LOCAL_INLINE) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (SOAD_LOCAL_INLINE) */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define SOAD_START_SEC_VAR_NO_INIT_32
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/*! Indicates the counter for shutdown finished timeout to switch to shutdown state if sockets could not be closed. */
SOAD_LOCAL VAR(SoAd_AtomicUint32Type, SOAD_VAR_NO_INIT) SoAd_ShutdownFinishedCnt;

#define SOAD_STOP_SEC_VAR_NO_INIT_32
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#define SOAD_START_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/*! Indicates if a shutdown was requested. */
SOAD_LOCAL VAR(boolean, SOAD_VAR_NO_INIT) SoAd_ShutdownRequested;

#define SOAD_STOP_SEC_VAR_NO_INIT_8
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#define SOAD_START_SEC_VAR_CLEARED_8
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/*! Indicates current state of the module state machine (Valid values: SOAD_STATE_*). */
SOAD_LOCAL VAR(SoAd_StateType, SOAD_VAR_CLEARED) SoAd_State = SOAD_STATE_UNINIT;

#define SOAD_STOP_SEC_VAR_CLEARED_8
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_State_ProcessInitState()
 *********************************************************************************************************************/
/*! \brief      Sets the module in shutdown in progress state if shutdown is requested.
 *  \details    This is only performed if the function is called for the main instance.
 *  \param[in]  InstMapIdx   Instance Map index.
 *                           [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre        Module is in state SOAD_STATE_INIT.
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_State_ProcessInitState(SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_State_ProcessShutdownInProgressState()
 *********************************************************************************************************************/
/*! \brief      Handle shutdown.
 *  \details    Check for open sockets and close them softly within timeout. Close all sockets at the end of the 
 *              timeout and finish shutdown.
 *  \param[in]  InstMapIdx   Instance Map index.
 *                           [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre        Module is in state SOAD_STATE_SHUTDOWN_IN_PROGRESS.
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_State_ProcessShutdownInProgressState(SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_State_CheckAllSocketsClosedState()
 *********************************************************************************************************************/
/*! \brief      Checks instance(s) for open sockets.
 *  \details    In the multi-instance use-case, all instances are checked for open sockets.
 *              Otherwise, only the open sockets of the current instance are checked.
 *  \param[in]  InstMapIdx   Instance Map index.
 *                           [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre        -
 *  \return     E_OK         All sockets of all instances are closed.
 *  \return     E_NOT_OK     Not all sockets are closed.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \note       Function is called only from the main instance.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_State_CheckAllSocketsClosedState(
  SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_State_FinishShutdown()
 *********************************************************************************************************************/
/*! \brief      Sets the module to shutdown state and calls the user notification if configured.
 *  \details    -
 *  \pre        Module is in state SOAD_STATE_SHUTDOWN_IN_PROGRESS.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \note       Function is called only from the main instance.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_State_FinishShutdown(void);

/**********************************************************************************************************************
 *  SoAd_State_CallShutdownFinishCallbacks()
 *********************************************************************************************************************/
/*! \brief      Calls all configured shutdown finished callbacks.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_State_CallShutdownFinishCallbacks(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_State_ProcessInitState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_State_ProcessInitState(SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a shutdown is requested in case the instance index is that of the main instance. */
#if ( SOAD_MULTI_INSTANCE == STD_ON )
  if ( InstMapIdx == 0u )
#endif /* SOAD_MULTI_INSTANCE == STD_ON */
  {
    if ( SoAd_ShutdownRequested == TRUE )
    {
      /* #20 Switch to the shutdown in progress state. */
      SoAd_Util_StoreU32(&SoAd_ShutdownFinishedCnt, SOAD_SHUTDOWN_FINISHED_WAIT, TRUE);                                /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      SoAd_State = SOAD_STATE_SHUTDOWN_IN_PROGRESS;
    }
  }

#if ( SOAD_MULTI_INSTANCE == STD_OFF )
  SOAD_DUMMY_STATEMENT(InstMapIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_MULTI_INSTANCE == STD_OFF */
} /* SoAd_State_ProcessInitState() */

/**********************************************************************************************************************
 *  SoAd_State_ProcessShutdownInProgressState()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_State_ProcessShutdownInProgressState(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ----------------------------------------------- */
  uint32 shutdownFinishedCnt = SoAd_Util_LoadU32(&SoAd_ShutdownFinishedCnt, TRUE);                                     /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process shutdown for main instance. */
#if ( SOAD_MULTI_INSTANCE == STD_ON )
  if ( InstMapIdx == 0u )
#endif /* SOAD_MULTI_INSTANCE == STD_ON */
  {
    /* #100 Check if timeout occurred. */
    if ( shutdownFinishedCnt == 0u )
    {
      /* #1000 Finish shutdown. */
      SoAd_State_FinishShutdown();
    }
    /* #101 Check if timeout occurs in the next main function cycle otherwise. */
    else if ( shutdownFinishedCnt == 1u )
    {
      /* #1010 Decrement shutdown finish counter. */
      SoAd_Util_FetchSubU32(&SoAd_ShutdownFinishedCnt, SOAD_INC_DEC_BY_ONE);                                           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

      /* #1011 Close all sockets. */
      SoAd_SoCon_CloseAllSockets(InstMapIdx);
    }
    /* #102 Process shutdown if timeout is running otherwise. */
    else
    {
      /* #1020 Decrement shutdown finish counter. */
      SoAd_Util_FetchSubU32(&SoAd_ShutdownFinishedCnt, SOAD_INC_DEC_BY_ONE);                                           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

      /* #1021 Check if all socket connections of the current instance are closed and trigger closing. */
      if ( SoAd_SoCon_CloseAllSoCons(InstMapIdx) == TRUE )
      {
        /* #10210 Close all UDP sockets of the current instance which are used to open socket connection on
         *        reception. */
        SoAd_SoCon_CloseAllSockets(InstMapIdx);

        /* #10211 Finish shutdown if all sockets of all instances are closed. */
        if ( SoAd_State_CheckAllSocketsClosedState(InstMapIdx) == E_OK )
        {
          SoAd_State_FinishShutdown();
        }
      }
    }
  }
#if ( SOAD_MULTI_INSTANCE == STD_ON )
  /* #20 Process shutdown for non-main instance otherwise. */
  else
  {
    /* #200 Check if timeout occurred (no actions required). */
    if ( shutdownFinishedCnt == 0u )
    {
      /* Nothing to do. */
    }
    /* #201 Check if timeout occurs in the next main function cycle otherwise. */
    else if ( shutdownFinishedCnt == 1u )
    {
      /* #2010 Close all sockets of the current instance. */
      SoAd_SoCon_CloseAllSockets(InstMapIdx);
    }
    /* #202 Process shutdown if timeout is running otherwise. */
    else
    {
      /* #2020 Check if all socket connections of the current instance are closed and trigger closing. */
      if ( SoAd_SoCon_CloseAllSoCons(InstMapIdx) == TRUE )
      {
        /* #20200 Close all UDP sockets of the current instance which are used to open socket connection on
         *        reception. */
        SoAd_SoCon_CloseAllSockets(InstMapIdx);
      }
    }
  }
#endif /* SOAD_MULTI_INSTANCE == STD_ON */
} /* SoAd_State_ProcessShutdownInProgressState() */

/**********************************************************************************************************************
 *  SoAd_State_CheckAllSocketsClosedState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_State_CheckAllSocketsClosedState(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{ 
  /* ----- Local Variables ----------------------------------------------- */
  SoAd_SizeOfInstanceType     instanceIdx; /* Not initialized: Avoid multiple declaration for same variable. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Avoid multiple declaration for same variable. */
  Std_ReturnType              retVal = E_OK;

  /* ----- Implementation ------------------------------------------------ */
#if ( SOAD_MULTI_INSTANCE == STD_ON )
{
  /* #10 In multi-instance use-case iterate over all instances and check if any instance has still open sockets. */
  SoAd_InstanceMapIterType instMapIter; /* Not initialized: For-loop counter. */

  for ( instMapIter = 0u; instMapIter < SoAd_GetSizeOfInstanceMap(); instMapIter++ )
  {
    instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(instMapIter);
    partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(instMapIter);

    if ( SoAd_Util_LoadU32(SoAd_GetAddrOpenSocksOfInstanceDyn(instanceIdx, partitionIdx), TRUE) > 0u )                 /* SBSW_SOAD_CONST_POINTER_FORWARD */
    {
      retVal = E_NOT_OK;
      break;
    }
  }
  SOAD_DUMMY_STATEMENT(InstMapIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
}
#else
{
  instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(InstMapIdx);
  partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);

  /* #20 Check for open sockets on the current instance otherwise. */
  if ( SoAd_Util_LoadU32(SoAd_GetAddrOpenSocksOfInstanceDyn(instanceIdx, partitionIdx), TRUE) > 0u )                   /* SBSW_SOAD_CONST_POINTER_FORWARD */
  {
    retVal = E_NOT_OK;
  }
}
#endif /* SOAD_MULTI_INSTANCE == STD_ON */
  return retVal;
} /* SoAd_State_CheckAllSocketsClosedState() */

/**********************************************************************************************************************
 *  SoAd_State_FinishShutdown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_State_FinishShutdown(void)
{
  /* ----- Local Variables ----------------------------------------------- */
  SoAd_PartitionIdentifiersIterType partitionIdentifierIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset timeout. */
  SoAd_Util_StoreU32(&SoAd_ShutdownFinishedCnt, 0u, TRUE);                                                             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

  /* #20 Set to shutdown state. */
  SoAd_State = SOAD_STATE_SHUTDOWN;

  /* #30 Reset instance specific Init states. */
  for ( partitionIdentifierIter = 0u; 
    partitionIdentifierIter < SoAd_GetSizeOfPartitionIdentifiers();
    partitionIdentifierIter++ )
  {
    SoAd_Util_StoreU8(SoAd_GetAddrInitializedOfPartitionDyn(0u, SoAd_GetPCPartitionConfigIdxOfPartitionIdentifiers(    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD_CSL01_CSL05 */
          partitionIdentifierIter)), SOAD_STATE_PARTITION_UNINIT, TRUE);
  }

  /* #40 Notify user about shutdown. */
  SoAd_State_CallShutdownFinishCallbacks();
} /* SoAd_State_FinishShutdown() */

/**********************************************************************************************************************
 *  SoAd_State_CallShutdownFinishCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_State_CallShutdownFinishCallbacks(void)
{
  /* ----- Local Variables ----------------------------------------------- */
  SoAd_ShutdownFinishedCbkIterType cbkIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all configured callbacks and call them. */
  for ( cbkIter = 0u; cbkIter < SoAd_GetSizeOfShutdownFinishedCbk(); cbkIter++ )
  {
    if ( SoAd_GetShutdownFinishedCbk(cbkIter) != NULL_PTR )
    {
      SoAd_GetShutdownFinishedCbk(cbkIter)();                                                                          /* SBSW_SOAD_FUNCTION_PTR */
    }
  }
} /* SoAd_State_CallShutdownFinishCallbacks() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/* ----- Service functions --------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set module state to uninit. */
  SoAd_State = SOAD_STATE_UNINIT;

  /* #20 Reset configuration pointer. */
  SoAd_ConfigDataPtr = NULL_PTR;
} /* SoAd_InitMemory() */

/**********************************************************************************************************************
 *  SoAd_PreInit()
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
FUNC(void, SOAD_CODE) SoAd_PreInit(
  P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_CONST) SoAdConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameter. */
  if ( SoAdConfigPtr == NULL_PTR )
  {
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    EcuM_BswErrorHook((uint16) SOAD_MODULE_ID, (uint8) ECUM_BSWERROR_NULLPTR);
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
    errorId = SOAD_E_INIT_FAILED;
  }
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
  else if ( SoAdConfigPtr->FinalMagicNumberOfPBConfig != SOAD_FINAL_MAGIC_NUMBER )
  {
    EcuM_BswErrorHook((uint16) SOAD_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
    errorId = SOAD_E_INIT_FAILED;
  }
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    SoAd_PartitionIdentifiersIterType partitionIdentifierIter; /* Not initialized: For-loop counter. */ 

    /* #20 Initialize configuration pointer. */
    SoAd_ConfigDataPtr = SoAdConfigPtr;

    /* #30 Initialize global data. */
    SoAd_Util_StoreU32(&SoAd_ShutdownFinishedCnt, 0u, FALSE);                                                          /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    SoAd_ShutdownRequested = FALSE;

    /* #40 Initialize measurement data (if enabled). */
#if ( SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON )
    SoAd_Anomaly_Init();
#endif /* SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON */

    /* #50 Assert that all partition initialized states are not set. */
    for ( partitionIdentifierIter = 0u; partitionIdentifierIter < SoAd_GetSizeOfPartitionIdentifiers();
      partitionIdentifierIter++ )
    {
      SoAd_Util_StoreU8(SoAd_GetAddrInitializedOfPartitionDyn(0u, SoAd_GetPCPartitionConfigIdxOfPartitionIdentifiers(  /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
        partitionIdentifierIter)), SOAD_STATE_PARTITION_UNINIT, FALSE);
    }

    /* #60 Set preinitialized state. */
    SoAd_State = SOAD_STATE_PREINIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #70 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_PRE_INIT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
} /* SoAd_PreInit() */

/**********************************************************************************************************************
 *  SoAd_Init()
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
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, SOAD_CODE) SoAd_Init(
  P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_CONST) SoAdConfigPtr)
{
  /* ----- Local Variable ----------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check preinitialized state. */
  if ( SoAd_State_GetState() != SOAD_STATE_PREINIT )
  {
    errorId = SOAD_E_NO_PREINIT;
  }
  else
#else
  if ( SoAd_State_GetState() == SOAD_STATE_PREINIT )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    SoAd_PartitionIdentifiersIterType partitionIdentifierIter; /* Not initialized: For-loop counter. */ 

    for ( partitionIdentifierIter = 0u; partitionIdentifierIter < SoAd_GetSizeOfPartitionIdentifiers();
      partitionIdentifierIter++ )
    {
      const SoAd_PartitionConfigIdxType partitionIdx = SoAd_GetPCPartitionConfigIdxOfPartitionIdentifiers(
        partitionIdentifierIter);
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
      const ApplicationType       osApplicationId = (ApplicationType)GetApplicationID();

      if ( SoAd_GetOsApplicationIdOfPartition(0u, partitionIdx) == osApplicationId )
#endif /* SOAD_MULTI_PARTITION == STD_ON */
      {
        /* #20 Initialize instance specific data. */
        SoAd_SizeOfInstanceType instanceIdx; /* Not initialized: For-loop counter. */

        for ( instanceIdx = 0u; instanceIdx < SoAd_GetSizeOfInstanceDyn(partitionIdx); instanceIdx++ )
        {
          SoAd_SetCounterOfInstanceDyn(instanceIdx, 0u, partitionIdx);                                                 /* SBSW_SOAD_CSL01_CSL05 */
          SoAd_BestMatch_Init(instanceIdx, partitionIdx);
        }

        /* #30 Initialize event queues. */
        SoAd_EventQueue_Init(partitionIdx);

        /* #40 Initialize timeout lists. */
        SoAd_TimeoutList_Init(partitionIdx);

        /* #50 Initialize local address structs. */
        SoAd_LocalAddr_Init(partitionIdx);

        /* #60 Initialize socket connection structs. */
        SoAd_SoCon_InitSoCon(partitionIdx);
        SoAd_SoCon_InitSocket(partitionIdx);

        /* #70 Initialize routing group structs. */
        SoAd_RouteGrp_Init(partitionIdx);

        /* #80 Initialize Tx structs. */
        SoAd_Tx_Init(partitionIdx);

        /* #90 Initialize Rx structs. */
        SoAd_Rx_Init(partitionIdx);

        /* #100 Set the initialized state of the current partition. */
        SoAd_Util_StoreU8(SoAd_GetAddrInitializedOfPartitionDyn(0u, partitionIdx), SOAD_STATE_PARTITION_INIT, FALSE);  /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      }
    }
  }
  SOAD_DUMMY_STATEMENT(SoAdConfigPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #110 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_INIT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
} /* SoAd_Init() */

/**********************************************************************************************************************
 *  SoAd_PostInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_PostInit(void)
{
  /* ----- Local Variable ----------------------------------------------- */
  uint8 errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check preinitialized state. */
  if ( SoAd_State_GetState() != SOAD_STATE_PREINIT )
  {
    errorId = SOAD_E_NO_PREINIT;
  }
  else
#else
  if ( SoAd_State_GetState() == SOAD_STATE_PREINIT )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check initialization states of all partitions. */
    SoAd_PartitionIdentifiersIterType partitionIdentifierIter; /* Not initialized: For-loop counter. */

    for ( partitionIdentifierIter = 0u; partitionIdentifierIter < SoAd_GetSizeOfPartitionIdentifiers();
      partitionIdentifierIter++ )
    {
      if ( SoAd_Util_LoadU8(SoAd_GetAddrInitializedOfPartitionDyn(0u,                                                  /* SBSW_SOAD_CONST_POINTER_FORWARD */
        SoAd_GetPCPartitionConfigIdxOfPartitionIdentifiers(partitionIdentifierIter))) == SOAD_STATE_PARTITION_UNINIT )
      {
        errorId = SOAD_E_NOTINIT;
        break;
      }
    }

    /* #30 Set module state to initialized if all partitions are initialized. */
    if ( errorId == SOAD_E_NO_ERROR )
    {
      SoAd_State = SOAD_STATE_INIT;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_POST_INIT, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
} /* SoAd_PostInit() */

/**********************************************************************************************************************
 *  SoAd_Shutdown()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Shutdown(void)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  Std_ReturnType retVal = E_NOT_OK;
#else
  Std_ReturnType retVal; /* Not initialized: Improved readability. */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  uint8          errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  else
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    switch (SoAd_State)
    {
      /* #20 Handle shutdown request in case module is initialized. */
      case SOAD_STATE_INIT:
      {
        /* #200 Set shutdown requested flag to shut down in main function and return that shutdown is in progress. */
        SoAd_ShutdownRequested = TRUE;
        retVal = SOAD_E_INPROGRESS;
        break;
      }
      /* #21 Return corresponding state in case shutdown is in progress. */
      case SOAD_STATE_SHUTDOWN_IN_PROGRESS:
      {
        retVal = SOAD_E_INPROGRESS;
        break;
      }
      /* #22 Return corresponding state in case shutdown is finished. */
      default: /* SOAD_STATE_SHUTDOWN */
      {
        retVal = E_OK;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_SHUTDOWN, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_Shutdown() */

/* ----- Functions ----------------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_State_ProcessStateMachine()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_State_ProcessStateMachine(SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  switch ( SoAd_State )
  {
    /* #10 Process init state in case module is initialized. */
    case SOAD_STATE_INIT:
    {
      SoAd_State_ProcessInitState(InstMapIdx);
      break;
    }
    /* #20 Handle shutdown in case module is in shutdown in progress state. */
    case SOAD_STATE_SHUTDOWN_IN_PROGRESS:
    {
      SoAd_State_ProcessShutdownInProgressState(InstMapIdx);
      break;
    } 
    default: /* SOAD_STATE_UNINIT || SOAD_STATE_PREINIT || SOAD_STATE_SHUTDOWN */
    {
      /* Nothing to do. */
      break;
    }
  }
} /* SoAd_State_ProcessStateMachine() */

/**********************************************************************************************************************
 *  SoAd_State_GetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_StateType, SOAD_CODE) SoAd_State_GetState(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return SoAd_State. */
  return SoAd_State;
} /* SoAd_State_GetState() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_State.c
 *********************************************************************************************************************/
