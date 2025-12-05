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
/**        \file  Dcm_Svc86.h
 *         \unit  Svc86
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x86 unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/* *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author     Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.00  2013-07-18  vissa      ESCAN00068413 AR4-462: Support of GM Diagnostic specification(s)
 *                        vissa      ESCAN00068551 AR4-231: Implement ROE according to GM norms
 *  02.00.01  2013-12-05  vissa      ESCAN00071467 Dcm causes DET invocation in DEM during ECU power down phase
 *                        vissa      ESCAN00072154 Compiler warning: Dcm does not always use the compiler abstraction for pointers
 *                        vissa      ESCAN00072524 SID 0x86: No RoE responses on multi protocol configurations with enabled declined second request feature
 *                        vissa      ESCAN00072530 SID 0x86: Compiler optimized session specific context switch
 *  03.00.00  2014-10-30  vissa      ESCAN00078136 Diagnostic client cannot communicate with the ECU
 *  05.01.00  2016-02-28  vissat     ESCAN00087636 FEAT-1472: Implement Service 0x86 (RoE)
 *  07.00.00  2016-05-02  vissat     ESCAN00090614 Service 0x86: No RoE message is sent
 *  09.04.00  2018-01-23  visahe     STORYC-3192   DCM: RoE - Release
 *  11.05.00  2019-01-18  visahe     STORY-6107    Parallel OBD/UDS protocol processing - Multi thread handling
 *  15.05.00  2020-11-23  visahe     DCM-2594      Service 0x86: Support new start/stop behavior according to ISO14229-1:2020
 *                        visvkr     DCM-3638      Service 0x86: Support new start/stop behavior according to ISO14229-1:2020 - Part II
 *  15.07.00  2021-01-21  visvkr     DCM-3696      Service 0x86: Adapt sub-function 0x03 to ISO 14229-1 (2020)
 *                        visvkr     DCM-3698      Service 0x86: Adapt sub-function 0x01 to ISO 14229-1 (2020)
 *  15.08.00  2021-02-15  visvkr     DCM-2597      Service 0x86: Support multiple events for sub-function 0x03
 *  16.04.00  2021-07-05  visvkr     DCM-4560      Service 0x86: Support STRT 0x19 for sub-function 0x01
 *                        visvkr     ESCAN00109790 Compiler error: Parse error near 'Dcm_NegativeResponseCodeType'
 *  16.05.00  2021-08-02  visvkr     DCM-4562      Service 0x86: Support STRT 0x22 with any DID for sub-function 0x03
 *  17.01.00  2021-09-27  visvkr     DCM-5011      Service 0x86: Update to revised version of ISO 14229-1 (2020)
 *  17.04.00  2021-12-20  visahe     DCM-2601      Service 0x86: Support sub-function 0x07
 *                        visvkr     DCM-4564      Service 0x86: Support NvM storage
 *  19.05.00  2023-02-09  dng        DCM-6714      Service 0x86: Support sub-function 0x09
 *                        dng        DCM-6772      Service 0x86 0x09: Support queue and intermediate response
 *                        dng        ESCAN00113995 Service 0x86: Endless loop or no response when triggering serviceToRespondTo
 *  20.00.00  2023-03-13  dng        DCM-4174      SafeBSW for DCM - Unit Service 0x86
 *  21.03.00  2023-11-30  visepp     ESCAN00116063 Compiler error: 'Dcm_Svc86NvMData': undeclared identifier
 *                        mosman     DCM-7919      SafeBSW for DCM - Unit Service 0x86 Scheduler OEM Vector
 *********************************************************************************************************************/
#if !defined(DCM_SVC86_H)
# define DCM_SVC86_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc86Types.h"
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
#  include "Dcm.h"
#  include "Dcm_Diag.h"
#  if ((DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON))
#   include "Dcm_DidMgr.h"
#  endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#  if (DCM_EXTENSION_ID != 0x96ACD543u)
#   error "Mismatching OEMs between static and generated code"
#  endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#  define Dcm_Svc86TxEnd()                                           /* not used */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Service 0x86 temporary processing context */
struct DCM_SVC86REPEATERPROXYCONTEXTTYPE_TAG
{
  Dcm_ExtSvc86NvMDataType         SessionContext;   /*!< Temporary session context */
#  if ((DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON))
  Dcm_DidMgrDidInfoContextType    DidInfoContext;   /*!< DID information context */
  Dcm_DidMgrDidOpTypeContextType  DidOpTypeContext; /*!< Did configuration operation context */
#  endif
  Dcm_DiagSubServiceRefMemType    SubSvcRef;        /*!< Index of the requested subfunction */
  boolean                         StoreState;       /*!< Whether the storageState Bit 6 was set */
};
typedef struct DCM_SVC86REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc86RepeaterProxyContextType;

typedef P2VAR(Dcm_Svc86RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT)  Dcm_Svc86RepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc86RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT)  Dcm_Svc86RepeaterProxyContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  define DCM_START_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc86Context()
 *********************************************************************************************************************/
/*! \brief          Returns the thread specific repeater context of service 0x86.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc86RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc86Context(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
*  Dcm_Svc86OnChangeSession()
*********************************************************************************************************************/
/*! \brief          Service 0x86 utility function to notify RoE about a session change.
 *  \details        -
 *  \param[in]      oldSession    The old session
 *  \param[in]      newSession    The new session
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86OnChangeSession(
  Dcm_CfgStateGroupOptType oldSession,
  Dcm_CfgStateGroupOptType newSession
  );
#  define DCM_STOP_SEC_CODE
#  include "Dcm_MemMap.h"                                                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC86_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc86.h
 *********************************************************************************************************************/
