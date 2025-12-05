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
/**        \file  Dcm_Svc10.h
 *         \unit  Svc10
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x10 unit.
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
#if !defined(DCM_SVC10_H)
# define DCM_SVC10_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc10Types.h"
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
/*! Service 0x10 temporary processing context */
struct DCM_SVC10REPEATERPROXYCONTEXTTYPE_TAG
{
# if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
  Dcm_ProgConditionsType    ProgConditions;          /*!< Programming condition for further processing steps */
  Rte_ModeType_DcmEcuReset  ResetMode;               /*!< Reset mode for Ecu reset */
# endif
  Dcm_StateIndexMemType     SesStateIdx;             /*!< Requested new Session State to execute transition in post processor */
};
typedef struct DCM_SVC10REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc10RepeaterProxyContextType;
typedef P2VAR(Dcm_Svc10RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc10RepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc10RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc10RepeaterProxyContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc10Context()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x10.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc10RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc10Context(
  Dcm_ThreadIdOptType threadId
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_SVC10_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc10.h
 *********************************************************************************************************************/
