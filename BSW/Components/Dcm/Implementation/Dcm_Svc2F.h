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
/**        \file  Dcm_Svc2F.h
 *         \unit  Svc2F
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x2F unit.
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
#if !defined(DCM_SVC2F_H)
# define DCM_SVC2F_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc2FTypes.h"
# if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
#  include "Dcm_DidMgr.h"
#  include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Repeater context for the handling of a 2F request */
struct DCM_SVC2FREPEATERPROXYCONTEXTTYPE_TAG
{
  Dcm_DidMgrDidInfoContextType    DidInfoContext;   /*!< DID information context */
  Dcm_DidMgrDidOpTypeContextType  DidOpTypeContext; /*!< DID operation type context */
  Dcm_DidMgrOpMemType             OpType;           /*!< Requested operation type */
};
typedef struct DCM_SVC2FREPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc2FRepeaterProxyContextType;
typedef P2VAR(Dcm_Svc2FRepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT)  Dcm_Svc2FRepeaterProxyContextPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc2FContext()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x2F.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc2FRepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc2FContext(
  Dcm_ThreadIdOptType threadId
  );

#  if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2FReturnControlToEcu()
 *********************************************************************************************************************/
/*! \brief          Reset all active IO controls.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2FReturnControlToEcu(
  void
  );
#  endif

#  if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2FOnStateChanged()
 *********************************************************************************************************************/
/*! \brief          Reset all active IO controls no more supported in the active DCM state.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2FOnStateChanged(
  void
  );
#  endif
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC2F_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2F.h
 *********************************************************************************************************************/
