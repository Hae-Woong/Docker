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
/**        \file  Dcm_Diag.c
 *         \unit  Diag
 *        \brief  Contains the implementation of Diag unit.
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
#define DCM_DIAG_SOURCE

#ifdef __PRQA__                                                                                                                                      /* COV_DCM_PRQA_UNREACHABLE XF */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetReqData" 2985 /* MD_Dcm_Redundant_2985 */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetResData" 2985 /* MD_Dcm_Redundant_2985 */
                                                                                                                                                     /* PRQA S 2991 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2992 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2995 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2996 EOF */ /* MD_Dcm_ConstExpr */
#endif
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Diag.h"
#include "Dcm_Net.h"
#include "Dcm_NetPbCfg.h"
#include "Dcm_Tsk.h"
#include "Dcm_Tmr.h"
#include "Dcm_AuthMgr.h"
#include "Dcm_CfgWrap.h"
#include "Dcm_Debug.h"
#include "Dcm_Uti.h"
#include "Dcm_Repeater.h"
#include "Dcm_State.h"
#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
# include "Dcm_RsrcMgr.h"
#endif
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
# include "Dcm_PagedBuffer.h"
#endif
#if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
# include "Dcm_Svc11.h"
#endif
#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
# include "Dcm_Mode.h"
# include "Dcm_Svc2CDefMgr.h"
# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
#  include "Dcm_Svc85.h"
# endif
#endif
#if ((DCM_DIAG_NOTIF_BSWM_APPL_UPDATED_ENABLED == STD_ON) || \
    (DCM_SVC_28_SUPPORT_ENABLED == STD_ON))
# include "BswM_Dcm.h"
#endif
#include "PduR_Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON) || \
    (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON) || \
    ((DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON) && (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)) || \
    (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
# define DCM_DIAG_CONTEXT_ENABLED                                    STD_ON
#else
# define DCM_DIAG_CONTEXT_ENABLED                                    STD_OFF
#endif

/*! Network to diagnostic variant bridge abstraction */
#if (DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON) && (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
# define Dcm_PbCfgDiagSvcId2ProtMap                                  (Dcm_DiagSingletonContext.Svc2ProtocolMap)
#else
# define Dcm_PbCfgDiagSvcId2ProtMap                                  Dcm_CfgDiagSvcId2ProtMap
#endif

#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON) && (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
# define Dcm_PbCfgDiagVariantFilter                                  (Dcm_DiagSingletonContext.RomVariant.VariantFilter)
#else
# define Dcm_PbCfgDiagVariantFilter                                  Dcm_CfgVarMgrVariantTable
#endif

/*! Diagnostic kernel application notification levels reached during diagnostic service processing.
    This levels are used to determine which finalization callouts shall be performed later (e.g. service execution confirmation, post-handlers etc.) */
#define DCM_DIAG_APPL_NOTIFICATION_NONE                              ((Dcm_DiagApplNotificationType)0x00u) /*!< No application call was made */
#define DCM_DIAG_APPL_NOTIFICATION_OEM                               ((Dcm_DiagApplNotificationType)0x01u) /*!< Manufacturer specific request indication(s) reached (Xxx_Indication()) */
#define DCM_DIAG_APPL_NOTIFICATION_SYS                               ((Dcm_DiagApplNotificationType)0x02u) /*!< System supplier specific request indication(s) reached (Xxx_Indication()) */
#define DCM_DIAG_APPL_NOTIFICATION_CONFIRMATION                      ((Dcm_DiagApplNotificationType)0x04u) /*!< Call the DCM confirmation after service processing */
#define DCM_DIAG_APPL_NOTIFICATION_POSTPROCESSOR                     ((Dcm_DiagApplNotificationType)0x08u) /*!< Call the post processor function */
#define DCM_DIAG_APPL_NOTIFICATION_FASTPOSTPROCESSOR                 ((Dcm_DiagApplNotificationType)0x10u) /*!< Call the fast post processor function */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Getter/Setter of the transport object carrying the diagnostic request currently in processing */
#define Dcm_DiagGetCurTranspObj(pDiagContext)                        (Dcm_NetPbCfgGetTransportObject(pDiagContext->TObjHdl))                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_DiagSetCurTranspObj(pDiagContext, tObjPtr)               (pDiagContext->TObjHdl = (tObjPtr)->Handle)                                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Abstract interface of getter of the transport object carrying the diagnostic request to be processed at next Dcm_DiagTaskWorker() activation */
#define Dcm_DiagGetCurQueuedTranspObj(pDiagContext)                  (Dcm_NetPbCfgGetTransportObject(pDiagContext->QueuedSet.TObjHdl))               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Abstract interface of the RCR-RP limiter counter management */
#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
/*! Reset the RCR-RP limiter */
# define Dcm_DiagResetRcrRpLimiter(pDiagContext)                     (pDiagContext->NumRcrRpTimeouts = DCM_DIAG_RCRRP_LIMIT_COUNT)                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Get current RCR-RP limiter value */
# define Dcm_DiagGetRcrRpLimiter(pDiagContext)                       (pDiagContext->NumRcrRpTimeouts)                                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Decrement by one RCR-RP limiter value */
# define Dcm_DiagDecrementRcrRpLimiter(pDiagContext)                 (--(pDiagContext->NumRcrRpTimeouts))                                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
# define Dcm_DiagResetRcrRpLimiter(pDiagContext)                     /* not used */
# define Dcm_DiagGetRcrRpLimiter(pDiagContext)                       /* not used */
# define Dcm_DiagDecrementRcrRpLimiter(pDiagContext)                 /* not used */
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/*! Control data of the unsolicited response at ECU start-up task */
struct DCM_STARTUPRESPONSECONTEXTTYPE_TAG
{
  Dcm_ProgConditionsType          ProgConditions;                             /*!< Dedicated programming condition storage in case a parallel request needs the repeater proxy pool */
  Dcm_MsgItemType                 ResBuffer[DCM_DIAG_START_UP_FBL_RES_SIZE];  /*!< Dedicated response buffer for the final response */
  boolean                         IsResponseRequired;                         /*!< Response-required information extracted from the pre-boot time context */
  Dcm_NetTransmissionResultType   TxStatus;                                   /*!< Transports the response transmission result to the Dcm_DiagTaskFblRes() */
  sint8                           SessStateRef;                               /*!< Diagnostic session index (values <0 mean no session transition). Use sint8 since no more than 127 sessions are possible */
  Dcm_NetConnRefMemType           ConnHdl;                                    /*!< Connection handle on which the unsolicited response was sent for post-processing purposes */
  Dcm_OpStatusType                OpStatus;                                   /*!< Operation status of the reset FBL flag operation */
};
typedef struct DCM_STARTUPRESPONSECONTEXTTYPE_TAG Dcm_DiagStartUpResContextType;
#endif

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/*! Control data of the specific cause code */
struct DCM_CAUSECODECONTEXTTYPE_TAG
{
  Dcm_SpecificCauseCodeType CauseCode;      /*!< The specific cause code */
  boolean                   SendCauseCode;  /*!< Flag to send the CauseCode */
};
typedef struct DCM_CAUSECODECONTEXTTYPE_TAG Dcm_DiagCauseCodeContextType;
#endif

#if (DCM_DIAG_CONTEXT_ENABLED == STD_ON)
/*! Control intanced data of the sub-component diagnostic kernel */
struct DCM_DIAGSINGLETONCONTEXTTYPE_TAG
{
# if (DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON) && (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
  P2CONST(Dcm_CfgDiagSvc2ProtMapMemType, AUTOMATIC, DCM_CONST_PBCFG) Svc2ProtocolMap; /*!< Points to the "diagnostic service to protocol map" for a particual DCM variant (criterion): Dcm_CfgDiagSvcId2ProtMap[] */
# endif
# if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON) && (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
  Dcm_CfgDiagRomVariantInfoType  RomVariant;        /*!< Relocatable ROM for the diagnostic sub-component */
# endif
# if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
  Dcm_DiagStartUpResContextType  StartUpFbl;        /*!< The global data of the unsolicited response task (Dcm_DiagTaskFblRes()) */
# endif
# if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
  Dcm_DiagCauseCodeContextType   CauseCodeContext;  /*!< Context of the specific cause code */
# endif
};
typedef struct DCM_DIAGSINGLETONCONTEXTTYPE_TAG Dcm_DiagSingletonContextType;
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/*! Recovery context containing the operation state and the recovery information */
struct DCM_DIAGRECOVERYSINGLETONCONTEXTTYPE_TAG
{
  Dcm_RecoveryInfoType RecoveryInfo;  /*!< Recovery information */
  Dcm_OpStatusType     OpStatus;      /*!< Current operation status */
};
typedef struct DCM_DIAGRECOVERYSINGLETONCONTEXTTYPE_TAG Dcm_DiagRecoverySingletonContextType;
#endif

/*! Control data for diagnostic service post-processing */
struct DCM_DIAGPOSTPROCESSORCONTEXTTYPE_TAG
{
  uint16 SrcAddr; /*!< Source address */
  uint8  Sid;     /*!< Service identifier */
  uint8  ReqType; /*!< Request type (physical/functional) */
};
typedef struct DCM_DIAGPOSTPROCESSORCONTEXTTYPE_TAG Dcm_DiagPostProcessorContextType;

/*! Diagnostic kernel synchronization data */
struct DCM_DIAGSYNCHDATATYPE_TAG
{
  Dcm_CfgNetTObjHandleMemType      TObjHdl;        /*!< A single element queue for the transport object handle that holds the request data to be processed at next Dcm_DiagTaskWorker() activation */
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
  Dcm_TskTaskEvMemType             QueuedTObjects; /*!< Bitmask of queued transport objects */
#endif
  Dcm_DiagProcessorFlagType        Flags;          /*!< The diagnostic kernel status flags (Valid values: DCM_DIAG_QUEUE_FLAG_*) */
};
typedef struct DCM_DIAGSYNCHDATATYPE_TAG Dcm_DiagSynchDataType;

/*! Control thread specific data of the sub-component diagnostic kernel */
struct DCM_DIAGTHREADCONTEXTTYPE_TAG
{
  Dcm_MsgContextType               MsgContext;            /*!< The diagnostic service/sub-service processor message context */
  volatile Dcm_DiagSynchDataType   QueuedSet;             /*!< Diagnostic kernel processing synchronization data */
  Dcm_DiagPostProcessorContextType PostProcessorContext;  /*!< Post-processing context */
  Dcm_CfgNetTObjHandleMemType      TObjHdl;               /*!< The transport object handle of the diagnostic request in processing */
  Dcm_DiagP2TimingsType            P2Timings;             /*!< Currently active set of the P2/P2* timings */
  Dcm_NetTransmissionResultType    TxStatus;              /*!< Transports the (RCR-RP and final) USDT response transmission result to the Dcm_DiagTaskWorker() */
  Dcm_DiagApplNotificationType     ApplNotification;      /*!< Application notification level reached for current diagnostic request (Valid values: DCM_DIAG_APPL_NOTIFICATION_*) */
  volatile Dcm_DiagProcessorStateType State;              /*!< Keeps track of the diagnostic kernel processing state per diagnostic request */
  Dcm_NegativeResponseCodeType     ErrorRegister;         /*!< Keeps the NRC for the current diagnostic service in processing */
  uint8                            SidIndex;              /*!< Reference to Dcm_CfgDiagServiceInfo[] entry */
  Dcm_ThreadIdMemType              ThreadId;              /*!< The Id of the thread for which the instance of the context is defined */
#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
  uint8                            NumRcrRpTimeouts;      /*!< Keeps track of the number of RCR-RPs sent for the current diagnostic service processing */
#endif
  boolean                          ProcessServiceExtern;  /*!< Diagnostic service dispatching */
  boolean                          DoSuppressResponse;    /*!< Used dedicated flag instead of "NRC 0x11 and func_req" combination with the same effect, because of possible application confirmation functions! */
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  boolean                          IsInternRequest;       /*!< Specifies whether current request in processing is internal i.e. send via Dcm_ProcessVirtualRequest() or internal Dcm_NetRxIndInternal() API (TRUE - internal, FALSE - external (normal)) */
#endif
};
typedef struct DCM_DIAGTHREADCONTEXTTYPE_TAG Dcm_DiagThreadContextType;
typedef P2VAR(Dcm_DiagThreadContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_DiagThreadContextPtrType;
typedef P2CONST(Dcm_DiagThreadContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_DiagThreadContextConstPtrType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DiagGetThreadContext()
 *********************************************************************************************************************/
 /*! \brief          Returns a diag unit thread context element of the given index.
  *  \details        -
  *  \param[in]      threadId    The active thread identifier
  *  \return         Valid diag unit thread context
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_DiagThreadContextPtrType, DCM_CODE) Dcm_DiagGetThreadContext(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetNewReqBaseToCurProgress()
 *********************************************************************************************************************/
/*! \brief          Sets the request context base to the current progress.
 *  \details        Updates all request related members to an initial value, starting with buffer that skips the already
 *                  consumed request bytes.
 *  \param[in,out]  pMsgContext   Current message context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetNewReqBaseToCurProgress(
  Dcm_MsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetQueuedTranspObj()
 *********************************************************************************************************************/
/*! \brief          Setter of the transport object carrying the diagnostic request to be processed at next.
 *                  Dcm_DiagTaskWorker() activation
 *  \details        -
 *  \param[in]      pDiagContext  Pointer to the context with all thread specific elements
 *  \param[in]      pTranspObj    Currently active DCM external/internal connection
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetQueuedTranspObj(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetNewResBaseToCurProgress()
 *********************************************************************************************************************/
/*! \brief          Sets the response context base to the current progress.
 *  \details        Updates all response related members to an initial value, starting with buffer that skips the already
 *                  provided response bytes.
 *  \param[in,out]  pMsgContext   Current message context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetNewResBaseToCurProgress(
  Dcm_MsgContextPtrType pMsgContext
  );

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagPutStartUpFblResBuffer()
 *********************************************************************************************************************/
/*! \brief          Writes into the global start up FBL response buffer.
 *  \details        -
 *  \param[in]      index         Index to the start up FBL response buffer
 *  \param[in]      value         New value of the start up FBL response buffer
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagPutStartUpFblResBuffer(
  uint8_least index,
  Dcm_MsgItemType value
  );
#endif

#if (DCM_DIAG_SYSTEM_NOTIFICATION_ENABLED == STD_ON) || (DCM_DIAG_OEM_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagExecConfirmationFunc()
 *********************************************************************************************************************/
/*! \brief          Executes a confirmation callback and evaluates its return values.
 *  \details        -
 *  \param[in]      pDiagContext          Pointer to the diag unit context
 *  \param[in]      notifList             The diagnostic request confirmation list to be executed
 *  \param[in]      notifListSize         The notifList table size
 *  \param[in]      confStatus            The post-processing status
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            The last element of the passed notifList must be marked by NULL_PTR.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagExecConfirmationFunc(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_CfgDiagNotificationInfoPtrType notifList,
  uint16_least notifListSize,
  Dcm_ConfirmationStatusType confStatus
  );
#endif
/**********************************************************************************************************************
 *  Dcm_DiagGetPostProcessResult()
 *********************************************************************************************************************/
/*! \brief          Calculates the Dcm_ConfirmationType value for the post processors.
 *  \details        -
 *  \param[in]      pDiagContext             Pointer to the diag unit context
 *  \return         The confirmation status
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_ConfirmationStatusType, DCM_CODE) Dcm_DiagGetPostProcessResult(
  Dcm_DiagThreadContextPtrType pDiagContext
  );

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoverySignature()
 *********************************************************************************************************************/
/*! \brief          Composes the recovery information control check-sum signature.
 *  \details        -
 *  \return         The control checksum
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(uint32, DCM_CODE) Dcm_DiagGetRecoverySignature(
  void
  );
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagProvideRecoveryStates()
 *********************************************************************************************************************/
/*! \brief          Takes the restored DCM state over.
 *  \details        Takes the external stored and recovered DCM state over.
 *  \param[in,out]  RecoveryInfo    Reference to structure that stores recovery info
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagProvideRecoveryStates(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo
  );
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagProcessRecoveryInfo()
 *********************************************************************************************************************/
/*! \brief          Takes the restored DCM state over.
 *  \details        Takes the external stored and recovered DCM state over.
 *  \param[in,out]  RecoveryInfo    Reference to structure that stores recovery info
 *  \param[in]      opStatus        Current operation status
 *  \context        TASK
 *  \return         DCM_E_OK        Processing succeeded
 *  \return         DCM_E_PENDING   Disable DTC API call is pending
 *  \return         DCM_E_NOT_OK    Disable DTC API call failed
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagProcessRecoveryInfo(
  P2CONST(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo,
  Dcm_OpStatusType opStatus
  );
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSafeProcessRecoveryInfo()
 *********************************************************************************************************************/
/*! \brief          Checks and takes the restored DCM state over.
 *  \details        Checks and takes the external stored and recovered DCM state over.
 *  \param[in,out]  RecoveryInfo    Reference to structure that stores recovery info
 *  \param[in]      opStatus        Current operation status
 *  \context        TASK
 *  \return         DCM_E_OK        Processing succeeded
 *  \return         DCM_E_PENDING   Disable DTC API call is pending
 *  \return         DCM_E_NOT_OK    Processing failed
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSafeProcessRecoveryInfo(
  P2CONST(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo,
  Dcm_OpStatusType opStatus
  );
#endif

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSetSpecificCauseCode()
 *********************************************************************************************************************/
/*! \brief          Registers a specific cause code.
 *  \details        Registers a specific cause code to the request in progress if no other specific cause code already
 *                  set.
 *  \param[in]      specificCauseCode    The specific cause code to be registered
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetSpecificCauseCode(
  Dcm_SpecificCauseCodeType specificCauseCode
  );
#endif

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsProcessingContextSwitchAllowed()
 *********************************************************************************************************************/
/*! \brief          Checks whether a switch of the processing context is allowed.
 *  \details        A switch is only allowed, if no kill job event is set to any thread.
 *  \param[in]      threadId         The id of the thread
 *  \return         DCM_E_OK         No kill job event in progress, switch allowed
 *  \return         DCM_E_NOT_OK     A kill job event was received for the own thread, switch not allowed
 *  \return         DCM_E_PENDING    A kill job event was received for another thread, try next cycle
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagIsProcessingContextSwitchAllowed(
  Dcm_ThreadIdMemType threadId
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcCancelOpenJobs()
 *********************************************************************************************************************/
/*! \brief          Cancel open diagnostic jobs.
 *  \details        -
 *  \param[in]      pContext            Pointer to the context
 *  \param[in]      pDiagContext        Pointer to the diag unit context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcCancelOpenJobs(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextConstPtrType pDiagContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcReviveKilledTasks()
 *********************************************************************************************************************/
/*! \brief          Revive tasks that were killed during cancellation.
 *  \details        -
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcReviveKilledTasks(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcessing()
 *********************************************************************************************************************/
/*! \brief          Performs diagnostic job cancellation.
 *  \details        If a diagnostic job is ongoing the appropriate action will be taken to close it deterministically.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcessing(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoPostProcessing()
 *********************************************************************************************************************/
/*! \brief          Performs post processing of currently finished request.
 *  \details        -
 *  \param[in]      pContext            Pointer to the context
 *  \param[in]      pDiagContext        Pointer to the diag unit context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoPostProcessing(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetDefSessionExtern()
 *********************************************************************************************************************/
/*! \brief          Performs session transition to the default session on external request.
 *  \details        Synchronizes an external session change request with the internal DCM state. If a diagnostic job is
 *                  still in processing, the session change request will be postponed till the job is finished. This is
 *                  required in order to guarantee clean sequence flow on session change and active diagnostic jobs.
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetDefSessionExtern(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagSwitchProcessingContext()
 *********************************************************************************************************************/
/*! \brief          Performs a context switch of the current working serivice processing set.
 *  \details        -
 *  \param[in]      pDiagContext            Pointer to the diag unit context
 *  \context        TASK
 *  \return         DCM_E_OK                Initialization succeeded, proceed with next step
 *  \return         DCM_E_NOT_OK            Initialization failed, repeat again next time
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSwitchProcessingContext(
  Dcm_DiagThreadContextPtrType pDiagContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagInitMsgContextBufferInfo()
 *********************************************************************************************************************/
/*! \brief          Initializes the given message context.
 *  \details        Initializes the given message context so that it points always to a valid buffer.
 *  \param[in,out]  pMsgContext   Current message context
 *  \param[in]      index         Unique handle to a buffer information element
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitMsgContextBufferInfo(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgNetBufferRefOptType index
  );

/**********************************************************************************************************************
 *  Dcm_DiagInitiateServiceProcessing()
 *********************************************************************************************************************/
/*! \brief          Initiates diagnostic kernel for new request processing.
 *  \details        -
 *  \param[in]      pDiagContext        Pointer to the diag unit context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *  \note           The parameter pMsgContext is passed to this function for initialization.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitiateServiceProcessing(
  Dcm_DiagThreadContextPtrType pDiagContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagHandleSuppressBit()
 *********************************************************************************************************************/
/*! \brief          Handles the SPRMIB if set and supported by the requested service.
 *  \details        -
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in]      pTranspObj     The transport object
 *  \param[in,out]  pMsgContext    Current request context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagHandleSuppressBit(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_MsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagHandleDispatching()
 *********************************************************************************************************************/
/*! \brief          Handles the service dispatching.
 *  \details        -
 *  \param[in]      sid            The SID of the request
 *  \param[in,out]  pMsgContext    Current request context
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagHandleDispatching(
  uint8 sid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DiagThreadContextPtrType pDiagContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagInitPostProcesserContext()
 *********************************************************************************************************************/
/*! \brief          Initializes the post processor context.
 *  \details        -
 *  \param[in,out]  pDiagContext            The diag unit context
 *  \param[in]      sid                     The requested SID
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitPostProcesserContext(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_MsgItemType sid
  );

/**********************************************************************************************************************
 *  Dcm_DiagValidateAndDispatchService()
 *********************************************************************************************************************/
/*! \brief          Validates received SID and dispatches further processing.
 *  \details        -
 *  \param[in,out]  pDiagContext            The diag unit context
 *  \param[out]     ErrorCode               The error code to be returned
 *  \return         DCM_E_OK                Validation succeeded. Initiate recognized service processing
 *  \return         DCM_E_NOT_OK            Validation failed. Finish service processing with the ErrorCode result
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Dcm_DiagInitiateServiceProcessing() shall be called first.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagValidateAndDispatchService(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerProcessNewRequest()
 *********************************************************************************************************************/
/*! \brief          Initiates diagnostic service processing once a new request is received.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerProcessNewRequest(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagWorkerProcessRcrRpTxEnd()
 *********************************************************************************************************************/
/*! \brief          Prepares for application notification after enforced RCR-RP transmission.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerProcessRcrRpTxEnd(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON) && \
    (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerCancelOperation()
 *********************************************************************************************************************/
/*! \brief          Cancels any ongoing diagnostic operation.
 *  \details        -
 *  \param[in]      pContext       Pointer to the diag unit context
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerCancelOperation(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetSecurityBypass()
 *********************************************************************************************************************/
/*! \brief          Performs security level bypass.
 *  \details        Enables or disables a security bypass mode. If a diagnostic job is still in processing,
 *                  the security bypass mode will be postponed until the job is finished. This is required
 *                  in order to prevent security state inconsistencies.
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetSecurityBypass(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_STATE_AUTHENTICATION_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetAuthenticationBypass()
 *********************************************************************************************************************/
/*! \brief          Performs authentication pre-conditions check bypass for service 0x29.
 *  \details        Enables or disables an authentication bypass mode. If a diagnostic job is still in processing,
 *                  the authentication bypass mode will be postponed until the job is finished. This is required
 *                  in order to prevent authentication pre-conditions check result inconsistencies.
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetAuthenticationBypass(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
);
#endif

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoRepeat()
 *********************************************************************************************************************/
/*! \brief          Triggers a scheduled diagnostic job activity.
 *  \details        Each registered diagnostic job activity will be called out from this point.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoRepeat(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextConstPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoGetProgCond()
 *********************************************************************************************************************/
/*! \brief          Gets the programming conditions from FBL/Application.
 *  \details        Checks and takes the external stored and recovered DCM state over.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoGetProgCond(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoWaitTxComm()
 *********************************************************************************************************************/
/*! \brief          Waits until the ComM has processed the DCM communication request.
 *  \details        Waits for either P2Star timeout of ComM acknowledge of channel wake up.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoWaitTxComm(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoResetFlags()
 *********************************************************************************************************************/
/*! \brief          Resets all FBL flags.
 *  \details        Resets all FBL related flags so at next power-on/reset no final response to be sent.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoResetFlags(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoProcessTxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Performs a session transition after a final positive response.
 *  \details        If the final response was for SID 0x10, the corresponding session transition is performed here.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoProcessTxConfirmation(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagTxPostponeFinalPagedResponse()
 *********************************************************************************************************************/
/*! \brief          Postpones the diagnostic service final response transmission if paged-buffer used.
 *  \details        The postponing is required only if the SPRMIB was set. Since the SPMRIB=TRUE means
 *                  "execute job, but do not send a response", in the case of paged-buffer the response is sent during
 *                  the "job execution". So if the P2 time is exceeded while performing the job, the final response may
 *                  not be able to provide all of the response data (the previous data pages are already lost)
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxPostponeFinalPagedResponse(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagTxStartFinalResponse()
 *********************************************************************************************************************/
/*! \brief          Initiates diagnostic service final response transmission.
 *  \details        -
 *  \param[in]      pDiagContext   Pointer to the diag unit context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxStartFinalResponse(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagTxStartRcrRpResponse()
 *********************************************************************************************************************/
/*! \brief          Initiates diagnostic service RCR-RP response transmission.
 *  \details        -
 *  \param[in]      pDiagContext        Pointer to the diag unit context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxStartRcrRpResponse(
  Dcm_DiagThreadContextPtrType pDiagContext
  );

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoveryInfoComMChannelState()
*********************************************************************************************************************/
/*! \brief          Returns a pointer to a ComM channel state of the recovery info.
 *  \details        -
 *  \param[in]      RecoveryInfo    Reference to structure that stores recovery info
 *  \param[in]      index           Index to a ComM channel state
 *  \return         A valid pointer to a ComM channel state
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
*********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_RecoveryInfoComMChannelStatePtrType, DCM_CODE) Dcm_DiagGetRecoveryInfoComMChannelState(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo,
  Dcm_CfgNetNetIdRefOptType index
  );
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON && DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoveryInfoCommControlState()
*********************************************************************************************************************/
/*! \brief          Returns a pointer to the communication state of a ComM channel of the recovery info.
 *  \details        -
 *  \param[in]      RecoveryInfo    Reference to structure that stores recovery info
 *  \param[in]      index           Index to the communication state of a ComM channel
 *  \return         A valid pointer to the communication state of a ComM channel
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
*********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_RecoveryInfoCommControlStatePtrType, DCM_CODE) Dcm_DiagGetRecoveryInfoCommControlState(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo,
  Dcm_CfgNetNetIdRefOptType index
  );
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DiagRcrRpTxFinished()
 *********************************************************************************************************************/
/*! \brief          Performs post processing of currently finished RCR-RP transmission.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM TP connection
 *  \param[in]      txStatus      Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagRcrRpTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType txStatus
  );

/**********************************************************************************************************************
 *  Dcm_DiagLookUpFilter()
 *********************************************************************************************************************/
/*! \brief          Performs a lookup result filtering.
 *  \details        -
 *  \param[in,out]  pMsgContext          The current request context
 *  \param[in]      lookUpTable          Pointer to the table to be scanned (first element is table-size!)
 *  \param[in]      lookUpFilterTable    List of filters
 *  \param[in]      value                Value to be found
 *  \param[in]      chkLevel             Hierarchy level on which the lookup is performed
 *  \param[out]     errorCode            The error code to be returned
 *  \return         -1                   Diagnostic entity not active
 *  \return         >=0                  Success, index of the matched position
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(sint16_least, DCM_CODE) Dcm_DiagLookUpFilter(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  uint8 value,
  Dcm_DiagCheckLvlType chkLevel,
  Dcm_NegativeResponseCodePtrType errorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagSvcWithSubFuncStateCheck()
 *********************************************************************************************************************/
/*! \brief          Performs a state rule check for a service with sub-function.
 *  \details        -
 *  \param[in,out]  pMsgContext               Current request context
 *  \param[in]      stateRef                  Reference to the state precondition table
 *  \param[in]      subFuncSequenceChecker    Function for accessing a service specific sequence check
 *  \param[in]      subSvcRef                 The sub-function zero-based index
 *  \param[out]     ErrorCode                 The NRC
 *  \return         DCM_E_OK                  State rule check was successful
 *  \return         DCM_E_NOT_OK              State rule check was not successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSvcWithSubFuncStateCheck(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgStateRefOptType stateRef,
  Dcm_DiagSubFuncSequenceCheckerFuncType subFuncSequenceChecker,
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

#if (DCM_DIAG_SYSTEM_NOTIFICATION_ENABLED == STD_ON) || (DCM_DIAG_OEM_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagExecIndicationFunc()
 *********************************************************************************************************************/
/*! \brief          Executes an indication callback and evaluates its return values.
 *  \details        -
 *  \param[in]      pDiagContext            Pointer to the diag unit context
 *  \param[in]      sid                     The requested SID
 *  \param[in]      notificationType        The notification kind (OEM, SYS)
 *  \param[in]      notifList               The request indication list to be executed
 *  \param[in]      notifListSize           The notifList table size
 *  \param[out]     ErrorCode               The NRC
 *  \return         DCM_E_OK                Proceed with service validation after return
 *  \return         DCM_E_NOT_OK            At least one Xxx_Indication() has rejected further processing
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            The last element of the passed notifList must be marked by NULL_PTR.
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagExecIndicationFunc(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_MsgItemType sid,
  Dcm_DiagApplNotificationType notificationType,
  Dcm_CfgDiagNotificationInfoPtrType notifList,
  uint16_least notifListSize,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#endif

/**********************************************************************************************************************
 *  Dcm_DiagTxFinalization()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM TP connection
 *  \param[in]      txStatus      Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagTxFinalization(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType txStatus
  );

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagOnFblResTxFinished()
 *********************************************************************************************************************/
/*! \brief          Called by the TP once an ongoing transmission of final response for jump from Fbl is finished.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM TP connection
 *  \param[in]      txStatus      Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagOnFblResTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType txStatus
  );
#endif

#if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagOnRoeIntmRspTxFinished()
 *********************************************************************************************************************/
/*! \brief          Called by the TP once an ongoing transmission of intermediate response for Svc86 is finished.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM TP connection
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagOnRoeIntmRspTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj
  );
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
DCM_LOCAL VAR(Dcm_DiagThreadContextType, DCM_VAR_NOINIT)    Dcm_DiagThreadContext[DCM_NUM_THREADS];                                                  /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
#if (DCM_DIAG_CONTEXT_ENABLED == STD_ON)
/*! Singleton context of Diag unit */
DCM_LOCAL VAR(Dcm_DiagSingletonContextType, DCM_VAR_NOINIT) Dcm_DiagSingletonContext;
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/*! Singleton context of Recovery feature */
DCM_LOCAL VAR(Dcm_DiagRecoverySingletonContextType, DCM_VAR_NOINIT) Dcm_DiagRecoverySingletonContext;                                                /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
#endif
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Diagnostic service verification level to NRC map */
DCM_LOCAL CONST(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_DiagObjNotFoundNrcs[DCM_DIAG_NUM_CHK_LVLS] =                                             /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_E_SERVICENOTSUPPORTED     /*!< on SID level */
 ,DCM_E_REQUESTOUTOFRANGE       /*!< on Parameter (DID, RID, etc.) level */
 ,DCM_E_SUBFUNCTIONNOTSUPPORTED /*!< on SubFunction level */
};
#define DCM_STOP_SEC_CONST_8
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DiagGetThreadContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_DiagThreadContextPtrType, DCM_CODE) Dcm_DiagGetThreadContext(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext;

  if (Dcm_DebugDetectRuntimeError(threadId >= DCM_NUM_THREADS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pDiagContext = &Dcm_DiagThreadContext[0];
  }
  else
  {
    pDiagContext = &Dcm_DiagThreadContext[threadId];
  }

  return pDiagContext;
}

/**********************************************************************************************************************
 *  Dcm_DiagSetNewReqBaseToCurProgress()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetNewReqBaseToCurProgress(
  Dcm_MsgContextPtrType pMsgContext
  )
{
  if (Dcm_DebugDetectRuntimeError(pMsgContext->reqIndex >= pMsgContext->reqBufSize))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    pMsgContext->reqBufSize -= pMsgContext->reqIndex;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_UtiNextItemByPtr(pMsgContext->reqData, pMsgContext->reqIndex);                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->reqIndex = 0;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagSetQueuedTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetQueuedTranspObj(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  pDiagContext->QueuedSet.TObjHdl = pTranspObj->Handle;                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
  Dcm_UtiBitOpSet(Dcm_TskTaskEvMemType                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
                 ,pDiagContext->QueuedSet.QueuedTObjects
                 ,Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle));                                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif
}

/**********************************************************************************************************************
 *  Dcm_DiagSetNewResBaseToCurProgress()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetNewResBaseToCurProgress(
  Dcm_MsgContextPtrType pMsgContext
  )
{
  if (Dcm_DebugDetectRuntimeError(pMsgContext->resIndex >= pMsgContext->resBufSize))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    pMsgContext->resBufSize -= pMsgContext->resIndex;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->resMaxDataLen = pMsgContext->resBufSize;                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->resProtHeadLen += pMsgContext->resDataLen;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->resDataLen = 0;                                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_UtiNextItemByPtr(pMsgContext->resData, pMsgContext->resIndex);                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    pMsgContext->resIndex = 0;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagPutStartUpFblResBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagPutStartUpFblResBuffer(
  uint8_least index,
  Dcm_MsgItemType value
  )
{
  if(Dcm_DebugDetectRuntimeError(index >= DCM_DIAG_START_UP_FBL_RES_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_DiagSingletonContext.StartUpFbl.ResBuffer[index] = value;                                                                                    /* SBSW_DCM_POINTER_WRITE_START_UP_FBL_RES_BUFFER */
  }
}
#endif

#if (DCM_DIAG_SYSTEM_NOTIFICATION_ENABLED == STD_ON ) || (DCM_DIAG_OEM_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagExecConfirmationFunc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagExecConfirmationFunc(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgDiagNotificationInfoPtrType notifList,                                                                                                      /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  uint16_least notifListSize,
  Dcm_ConfirmationStatusType confStatus
  )
{
  uint16_least lIter;
  for (lIter = 0u; lIter < notifListSize; ++lIter)                                                                                                   /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
    (void)notifList[lIter].ConfFunc(pDiagContext->PostProcessorContext.Sid
                                   ,pDiagContext->PostProcessorContext.ReqType
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_450)
                                   ,Dcm_NetPbCfgGetConnId(Dcm_DiagGetCurTranspObj(pDiagContext)->ConnHdl)
                                   ,confStatus
                                   ,Dcm_NetPbCfgGetProtocolId(Dcm_NetPbCfgGetConnProtIdOfRxPduId(pDiagContext->MsgContext.rxPduId))                  /* SBSW_DCM_PARAM_PTR_FORWARD */
                                   ,pDiagContext->PostProcessorContext.SrcAddr
# else
                                   ,pDiagContext->PostProcessorContext.SrcAddr
                                   ,confStatus
# endif
                                   );                                                                                                                /* SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC */
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagGetPostProcessResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_ConfirmationStatusType, DCM_CODE) Dcm_DiagGetPostProcessResult(
  Dcm_DiagThreadContextPtrType pDiagContext                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_ConfirmationStatusType confStatus  = DCM_DIAG_RES_ANY_OK;

  if(pDiagContext->TxStatus != DCM_E_OK)                                                                                                             /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
    /* Set Tx error flag */
    confStatus |= DCM_DIAG_RES_ANY_NOT_OK;
  }

  if(pDiagContext->ErrorRegister != DCM_E_POSITIVERESPONSE)
  {
    /* Set neg response flag */
    confStatus |= DCM_DIAG_RES_NEG_ANY;
  }
  return confStatus;
}

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoverySignature()
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
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(uint32, DCM_CODE) Dcm_DiagGetRecoverySignature(
  void
  )
{
  uint32 lResult;

  lResult = Dcm_NetPbCfgGetNumComMChannels();

# if (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)
  lResult |= Dcm_UtiMake32Bit(0u, 0u, Dcm_NetPbCfgGetNumAllComMChannels(), 0u);                                                                      /* PRQA S 2985, 2986 */ /* MD_Dcm_Redundant_2985, MD_Dcm_Redundant_2986 */
# endif

  /* The number of protocols never exceeds 32. No bit mask is needed before assigning the NumProtocols to the bit field  */
  lResult |= Dcm_UtiMake32Bit(0u, Dcm_NetPbCfgGetNumProtocols(), 0u, 0u);                                                                            /* PRQA S 2985, 2986 */ /* MD_Dcm_Redundant_2985, MD_Dcm_Redundant_2986 */

  lResult |= Dcm_UtiGetBitFromIndex(uint32, 21u);

# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  lResult |= Dcm_UtiGetBitFromIndex(uint32, 22u);
# endif

# if (DCM_SVC_85_SUPPORT_ENABLED == STD_ON)
  lResult |= Dcm_UtiGetBitFromIndex(uint32, 23u);
# endif

# if (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
  lResult |= Dcm_UtiGetBitFromIndex(uint32, 24u);
# endif

# if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
  lResult |= Dcm_UtiGetBitFromIndex(uint32, 25u);
# endif

  lResult |= Dcm_UtiMake32Bit(0x08, 0u, 0u, 0u);

  lResult |= Dcm_UtiMake32Bit(0xA0, 0u, 0u, 0u);

  return lResult;
}
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagProvideRecoveryStates()
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagProvideRecoveryStates(                                                                                 /* PRQA S 6050 */ /* MD_MSR_STCAL */
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo
  )
{
  /* Retrieve ComM channel states */
  NetworkHandleType lNetHandleIter;
  NetworkHandleType lNumComMChannels = Dcm_NetPbCfgGetNumComMChannels();
  Dcm_RecoveryInfoComMChannelStatePtrType pChannelState;
  for(lNetHandleIter = 0; lNetHandleIter < lNumComMChannels; ++lNetHandleIter)
  {
    pChannelState = Dcm_DiagGetRecoveryInfoComMChannelState(RecoveryInfo, lNetHandleIter);                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    *pChannelState = (boolean)(Dcm_NetPbCfgGetComMContext(lNetHandleIter)->RegisteredNetworks != 0u);                                                /* SBSW_DCM_POINTER_WRITE_RECOVERYINFOCOMMCHANNELSTATE */ /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  }

  /* Retrieve session state */
  RecoveryInfo->SessionLevel = Dcm_UtiGetBitPosition(Dcm_StateGetSession());                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Retrieve session connection and tester */
  RecoveryInfo->SessionConnection = Dcm_NetGetSessionConnection();                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  RecoveryInfo->SessionClientSrcAddr = Dcm_NetGetSessionClientSrcAddr();                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Retrieve active protocol */
  RecoveryInfo->ActiveProtocol = Dcm_NetGetProtIdOfActiveProtocol();                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Retrieve security state */
# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
#  if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
  if(Dcm_StateIsSecurityBypassPending())
  {
    /* Store locked security */
    RecoveryInfo->SecurityLevel = 0u;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
#  endif
  {
    RecoveryInfo->SecurityLevel = Dcm_UtiGetBitPosition(Dcm_StateGetSecurity());                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }
# endif

  /* Retrieve DTC settings state */
# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
#  if (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
  RecoveryInfo->ControlDTCSettingDTCGroup = Dcm_ModeGetControlDtcSettingGroup();                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
#  endif
  RecoveryInfo->ControlDTCSettingDisabled = (boolean)(Dcm_ModeGetControlDtcSettingMode() == RTE_MODE_DcmControlDtcSetting_DISABLEDTCSETTING);        /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
# endif

  /* Retrieve communication control state */
# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
  {
    uint8_least lChannelIter;
    Dcm_RecoveryInfoCommControlStatePtrType pControlState;
    NetworkHandleType lNumAllComMChannels = Dcm_NetPbCfgGetNumAllComMChannels();
    for(lChannelIter = 0; lChannelIter < lNumAllComMChannels; ++lChannelIter)
    {
      pControlState = Dcm_DiagGetRecoveryInfoCommControlState(RecoveryInfo, lChannelIter);                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
      *pControlState = Dcm_ModeGetCommControlState(lChannelIter);                                                                                    /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_RECOVERYINFOCOMMCONTROLSTATE */
    }
  }
# endif

  RecoveryInfo->Signature = Dcm_DiagGetRecoverySignature();                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
}
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagProcessRecoveryInfo()
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
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagProcessRecoveryInfo(                                                                         /* PRQA S 6050 */ /* MD_MSR_STCAL */
  P2CONST(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo,
  Dcm_OpStatusType opStatus
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  if (opStatus == DCM_INITIAL)
  {
    /* If a non-default session was active: */
    if(RecoveryInfo->SessionLevel != 0u)
    {
      /* Activate ComM channels */
      NetworkHandleType lNetHandleIter;
      NetworkHandleType lNumComMChannels = Dcm_NetPbCfgGetNumComMChannels();
      for(lNetHandleIter = 0; lNetHandleIter < lNumComMChannels; ++lNetHandleIter)
      {
        if(RecoveryInfo->ComMChannelState[lNetHandleIter] == TRUE)
        {
          Dcm_NetComMActiveDiagnostic(lNetHandleIter);
        }
      }

      /* Recover session state */
      Dcm_StateSetSession(RecoveryInfo->SessionLevel);

      /* Recover active protocol */
      Dcm_NetSetProtIdOfActiveProtocol(RecoveryInfo->ActiveProtocol);

      /* Update P2 timings at once */
      Dcm_DiagSetP2Timings(RecoveryInfo->SessionLevel, RecoveryInfo->ActiveProtocol, Dcm_NetPbCfgGetProtocolThreadId(RecoveryInfo->ActiveProtocol));

      /* Recover session connection */
      Dcm_NetSetSessionConnection(RecoveryInfo->SessionConnection);
      Dcm_NetSetSessionClientSrcAddr(RecoveryInfo->SessionClientSrcAddr);

      Dcm_TmrStartTimer(DCM_TMR_ID_S3, DCM_DIAG_TIME_S3);
    }

# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
    /* If any security level was unlocked: */
    if(RecoveryInfo->SecurityLevel != 0u)
    {
      /* Recover security state */
      Dcm_StateSetSecurity(RecoveryInfo->SecurityLevel);
    }
# endif
  }

# if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
  /* If DTC settings were disabled: */
  if(RecoveryInfo->ControlDTCSettingDisabled == TRUE)
  {
    /* Recover DTC updating state */
    lStdResult = Dcm_Svc85DisableDtcSetting(
#  if (DCM_SVC_85_DTC_GRP_ENABLED == STD_ON)
                                            RecoveryInfo->ControlDTCSettingDTCGroup
#  else
                                            DEM_DTC_GROUP_ALL_DTCS
#  endif
                                           ,Dcm_NetPbCfgGetProtocolDemClientId(Dcm_NetGetProtIdOfActiveProtocol())                                   /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */
                                           );

  }
# endif

# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
#  if (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
  /* If no DTC settings update was necessary or it succeeded */
  if (lStdResult == DCM_E_OK)
#  endif
  /* Recover communication control states */
  {
    uint8_least lChannelIter;
    NetworkHandleType lNumAllComMChannels = Dcm_NetPbCfgGetNumAllComMChannels();
    for(lChannelIter = 0; lChannelIter < lNumAllComMChannels; ++lChannelIter)
    {
      if(RecoveryInfo->CommControlState[lChannelIter] != DCM_ENABLE_RX_TX_NORM_NM)
      {
        Dcm_ModeSwitchCommControl(lChannelIter, RecoveryInfo->CommControlState[lChannelIter]);
      }
    }
  }
# endif
  return lStdResult;
}
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSafeProcessRecoveryInfo()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSafeProcessRecoveryInfo(                                                                     /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo,
  Dcm_OpStatusType opStatus
  )
{
  uint8          lErrorId   = DCM_E_NO_ERROR;
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(RecoveryInfo->Signature != Dcm_DiagGetRecoverySignature()))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(RecoveryInfo->SessionLevel >= DCM_STATE_NUM_SESSION))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError( (RecoveryInfo->SessionLevel != 0u) /* non-default session */
                              &&(RecoveryInfo->ActiveProtocol >= DCM_NET_INVALID_PROTID) ))                                                          /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError( (RecoveryInfo->SessionLevel != 0u) /* non-default session */
                              &&(RecoveryInfo->SessionConnection >= DCM_NET_INVALID_CONNHDL) ))                                                      /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError( (RecoveryInfo->SessionLevel != 0u) /* non-default session */
                              &&(Dcm_NetPbCfgGetProtIdOfConnection(RecoveryInfo->SessionConnection) != RecoveryInfo->ActiveProtocol) ))              /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
    lErrorId = DCM_E_PARAM;
  }
  else
# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  if(Dcm_DebugDetectError(RecoveryInfo->SecurityLevel >= DCM_STATE_NUM_SECURITY))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
# endif
  {
    /* ----- Implementation ------------------------------------------------ */
    lStdResult = Dcm_DiagProcessRecoveryInfo(RecoveryInfo, opStatus);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETRECOVERYSTATES, lErrorId);

  return lStdResult;
}
#endif

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSetSpecificCauseCode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagSetSpecificCauseCode(
  Dcm_SpecificCauseCodeType specificCauseCode
  )
{
  if(Dcm_DiagSingletonContext.CauseCodeContext.SendCauseCode == FALSE)
  {
    Dcm_DiagSingletonContext.CauseCodeContext.CauseCode = specificCauseCode;
    Dcm_DiagSingletonContext.CauseCodeContext.SendCauseCode = TRUE;
  }
}
#endif

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsProcessingContextSwitchAllowed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */

DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagIsProcessingContextSwitchAllowed(
  Dcm_ThreadIdMemType threadId                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  /* No (nested) kill job received? */
  if ((DCM_TSK_EV_DIAG_WORK_KILL_JOBS & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, threadId)) != 0u)
  {
    lStdResult = DCM_E_NOT_OK; /* skip any further processing in this Dcm_DiagTaskWorker() activation (i.e. return immediately) */
  }
# if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  else /* No (nested) kill job received for any other thread? */
  {
    Dcm_ThreadIdOptType lThreadIter;

    for (lThreadIter = 0u; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
    {
      if ((DCM_TSK_EV_DIAG_WORK_KILL_JOBS & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, lThreadIter)) != 0u)
      {
        lStdResult = DCM_E_PENDING;
        break;
      }
    }
  }
# endif

  return lStdResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcCancelOpenJobs()
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
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcCancelOpenJobs(                                                                      /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextConstPtrType pDiagContext
  )
{
  /* #10 If any diagnostic job still open (paged-buffer transfers are not part of them), call the job handle one last time with DCM_CANCEL signal */
  if(pDiagContext->State == DCM_DIAG_STATE_PROCESS)
  {
    Dcm_NegativeResponseCodeType lNrc = DCM_E_PANIC_NRC; /* to avoid compiler warnings */
    Dcm_RepeaterSetOpStatus(pContext, DCM_CANCEL);/* Override any other opStatus values up to now */                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    (void)Dcm_RepeaterExecute(pContext, &lNrc);/* signal cancellation */                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

    Dcm_DiagSetNegResponse(lNrc, pContext->ThreadId); /* register negative result */
  }

  /* #20 If any diagnostic job still open (including paged-buffer transfers) or already on transmission: */
  if(((DCM_DIAG_STATE_PROCESS | DCM_DIAG_STATE_ON_TX) & pDiagContext->State) != 0u) /* any open diagnostic job? (optimize interrupt lock times) */
  {
    /* Immediately cancel any ongoing transmission (RCR-RP, final response (RoE or external)) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    if(((DCM_DIAG_STATE_PROCESS | DCM_DIAG_STATE_ON_TX) & pDiagContext->State) != 0u) /* STILL any open diagnostic job -> diagnostic transport object is still in use -> try cancel it */ /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY TX */
    {
      Dcm_NetTransportObjectPtrType pTranspObj;

      pTranspObj = Dcm_DiagGetCurTranspObj(pDiagContext);

      /* #30 If any USDT response transmission is still ongoing (incl. RCR-RPs): */
      if(pTranspObj->State == DCM_NET_TOBJ_STATE_ONTX)
      {
        pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_CANCELED;/* isolate this connection */                                                                /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        /* Short cut to speed up the service processing finalization - RCR-RP responses will just end with the cancellation below */
        Dcm_DiagTxFinalization(pTranspObj, DCM_E_NOT_OK);                                                                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

        /* Try cancel any ongoing response. The return value does not matter:
          * OK -> there will be a confirmation for this cancellation
          * N_OK -> request rejected, but there will be a confirmation due to the ongoing transmission */
        (void)PduR_DcmCancelTransmit(Dcm_NetPbCfgGetConnTxPduIdMain(pTranspObj->ConnHdl));
        /* Any response (final, RCR-RP, etc) will just end with releasing the connection assigned to the tObject! No confirmation post-processing will be performed! */
      }
      /* Otherwise, if there is no USDT response initiated yet, just notify the diagnostic kernel for the transmission finalization to accomplish the diagnostic job cycle */
      else if (pTranspObj->State == DCM_NET_TOBJ_STATE_READY)                                                                                        /* COV_DCM_RTM_UNREACHABLE TX */
      {
        /* no transmission yet started -> just loop back the diagnostic confirmation immediately. Note: no other Tx-confirmation type is expected here */
        Dcm_DiagTxFinalization(pTranspObj, DCM_E_NOT_OK);                                                                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      }
      else
      {
        /* Otherwise, unexpected transport object state reached (inconsistent in the context of diagnostic kernel state, showing a job is still in progress) */
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
      }
    }
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
  /* Otherwise, diagnostic kernel is in "post processing" or "idle" state and they are to be handled in a normal way
         Note: This case can be reached since the prioritization just fires a cancellation event, disregarding any internal states of the diagnostic kernel */
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcReviveKilledTasks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcReviveKilledTasks(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pEventContext);

  /* All cancellation activities are done now - revive all killed tasks to allow normal processing of the newly received diagnostic job (the one caused the interruption) */
  /* Look ahead: Any nested kill job received? If so, no task revival! */
  if((DCM_TSK_EV_DIAG_WORK_KILL_JOBS & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, pDiagContext->ThreadId)) == 0u)                                 /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY TX */
  {
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
    Dcm_TskTaskEvOptType queuedTObjBitSet = 0; /* nothing to clean (for now) */
#endif
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
     /* STILL no nested kill job received? */
    if((DCM_TSK_EV_DIAG_WORK_KILL_JOBS & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, pDiagContext->ThreadId)) == 0u)                               /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY TX */
    {
      Dcm_TskReviveAllTasks(pDiagContext->ThreadId);/* from now on enable tasks for working */
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
      queuedTObjBitSet = pDiagContext->QueuedSet.QueuedTObjects;

      /* Clear all except the last queued one  */
      Dcm_UtiBitOpClr(Dcm_TskTaskEvOptType, queuedTObjBitSet, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, Dcm_DiagGetCurQueuedTranspObj(pDiagContext)->Handle));

      /* Remove all processed transport objects from the queue (the last queued one will be removed from the queue once transfered to the active tObject or canceled by higher prio request here again) */
      Dcm_UtiBitOpClr(Dcm_TskTaskEvMemType, pDiagContext->QueuedSet.QueuedTObjects, queuedTObjBitSet);                                               /* SBSW_DCM_PARAM_PTR_WRITE */
#endif
    }
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
    if(queuedTObjBitSet > 0u)
    {
      /* Some connections to clean up (in background)? */
      Dcm_TskSetEvent(DCM_TSK_ID_DIAG_GARB_COL, (Dcm_TskTaskEvMemType)queuedTObjBitSet);
    }
#endif
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoCancelProcessing()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoCancelProcessing(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_DiagWorkerDoCancelProcCancelOpenJobs(pContext, pDiagContext); /* Cancel open diagnostic jobs */                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Sync with TxEnd events set above OR Tx confirmation. No ISR lock since:
   * - No Tx confirmation can occur from now on
   * - If above Tx confirmation executed -> no other changes of the state are possible!
   * - If there was already a Tx Confirmation (above code not executed) -> the task will just be re-triggered
   */
  /* If the Dcm_DiagTaskWorker() has (by finished transmission or through the above cancellation activity) already received a post-processing event: */
  if((DCM_TSK_EV_DIAG_WORK_TX_END_FINAL & Dcm_TskGetEventByThread(DCM_TSK_ID_DIAG_WORK, pDiagContext->ThreadId)) != 0u)
  {
    Dcm_TskClrEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_TX_END_FINAL, pDiagContext->ThreadId);/* clear any potentially globally set Tx-end-response (from the above Tx cancellation code) */
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_TX_END_FINAL);/* set/transfer the Tx end event */                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  /* Otherwise, either no Tx-end event OR nothing to do OR it is already set in the local "ev" -> will be post-processed in next task stages */

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON) && (DCM_MULTI_THREAD_ENABLED == STD_ON)
  /* Prevent that a pending repeater post event is executed the next main function cycle for the currently killed job */
  Dcm_TskClrLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_REPEAT);                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
#endif

  Dcm_DiagWorkerDoCancelProcReviveKilledTasks(pDiagContext, pEventContext); /* Revive killed tasks */                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoPostProcessing()
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
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoPostProcessing(                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext
  )
{
  Dcm_ConfirmationStatusType   confStatus;
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  Dcm_NegativeResponseCodeType lNrc = DCM_E_PANIC_NRC; /* to avoid compiler warnings */
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  /* Reset any pending PB processing */
  if(Dcm_PagedBufferCancel(pContext, &lNrc) == DCM_E_NOT_OK)                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  {
    Dcm_DiagSetNegResponse(lNrc, pContext->ThreadId);/* Set NRC now to affect the post-processor confirmation status */
  }
#endif

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  Dcm_RsrcMgrReleaseServiceLock(pContext->ThreadId);
#endif

  confStatus = Dcm_DiagGetPostProcessResult(pDiagContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  /* First process any diagnostic service post-handler (e.g. to perform any state transitions at this stage): */
  if((DCM_DIAG_APPL_NOTIFICATION_POSTPROCESSOR & pDiagContext->ApplNotification) != 0u)
  {
    Dcm_DiagSvcConfirmationFuncType lPostProcessorFunc;
    lPostProcessorFunc = Dcm_CfgWrapDiagGetPostProcessorFunc(Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex)->ConfFuncRef);

    lPostProcessorFunc(pContext, confStatus);/* execute any internal service processor post-handlers */                                              /* SBSW_DCM_CALL_FUNCPTR_SVCPOSTPROCESSORS */
  }

  /* Process the application notification confirmation */
  if((DCM_DIAG_APPL_NOTIFICATION_CONFIRMATION & pDiagContext->ApplNotification) != 0u)
  {
    Dcm_Confirmation(0, pDiagContext->MsgContext.rxPduId, confStatus);/* notify the application about transmission end (of any service) */
  }

#if (DCM_DIAG_SYSTEM_NOTIFICATION_ENABLED == STD_ON)
  /* At next process any system supplier and OEM notification (e.g. to perform any additional state transitions at this stage): */
  if((DCM_DIAG_APPL_NOTIFICATION_SYS & pDiagContext->ApplNotification) != 0u)
  {
    Dcm_DiagExecConfirmationFunc(pDiagContext, Dcm_CfgDiagSysNotificationInfo, DCM_CFGDIAGSYSNOTIFICATIONINFO_SIZE, confStatus);                     /* SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC */
  }
#endif

#if (DCM_DIAG_OEM_NOTIFICATION_ENABLED == STD_ON)
  if((DCM_DIAG_APPL_NOTIFICATION_OEM & pDiagContext->ApplNotification) != 0u)
  {
    Dcm_DiagExecConfirmationFunc(pDiagContext, Dcm_CfgDiagOemNotificationInfo, DCM_CFGDIAGOEMNOTIFICATIONINFO_SIZE, confStatus);                     /* SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC */
  }
#endif

#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if(pDiagContext->IsInternRequest == FALSE)/* manage ComM states only on processed requests from external clients */
#endif
  {
    /* Process here to take a potentially new session change into account */
    Dcm_NetUnRegisterComMActivity(Dcm_NetPbCfgGetConnHdlOfRxPduId(pDiagContext->MsgContext.rxPduId));                                                /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  }
  pDiagContext->State = DCM_DIAG_STATE_IDLE;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetDefSessionExtern()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetDefSessionExtern(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* If diagnostic kernel is still busy with a diagnostic job, retry next task activation if there is no other such request yet */
  if(Dcm_DiagIsKernelBusy())
  {
    if(!Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF))                                                                 /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY XX */
    {
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_EXT_SET2DEF); /* retry later again */                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    /* Redirect the external request for resetting the session to the internal request handler (same activity) */
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF);                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

    /* If non-default session is active the current protocol id does not equal to the total number of protocols */
    if(!Dcm_StateIsDefaultSessionActive())
    {
      Dcm_DiagSetP2Timings(0, Dcm_NetGetProtIdOfActiveProtocol(), pDiagContext->ThreadId);
    }/* else - P2 timings of default session already set */

    Dcm_NetSetRxAllowed(TRUE);
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagSwitchProcessingContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSwitchProcessingContext(
  Dcm_DiagThreadContextPtrType pDiagContext                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  Dcm_SplitTaskEnterCS();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
#if (DCM_SPLIT_TASKS_ENABLED == STD_OFF)
  lStdResult = DCM_E_OK;
#else
  lStdResult = Dcm_DiagIsProcessingContextSwitchAllowed(pDiagContext->ThreadId);                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
#endif
  {
    Dcm_NetTransportObjectConstPtrType pQueuedTObj;

    pQueuedTObj = Dcm_DiagGetCurQueuedTranspObj(pDiagContext);

    Dcm_UtiBitOpClr(Dcm_DiagProcessorFlagType, pDiagContext->QueuedSet.Flags, DCM_DIAG_QUEUE_FLAG_IS_WAITING);                                       /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_UtiBitOpSet(Dcm_DiagProcessorFlagType, pDiagContext->QueuedSet.Flags, DCM_DIAG_QUEUE_FLAG_IS_ACTIVE);                                        /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_DiagSetCurTranspObj(pDiagContext, pQueuedTObj); /* switch work contexts! RCR-RP will be sent also from this one (QueuedCtxt) */              /* SBSW_DCM_PARAM_PTR_WRITE */

#if (DCM_DIAG_GARB_COL_ENABLED ==STD_ON)
    Dcm_UtiBitOpClr(Dcm_TskTaskEvMemType, pDiagContext->QueuedSet.QueuedTObjects, (Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pQueuedTObj->Handle))); /* SBSW_DCM_PARAM_PTR_WRITE */
#endif
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_SplitTaskLeaveCS();

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagInitMsgContextBufferInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitMsgContextBufferInfo(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_CfgNetBufferRefOptType index
  )
{
  Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;

  lBufferInfo = Dcm_NetPbCfgGetBufferInfo(index);

  pMsgContext->reqData = &(lBufferInfo->BufferPtr[0]);                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resData = &(lBufferInfo->BufferPtr[0]);                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->reqDataLen = 0;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resDataLen = 0u;                                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->reqBufSize = lBufferInfo->Size;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resBufSize = lBufferInfo->Size;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resMaxDataLen = lBufferInfo->Size;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resProtHeadLen = 0;                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->reqIndex = 0u;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->resIndex = 0u;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_DiagInitiateServiceProcessing()
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitiateServiceProcessing(
  Dcm_DiagThreadContextPtrType pDiagContext                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetTransportObjectConstPtrType pTranspObj;
  Dcm_MsgContextPtrType              pMsgContext;

  /* Now take the active transport object */
  pTranspObj = Dcm_DiagGetCurTranspObj(pDiagContext);
  pMsgContext = &(pDiagContext->MsgContext);

  /* Compose message context (skip SID byte) */
  Dcm_DiagInitMsgContextBufferInfo(pMsgContext, pTranspObj->BufferHdl);                                                                              /* SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT */

  pMsgContext->reqDataLen = pTranspObj->RxLength;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->rxPduId = pTranspObj->RxPduId;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->msgAddInfo.reqType  = (uint8)((Dcm_NetPbCfgIsFuncRequest(pTranspObj->RxPduId) == TRUE)?1u:0u);                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->msgAddInfo.suppressPosResponse = 0;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  pMsgContext->threadId = pDiagContext->ThreadId;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Update diagnostic kernel context */
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  pDiagContext->IsInternRequest    = (boolean)((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) != 0u);                                              /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */ /* SBSW_DCM_PARAM_PTR_WRITE */
#endif
  pDiagContext->ErrorRegister      = DCM_E_POSITIVERESPONSE;                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  pDiagContext->ApplNotification   = DCM_DIAG_APPL_NOTIFICATION_NONE;                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  pDiagContext->DoSuppressResponse = FALSE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  pDiagContext->State              = DCM_DIAG_STATE_PROCESS;                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
  /* Initialize cause code context */
  Dcm_DiagSingletonContext.CauseCodeContext.CauseCode = 0x00;
  Dcm_DiagSingletonContext.CauseCodeContext.SendCauseCode = FALSE;
#endif

  /* Initialize IdsM security event */
  Dcm_DebugResetSecurityEventStatus(pDiagContext->ThreadId);
}

/**********************************************************************************************************************
 *  Dcm_DiagHandleSuppressBit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagHandleSuppressBit(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgContextPtrType pMsgContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pTranspObj);

  /* Check if the new service supports sub-function parameter */
  if((Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex)->Props & DCM_DIAG_SVC_CFG_PROP_HAS_SUBFUNC)  != 0u)
  {
    uint8 reqSf = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                                /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
    /* If so, deal with the SPRMIB: extract and store */
    if ((reqSf & 0x80u) != 0u)
    {
      Dcm_UtiSetReqDataAsU8(pMsgContext, (uint8)(reqSf & 0x7Fu));/* mask out the SPRMIB */                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

      Dcm_SplitTaskEnterCS();/* protect against hi-priority Dcm_DiagTaskRx/Tx */
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
      /* On split task it could happen that RCR-RP is already on transmission */
      if((pTranspObj->Flags & (DCM_NET_TOBJ_FLAG_RCRRP_ON_TX | DCM_NET_TOBJ_FLAG_RCRRP_SENT)) == 0u)/* RCR-RP is NOT on transmission or was sent */
#endif
      {
        pMsgContext->msgAddInfo.suppressPosResponse = 1;                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
      }
      /*=================================*
        END CRITICAL SECTION
       *=================================*/
      Dcm_SplitTaskLeaveCS();
    }
    /* copy the sub-function byte once split Rx/Tx buffers are supported.
    pMsgContext->resData[0] = pMsgContext->reqData[0];
    */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagHandleDispatching()
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagHandleDispatching(
  uint8 sid,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagThreadContextPtrType pDiagContext                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(sid);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  pDiagContext->ProcessServiceExtern = FALSE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */

#if (DCM_DIAG_SERVICE_DISPATCHER_ENABLED == STD_ON)
  /*  Check for necessity of service dispatching (service supports the dispatching parameter) */
  if((Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex)->Props & DCM_DIAG_SVC_CFG_PROP_CALL_SVC_DISPATCHER) != 0u)
  {
    boolean lResult;

    /* Execute the call-out to the application and verify the result */
    lResult = Dcm_HandleServiceExtern(sid
                                     ,pMsgContext->reqData
                                     ,(uint16)pMsgContext->reqDataLen
                                     ,(uint8) pMsgContext->msgAddInfo.reqType
                                     ,Dcm_DiagGetCurTranspObj(pDiagContext)->ClientSrcAddr);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (Dcm_DebugDetectError((lResult != TRUE) && (lResult != FALSE)))
    {
      Dcm_DebugReportError(DCM_SID_SVCDISPATCHER, DCM_E_INTERFACE_RETURN_VALUE);                                                                     /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    else
    {
      pDiagContext->ProcessServiceExtern = lResult;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  /* Set the flags for post processing on internal service handling */
  if(pDiagContext->ProcessServiceExtern == FALSE)
#endif
  {
    if(Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex)->ConfFuncRef != 0u)
    {
      pDiagContext->ApplNotification |= DCM_DIAG_APPL_NOTIFICATION_POSTPROCESSOR; /* mark application post processor */                              /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    if(Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex)->FastConfFuncRef != 0u)
    {
      pDiagContext->ApplNotification |= DCM_DIAG_APPL_NOTIFICATION_FASTPOSTPROCESSOR; /* mark application fast post processor */                     /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  /* From now on, mark application calls for post-processing later */
  pDiagContext->ApplNotification |= DCM_DIAG_APPL_NOTIFICATION_CONFIRMATION; /* mark application notification at last here */                        /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_DiagInitPostProcesserContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagInitPostProcesserContext(
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_MsgItemType sid
)
{
  pDiagContext->PostProcessorContext.Sid     = sid;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
  pDiagContext->PostProcessorContext.SrcAddr = Dcm_DiagGetCurTranspObj(pDiagContext)->ClientSrcAddr;                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
  pDiagContext->PostProcessorContext.ReqType = (uint8)pDiagContext->MsgContext.msgAddInfo.reqType;                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_DiagValidateAndDispatchService()
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
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagValidateAndDispatchService(                                                                  /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  Dcm_MsgContextPtrType pMsgContext = &pDiagContext->MsgContext;
  Dcm_MsgItemType lSid;

  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lSid);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_UtiProvideResDataAsU8(pMsgContext, (uint8)(lSid + 0x40u));                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Skip SID data, since already processed */
  Dcm_DiagSetNewReqBaseToCurProgress(pMsgContext);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_DiagSetNewResBaseToCurProgress(pMsgContext);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

  Dcm_DiagInitPostProcesserContext(pDiagContext, lSid);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

#if (DCM_DIAG_OEM_NOTIFICATION_ENABLED == STD_ON)
  /* Execute manufacturer specific environment checks */
  if(Dcm_DiagExecIndicationFunc(pDiagContext, lSid, DCM_DIAG_APPL_NOTIFICATION_OEM, Dcm_CfgDiagOemNotificationInfo, DCM_CFGDIAGOEMNOTIFICATIONINFO_SIZE, ErrorCode) == DCM_E_OK) /* SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC */
#endif
  {
    sint16_least sidIter = Dcm_DiagLookUpFilter(pMsgContext
                                               ,Dcm_CfgDiagSvcIdLookUpTable
                                               ,Dcm_CfgDiagSvcIdExecPrecondTable
                                               ,lSid
                                               ,DCM_DIAG_CHK_LVL_SERVICE_ID
                                               ,ErrorCode);                                                                                          /* SBSW_DCM_POINTER_FORWARD_GLOBAL */ /* SBSW_DCM_COMB_PTR_FORWARD */
    /* Service found? */
    if(sidIter >= 0)
    {
      pDiagContext->SidIndex = (uint8)sidIter;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

      if((Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex)->Props & DCM_DIAG_SVC_CFG_PROP_CALL_POST_HDLR_ALWAYS)  != 0u)
      {
        /* mark application notification once the SID is identified! */
        pDiagContext->ApplNotification |= DCM_DIAG_APPL_NOTIFICATION_POSTPROCESSOR;                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      }

#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
      /* Check authentication access rights */
      lStdResult = Dcm_AuthMgrCheckDispatcherLevel(lSid
                                                  ,Dcm_CfgWrapDiagSvcIdExecPrecondTable(pDiagContext->SidIndex)
                                                  ,pMsgContext
                                                  ,Dcm_NetPbCfgGetConnAuthInfoRefByRxPduId(pMsgContext->rxPduId)                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                  ,ErrorCode);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */

      if(lStdResult == DCM_E_OK)
#endif
      {
        /* Check SID pre-conditions */
        lStdResult = Dcm_StateCheck(Dcm_CfgWrapDiagSvcIdExecPrecondTable(pDiagContext->SidIndex)
                                   ,DCM_DIAG_CHK_LVL_SERVICE_ID
                                   ,ErrorCode);                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

        if(lStdResult == DCM_E_OK)
        {
#if (DCM_DIAG_SYSTEM_NOTIFICATION_ENABLED == STD_ON)
          /* Execute supplier specific environment checks */
          lStdResult = Dcm_DiagExecIndicationFunc(pDiagContext, lSid, DCM_DIAG_APPL_NOTIFICATION_SYS, Dcm_CfgDiagSysNotificationInfo, DCM_CFGDIAGSYSNOTIFICATIONINFO_SIZE, ErrorCode); /* SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC */

          if(lStdResult == DCM_E_OK)
#endif
          {
            /* Check for available minimum length */
            if(pMsgContext->reqDataLen >= Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex)->MinLength)
            {
              /* Handle SPRMIB */
              Dcm_DiagHandleSuppressBit(pDiagContext, Dcm_DiagGetCurTranspObj(pDiagContext), pMsgContext);                                           /* SBSW_DCM_COMB_PTR_FORWARD */

              /* Handle service dispatching and set the flags for post-processing */
              Dcm_DiagHandleDispatching(lSid, pMsgContext, pDiagContext);                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */
            }
            else
            {
              /* Minimum length is not reached  */
              *ErrorCode = (Dcm_NegativeResponseCodeType)( (lSid < 0x10u)
                                                          ? DCM_E_REQUESTOUTOFRANGE
                                                          : DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);                                            /* SBSW_DCM_PARAM_PTR_WRITE */
              lStdResult = DCM_E_NOT_OK;
            }
          } /* else - NRC already set - finalize processing */
        }/* else - NRC already set - finalize processing */
      }/* else - NRC already set - finalize processing */
    }/* else - already written "Unsupported service requested" */
  }/* else - NRC already set - finalize processing */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerProcessNewRequest()
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerProcessNewRequest(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType               lStdResult;
  Dcm_NegativeResponseCodeType lNrc = DCM_E_PANIC_NRC; /* to avoid compiler warnings */

  lStdResult = Dcm_DiagSwitchProcessingContext(pDiagContext);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    Dcm_DiagInitiateServiceProcessing(pDiagContext);                                                                                                 /* SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT */

    /* Validate received diagnostic service on SID level */
    lStdResult = Dcm_DiagValidateAndDispatchService(pDiagContext
                                                   ,&lNrc);                                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)
    {
      /* Execute Main-Handler (register it and it will be called immediately) */
      Dcm_RepeaterSetCallee(pContext, pDiagContext->SidIndex);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
      /* Let the service processor take over immediately */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_REPEAT);                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* DCM_E_NOT_OK */
      Dcm_DiagSetNegResponse(lNrc, pContext->ThreadId);
      Dcm_DiagProcessingDone(pDiagContext->ThreadId);
    }
  }
  else if (lStdResult == DCM_E_PENDING)
  {
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_NEW_REQ); /* Retry next time */                                                 /* PRQA S 2880 */ /* MD_MSR_Unreachable */ /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else /* DCM_E_NOT_OK */
  {
    /* skip any further processing in this Dcm_DiagTaskWorker() activation (i.e. return immediately) */
    pEventContext->Ev = 0;                                                                                                                           /* PRQA S 2880 */ /* MD_MSR_Unreachable */ /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagWorkerProcessRcrRpTxEnd()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerProcessRcrRpTxEnd(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;

  pTranspObj = Dcm_DiagGetCurTranspObj(pDiagContext);

  if((DCM_NET_TOBJ_FLAG_RCRRP_TYPE_APPL & pTranspObj->Flags) != 0u)/* this is a RCR-RP response - check which kind */
  {
    Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_TYPE_APPL);                                           /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

    if (pDiagContext->TxStatus != DCM_E_OK)
    {
      Dcm_RepeaterSetOpStatus(pContext, DCM_FORCE_RCRRP_NOT_OK);                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      Dcm_RepeaterSetOpStatus(pContext, DCM_FORCE_RCRRP_OK);                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_REPEAT); /* notify immediately */                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON) && \
    (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerCancelOperation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerCancelOperation(
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NegativeResponseCodeType lNrc;

  DCM_IGNORE_UNREF_PARAM(pEventContext);

  if(Dcm_PagedBufferCancel(pContext, &lNrc) == DCM_E_NOT_OK)                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  {
    /* Stop polling and wait for confirmation */
    Dcm_DiagSetNegResponse(lNrc, pContext->ThreadId);
    Dcm_DiagProcessingDone(pDiagContext->ThreadId);
  }
}
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetSecurityBypass()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetSecurityBypass(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pDiagContext);

  /* Enforce bypass mode if no service is in processing! */
  if(Dcm_DiagIsKernelBusy())
  {
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_EXT_SETSECBYPASS); /* retry later again */                                      /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    if(Dcm_StateIsSecurityBypassPending())
    {
      /* Unlock security (excluding locked level = 0x00) */
      Dcm_StateSetSecurityBypass();
    }
    else
    {
      /* Lock security */
      Dcm_StateSetSecurity(0);
    }
  }
}
#endif

#if (DCM_STATE_AUTHENTICATION_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagWorkerSetAuthenticationBypass()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerSetAuthenticationBypass(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
)
{
  DCM_IGNORE_UNREF_PARAM(pDiagContext);

  /* Enforce bypass mode if no service is in processing! */
  if (Dcm_DiagIsKernelBusy())
  {
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_EXT_SETAUTHBYPASS); /* retry later again */                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    if (Dcm_StateIsAuthenticationBypassPending())
    {
      /* Set the bypass variable to bypass the preconditions check for service 0x29 */
      Dcm_AuthMgrSetAuthBypassValue(TRUE);
    }
    else
    {
      /* Clear the bypass variable to bypass the preconditions check for service 0x29 */
      Dcm_AuthMgrSetAuthBypassValue(FALSE);
    }
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagWorkerDoRepeat()
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagWorkerDoRepeat(                                                                                        /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,
  Dcm_DiagThreadContextConstPtrType pDiagContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_NegativeResponseCodeType lNrc = DCM_E_CONDITIONSNOTCORRECT;
  Dcm_NetTransportObjectPtrType pTranspObj;

  pTranspObj = Dcm_DiagGetCurTranspObj(pDiagContext);

#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_CANCEL_OP))
  {
    Dcm_RepeaterSetOpStatus(pContext, DCM_CANCEL); /* override any other opStatus values up to now */                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  }
#endif

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  lStdResult = Dcm_RepeaterCheckAndGetAccess(pContext, &lNrc);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)
#endif
  {
    lStdResult = Dcm_RepeaterExecute(pContext, &lNrc);                                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  switch (lStdResult)
  {
  case DCM_E_NOT_OK:
    Dcm_DiagSetNegResponse(lNrc, pContext->ThreadId);
    /* fall through */
  case DCM_E_OK:                                                                                                                                     /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
  case DCM_E_PROCESSINGDONE:
    Dcm_DiagProcessingDone(pDiagContext->ThreadId);
    break;
  case DCM_E_PENDING:
    Dcm_RepeaterSetOpStatus(pContext, DCM_PENDING);                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    /* fall through - keep the last opStatus on "pending" forced by Dcm_RepeaterExecute code */
  case DCM_E_PENDING_LIMITER:                                                                                                                        /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
    /* Retry next cycle with the same parameter */
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_WORK_REPEAT);                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case DCM_E_FORCE_RCRRP:
    Dcm_UtiEnterCriticalSection();/* protect against Dcm_TpTxConfirmation or hi-priority Dcm_DiagTaskRx */
    /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
    pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_RCRRP_TYPE_APPL;                                                                                          /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    if ((pTranspObj->Flags & (DCM_NET_TOBJ_FLAG_RCRRP_ON_TX | DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ)) == 0u)/* no RCR-RP already requested for transmission/ongoing? */
    {
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_RX, DCM_TSK_EV_DIAG_RX_TMR_P2_TO, pDiagContext->ThreadId);/* trigger a RCR-RP event timeout */
    }/* else - just wait for confirmation signal */

    /*=================================*
      END CRITICAL SECTION
      *=================================*/
    Dcm_UtiLeaveCriticalSection();
    break;
  default: /* DCM_E_STOP_REPEATER */
    Dcm_RepeaterSetCallee(pContext, DCM_DIAG_SERVICEINFO_REPEATEREND_IDX); /* any other return value leads to a dead end */                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  }
}

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoGetProgCond()
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoGetProgCond(                                                                                   /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Read the programming conditions of the pre-boot cycle */
  /* #10 If the ECU boot type is marked as a transition from the bootloader: */
  if(Dcm_GetProgConditions(&Dcm_DiagSingletonContext.StartUpFbl.ProgConditions) == DCM_WARM_START)                                                   /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  {
    sint16_least sesSfIter = -1;

# if (DCM_DIAG_NOTIF_BSWM_APPL_UPDATED_ENABLED == STD_ON)
    /* #20 If the application has been updated, notify the BswM (if enabled BswM notification) */
    if(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.ApplUpdated == TRUE)
    {
      BswM_Dcm_ApplicationUpdated();
    }
# endif

    /* Check the diagnostic service identifier to be processed here: */
# if (DCM_SVC_11_RST2BOOT_HIS_ENABLED == STD_ON)
    /* #30 If the SID is for "EcuReset": */
    if(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.Sid == 0x11u)
    {
      sint16_least subFncIter;

      subFncIter = Dcm_DiagVariantLookUpFilter(Dcm_CfgSvc11SubFuncLookUpTable
                                              ,Dcm_CfgSvc11SubFuncExecPrecondTable
                                              ,Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.SubFuncId);                                        /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

      /* Check if reset was internally triggered */
      if( (subFncIter >= 0)
        &&(Dcm_CfgWrapSvc11SubFuncInfo(subFncIter)->ResponseType == DCM_SVC11_RESPONSE_AFTER_RESET) )
      {
        Dcm_DebugReportSecurityEvent(pContext->ThreadId, DCM_DEBUG_SEV_ECU_RESET);
      }
    }
# endif

    /* #40 If the SID is for "DiagnosticSessionControl": */
    if(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.Sid == 0x10u)
    {
      sesSfIter = Dcm_DiagVariantLookUpFilter(Dcm_CfgSvc10SubFuncLookUpTable
                                             ,Dcm_CfgSvc10SubFuncExecPrecondTable
                                             ,Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.SubFuncId);                                         /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
    }
    /* Otherwise, send a response with any other SID and sub-function directly */

    if( (Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.ResponseRequired == TRUE) /* a response is to be sent */
      ||(sesSfIter > 0) ) /* OR a non-default session is to be activated (need ComM active) */
    {
      Dcm_NetConnRefMemType lConnHdl;
# if (DCM_NET_CONNECTION_ID_ENABLED == STD_ON)
      lConnHdl = Dcm_NetGetConnHdlByConnectionId(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.ConnectionId);
# else
      lConnHdl = Dcm_NetGetConnHdlByTesterAddress(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.TesterSourceAddr);
# endif

      if( (lConnHdl < DCM_NET_INVALID_CONNHDL)                                                                                                       /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
        &&( ( (Dcm_NetIsGenericConnection(lConnHdl))                                                                                                 /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
            &&(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.TesterSourceAddr <= DCM_NET_MAX_VAL_GENERIC_SRC_ADDRESS) )
          ||( (!Dcm_NetIsGenericConnection(lConnHdl))                                                                                                /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
            &&(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.TesterSourceAddr == Dcm_NetPbCfgGetConnTesterAddress(lConnHdl)) ) )                /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
# endif
         )
      {
        Dcm_NetTransportObjectPtrType pTranspObj; /* use a local copy to avoid collision with a RxIndication! */
        BufReq_ReturnType             lResult;

        lResult = Dcm_NetLockConnection(lConnHdl, &pTranspObj);                                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

        if(lResult == BUFREQ_OK) /* try to lock the connection to this tester */
        {
          Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(pContext->ThreadId);

          Dcm_DiagSingletonContext.StartUpFbl.ConnHdl = pTranspObj->ConnHdl;
          /* Source address of the request shall be the target address of the response. */
          pTranspObj->ClientSrcAddr = Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.TesterSourceAddr;                                           /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */

          Dcm_DiagSetCurTranspObj(pDiagContext, pTranspObj); /* store working context */                                                             /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */

          Dcm_TmrStartTimer(DCM_TMR_ID_FBLRES, pDiagContext->P2Timings.P2Star); /* wait at most P2 star to finalize the FBL positive response */
          /* Start communication */
          Dcm_NetRegisterActiveConnection(pTranspObj);                                                                                               /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
          Dcm_NetRegisterComMActivity(pTranspObj);                                                                                                   /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
          Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_WAITTXCOMM);/* Try immediately */                                           /* SBSW_DCM_PARAM_PTR_WRITE */
        }
        /* else - there is a problem locking the channel (there is already a tester request) */
      }
      else
      {
        Dcm_DebugReportError(DCM_SID_GETPROGCONDITIONS, DCM_E_PARAM);
      }
    }
    /* In any case store any analysis results for later use (i.e. during the clear response required flag) */
    Dcm_DiagSingletonContext.StartUpFbl.SessStateRef = (sint8)sesSfIter;
    Dcm_DiagSingletonContext.StartUpFbl.IsResponseRequired = Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.ResponseRequired;
    /* Prepare the OpStatus for the clear response required flag operation */
    Dcm_DiagSingletonContext.StartUpFbl.OpStatus = DCM_INITIAL;
    /* Signal the current task to start immediately the parallel job for clearing the response required flag, to avoid re-triggering of this sequence at next power on cycle */
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_RSTFLAGS);                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoWaitTxComm()
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
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoWaitTxComm(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;

  pTranspObj = Dcm_DiagGetCurTranspObj(Dcm_DiagGetThreadContext(pContext->ThreadId));                                                                /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */

  if(Dcm_NetIsComMChannelTransmissionReady(pTranspObj))                                                                                              /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
  {
    Dcm_TmrStopTimer(DCM_TMR_ID_FBLRES);

    if(Dcm_DiagSingletonContext.StartUpFbl.IsResponseRequired == FALSE) /* is final response required ? */
    {
      Dcm_DiagOnFblResTxFinished(pTranspObj, DCM_E_OK);/* Loop-back to the confirmation (successful "virtual" transmission) to clean up transportObject and ComM */ /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    }
    else
    {
      pTranspObj->State   = DCM_NET_TOBJ_STATE_PREPTX;                                                                                               /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_FBL_FINALRSP;                                                                                       /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->BuffInfo.SduDataPtr = &Dcm_DiagSingletonContext.StartUpFbl.ResBuffer[0];                                                           /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->BuffInfo.SduLength  = 2u; /* only service and a sub-function to be sent */                                                         /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      /* Prepare final response to be sent */
      Dcm_DiagPutStartUpFblResBuffer(0, (uint8)(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.Sid + 0x40u));
      Dcm_DiagPutStartUpFblResBuffer(1, Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.SubFuncId);

      if(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.Sid == 0x10u)
      {
        if(Dcm_DiagSingletonContext.StartUpFbl.SessStateRef >= 0)
        {
          Dcm_DiagPutStartUpFblResBuffer(2, Dcm_UtiGetHiByte(Dcm_CfgWrapSvc10SubFuncInfo(Dcm_DiagSingletonContext.StartUpFbl.SessStateRef)->ResponseTime.P2));
          Dcm_DiagPutStartUpFblResBuffer(3, Dcm_UtiGetLoByte(Dcm_CfgWrapSvc10SubFuncInfo(Dcm_DiagSingletonContext.StartUpFbl.SessStateRef)->ResponseTime.P2));
          Dcm_DiagPutStartUpFblResBuffer(4, Dcm_UtiGetHiByte(Dcm_CfgWrapSvc10SubFuncInfo(Dcm_DiagSingletonContext.StartUpFbl.SessStateRef)->ResponseTime.P2Star));
          Dcm_DiagPutStartUpFblResBuffer(5, Dcm_UtiGetLoByte(Dcm_CfgWrapSvc10SubFuncInfo(Dcm_DiagSingletonContext.StartUpFbl.SessStateRef)->ResponseTime.P2Star));
          pTranspObj->BuffInfo.SduLength = 6u;                                                                                                       /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
        else
        {
          Dcm_DebugReportError(DCM_SID_GETPROGCONDITIONS, DCM_E_PARAM);
        }
      }/* else - for SID 0x11 or other it is not critical to send any response - it will be up to the application to use the API! */

      Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
    }
  }
  else
  { /* Otherwise, check if still shall wait for ComM (deadline not yet expired): */
    if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_CANCEL))/* stop polling since time is up */
    {
      /* Loop-back to the confirmation (failed transmission) to clean up transportObject and ComM */
      Dcm_DiagOnFblResTxFinished(pTranspObj, DCM_E_NOT_OK);                                                                                          /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    }
    else
    {
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_FBLRES_WAITTXCOMM); /* Retry next time */                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
}
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoResetFlags()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoResetFlags(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);

  /* Reset all programming condition fields to their inactive states */
  Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.ReprogrammingRequest = FALSE;
  Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.ApplUpdated      = FALSE;
  Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.ResponseRequired = FALSE;

  /* Write the updated pre-conditions */

  lStdResult = Dcm_SetProgConditions(
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_450)
                                     Dcm_DiagSingletonContext.StartUpFbl.OpStatus,
# endif
                                     &Dcm_DiagSingletonContext.StartUpFbl.ProgConditions);                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch(lStdResult)
  {
  case DCM_E_OK:
    break;
  case DCM_E_PENDING:
    Dcm_DiagSingletonContext.StartUpFbl.OpStatus = DCM_PENDING;
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_FBLRES_RSTFLAGS);/* Retry next time */                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:                                                                                                                                           /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    Dcm_DebugReportError(DCM_SID_SETPROGCONDITIONS, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }
}
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagFblResDoProcessTxConfirmation()
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagFblResDoProcessTxConfirmation(                                                                         /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pEventContext);

  /* If the final response was for a non-default session and was successfully sent: */
  if( (Dcm_DiagSingletonContext.StartUpFbl.TxStatus == DCM_E_OK) /* only if successfully sent response ... */
    &&(Dcm_DiagSingletonContext.StartUpFbl.SessStateRef > 0) )/* ... of diagnostic session control for a non-default session ... */
  {
    /* Enter critical section (Reason: Protect against Dcm_StartOfReception) */
    Dcm_UtiEnterCriticalSection();

    /* Register the diagnostic client for this session */
    Dcm_NetSetSessionConnection(Dcm_DiagSingletonContext.StartUpFbl.ConnHdl); /* set connection for the session */
    Dcm_NetSetSessionClientSrcAddr(Dcm_DiagSingletonContext.StartUpFbl.ProgConditions.TesterSourceAddr); /* set tester for the session */

    /* Register new active protocol */
    Dcm_NetSetActiveProtocolConnHdl(Dcm_DiagSingletonContext.StartUpFbl.ConnHdl);

    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();

    /* Perform the corresponding session transition */
    Dcm_StateSetSession((Dcm_StateIndexMemType)Dcm_DiagSingletonContext.StartUpFbl.SessStateRef);

    /* Update P2 timings at once */
    Dcm_DiagSetP2Timings((Dcm_StateIndexMemType)Dcm_DiagSingletonContext.StartUpFbl.SessStateRef
                        ,Dcm_NetGetProtIdOfActiveProtocol()
                        ,pContext->ThreadId);
  }
  /* Unregister diagnostic client in order to consider any non-default session activated above */
  Dcm_NetUnRegisterComMActivity(Dcm_DiagSingletonContext.StartUpFbl.ConnHdl);
}
#endif

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagTxPostponeFinalPagedResponse()
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
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxPostponeFinalPagedResponse(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Wait for RCR-RP if SPRMIB was set */
  if (pDiagContext->MsgContext.msgAddInfo.suppressPosResponse != 0u)
  {
    /* wait for P2 timeout and RCR-RP response transmission */
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_TX_SEND_PAGED); /* retry later */                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_TskClrLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_PAGED); /* avoid transmit attempt now */                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagTxStartFinalResponse()
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
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxStartFinalResponse(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;

  pTranspObj = Dcm_DiagGetCurTranspObj(pDiagContext);

  /* kill any pending RCR-RP orders */
  Dcm_TskClrLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_RCRRP);                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  /* stop in any case since from now on only the service will be finalized (either with or without response) */
  Dcm_TmrStopTimerByThread(DCM_TMR_ID_P2, pDiagContext->ThreadId);

  if ( (pDiagContext->DoSuppressResponse == TRUE)                 /* No response to send */
      &&(0u == (pTranspObj->Flags & (DCM_NET_TOBJ_FLAG_RCRRP_SENT | DCM_NET_TOBJ_FLAG_RCRRP_ON_TX /* Suppress only if RcrRp was not sent or not waiting for RCR-RP confirmation. If just attempted -> skip RCR-RP transmission */
                              /* | DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ - since DCM_TSK_EV_DIAG_TX_SEND_RCRRP cleared -> RCR-RP will not be sent. Instead finish the service processing at P2 time */))) )
  {
    Dcm_DiagTxFinalization(pTranspObj, DCM_E_OK);/* always OK */                                                                                     /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
  }
  else
  {
    /* prepare final response transmission */
    if(pTranspObj->State == DCM_NET_TOBJ_STATE_READY)
    {
      Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;

      lBufferInfo = Dcm_NetPbCfgGetBufferInfo(pTranspObj->BufferHdl);

      /* clear flag */
      Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, (DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ | DCM_NET_TOBJ_FLAG_RCRRP_ON_TX));          /* PRQA S 4399 */ /* MD_Dcm_BitNegation_4399 */ /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->State = DCM_NET_TOBJ_STATE_PREPTX;                                                                                                 /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      pTranspObj->BuffInfo.SduDataPtr = lBufferInfo->BufferPtr;/* reassign the worker buffer */                                                      /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

      if(pDiagContext->ErrorRegister == DCM_E_POSITIVERESPONSE)
      {
        /* Positive response */
        pTranspObj->BuffInfo.SduLength = (PduLengthType)(pDiagContext->MsgContext.resDataLen
                                                       + pDiagContext->MsgContext.resProtHeadLen); /* don't forget the SID or/and SF bytes */        /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
        if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_PAGED))
        {
          pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_PAGED;                                                                                          /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
        else
#endif
        {
          pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_LINEAR;                                                                                         /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
      }
      else
      {
        /* Neg response */
        Dcm_NetComposeAddNegResponse(pTranspObj
                                    ,DCM_NET_TOBJ_RESTYPE_LINEAR
                                    ,pDiagContext->ErrorRegister);                                                                                   /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
        if(Dcm_DiagSingletonContext.CauseCodeContext.SendCauseCode == TRUE)
        {
          /* Provide specific cause code to negative response */
          Dcm_NetWriteAddBufferU8At(pTranspObj, 3, Dcm_DiagSingletonContext.CauseCodeContext.CauseCode);                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
          /* Add the additional byte of the cause code to the SduLength */
          pTranspObj->BuffInfo.SduLength = 4u;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
          Dcm_DiagSingletonContext.CauseCodeContext.SendCauseCode = FALSE;
        }
#endif
      }

      Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
    }
    else
    {
      /* retry later */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, (pEventContext->Ev & (Dcm_TskTaskEvOptType)(DCM_TSK_EV_DIAG_TX_SEND_PAGED | DCM_TSK_EV_DIAG_TX_SEND_LINEAR))); /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagTxStartRcrRpResponse()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DiagTxStartRcrRpResponse(
  Dcm_DiagThreadContextPtrType pDiagContext                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetTransportObjectPtrType pQueuedTObj;

  pQueuedTObj = Dcm_DiagGetCurQueuedTranspObj(pDiagContext);

  /*
   * Don't start the P2* timer within the RCR-RP confirmation but here because of prioritization and synchronization
   * issues which could occur otherwise.
   * Start now to cover also the application triggered RCR-RPs.
   */
  Dcm_TmrStartTimerByThread(DCM_TMR_ID_P2, pDiagContext->P2Timings.P2Star, pDiagContext->ThreadId);

  pDiagContext->MsgContext.msgAddInfo.suppressPosResponse = 0; /* reset the suppress bit and send final response */                                  /* SBSW_DCM_PARAM_PTR_WRITE */

  Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pQueuedTObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ);                                               /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */
  Dcm_UtiBitOpSet(Dcm_NetTransportObjectFlagType, pQueuedTObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_ON_TX); /* mark transmission flag */                   /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */

#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if((pQueuedTObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) != 0u)
  {
    /* suppress any RCR-RP responses on RoE - just do loop back ! */
    Dcm_DiagRcrRpTxFinished(pQueuedTObj, DCM_E_OK); /* always OK */                                                                                  /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */
  }
  else
#endif
  {
    Dcm_NetComposeAddNegResponse(pQueuedTObj
                                ,DCM_NET_TOBJ_RESTYPE_RCRRP
                                ,DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING);                                                                     /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */

    Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT); /* delegate job */
  }
}

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoveryInfoComMChannelState()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_RecoveryInfoComMChannelStatePtrType, DCM_CODE) Dcm_DiagGetRecoveryInfoComMChannelState(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo ,                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgNetNetIdRefOptType index
  )
{
  Dcm_RecoveryInfoComMChannelStatePtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= DCM_NET_MAX_NUM_COMM_CHANNELS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &(RecoveryInfo->ComMChannelState[0]);
  }
  else
  {
    lResult = &(RecoveryInfo->ComMChannelState[index]);
  }
  return lResult;
}
#endif

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON && DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagGetRecoveryInfoCommControlState()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_RecoveryInfoCommControlStatePtrType, DCM_CODE) Dcm_DiagGetRecoveryInfoCommControlState(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo ,                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgNetNetIdRefOptType index
  )
{
  Dcm_RecoveryInfoCommControlStatePtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= DCM_NET_MAX_NUM_ALL_COMM_CHANNELS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &(RecoveryInfo->CommControlState[0]);
  }
  else
  {
    lResult = &(RecoveryInfo->CommControlState[index]);
  }
  return lResult;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DiagRcrRpTxFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagRcrRpTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* COV_DCM_RTM_NO_RCR_RP_TX */ /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransmissionResultType txStatus
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));                      /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  pDiagContext->TxStatus = txStatus;/* store current Tx state */                                                                                     /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */

  pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_NONE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_ON_TX);                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_UtiBitOpSet(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, DCM_NET_TOBJ_FLAG_RCRRP_SENT);/* mark RCR-RP at least once sent state */        /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->State   = DCM_NET_TOBJ_STATE_READY;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */

  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_TX_END_RCRRP, Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));  /* Notify for sent RCR_RP */ /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_DiagTxFinalization()
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
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagTxFinalization(                                                                                               /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransmissionResultType txStatus
  )
{
  Dcm_ContextPtrType pContext = Dcm_GetThreadContext(Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));                                        /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(pContext->ThreadId);

  pDiagContext->TxStatus = txStatus;                                                                                                                 /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
  pDiagContext->State = DCM_DIAG_STATE_POST_PROCESS;                                                                                                 /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */

  Dcm_SplitTaskEnterCS();
  /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/
  Dcm_UtiBitOpClr(Dcm_DiagProcessorFlagType, pDiagContext->QueuedSet.Flags, DCM_DIAG_QUEUE_FLAG_IS_ACTIVE);                                          /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
  /*=================================*
    END CRITICAL SECTION
    *=================================*/
  Dcm_SplitTaskLeaveCS();

  if(((DCM_DIAG_APPL_NOTIFICATION_FASTPOSTPROCESSOR) & pDiagContext->ApplNotification) != 0u)                                                        /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
    Dcm_DiagSvcConfirmationFuncType lFastConfFunc;
    Dcm_ConfirmationStatusType      lConfStatus;

    lFastConfFunc = Dcm_CfgWrapDiagGetPostProcessorFunc(Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex)->FastConfFuncRef);                        /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
    lConfStatus   = Dcm_DiagGetPostProcessResult(pDiagContext);                                                                                      /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */

    /* Execute any internal service processors fast-post-handlers */
    lFastConfFunc(pContext, lConfStatus);                                                                                                            /* SBSW_DCM_CALL_FUNCPTR_SVCPOSTPROCESSORS */ /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */
  }

  /* Release the connection */
  if((DCM_NET_TOBJ_FLAG_CANCELED & pTranspObj->Flags) == 0u) /* let the connection expire with the Tx confirmation (after CancelTransmit) */
  {
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
    Dcm_NetUnRegisterActiveConnection(pTranspObj);/* Start the S3 timer if needed here to have maximum real-time accuracy (from communication bus point of view) */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_TX_END_FINAL, Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));          /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
}

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagOnFblResTxFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagOnFblResTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransmissionResultType txStatus
  )
{
  Dcm_NetBufferRelease(pTranspObj->BufferHdl);
  Dcm_NetUnRegisterActiveConnection(pTranspObj);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  Dcm_DiagSingletonContext.StartUpFbl.TxStatus = txStatus;
  Dcm_TskSetEvent(DCM_TSK_ID_DIAG_FBLRES, DCM_TSK_EV_DIAG_FBLRES_TXCONF);
}
#endif

#if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagOnRoeIntmRspTxFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_DiagOnRoeIntmRspTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetBufferRelease(pTranspObj->BufferHdl);
  Dcm_NetUnRegisterActiveConnection(pTranspObj);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
}
#endif

#if (DCM_DIAG_SYSTEM_NOTIFICATION_ENABLED == STD_ON) || (DCM_DIAG_OEM_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagExecIndicationFunc()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagExecIndicationFunc(
  Dcm_DiagThreadContextPtrType pDiagContext,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgItemType sid,
  Dcm_DiagApplNotificationType notificationType,
  Dcm_CfgDiagNotificationInfoPtrType notifList,
  uint16_least notifListSize,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lResult = DCM_E_OK; /* positive case */
  uint16_least   lIter;

  pDiagContext->ApplNotification |= notificationType; /* mark application has been notified */                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

  *ErrorCode = DCM_E_POSITIVERESPONSE;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  for (lIter = 0u; lIter < notifListSize; ++lIter)                                                                                                   /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  {
    Std_ReturnType               lStdResult;
    Dcm_NegativeResponseCodeType lNrc = DCM_E_POSITIVERESPONSE; /* Initialize out parameter */

    lStdResult = notifList[lIter].IndFunc(sid
                                         ,pDiagContext->MsgContext.reqData
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_450)
                                         ,pDiagContext->MsgContext.reqDataLen
                                         ,(uint8)pDiagContext->MsgContext.msgAddInfo.reqType
                                         ,Dcm_NetPbCfgGetConnId(Dcm_DiagGetCurTranspObj(pDiagContext)->ConnHdl)
                                         ,&lNrc
                                         ,Dcm_NetPbCfgGetProtocolId(Dcm_NetPbCfgGetConnProtIdOfRxPduId(pDiagContext->MsgContext.rxPduId))            /* SBSW_DCM_PARAM_PTR_FORWARD */
                                         ,pDiagContext->PostProcessorContext.SrcAddr
# else
                                         ,(uint16)pDiagContext->MsgContext.reqDataLen
                                         ,(uint8)pDiagContext->MsgContext.msgAddInfo.reqType
                                         ,pDiagContext->PostProcessorContext.SrcAddr
                                         ,&lNrc
# endif
                                         );                                                                                                          /* SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC */

    if(lStdResult != DCM_E_OK)
    {
      lResult = DCM_E_NOT_OK; /* Mark error, but let other checks to be performed - check for higher priority NOT_ACCEPTED result */

      switch(lStdResult)
      {
      case DCM_E_NOT_OK:
        Dcm_UtiHandleApplNrc(lStdResult, &lNrc, DCM_E_CONDITIONSNOTCORRECT);                                                                         /* SBSW_DCM_POINTER_FORWARD_STACK */
        break;
      case DCM_E_REQUEST_NOT_ACCEPTED:
        lNrc = DCM_E_CONDITIONSNOTCORRECT; /* In DCM_E_REQUEST_NOT_ACCEPTED case no positive response shall be sent! */
        pDiagContext->DoSuppressResponse = TRUE; /* Try to suppress, but if an RCR-RP has been already sent - send the registered NRC */             /* SBSW_DCM_PARAM_PTR_WRITE */
        break;
      default:
        /* Unexpected return value */
        Dcm_DebugReportError(DCM_SID_INDICATIONFUNC, DCM_E_INTERFACE_RETURN_VALUE);
        lNrc = DCM_E_PANIC_NRC;
        break;
      }

      /* Try to register NRC if no other has been set! */
      if (*ErrorCode == DCM_E_POSITIVERESPONSE)
      {
        *ErrorCode = lNrc;                                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
  }

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagLookUpFilter()
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
DCM_LOCAL FUNC(sint16_least, DCM_CODE) Dcm_DiagLookUpFilter(                                                                                         /* PRQA S 6060 */ /* MD_MSR_STPAR */
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  uint8 value,
  Dcm_DiagCheckLvlType chkLevel,
  Dcm_NegativeResponseCodePtrType errorCode
  )
{
  sint16_least lLookUpResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  /* Check if diagnostic object exists in current variant */
  lLookUpResult = Dcm_DiagVariantLookUpFilter(lookUpTable, lookUpFilterTable, value);                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* If diagnostic object not supported in current variant at all: */
  if(lLookUpResult < 0)
  {
    /* Set appropriate NRC depending on the diagnostic object type */
    *errorCode = Dcm_DiagObjNotFoundNrcs[chkLevel];                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  /* Otherwise if requested SID is not accessible by the client sent it, exit service validation by sending NRC 0x11 */
  else if( (chkLevel == DCM_DIAG_CHK_LVL_SERVICE_ID)
         &&((Dcm_PbCfgDiagSvcId2ProtMap[lLookUpResult] & Dcm_UtiGetBitFromIndex(Dcm_CfgDiagSvc2ProtMapMemType, Dcm_NetPbCfgGetConnProtIdOfRxPduId(pMsgContext->rxPduId))) == 0u) ) /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    lLookUpResult = -1; /* override result */
    *errorCode = DCM_E_SERVICENOTSUPPORTED;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    /* Otherwise return positive result  */
  }

  return lLookUpResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagSvcWithSubFuncStateCheck()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagSvcWithSubFuncStateCheck(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_CfgStateRefOptType stateRef,
  Dcm_DiagSubFuncSequenceCheckerFuncType subFuncSequenceChecker,
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);
  DCM_IGNORE_UNREF_PARAM(subFuncSequenceChecker);
  DCM_IGNORE_UNREF_PARAM(subSvcRef);

  if(!Dcm_StateIsSupportedInSession(stateRef))
  {
    /* If session is not correct, send NRC 0x7E */
    *ErrorCode = DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION;                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if(Dcm_AuthMgrCheckRole(stateRef, Dcm_NetPbCfgGetConnAuthInfoRefByRxPduId(pMsgContext->rxPduId)) == DCM_E_NOT_OK)                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    /* If authentication role is not correct, send NRC 0x34 */
    lStdReturn = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_AUTHENTICATIONREQUIRED;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
#endif
  /* If supported, verify the security access level pre-condition (UDS verification order) */
#if (DCM_STATE_SECURITY_ENABLED == STD_ON)
  if(!Dcm_StateIsSupportedInSecurityLevel(stateRef))
  {
    /* If not met, do return NRC 0x33 (UDS) */
    *ErrorCode = DCM_E_SECURITYACCESSDENIED;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
#endif
  if(subFuncSequenceChecker(pMsgContext, subSvcRef, ErrorCode) != DCM_E_OK)                                                                          /* SBSW_DCM_PARAM_FUNCPTR_CALL */
  {
    /* If SF sequence is not correct, ErrorCode already set */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
  {
#if (DCM_MODEMGR_CHECK_SUPPORT_ENABLED == STD_ON)
    /* Otherwise, if supported, get the the mode-related pre-condition expression(s) result */
    lStdReturn = Dcm_ModeCheck(Dcm_CfgWrapStatePreconditions(stateRef)->ModeRuleRef, ErrorCode);                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
#else
    /* Otherwise, everything fine - return positive response code */
    lStdReturn = DCM_E_OK;
#endif
  }

  return lStdReturn;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DiagGetTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetTransportObjectPtrType, DCM_CODE) Dcm_DiagGetTranspObj(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_DiagThreadContextConstPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);

  return Dcm_DiagGetCurTranspObj(pDiagContext);
}

/**********************************************************************************************************************
 *  Dcm_DiagGetQueuedTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetTransportObjectPtrType, DCM_CODE) Dcm_DiagGetQueuedTranspObj(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_DiagThreadContextConstPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);

  return Dcm_DiagGetCurQueuedTranspObj(pDiagContext);
}

/**********************************************************************************************************************
 *  Dcm_DiagSetTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagSetTranspObj(
  Dcm_ThreadIdOptType threadId,
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);

  Dcm_DiagSetCurTranspObj(pDiagContext, pTranspObj);                                                                                                 /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
}

/**********************************************************************************************************************
 *  Dcm_DiagIsKernelBusy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_DiagIsKernelBusy(
  void
  )
{
  boolean             lResult = FALSE;
  Dcm_ThreadIdOptType lThreadIter;

  for(lThreadIter = 0u; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
  {
    lResult = Dcm_DiagIsThreadBusy(lThreadIter);

    if (lResult == TRUE)
    {
      break;
    }
  }
  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagIsThreadBusy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_DiagIsThreadBusy(
  Dcm_ThreadIdOptType threadId
  )
{
  boolean lResult = FALSE;

  Dcm_DiagThreadContextConstPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);

  if (pDiagContext->State != DCM_DIAG_STATE_IDLE)
  {
    lResult = TRUE;
  }

  return lResult;
}

#if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsEnabledInActiveVariants()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_DiagIsEnabledInActiveVariants(
  Dcm_CfgStateRefOptType cfgVariantRef
  )
{
  return Dcm_VarMgrIsEnabledInActiveVariants(Dcm_PbCfgDiagVariantFilter[cfgVariantRef]);                                                             /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagVariantLookUpFilter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(sint16_least, DCM_CODE) Dcm_DiagVariantLookUpFilter(
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  uint8 value
  )
{
  sint16_least lLookUpResult = Dcm_UtiLookUpUint8(lookUpTable, value);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

  DCM_IGNORE_UNREF_PARAM(lookUpFilterTable);

#if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
  if (lLookUpResult >= 0)
  {
    if (Dcm_DiagIsEnabledInActiveVariants(lookUpFilterTable[lLookUpResult]) == FALSE)                                                                /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      lLookUpResult = -1; /* override result */
    }
  }
#endif

  return lLookUpResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagSetP2Timings()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagSetP2Timings(
  Dcm_StateIndexMemType sesStateIdx,
  Dcm_NetProtRefMemType protocolIdx,
  Dcm_ThreadIdMemType threadId
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);
  Dcm_DiagP2TimingsConstPtrType lSrvAdjTimes;

  Dcm_UtiEnterCriticalSection();

  /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/
  /* Retrieve the adjustment time */
  lSrvAdjTimes = Dcm_NetPbCfgGetProtocolSrvAdjTime(protocolIdx);

  /* Calculate the current P2 timings considering the protocol related adjust timings */
  pDiagContext->P2Timings.P2 = Dcm_DiagGetP2Time(sesStateIdx)
                              - lSrvAdjTimes->P2;                                                                                                    /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */

  pDiagContext->P2Timings.P2Star = Dcm_DiagGetP2StarTime(sesStateIdx)
                                  - lSrvAdjTimes->P2Star;                                                                                            /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */

  /*=================================*
    END CRITICAL SECTION
    *=================================*/
  Dcm_UtiLeaveCriticalSection();
}

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsSessionChangeAllowed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagIsSessionChangeAllowed(
  Dcm_ThreadIdMemType threadId                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  /* If a non-default session will be active: */
  if(Dcm_StateIsDefaultSessionPending() == FALSE)
  {
    Dcm_ThreadIdOptType lThreadIter;

    for(lThreadIter = 0u; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
    {
      /* Skip own thread */
      if(lThreadIter != threadId)
      {
        Dcm_DiagThreadContextConstPtrType otherContext = Dcm_DiagGetThreadContext(lThreadIter);

        if( (otherContext->State != DCM_DIAG_STATE_IDLE)
          ||(otherContext->QueuedSet.Flags != DCM_DIAG_QUEUE_FLAG_NONE) )
        {
          lStdResult = DCM_E_NOT_OK;
        }
      }
    }
  }

  return lStdResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagGetDemClientId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_DiagGetDemClientId(
  Dcm_ReadOnlyMsgContextPtrType pMsgContext
  )
{
  return Dcm_NetPbCfgGetProtocolDemClientIdByConnHdl(Dcm_NetPbCfgGetConnHdlOfRxPduId((pMsgContext)->rxPduId));
}

/**********************************************************************************************************************
 *  Dcm_DiagSvcWithOnlySubFuncReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_DiagSvcWithOnlySubFuncReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  DCM_IGNORE_UNREF_PARAM(sfRef);

  return 1u;
}

/**********************************************************************************************************************
 *  Dcm_DiagNoSequenceChecker()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagNoSequenceChecker(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(subSvcRef);
  DCM_IGNORE_UNREF_PARAM(ErrorCode);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  return DCM_E_OK;
}

/**********************************************************************************************************************
 *  Dcm_DiagVerifyServiceWithSubFunction()
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
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagVerifyServiceWithSubFunction(                                                                                 /* PRQA S 6060 */ /* MD_MSR_STPAR */
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  Dcm_DiagServiceLengthGetterFuncType svcLengthGetter,
  Dcm_DiagSubFuncSequenceCheckerFuncType subFuncSequenceChecker,
  Dcm_DiagSubServiceRefOptPtrType pSubSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  sint16_least   lLookUpResult;
  uint8          sfID = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                          /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  *pSubSvcRef = 0;                                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */

  /* Min Length already checked in DiagDispatcher and expected to be 1 !!! */
  Dcm_DebugAssert((pMsgContext->reqDataLen > 0u), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                            /* COV_DCM_RTM_DEV_DEBUG TX */

  lLookUpResult = Dcm_DiagLookUpFilter(pMsgContext
                                      ,lookUpTable
                                      ,lookUpFilterTable
                                      ,sfID
                                      ,DCM_DIAG_CHK_LVL_SUBFUNC
                                      ,ErrorCode);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lLookUpResult >= 0)
  {
    /* Mark found position in any case if found */
    *pSubSvcRef = (Dcm_DiagSubServiceRefOptType)lLookUpResult;                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Check security, sequence, and mode */
    lStdResult = Dcm_DiagSvcWithSubFuncStateCheck(pMsgContext
                                                 ,lookUpFilterTable[lLookUpResult]
                                                 ,subFuncSequenceChecker
                                                 ,*pSubSvcRef
                                                 ,ErrorCode);                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)
    {
      Dcm_DiagSubServiceLengthType expectedLength;
      expectedLength = svcLengthGetter(*pSubSvcRef);                                                                                                 /* SBSW_DCM_PARAM_FUNCPTR_CALL */

      if( (expectedLength != 0u) /* dynamic length */
        &&(expectedLength != pMsgContext->reqDataLen) ) /* SF + data */
      {
        *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }
    /* Extract SF in any case if supported */
    Dcm_UtiConsumeReqData(pMsgContext, 1u);                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, sfID);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_DiagSetNewReqBaseToCurProgress(pMsgContext);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_DiagSetNewResBaseToCurProgress(pMsgContext);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    /* else - ErrorCode is already set */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagTaskRx()
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
 */
FUNC(void, DCM_CODE) Dcm_DiagTaskRx(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_NetTransportObjectPtrType pQueuedTObj;
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(pContext->ThreadId);                                                          /* PRQA S 3679 */ /* MD_Dcm_Design_3679 */

  pQueuedTObj = Dcm_DiagGetCurQueuedTranspObj(pDiagContext);

  /* If new request received: */
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RX_NEW_REQ))
  {
    /* Initialize related objects */
    pQueuedTObj->State = DCM_NET_TOBJ_STATE_READY;                                                                                                   /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */
    Dcm_DiagResetRcrRpLimiter(pDiagContext);                                                                                                         /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
    /* Synchronize with diagnostic job processing task */
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_NEW_REQ, pContext->ThreadId);
  }

  /* If P2 timeout detected: */
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RX_TMR_P2_TO))
  {
    /* Pre-processing a P2 timeout in oder to cancel the working thread in time (on non-split task configuration) */
#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
    /* If RCR-RP limit reached: */
    if(Dcm_DiagGetRcrRpLimiter(pDiagContext) == 0u)
    {
      /* Reset any RCR-RP related states and notify worker task to initiate job cancellation */
      Dcm_DiagRcrRpTxFinished(pQueuedTObj, DCM_E_NOT_OK);                                                                                            /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */

      /*
       * Signal cancellation to the worker. If worker gets interrupted by this high priority task,
       * while already called "processing done", the cancellation signal will be lost and
       * the final response will be sent at next worker task call.
       */
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_CANCEL_OP, pContext->ThreadId);

      /*
       * Do not send RCR-RP and restart the P2 timer again since the limit has been reached and the worker will
       * finalize the job, once the cancel signal is received.
       */
    }
    else
#endif
    {
#if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
      /* Otherwise - update RCR-RP limiter */
      Dcm_DiagDecrementRcrRpLimiter(pDiagContext);                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
#endif
      /* Notify diagnostic transmission task to initiate RCR-RP transmission */
      pQueuedTObj->Flags |= DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ;                                                                                          /* SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ */
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_RCRRP, pContext->ThreadId);
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagTaskWorker()
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
FUNC(void, DCM_CODE) Dcm_DiagTaskWorker(                                                                                                             /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Important: this event is a synchronization event between high- and low-priority tasks.
   * Never move this event processing into another task than the one that initiates the new request processing (DCM_TSK_EV_DIAG_WORK_NEW_REQ).
   * Otherwise the clean-up jobs from DCM_TSK_EV_DIAG_WORK_KILL_JOBS event may be performed AFTER the new request processing has been started.
   * This will lead to out of sync of services post- and main-processing!
  */

  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(pContext->ThreadId);

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_KILL_JOBS))
  {
    Dcm_DiagWorkerDoCancelProcessing(pContext, pDiagContext, pEventContext);                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_TX_END_FINAL))
  {
    Dcm_DiagWorkerDoPostProcessing(pContext, pDiagContext);                                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_EXT_SET2DEF))
  {
    Dcm_DiagWorkerSetDefSessionExtern(pDiagContext, pEventContext);                                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF))
  {
    Dcm_StateSetSession(0);
  }

#if (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_IDLE_CONN))
  {
    Dcm_AuthMgrDeauthenticateIdleConn();
  }
#endif

#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_EXT_AUTHENTICATION))
  {
    Dcm_AuthMgrSetExtAuthentication(pContext, pEventContext);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
#endif

#if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_EXT_SETSECBYPASS))
  {
    Dcm_DiagWorkerSetSecurityBypass(pDiagContext, pEventContext);                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
  }
#endif

#if (DCM_STATE_AUTHENTICATION_BYPASS_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_EXT_SETAUTHBYPASS))
  {
    Dcm_DiagWorkerSetAuthenticationBypass(pDiagContext, pEventContext);                                                                              /* SBSW_DCM_COMB_PTR_FORWARD */
  }
#endif

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_NEW_REQ))
  {
    Dcm_DiagWorkerProcessNewRequest(pContext, pDiagContext, pEventContext);                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_TX_END_RCRRP))
  {
    Dcm_DiagWorkerProcessRcrRpTxEnd(pContext, pDiagContext, pEventContext);                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
  }

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON) && \
    (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_CANCEL_OP))
  {
    Dcm_DiagWorkerCancelOperation(pContext, pDiagContext, pEventContext);                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */
  }
#endif

  if(Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_WORK_REPEAT))
  {
    Dcm_DiagWorkerDoRepeat(pContext, pDiagContext, pEventContext);                                                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagTaskTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagTaskTx(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(pContext->ThreadId);

#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_PAGED))
  {
    Dcm_DiagTxPostponeFinalPagedResponse(pDiagContext, pEventContext);                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */
  }
#endif

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, (DCM_TSK_EV_DIAG_TX_SEND_PAGED | DCM_TSK_EV_DIAG_TX_SEND_LINEAR)))
  {
    Dcm_DiagTxStartFinalResponse(pDiagContext, pEventContext);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_TX_SEND_RCRRP))
  {
    Dcm_DiagTxStartRcrRpResponse(pDiagContext);                                                                                                      /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
  }
}

#if (DCM_TSK_NUM_DIAG_RECOVERY > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskRecovery()
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
FUNC(void, DCM_CODE) Dcm_DiagTaskRecovery(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_GETSTATE))
  {
    Std_ReturnType lResult;

    /* Retrieve the recovery information from the application */
    lResult = Dcm_GetRecoveryStates(&Dcm_DiagRecoverySingletonContext.RecoveryInfo);                                                                 /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

    switch(lResult)
    {
    case DCM_E_OK:
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_PROCESS);                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_DiagRecoverySingletonContext.OpStatus = DCM_INITIAL;
      break;
    case DCM_E_PENDING:
      /* Retry next time */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_RECOVERY_GETSTATE);                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    default:
      Dcm_DebugAssert((lResult == DCM_E_NOT_OK), DCM_SID_GETRECOVERYSTATES, DCM_E_INTERFACE_RETURN_VALUE);                                           /* COV_DCM_RTM_DEV_DEBUG TX */
      /* Abort recovering */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_FINISH);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_PROCESS))
  {
    Std_ReturnType   lResult;
    lResult = Dcm_DiagSafeProcessRecoveryInfo(&Dcm_DiagRecoverySingletonContext.RecoveryInfo
                                             ,Dcm_DiagRecoverySingletonContext.OpStatus);                                                            /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

    switch(lResult)
    {
    case DCM_E_OK:
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_FINISH);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    case DCM_E_PENDING:
      /* Retry next time */
      Dcm_DiagRecoverySingletonContext.OpStatus = DCM_PENDING;
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_DIAG_RECOVERY_PROCESS);                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    default: /* DCM_E_NOT_OK */
      /* Abort recovering */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_FINISH);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_RECOVERY_FINISH))
  {
# if (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON) && \
     (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
    Dcm_Svc2CDefMgrOnStateChanged();
# endif
    Dcm_NetSetRxAllowed(TRUE);
  }
}
#endif

#if (DCM_TSK_NUM_DIAG_GARB_COL > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskGarbageCollector()
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
 */
FUNC(void, DCM_CODE) Dcm_DiagTaskGarbageCollector(                                                                                                   /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_TskTaskEvOptType        lEv = pEventContext->Ev;
  Dcm_CfgNetTObjHandleOptType lTObjCntr;

  DCM_IGNORE_UNREF_PARAM(pContext);

  /* Cleanup or reject all transport objects that never succeeded and are now canceled */
  lTObjCntr = 0;
  DCM_TSK_EVENT_SCAN_LOOP(lEv)
  {
    /* Check who shall be removed */
    if (Dcm_TskIsLocalEventSet(lEv, 0x01u))
    {
      Dcm_NetTransportObjectPtrType pTranspObj;

      pTranspObj = Dcm_NetPbCfgGetTransportObject(lTObjCntr);
      /* Always unregister ComM activity here, since CANCELED flag will not clean it up!
       * If no RCR-RP on transmission: everything has to be cleaned up here!
       */
      Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl);

      /* If just requested, then only this TObject could be the trigger for P2 timer - avoid any RCR-RP transmission */
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ) != 0u)
      {
        Dcm_TskClrEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_RCRRP, Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));          /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      }

      Dcm_UtiEnterCriticalSection();
      /*=================================*
       BEGIN CRITICAL SECTION
       *=================================*/
      /* Check if any RCR-RP for this object is on Tx (DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ is excluded since the high-prio
       * Dcm_DiagTaskTx() shall already have switched to ON_TX!) */
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_RCRRP_ON_TX) != 0u)
      {
        pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_CANCELED; /* isolate this connection */                                                               /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

        /* Try cancel any ongoing response. The return value does not matter:
         * OK -> there will be a confirmation for this cancellation
         * N_OK -> request rejected, but there will be a confirmation due to the ongoing transmission
         */
        (void)PduR_DcmCancelTransmit(Dcm_NetPbCfgGetConnTxPduIdMain(pTranspObj->ConnHdl));
        /* Any RCR-RP will just end with releasing the connection assigned to the tObject!
         * No confirmation post-processing will be performed! */
      }
      else
      {
        Dcm_NetBufferRelease(pTranspObj->BufferHdl);
        Dcm_NetUnRegisterActiveConnection(pTranspObj); /* no further processing needed */                                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      }
      /*=================================*
       END CRITICAL SECTION
       *=================================*/
      Dcm_UtiLeaveCriticalSection();
    }
    ++lTObjCntr;
  }
}
#endif

#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskFblRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagTaskFblRes(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Check for eventual FBL final response request */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_SEND))
  {
    Dcm_DiagFblResDoGetProgCond(pContext, pEventContext);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* If final response required, wait for ComM before sending the response */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_WAITTXCOMM))
  {
    Dcm_DiagFblResDoWaitTxComm(pContext, pEventContext);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* Once final FBL response request processed, reset all flags to prevent from a new response at next power-on/reset */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_RSTFLAGS))
  {
    Dcm_DiagFblResDoResetFlags(pContext, pEventContext);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* If any FBL final response confirmation received, do the designated job (e.g. session transition) */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_DIAG_FBLRES_TXCONF))
  {
    Dcm_DiagFblResDoProcessTxConfirmation(pContext, pEventContext);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}
#endif
/**********************************************************************************************************************
 *  Dcm_DiagRxIndication()
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagRxIndication(                                                                                                           /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));                      /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */

#if (DCM_DIAG_RST2DEF_ON_FUNC_OBD_REQ_ENABLED == STD_ON)
  /* #10 Handle OBD requests (SID [0x00-0x0F]) when only one diagnostic client is used */
  if ( (pTranspObj->RxData[0] < 0x10u)
# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
     &&(Dcm_NetPbCfgGetNumProtocols() == 1u)                                                                                                         /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
# endif
     &&(Dcm_StateIsDefaultSessionPending() == FALSE)                                                                                                 /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
     &&(Dcm_NetPbCfgIsFuncRequest(pTranspObj->RxPduId) == TRUE)                                                                                      /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
     )
  {
    /* The number of threads is limited to one, so use basic thread here */
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF, DCM_THREAD_ID_BASIC);
    Dcm_DiagSetP2Timings(0, 0, DCM_THREAD_ID_BASIC);
  }
#endif

  /* #20 Update diagnostic kernel queue flags: */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/
  pDiagContext->QueuedSet.Flags |= DCM_DIAG_QUEUE_FLAG_IS_WAITING;                                                                                   /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
  /*=================================*
    END CRITICAL SECTION
    *=================================*/
  Dcm_UtiLeaveCriticalSection();

  /* #30 Prepare for new diagnostic request processing by setting all relevant for the real-time task properties: */
  Dcm_DiagSetQueuedTranspObj(pDiagContext, pTranspObj);                                                                                              /* SBSW_DCM_COMB_PTR_FORWARD */
  Dcm_TmrStartTimerByThread(DCM_TMR_ID_P2, pDiagContext->P2Timings.P2, Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));                      /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
  Dcm_NetSetKeepAlive(TRUE);
#endif

  /* #40 Signal the Dcm_DiagTaskWorker that the request is now ready for processing */
  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_RX, DCM_TSK_EV_DIAG_RX_NEW_REQ, Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));                   /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_DiagInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagInit(
  Dcm_ConfigPtrType configPtr
  )
{
  DCM_IGNORE_UNREF_PARAM(configPtr);

#if (DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON) && (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
  Dcm_DiagSingletonContext.Svc2ProtocolMap = configPtr->BridgeVariant.Svc2ProtocolMap;
#endif
#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON) && (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
  Dcm_DiagSingletonContext.RomVariant = configPtr->RomVariant->DiagRom;
#endif

  {
    Dcm_ThreadIdOptType lThreadIter;
    for(lThreadIter = 0; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
    {
      Dcm_ContextPtrType pContext = Dcm_GetThreadContext(lThreadIter);
      Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(lThreadIter);

      pDiagContext->ThreadId = (Dcm_ThreadIdMemType)lThreadIter;                                                                                     /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
      pContext->ThreadId = (Dcm_ThreadIdMemType)lThreadIter;                                                                                         /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

      /*
      * Initialize pMsgContext so that it points always to a valid buffer.
      * From now on the buffer information can only be changed via Dcm_DiagInitiateServiceProcessing().
      * So in case that Dcm_DiagInitiateServiceProcessing() was called, but for a different (wrong) thread,
      * DCM still never writes to an invalid memory locations due to this initialization.
      */
      Dcm_DiagInitMsgContextBufferInfo(&(pDiagContext->MsgContext), 0u);                                                                             /* SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT */
    }
  }

#if (DCM_DIAG_SVC_INITIALIZERS_ENABLED == STD_ON)
  /* Initialize all processors */
  {
    uint8_least lIter;

    for (lIter = 0u; lIter < DCM_CFGDIAGSVCINITIALIZERS_SIZE; ++lIter)                                                                               /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
    {
      Dcm_CfgDiagSvcInitializers[lIter]();                                                                                                           /* SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC */
    }
  }
#endif

  {
    Dcm_ThreadIdOptType lThreadIter;

    /* Initialize all P2 Timers */
    for(lThreadIter = 0u; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
    {
      Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(lThreadIter);

      pDiagContext->QueuedSet.Flags = DCM_DIAG_QUEUE_FLAG_NONE;                                                                                      /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
#if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
      pDiagContext->QueuedSet.QueuedTObjects = 0;                                                                                                    /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
#endif
      pDiagContext->State = DCM_DIAG_STATE_IDLE;                                                                                                     /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */

      Dcm_DiagSetP2Timings(0, 0, (Dcm_ThreadIdMemType)lThreadIter);
    }
  }

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
  Dcm_NetSetRxAllowed(FALSE);
  Dcm_TskSetEvent(DCM_TSK_ID_DIAG_RECOVERY, DCM_TSK_EV_DIAG_RECOVERY_GETSTATE);
#endif

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
  /* Delegate to the first task activation */
  Dcm_TskSetEvent(DCM_TSK_ID_DIAG_FBLRES, DCM_TSK_EV_DIAG_FBLRES_SEND);
#endif
}

/**********************************************************************************************************************
 *  Dcm_DiagAcceptNewRequest()
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
 */
FUNC(boolean, DCM_CODE) Dcm_DiagAcceptNewRequest(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  boolean lResult = TRUE;
  /* If SID in range: [0x40, 0x7F]U[0xC0, 0xFF] */
#if (DCM_DIAG_IGNORE_RESPONSE_SID_ENABLED == STD_ON)
  if((pTranspObj->RxData[0] & 0x40u) != 0u)
  {
    pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_IGNORE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = FALSE;
  }
  else
#endif
  /* If valid functional "0x3E 0x80" */
  if( (pTranspObj->RxData[0] == 0x3Eu)
    &&(pTranspObj->RxData[1] == 0x80u)
    &&(pTranspObj->RxLength  == 2u)
    &&(Dcm_NetPbCfgIsFuncRequest(pTranspObj->RxPduId) == TRUE) )                                                                                     /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
#if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
    /* In case of generic connections, a connection can be used by a pool of testers. Tester address has to be checked as well. */
    if( (Dcm_StateIsDefaultSessionPending() == FALSE) /* A non-default session is/will be active */
      &&( (Dcm_NetGetSessionConnection() != pTranspObj->ConnHdl)                                                                                     /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
        ||(Dcm_NetGetSessionClientSrcAddr() != pTranspObj->ClientSrcAddr)) ) /* Request from different tester */                                     /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
    {
      pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_IGNORE;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    }
#endif
    lResult = FALSE;
  }
  else
  {
    /* nothing to do */
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_DiagOnTxFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagOnTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransmissionResultType txStatus
  )
{
  switch(pTranspObj->ResType)
  {
  case DCM_NET_TOBJ_RESTYPE_LINEAR:
  case DCM_NET_TOBJ_RESTYPE_PAGED:
    /* Do finalize main response */
    Dcm_DiagTxFinalization(pTranspObj, txStatus);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_NET_TOBJ_RESTYPE_RCRRP:
    Dcm_DiagRcrRpTxFinished(pTranspObj, txStatus);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
  case DCM_NET_TOBJ_RESTYPE_FBL_FINALRSP:
    Dcm_DiagOnFblResTxFinished(pTranspObj, txStatus);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#endif
#if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
  case DCM_NET_TOBJ_RESTYPE_ROE_INTMRSP:
    Dcm_DiagOnRoeIntmRspTxFinished(pTranspObj);                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    /* Unknown response type */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagProcessingDone()
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
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagProcessingDone(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);

  /* If positive response to be sent: */
  if(pDiagContext->ErrorRegister == DCM_E_POSITIVERESPONSE)
  {
    /* If SPRMIB == TRUE (suppression of positive response): */
    if(pDiagContext->MsgContext.msgAddInfo.suppressPosResponse != 0u)
    {
      /* Mark no response to be sent */
      pDiagContext->DoSuppressResponse = TRUE;                                                                                                       /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
    }/* Otherwise - proceed with positive response transmission */
  }
  else /* Otherwise (negative response): */
  {
    /* If functionally requested diagnostic job: */
    if(pDiagContext->MsgContext.msgAddInfo.reqType != 0u)
    {
      switch(pDiagContext->ErrorRegister)
      {
        case DCM_E_SERVICENOTSUPPORTED:
        case DCM_E_SUBFUNCTIONNOTSUPPORTED:
        case DCM_E_REQUESTOUTOFRANGE:
#if (DCM_DIAG_SUPPRESS_NRC_7F_7E_ENABLED == STD_ON)
        case DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION:
        case DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION:
#endif
          /* Suppress negative response for certain NRCs only */
          pDiagContext->DoSuppressResponse = TRUE;                                                                                                   /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
          break;
        default:
          /* Otherwise - proceed with negative response transmission */
          break;
      }
    }/* Otherwise - proceed with negative response transmission */
    /* Update negative response code IdsM events */
    Dcm_DebugReportSecurityEventNRC(threadId, pDiagContext->ErrorRegister);
  }
  /* Update diagnostic sub-component activity state */
  pDiagContext->State = DCM_DIAG_STATE_ON_TX;                                                                                                        /* SBSW_DCM_POINTER_WRITE_THREADCONTEXT */

  /* Notify the transmission task for linear data response transmission/suppression */
  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_LINEAR, threadId);
}

/**********************************************************************************************************************
 *  Dcm_DiagSetNegResponse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagSetNegResponse(
  Dcm_NegativeResponseCodeType nrc,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);

  if(pDiagContext->ErrorRegister == DCM_E_POSITIVERESPONSE)
  {
    pDiagContext->ErrorRegister = nrc;                                                                                                               /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
  }
}

/**********************************************************************************************************************
 *  Dcm_DiagCancelProcessing()
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
FUNC(void, DCM_CODE) Dcm_DiagCancelProcessing(
  Dcm_ThreadIdMemType threadId
  )
{
#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  if((DCM_DIAG_QUEUE_FLAG_IS_WAITING & Dcm_DiagGetThreadContext(threadId)->QueuedSet.Flags) == 0u)
#endif
  {
    Dcm_TmrStopTimerByThread(DCM_TMR_ID_P2, threadId);

    /* Cancel any ongoing RCR-RP transmission */
    Dcm_TskClrEventByThread(DCM_TSK_ID_DIAG_RX, DCM_TSK_EV_DIAG_RX_TMR_P2_TO, threadId);
    Dcm_TskClrEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_RCRRP, threadId);
  }

  Dcm_TskKillAllTasks(threadId);
  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_KILL_JOBS, threadId);
}

/**********************************************************************************************************************
 *  Dcm_OnTimeoutP2()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutP2(
  Dcm_ThreadIdMemType threadId                                                                                                                       /* COV_DCM_RTM_NO_RCR_RP_TX */
  )
{
  Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_RX, DCM_TSK_EV_DIAG_RX_TMR_P2_TO, threadId);
  return 0u;
}

/**********************************************************************************************************************
 *  Dcm_OnTimeoutS3()
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
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutS3(                                                                                             /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ThreadIdMemType threadId
  )
{
  if(!Dcm_StateIsDefaultSessionActive())
  {
    Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF, threadId);
    Dcm_DiagSetP2Timings(0, Dcm_NetGetProtIdOfActiveProtocol(), threadId);

#if (DCM_AUTHMGR_APCE_INTERNAL_ENABLED == STD_ON)
    {
      Dcm_CfgNetAuthInfoRefMemType lAuthInfoRef = Dcm_NetPbCfgGetConnAuthInfoRef(Dcm_NetGetSessionConnection());

      if (lAuthInfoRef != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
      {
        if (Dcm_AuthMgrAuthenticationState(lAuthInfoRef) == DCM_AUTHENTICATED)
        {
          Dcm_AuthMgrTimerExpireMaskSetBit(lAuthInfoRef);
          Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_IDLE_CONN, threadId);
        }
      }
    }
#endif
  }
  return 0u;
}

#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutFblRes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutFblRes(
  Dcm_ThreadIdMemType threadId
  )
{
  DCM_IGNORE_UNREF_PARAM(threadId);
  Dcm_TskSetEvent(DCM_TSK_ID_DIAG_FBLRES, DCM_TSK_EV_DIAG_FBLRES_CANCEL);
  return 0u;
}
#endif

#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutKeepAlive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutKeepAlive(
  Dcm_ThreadIdMemType threadId
  )
{
  DCM_IGNORE_UNREF_PARAM(threadId);

  if(Dcm_StateIsDefaultSessionActive())                                                                                                              /* COV_DCM_RTM_DEPENDS_ON_CFG */
  {
    Dcm_NetUnRegisterAllComMActivity();
  } /* else - ComM activity will be unregistered by S3 timeout */

  Dcm_NetSetKeepAlive(FALSE);

  return 0u; /* No reload */
}
#endif

/**********************************************************************************************************************
 *  Dcm_DiagGetProcessorState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_DiagProcessorStateType, DCM_CODE) Dcm_DiagGetProcessorState(
  Dcm_ThreadIdOptType threadId
  )
{
  return Dcm_DiagGetThreadContext(threadId)->State;
}

/**********************************************************************************************************************
 *  Dcm_DiagSetProcessorState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DiagSetProcessorState(
  Dcm_ThreadIdOptType threadId,
  Dcm_DiagProcessorStateType state
  )
{
  Dcm_DiagThreadContextPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);

  pDiagContext->State = state;                                                                                                                       /* SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT */
}

/**********************************************************************************************************************
 *  Dcm_DiagGetMsgContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_MsgContextPtrType, DCM_CODE) Dcm_DiagGetMsgContext(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_DiagGetThreadContext(threadId)->MsgContext;
}

/**********************************************************************************************************************
 *  Dcm_DiagGetServiceInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgDiagServiceInfoPtrType, DCM_CODE) Dcm_DiagGetServiceInfo(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_DiagThreadContextConstPtrType pDiagContext = Dcm_DiagGetThreadContext(threadId);

  return Dcm_CfgWrapDiagServiceInfo(pDiagContext->SidIndex);
}

/**********************************************************************************************************************
 *  Dcm_DiagGetSid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_DiagGetSid(
  Dcm_ThreadIdOptType threadId
  )
{
  return Dcm_DiagGetThreadContext(threadId)->PostProcessorContext.Sid;
}

/**********************************************************************************************************************
 *  Dcm_DiagIsExternallyHandled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_DiagIsExternallyHandled(
  Dcm_ThreadIdOptType threadId
  )
{
  return Dcm_DiagGetThreadContext(threadId)->ProcessServiceExtern;
}

/**********************************************************************************************************************
 *  Dcm_DiagGetQueuedSetFlags()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_DiagProcessorFlagType, DCM_CODE) Dcm_DiagGetQueuedSetFlags(
  Dcm_ThreadIdOptType threadId
  )
{
  return Dcm_DiagGetThreadContext(threadId)->QueuedSet.Flags;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_ExternalProcessingDone()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ExternalProcessingDone(
  Dcm_MsgContextPtrType pMsgContext                                                                                                                  /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(pMsgContext == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_DiagProcessingDone(pMsgContext->threadId);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_EXT_PROCESSINGDONE, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_ExternalSetNegResponse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ExternalSetNegResponse(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodeType errorCode
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(pMsgContext == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_DiagSetNegResponse(errorCode, pMsgContext->threadId);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_EXT_SETNEGRESPONSE, lErrorId);
}

#if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProvideRecoveryStates()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ProvideRecoveryStates(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo
  )
{
  Std_ReturnType  lResult  = DCM_E_NOT_OK;
  uint8           lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(RecoveryInfo == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_DiagProvideRecoveryStates(RecoveryInfo);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    lResult = DCM_E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_PROVIDERECOVERYSTATES, lErrorId);

  return lResult;
}
#endif

#if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetSpecificCauseCode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetSpecificCauseCode(
  Dcm_SpecificCauseCodeType specificCauseCode
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_DiagSetSpecificCauseCode(specificCauseCode);
    lResult = DCM_E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETSPECIFICCAUSECODE, lErrorId);

  return lResult;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Diag.c
 *********************************************************************************************************************/
