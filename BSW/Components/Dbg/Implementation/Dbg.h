/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 * 
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Dbg.h
 *      Project:  Debugging
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Implements partly the functionality of the AUTOSAR 4.x module DBG based on XCP.
 *
 *
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id       Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2013-10-04  visore  ESCAN00050129   Creation
 *  01.00.01  2015-12-16  viszda  ESCAN00087182   Fix violation of MISRA rule 16.10
 *                        viszda  ESCAN00087207   Wrong module id of DBG
 *  01.00.02  2019-09-16  viszda  ESCAN00095153   XCP events created by the Dbg module cannot be deleted if the corresponding Dbg configuration is removed
 *  02.00.00  2021-03-02  viszda  SWAT-32         Fix of review findings
 *            2021-03-02  viszda  SWAT-1372       Support new data structure of CANoe
 *            2021-03-09  viszda  ESCAN00092556   Compiler error: The include file 'Det.h' cannot be opened
 *********************************************************************************************************************/

#if !defined(DBG_H)
# define DBG_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Dbg_Cfg.h"

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define MONITORING_ASRDBG_VERSION              (0x0200u)
# define MONITORING_ASRDBG_RELEASE_VERSION      (0x00u)

/* vendor and module identification */
# define DBG_VENDOR_ID                          (30u)
# define DBG_MODULE_ID                          (57u) /* ESCAN00087207 */

/* Component Version Information */
# define DBG_SW_MAJOR_VERSION                   (MONITORING_ASRDBG_VERSION >> 8u)
# define DBG_SW_MINOR_VERSION                   (MONITORING_ASRDBG_VERSION & 0x00FFu)
# define DBG_SW_PATCH_VERSION                   (MONITORING_ASRDBG_RELEASE_VERSION)

/* Development Error Detection */
# define DBG_E_NO_ERROR                         ((uint8)0x00u)
# define DBG_E_INVALID_DID                      ((uint8)0x01u) /* [SWS_Dbg_00228] */
# define DBG_E_PARAM_POINTER                    ((uint8)0x02u) /* [SWS_Dbg_00234]  */

/* API ID */
# define DBG_API_ID_INIT                        ((uint8)0x01u) /* [SWS_Dbg_00138] */
# define DBG_API_ID_GETVERSIONINFO              ((uint8)0x03u) /* [SWS_Dbg_00139] */
# define DBG_API_ID_ENABLEDIDCOLLECTION         ((uint8)0x14u) /* [SWS_Dbg_00152] */
# define DBG_API_ID_USELOCALTIMESTAMPACTIVATION ((uint8)0x16u) /* [SWS_Dbg_00154] */
# define DBG_API_ID_PERIODICSAMPLINGFUNC        ((uint8)0x1Eu) /* [SWS_Dbg_00124] */
# define DBG_API_ID_DEINIT                      ((uint8)0x24u) /* [SWS_Dbg_00220] */

/* Keyword macros */
#if !defined (STATIC)                               /* COV_DBG_MSR_COMPATIBILITY */
# define STATIC static
#endif

#if !defined (LOCAL_INLINE)                         /* COV_DBG_MSR_COMPATIBILITY */
# define LOCAL_INLINE INLINE STATIC
#endif

#if !defined (DBG_LOCAL)                            /* COV_DBG_MSR_COMPATIBILITY */
# define DBG_LOCAL STATIC
#endif

#if !defined (DBG_LOCAL_INLINE)                     /* COV_DBG_MSR_COMPATIBILITY */
# define DBG_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

#if !defined (DBG_DUMMY_STATEMENT)                  /* COV_DBG_MSR_COMPATIBILITY */
# if defined (DBG_USE_DUMMY_STATEMENT)              /* COV_DBG_MSR_COMPATIBILITY */
#  define DBG_DUMMY_STATEMENT(statement)            (void)(statement) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define DBG_DUMMY_STATEMENT(statement)
# endif
#endif

#if !defined (DBG_DUMMY_STATEMENT_CONST)            /* COV_DBG_MSR_COMPATIBILITY */
# if defined (DBG_USE_DUMMY_STATEMENT_CONST)        /* COV_DBG_MSR_COMPATIBILITY */
#  define DBG_DUMMY_STATEMENT_CONST(statement)      (void)(statement) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define DBG_DUMMY_STATEMENT_CONST(statement)
# endif
#endif

#if (DBG_DEV_ERROR_REPORT == STD_ON)
# if !defined Dbg_DetReportError
#  define Dbg_DetReportError(api, err)              (void)Det_ReportError(DBG_MODULE_ID, 0u, (api), (err)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
#else
# if !defined Dbg_DetReportError
# define Dbg_DetReportError(api, err)
# endif
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define DBG_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * Dbg_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the Dbg module.
 *  \details     This function initializes the module Dbg. It initializes all variables and sets the module state to
 *               initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \ingroup     general
 *********************************************************************************************************************/
extern FUNC(void, DBG_CODE) Dbg_Init(void);

/**********************************************************************************************************************
 * Dbg_DeInit()
 *********************************************************************************************************************/
/*! \brief       Deinitializes the DBG module.
 *  \details     The deinitialization function shall disable the collection of further debugging data, cancel the 
 *               alarm for cyclic data collection, stop passing data to communication part of the debugging module,
 *               and call Dbg_ComDeInit to stop all communication with the host.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \pre         Interrupts are disabled.
 *  \pre         Module is uninitialized.
 *  \ingroup     general
 *********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_DeInit(void);

# if (DBG_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 * Dbg_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  Versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \pre         Available only if DBG_VERSION_INFO_API == STD_ON.
 *  \ingroup     general
 *********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DBG_APPL_VAR) VersionInfo);
# endif

/**********************************************************************************************************************
 * Dbg_EnableDidCollection
 *********************************************************************************************************************/
/*! \brief       Dis-/enables DID collection.
 *  \details     Acceptance of data can be set to TRUE/FALSE in general. If set to FALSE, all data which is passed to the
 *               debugging core module is discarded. The information if DID collection is set to TRUE/FALSE is part of the
 *               status of the debugging core module.
 *               DID collection TRUE/FALSE does not change the individual DID activation TRUE/FALSE setting. If DID
 *               collection is set to FALSE and then to TRUE again, the old individual settings are in place.
 *  \param[in]   DidCollectionStatus     Dis-/enables the DID collection.
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     general
 *********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_EnableDidCollection(boolean DidCollectionStatus);

/**********************************************************************************************************************
 * Dbg_PeriodicSamplingFunction
 *********************************************************************************************************************/
/*! \fn          void Dbg_PeriodicSamplingFunction(void)
 *  \brief       Cyclically executed API service of the Dbg.
 *  \details     Checks if debugging data is active. If yes, gathers the current debug data and triggers transmission.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \pre         -
 *  \ingroup     general
 *********************************************************************************************************************/
FUNC(void, DBG_CODE) Dbg_PeriodicSamplingFunction(void);

#define DBG_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif

/**********************************************************************************************************************
 *  END OF FILE: Dbg.h
 *********************************************************************************************************************/
