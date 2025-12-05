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
/**        \file  Dcm_Diag.h
 *         \unit  Diag
 *        \brief  Contains public types, function declarations and inline function definitions of Diag unit.
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
#if !defined(DCM_DIAG_H)
# define DCM_DIAG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_DiagTypes.h"
# include "Dcm_VarMgr.h"
# include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Diagnostic entity evaluation level */
# define DCM_DIAG_CHK_LVL_SERVICE_ID                                 ((Dcm_DiagCheckLvlType)0u) /*!< Evaluation on diagnostic service ID level */
# define DCM_DIAG_CHK_LVL_PARAMETER                                  ((Dcm_DiagCheckLvlType)1u) /*!< Evaluation on diagnostic parameter ID level */
# define DCM_DIAG_CHK_LVL_SUBFUNC                                    ((Dcm_DiagCheckLvlType)2u) /*!< Evaluation on diagnostic sub-function ID level */

/*! Total number of diagnostic entities that can be verified (use always the one with highest ID as reference) */
# define DCM_DIAG_NUM_CHK_LVLS                                       (DCM_DIAG_CHK_LVL_SUBFUNC + 1u)

/*! Bitmap encoding for the enumerator values below (used to build the AR standard confirmation status values e.g. DCM_RES_POS_OK !) */
# define DCM_DIAG_RES_ANY_OK                                         ((Dcm_ConfirmationStatusType)0x00u) /*!< The response (positive/negative) was successfully sent */
# define DCM_DIAG_RES_ANY_NOT_OK                                     ((Dcm_ConfirmationStatusType)0x01u) /*!< The response (positive/negative) transmission failed */
# define DCM_DIAG_RES_POS_ANY                                        ((Dcm_ConfirmationStatusType)0x00u) /*!< It is a positive response */
# define DCM_DIAG_RES_NEG_ANY                                        ((Dcm_ConfirmationStatusType)0x02u) /*!< It is a negative response */

/*! Diagnostic kernel request queued flags. These are used to keep track of any overlapping diagnostic request in processing with a new one just won a request prioritization
    Especially in case of split-task this mechanism provides a look-ahead whether a new request is already queue to be processed within the low-priority Dcm_DiagTaskWorker() */
# define DCM_DIAG_QUEUE_FLAG_NONE                                    ((Dcm_DiagProcessorFlagType)0x00u) /*!< There is no diagnostic request queued to be processed */
# define DCM_DIAG_QUEUE_FLAG_IS_WAITING                              ((Dcm_DiagProcessorFlagType)0x01u) /*!< There is already a diagnostic request queued (waiting) to be processed */
# define DCM_DIAG_QUEUE_FLAG_IS_ACTIVE                               ((Dcm_DiagProcessorFlagType)0x02u) /*!< There diagnostic request in started processing */

/*! The diagnostic sub-component internal states
  Note: Bit-mapped states chose, since multiple states can be active at a time (e.g. an still in processing and on transmission for paged-buffer responses) */
# define DCM_DIAG_STATE_IDLE                                         ((Dcm_DiagProcessorStateType)0x00u) /*!< No diagnostic service processing in progress (yet) */
# define DCM_DIAG_STATE_PROCESS                                      ((Dcm_DiagProcessorStateType)0x01u) /*!< A diagnostic request is in processing */
# define DCM_DIAG_STATE_ON_TX                                        ((Dcm_DiagProcessorStateType)0x02u) /*!< A diagnostic response is on transmission */
# define DCM_DIAG_STATE_POST_PROCESS                                 ((Dcm_DiagProcessorStateType)0x04u) /*!< A diagnostic request is in the post-processing phase */

/*! Diagnostic service specific properties */
# define DCM_DIAG_SVC_CFG_PROP_HAS_SUBFUNC                           ((DcmCfg_DiagServicePropertiesType)0x01u) /*!< Has a sub-function parameter */
# define DCM_DIAG_SVC_CFG_PROP_CALL_POST_HDLR_ALWAYS                 ((DcmCfg_DiagServicePropertiesType)0x02u) /*!< Requires the post-handler (Dcm_ServiceXXPostHandler()) to be called always once the service evaluation has started */
# define DCM_DIAG_SVC_CFG_PROP_CALL_SVC_DISPATCHER                   ((DcmCfg_DiagServicePropertiesType)0x04u) /*!< Requires a service dispatching (internal and external service processing) */
# define DCM_DIAG_SVC_CFG_PROP_NEEDS_LOCK                            ((DcmCfg_DiagServicePropertiesType)0x08u) /*!< Requires a service lock */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Getter for the diagnostic session specific P2 timings */
# define Dcm_DiagGetP2Time(sesStateIdx)                              (Dcm_CfgWrapStateSessionInfo((sesStateIdx))->P2ServerTime.P2)                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_DiagGetP2StarTime(sesStateIdx)                          (Dcm_CfgWrapStateSessionInfo((sesStateIdx))->P2ServerTime.P2Star)               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! A sub-service length (i.e. sub-function and eventually some data byte) must always fit the linear buffer size (no paged-buffer request reception) */
typedef Dcm_CfgNetBufferSizeMemType Dcm_DiagSubServiceLengthType;
typedef P2VAR(Dcm_DiagSubServiceRefOptType, TYPEDEF, DCM_VAR_NOINIT)  Dcm_DiagSubServiceRefOptPtrType;

/*! Function pointer prototype for a diagnostic service specific length getter */
typedef Dcm_DiagSubServiceLengthType(*Dcm_DiagServiceLengthGetterFuncType) (
  Dcm_DiagSubServiceRefOptType pSubSvcRef  /* IN: Abstract (diagnostic service processor specific) sub-function handle */
  );

/*! Function pointer prototype for a diagnostic service with sub-function specific sequence checker */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DiagSubFuncSequenceCheckerFuncType) (
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DiagSubServiceRefOptType pSubSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Base data type for the diagnostic sub-component internal state machine */
typedef uint8 Dcm_DiagProcessorStateType;

/*! Base data type for the application notification levels */
typedef uint8 Dcm_DiagApplNotificationType;

/*! Base data type for the diagnostic kernel processing state flags */
typedef uint8 Dcm_DiagProcessorFlagType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DiagRxIndication()
 *********************************************************************************************************************/
/*! \brief          Called once an incoming reception is finished.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM external/internal connection
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagRxIndication(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_DiagInit()
 *********************************************************************************************************************/
/*! \brief          Initializes DCM diagnostic sub-module.
 *  \details        -
 *  \param[in]      configPtr    Pointer to a concrete configuration root
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts must be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagInit(
  Dcm_ConfigPtrType configPtr
  );

/**********************************************************************************************************************
 *  Dcm_DiagIsKernelBusy()
 *********************************************************************************************************************/
/*! \brief          Checks if the diagnostic kernel is busy with a diagnostic job on any thread.
 *  \details        -
 *  \return         TRUE     Diagnostic kernel is busy
 *  \return         FALSE    Diagnostic kernel is not busy
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_DiagIsKernelBusy(
  void
  );

/**********************************************************************************************************************
 *  Dcm_DiagIsThreadBusy()
 *********************************************************************************************************************/
/*! \brief          Checks if the diagnostic kernel is busy with a diagnostic job on a specific thread.
 *  \details        -
 *  \param[in]      threadId Reference to the variant configuration
 *  \return         TRUE     Diagnostic kernel is busy
 *  \return         FALSE    Diagnostic kernel is not busy
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_DiagIsThreadBusy(
  Dcm_ThreadIdOptType threadId
  );

# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsEnabledInActiveVariants()
 *********************************************************************************************************************/
/*! \brief          Checks if a diagnostic object is active in at least one variant.
 *  \details        -
 *  \param[in]      cfgVariantRef    Reference to the variant configuration
 *  \return         TRUE             Diagnostic object is active in at least one variant
 *  \return         FALSE            Diagnostic object is not active
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_DiagIsEnabledInActiveVariants(
  Dcm_CfgStateRefOptType cfgVariantRef
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DiagVariantLookUpFilter()
 *********************************************************************************************************************/
/*! \brief          Performs a lookup result filtering.
 *  \details        Checks whether a diagnostic object is supported in the active variant.
 *  \param[in]      lookUpTable          Pointer to the table to be scanned (first element is table-size!)
 *  \param[in]      lookUpFilterTable    List of filters
 *  \param[in]      value                Value to be found
 *  \return         -1     Diagnostic entity not active in current variant
 *  \return         >=0    Success, index of the matched position
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(sint16_least, DCM_CODE) Dcm_DiagVariantLookUpFilter(
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  uint8 value
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetDemClientId()
 *********************************************************************************************************************/
/*! \brief          Get DEM Client Id.
 *  \details        -
 *  \param[in]      pMsgContext     Message-related information for one diagnostic protocol identifier 
 *  \return         DEM Client Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_DiagGetDemClientId(
  Dcm_ReadOnlyMsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagSvcWithOnlySubFuncReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Default sub-function length getter.
 *  \details        Always returns length of 1 byte (sub-function byte only expected)
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length (1 byte only)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_DiagSvcWithOnlySubFuncReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );

/**********************************************************************************************************************
 *  Dcm_DiagNoSequenceChecker()
 *********************************************************************************************************************/
/*! \brief          Default sub-function sequence checker.
 *  \details        Always returns DCM_E_OK.
 *  \param[in,out]  pMsgContext          The current request context
 *  \param[in]      subSvcRef            The sub-function zero-based index.
 *  \param[out]     ErrorCode            The NRC
 *  \return         DCM_E_OK             Validation passed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagNoSequenceChecker(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagVerifyServiceWithSubFunction()
 *********************************************************************************************************************/
/*! \brief          Verifies a service with sub-function.
 *  \details        Implements the ISO14229-1:2013 algorithm for sub-function validation
 *  \param[in,out]  pMsgContext            Current request context
 *  \param[in]      lookUpTable            The sub-service lookup table
 *  \param[in]      lookUpFilterTable      List of sub-service related filters
 *  \param[in]      svcLengthGetter        Functor for accessing a sub-service specific length
 *  \param[in]      subFuncSequenceChecker Function for accessing a service specific sequence check
 *  \param[out]     pSubSvcRef             A zero-based sub-service index for further processing
 *  \param[out]     ErrorCode              The NRC
 *  \return         DCM_E_OK               Validation passed
 *  \return         DCM_E_NOT_OK           Validation failed with the returned ErrorCode
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagVerifyServiceWithSubFunction(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  Dcm_DiagServiceLengthGetterFuncType svcLengthGetter,
  Dcm_DiagSubFuncSequenceCheckerFuncType subFuncSequenceChecker,
  Dcm_DiagSubServiceRefOptPtrType pSubSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetP2Timings()
 *********************************************************************************************************************/
/*! \brief          Sets the adjusted P2 timings.
 *  \details        -
 *  \param[in]      sesStateIdx    The index of the current session.
 *  \param[in]      protocolIdx    The index of the current protocol.
 *  \param[in]      threadId       Active thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagSetP2Timings(
  Dcm_StateIndexMemType sesStateIdx,
  Dcm_NetProtRefMemType protocolIdx,
  Dcm_ThreadIdMemType threadId
  );

# if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsSessionChangeAllowed()
 *********************************************************************************************************************/
/*! \brief          Checks whether any other thread is currently busy with a parallel request.
 *  \details        -
 *  \param[in]      threadId        Id of the thread
 *  \return         DCM_E_OK        ECU is idle, session change allowed
 *  \return         DCM_E_NOT_OK    Another thread is busy, session change is not allowed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagIsSessionChangeAllowed(
  Dcm_ThreadIdMemType threadId
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DiagCancelProcessing()
 *********************************************************************************************************************/
/*! \brief          Cancel any ongoing job.
 *  \details        This function cancels any ongoing service processing.
 *  \param[in]      threadId  Active thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagCancelProcessing(
  Dcm_ThreadIdMemType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetNegResponse()
 *********************************************************************************************************************/
/*! \brief          Registers a NRC to the request in progress if no other NRC already set.
 *  \details        -
 *  \param[in]      nrc         The error code to be registered
 *  \param[in]      threadId    Active thread
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagSetNegResponse(
  Dcm_NegativeResponseCodeType nrc,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagProcessingDone()
 *********************************************************************************************************************/
/*! \brief          Finalizes the service job processing.
 *  \details        -
 *  \param[in]      threadId    Active thread
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagProcessingDone(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagAcceptNewRequest()
 *********************************************************************************************************************/
/*! \brief          Checks whether a new request will be accepted.
 *  \details        -
 *  \param[in]      pTranspObj    Currently active DCM USDT connection
 *  \return         TRUE          New request is accepted
 *  \return         FALSE         New request is not accepted
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_DiagAcceptNewRequest(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_DiagOnTxFinished()
 *********************************************************************************************************************/
/*! \brief          Notifies the diagnostic sub-module that an ongoing transmission just finished.
 *  \details        Called by the TP once an ongoing transmission is finished.
 *  \param[in]      pTranspObj    Currently active DCM USDT connection
 *  \param[in]      txStatus      Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagOnTxFinished(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType txStatus
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetTranspObj()
 *********************************************************************************************************************/
 /*! \brief          Returns the corresponding transport object by the given thread id.
  *  \details        -
  *  \param[in]      threadId    The active thread identifier
  *  \return         Valid transport object
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
FUNC(Dcm_NetTransportObjectPtrType, DCM_CODE) Dcm_DiagGetTranspObj(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetQueuedTranspObj()
 *********************************************************************************************************************/
 /*! \brief          Returns the queued transport object by the given thread id.
  *  \details        -
  *  \param[in]      threadId    The active thread identifier
  *  \return         Valid transport object
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
FUNC(Dcm_NetTransportObjectPtrType, DCM_CODE) Dcm_DiagGetQueuedTranspObj(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetTranspObj()
 *********************************************************************************************************************/
 /*! \brief          Assigns the given transport object to the given thread id.
  *  \details        -
  *  \param[in]      threadId      The active thread identifier
  *  \param[in]      pTranspObj    Currently active DCM USDT connection
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagSetTranspObj(
  Dcm_ThreadIdOptType threadId,
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetProcessorState()
 *********************************************************************************************************************/
 /*! \brief          Returns the diagnostic sub-component internal state.
  *  \details        -
  *  \param[in]      threadId    The active thread identifier
  *  \return         The diagnostic sub-component internal state
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
FUNC(Dcm_DiagProcessorStateType, DCM_CODE) Dcm_DiagGetProcessorState(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagSetProcessorState()
 *********************************************************************************************************************/
 /*! \brief          Sets the diagnostic sub-component internal state.
  *  \details        -
  *  \param[in]      threadId    The active thread identifier
  *  \param[in]      state       The new processor state
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagSetProcessorState(
  Dcm_ThreadIdOptType threadId,
  Dcm_DiagProcessorStateType state
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetMsgContext()
 *********************************************************************************************************************/
 /*! \brief          Returns the diagnostic service/sub-service processor message context.
  *  \details        -
  *  \param[in]      threadId    The active thread identifier
  *  \return         The diagnostic service/sub-service processor message context
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
FUNC(Dcm_MsgContextPtrType, DCM_CODE) Dcm_DiagGetMsgContext(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetServiceInfo()
 *********************************************************************************************************************/
/*! \brief          Returns the active service info descriptor.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         The service info descriptor of the active thread
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgDiagServiceInfoPtrType, DCM_CODE) Dcm_DiagGetServiceInfo(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetSid()
 *********************************************************************************************************************/
/*! \brief          Returns the identifier of the currently processed service.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         The service identifier
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_DiagGetSid(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagIsExternallyHandled()
 *********************************************************************************************************************/
/*! \brief          Returns whether the alternative service processor shall be called or the internal one.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         TRUE     The external service processor shall be called
 *  \return         FALSE    The internal service processor shall be called
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_DiagIsExternallyHandled(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetQueuedSetFlags()
 *********************************************************************************************************************/
/*! \brief          Returns the processing flags of the diagnostic kernel queue.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         The flags of the queued set
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_DiagProcessorFlagType, DCM_CODE) Dcm_DiagGetQueuedSetFlags(
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

#endif /* !defined(DCM_DIAG_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Diag.h
 *********************************************************************************************************************/
