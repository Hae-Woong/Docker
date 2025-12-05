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
/**        \file  Dcm_Svc28.h
 *         \unit  Svc28
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x28 unit.
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
#if !defined(DCM_SVC28_H)
# define DCM_SVC28_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc28Types.h"
# if (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
#  include "Dcm.h"
#  include "Dcm_DiagTypes.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define DCM_SVC_28_CTRL_OP_MASK                                    (0x03u)/*!< Leaves the relevant bits for the conversion function "sub-function id -> operation type" since only sub-functions 0x00-0x03 are handled in DCM */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Service 0x28 temporary processing context */
struct DCM_SVC28REPEATERPROXYCONTEXTTYPE_TAG
{
  Dcm_CommunicationModeType     ComMode;               /*!< Calculated CommunicationControl ModeDeclarationGroup mode value of the current service request */
  Dcm_CfgNetNetIdRefMemType     NetworkRef;            /*!< ComM channel of the current service request */
  uint8                         SubServiceId;          /*!< Sub-function ID of the current service request */
  boolean                       IsInternallyProcessed; /*!< Specifies whether the processed sub-function is internally handled (TRUE) or within application code (FALSE)*/
  Dcm_DiagSubServiceRefMemType  SubSvcRef;             /*!< A validly dispatched sub-function requested for this diagnostic service */
};
typedef struct DCM_SVC28REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc28RepeaterProxyContextType;
typedef P2VAR(Dcm_Svc28RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc28RepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc28RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc28RepeaterProxyContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc28Context()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x28.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc28RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc28Context(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_Svc28EnableCommunication()
 *********************************************************************************************************************/
/*! \brief          Enable communication on all affected channels.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc28EnableCommunication(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc28OnChangeSession()
 *********************************************************************************************************************/
/*! \brief         Service 0x28 utility function about a session change.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc28OnChangeSession(
  void
  );
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_28_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC28_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc28.h
 *********************************************************************************************************************/
