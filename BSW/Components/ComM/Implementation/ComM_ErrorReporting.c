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
/*!         \file     ComM_ErrorReporting.c
 *          \unit     ErrorReporting
 *          \brief    Communication Manager ASR4
 *
 *          \details  Implementation of the Autosar Communication Manager for the ErrorReporting unit.
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

#define CCL_ASR_COMM_ERRORREPORTING_SOURCE

/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "ComM.h"
#include "ComM_ErrorReporting.h"

 /**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#define COMM_START_SEC_CODE
 /* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"

/**********************************************************************************************************************
 *  ComM_ErrorReporting_ProcessDetErrorReport()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_ErrorReporting_ProcessDetErrorReport( uint8 serviceId, uint8 errorId )
{
  /* #10 Check if the error Id code indicates an error. */
#if (COMM_DEV_ERROR_REPORT == STD_ON)
  if ( errorId != COMM_E_NO_ERROR )
  {
    (void)Det_ReportError( COMM_MODULE_ID, COMM_INSTANCE_ID_DET, serviceId, errorId );
  }
#endif

  COMM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  COMM_DUMMY_STATEMENT( serviceId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

/**********************************************************************************************************************
 *  ComM_ErrorReporting_ProcessBswHookError()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( void, COMM_CODE ) ComM_ErrorReporting_ProcessBswHookError( uint8 errorId )
{
  /* #10 Report the indicated error. */
#if (COMM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
  EcuM_BswErrorHook( COMM_MODULE_ID, errorId );
#endif

  COMM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
}

#define COMM_STOP_SEC_CODE
/* PRQA S 5087 1 */ /* MD_MSR_MemMap */
#include "ComM_MemMap.h"
