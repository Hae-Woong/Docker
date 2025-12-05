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
/*!       \file  Can_30_Core_Initialization.c
 *        \brief  Can_30_Core_Initialization source file
 *        \details  Contains the implementation of the Initialization unit.
 *        \unit Can_30_Core_Initialization
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
#define CAN_30_CORE_INITIALIZATION_SOURCE
#define CAN_30_CORE_INTERNAL

/**********************************************************************************************************************
 * LOCAL MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2880, 2991, 2992, 2994, 2995, 2996 EOF */ /* MD_Can_30_Core_ConstValue */
/* PRQA  S 2741, 2742 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core.h"
#include "Can_30_Core_vCan.h"
#include "Can_30_Core_Initialization.h"
#include "Can_30_Core_RxQueueHandler.h"
#include "Can_30_Core_InterruptService.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ModeHandler.h"
#include "Can_30_Core_ModeRequest.h"
#include "Can_30_Core_ErrorHandler.h"
#include "Can_30_Core_TxBufferHandler.h"

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_VAR_INIT_UNSPECIFIED
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization state of the module */
VAR(Can_30_Core_InitState, CAN_30_CORE_VAR_INIT) Can_30_Core_GlobalInitState = CAN_30_CORE_UNINITIALIZED;

#define CAN_30_CORE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_VAR_INIT_UNSPECIFIED
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the configuration */
P2CONST(Can_30_Core_ConfigType, CAN_30_CORE_VAR_CLEARED, CAN_30_CORE_PBCFG) Can_30_Core_ConfigDataPtr = NULL_PTR;

#define CAN_30_CORE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_InitControllerMailboxes()
 *********************************************************************************************************************/
/*! \brief       Mailbox initialization
 *  \details     Initializes all the mailboxes for the requested controller
 *  \param[in]   Controller       The requested controller
 *  \param[in]   doRamCheck       Decides if the RAM check should be performed
 *  \param[in]   ramCheckIssue    Pointer to RAM check status
 *  \return      CAN_NOT_OK       Initialization failed
 *  \return      CAN_OK           Initialization is ok
 *  \pre         Controller is in init mode
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_InitControllerMailboxes(
  uint8 Controller,
  boolean doRamCheck,
  P2VAR(boolean, AUTOMATIC, CAN_30_CORE_VAR_NO_INIT) ramCheckIssue);

/**********************************************************************************************************************
 *  Can_30_Core_InitControllerMailbox()
 *********************************************************************************************************************/
/*! \brief       Mailbox initialization
 *  \details     Initializes the controller mailbox
 *  \param[in]   Controller       The requested controller
 *  \param[in]   doRamCheck       Decides if the RAM check should be performed
 *  \param[in]   mailboxHandle    Handle to the mailbox that should be initialized
 *  \param[in]   ramCheckIssue    Pointer to RAM check status
 *  \return      CAN_NOT_OK       Initialization failed
 *  \return      CAN_OK           Initialization is ok
 *  \pre         Controller is in init mode
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_InitControllerMailbox(
  uint8 Controller,
  boolean doRamCheck,
  Can_HwHandleType mailboxHandle,
  P2VAR(boolean, AUTOMATIC, CAN_30_CORE_VAR_NO_INIT) ramCheckIssue);

/**********************************************************************************************************************
 *  Can_30_Core_RamCheckMailboxNotification()
 *********************************************************************************************************************/
/*! \brief       RAM check notification
 *  \details     Notifies the upper layer in case an issue is detected in a mailbox during the RAM check
 *  \param[in]   Controller       The controller in which the issue was detected
 *  \param[in]   mailboxHandle    The mailboxhandle of the corrupted mailbox
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_RamCheckMailboxNotification(uint8 Controller, Can_HwHandleType mailboxHandle);

/**********************************************************************************************************************
 *  Can_30_Core_SetGlobalInitState()
 *********************************************************************************************************************/
/*! \brief       Sets the global init state
 *  \details     Sets the state of the driver initialization
 *  \param[in]   State    The intialization state
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetGlobalInitState(Can_30_Core_InitState State);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Can_30_Core_SetGlobalInitState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetGlobalInitState(Can_30_Core_InitState State)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the global init state to the requested value */
  Can_30_Core_GlobalInitState = State;
}

/**********************************************************************************************************************
 *  Can_30_Core_InitControllerMailboxes()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_InitControllerMailboxes
  Relation_Context:
Variant, VariantSel, DevErrorDetect
  Relation:
TxFullCan, ChannelAmount, RamCheck
RxFullCan, ChannelAmount, RamCheck
TxBasicCan, ChannelAmount, RamCheck
RxBasicCan, ChannelAmount, RamCheck
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010 1 */ /* MD_MSR_STPTH */
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_InitControllerMailboxes(
  uint8 Controller,
  boolean doRamCheck,
  P2VAR(boolean, AUTOMATIC, CAN_30_CORE_VAR_NO_INIT) ramCheckIssue)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_OK; /* Assume that the mailbox initialization is ok */
  Can_HwHandleType mailboxHandle;
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Perform Tx BasicCan Mailbox Initialization */
  for (mailboxHandle = (Can_HwHandleType)Can_30_Core_GetMailboxTxBasicStartIdxOfControllerConfig(Controller); mailboxHandle < (Can_HwHandleType)Can_30_Core_GetMailboxTxBasicEndIdxOfControllerConfig(Controller); mailboxHandle++) /* FETA_CAN_30_CORE_1 */
  {
    if(Can_30_Core_InitControllerMailbox(Controller, doRamCheck, mailboxHandle, ramCheckIssue) == CAN_NOT_OK)
    {
      retval = CAN_NOT_OK;
    } 
  }
  /* #110 Perform Tx FullCan Mailbox Initialization */
  for (mailboxHandle = (Can_HwHandleType)Can_30_Core_GetMailboxTxFullStartIdxOfControllerConfig(Controller); mailboxHandle < (Can_HwHandleType)Can_30_Core_GetMailboxTxFullEndIdxOfControllerConfig(Controller); mailboxHandle++) /* PRQA S 2996 */ /* MD_Can_30_Core_ConstValue */ /* FETA_CAN_30_CORE_1 */
  {
    if(Can_30_Core_InitControllerMailbox(Controller, doRamCheck, mailboxHandle, ramCheckIssue) == CAN_NOT_OK) /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    {
      retval = CAN_NOT_OK; 
    }
  }  
  /* #120 Perform Rx BasicCan Mailbox Initialization */
  for (mailboxHandle = (Can_HwHandleType)Can_30_Core_GetMailboxRxBasicStartIdxOfControllerConfig(Controller); mailboxHandle < (Can_HwHandleType)Can_30_Core_GetMailboxRxBasicEndIdxOfControllerConfig(Controller); mailboxHandle++) /* FETA_CAN_30_CORE_1 */
  {
    if(Can_30_Core_InitControllerMailbox(Controller, doRamCheck, mailboxHandle, ramCheckIssue) == CAN_NOT_OK)
    {
      retval = CAN_NOT_OK;
    }  
  }
  /* #130 Perform Rx FullCan Mailbox Initialization */
  for (mailboxHandle = (Can_HwHandleType)Can_30_Core_GetMailboxRxFullStartIdxOfControllerConfig(Controller); mailboxHandle < (Can_HwHandleType)Can_30_Core_GetMailboxRxFullEndIdxOfControllerConfig(Controller); mailboxHandle++) /* PRQA S 2996 */ /* MD_Can_30_Core_ConstValue */ /* FETA_CAN_30_CORE_1 */
  {
    if(Can_30_Core_InitControllerMailbox(Controller, doRamCheck, mailboxHandle, ramCheckIssue) == CAN_NOT_OK) /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    {
      retval = CAN_NOT_OK; 
    }  
  }
  /* #140 Return the accumulated result for all mailboxes */
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_InitControllerMailbox()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_InitControllerMailbox
  Relation_Context:
Variant, VariantSel, DevErrorDetect
  Relation:
TxFullCan, ChannelAmount, RamCheck
RxFullCan, ChannelAmount, RamCheck
TxBasicCan, ChannelAmount, RamCheck
RxBasicCan, ChannelAmount, RamCheck
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
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
 */
/* PRQA S 6010,6030 1 */ /* MD_MSR_STPTH,MD_MSR_STCYC */
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_InitControllerMailbox(
  uint8 Controller,
  boolean doRamCheck,
  Can_HwHandleType mailboxHandle,
  P2VAR(boolean, AUTOMATIC, CAN_30_CORE_VAR_NO_INIT) ramCheckIssue)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_OK; /* Assume that the mailbox initialization is ok */
  boolean ramCheckMailBoxIssue = FALSE;
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if RAM check should be performed */
  if ((Can_30_Core_GetRamCheckOfControllerConfig(Controller) != CAN_30_CORE_RAMCHECK_NONE) && (doRamCheck == TRUE))
  {
    /* #110 loop through RAM check patterns */
    uint8 ramCheckPatternIdx = 0u;
    for (; ramCheckPatternIdx < CAN_30_CORE_PATTERN_RAMCHECK_MAX; ramCheckPatternIdx++) /* PRQA S 0771 */ /* MD_Can_30_Core_MultipleBreak */
    {
      /* #111 Call Extension InitControllerMailbox with the index to the RAM check pattern */
      if (Can_30_Core_GetInitControllerMailBoxFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, mailboxHandle, ramCheckPatternIdx) == CAN_OK) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
      {
        /* #112 Call Extension CheckControllerMailbox with the index to the RAM check pattern */
        if (Can_30_Core_GetCheckControllerMailboxFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, mailboxHandle, ramCheckPatternIdx) != CAN_OK) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
        {
          /* A RAM check fail is not a reason to skip the Initialisation process (The retval should only reflect the state of the Initialisation not the state of the RAMCheck result, so should not set retval to CAN_NOT_OK) */
          ramCheckMailBoxIssue = TRUE;
          break; /* No need to continue pattern check if RAM check failed */
        }
      }
      else
      {
        ramCheckMailBoxIssue = TRUE;
        break; /* No need to continue pattern check if RAM check failed */
      }
    }
  }
  /* #120 Call Extension InitControllerMailbox */
  if (Can_30_Core_GetInitControllerMailBoxFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, mailboxHandle, CAN_30_CORE_PATTERN_RAMCHECK_NONE) != CAN_OK) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
  {
    /* #130 Mailbox initialization failed, set issue in mailbox */
    retval = CAN_NOT_OK;
    if ((Can_30_Core_GetRamCheckOfControllerConfig(Controller) != CAN_30_CORE_RAMCHECK_NONE) && (doRamCheck == TRUE))
    {
      ramCheckMailBoxIssue = TRUE;
    }
  }
  if ((Can_30_Core_GetRamCheckOfControllerConfig(Controller) != CAN_30_CORE_RAMCHECK_NONE))
  {
    /* #140 Check if controller is initialized */
    if (!Can_30_Core_IsControllerInitialized(Controller))
    { /* #150 Only set the state of the mailbox during PowerOn initialization */
      if ((retval == CAN_OK) && (ramCheckMailBoxIssue == FALSE))
      {
        Can_30_Core_SetMailboxState(mailboxHandle, CAN_OK); /* VCA_CAN_30_CORE_MAILBOXSTATE_CTRL */
      }
      else
      {
        Can_30_Core_SetMailboxState(mailboxHandle, CAN_NOT_OK); /* VCA_CAN_30_CORE_MAILBOXSTATE_CTRL */
      }
    }
    /* #160 If Ram Check failed or mailbox init failed */
    if ((ramCheckMailBoxIssue == TRUE) || (Can_30_Core_GetMailboxState(mailboxHandle) == CAN_NOT_OK))
    {
      /* #170 Call extension to deactive the mailbox */
      Can_30_Core_GetInitControllerMailboxDeactivateFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, mailboxHandle); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
      if (ramCheckMailBoxIssue == TRUE)
      {
        /* #180 Set the RAM check Issue parameter to True */
        *ramCheckIssue = TRUE;
        /* #190 Notification for the corrupted mailbox */
        Can_30_Core_RamCheckMailboxNotification(Controller, mailboxHandle);
      }
    }
  }
  CAN_30_CORE_DUMMY_STATEMENT(ramCheckMailBoxIssue); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_RamCheckMailboxNotification()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_RamCheckMailboxNotification
  Relation_Context:
  Relation:
TxFullCan, ChannelAmount, RamCheck
RxFullCan, ChannelAmount, RamCheck
TxBasicCan, ChannelAmount, RamCheck
RxBasicCan, ChannelAmount, RamCheck
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_RamCheckMailboxNotification(uint8 Controller, Can_HwHandleType mailboxHandle) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Deactivate Mailbox */
  Can_30_Core_SetMailboxState(mailboxHandle, CAN_NOT_OK); /* VCA_CAN_30_CORE_MAILBOXSTATE_CTRL */
  /* #110 Deactivate Controller */
  Can_30_Core_AppendLogStatus(Controller, CAN_30_CORE_DEACTIVATE_CONTROLLER);
  if (Can_30_Core_GetRamCheckOfControllerConfig(Controller) == CAN_30_CORE_RAMCHECK_MAILBOX) /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
  { /* #120 Call application to notify the corrupted mailbox */
    Can_30_Core_CallApplCanCorruptMailbox(Controller, mailboxHandle);
  }
}
#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerInitialized()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsControllerInitialized
  Relation_Context:
Variant, VariantSel, DevErrorDetect, ChannelAmount, RamCheck
  Relation:
ChannelAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerInitialized(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  boolean retval = FALSE;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #5 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }  
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #10 Check if the controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Return the (boolean) initialization state of the requested controller */
    retval =  Can_30_Core_IsControllerInitialized_Internal(Controller);
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_IS_CTRL_INIT, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_Init()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_Init
  Relation_Context:
  Relation:
Variant, VariantSel, DevErrorDetect
DevErrorDetect, ChannelAmount
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
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
 */
/* PRQA S 6010,6030,6050,6080 1 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Init(Can_30_Core_ConfigPtrType ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;

  CanHookBegin_Can_30_Core_Init();

  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_IsCheckInitPointerOfGeneralFeature(0)) /* True, if the configuration is Post-Build Loadable or Post-Build Selectable with real variance */
  {
    Can_30_Core_ConfigDataPtr = ConfigPtr;
    if (Can_30_Core_IsPostBuildLoadableOfGeneralFeature(0))
    {
      if (ConfigPtr == NULL_PTR)
      { /* #10 EcuM null pointer configuration check and notification */
        Can_30_Core_BswErrorHook((uint16)CAN_30_CORE_MODULE_ID, (uint8)ECUM_BSWERROR_NULLPTR);
        errorId = CAN_30_CORE_E_INIT_FAILED;
      }
      else if ((Can_30_Core_GetFinalMagicNumber() != CAN_30_CORE_MAGIC_NUMBER)) /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
      { /* #20 EcuM wrong magic number check and notification */
        Can_30_Core_BswErrorHook((uint16)CAN_30_CORE_MODULE_ID, (uint8)ECUM_BSWERROR_MAGICNUMBER); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
        errorId = CAN_30_CORE_E_INIT_FAILED;
      }
    }
    else /* Post-Build Selectable */
    {
      if (ConfigPtr == NULL_PTR)
      { /* #30 Check if the config pointer is a null pointer */
        errorId = CAN_30_CORE_E_INIT_FAILED;
      }
    }
  }
  /* #40 Check if driver init state not uninitialized */
  if (Can_30_Core_DevCheck_IsConditionTrue(Can_30_Core_IsDriverInitialized()))
  {
    errorId = CAN_30_CORE_E_TRANSITION;
  }
  /* #50 Generator version check */
  if ((errorId == CAN_30_CORE_E_NO_ERROR) && (Can_30_Core_GetCoreGeneratorVersion() != CAN_30_CORE_GEN_BASESASR_VERSION)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  {
    if (Can_30_Core_IsPostBuildLoadableOfGeneralFeature(0)) /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    {
      Can_30_Core_BswErrorHook((uint16)CAN_30_CORE_MODULE_ID, (uint8)ECUM_BSWERROR_COMPATIBILITYVERSION);
    }
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (errorId == CAN_30_CORE_E_NO_ERROR) /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Initialize the Rx Queue */
    Can_30_Core_RxQueueInit();
    /* #120 Initialize the global init state */
    Can_30_Core_SetGlobalInitState(CAN_30_CORE_INITIALIZED);
    /* #130 Initialize the bus off handler */
    Can_30_Core_ErrorHandlerInit();
    /* #140 Initialize the mode handler */
    Can_30_Core_ModeHandlerInit();
    /* #150 Initialize the interrupt counter */
    Can_30_Core_InterruptServiceInit();
    {
      uint8 ExtensionIdx = 0u;
      /* #151  Call power-on initialization for each extension that provides it */
      for (; ExtensionIdx < Can_30_Core_GetSizeOfExtensionConfig(); ExtensionIdx++) /* FETA_CAN_30_CORE_1 */
      {
        if (Can_30_Core_GetInitPowerOnFctOfExtensionConfig(ExtensionIdx) != NULL_PTR)
        {
          ((vCan_InitPowerOnFctType)Can_30_Core_GetInitPowerOnFctOfExtensionConfig(ExtensionIdx))(); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT_PARAMEXTENSION */
        }
      }
    }
    {
      uint8 Controller = 0u;
      /* #160 loop over all controllers */
      for (; Controller < Can_30_Core_GetSizeOfControllerConfig(); Controller++)
      {
        if (Can_30_Core_IsPostBuildSelectableOfGeneralFeature(0) &&
            (Can_30_Core_GetCanIfChannelId(Controller) == CAN_30_CORE_NO_CANIFCHANNELID))
        { /* Checking whether the controller is valid which is necessary for CSL macro call in #220 */
          continue;
        }
        /* #200 Initialize the controller log status */
        Can_30_Core_SetLogStatusOfControllerData(Controller, CAN_30_CORE_STATUS_UNINIT); /* VCA_CAN_30_CORE_CONTROLLERDATA */
        /* #210 Call extension power-on initialization */
        if (Can_30_Core_GetInitControllerPowerOnFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller) == CAN_OK) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
        {
          /* #220 Set default baudrate */
          Can_30_Core_SetLastInitObjectOfControllerData(Controller, (uint8)(Can_30_Core_GetCanControllerDefaultBaudrateIdxOfControllerConfig(Controller) + Can_30_Core_GetInitObjectBaudrateStartIdxOfControllerConfig(Controller))); /* PRQA S 2985, 2986 */ /* MD_Can_30_Core_ConstValue */ /* VCA_CAN_30_CORE_CONTROLLERDATA */
          /* #230 Initialize controller */
          {
            Can_ReturnType transitionState = Can_30_Core_ModeTransition(Controller, CAN_30_COREMODESTOPREINIT, FALSE, TRUE);
            if (transitionState == CAN_OK)
            { /* #240 Set controller status to stop */
              Can_30_Core_AppendLogStatus(Controller, CAN_30_CORE_STATUS_STOP);
            }
            else
            {
              errorId = CAN_30_CORE_E_TRANSITION;
            }
          }
        } /* Can_30_Core_InitControllerPowerOn() */
        else
        {
          errorId = CAN_30_CORE_E_TRANSITION;
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_INIT, errorId);
  CanHookEnd_Can_30_Core_Init();
}

/**********************************************************************************************************************
 *  Can_30_Core_InitController()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_Init
  Relation_Context:
DevErrorDetect, ChannelAmount, RamCheck, Wakeup
DevErrorDetect, ChannelAmount, VariantSel
  Relation:
Variant, VariantSel, DevErrorDetect
DevErrorDetect, ChannelAmount, RamCheck
DevErrorDetect, ChannelAmount, Wakeup
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 2980,6010,6030,6050,6080 1 */ /* MD_Can_30_Core_ParamValueMod,MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_InitController(uint8 Controller, boolean doRamCheck)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  Can_ReturnType retval = CAN_NOT_OK;
  boolean ramCheckIssue = FALSE;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if the driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 clean up status (do not activate communication and let SLEEP and initialization to detect ram-check request) */
    if (((Can_30_Core_GetRamCheckOfControllerConfig(Controller) == CAN_30_CORE_RAMCHECK_ACTIVE) ||
        (Can_30_Core_GetRamCheckOfControllerConfig(Controller) == CAN_30_CORE_RAMCHECK_MAILBOX)) &&
        (doRamCheck == TRUE))
    {
      /* #101 standard RAM check will be performed so delete old status CAN_30_CORE_DEACTIVATE_CONTROLLER but leave CAN_30_CORE_STATUS_INIT (info about PowerOn) */
      Can_30_Core_SetLogStatusOfControllerData(Controller, Can_30_Core_GetLogStatusOfControllerData(Controller) & (uint8)CAN_30_CORE_STATUS_INIT); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    }
    else
    { /* #102 no RAM check active so delete old status except CAN_30_CORE_DEACTIVATE_CONTROLLER and CAN_30_CORE_STATUS_INIT */
      Can_30_Core_SetLogStatusOfControllerData(Controller, Can_30_Core_GetLogStatusOfControllerData(Controller) & ((uint8)CAN_30_CORE_DEACTIVATE_CONTROLLER | (uint8)CAN_30_CORE_STATUS_INIT)); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    }
    if (Can_30_Core_GetRamCheckOfControllerConfig(Controller) == CAN_30_CORE_RAMCHECK_NONE)
    { /* #110 Do not perform RAM check if feature is not active */
      doRamCheck = FALSE; /* PRQA S 951, 1338 */ /* MD_Can_30_Core_ParamValueMod */
    }
    /* #120 Clear the Tx Buffers */
    Can_30_Core_TxBufferHandlerClear(Controller);
    {
      /* #130 initialization controller states and initialization objects */
      Can_30_Core_InitHandle initObject = Can_30_Core_GetLastInitObjectOfControllerData(Controller);
      /* #140 Begin Controller Initialization */
      retval = Can_30_Core_GetInitControllerBeginFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, initObject, CAN_30_CORE_PATTERN_RAMCHECK_NONE); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
      if (retval == CAN_OK)
      {
        /* #150 Process Controller Initialization */
        retval = Can_30_Core_GetInitControllerFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, initObject, CAN_30_CORE_PATTERN_RAMCHECK_NONE); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
        /* #160 Initialize MailBoxes */
        if (Can_30_Core_InitControllerMailboxes(Controller, doRamCheck, &ramCheckIssue) == CAN_NOT_OK)
        {
          retval = CAN_NOT_OK;
        }
        /* #170 End Controller initialization */
        if (Can_30_Core_GetInitControllerEndFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, initObject, CAN_30_CORE_PATTERN_RAMCHECK_NONE) != CAN_OK) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
        {
          retval = CAN_NOT_OK;
        }
      }
    }
    if ((Can_30_Core_GetRamCheckOfControllerConfig(Controller) != CAN_30_CORE_RAMCHECK_NONE) && (ramCheckIssue == TRUE))
    { /* #200 If standard RAM check issue occur - call Can_30_Core_CallApplCanRamCheckFailed() (any mailbox failed) */
      if (Can_30_Core_CallApplCanRamCheckFailed((uint8)Controller) != CAN_30_CORE_DEACTIVATE_CONTROLLER)
      { /* #201 let the application decide if communication stay disabled as set by RAM check or not */
        Can_30_Core_SetLogStatusOfControllerData(Controller, (Can_30_Core_GetLogStatusOfControllerData(Controller) & (uint8)(~CAN_30_CORE_DEACTIVATE_CONTROLLER))); /* VCA_CAN_30_CORE_CONTROLLERDATA */
      }
      else
      {
        /* #202 Controller already deactivated in Can_30_Core_RamCheckMailboxNotification() */
        retval = CAN_NOT_OK;
      }
    }
    if (retval == CAN_OK)
    {
      /* #210 Set Controller Init Flag if no issue occurred */
      Can_30_Core_AppendLogStatus(Controller, CAN_30_CORE_STATUS_INIT);
    }
    else
    {
      /* #220 Set error if an issue occurred */
      Can_30_Core_SetLogStatusOfControllerData(Controller, Can_30_Core_GetLogStatusOfControllerData(Controller) & (uint8)CAN_30_CORE_DEACTIVATE_CONTROLLER); /* VCA_CAN_30_CORE_CONTROLLERDATA */
      errorId = CAN_30_CORE_E_UNINIT;
    }
    if ((Can_30_Core_IsWakeupSupportOfControllerConfig(Controller)) &&
        (Can_30_Core_IsIsWakeupOfControllerData(Controller))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */ /* (SPEC-15124) */
    {
      /* #230 Notify EcuM in case controller WAKEUP while initialization (SPEC-15124) */
      Can_30_Core_SetWakeupEvent(Controller, FALSE); /* COV_CAN_30_CORE_WAKEUP_IN_INIT */
      EcuM_SetWakeupEvent(Can_30_Core_GetWakeupSourceRefOfControllerConfig(Controller));
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_INITCTR, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_InitMemory()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_InitMemory
  Relation_Context:
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_InitMemory(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Set the driver state to uninitialized */
  Can_30_Core_SetGlobalInitState(CAN_30_CORE_UNINITIALIZED);
  /* #110 Reset the global configuration pointer */
  Can_30_Core_ConfigDataPtr = NULL_PTR;
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_Initialization.c
 *********************************************************************************************************************/
