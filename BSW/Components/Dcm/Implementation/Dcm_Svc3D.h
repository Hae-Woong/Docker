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
/**        \file  Dcm_Svc3D.h
 *         \unit  Svc3D
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x3D unit.
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
#if !defined(DCM_SVC3D_H)
# define DCM_SVC3D_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc3DTypes.h"
# if (DCM_SVC_3D_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
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
/*! Service 0x3D temporary processing context */
struct DCM_SVC3DREPEATERPROXYCONTEXTTYPE_TAG
{
  Dcm_MemMgrMemBlockType  MemBlock; /*!< Shared memory block information */
};
typedef struct DCM_SVC3DREPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc3DRepeaterProxyContextType;
typedef P2VAR(Dcm_Svc3DRepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT)    Dcm_Svc3DRepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc3DRepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT)  Dcm_Svc3DRepeaterProxyContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc3DContext()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x3D.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc3DRepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc3DContext(
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

# endif /* (DCM_SVC_3D_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC3D_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc3D.h
 *********************************************************************************************************************/
