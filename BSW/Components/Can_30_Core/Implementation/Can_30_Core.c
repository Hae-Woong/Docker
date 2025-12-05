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
/*!       \file  Can_30_Core.c
 *        \brief  Can_30_Core source file
 *        \details  Component main source file
 *        \unit Can_30_Core
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
#define CAN_30_CORE_SOURCE
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
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_Initialization.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* A component internal version check is not necessary. */

/* Check the version of the configuration header file to ensure that the generated files match the static files. */
#if defined(CAN_30_CORE_GEN_BASESASR_VERSION)
# if (CAN_30_CORE_GEN_BASESASR_VERSION != 0x0407u)
#  error "Source and Generated Header file are inconsistent!"
#endif
# else
# error "CAN_30_CORE_GEN_BASESASR_VERSION is not defined"
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* \unit Can_30_Core_General */

/**********************************************************************************************************************
 *  Can_30_Core_IsFdMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsFdMessage(Can_IdType CanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if Id Contains FD bit and return the result */
  return (((CanId) & (Can_IdType)CAN_30_CORE_ID_FD_MASK) == (Can_IdType)CAN_30_CORE_ID_FD_MASK);
}

/**********************************************************************************************************************
 *  Can_30_Core_IsExtendedIdMessage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsExtendedIdMessage(Can_IdType CanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if Id Contains IDE bit and return the result */
  return (((CanId) & (Can_IdType)CAN_30_CORE_ID_IDE_MASK) == (Can_IdType)CAN_30_CORE_ID_IDE_MASK);
}

/***********************************************************************************************************************
 * Can_30_Core_IsFdBrs()
 ***********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsFdBrs
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, CanFdSupport
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
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsFdBrs(uint8 Controller)
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
    /* #100 Check if baud rate switching is activated for the current baud rate */
    if (Can_30_Core_GetInitObjectFdBrsConfig(Can_30_Core_GetLastInitObjectOfControllerData(Controller)) != (uint8)CAN_30_CORE_INITOBJECTFDBRS_NONE)
    {
      retval = TRUE;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CTR_BRS, errorId);
  return retval;
}

/***********************************************************************************************************************
 * Can_30_Core_IsFdTxBrs()
 ***********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsFdTxBrs
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, CanFdSupport
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
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsFdTxBrs(uint8 Controller)
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
    /* #100 Check if Tx baud rate switching is activated for the current baud rate */
    if (Can_30_Core_GetInitObjectFdBrsConfig(Can_30_Core_GetLastInitObjectOfControllerData(Controller)) == (uint8)CAN_30_CORE_INITOBJECTFDBRS_FD_RXTX)
    {
      retval = TRUE;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CTR_TX_BRS, errorId);
  return retval;
}

/***********************************************************************************************************************
 * Can_30_Core_AppendCanIdFdBit()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Can_IdType, CAN_30_CORE_CODE) Can_30_Core_AppendCanIdFdBit(Can_IdType CanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Append the FD-bit to the CAN-Id */
  return (CanId | (Can_IdType) CAN_30_CORE_ID_FD_MASK);
}

/***********************************************************************************************************************
 * Can_30_Core_AppendCanIdExtBit()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Can_IdType, CAN_30_CORE_CODE) Can_30_Core_AppendCanIdExtBit(Can_IdType CanId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Append the EXT-bit to the CAN-Id */
  return (CanId | (Can_IdType) CAN_30_CORE_ID_IDE_MASK);
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* Justification for module-specific MISRA deviations:

  MD_Can_30_Core_SideEffects:
    PRQA message 3415
    Reason:     QAC assumes that side effects always occur when a function is called as the right hand operand of logical
                && or || operator.
    Risk:       Unexpected side effect occurs.
    Prevention: Verified by code inspection, that the called function has no side effect.

  MD_Can_30_Core_MultipleBreak:
    PRQA message 0771
    Reason:     More than one break statement is used to terminate the loop. This leads to code which is easier to read and
                maintain.
    Risk:       Relevant fragments of code not executed.
    Prevention: Code inspection.

  MD_Can_30_Core_ConstValue:
    PRQA message 2982, 2985, 2986 ,2991, 2992, 2994, 2995, 2996, 3678
    Reason:     Value is constant depending on configuration aspects. This leads to constant control expressions,
                unreachable code and redundant operations.
    Risk:       Wrong or missing functionality.
    Prevention: Code inspection and test of the different variants in the component test.

  MD_Can_30_Core_NoElseAfterIf:
    PRQA message 2004
    Reason:     No default handling needed for if-else-if statement here.
    Risk:       Missing default handling.
    Prevention: Code inspection and test of different variants in the component test.

  MD_Can_30_Core_ParamValueMod:
    PRQA message 1338, 2980
    Reason:     The direct modification of the function parameter leads to code which is easier to read and maintain.
    Risk:       Incorrect expectation of the value of the modified function parameter.
    Prevention: Code inspection and test of the different variants in the component test.

  MD_Can_30_Core_NotUsedInUnit:
    PRQA message 3219
    Reason:     Static inline function is not used within this translation unit, because it is intended to be used as
                a inline function by other units.
    Risk:       None.
    Prevention: None.

  MD_Can_30_Core_DecInc:
    PRQA message 3387
    Reason:     QAC expects other possible side effects that are not only caused by incrementing or decrementing the
                variable, because the variable is marked as volatile.
    Risk:       Undefined behaviour, if code was changed at this line.
    Prevention: Verified by code inspection, that the variable is being only incremented or decremented.

  MD_Can_30_Core_ExternalScope:
    PRQA message 3449, 3451
    Reason:     The variable or function is used by other units or modules and can't be declared static.
    Risk:       Name conflicts.
    Prevention: Compile and link of the different variants in the component and integration test.

*/


/**********************************************************************************************************************
 *  Coverage Justification                                                  
 *********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

\ID COV_CAN_30_CORE_UNUSED_PREPROCESSOR
  \ACCEPT TX
  \REASON Unused preprocessor switch (only required for unit test).

\ID COV_CAN_30_CORE_DEFINE_PROTECTION
  \ACCEPT TX
  \REASON The condition checks whether a switch is defined and conditionally assigns a default value.

\ID COV_CAN_30_CORE_WAKEUP_IN_INIT
  \ACCEPT XF
  \REASON The condition checks for wakeup event during initialization that cannot be stimulated. The code is verified by code inspection.

\ID COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG
  \ACCEPT XX
  \REASON Function will not be executed for each configuration due to the function depending on a feature which may be deactivated.


END_COVERAGE_JUSTIFICATION */ 


/**********************************************************************************************************************
 *  VCA Justification
 *********************************************************************************************************************/
/* VCA_JUSTIFICATION_BEGIN

\ID VCA_CAN_30_CORE_CONTROLLERDATA
\DESCRIPTION Write access to a member of Can_30_Core_ControllerData array with controller as index.
\COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib ensures, that the controller index is always in bounce of Can_30_Core_ControllerConfig 
                   which has same size than Can_30_Core_ControllerData.

\ID VCA_CAN_30_CORE_CONTROLLERDATA_HTRH
\DESCRIPTION Write access to a member of Can_30_Core_ControllerData array with controller as index derived from Can_30_Core_Mailbox indirection.
\COUNTERMEASURE \N Qualified use-case CSL02 and CSL03 of ComStackLib ensures that the controller index is always in bounce of Can_30_Core_ControllerConfig 
                   which has same size than Can_30_Core_ControllerData derived from Can_30_Core_Mailbox indirection.

\ID VCA_CAN_30_CORE_FCTPOINTERTOEXT
\DESCRIPTION Function call via function pointer: Access to Can_30_Core_ExtensionConfig via indirection over Can_30_Core_ControllerConfig with parameter controller to 
             get the required function pointer. The indirection to Can_30_Core_ExtensionConfig is ensured via qualified use-case CSL03 of ComStackLib.
             The controller index is ensured by 'Can_30_Core_IsControllerValid()' that a unused and invalid controller index for the selected variant is not accessed.
\COUNTERMEASURE \S For the generated function table Can_30_Core_ExtensionConfig, it must be secured that valid service functions are provided with the valid signature SMI-1030730.
                \M Verify that 'ExtensionConfigIdx' in table 'Can_30_Core_ControllerConfig' is not invalid, for used controllers in selected variant.

\ID VCA_CAN_30_CORE_FCTPOINTERTOEXT_HTRH
\DESCRIPTION Function call via function pointer: Access to Can_30_Core_ExtensionConfig via indirection over Can_30_Core_ControllerConfig with parameter controller to 
             get the required function pointer. The controller index is derived from Can_30_Core_Mailbox indirection secured via qualified use-case CSL03 of ComStackLib.
             The controller index is ensured by 'Can_30_Core_IsControllerValid()' that a unused and invalid controller index for the selected variant is not accessed.
\COUNTERMEASURE \S For the generated function table Can_30_Core_ExtensionConfig, it must be secured that valid service functions are provided with the valid signature SMI-1030730.
                \M Verify that 'ExtensionConfigIdx' in table 'Can_30_Core_ControllerConfig' is not invalid, for used controllers in selected variant.

\ID VCA_CAN_30_CORE_FCTPOINTERTOEXT_PARAMEXTENSION
\DESCRIPTION Function call via function pointer: Access to Can_30_Core_ExtensionConfig with parameter extension index to get the required function pointer.
             The extension index is ensured by SizeOfExtensionConfig via qualified use-case CSL01 of ComStackLib.
\COUNTERMEASURE \S For the generated function table Can_30_Core_ExtensionConfig, it must be secured that valid service functions are provided with the valid signature SMI-1030730.

\ID VCA_CAN_30_CORE_MAILBOXSTATE
\DESCRIPTION Write access to Can_30_Core_MailboxState array with mailboxHandle as index.
\COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib ensures that mailboxHandle is within the size of Can_30_Core_MailboxState.

\ID VCA_CAN_30_CORE_MAILBOXSTATE_CTRL
\DESCRIPTION Write access to Can_30_Core_MailboxState array with mailboxHandle as index derived from Can_30_Core_ControllerConfig indirection.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib ensures, that Can_30_Core_Mailbox handle is within the start-end range derived from Can_30_Core_ControllerConfig. 
                   Qualified use-case CSL02 ensures that the size of table Can_30_Core_Mailbox is equal to Can_30_Core_MailboxState.

\ID VCA_CAN_30_CORE_RXQUEUEBUFFER
\DESCRIPTION Write access to Can_30_Core_RxQueueBuffer array with writeIndex as index.
\COUNTERMEASURE \R [CM_CAN_30_CORE_RXQUEUEBUFFER]

\ID VCA_CAN_30_CORE_RXQUEUEBUFFERDATA
\DESCRIPTION Write access to Can_30_Core_RxQueueBuffer data member with writeIndex as index in array and length to copy data.
\COUNTERMEASURE \R [CM_CAN_30_CORE_RXQUEUEBUFFER]
                \R A runtime check ensures that length used to copy data in the 'data' struct member of Can_30_Core_RxQueueBuffer does not exceed the size of 'data'.

\ID VCA_CAN_30_CORE_CALLAPPLGENERICPRECOPY
\DESCRIPTION A function with pointer parameter is called, which may lead to unintended write access.
\COUNTERMEASURE \N Contained var pointer is defined as read only by declaration.

\ID VCA_CAN_30_CORE_CALLVSTDMEMCPYTXREQUEST
\DESCRIPTION A function with pointer parameter is called, which may lead to out of bounce write access.
\COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib ensures that the length of the data to be copied is equal or smaller as the size of the memory represented by the destination pointer.

\ID VCA_CAN_30_CORE_CALLVSTDMEMCPYCONFIRMATION
\DESCRIPTION The function VStdMemCpy() with pointer parameter is called, which may lead to out of bounce write access if passed data length value exceeds the size of the given data pointer.
             The data pointer refer a local defined memory with size of 64.
\COUNTERMEASURE \R A runtime check in Can_30_Core_Write() ensures that the given data length does not exceed Can_30_Core_GetMaxDataLenOfMailbox().
                \M Verify that all entries of the struct member 'MaxDataLen' in table 'Can_30_Core_Mailbox' are smaller than the 65.

\ID VCA_CAN_30_CORE_CALLAPPLGENERICPRETRANSMIT
\DESCRIPTION A function with pointer parameter is called, which may lead to out of bounce write access.
\COUNTERMEASURE \N The parameter PduPtr contain the data pointer and length information. The declaration restrict a data write access beyond the length of the data.
                   The dimension of Can_30_ActiveSendData[] is never smaller than the length, secured by function Can_30_StoreData() when the buffer is stored as a runtime check.

\ID VCA_CAN_30_CORE_TXHWFIFO
\DESCRIPTION Write access to a member of Can_30_Core_TxHwFifo array with txFifoIndex as index.
\COUNTERMEASURE \N The txFifoIndex is get by Can_30_Core_GetTxHwFifoIdxOfMailbox that is secured by CSL03 of ComStackLib so that it will not access beyond the size of Can_30_Core_TxHwFifo memory.

\ID VCA_CAN_30_CORE_ACTIVESENDOBJECT
\DESCRIPTION Write access to a member of Can_30_Core_ActiveSendObject array with activeObject as index.
\COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib ensures that activeObject as index does not exceed the size of the used array.

\ID VCA_CAN_30_CORE_ERRORSTATEPTR
\DESCRIPTION Write access to address ErrorStatePtr which is given as parameter in Can_30_Core_GetControllerErrorState().
\COUNTERMEASURE \N The function parameter is defined as valid address in API description. The parameter is checked against null pointer as DET check.

\ID VCA_CAN_30_CORE_RXERRORCOUNTERPTR
\DESCRIPTION Write access to address RxErrorCounterPtr which is given as parameter in Can_30_Core_GetControllerRxErrorCounter().
\COUNTERMEASURE \N The function parameter is defined as valid address in API description. The parameter is checked against null pointer as DET check.

\ID VCA_CAN_30_CORE_TXERRORCOUNTERPTR
\DESCRIPTION Write access to address TxErrorCounterPtr which is given as parameter in Can_30_Core_GetControllerTxErrorCounter().
\COUNTERMEASURE \N The function parameter is defined as valid address in API description. The parameter is checked against null pointer as DET check.

\ID VCA_CAN_30_CORE_CONTROLLERMODEPTR
\DESCRIPTION Write access to address ControllerModePtr which is given as parameter in Can_30_Core_GetControllerMode().
\COUNTERMEASURE \N The function parameter is defined as valid address in API description. The parameter is checked against null pointer as DET check.

\ID VCA_CAN_30_CORE_CALLGETCOUNTERVALUE
\DESCRIPTION A function with pointer parameter is called, which may lead to unintended write access.
\COUNTERMEASURE \N Qualified use case CSL01 of ComStackLib ensures that the parameter points to a valid memory.

\ID VCA_CAN_30_CORE_CALLGETELAPSEDVALUE
\DESCRIPTION A function with pointer parameter is called, which may lead to unintended write access. 
\COUNTERMEASURE \N The pointer parameter is local defined and is always valid.

\ID VCA_CAN_30_CORE_VERSIONINFO
\DESCRIPTION Write access to address VersionInfo which is given as parameter in Can_30_Core_GetVersionInfo().
\COUNTERMEASURE \N The function parameter is defined as valid address in API description. The parameter is checked against null pointer as DET check.

\ID VCA_CAN_30_CORE_CALLAPPLGENERICCONFIRMATIONAPI2
\DESCRIPTION A function with pointer parameters is directly called, but the function is not
             defined within the analyzed sources. VCA is unable to determine the
             behavior of the function.
\COUNTERMEASURE \N The Compiler ensures the validity of the pointers because the pointer parameter is a constant pointer to a struct.
                   This struct also contain a pointer but this is defined as read only by declaration.

\ID VCA_CAN_30_CORE_CALLCANIFCANCELTXCONFIRMATION
\DESCRIPTION A function with pointer parameters is directly called, but the function is not
             defined within the analyzed sources. VCA is unable to determine the
             behavior of the function.
\COUNTERMEASURE \N The Compiler ensures the validity of the pointers because the pointer parameter is a constant pointer to a struct.
                   This struct also contain a pointer but this is defined as read only by declaration.

VCA_JUSTIFICATION_END */

/*
\CM CM_CAN_30_CORE_RXQUEUEBUFFER A runtime check in Can_30_Core_UpdateWriteIndex() ensures that writeIndex is always within the size of Can_30_Core_RxQueueBuffer.
*/


/**********************************************************************************************************************
 *  FETA JUSTIFICATIONS
 *********************************************************************************************************************/

/* FETA_JUSTIFICATION_BEGIN

\ID FETA_CAN_30_CORE_1
\DESCRIPTION The for loop iterates over mailboxes, controllers, baudrate or extension tables. The amount of elements depends on the configuration.
\COUNTERMEASURE \N The access to start and stop indexes of generated data is ensured via qualified use-case CSL03 of ComStackLib.
                   The access to SizeOf of generated data is ensured via qualified use-case CSL01 of ComStackLib.

\ID FETA_CAN_30_CORE_2
\DESCRIPTION The while loop has external termination condition computation (calls to vCan_30_*_ModeTransition).
             The termination condition depends on the hardware to complete the requested action or until a timeout occurs.
             The loop execution time is limited by the Can_30_Core_TimerLoop and therefore definite.
\COUNTERMEASURE \D DSGN-Can-TimeoutDetection
                \S SMI-1141633
                \S SMI-1141634

\ID FETA_CAN_30_CORE_3
\DESCRIPTION The while loop polls Rx messages from the queue until the queue is empty or the limit for the number of messages to be processed is reached.
             Rx messages may be received by the controller and pushed into the queue concurrently but the loopTimeout iteration counter ensures that the loop execution time remains finite.
\COUNTERMEASURE \R An iteration counter ensures that the loop is terminated after the defined limit is reached.

FETA_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core.c
 *********************************************************************************************************************/
