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
/*!        \file  Com_Reporting.c
 *         \unit  Reporting
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_REPORTING_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com.h"
#include "Com_Reporting.h"
#include "Com_Cfg_Reporting.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#if(COM_USE_INIT_POINTER == STD_ON)
# include "EcuM_Error.h"
#endif
#if (COM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
 * Com_Reporting_CallDetReportError
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Reporting_CallDetReportError(uint8 apiId, uint8 errorCode)
{
#if (COM_DEV_ERROR_REPORT == STD_ON)
  /* If the error code indicates an error. */
  if(errorCode != COM_E_NO_ERROR)
  {
    /* Call Det module with a development error. */
    (void) Det_ReportError(COM_MODULE_ID, 0u, apiId, errorCode);
  }
#endif

  COM_DUMMY_STATEMENT(apiId);           /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(errorCode);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

#if(COM_USE_INIT_POINTER == STD_ON)
/**********************************************************************************************************************
 * Com_Reporting_CallBswErrorHook
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Reporting_CallBswErrorHook(uint8 errorId)
{
#if(COM_USE_ECUM_BSW_ERROR_HOOK == STD_ON) /* COV_COM_ECUM_BSW_ERROR_HOOK */
  EcuM_BswErrorHook(COM_MODULE_ID, errorId);
#endif

  COM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

/**********************************************************************************************************************
  Com_Reporting_CallNotification
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Reporting_CallNotification(Com_ReportingIterType reportingIdx)
{
  if(Com_HasMultiIndirection2FuncPtrOfReporting() && (Com_GetMultiIndirection2FuncPtrOfReporting(reportingIdx) == COM_NOTIFICATIONS_MULTIINDIRECTION2FUNCPTROFREPORTING))
  {
    const ComNotificationType notification = Com_GetNotifications((Com_NotificationsIterType) Com_GetMultiIndirection2FuncPtrIdxOfReporting(reportingIdx));
    if(notification != NULL_PTR)
    {
      notification (); /* VCA_COM_VALID_FUNCTION_POINTER */
    }
  }
}

/**********************************************************************************************************************
Com_Reporting_CallTxIpduCallout
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Reporting_CallTxIpduCallout(Com_ReportingIterType reportingIdx, Com_TxPduInfoIterType txPduInfoIdx, P2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr)
{
  boolean calloutResult = TRUE; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the tx ComIPdu has a configured callout */
  if(Com_HasMultiIndirection2FuncPtrOfReporting() && (Com_GetMultiIndirection2FuncPtrOfReporting(reportingIdx) == COM_TXIPDUCALLOUTS_MULTIINDIRECTION2FUNCPTROFREPORTING))
  {
    /* Call the callout with the pduInfoPtr */
    if(Com_GetTxIPduCallouts((Com_TxIPduCalloutsIterType) Com_GetMultiIndirection2FuncPtrIdxOfReporting(reportingIdx)) != NULL_PTR)
    {
      calloutResult = Com_GetTxIPduCallouts((Com_TxIPduCalloutsIterType) Com_GetMultiIndirection2FuncPtrIdxOfReporting(reportingIdx)) ((PduIdType) txPduInfoIdx, pduInfoPtr); /* VCA_COM_CALL_EXTERNAL_FUNCTION_VAR_POINTER_ARGUMENT */
    }
  }
  return calloutResult;
}

/**********************************************************************************************************************
Com_Reporting_CallRxIpduCallout
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Reporting_CallRxIpduCallout(Com_ReportingIterType reportingIdx, Com_RxPduInfoIterType rxPduInfoIdx, P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) pduInfoPtr)
{
  boolean calloutResult = TRUE; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the rx ComIPdu has a configured callout */
  if(Com_HasMultiIndirection2FuncPtrOfReporting() && (Com_GetMultiIndirection2FuncPtrOfReporting(reportingIdx) == COM_RXIPDUCALLOUTS_MULTIINDIRECTION2FUNCPTROFREPORTING))
  {
    /* Call the callout with the pduInfoPtr */
    if(Com_GetRxIPduCallouts((Com_RxIPduCalloutsIterType) Com_GetMultiIndirection2FuncPtrIdxOfReporting(reportingIdx)) != NULL_PTR)
    {
      calloutResult = Com_GetRxIPduCallouts((Com_RxIPduCalloutsIterType) Com_GetMultiIndirection2FuncPtrIdxOfReporting(reportingIdx))((PduIdType) rxPduInfoIdx, (P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC)) pduInfoPtr); /* VCA_COM_CALL_EXTERNAL_FUNCTION_VAR_POINTER_ARGUMENT */
    }
  }
  return calloutResult;
}


#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_Reporting.c
 *********************************************************************************************************************/
