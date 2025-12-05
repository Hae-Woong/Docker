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
/**        \file  Dcm_Svc22.h
 *         \unit  Svc22
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x22 unit.
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
#if !defined(DCM_SVC22_H)
# define DCM_SVC22_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc22Types.h"
# if (DCM_SVC_22_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
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
/*! Service 0x22 temporary processing context */
struct DCM_SVC22REPEATERPROXYCONTEXTTYPE_TAG
{
  Dcm_DidMgrDidInfoContextType     DidInfoContextList[DCM_SVC_22_MAX_DID_LIST_LEN];  /*!< Array of DidInfoContext */
  Dcm_DidMgrDidOpTypeContextType   DidOpTypeContext;                                 /*!< Did configuration operation context */
  Dcm_MsgLenType                   ActualLength;                                     /*!< Actual response data length  */
  Dcm_MsgLenType                   ReservedLength;                                   /*!< Reserved buffer size needed for reading the data */
  uint8                            DidInfoContextIdx;                                /*!< Reference to DidInfoContextList[] entry */
  uint8                            NumDids;                                          /*!< Number of Dids to process */
  uint8                            DidIter;                                          /*!< Current Did to process */
};
typedef struct DCM_SVC22REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc22RepeaterProxyContextType;
typedef P2VAR(Dcm_Svc22RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc22RepeaterProxyContextPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc22Context()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x22.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc22RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc22Context(
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

# endif /* (DCM_SVC_22_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC22_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc22.h
 *********************************************************************************************************************/
