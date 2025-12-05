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
/*!         \file     ComM.c
 *          \unit     General
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager
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

#define CCL_ASR_COMM_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1_0777 */
/* PRQA S 2991, 2992, 2995, 2996 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA S 3453 EOF */ /* MD_MSR_FctLikeMacro */
/* PRQA S 6050 EOF */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ComM_Private_Cfg.h"
#include "ComM.h"
#include "ComM_Private_BusSM.h"
#include "ComM_Private_Dcm.h"
#include "ComM_Private_EcuMBswM.h"
#include "ComM_Private_Nm.h"
#include "ComM_Channel_StatusAndState.h"
#include "ComM_ErrorReporting.h"
#include "ComM_Initialization.h"
#include "ComM_ModeInhibition.h"
#include "ComM_PNC_General.h"
#include "ComM_User.h"
#include "ComM_Util.h"
#include "ComM_MainFunction.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of ComM.h */
#if (  (COMM_SW_MAJOR_VERSION != (0x17u)) \
    || (COMM_SW_MINOR_VERSION != (0x00u)) \
    || (COMM_SW_PATCH_VERSION != (0x01u)) )
# error "Version numbers of ComM.h are inconsistent!"
#endif

/* Check the version of ComM_BusSM.h */
#if (  (COMM_BUSSM_MAJOR_VERSION != (0x17u)) \
    || (COMM_BUSSM_MINOR_VERSION != (0x00u)) \
    || (COMM_BUSSM_PATCH_VERSION != (0x01u)) )
# error "Version numbers of ComM_BusSM.h are inconsistent!"
#endif

/* Check the version of ComM_Dcm.h */
#if (COMM_DCM_INDICATION == STD_ON)
# if (  (COMM_DCM_MAJOR_VERSION != (0x17u)) \
     || (COMM_DCM_MINOR_VERSION != (0x00u)) \
     || (COMM_DCM_PATCH_VERSION != (0x01u)) )
#  error "Version numbers of ComM_Dcm.h are inconsistent!"
# endif
#endif

/* Check the version of ComM_EcuMBswM.h */
#if (  (COMM_ECUMBSWM_MAJOR_VERSION != (0x17u)) \
    || (COMM_ECUMBSWM_MINOR_VERSION != (0x00u)) \
    || (COMM_ECUMBSWM_PATCH_VERSION != (0x01u)) )
# error "Version numbers of ComM_EcuMBswM.h are inconsistent!"
#endif

/* Check the version of ComM_Nm.h */
#if (  (COMM_NM_MAJOR_VERSION != (0x17u)) \
    || (COMM_NM_MINOR_VERSION != (0x00u)) \
    || (COMM_NM_PATCH_VERSION != (0x01u)) )
# error "Version numbers of ComM_Nm.h are inconsistent!"
#endif

/* Check the version of ComM_Types.h */
#if (  (COMM_TYPES_MAJOR_VERSION != (0x17u)) \
    || (COMM_TYPES_MINOR_VERSION != (0x00u)) \
    || (COMM_TYPES_PATCH_VERSION != (0x01u)) )
# error "Version numbers of ComM_Types.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  COMPATIBILITY / DEFAULTS
 *********************************************************************************************************************/

#if (  (COMM_CONFIGURATION_VARIANT != COMM_CONFIGURATION_VARIANT_PRECOMPILE) \
    && (COMM_CONFIGURATION_VARIANT != COMM_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) )
# error "ComM currently supports configuration variants PreCompile and PostBuild Loadable only"
#endif

/**********************************************************************************************************************
 * GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES OF STATE TRANSITION FUNCTIONS (TF)
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
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ComM_PreInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_PreInit( P2CONST( ComM_ConfigType, AUTOMATIC, COMM_INIT_DATA ) ConfigPtr )
{
  /* ----- Local Variable ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

#if (COMM_USE_INIT_POINTER == STD_ON)
  /* #10 In the post-build variant: Save the pointer to configuration. */
  ComM_ConfigPtr = ConfigPtr;

  /* ----- Development Error Checks ------------------------------------- */
  if ( ConfigPtr == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set the InitStatus and the PostInitialized status of ComM.*/
    ComM_Initialization_PreInit();
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_PREINIT, errorId );

  COMM_DUMMY_STATEMENT_CONST( ConfigPtr ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_PostInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_PostInit( void )
{
  /* ----- Local Variable ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process the PostInitialized status of ComM.*/
  errorId = ComM_Initialization_PostInit();

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_POSTINIT, errorId );
}

/***********************************************************************************************************************
 *  ComM_Init()
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
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, COMM_CODE) ComM_Init(P2CONST(ComM_ConfigType, AUTOMATIC, COMM_INIT_DATA) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_USE_INIT_POINTER == STD_ON)
  /* #10 Check that the configuration pointer is valid. */
  if ( ConfigPtr == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
    ComM_ErrorReporting_ProcessBswHookError( ECUM_BSWERROR_NULLPTR );
  }
  else
#endif
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the received partition index is valid. */
  if ( partitionIdx >= ComM_GetSizeOfPartitionIdentifiers() )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
  else
# endif
  /* #30 Check that the module is pre-initialized. */
  if ( ComM_Initialization_IsPreInitialized() == FALSE )
  {
    errorId = COMM_E_NO_PREINIT;
  }
  else
  /* #40 Check that the module is not already initialized. */
  if ( ComM_GetInitStatus( partitionIdx ) == COMM_INIT )
  {
    errorId = COMM_E_ALREADY_INITIALIZED;
  }
  else
#endif
#if ( COMM_USE_INIT_POINTER == STD_ON )
  /* #50 Check the magic number. */  /* SPEC-35483 */
  if ( ComM_HasFinalMagicNumber() && (ComM_GetFinalMagicNumber() != COMM_FINAL_MAGIC_NUMBER) )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
    ComM_ErrorReporting_ProcessBswHookError(ECUM_BSWERROR_MAGICNUMBER);
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #60 Process the initialization status. */
    errorId = ComM_Initialization_Init(partitionIdx);
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport(COMM_SID_INIT, errorId);

  COMM_DUMMY_STATEMENT_CONST( ConfigPtr ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/*********************************************************************************************************************
 *  ComM_DeInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_DeInit( void )
{
  /* ----- Local Variable ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  /* #10 If Multi-Partition is supported, check if the service is called from the correct partition context. */
  if ( ComM_GetInternalPartitionIdx( GetCurrentApplicationID()) != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
  else
# endif
# if (COMM_USE_INIT_POINTER == STD_ON)
  /* #20 Check that the configuration pointer is valid. */
  if ( ComM_ConfigPtr == NULL_PTR )
  {
    errorId = COMM_E_NOT_INITED;
  }
  else
# endif
  /* #30 Check that the module is post-initialized. */
  if (ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Process the De-Initialization request. */
    errorId = ComM_Initialization_DeInit();
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport(COMM_SID_DEINIT, errorId);
}

/*********************************************************************************************************************
 *  ComM_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, COMM_CODE) ComM_InitMemory(void)
{
#if (COMM_USE_INIT_POINTER == STD_ON)
  /* #10 In the post-build variant: set the config pointer to NULL. */
  ComM_ConfigPtr = NULL_PTR;
#endif

  /* #20 Set the pre-initialization status. */
  ComM_Initialization_InitMemory();
}

/*********************************************************************************************************************
 *  ComM_GetStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetStatus( P2VAR( ComM_InitStatusType, AUTOMATIC, COMM_APPL_VAR ) Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check the parameter 'Status': it must be a valid pointer. */
  if ( Status == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If parameter checks are successful, store the initialization state of ComM. */
    retVal = ComM_Initialization_GetStatus( Status );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETSTATUS, errorId );

  return retVal;
}

#if (COMM_VERSION_INFO_API == STD_ON)
/*********************************************************************************************************************
 *  ComM_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_GetVersionInfo( P2VAR( Std_VersionInfoType, AUTOMATIC, COMM_APPL_VAR ) Versioninfo )
{
  /* ----- Local Variable ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check Versioninfo for NULL pointer. Note: no initialization check is needed. */
  if ( Versioninfo == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set Versioninfo with corresponding macros from component header. */
      Versioninfo->vendorID = COMM_VENDOR_ID;
      Versioninfo->moduleID = COMM_MODULE_ID;
      Versioninfo->sw_major_version = (uint8)COMM_SW_MAJOR_VERSION;
      Versioninfo->sw_minor_version = COMM_SW_MINOR_VERSION;
      Versioninfo->sw_patch_version = COMM_SW_PATCH_VERSION;
  }
  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETVERSIONINFO, errorId );
}
#endif

/**********************************************************************************************************************
 *  ComM_CommunicationAllowed()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_CommunicationAllowed( NetworkHandleType Channel, boolean Allowed )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
# endif
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  /* #30 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, store the Communication Allowed status of the Channel. */
    ComM_SetComAllowed( Channel, Allowed, COMM_MASTER_PARTITION_IDX );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_COMMUNICATIONALLOWED, errorId );
}

/**********************************************************************************************************************
 *  ComM_GetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetState( NetworkHandleType Channel, P2VAR( ComM_StateType, AUTOMATIC, COMM_APPL_VAR ) State )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'State': it must be a valid pointer. */
  else if ( State == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, store the current active state of the channel by default. */
    ComM_Channel_GetState( Channel, State );
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETSTATE, errorId );

  return retVal;
}

/**********************************************************************************************************************
 *  ComM_GetMinFullComModeTimerStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetMinFullComModeTimerStatus( NetworkHandleType Channel, P2VAR( boolean, AUTOMATIC, COMM_APPL_VAR ) Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'Status': it must be a valid pointer. */
  else if ( Status == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /*-#40 If parameter checks are successful, process the MinFullComModeTimer status request.*/
    ComM_Channel_GetMinFullComModeTimerStatus( Channel, Status );
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETMINFULLCOMMODETIMERSTATUS, errorId );

  return retVal;
}

/**********************************************************************************************************************
 *  ComM_GetInhibitionStatus()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetInhibitionStatus( NetworkHandleType Channel, P2VAR( ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_VAR ) Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'Status': it must be valid pointer. */
  else if ( Status == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, store the inhibition status of the channel. */
    ComM_ModeInhibition_GetInhibitionStatus( Channel, Status );
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETINHIBITIONSTATUS, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_PreventWakeUp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_PreventWakeUp( NetworkHandleType Channel, boolean Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
# endif
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  /* #30 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the request. */
    retVal = ComM_ModeInhibition_PreventWakeUp( Channel, Status );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_PREVENTWAKEUP, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_LimitChannelToNoComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_LimitChannelToNoComMode( NetworkHandleType Channel, boolean Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;

#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
# endif
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  /* #30 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the request. */
    retVal = ComM_ModeInhibition_LimitChannelToNoComMode( Channel, Status );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_LIMITCHANNELTONOCOMMODE, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_LimitECUToNoComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_LimitECUToNoComMode( boolean Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
# endif
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the request. */
    retVal = ComM_ModeInhibition_LimitECUToNoComMode( Status );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_LIMITECUTONOCOMMODE, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_ReadInhibitCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_ReadInhibitCounter( P2VAR( uint16, AUTOMATIC, COMM_APPL_VAR ) CounterValue )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'CounterValue': it must be a valid pointer. */
  else if ( CounterValue == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the request. */
    ComM_ModeInhibition_ReadInhibitCounter( CounterValue );
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_READINHIBITCOUNTER, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_ResetInhibitCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_ResetInhibitCounter( void )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
# endif
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the request. */
    ComM_ModeInhibition_ResetInhibitCounter();
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_RESETINHIBITCOUNTER, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_SetECUGroupClassification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_SetECUGroupClassification( ComM_InhibitionStatusType Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
#if (COMM_DEV_ERROR_DETECT == STD_ON)
# if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
# endif
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
# if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
# endif
  /* #30 Check parameter 'Status': it must have a value between 0 and 3. */
  else if ( Status > 0x03u )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the request. */
    ComM_ModeInhibition_SetECUGroupClassification( Status );
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_SETECUGROUPCLASSIFICATION, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_RequestComMode()
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
FUNC( Std_ReturnType, COMM_CODE ) ComM_RequestComMode( ComM_UserHandleType User, ComM_ModeType ComMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserIterType userIdx = (ComM_UserIterType)User;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'User': it must be a valid user identifier. */
  else if ( userIdx >= ComM_GetSizeOfUser() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check that the current user partitionIdx is valid. */
  else if ( ComM_GetPartitionConfigIdxOfUser( userIdx ) >= ComM_GetSizeOfPartitionIdentifiers() )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
  /* #40 Check parameter 'ComMode': it must be either equal to COMM_NO_COMMUNICATION or COMM_FULL_COMMUNICATION. */
  /* If called with ComMode != COMM_NO_COMMUNICATION && ComMode != COMM_FULL_COMMUNICATION (ESCAN00068871) */
  else if ( (ComMode != COMM_NO_COMMUNICATION) && (ComMode != COMM_FULL_COMMUNICATION) )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 If parameter checks are successful, process the request. */
    retVal = ComM_User_RequestComMode( userIdx, ComMode, (ComM_PCPartitionConfigIdxOfPartitionIdentifiersType)ComM_GetPartitionConfigIdxOfUser( userIdx ) );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_REQUESTCOMMODE, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_GetMaxComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetMaxComMode( ComM_UserHandleType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserIterType userIdx = (ComM_UserIterType)User;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'User': it must be a valid user identifier. */
  else if ( User >= ComM_GetSizeOfUser() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'ComMode': it must be a valid pointer. */
  else if ( ComMode == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the request. */
    ComM_User_GetMaxComMode( userIdx, ComMode );
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETMAXCOMMODE, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_GetRequestedComMode()
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
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetRequestedComMode( ComM_UserHandleType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserIterType userIdx = (ComM_UserIterType)User;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'User': it must be a valid user identifier. */
  else if ( userIdx >= ComM_GetSizeOfUser() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check that the current user partitionIdx is valid. */
  else if ( ComM_GetPartitionConfigIdxOfUser( userIdx ) >= ComM_GetSizeOfPartitionIdentifiers() )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
  /* #40 Check parameter 'ComMode': it must be a valid pointer. */
  else if ( ComMode == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #50 If parameter checks are successful, process the request. */
    ComM_User_GetRequestedComMode( userIdx, ComMode, (ComM_PCPartitionConfigIdxOfPartitionIdentifiersType)ComM_GetPartitionConfigIdxOfUser( userIdx ) );
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETREQUESTEDCOMMODE, errorId );

  return retVal;
}

/*********************************************************************************************************************
 *  ComM_GetCurrentComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetCurrentComMode( ComM_UserHandleType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserIterType userIdx = (ComM_UserIterType)User;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'User': it must be a valid user identifier. */
  else if ( userIdx >= ComM_GetSizeOfUser() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'ComMode': it must be a valid pointer. */
  else if ( ComMode == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the request. */
    retVal = ComM_User_GetCurrentComMode( userIdx, ComMode );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETCURRENTCOMMODE, errorId );

  return retVal;
}

#if (COMM_PNC_SUPPORT == STD_ON)
/*********************************************************************************************************************
 *   ComM_GetCurrentPNCComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetCurrentPNCComMode( ComM_UserHandleType User, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----------------------- Local Variables ----------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
  ComM_UserIterType userIdx = (ComM_UserIterType)User;

  /* ------------------- Development Error Checks ------------------------ */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'User': it must be a valid user identifier. */
  else if ( userIdx >= ComM_GetSizeOfUser() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'ComMode': it must be a valid pointer. */
  else if ( ComMode == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the request. */
    retVal = ComM_User_GetCurrentPNCComMode( userIdx, ComMode );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETCURRENTPNCCOMMODE, errorId );

  return retVal;
}


/*********************************************************************************************************************
 *  ComM_LimitPncToChannelRouting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_LimitPncToChannelRouting( PNCHandleType Pnc, NetworkHandleType Channel, boolean Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;
# if (COMM_DEV_ERROR_DETECT == STD_ON)
#  if (COMM_MULTIPARTITION == STD_ON)
  ComM_PartitionConfigIdxOfChannelType partitionIdx = ComM_GetInternalPartitionIdx( GetCurrentApplicationID() );
#  endif
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
#  if (COMM_MULTIPARTITION == STD_ON)
  /* #20 Check that the current partitionIdx is the ComM Master Partition. */
  else if ( partitionIdx != COMM_MASTER_PARTITION_IDX )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
#  endif
  /* #30 Check parameter 'Channel': it must be valid and PNC routing limitation must be supported on the channel. */
  /* Note: it is ensured by the code generator that only active channels having GW type != NONE support routing limitation. */
  else if ( (Channel >= ComM_GetSizeOfChannelPb()) || (!ComM_IsPncLimitationUsedOfChannelPb( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the channel routing limitation. */
    retVal = ComM_Pnc_LimitPncToChannelRouting( Pnc, Channel, Status );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_LIMITPNCTOCHANNELROUTING, errorId );

  return retVal;
}

#endif

/**********************************************************************************************************************
 *  ComM_MainFunction()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_MainFunction( NetworkHandleType Channel )
{
  /* ----- Local Variable ---------------------------------------------- */
  uint8  errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check that the module is initialized. If not, the function returns immediately without performing any functionality and without raising any errors. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    /* no error reporting */
  }
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( Channel >= ComM_GetSizeOfChannel() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Analyse the channel's status and state and process the state transition accordingly. */
    ComM_MainFunction_ProcessCycle( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_MAINFUNCTION, errorId );
}

#if ( COMM_MULTIPARTITION == STD_ON )
/**********************************************************************************************************************
 *  ComM_MainFunction_Satellite()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_MainFunction_Satellite( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check parameter 'Channel': it must be valid and active in the current variant. */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  if ( Channel >= ComM_GetSizeOfChannel() )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #20 Check if ComM is initialized. */
  else if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #30 Check the validity of the received partition index. */
  else if ( ComM_GetPartitionConfigIdxOfChannel( Channel ) >= ComM_GetSizeOfPartitionIdentifiers() )
  {
    errorId = COMM_E_INVALID_PARTITION;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Process the synchronization for master and satellite partitions. */
    ComM_MainFunction_ProcessSatelliteCycle( Channel, ComM_GetPartitionConfigIdxOfChannel( Channel ) );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_MAINFUNCTION_SATELLITE, errorId );
}
#endif


/**********************************************************************************************************************
  CALLBACK FUNCTIONS (AUTOSAR BusSM)
 *********************************************************************************************************************/

/*********************************************************************************************************************
 *  ComM_BusSM_ModeIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_BusSM_ModeIndication( NetworkHandleType Channel, P2VAR( ComM_ModeType, AUTOMATIC, COMM_APPL_VAR ) ComMode )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'ComMode': it must be a valid pointer and its value must be a valid communication mode. */
  else if ( ComMode == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else if ( *ComMode > COMM_FULL_COMMUNICATION )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the BusSM mode indication.*/
    ComM_BusSM_ProcessModeIndication( Channel, ComMode );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_BUSSMMODEINDICATION, errorId );
}

/*********************************************************************************************************************
 *  ComM_BusSM_BusSleepMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_BusSM_BusSleepMode( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the BusSM bus sleep mode indication.*/
    ComM_BusSM_ProcessBusSleepMode( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_BUSSMBUSSLEEPMODE, errorId );
}

/**********************************************************************************************************************
  CALLBACK FUNCTIONS (AUTOSAR DCM)
 *********************************************************************************************************************/

#if (COMM_DCM_INDICATION == STD_ON)
/*********************************************************************************************************************
 *  ComM_DCM_ActiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_DCM_ActiveDiagnostic( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the active diagnostic indication. */
    errorId = ComM_DCM_ProcessActiveDiagnostic( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_DCM_ACTIVEDIAGNOSTIC, errorId );
}

/*********************************************************************************************************************
 *  ComM_DCM_InactiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_DCM_InactiveDiagnostic( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the inactive diagnostic indication. */
    errorId = ComM_DCM_ProcessInactiveDiagnostic( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_DCM_INACTIVEDIAGNOSTIC, errorId );
}

/**********************************************************************************************************************
 *  ComM_GetDcmRequestStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( Std_ReturnType, COMM_CODE ) ComM_GetDcmRequestStatus( NetworkHandleType Channel, P2VAR( boolean, AUTOMATIC, COMM_APPL_VAR ) Status )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
    retVal = COMM_E_UNINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'Status': it must be a valid pointer. */
  else if ( Status == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, query the request status of Dcm. */
    ComM_DCM_GetActiveRequestStatus( Channel, Status );
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_GETDCMREQUESTSTATUS, errorId );

  return retVal;
}
#endif

/**********************************************************************************************************************
  CALLBACK FUNCTIONS (AUTOSAR EcuM)
 *********************************************************************************************************************/

/*********************************************************************************************************************
  *  ComM_EcuM_WakeUpIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_EcuM_WakeUpIndication( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the EcuM wakeUp indication. */
    ComM_EcuM_ProcessWakeUpIndication( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_ECUM_WAKEUPINDICATION, errorId );
}

#if (COMM_PNC_SUPPORT == STD_ON)
/*********************************************************************************************************************
 *  ComM_EcuM_PNCWakeUpIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_EcuM_PNCWakeUpIndication( PNCHandleType PNCid )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_WRONG_PARAMETERS;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 If parameter checks are successful, process the EcuM Pnc wakeUp indication. */
    if ( ComM_EcuM_ProcessPNCWakeUpIndication( PNCid ) == E_OK )
    {
      errorId = COMM_E_NO_ERROR;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_ECUM_PNCWAKEUPINDICATION, errorId );
}
#endif

/**********************************************************************************************************************
 CALLBACK FUNCTIONS (AUTOSAR NM)
 *********************************************************************************************************************/

#if (COMM_NMSUPPORTOFCHANNEL == STD_ON)
/*********************************************************************************************************************
 *  ComM_Nm_NetworkStartIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_NetworkStartIndication( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the Network Start Indication. */
    ComM_Nm_ProcessNetworkStartIndication( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_NM_NETWORKSTARTINDICATION, errorId );
}

/*********************************************************************************************************************
 *  ComM_Nm_NetworkMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_NetworkMode( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the Network Mode notification. */
    ComM_Nm_ProcessNetworkMode( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_NM_NETWORKMODE, errorId );
}

/*********************************************************************************************************************
 *  ComM_Nm_PrepareBusSleepMode()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_PrepareBusSleepMode( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the Prepare Bus Sleep Mode indication. */
    ComM_Nm_ProcessPrepareBusSleepMode( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_NM_PREPAREBUSSLEEPMODE, errorId );
}

/*********************************************************************************************************************
 *  ComM_Nm_BusSleepMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_BusSleepMode( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the Bus Sleep Mode indication. */
    ComM_Nm_ProcessBusSleepMode( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_NM_BUSSLEEPMODE, errorId );
}

/*********************************************************************************************************************
 *  ComM_Nm_RestartIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_RestartIndication( NetworkHandleType Channel )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the Network Restart Indication. */
    ComM_Nm_ProcessRestartIndication( Channel );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_NM_RESTARTINDICATION, errorId );
}
#endif

#if ( COMM_PNC_SUPPORT == STD_ON )
/*********************************************************************************************************************
 *  ComM_Nm_StateChangeNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_StateChangeNotification( CONST( NetworkHandleType, AUTOMATIC ) Channel, CONST( Nm_StateType, AUTOMATIC ) NmPreviousState, CONST( Nm_StateType, AUTOMATIC ) NmCurrentState )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If parameter checks are successful, process the state change notification from Nm. */
    ComM_Nm_ProcessStateChangeNotification( Channel, NmPreviousState, NmCurrentState );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_NM_STATECHANGENOTIFICATION, errorId );
}

# if ( COMM_SYNCHRONIZED_PNC_SHUTDOWN_ENABLED == STD_ON )
/*********************************************************************************************************************
 *  ComM_Nm_ForwardSynchronizedPncShutdown()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_Nm_ForwardSynchronizedPncShutdown( CONST( NetworkHandleType, AUTOMATIC ) Channel, CONSTP2CONST( uint8, AUTOMATIC, COMM_APPL_VAR ) PncBitVectorPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COMM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (COMM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check that the module is initialized. */
  if ( ComM_Initialization_IsPostInitialized() == FALSE )
  {
    errorId = COMM_E_NO_POSTINIT;
  }
  /* #20 Check parameter 'Channel': it must be valid and active in the current variant. */
  else if ( (Channel >= ComM_GetSizeOfChannel()) || (ComM_IsInvalidHndOfChannel( Channel )) ) /* PRQA S 3415 */ /* MD_ComM_3415 */
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  /* #30 Check parameter 'PncBitVectorPtr': it must be a valid pointer. */
  else if ( PncBitVectorPtr == NULL_PTR )
  {
    errorId = COMM_E_WRONG_PARAMETERS;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 If parameter checks are successful, process the Synchronized Pnc Shutdown indication from Nm. */
    ComM_Nm_ProcessForwardSynchronizedPncShutdown( Channel, PncBitVectorPtr );
  }

  /* ----- Development Error Report --------------------------------------- */
  ComM_ErrorReporting_ProcessDetErrorReport( COMM_SID_CBK_NM_FORWARDSYNCHRONIZEDPNCSHUTDOWN, errorId );
}
# endif
#endif

/* module specific MISRA deviations:
MD_ComM_0771:
      Reason:       Multiple break statements used to exit from for-loop immediately in case an error was detected due to wrong generated data.
      Risk:         There is no risk.
      Prevention:   Covered by code review.

MD_ComM_3218:
      Reason:       Some compilers are not able to compile code with static declarations inside functions.
      Risk:         There is no risk.
      Prevention:   N/A

MD_ComM_3415:
      Reason:       The function does not have any side-effects (it does not change the module' state').
      Risk:         There is no risk.
      Prevention:   Covered by code review.

MD_ComM_3451_3449:
      Reason:       It is ensured by code generator that the declarations remain compatible.
                    1. The first declaration is in ComM_Private_Cfg.h, which is not included via ComM.h.The second declaration is in ComM_Lcfg.h, which is included via ComM.h and therefore visible to other modules.
                    2. The first declaration is in ComM.h. The second declaration is in Rte_ComM.h. Multiple external declarations of the functions exist so that other components can use that function as well.
      Risk:         There is no risk.
      Prevention:   Covered by code review.

MD_ComM_3673:
      Reason:       The function parameter list is required by the AUTOSAR specification.
      Risk:         There is no risk.
      Prevention:   N/A.

MD_ComM_2985:
      Reason:       The operation is needed if the right hand operand is non zero.
      Risk:         No Risk.
      Prevention:   Covered by code review.

MD_ComM_2982:
      Reason:       The assignment is needed to set the default value.
      Risk:         No Risk.
      Prevention:   Covered by code review.

MD_ComM_2994:
     Reason:     The result of the expression depends on the loaded configuration.
     Risk:       There is no risk.
     Prevention: No prevention necessary.

MD_ComM_1039:
      Reason:       The array can have the length one depending on configuration. However, the array is never used as a flexible array member.
                    It is always accessed with the index zero in this case.
      Risk:         No Risk.
      Prevention:   Covered by code review.

MD_ComM_0315:
      Reason:       Com_SendSignal and Com_RecieveSignal have a void pointer as a function parameter. This is defined by AUTOSAR.
      Risk:         No risk, because the underlying uint8 pointer type is known.
      Prevention:   No prevention necessary.

MD_ComM_1533:
      Reason:       The value of the object is generated, therefore cannot be moved to the static file ComM.c.
      Risk:         No Risk.
      Prevention:   Covered by code review.

MD_ComM_1339:
      Reason:       The value of the object is never modified
      Risk:         No Risk.
      Prevention:   Covered by code review.

MD_ComM_2823:
      Reason:       It is ensured by the generator that the value of the pointer is never NULL.
      Risk:         No Risk.
      Prevention:   Covered by code review.

MD_ComM_2983:
      Reason:       There might be configurations, in which this assignment is redundant and the value is never subsequently used.
                    Nevertheless, for improved readability this assignment is always performed.
      Risk:         No risk.
      Prevention:   None.

MD_ComM_0404:
      Reason:       The sync counter on the master partition is only written from the master partition (task context), i.e. changing this value via
                    interrupt is excluded. The sync counter on the satellite partition can only be changed to (master counter + 1) and only from
                    the satellite partition. Therefore, the order of the read accesses to these two volatile objects is not significant.
      Risk:         No Risk.
      Prevention:   Covered by code review.
*/

/* FETA_JUSTIFICATION_BEGIN
\ID                FETA_COMM_CSL_USE_CASE
\DESCRIPTION       Iteration from a defined lower limit to a defined upper limit. The upper limit is provided as a constant by the CSL.
                   The qualified CSL APIs return the positive constant values from CSL tables or the size of the CSL tables itself. The size of the table and the
                   positive values in the table depend on the configuration. The stop index is a defined upper border of the loop. The CSL guarantees that the type
                   of the loop variable is equal to the type of the upper limit.
                   Besides the stepwise loop increments, the loop variable is not modified elsewhere.
\COUNTERMEASURE \N Qualified use-cases of ComStackLib

\ID                FETA_COMM_CHANNEL_STATE_TRANSITION
\DESCRIPTION       Repetition of the channel state machine until either the target state is reached or an exit criteria is met (e.g. target state cannot be reached).
                   The iteration is performed in a do-while-loop, since there are transitions where multiple state transitions need to be executed until the target state is reached.
                   The exit criteria of this iteration is defined by the actual state and the target state. Both, the actual and the target state, cannot be changed by another task
                   while the channel state machine is executed. Thus it is ensured that the state of these variables is consistent throught the execution of the channel state machine.
\COUNTERMEASURE \T All possible combinations of actual state and target state have been fully tested.

\ID                FETA_COMM_PNC_STATE_TRANSITION
\DESCRIPTION       Repetition of the PNC state machine until either the target state is reached or an exit criteria is met (e.g. target state cannot be reached).
                   The iteration is performed in a do-while-loop, since there are transitions where multiple state transitions need to be executed until the target state is reached.
                   The exit criteria of this iteration is defined by the actual state and the target state. Both, the actual and the target state, cannot be changed by another task
                   while the PNC state machine is executed. Thus it is ensured that the state of these variables is consistent throught the execution of the PNC state machine.
\COUNTERMEASURE \T All possible combinations of actual state and target state have been fully tested.

 FETA_JUSTIFICATION_END */

/* START_COVERAGE_JUSTIFICATION

\ID COV_COMM_MISRA
  \ACCEPT X
  \ACCEPT TX
  \REASON [COV_MSR_MISRA]

END_COVERAGE_JUSTIFICATION */

/* VCA_JUSTIFICATION_BEGIN

\ID                VCA_COMM_CALL_EXTERNAL_FUNCTION_POINTER_ARGUMENT
\DESCRIPTION       A function with pointer parameters is directly called, but the function is not
                   defined within the analyzed sources.VCA is unable to determine the
                   behavior of the function.
\COUNTERMEASURE \N The validity of the pointer is verified by the caller.

\ID                VCA_COMM_CALL_UNDEFINED_EXTERNAL_FUNCTION
\DESCRIPTION       A function from a different module is called, but the function is not
                   defined within the analyzed sources. VCA is unable to determine the
                   behavior of the function.
\COUNTERMEASURE \N The validity of the pointer is verified by the caller.

\ID                VCA_COMM_PNC_IDX
\DESCRIPTION       Access to an array that has a size relation to ComM_Pnc.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

\ID                VCA_COMM_CHANNEL_CSL03
\DESCRIPTION       Access to an array with a 'ChannelIndex' via indirection over PncSignal or PncChannelMapping.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

\ID                VCA_COMM_PARTITION_IDX
\DESCRIPTION       Access to an array with a 'partitionIdx'
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

\ID                VCA_COMM_UserModeNotiFunc_PointerCall
\DESCRIPTION       Calling a function pointer defined in ComM_UserModeNotiFunc[].
\COUNTERMEASURE \S The user of MICROSAR Safe shall verify that each element of ComM_UserModeNotiFunc[] is either a valid function pointer or NULL_PTR.
                   This measure is only needed if at least one ComM user has enabled the parameter 'User Mode Notification'. [SMI-1046]
                \R A runtime check prevents dereferencing a NULL pointer.
                \R Qualified use-case CSL01 of ComStackLib because ComM_GetSizeOfUserModeNotiFunc() is used to check the array index.

\ID                VCA_COMM_USERREQFULLCOM_IDX
\DESCRIPTION       Access to ComM_UserReqFullCom via indirection over ComM_UserByteMask. Currently VCA is unable to determine the indirection
                   specifically the start and end access of UserByteMask.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

\ID                VCA_COMM_USERREQPNCFULLCOM_IDX
\DESCRIPTION       Access to ComM_UserReqPncFullCom via indirection over ComM_UserPncByteMask. Currently VCA is unable to determine the indirection
                   specifically the start and end access of UserByteMask.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

VCA_JUSTIFICATION_END */

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
