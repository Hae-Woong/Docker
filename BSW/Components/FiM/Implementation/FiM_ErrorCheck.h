/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!        \file     FiM_ErrorCheck.h
 *         \unit     ErrorCheck
 *         \brief    Internal Header for Autosar 4 module FiM - Unit Error Check
 *
 *         \details  Static source file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  
 *  see FiM.h
 *  
 **********************************************************************************************************************/

#if !defined(FIM_ERRORCHECK_H)
#define FIM_ERRORCHECK_H

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
*  INCLUDES
***********************************************************************************************************************/

#include "Std_Types.h"
#include "FiM_Cfg.h"
#include "FiM_Types.h"
#include "FiM_Int.h"
#include "FiM.h"

#if (FIM_DEV_ERROR_REPORT == STD_ON)
  /* Include Development Error Tracer */
# include "Det.h"                        
#endif


/***********************************************************************************************************************
*  UNIT LOCAL FUNCTION MACROS
***********************************************************************************************************************/



/***********************************************************************************************************************
*  UNIT LOCAL DATA
***********************************************************************************************************************/


/***********************************************************************************************************************
 *  UNIT EXTERNAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  public FiM_ErrorCheck_IsDevErrorDetected()
 **********************************************************************************************************************/
 /*! \brief                   Handling of DevDetectError call depending on configuration.
 *
 *  \details                  See brief.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_ErrorCheck_IsDevErrorDetected(boolean condition);

/***********************************************************************************************************************
 *  public FiM_ErrorCheck_IsRuntimeErrorDetected()
 **********************************************************************************************************************/
 /*! \brief                   Handling of DevRuntimeCheck call depending on configuration.
 *
 *  \details                  Returns False if runtime checks are not configured.
 *  \return                   TRUE:   Runtime checks are configured and condition is TRUE.
 *                            FALSE:  Otherwise.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE) FiM_ErrorCheck_IsRuntimeErrorDetected(boolean condition);

/***********************************************************************************************************************
 *  public FiM_ErrorCheck_ReportError()
 **********************************************************************************************************************/
 /*! \brief                   Handling of DevReportError call depending on configuration.
 *
 *  \details                  See brief.
 *  \pre                      -
 *  \context                  TASK|ISR2
 *  \synchronous              TRUE
 *  \reentrant                TRUE
 **********************************************************************************************************************/
FIM_LOCAL_INLINE FUNC(void, FIM_CODE)  FiM_ErrorCheck_ReportError(uint8 apiId, uint8 errorId);



#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */



/**********************************************************************************************************************
 *  UNIT EXTERNAL INLINE FUNCTIONS
 **********************************************************************************************************************/

#if (!defined (FIM_NOUNIT_ERRORCHECK))                                                         /* COV_FIM_UNITTEST TX */

#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */



/**********************************************************************************************************************
 *  FiM_ErrorCheck_IsErrorDetected()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_ErrorCheck_IsDevErrorDetected(boolean condition)                              /* PRQA S 3219 */ /* MD_FiM_3219 */
{
#if FIM_DEV_ERROR_DETECT == STD_ON                                                          /* COV_FIM_DET_DETECT TX */
  return (condition);
#else
  FIM_DUMMY_STATEMENT(condition);                                              /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (FALSE);
#endif
}

/**********************************************************************************************************************
 *  FiM_ErrorCheck_IsRuntimeErrorDetected()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(boolean, FIM_CODE)
  FiM_ErrorCheck_IsRuntimeErrorDetected(boolean condition)             /* PRQA S 3219, 1527 */ /* MD_MSR_Unreachable */
{
#if FIM_DEV_RUNTIME_CHECKS == STD_ON                                                    /* COV_FIM_RUNTIME_CHECKS TX */
  return (condition);
#else
  FIM_DUMMY_STATEMENT(condition);                                              /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return (FALSE);
#endif
}


/**********************************************************************************************************************
 *  public FiM_ErrorCheck_ReportError()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FIM_LOCAL_INLINE FUNC(void, FIM_CODE)
  FiM_ErrorCheck_ReportError(uint8 apiId, uint8 errorId) /* PRQA S 3219 */ /* MD_FiM_3219 */
{
#if FIM_DEV_ERROR_REPORT == STD_ON
    /* if error reporting is on and error was detected report DET */
  if (errorId != FIM_E_NO_ERROR)
  {
    (void)Det_ReportError(FIM_MODULE_ID, FIM_INSTANCE_ID_DET, apiId, errorId);
  }
#else
  FIM_DUMMY_STATEMENT(apiId);                        /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
  FIM_DUMMY_STATEMENT(errorId);                      /*lint -e{438} */ /* PRQA S 1338, 2983 */ /*  MD_MSR_DummyStmt */
#endif
}


#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* FIM_NOUNIT_ERRORCHECK */


#endif /* FIM_ERRORCHECK_H */

/***********************************************************************************************************************
*  END OF FILE: FiM_ErrorCheck.h
***********************************************************************************************************************/
