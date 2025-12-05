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
/**        \file  Dcm_Repeater.h
 *         \unit  Repeater
 *        \brief  Contains public types, function declarations and inline function definitions of Repeater unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#if !defined(DCM_REPEATER_H)
# define DCM_REPEATER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_RepeaterTypes.h"
# include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_REPEATER_PROGRESS_PRE_HANDLER                           ((Dcm_RepeaterProgressType)0)

# define DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL               ((Dcm_RepeaterProgressType)1)
# define DCM_REPEATER_PROGRESS_USERS_FIRST                           ((Dcm_RepeaterProgressType)2)
# define DCM_REPEATER_PROGRESS_USERS_LAST                            ((Dcm_RepeaterProgressType)253)

# define DCM_REPEATER_PROGRESS_POST_HANDLER                          ((Dcm_RepeaterProgressType)254)

# define DCM_REPEATER_PROGRESS_INVALID                               ((Dcm_RepeaterProgressType)255)
/* all other progress types are client /service processor specific and shall start with DCM_REPEATER_PROGRESS_USERS_FIRST and end with DCM_REPEATER_PROGRESS_USERS_LAST */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetProgress()
 *********************************************************************************************************************/
/*! \brief          Gets next step of repeater proxy.
 *  \details        -
 *  \param[in]      pContext    Pointer to the repeater thread context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(Dcm_RepeaterProgressType, DCM_CODE) Dcm_RepeaterGetProgress(
  Dcm_ContextPtrType pContext
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterGetSubProgress()
 *********************************************************************************************************************/
/*! \brief          Gets next substep of repeater proxy.
 *  \details        -
 *  \param[in]      pContext    Pointer to the repeater thread context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(Dcm_RepeaterProgressType, DCM_CODE) Dcm_RepeaterGetSubProgress(
  Dcm_ContextPtrType pContext
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterNextSubStep()
 *********************************************************************************************************************/
/*! \brief          Sets next substep of repeater proxy.
 *  \details        -
 *  \param[in]      pContext    Pointer to the repeater thread context
 *  \param[in]      nextSubStep Next substep to be processed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RepeaterNextSubStep(                                                                                                        /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_ContextPtrType pContext,
  Dcm_RepeaterProgressType nextSubStep
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterNextStep()
 *********************************************************************************************************************/
/*! \brief          Sets next step of repeater proxy.
 *  \details        -
 *  \param[in]      pContext    Pointer to the repeater thread context
 *  \param[in]      nextStep    Next step to be processed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RepeaterNextStep(                                                                                                           /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_ContextPtrType pContext,
  Dcm_RepeaterProgressType nextStep
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterSetCallee()
 *********************************************************************************************************************/
/*! \brief          Sets a new callee for the repeater proxy.
 *  \details        -
 *  \param[in]      pContext               Pointer to the context
 *  \param[in]      serviceProcessorIdx    The proxy callee
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RepeaterSetCallee(
  Dcm_ContextPtrType pContext,
  uint8 serviceProcessorIdx
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterSetOpStatus()
 *********************************************************************************************************************/
/*! \brief          Sets the OpStatus for the repeater proxy.
 *  \details        -
 *  \param[in]      pContext               Pointer to the context
 *  \param[in]      opStatus               The operating status of the service processing
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_RepeaterSetOpStatus(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterExecute()
 *********************************************************************************************************************/
/*! \brief          Starts polling a repeater proxy.
 *  \details        -
 *  \param[in]      pContext               Pointer to the context
 *  \param[out]     ErrorCode              The NRC
 *  \return         DCM_E_OK               Operation is done positive
 *  \return         DCM_E_NOT_OK           Operation is done negative
 *  \return         DCM_E_PENDING          Final result is pending, retry later
 *  \return         DCM_E_FORCE_RCRRP      Forces a RCR-RP response
 *  \return         DCM_E_PENDING_LIMITER  Runtime limit reached, retry later
 *  \return         DCM_E_PROCESSINGDONE   Send response
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterExecute(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

# if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterCheckAndGetAccess()
 *********************************************************************************************************************/
/*! \brief          Checks whether the access for a Sid is granted or tries to get it.
 *  \details        -
 *  \param[in]      pContext    Pointer to the context
 *  \param[out]     ErrorCode      NRC
 *  \return         DCM_E_OK       Service is locked
 *  \return         DCM_E_PENDING  Service is not locked, try later
 *  \return         DCM_E_NOT_OK   In case of cancellation
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterCheckAndGetAccess(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_REPEATER_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Repeater.h
 *********************************************************************************************************************/
