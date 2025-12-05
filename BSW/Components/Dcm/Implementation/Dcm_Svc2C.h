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
/**        \file  Dcm_Svc2C.h
 *         \unit  Svc2C
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x2C unit.
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
#if !defined(DCM_SVC2C_H)
# define DCM_SVC2C_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc2CTypes.h"
# if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
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
/*! Service 0x2C temporary processing context */
struct DCM_SVC2CREPEATERPROXYCONTEXTTYPE_TAG
{
  Dcm_DidMgrDidInfoContextType          DynDidInfoContext;      /*!< DynDID info context */
  Dcm_DidMgrDidInfoContextType          SrcItemDidInfoContext;  /*!< Source item DID info context */
  Dcm_DidMgrDidOpTypeContextType        DynDidOpTypeContext;    /*!< DynDID operation type context */
  Dcm_CfgDidMgrDynDidSrcItemIdxMemType  SrcItemIndex;           /*!< Index of currently processed source item */
  Dcm_CfgDidMgrDynDidHandleMemType      DynDidHandle;           /*!< Handle of the currently processed DynDID */
  Dcm_DidMgrDidLengthType               MaxAllowedLength;       /*!< Maximum allowed response length (depending on the protocol) */
  Dcm_DidMgrDidLengthType               DynDidLength;           /*!< Length of the DynDID */
  uint8                                 DynDidItemCount;        /*!< Number of source items in current request */
  uint8                                 ReqNumOfItems;          /*!< Number of requested items that are not yet processed */
  Dcm_DiagSubServiceRefMemType          SubSvcRef;              /*!< Index of the requested subfunction */
};
typedef struct DCM_SVC2CREPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc2CRepeaterProxyContextType;
typedef P2VAR(Dcm_Svc2CRepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2CRepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc2CRepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2CRepeaterProxyContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc2CContext()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x2C.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc2CRepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc2CContext(
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

# endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC2C_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2C.h
 *********************************************************************************************************************/
