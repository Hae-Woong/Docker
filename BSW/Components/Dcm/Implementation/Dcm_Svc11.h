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
/**        \file  Dcm_Svc11.h
 *         \unit  Svc11
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x11 unit.
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
#if !defined(DCM_SVC11_H)
# define DCM_SVC11_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc11Types.h"
# if (DCM_SVC_11_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
#  include "Dcm_DiagTypes.h"
#  include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Diagnostic service 0x11 reset response types */
#  define DCM_SVC11_RESPONSE_BEFORE_RESET                            (Dcm_Svc11ResponseType)0 /*!< Send response before reset */
#  define DCM_SVC11_RESPONSE_AFTER_RESET                             (Dcm_Svc11ResponseType)1 /*!< Send response after reset */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Service 0x11 temporary processing context */
struct DCM_SVC11REPEATERPROXYCONTEXTTYPE_TAG
{
#  if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
  Dcm_ProgConditionsType         ProgConditions;     /*!< Programming condition for further processing steps */
#  endif
  uint8                          SubSvcId;           /*!< Sub-function ID of the current request */
  Dcm_DiagSubServiceRefMemType   SubSvcRef;          /*!< Reference to Dcm_CfgSvc11SubFuncInfo[] entry */
};
typedef struct DCM_SVC11REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc11RepeaterProxyContextType;
typedef P2VAR(Dcm_Svc11RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc11RepeaterProxyContextPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc11Context()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x11.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc11RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc11Context(
  Dcm_ThreadIdOptType threadId
  );
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_11_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC11_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc11.h
 *********************************************************************************************************************/
