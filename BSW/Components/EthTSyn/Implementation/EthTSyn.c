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
/*!        \file  EthTSyn.c
 *        \brief  EthTSyn main source file
 *      \details  Implementation of the Init, Scheduling and VersionInfo units.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/


#define ETHTSYN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn.h"
#include "EthTSyn_Init_Int.h"
#include "SchM_EthTSyn.h"
#include "EthTSyn_GeneratedCfgAccess.h"
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
#include "EthTSyn_PortSyncSendSm_Int.h"
#include "EthTSyn_SyncSendSm_Int.h"
#include "EthTSyn_SyncSendTx_Int.h"
#include "EthTSyn_AnnounceSendSm_Int.h"

#include "EthTSyn_SyncRcvSm_Int.h"
#include "EthTSyn_AnnounceRcvSm_Int.h"
#include "EthTSyn_SlaveRx_Int.h"

#include "EthTSyn_PdReqSm_Int.h"
#include "EthTSyn_PdReqTrcv_Int.h"
#include "EthTSyn_PdRespSm_Int.h"
#include "EthTSyn_SiteSyncSyncSm_Int.h"
#include "EthTSyn_AsymSiteSyncSyncSm_Int.h"
#include "EthTSyn_SiteSyncSyncTx_Int.h"
#include "EthTSyn_SwtMgmtQ_Int.h"
#include "EthTSyn_SwtMgmtTx_Int.h"
#include "EthTSyn_SwtMgmtRx_Int.h"
#include "EthTSyn_SwtTimeSyncSm_Int.h"
#include "EthTSyn_RateMeas_Int.h"
#include "EthTSyn_ComCtrl_Int.h"
#include "EthTSyn_TxConf_Int.h"
#include "EthTSyn_PortParamStats_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
#if (  (ETHTSYN_SW_MAJOR_VERSION != (17u)) \
    || (ETHTSYN_SW_MINOR_VERSION != (0u)) \
    || (ETHTSYN_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of EthTSyn.c and EthTSyn.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (ETHTSYN_CFG_MAJOR_VERSION != (17u)) \
    || (ETHTSYN_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of EthTSyn.c and EthTSyn_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
/* ----- Local data of Init --------------------------------------------- */
#define ETHTSYN_START_SEC_VAR_CLEARED_8
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

ETHTSYN_LOCAL VAR(EthTSyn_Init_StateEnumType, ETHTSYN_VAR_ZERO_INIT) EthTSyn_Init_State = ETHTSYN_INIT_STATE_UNINIT;

#define ETHTSYN_STOP_SEC_VAR_CLEARED_8
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Internal function prototypes of Init
 *!
 * \unit Init
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_INIT)
/**********************************************************************************************************************
 *  EthTSyn_Init_Initialize
 *********************************************************************************************************************/
/*! \brief        Initializes the EthTSyn module.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_Initialize(void);

# if (ETHTSYN_MASTER_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializeMaster
 *********************************************************************************************************************/
/*! \brief        Initializes all units of the logical unit Master.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializeMaster(void);
# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */

# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializeSlave
 *********************************************************************************************************************/
/*! \brief        Initializes all units of the logical unit Slave.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializeSlave(void);
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializePdelayInitiator
 *********************************************************************************************************************/
/*! \brief        Initializes all units of the logical unit PdelayInitiator.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializePdelayInitiator(void);
# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializeSwtMgmt
 *********************************************************************************************************************/
/*! \brief        Initializes all units of the logical unit SwitchManagement.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializeSwtMgmt(void);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

# if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializeSwtTimeSync
 *********************************************************************************************************************/
/*! \brief        Initializes all units of the logical unit SwitchTimeSynchronization.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializeSwtTimeSync(void);
# endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */
#endif /* ETHTSYN_NOUNIT_INIT */

/**********************************************************************************************************************
 * Internal function prototypes of Scheduling
 *!
 * \unit Scheduling
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_SCHEDULING)
# if (ETHTSYN_MASTER_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicMasterFuncs
 *********************************************************************************************************************/
/*! \brief        Triggers cyclic functions of all units of the logical unit Master.
 *  \details      -
 *  \pre          Has to be called in context of the EthTSyn_MainFunction() and with the same cycle.
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicMasterFuncs(void);
# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */

# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicSlaveFuncs
 *********************************************************************************************************************/
/*! \brief        Triggers cyclic functions of all units of the logical unit Slave.
 *  \details      -
 *  \pre          Has to be called in context of the EthTSyn_MainFunction() and with the same cycle.
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicSlaveFuncs(void);
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicPdelayInitiatorFuncs
 *********************************************************************************************************************/
/*! \brief        Triggers cyclic functions of all units of the logical unit PdelayInitiator.
 *  \details      -
 *  \pre          Has to be called in context of the EthTSyn_MainFunction() and with the same cycle.
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicPdelayInitiatorFuncs(void);
# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */

# if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicPdelayResponderFuncs
 *********************************************************************************************************************/
/*! \brief        Triggers cyclic functions of all units of the logical unit PdelayResponder.
 *  \details      -
 *  \pre          Has to be called in context of the EthTSyn_MainFunction() and with the same cycle.
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicPdelayResponderFuncs(void);
# endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */

# if ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) )
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicSiteSyncSyncFuncs
 *********************************************************************************************************************/
/*! \brief        Triggers cyclic functions of all units of the logical unit SiteSyncSync.
 *  \details      -
 *  \pre          Has to be called in context of the EthTSyn_MainFunction() and with the same cycle.
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicSiteSyncSyncFuncs(void);
# endif /* ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) */

# if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicSwtTimeSyncFuncs
 *********************************************************************************************************************/
/*! \brief        Triggers cyclic functions of all units of the logical unit SwitchTimeSynchronization.
 *  \details      -
 *  \pre          Has to be called in context of the EthTSyn_MainFunction() and with the same cycle.
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicSwtTimeSyncFuncs(void);
# endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */

#endif /* ETHTSYN_NOUNIT_SCHEDULING */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Internal functions of Init
 *!
 * \unit Init
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_INIT)
/**********************************************************************************************************************
 *  EthTSyn_Init_Initialize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_EthTSyn_6050_DispatchingFunction */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_Initialize(void)
{
  /* ----- Implementation ----------------------------------------------- */
# if (ETHTSYN_MASTER_SUPPORT == STD_ON)
  EthTSyn_Init_InitializeMaster();
# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */

# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
  EthTSyn_Init_InitializeSlave();
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
  EthTSyn_Init_InitializePdelayInitiator();
# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */

# if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON)
  EthTSyn_PdRespSm_Init();
# endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  EthTSyn_Init_InitializeSwtMgmt();
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT === STD_ON) */

# if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON)
  EthTSyn_Init_InitializeSwtTimeSync();
# endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */

  EthTSyn_ComCtrl_Init();
  EthTSyn_PortParamStats_Init();

# if (ETHTSYN_TX_SUPPORT == STD_ON)
  EthTSyn_TxConf_Init();
# endif /* ETHTSYN_TX_SUPPORT == STD_ON */

  EthTSyn_Init_State = ETHTSYN_INIT_STATE_INIT;
} /* EthTSyn_Init_Initialize() */

# if (ETHTSYN_MASTER_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializeMaster
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializeMaster(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SyncSendSm_Init();
  EthTSyn_PortSyncSendSm_Init();
  EthTSyn_AnnounceSendSm_Init();
} /* EthTSyn_Init_InitializeMaster() */
# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */

# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializeSlave
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializeSlave(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SyncRcvSm_Init();
  EthTSyn_AnnounceRcvSm_Init();
  EthTSyn_SlaveRx_Init();
} /* EthTSyn_Init_InitializeSlave() */
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializePdelayInitiator
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializePdelayInitiator(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_PdReqSm_Init();
  EthTSyn_PdReqTrcv_Init();
} /* EthTSyn_Init_InitializePdelayInitiator() */
# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializeSwtMgmt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializeSwtMgmt(void)
{
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  EthTSyn_SiteSyncSyncSm_Init();
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

#  if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
  EthTSyn_AsymSiteSyncSyncSm_Init();
#  endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */

#  if ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) )
  EthTSyn_SiteSyncSyncTx_Init();
#  endif /* ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) */

  EthTSyn_SwtMgmtRx_Init();
  EthTSyn_SwtMgmtTx_Init();
  EthTSyn_SwtMgmtQ_Init();
} /* EthTSyn_Init_InitializeSwtMgmt() */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

# if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Init_InitializeSwtTimeSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Init_InitializeSwtTimeSync(void)
{
  EthTSyn_SwtTimeSyncSm_Init();

  EthTSyn_RateMeas_Init();
} /* EthTSyn_Init_InitializeSwtTimeSync() */
# endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */
#endif /* ETHTSYN_NOUNIT_INIT */

/**********************************************************************************************************************
 * Internal functions of Scheduling
 *!
 * \unit Scheduling
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_SCHEDULING)
# if (ETHTSYN_MASTER_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicMasterFuncs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicMasterFuncs(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SyncSendSm_MainFunction();
  EthTSyn_AnnounceSendSm_MainFunction();
} /* EthTSyn_Scheduling_TriggerCyclicMasterFuncs() */
# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */

# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicSlaveFuncs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicSlaveFuncs(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SyncRcvSm_MainFunction();
  EthTSyn_AnnounceRcvSm_MainFunction();
} /* EthTSyn_Scheduling_TriggerCyclicSlaveFuncs() */
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicPdelayInitiatorFuncs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicPdelayInitiatorFuncs(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_PdReqSm_MainFunction();
} /* EthTSyn_Scheduling_TriggerCyclicPdelayInitiatorFuncs() */
# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */

# if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicPdelayResponderFuncs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicPdelayResponderFuncs(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_PdRespSm_MainFunction();
} /* EthTSyn_Scheduling_TriggerCyclicPdelayResponderFuncs() */
# endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */

# if ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) )
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicSiteSyncSyncFuncs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicSiteSyncSyncFuncs(void)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  EthTSyn_SiteSyncSyncSm_MainFunction();
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

  EthTSyn_SiteSyncSyncTx_MainFunction();
} /* EthTSyn_Scheduling_TriggerCyclicSiteSyncSyncFuncs() */
# endif /* ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) */


# if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Scheduling_TriggerCyclicSwtTimeSyncFuncs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Scheduling_TriggerCyclicSwtTimeSyncFuncs(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SwtTimeSyncSm_MainFunction();
} /* EthTSyn_Scheduling_TriggerCyclicSwtTimeSyncFuncs() */
# endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */

#endif /* ETHTSYN_NOUNIT_SCHEDULING */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Service functions of Init
 *!
 * \unit Init
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_INIT)
/**********************************************************************************************************************
 *  EthTSyn_InitMemory
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_Init_State = ETHTSYN_INIT_STATE_UNINIT;
}

/**********************************************************************************************************************
 *  EthTSyn_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_Init(
  P2CONST(EthTSyn_ConfigType, AUTOMATIC, ETHTSYN_INIT_DATA) CfgPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  EthTSyn_ConfigDataPtr = CfgPtr;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if (CfgPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
#  if (ETHTSYN_MASTER_SUPPORT == STD_ON)
  else if (EthTSyn_SyncSendTx_VerifyCfg(&errorId) == FALSE)
  {
    /* ErrorId already set by EthTSyn_SyncSendTx_VerifyCfg(). */
  }
#  endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */
  else
# endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #100 Trigger initialization of the module. */
    EthTSyn_Init_Initialize();
  }

  /* ----- Development Error Report ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_INIT, errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
} /* EthTSyn_Init() */

/**********************************************************************************************************************
 * Public interfaces of Init
 *!
 * \unit Init
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthTSyn_Init_IsInitialized
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_Init_IsInitialized(void)
{
  return (boolean)(EthTSyn_Init_State == ETHTSYN_INIT_STATE_INIT);
} /* EthTSyn_Init_IsInitialized() */
#endif /* ETHTSYN_NOUNIT_INIT */

/**********************************************************************************************************************
 * Service functions of VersionInfo
 *!
 * \unit VersionInfo
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_VERSIONINFO)
# if (ETHTSYN_VERSION_INFO_API == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  EthTSyn_GetVersionInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTSYN_APPL_VAR) VersionInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check VersionInfoPtr for NULL pointer. note: no uninit check is performed. */
  if (VersionInfoPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Set version info with corresponding macros from component header. */
    VersionInfoPtr->vendorID = (uint16)ETHTSYN_VENDOR_ID;
    VersionInfoPtr->moduleID = (uint16)ETHTSYN_MODULE_ID;
    VersionInfoPtr->sw_major_version = (uint8)ETHTSYN_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = (uint8)ETHTSYN_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = (uint8)ETHTSYN_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report ------------------------------------- */
#  if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_GET_VERSION_INFO, errorId);
  }
#  else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif
} /* EthTSyn_GetVersionInfo() */
# endif /* (ETHTSYN_VERSION_INFO_API == STD_ON) */
#endif /* ETHTSYN_NOUNIT_VERSIONINFO */

/**********************************************************************************************************************
 * Service functions of Scheduling
 *!
 * \unit Scheduling
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_SCHEDULING)
/**********************************************************************************************************************
 *  EthTSyn_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_EthTSyn_6050_DispatchingFunction */
FUNC(void, ETHTSYN_CODE) EthTSyn_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of component. */
  if (EthTSyn_Init_IsInitialized() == FALSE)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else
# endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Note: For correct message prioritization for debouncing, it is important that the Master and
     * SiteSyncSync units are executed before the Pdelay units (because Master and SiteSyncSync units
     * are responsible for Sync and FollowUp transmission which have a higher priority when Pdelay messages). */

    /* #20 Trigger cyclic tasks of all units of the Master logical unit. */
# if (ETHTSYN_MASTER_SUPPORT == STD_ON)
    EthTSyn_Scheduling_TriggerCyclicMasterFuncs();
# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */

    /* #30 Trigger cyclic tasks of all units of the Slave logical unit. */
# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
    EthTSyn_Scheduling_TriggerCyclicSlaveFuncs();
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

    /* #40 Trigger cyclic tasks of all units of the SiteSyncSync logical unit. */
# if ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) )
    EthTSyn_Scheduling_TriggerCyclicSiteSyncSyncFuncs();
# endif /* ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) */

    /* #50 Trigger cyclic tasks of all units of the PdelayInitiator logical unit. */
# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
    EthTSyn_Scheduling_TriggerCyclicPdelayInitiatorFuncs();
# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */

    /* #60 Trigger cyclic tasks of all units of the PdelayResponder logical unit. */
# if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON)
    EthTSyn_Scheduling_TriggerCyclicPdelayResponderFuncs();
# endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */

    /* #70 Trigger cyclic tasks of all units of the SwitchTimeSynchronization logical unit. */
# if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON)
    EthTSyn_Scheduling_TriggerCyclicSwtTimeSyncFuncs();
# endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */

    /* #80 Trigger cyclic task of the ComCtrl unit. */
    EthTSyn_ComCtrl_MainFunction();
  }

  /* ----- Development Error Report ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #11 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_MAIN_FUNCTION, errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

} /* EthTSyn_MainFunction() */

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmt_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of component. */
  if (EthTSyn_Init_IsInitialized() == FALSE)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else
#  endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Trigger cyclic task of SwtMgmtQ unit. */
    EthTSyn_SwtMgmtQ_MainFunction();
  }

  /* ----- Development Error Report ------------------------------------- */
#  if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_SWT_MGMT_MAIN_FUNCTION, errorId);
  }
#  else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
} /* EthTSyn_SwtMgmt_MainFunction() */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
#endif /* ETHTSYN_NOUNIT_SCHEDULING */



#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justifications for module-specific MISRA deviations:

  MD_EthTSyn_PtrCast_EthBuffer: MISRA rule 11.3 (A cast should not be performed between a pointer to object type and a
                                                  different pointer to object type)
                                MISRA rule 11.8 (A cast shall not be performed that removes any const or volatile
                                                 qualification from the type addressed by a pointer)
                                 QAC: 0310 - Casting to different object pointer type
                                      3305 - Pointer cast to stricter alignment.
                                      0311 - Dangerous pointer cast results in loss of const qualification
      Reason:     An Ethernet Rx/Tx buffer is provided to the EthTSyn as pointer to Eth_DataType. This data type is
                  Platform depended. Therefore for internal processing the EthTSyn uses an uint8 pointer and byte-wise
                  access to avoid mis-aligned access. However EthIf APIs expect a P2VAR with data type Eth_DataType.
                  Therefore when calling an EthIf API the Ethernet Rx/Tx buffer pointer is casted to P2VAR(Eth_DataType)
                  again.
      Risk:       No risk. Pointer was originally provided as pointer to Eth_DataType. In addition the object addressed
                  the pointer parameter is not modified within the called API. The parameter is specified as 'in'
                  parameter by AUTOSAR.
      Prevention: None.

  MD_EthTSyn_3673_EthIfCompatibility: MISRA rule 8.13 (A pointer parameter in a function prototype should be declared
                                      as pointer to const if the pointer is not used to modify the addressed object)
                                      QAC: The object addressed by the pointer parameter '%s' is not modified and so
                                      the pointer could be of type 'pointer to const'
      Reason:     The parameter is of type P2VAR to ensure compatibility with the MICROSAR EthIf module.
      Risk:       Unnoticed/unwilling modification of in-parameter.
      Prevention: Covered by code review.

  MD_EthTSyn_6050_TrivialFunctions: MISRA rule N/A
                              QAC:  Number of distinct function calls defined by HIS shall be in range 0..7
      Reason:     Number of distinct function calls is only too high due to trivial functions calls like simple
                  getters/setters, critical section handling and/or functions for byte-wise read/write access of
                  Ethernet Rx/Tx buffer.
                  Complexity of the function does not increase compared to usage of macros or direct access.
      Risk:       High complexity of functions.
      Prevention: Design and code review + clearly structured and commented code.

  MD_EthTSyn_6050_DispatchingFunction: MISRA rule N/A
                                       QAC:  Number of distinct function calls defined by HIS shall be in range 0..7
      Reason:     The functions EthTSyn_Init_Initialize() and EthTSyn_MainFunction() are dispatching functions whose
                  main purpose is to call <Unit>_Init() and <Unit>_MainFunction(), respectively, of relevant units.
                  The number of function calls is therefore high, but the complexity of the two functions is still
                  low. A further refactoring in sub-functions would rather increase complexity and decrease
                  readability.
      Risk:       High complexity of functions.
      Prevention: Design and code review + clearly structured and commented code.

  MD_EthTSyn_6080_TimestampChecking: MISRA rule N/A
                                     QAC: Maximum nesting of control structures defined by HIS shall be in range 0..4
      Reason:     The function retrieves timestamps and performs several checks, like validity of the timestamp,
                  conversion checks to other timestamp types or plausibility of calculated timestamps). Since all
                  checks are performed successively in one control path, the complexity of the function is not very
                  high and can easily be reviewed. Refactoring parts in a separate function would rather decrease
                  readability.
      Risk:       Code is difficult to maintain.
      Prevention: Design and code review + clearly structured and commented code.

  MD_EthTSyn_6080_RobustnessChecks:    MISRA rule N/A
                                       QAC: Maximum nesting of control structures defined by HIS shall be in range 0..4
      Reason:     Multiple robustness checks are implemented. Since all checks are performed successively in one
                  control path, the complexity of the function is not very high and can easily be reviewed.
                  Refactoring parts in a separate function would rather decrease readability.
      Risk:       Code is difficult to maintain.
      Prevention: Design and code review + clearly structured and commented code.

  MD_EthTSyn_3415_ReadOnly: MISRA rule 13.5 (The right hand operand of a logical && or || shall not contain side
                                             effects)
                            QAC: The right hand operand of '&&' or '||' has side effects
      Reason:     No side effect possible because the function does not modify any variable.
                  It is intended that this function is only called if the first check failed (||) / succeeded (&&).
      Risk:       No risk. No assignments are done in conditional statements
      Prevention: Covered by code review.

  MD_EthTSyn_2985_PP: MISRA rule 2.2 (There shall be no dead code)
                      QAC: This operation is redundant. The value of the result is always that of the left-hand
                      operand.
      Reason:     The operation is only redundant for certain Pre-Processor settings. To avoid complex Pre-Processor
                  conditions and increase readability of the code, the operation is always done.
      Risk:       Runtime overhead due to not required operation.
      Prevention: None.

  MD_EthTSyn_2986_PP: MISRA rule 2.2 (There shall be no dead code)
                      QAC: This operation is redundant. The value of the result is always that of the right-hand
                      operand.
      Reason:     The operation is only redundant for certain Pre-Processor settings. To avoid complex Pre-Processor
                  conditions and increase readability of the code, the operation is always done.
      Risk:       Runtime overhead due to not required operation.
      Prevention: None.

  MD_EthTSyn_2995_PP: MISRA rule 14.3 (Boolean operations whose results are invariant shall not be permitted)
                      QAC: The result of this logical operation is always 'true'.
      Reason:     This logical operation is only 'always true' for specific preprocessor variants.
      Risk:       Runtime overhead due to not required operation.
      Prevention: None.

  MD_EthTSyn_0883: MISRA rule 1.3: Include file code is not protected against repeated inclusion
      Reason:     Wrong MISRA Warning. Standard Include Guard is present.
      Risk:       No risk.
      Prevention: Covered by code review.

  MD_EthTSyn_0779_CslAccessName: MISRA rule 1.3 (Identifiers (internal and external) shall not rely on the significance
                                                of more than 31 characters)
                                 QAC: [U] Identifier does not differ from other identifier(s) (e.g. '%s') within the
                                      specified number of significant characters
      Reason:     The identifiers are generated by the ComStackLib based on the names of the underlying data.
      Risk:       A compiler might not correctly pre-process the software what might result in incorrect code.
      Prevention: Each delivery is integrated and tested on the real target system. In addition, preprocessors and
                  compilers used in current MICROSAR projects are not expected to suffer from this (historic)
                  limitation. Compiler selection and validation is done by ECU supplier.

  MD_EthTSyn_1257_Snv: MISRA rule 10.3 (The value of an expression shall not be assigned to an object with a narrower
                                        essential type or of a different essential type category.)
                       QAC: An integer constant suffixed with L or LL is being converted to a type of lower rank on
                            assignment.
      Reason:     Symbolic name value is used in a generated structure as handle. The Snv is generated with 'uL' suffix
                  but the used data type is smaller.
      Risk:       No risk. It is ensured by the ComStackLib that the used data type can hold the SNV.
      Prevention: None.

  MD_EthTSyn_UnusedStaticFunc_MemUnit: MISRA rule 1.2 (A project shall not contain unreachable code)
                                       QAC: Static function '%s()' is not used within this translation unit.
      Reason:     The Memory units provides multiple utility function for read/write access of an Ethernet buffer.
                  These small functions are implemented as 'inline' in the header file to reduce runtime overhead.
                  The same header file is included by all other units using these utility functions. However, not
                  every unit uses all utility functions.
      Risk:       Small resource overhead due to superfluous functions.
      Prevention: None.

  MD_EthTSyn_BooleanValueRangeCheck: MISRA rule 2.1 (A project shall not contain unreachable code.)
                                     MISRA rule 2.2 (There shall be no dead code.)
                                     MISRA rule 14.3 (Controlling expressions shall not be invariant.)
                                     QAC: 2992 - The value of this 'if' controlling expression is always 'false'.
                                          2996 - The result of this logical operation is always 'false'.
                                          2880 - This code is unreachable.
      Reason:     The value range size of boolean may be greater than 2. Therefore, the value range is checked for
                  invalid values. In configurations with a value range of 2 causes this check "always true/false"
                  conditions and unreachable code.
      Risk:       None.
      Prevention: None.
*/

/* Safe BSW assumptions:

  Runtime Checking:
    The EthTSyn makes use of the ComStackLib Runtime Checking feature. Due to this feature, the GenData access is
    modeled via a configuration pointer by the ComStackLib. For all subsequent defined countermeasures it is
    essential that the EthTSyn_Init function and the config pointer check were executed. The config pointer passed
    to the EthTSyn_Init is provided by the ComStackLib as well.

  These assumptions are modeled in ComStackLib
*/

/* VCA_JUSTIFICATION_BEGIN

  \ID VCA_ETHTSYN_TX_BUF_VALID
    \DESCRIPTION      Ethernet Tx-Buffer pointer is used without additional check for NULL_PTR and/or length
                      verification.
    \COUNTERMEASURE   \S The internal function EthTSyn_Tx_StartTxSequence() is used to retrieve the Tx-Buffer from
                         the lower layer. In case it returns with E_OK, the Tx-Buffer pointer is always valid
                         (i.e. pointing to a valid Tx-Buffer provided by the lower layer) and at least as big as
                         requested. Call to EthTSyn_Tx_StartTxSequence() might not be done directly from the function
                         using the Tx-Buffer. However, the same assumption holds true for other internal routines
                         returning the Ethernet Tx-Buffer.
                         SMI-233513

  \ID VCA_ETHTSYN_SWT_MGMT_RX_BUF_VALID_CTRL_IDX
    \DESCRIPTION      ComCtrl EthIf controller index stored in the SwtMgmtRxBuf is used without additional
                      verification.
    \COUNTERMEASURE   \N The stored ComCtrl EthIf controller index is always valid in case the SwtMgmtRxBuf is in state
                         PENDING. This is ensured during call of EthTSyn_SwtMgmtRx_Receive() where the controller index
                         is passed as function parameter.

  \ID VCA_ETHTSYN_SWT_MGMT_TX_TX_CONF_IDX
    \DESCRIPTION      Tx confirmation context index stored in the SwtMgmtTxDyn is used without additional verification.
    \COUNTERMEASURE   \N The stored Tx confirmation context index is always valid if EthTSyn_SwtMgmtTx_ProcTs() is
                         called for a SwtMgmtTx instance which is in state PENDING. This state is only reached if
                         EthTSyn_SwtMgmtTx_HndlTxConf() was called before for the same instance with a valid
                         Tx confirmation context index.

  \ID VCA_ETHTSYN_SWT_MGMT_Q_BUF_ITER_IDX
    \DESCRIPTION      SwtMgmtQ buffer index, which is returned by EthTSyn_SwtMgmtQ_GetIterBufIdx(), is used without
                      additional verification.
    \COUNTERMEASURE   \N The returned SwtMgmtQ buffer index is always valid for the buffer range of the SwtMgmtQ
                         instance for which EthTSyn_SwtMgmtQ_GetIterBufIdx() was called. The passed CurrProcBufIdx
                         is checked to be valid. The passed BufIter is restricted to values lower than the buffer
                         length of the SwtMgmtQ instance. If the sum of both values is higher than the buffer range,
                         the buffer length is subtracted.

  \ID VCA_ETHTSYN_ETH_SWT_MGMT_OBJ
    \DESCRIPTION      Pointer to the EthSwtMgmtObj is stored in SwtMgmtQ buffer and is later used to process required
                      information which are stored in the EthSwtMgmtObj.
    \COUNTERMEASURE   \S The EthSwtMgmtObj is retrieved via the corresponding EthIf API. When the API returns E_OK
                         it is assumed that the EthSwtMgmtObj pointer was set to a valid EthSwtMgmtObj. When the
                         SwtMgmtQ buffer is in state REQUESTED, the corresponding EthSwtMgmtObj pointer is always valid
                         and a write operation is allowed when the state of the EthSwtMgmtObj is OWNED_BY_UPPER_LAYER.
                         [SMI-585337]

  \ID VCA_ETHTSYN_CALL_EXTERNAL_FUNCTION_VAR_POINTER_ETHIF_SWT_MGMT_INFO
    \DESCRIPTION      A function with pointer parameters is directly called, but the function is not
                      defined within the analyzed sources. VCA is unable to determine the
                      behavior of the function.
    \COUNTERMEASURE   \N EthIf API must be called with a variable pointer to a switch management info object. The
                         correct data type is checked during code review. This parameter is defined as an
                         in-parameter and it is therefore assumed that no changes occur during the API call.

  \ID VCA_ETHTSYN_0_N_IND_CSL03
    \DESCRIPTION      Iteration of 0:N indirection. VCA is unable to detect validity of resulting index.
    \COUNTERMEASURE   \N The iteration index will always be valid when iterating from Start to End index. This
                         is qualified use-case CSL03 of the ComStackLib.

  \ID VCA_ETHTSYN_SYNC_FUP_CBK_BUF_LEN
    \DESCRIPTION      Transmitted Sync/FollowUp message is copied into the generated SyncSent/FollowUpSent buffer
                      without repeated check of the buffer length.
    \COUNTERMEASURE   \N As a preparation to invoke the corresponding callback, the length of the buffer is checked via
                         EthTSyn_GetMsgSentCbkBufLengthOfPortSyncSendSm(). The callback pointer is only set to a valid
                         value when the length is sufficient. This callback pointer is check for validity before
                         the message is copied. Defaults to CSL-Use-Case CSL03.

  \ID VCA_ETHTSYN_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION
    \DESCRIPTION      Compiler specific memcpy function is called and VCA is unable to recognize the specification of
                      the calling function.
    \COUNTERMEASURE   \N Specification at the calling function satisfies the requirements of memcpy.

  \ID VCA_ETHTSYN_EGR_TS_BUF_DYN_IDX_CALC_VALID
    \DESCRIPTION      Index to access the SwtMgmtQEgrTsBufDyn array is calculated and used without further check.
    \COUNTERMEASURE   \N Index is calculated by using the start index of a 1:N indirection and adding a loop variable.
                         The loop variable has a range from 0..M. Beforehand, it is ensured that M is <= N.
                         Therefore, the resulting index is always valid. Defaults to CSL-Use-Case CSL03.

  \ID VCA_ETHTSYN_MANDATORY_CHOICE_IND_CSL04
    \DESCRIPTION      Index of a mandatory indirection to multiple destinations is used without additional range check.
    \COUNTERMEASURE   \N Indirection target is checked before usage of th index. Therefore, the index is always valid
                         for the identified target. Defaults to CSL-Use-Case CSL04.

  \ID VCA_ETHTSYN_FUP_TX_BUFFER_SIZE
    \DESCRIPTION      AUTOSAR Sub-TLVs are serialized into the Ethernet Tx-Buffer without additional buffer length
                      checks.
    \COUNTERMEASURE   \N The requested Tx-Buffer length for a FollowUp message is always big enough to hold all
                         AUTOSAR Sub-TLVs which might be transmitted by EthTSyn. EthTSyn_GetSyncSendSmMaxFupTxLen()
                         is generated and used for this purpose. The validity of the value is ensured during
                         initialization (EthTSyn_SyncSendTx_VerifyCfg).

  \ID VCA_ETHTSYN_SITE_SYNC_SYNC_SM_SYNC_FUP_MSG_LEN
    \DESCRIPTION      The (Asym)SiteSyncSyncSm uses the message length from received Sync and/or FollowUp
                      to request an Ethernet transmission buffer and copy message data without further length check.
    \COUNTERMEASURE   \N The message length of the header needs to be valid in accordance to preconditions of
                         EthTSyn_SiteSyncSyncSm_ProcRcvdSync, EthTSyn_SiteSyncSyncSm_ProcRcvdFup and
                         EthTSyn_AsymSiteSyncSyncSm_ProcRcvdFup (checked by EthTSyn_SlaveRx_ProcRcvdSync and
                         EthTSyn_SlaveRx_ProcRcvdFup). This header is then stored by the (Asym)SiteSyncSyncSm for later
                         reuse. The (Asym)SiteSyncSyncSm only transmits Sync/FollowUp message when a corresponding
                         valid message header is already stored. Therefore, the message length of the header is
                         ETHTSYN_MSG_SYNC_LEN for a Sync message and at least ETHTSYN_MSG_FUP_LEN for a FollowUp
                         message.

  \ID VCA_ETHTSYN_SITE_SYNC_SYNC_UPDATE_TIME_CRC
    \DESCRIPTION      CRC of time secured TLV is updated without further validation of FollowUp message length and
                      TimeSecuredTlvRelOfs.
    \COUNTERMEASURE   \N The flag CrcUpdateRequired and value of TimeSecuredTlvRelOfs are always written consistently
                         by EthTSyn_SiteSyncSyncTx_StartFupTransmission. The offset is retrieved via
                         EthTSyn_SiteSyncSyncTlv_GetTimeTlvInfo (more specific
                         EthTSyn_SiteSyncSyncTlv_FindTimeSecuredTlv) and always valid and within the FollowUp message
                         length range when greater 0. The Ethernet Tx-Buffer for the FollowUp message is requested
                         using the received FollowUp message length as buffer length. Therefore, there is no risk
                         when updating the time secured TLV CRC.
VCA_JUSTIFICATION_END */

/*
  \CM CM_ETHTSYN_INT_FUNC_PARAM_PTR_FW It is assumed that pointers passed to internal functions are pointing to valid
                                       memory and are not NULL_PTR (unless otherwise stated in function header).
                                       The validity of the pointer has to be ensured by the caller. This also applies
                                       if the passed pointer is pointing to a struct and a specific struct member is
                                       forwarded as pointer.

  \CM CM_ETHTSYN_LOC_VAR_REF Pointer is generated by referencing a local variable or a struct member of a local variable
                             via the address operator.

  \CM CM_ETHTSYN_INT_FUNC_IDX_PARAM For internal functions it is common that only one index (e.g. PortIdx) is passed as
                                    argument. Depending on the function context and/or another parameter, specific
                                    preconditions have to be fulfilled for this index parameters. The valid range and
                                    the preconditions for the index parameter are described in the function header and
                                    have to be ensured by the caller. The usage of the index parameter then defaults
                                    to a combination of the qualified use-cases CSL01, CSL02 and CSL03 of ComStackLib.
 */

/* COV_JUSTIFICATION_BEGIN

  \ID COV_ETHTSYN_TESTSUITE_INSTRUMENTATION
    \ACCEPT TX
    \REASON This code is only deactivated by test suite for unit testing purpose.

  \ID COV_ETHTSYN_EMPTY_BRANCH
    \ACCEPT XX
    \REASON [COV_MSR_MISRA].

  \ID COV_ETHTSYN_LOOP_GUARD
    \ACCEPT TF tf xf
    \REASON The loop guard is used to ease the loop termination in case of wrong queue implementation and is therefore
            not covered.

  \ID COV_ETHTSYN_MEASBUF_ACTIVE
    \ACCEPT TX
    \REASON At least one RateMeasBuf of each RateMeas is set to active at all times, so checking for an active
            RateMeasBuf always succeeds and is implemented as sanity check.

  \ID COV_ETHTSYN_MEASBUF_INACTIVE
    \ACCEPT TX
    \REASON This part of the code is only reached if not all rate measurements are started yet. Purpose is to find one
            of the remaining inactive RateMeasBuf. This means that the iteration always breaks before the ending
            condition can fail.

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn.c
 *********************************************************************************************************************/
