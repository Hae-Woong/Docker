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
/**        \file  Dcm_Svc2A.h
 *         \unit  Svc2A
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x2A unit.
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
#if !defined(DCM_SVC2A_H)
# define DCM_SVC2A_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc2ATypes.h"
# if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
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
#  define Dcm_Svc2AIsPeriodicDid(did)                                ((((uint16)(did)) & 0xFF00u) == 0xF200u)                                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Dcm_Svc2ATransmissionModeType;

/*! Service 0x2A temporary processing context */
struct DCM_SVC2AREPEATERPROXYCONTEXTTYPE_TAG
{
  Dcm_DidMgrDidInfoContextType    DidInfoContext;          /*!< Info context of the currently requested DID */
  Dcm_DidMgrDidOpTypeContextType  DidOpTypeContext;        /*!< Operation type context of the currently requested DID */
  Dcm_NetConnRefMemType           ConnHdl;                 /*!< Reference to Dcm_CfgNetConnectionInfo[] entry */
  Dcm_Svc2ATransmissionModeType   UdsRate;                 /*!< Requested UDS rate */
  boolean                         HasAnySupportedDids;     /*!< Flag whether current request contains a supported DID */
};
typedef struct DCM_SVC2AREPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc2ARepeaterProxyContextType;
typedef P2VAR(Dcm_Svc2ARepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2ARepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc2ARepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2ARepeaterProxyContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc2AContext()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x2A.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc2ARepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc2AContext(
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

# endif /* (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC2A_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2A.h
 *********************************************************************************************************************/
