/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Csm.c
 *        \brief  MICROSAR Crypto Service Manager (CSM)
 *
 *      \details  Implementation of the MICROSAR Crypto Service Manager (CSM)
 *
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

#define CSM_SOURCE
/* PRQA S 6060 EOF */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Csm.h"
#include "Csm_Cbk.h"
#include "CryIf.h"
#include "SchM_Csm.h"

#if (CSM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

 /* Check the version of CSM header file */
#if ((CSM_SW_MAJOR_VERSION != (7u)) \
    || (CSM_SW_MINOR_VERSION != (2u)) \
    || (CSM_SW_PATCH_VERSION != (1u)))
# error "Vendor specific version numbers of Csm.c and Csm.h are inconsistent"
#endif

/* AUTOSAR version information check has to match definition in header file */
#if ((CSM_AR_RELEASE_MAJOR_VERSION      != (4u)) \
    || (CSM_AR_RELEASE_MINOR_VERSION    != (7u)) \
    || (CSM_AR_RELEASE_REVISION_VERSION != (0u)))
# error "AUTOSAR Specification Version numbers of Csm.c and Csm.h are inconsistent!"
#endif

/* Check the version of the configuration header file to ensure that the generated files match the static files. */
#if (  (CSM_CFG_MAJOR_VERSION != (7u)) \
    || (CSM_CFG_MINOR_VERSION != (2u)) \
    || (CSM_CFG_PATCH_VERSION != (1u)) )
# error "Version numbers of Csm.c and Csm_Cfg.h are inconsistent!"
#endif

/* Check the version of the generator. */
#if (  (CSM_GENERATOR_MAJOR_VERSION != (7u)) \
    || (CSM_GENERATOR_MINOR_VERSION != (2u)) )
# error "Version numbers of Csm.c and Csm_Cfg.h (generator version) are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Callback is idle and must not be notified to RTE. */
#define CSM_CALLBACK_IDLE                                             (0xFEu)

/*! Job ID which is used to mark Job as empty/free for usage. */
#define CSM_EMPTY_JOB                                                 (0xFFFFFFFFu)

/* States used for internal job state of Csm. */

/*! Job is IDLE - Read to be used by application */
#define CSM_JOB_STATE_IDLE                                            CRYPTO_JOBSTATE_IDLE

/*! Job is Active - That means was started but waits for a further UPDATE or the FINISH call */
#define CSM_JOB_STATE_ACTIVE                                          CRYPTO_JOBSTATE_ACTIVE

/*! Job is currently progressing in lower layer - Csm is waiting for function to return or for callback notification. */
#define CSM_JOB_STATE_PROGRESSING                                     CRYPTO_JOBSTATE_PROGRESSING

/*! An asynchronous job was passed to lower layer - Csm is waiting for the callback, job is now cancelable as known by
 *  lower layer */
#define CSM_JOB_STATE_WAITING                                         3u

/*! Asynchronous job is on QUEUE */
#define CSM_JOB_STATE_QUEUED                                          4u

/*! Job canceling pending - currently the job is tried to be canceled in lower layer */
#define CSM_JOB_STATE_CANCELING                                       5u

/*! Asynchronous Job was cancelled in lower layer but CallbackNotification is required to complete cancellation. */
#define CSM_JOB_STATE_CANCELED                                        6u

/*! Asynchronous job's callback has occurred  */
#define CSM_JOB_STATE_CALLBACK_OCCURRED                               64u

/*! Clear mask for state Asynchronous job's callback has occurred */
#define CSM_JOB_STATE_CALLBACK_OCCURRED_CLEAR_MASK                    191u /* = ~CSM_JOB_STATE_CALLBACK_OCCURRED */

/*! State of asynchronous job retriggering  */
#define CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE                            (0x00u)
#define CSM_ASYNC_JOB_RETRIGGER_STATE_PENDING                         (0x01u)

/*! Determine which return value shall be used if queue is full.
 *  Since ASR 4.4, CRYPTO_E_BUSY is used instead of CRYPTO_E_QUEUE_FULL. */
#if !defined (CSM_QUEUE_FULL_RETVAL) /* COV_CSM_ASR_COMPATIBILITY_DEFINE */
# define CSM_QUEUE_FULL_RETVAL                                        CRYPTO_E_BUSY
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (CSM_LOCAL) /* COV_CSM_LOCAL_DEFINE */
# define CSM_LOCAL                                                    static
#endif

#if !defined (CSM_LOCAL_INLINE) /* COV_CSM_LOCAL_DEFINE */
# define CSM_LOCAL_INLINE                                             LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CSM_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CSM_JOB == STD_ON)
/**********************************************************************************************************************
 *  Csm_ProcessJob()
 *********************************************************************************************************************/
/*! \brief         Process the received job
 *  \details       This function unifies all external calls to call CryIf_ProcessJob.
 *  \param[in,out] job                         Pointer to the configuration of the job. Contains structures with user and
 *                                             primitive relevant information.
 *  \param[in]     partition                   Id of the current active partition.
 *  \return        E_OK                        Request successful
 *                 E_NOT_OK                    Request failed
 *                 CRYPTO_E_BUSY               Request failed, service is busy or queue is full
 *                 CRYPTO_E_KEY_READ_FAIL      Request failed, not allowed to extract key element
 *                 CRYPTO_E_KEY_WRITE_FAIL     Request failed, not allowed to write key element
 *                 CRYPTO_E_KEY_NOT_VALID      Request failed, the key's state is "invalid"
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element sizes are not compatible
 *                 CRYPTO_E_KEY_EMPTY          Request failed because of uninitialized source key element
 *                 CRYPTO_E_SMALL_BUFFER       The provided buffer is too small to store the result
 *                 CRYPTO_E_ENTROPY_EXHAUSTION Request failed, entropy of random number generator is exhausted
 *  \pre           Param queueIdx needs to be a valid index.
 *  \pre           Job must point to a valid job object.
 *  \pre           CSM_EXCLUSIVE_AREA_0 must be entered as it is left inside this function.
 *  \pre           Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_ProcessJob(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition);

/**********************************************************************************************************************
 *  Csm_GetJobObj()
 *********************************************************************************************************************/
/*! \brief         Fetches the job object for the given job id.
 *  \details       Job has either a dedicated object or it is received from the job pool
 *  \param[in]     jobId                   Holds the identifier of the job.
 *  \param[in]     partition               Id of the current active partition.
 *  \param[out]    jobObjId                Pointer which will hold the id of the job object to use.
 *  \return        E_OK                    Request successful.
 *                 E_NOT_OK                Request failed.
 *                 CRYPTO_E_BUSY           Request failed, Crypto Driver Object is busy or queue is full.
 *  \pre           Should only be called with locked interrupts
 *  \pre           Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_GetJobObj(
  uint32 jobId,
  Csm_SizeOfPartitionIdentifiersType partition,
  P2VAR(Csm_SizeOfJobType, AUTOMATIC, AUTOMATIC) jobObjId);

# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
/**********************************************************************************************************************
 *  Csm_PrepareJobObj()
 *********************************************************************************************************************/
/*! \brief         Set all fields of the given job structure to the data of the given job id.
 *  \details       -
 *  \param[in]     jobId                   Holds the identifier of the job.
 *  \param[out]    job                     Pointer to a job which will be adapted to the given job id.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_PrepareJobObj(
  uint32 jobId,
  P2VAR(Crypto_JobType, AUTOMATIC, AUTOMATIC) job);
# endif /* (CSM_JOBSHARINGOFQUEUEINFO == STD_ON) */

# if (CSM_ASYNC_PROCESSING == STD_ON)
/**********************************************************************************************************************
 *  Csm_EnqueueJob()
 *********************************************************************************************************************/
/*! \brief         Enqueue Job.
 *  \details       This function is enqueuing a job to the given queue directly sorting by its priority.
 *  \param[in]     queueIdx                Holds the identifier of the queue info.
 *  \param[in,out] job                     Pointer to a job that shall be enqueued.
 *  \param[in]     partition               Id of the current active partition.
 *  \return        E_OK                    Request successful.
 *                 CRYPTO_E_BUSY           Request failed, the queue is full.
 *  \pre           Param queueIdx needs to be a valid index.
 *  \pre           Job must point to a valid job object.
 *  \pre           Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \note          Shall only be called with locked interrupts
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_EnqueueJob(
  Csm_QueueInfoIterType queueIdx,
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition);

/**********************************************************************************************************************
 *  Csm_DequeueJob()
 *********************************************************************************************************************/
/*! \brief         Dequeue Job.
 *  \details       This function removes a given job id from the queue filling gaps.
 *  \param[in]     queueIdx                Holds the identifier of the queue.
 *  \param[in]     jobId                   Id of the job that shall be removed.
 *  \param[in]     partition               Id of the current active partition.
 *  \pre           Param queueIdx needs to be a valid index.
 *  \pre           Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \note          Shall only be called with locked interrupts
 *  \context       TASK|ISR
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(void, CSM_CODE) Csm_DequeueJob(
  Csm_QueueInfoIterType queueIdx,
  uint32 jobId,
  Csm_SizeOfPartitionIdentifiersType partition);

/**********************************************************************************************************************
 *  Csm_TriggerAsynchJobProcessing()
 *********************************************************************************************************************/
/*! \brief        Triggers the Processing of asynchronous jobs
 *  \details      This function checks for the given queue if an asynchronous job is available. If so, it triggers the
 *                CryIf to process this asynchronous job.
 *  \param[in]    queueIdx  Holds the identifier of the queue.
 *  \pre          Param queueIdx needs to be a valid index.
 *                Must be called within CSM_EXCLUSIVE_AREA_0.
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(void, CSM_CODE) Csm_TriggerAsynchJobProcessing(Csm_QueueInfoIterType queueIdx);

/**********************************************************************************************************************
 *  Csm_HandleCryIfCancelResultInStateCanceling()
 *********************************************************************************************************************/
/*! \brief          Handles result of CryIf_CancelJob is job state in CANCELING.
 *  \details        Depending on CryIf_CancelJob result, the necessary steps for canceling an asynchronous jobs are executed.
 *  \param[in]      partition               Id of the current active partition.
 *  \param[in]      jobId                   Holds the identifier of the job.
 *  \param[in]      cryIfRetVal             Return value of CryIf_CancelJob
 *  \param[in]      cancelFailedState       Job state which shall be entered if cancel failed
 *  \return         TRUE                    Notify via callback
 *                  FALSE                   Do not notify via callback
 *  \pre            CSM_EXCLUSIVE_AREA_0 must be entered as is is left inside this function.
 *  \pre            Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \pre            Param jobId needs to be a valid index.
 *  \pre            Job state must be CANCELING
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(boolean, CSM_CODE) Csm_HandleCryIfCancelResultInStateCanceling(
  Csm_SizeOfPartitionIdentifiersType partition,
  uint32 jobId,
  Std_ReturnType cryIfRetVal,
  uint8 cancelFailedState);

#  if (CSM_CANCELATION_DURING_PROCESSING == STD_ON)
/**********************************************************************************************************************
 *  Csm_HandleCryIfCancelResultStateHasChanged()
 *********************************************************************************************************************/
/*! \brief          Handles result of CryIf_CancelJob if job state has changed.
 *  \details        Depending on CryIf_CancelJob result, the necessary steps for canceling an asynchronous jobs are executed.
 *  \param[in]      partition               Id of the current active partition.
 *  \param[in]      jobId                   Holds the identifier of the job.
 *  \param[in]      cryIfRetVal             Return value of CryIf_CancelJob
 *  \pre            CSM_EXCLUSIVE_AREA_0 must be entered as is is left inside this function.
 *  \pre            Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \pre            Param jobId needs to be a valid index.
 *  \pre            Job state must not be CANCELING
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleCryIfCancelResultStateHasChanged(
  Csm_SizeOfPartitionIdentifiersType partition,
  uint32 jobId,
  Std_ReturnType cryIfRetVal);
#  endif /* (CSM_CANCELATION_DURING_PROCESSING == STD_ON) */

/**********************************************************************************************************************
 *  Csm_CancelAsynchronousJob()
 *********************************************************************************************************************/
/*! \brief          Cancels an asynchronous job
 *  \details        Depending on job state, the necessary steps for canceling an asynchronous jobs are executed
 *  \param[in,out]  job                     Holds the asynchronous job object which shall be cancelled
 *  \param[in]      partition               Id of the current active partition.
 *  \return         E_OK                    Request successful - Job was cancelled or was already Idle
 *                  E_NOT_OK                Request failed - Job could not be cancelled
 *                  CRYPTO_E_JOB_CANCELED   Request pending - Job will be cancelled with next callback notification
 *  \pre            CSM_EXCLUSIVE_AREA_0 must be entered as is is left inside this function.
 *  \pre            Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_CancelAsynchronousJob(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition);

/**********************************************************************************************************************
 *  Csm_HandleCallbackNotification()
 *********************************************************************************************************************/
/*! \brief         Handle the Callback Notification from the lower layer
 *  \details       Handle the result of an asynchronous job and forwards the notification to the application.
 *  \param[in,out] job                Holds a pointer to the job which has been finished.
 *  \param[in]     result             Contains the result of the cryptographic operation.
 *  \param[out]    errorIdPtr         Pointer to store the ID of the detected development error.
 *  \param[out]    runtimeErrorIdPtr  Pointer to store the ID of the detected runtime development error.
 *  \pre           All pointers must be valid.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleCallbackNotification(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Crypto_ResultType result,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorIdPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) runtimeErrorIdPtr);

/**********************************************************************************************************************
 *  Csm_HandleApplicationCallback()
 *********************************************************************************************************************/
/*! \brief          Handles the application callback.
 *  \details        Calls the configured user callback function.
 *  \param[in,out]  job       Holds a pointer to the job, which has finished.
 *  \param[in]      result    Contains the result of the cryptographic operation.
 *  \param[out]     errorId   Pointer to a buffer where the ID of the detected development error shall be written to.
 *  \param[in]      partition Id of the current active partition.
 *  \pre            CSM_EXCLUSIVE_AREA_0 must be entered as it is left inside this function.
 *  \context        TASK, ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleApplicationCallback(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Std_ReturnType result,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  Csm_SizeOfPartitionIdentifiersType partition);

#  if (CSM_RTECALLBACK == STD_ON)
/**********************************************************************************************************************
 *  Csm_HandleRteCallbacks()
 *********************************************************************************************************************/
/*! \brief        Handles the RTE callbacks.
 *  \details      Calls the RTE callback functions.
 *  \param[in]    partition    The current partition index.
 *  \pre          Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleRteCallbacks(Csm_SizeOfPartitionIdentifiersType partition);
#  endif /* (CSM_RTECALLBACK == STD_ON) */

# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */

/**********************************************************************************************************************
 *  Csm_HandleJobProcessing()
 *********************************************************************************************************************/
/*! \brief         Triggers the CryIf to process the given job.
 *  \details       This function triggers the CryIf to process the corresponding job. Furthermore, pre- and post-job
 *                 callouts are called from this function if enabled.
 *  \param[in]     channelId           Holds the identifier of the queue.
 *  \param[in,out] job                 Pointer to the corresponding job
 *  \param[in]     partition           Id of the current active partition.
 *  \return        E_OK                        Request successful
 *                 E_NOT_OK                    Request failed
 *                 CRYPTO_E_BUSY               Request failed, service is busy or queue is full
 *                 CRYPTO_E_KEY_READ_FAIL      Request failed, not allowed to extract key element
 *                 CRYPTO_E_KEY_WRITE_FAIL     Request failed, not allowed to write key element
 *                 CRYPTO_E_KEY_NOT_VALID      Request failed, the key's state is "invalid"
 *                 CRYPTO_E_KEY_SIZE_MISMATCH  Request failed, key element sizes are not compatible
 *                 CRYPTO_E_KEY_EMPTY          Request failed because of uninitialized source key element
 *                 CRYPTO_E_SMALL_BUFFER       The provided buffer is too small to store the result
 *                 CRYPTO_E_ENTROPY_EXHAUSTION Request failed, entropy of random number generator is exhausted
 *  \pre           Param channelId needs to be a valid CryIf channel.
 *  \pre           Param Job needs to be a valid pointer to a job object.
 *  \pre           Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_HandleJobProcessing(
  Csm_ChannelIdOfQueueInfoType channelId,
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition);

/**********************************************************************************************************************
 *  Csm_AppendRedirectionToJob()
 *********************************************************************************************************************/
/*! \brief         Appends redirection info to job object.
 *  \details       If the job is configured to use an in-out redirection, it is appended to the job object in this function.
 *  \param[in,out] job        Pointer to the corresponding job.
 *  \param[in]     partition  Id of the current active partition.
 *  \pre           Param job needs to point to a valid job object.
 *  \pre           Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(void, CSM_CODE) Csm_AppendRedirectionToJob(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition);

/**********************************************************************************************************************
 *  Csm_GetPartitionIdx()
 *********************************************************************************************************************/
/*! \brief         Provide the currently active partition index.
 *  \details       The partition index is determined by the OS application id.
 *  \return        The executing partition index or Csm_GetSizeOfPartitionIdentifiers() if application was not found.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Csm_SizeOfPartitionIdentifiersType, CSM_CODE) Csm_GetPartitionIdx(void);

# if (CSM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  Csm_VerifyJobParam()
 *********************************************************************************************************************/
/*! \brief         Verifies the provided jobId
 *  \details       -
 *  \param[in]     jobId                     Id of the job which shall be verified
 *  \param[in,out] errorId                   Pointer to the errorId. If an error occurs, the errorId is set
 *                                           respectively
 *  \param[in]     service                   Service which is called for the Job
 *  \return        E_OK                      Request successful
 *                 E_NOT_OK                  Request failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_VerifyJobParam(
  uint32 jobId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  Crypto_ServiceInfoType service);

/**********************************************************************************************************************
 *  Csm_VerifyInitAndPartition()
 *********************************************************************************************************************/
/*! \brief         Verify the initialization state of the related partition
 *  \details       -
 *  \param[in]     jobId                     Id of the job which shall be checked
 *  \param[in,out] errorId                   Pointer to the errorId. If an error occurs, the errorId is set
 *                                           respectively
 *  \return        E_OK                      Request successful
 *                 E_NOT_OK                  Request failed
 *  \pre           jobId has to be verified to be in range of the configured jobs.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_VerifyInitAndPartition(
  uint32 jobId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId);
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */

/**********************************************************************************************************************
 *  Csm_GetPartitionOfJob()
 *********************************************************************************************************************/
/*! \brief         Provides the partition on which the job is configured
 *  \details       -
 *  \param[in]     jobId              Id of Job from which the partitionIndex is returned.
 *  \return        The partition index configured for the requested job
 *  \pre           jobId must be verified before this function is used.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Csm_SizeOfPartitionIdentifiersType, CSM_CODE) Csm_GetPartitionOfJob(uint32 jobId);
#endif /* (CSM_JOB == STD_ON) */

#if (CSM_CALLOUT == STD_ON)
/**********************************************************************************************************************
 *  Csm_CallPreJobCallout()
 *********************************************************************************************************************/
/*! \brief         Triggers the calling of the configured pre-job callout.
 *  \details       This function checks for the given job if an pre-job callout has to be called. If so, it triggers the
 *                 call.
 *  \param[in,out] job        Pointer to the corresponding job.
 *  \param[in]     partition  Id of the current active partition.
 *  \return        E_OK                      Request successful
 *                 E_NOT_OK                  Request failed
 *  \pre           Param job needs to point to a valid job object.
 *  \pre           Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_CallPreJobCallout(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition);

/**********************************************************************************************************************
 *  Csm_CallPostJobCallout()
 *********************************************************************************************************************/
/*! \brief         Triggers the calling of the configured post-job callout.
 *  \details       This function checks for the given job if an post-job callout has to be called. If so, it triggers
 *                 the call.
 *  \param[in,out] job                       Pointer to the job.
 *  \param[in,out] retValFromProcessing      Pointer to the processing result of the corresponding job. It can be
 *                                           overwritten by the post-job callout.
 *  \param[in]     partition                 Id of the current active partition.
 *  \pre           Param job needs to point to a valid job object.
 *  \pre           Param retValFromProcessing needs to point to a valid location.
 *  \pre           Param partition must be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() -1)
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
CSM_LOCAL FUNC(void, CSM_CODE) Csm_CallPostJobCallout(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) retValFromProcessing,
  Csm_SizeOfPartitionIdentifiersType partition);
#endif /* (CSM_CALLOUT == STD_ON) */
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (CSM_CALLOUT == STD_ON)
/**********************************************************************************************************************
 *  Csm_CallPostJobCallout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CSM_LOCAL FUNC(void, CSM_CODE) Csm_CallPostJobCallout(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  P2VAR(Std_ReturnType, AUTOMATIC, AUTOMATIC) retValFromProcessing,
  Csm_SizeOfPartitionIdentifiersType partition)
{
  Csm_CalloutInfoIdxOfJobTableType calloutInfoId;

  calloutInfoId = Csm_GetCalloutInfoIdxOfJobTable(job->jobId);

  /* #10 Check if post-job callout is configured for the given job. */
  if (calloutInfoId < Csm_GetSizeOfCalloutInfo())
  {
    /* #20 Call post-job callout and reset the callout state to IDLE. */
    (void)Csm_GetPostJobCalloutFuncOfCallout(Csm_GetCalloutIdxOfCalloutInfo(calloutInfoId))( /* SBSW_CSM_FUNCTION_POINTER */
      job,
      CSM_CALLOUT_STATE_POST_INITIAL,
      retValFromProcessing);

    Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION */
  }
# if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */
} /* Csm_CallPostJobCallout() */

/**********************************************************************************************************************
 *  Csm_CallPreJobCallout()
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
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_CallPreJobCallout(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition)
{
  Csm_CalloutInfoIdxOfJobTableType calloutInfoId;
  Std_ReturnType retVal = E_OK;

  /* Retrieve reference to callout for given job */
  calloutInfoId = Csm_GetCalloutInfoIdxOfJobTable(job->jobId);

  /* Check if pre-job callout is configured for the given job. */
  if (calloutInfoId < Csm_GetSizeOfCalloutInfo())
  {
# if (CSM_ASYNC_PROCESSING == STD_ON)
    uint8 calloutState = Csm_GetCalloutStateOfCalloutState(calloutInfoId, partition);

    if( job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
    {
      /* If no callout has been called before or callout did not finish yet, call pre-job. */
      if ((calloutState == CSM_CALLOUT_STATE_IDLE) || (calloutState == CSM_CALLOUT_STATE_PRE_PENDING))
      {
        /* Call pre-job callout */
        retVal = Csm_GetPreJobCalloutFuncOfCallout(Csm_GetCalloutIdxOfCalloutInfo(calloutInfoId))( /* SBSW_CSM_FUNCTION_POINTER */
          job,
          (calloutState == CSM_CALLOUT_STATE_IDLE) ? (CSM_CALLOUT_STATE_PRE_INITIAL) : (CSM_CALLOUT_STATE_PRE_PENDING));

        /* Set state according to return value of callout. */
        switch (retVal)
        {
          case E_OK:
          {
            /* pre-job callout has finished, Processing can start */
            Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_PROCESSING, partition); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION */
            break;
          }

          case CSM_E_PENDING:
          {
            /* pre-job callout has not yet finished, this is only possible in asynchronous job configurations */
            Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_PRE_PENDING, partition); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION */
            break;
          }

          default:
          {
            Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_PROCESSING_ABORTED_BY_CALLOUT, partition); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION */
            /* Change retVal to E_OK to remove job from queue since processing shall never be started */
            retVal = E_OK;
            break;
          }
        }
      }
    }
    else
# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */
    {
      /* Call pre-job callout. */
      retVal = Csm_GetPreJobCalloutFuncOfCallout(Csm_GetCalloutIdxOfCalloutInfo(calloutInfoId))(job, CSM_CALLOUT_STATE_PRE_INITIAL); /* SBSW_CSM_FUNCTION_POINTER */

      /* Set state according to return value of callout. */
      if(retVal == E_OK)
      {
        /* If return value is equal to E_OK, the job shall be executed as without callout. */
        Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_PROCESSING, partition); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION */
      }
      else
      {
        /* If return value is different to E_OK, processing of the job shall be suppressed. */
        Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_POST_INITIAL, partition); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION */
        /* Call post-job callout. */
        Csm_CallPostJobCallout(job, &retVal, partition); /* SBSW_CSM_PASS_JOB_STACK_AND_PARTITION */
      }
    }
  }
  return retVal;
} /* Csm_CallPreJobCallout() */
#endif /* (CSM_CALLOUT == STD_ON) */

#if (CSM_JOB == STD_ON)
/**********************************************************************************************************************
 *  Csm_AppendRedirectionToJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CSM_LOCAL FUNC(void, CSM_CODE) Csm_AppendRedirectionToJob(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  Csm_SizeOfPartitionIdentifiersType partition)
{
# if (CSM_INOUTREDIRECTION == STD_ON)
  uint32 jobId = job->jobId;

  /* #10 If job in- or output shall be redirected, copy corresponding redirection sources and destinations to the job object. */
  if(Csm_IsInOutRedirectionMapUsedOfJobTable(jobId))
  {
    Csm_SizeOfInOutJobRedirectionTableType indexOfInOutRedirectionTable = Csm_GetInOutJobRedirectionTableIdxOfInOutRedirectionMap(Csm_GetInOutRedirectionMapIdxOfJobTable(jobId), partition);
    Csm_RedirectionConfigValueOfInOutJobRedirectionTableType redirectionConfigValue = Csm_GetRedirectionConfigValueOfInOutJobRedirectionTable(indexOfInOutRedirectionTable);

    job->jobRedirectionInfoRef = Csm_GetAddrInOutRedirection(Csm_GetInOutRedirectionMapIdxOfJobTable(jobId), partition); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    job->jobRedirectionInfoRef->redirectionConfig = redirectionConfigValue; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_INPUT)
    {
      job->jobRedirectionInfoRef->inputKeyElementId = Csm_GetInputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->inputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetInputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_INPUT)
    {
      job->jobRedirectionInfoRef->secondaryInputKeyElementId = Csm_GetSecondaryInputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->secondaryInputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetSecondaryInputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_TERTIARY_INPUT)
    {
      job->jobRedirectionInfoRef->tertiaryInputKeyElementId = Csm_GetTertiaryInputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->tertiaryInputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetTertiaryInputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_PRIMARY_OUTPUT)
    {
      job->jobRedirectionInfoRef->outputKeyElementId = Csm_GetOutputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->outputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetOutputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }

    if((redirectionConfigValue & ((uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT)) == (uint8)CRYPTO_REDIRECT_CONFIG_SECONDARY_OUTPUT)
    {
      job->jobRedirectionInfoRef->secondaryOutputKeyElementId = Csm_GetSecondaryOutputKeyElementIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
      job->jobRedirectionInfoRef->secondaryOutputKeyId = Csm_GetCryIfKeyIdOfKey(Csm_GetSecondaryOutputKeyIdOfInOutJobRedirectionTable(indexOfInOutRedirectionTable)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }
  }
  else
# endif /* (CSM_INOUTREDIRECTION == STD_ON) */
  {
    /* #20 Else set redirection to dummy value. */
# if (CSM_JOB_TYPE_LAYOUT_REDIRECTION_INFO_REF == STD_ON)
    job->jobRedirectionInfoRef = (P2VAR(Crypto_JobRedirectionInfoType, AUTOMATIC, CSM_VAR_NOINIT)) (NULL_PTR); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# else
    CSM_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  }
# if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */
} /* Csm_AppendRedirectionToJob() */

/**********************************************************************************************************************
 *  Csm_HandleJobProcessing()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_HandleJobProcessing(
  Csm_ChannelIdOfQueueInfoType channelId,
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition)
{
  Std_ReturnType retVal;

# if (CSM_CALLOUT == STD_ON)
  Csm_CalloutInfoIdxOfJobTableType calloutInfoId;

  /* Retrieve reference to callout for given job */
  calloutInfoId = Csm_GetCalloutInfoIdxOfJobTable(job->jobId);

  /* #10 Call preJobCallout */
  retVal = Csm_CallPreJobCallout(job, partition); /* SBSW_CSM_PASS_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */

  if ((calloutInfoId >= Csm_GetSizeOfCalloutInfo()) || (Csm_GetCalloutStateOfCalloutState(calloutInfoId, partition) == CSM_CALLOUT_STATE_PROCESSING))
# endif /* (CSM_CALLOUT == STD_ON) */
  {
    /* #20 Call CryIf to process given job */
    retVal = CryIf_ProcessJob(channelId, job); /* SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT */
# if (CSM_CALLOUT == STD_ON)
    if (calloutInfoId < Csm_GetSizeOfCalloutInfo())
    {
      /* Job has finished, either successful (E_OK) or not */
#  if (CSM_ASYNC_PROCESSING == STD_ON)
      if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_SYNC)
#  endif
      {
        /* We need to call the post callout routine */
        Csm_SetCalloutStateOfCalloutState(calloutInfoId, CSM_CALLOUT_STATE_POST_INITIAL, partition); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION */
        Csm_CallPostJobCallout(job, &retVal, partition); /* SBSW_CSM_PASS_JOB_POINTER_PARTITION_AND_STACK_API_REQUIREMENT */
      }
    }
# else /* !(CSM_CALLOUT == STD_ON) */
    CSM_DUMMY_STATEMENT_CONST(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  }
  return retVal;
} /* Csm_HandleJobProcessing() */

/**********************************************************************************************************************
 *  Csm_ProcessJob()
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
 *
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_ProcessJob(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition)
{
  Std_ReturnType retVal;
  uint32 jobId = job->jobId;
  Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueInfoIdxOfJobTable(jobId);
  Csm_ChannelIdOfQueueInfoType channelId = Csm_GetChannelIdOfQueueInfo(queueInfoIdx);

# if (CSM_ASYNC_PROCESSING == STD_ON)
  Crypto_JobStateType combinedJobState = job->jobState;

#  if (CSM_JOB_TYPE_LAYOUT_ASR430_COMPATIBILITY == STD_ON)
  combinedJobState |= job->state;
#  endif
# endif

  /* #05 Append job redirection to job object */
  Csm_AppendRedirectionToJob(job, partition); /* SBSW_CSM_PASS_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */

  /* Switch depending on whether there is asynchronous processing at all */
# if (CSM_ASYNC_PROCESSING == STD_ON)
  /* Check if the job is asynchronous. */
  if (job->jobPrimitiveInfo->processingType == CRYPTO_PROCESSING_ASYNC)
  {
    boolean gotLockOfQueue = (Csm_GetLockOfQueueState(queueInfoIdx, partition) == 0u) ? TRUE : FALSE;
    /* Job is asynchronous */
    /* #20 Determine if queue is empty or job is already active and queue is not locked
     *     (e.g. by this function) and call Csm_HandleJobProcessing if so */
    if ((gotLockOfQueue == TRUE) &&
      ((Csm_GetQueueIdxOfQueueState(queueInfoIdx, partition) == Csm_GetQueueStartIdxOfQueueInfo(queueInfoIdx))
        || ((combinedJobState & CRYPTO_JOBSTATE_ACTIVE) == CRYPTO_JOBSTATE_ACTIVE)))
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_PROGRESSING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
      Csm_IncLockOfQueueState(queueInfoIdx, partition);  /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
      Csm_SetRetriggeringOfQueueState(queueInfoIdx, CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
#  endif
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      /* Call CryIf_ProcessJob() */
      retVal = Csm_HandleJobProcessing(channelId, job, partition); /* SBSW_CSM_PASS_CHANNEL_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */

      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
      Csm_DecLockOfQueueState(queueInfoIdx, partition);  /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
    }
    else
    {
      /* Set retVal temporarily to busy to force enqueuing */
      retVal = CRYPTO_E_BUSY;
    }

    /* #22 Enqueuing necessary if return value is either BUSY or QUEUE_FULL */
    if ((retVal == CRYPTO_E_BUSY) || (retVal == CRYPTO_E_QUEUE_FULL))
    {
      /* #23 Append job redirection to job object again to overwrite changes that were made during last processing attempt */
      Csm_AppendRedirectionToJob(job, partition); /* SBSW_CSM_PASS_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */

      /* #24 Enqueue job or return QUEUE_FULL */
      retVal = Csm_EnqueueJob(queueInfoIdx, job, partition); /* SBSW_CSM_PASS_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */
#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
      if (Csm_IsTriggerAsynchJobsInCallbackOfQueueInfo(queueInfoIdx)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        /* #25 If triggering asynchronous jobs is enabled for the queue handle the retriggering depending on whether
         *      getting the queue lock failed or not. */
        if (!gotLockOfQueue)
        {
          Csm_SetRetriggeringOfQueueState(queueInfoIdx, CSM_ASYNC_JOB_RETRIGGER_STATE_PENDING, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
        }
        else if (Csm_GetRetriggeringOfQueueState(queueInfoIdx, partition) == CSM_ASYNC_JOB_RETRIGGER_STATE_PENDING)
        {
          Csm_TriggerAsynchJobProcessing(queueInfoIdx);
        }
        else
        {
          /* Do nothing */
        }
      }
#  endif /* (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON) */
    }
    /* #26 Request has been successfully passed to lower layer */
    else if (retVal == E_OK)
    {
      /* #27 Check if the job is still in state PROGRESSING or callback already occurred */
      if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_PROGRESSING)
      {
        Csm_SetJobState(jobId, CSM_JOB_STATE_WAITING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
      }
      else
      {
        /* Callback already occurred but was not ready to process the application callback - do that now. */
        /* Lock queue to prevent a recursive call of Csm_ProcessJob from the application callback function */
        Csm_IncLockOfQueueState(queueInfoIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_CallbackNotification(job, Csm_GetJobState(jobId, partition) & CSM_JOB_STATE_CALLBACK_OCCURRED_CLEAR_MASK); /* SBSW_CSM_PASS_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_DecLockOfQueueState(queueInfoIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
      }
    }
    else
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
      job->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
#  endif
    }
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
  }
  else
  {
    /* The job is synchronous */
    /* #35 If any jobs are queued and job is not active, compare the job priority with the highest priority in the queue */
    if (
      (Csm_GetQueueIdxOfQueueState(queueInfoIdx, partition) != Csm_GetQueueStartIdxOfQueueInfo(queueInfoIdx))
      && ((combinedJobState & CRYPTO_JOBSTATE_ACTIVE) != CRYPTO_JOBSTATE_ACTIVE)
      )
    {
      /* #37 Call Csm_HandleJobProcessing function if job priority is higher than the highest priority in the queue,
       *     otherwise return busy */
      if (job->jobPriority > (Csm_GetQueue((Csm_QueueIdxOfQueueStateType)(Csm_GetQueueIdxOfQueueState(queueInfoIdx, partition) - 1u), partition)->jobPriority))
      {
        Csm_SetJobState(jobId, CSM_JOB_STATE_PROGRESSING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
        Csm_IncLockOfQueueState(queueInfoIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        retVal = Csm_HandleJobProcessing(channelId, job, partition); /* SBSW_CSM_PASS_CHANNEL_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_DecLockOfQueueState(queueInfoIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
      }
      else
      {
        retVal = CRYPTO_E_BUSY;
      }

      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
    else
# else /* !(CSM_ASYNC_PROCESSING == STD_ON) */
  {
# endif
    {
      /* #40 Process job if Queue is empty or job is active */
      Csm_SetJobState(jobId, CSM_JOB_STATE_PROGRESSING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      /* Call CryIf_ProcessJob() */
      retVal = Csm_HandleJobProcessing(channelId, job, partition); /* SBSW_CSM_PASS_CHANNEL_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */
    }

    /* #41 Free object if job finishes or is rejected, otherwise set to active */
    if ((retVal != E_OK) || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
      job->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# endif
    }
    else
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_ACTIVE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
    }
  }

  return retVal;
} /* Csm_ProcessJob() */

# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
/**********************************************************************************************************************
 *  Csm_PrepareJobObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_PrepareJobObj(
  uint32 jobId,
  P2VAR(Crypto_JobType, AUTOMATIC, AUTOMATIC) job)
{
  job->jobId = jobId; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
  job->jobPrimitiveInfo = Csm_GetAddrJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
#  if (CSM_JOB_TYPE_LAYOUT_JOB_INFO_PTR == STD_ON)
  job->jobInfo = Csm_GetAddrJobInfo(Csm_GetJobInfoIdxOfJobTable(jobId)); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
#  endif
  job->jobPriority = Csm_GetPriorityOfJobTable(jobId); /* SBSW_CSM_WRITE_TO_JOB_POINTER */
  job->jobState = CRYPTO_JOBSTATE_IDLE; /* SBSW_CSM_WRITE_TO_JOB_POINTER */

#  if (CSM_JOB_TYPE_LAYOUT_ASR430_COMPATIBILITY == STD_ON)
  job->PrimitiveInputOutput = job->jobPrimitiveInputOutput; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
  job->state = job->jobState; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
#  endif
} /* Csm_PrepareJobObj() */
# endif /* (CSM_JOBSHARINGOFQUEUEINFO == STD_ON) */

/**********************************************************************************************************************
 *  Csm_GetJobObj()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_GetJobObj(
  uint32 jobId,
  Csm_SizeOfPartitionIdentifiersType partition,
  P2VAR(Csm_SizeOfJobType, AUTOMATIC, AUTOMATIC) jobObjId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = CRYPTO_E_BUSY;

  /* #10 Differ between job states */
  if (Csm_GetJobState(jobId, partition) >= CSM_JOB_STATE_PROGRESSING)
  {
    /* #11 Job is currently in use, e.g. queued or in driver, reject request with busy */
    /* retval is already set. */
  }
  else if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_ACTIVE)
  {
    /* #12 Job is active but is waiting for further input. Job object was reserved already - return it.*/
    *jobObjId = Csm_GetJobToObjMap(jobId, partition); /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
    retVal = E_OK;
  }
# if (CSM_JOBIDXOFJOBTABLE == STD_ON)
  else
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
  if (Csm_IsJobUsedOfJobTable(jobId))
#  endif
  {
    /* #20 Job has a dedicated job object, return it. */
    *jobObjId = Csm_GetJobIdxOfJobTable(jobId); /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
    Csm_SetJobToObjMap(jobId, *jobObjId, partition); /* SBSW_WRITE_TO_OBJECTMAP_BY_JOBID */
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
    Csm_GetAddrJob(*jobObjId, partition)->jobId = jobId; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
#  endif
    retVal = E_OK;
  }
# endif /* (CSM_JOBIDXOFJOBTABLE == STD_ON) */
# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
  else
  {
    /* #21 Job Sharing: Find a free slot and return it, return busy if there is none */
    Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueInfoIdxOfJobTable(jobId);
    Csm_SizeOfJobType idx;
#  if (CSM_ASYNC_PROCESSING == STD_ON)
    /* If it is an asynchronous job fetch the next job object from the pool which is free for usage. */
    if (Csm_IsAsynchronousOfJobTable(jobId))
    {
      retVal = CSM_QUEUE_FULL_RETVAL;

      for (idx = Csm_GetJobPoolStartIdxOfQueueInfo(queueInfoIdx); idx < Csm_GetJobPoolEndIdxOfQueueInfo(queueInfoIdx); idx++) /* FETA_CSM_STRICTLY_INCREASING_BY_ONE */
      {
        if (Csm_GetJob(idx, partition).jobId == CSM_EMPTY_JOB)
        {
          *jobObjId = idx; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
          Csm_SetJobToObjMap(jobId, idx, partition); /* SBSW_WRITE_TO_OBJECTMAP_BY_JOBID */
          Csm_PrepareJobObj(jobId, Csm_GetAddrJob(idx, partition)); /* SBSW_CSM_PASS_JOB */
          retVal = E_OK;
          break;
        }
      }
#   if (CSM_DEV_ERROR_REPORT == STD_ON)
      if (retVal == CSM_QUEUE_FULL_RETVAL)
      {
        /* #22 Report runtime error if no free slot is available equivalent to queue is full */
        (void)Det_ReportRuntimeError(CSM_MODULE_ID, CSM_INSTANCE_ID, CSM_GENERIC_JOB_PROCESSING_ID, CSM_E_QUEUE_FULL);
      }
#   endif
    }
    else
#  endif /* (CSM_ASYNC_PROCESSING == STD_ON) */
    {
      /* Fetch the one job object reserved for all synchronous jobs for this queue and check if it is free for usage. */
      idx = Csm_GetJobObjSynchronousIdxOfQueueInfo(queueInfoIdx);
      if (Csm_GetJob(idx, partition).jobId == CSM_EMPTY_JOB)
      {
        *jobObjId = idx; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
        Csm_SetJobToObjMap(jobId, idx, partition); /* SBSW_WRITE_TO_OBJECTMAP_BY_JOBID */
        Csm_PrepareJobObj(jobId, Csm_GetAddrJob(idx, partition)); /* SBSW_CSM_PASS_JOB */
        retVal = E_OK;
      }
    }
  }
# endif /* (CSM_JOBSHARINGOFQUEUEINFO == STD_ON) */

# if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */

  return retVal;
} /* Csm_GetJobObj() */

# if (CSM_ASYNC_PROCESSING == STD_ON)
/**********************************************************************************************************************
 *  Csm_EnqueueJob()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_EnqueueJob(
  Csm_QueueInfoIterType queueIdx,
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition)
{
  Std_ReturnType retVal = E_OK;
  Csm_QueueIterType i, j;

  /* #5 Check if job that shall be enqueued is active and if queue has an empty place.*/
  if ((Csm_GetJobState(job->jobId, partition) == CSM_JOB_STATE_ACTIVE) && (Csm_GetQueueIdxOfQueueState(queueIdx, partition) < Csm_GetQueueEndIdxOfQueueInfo(queueIdx)))
  {
    Csm_SetQueue(Csm_GetQueueIdxOfQueueState(queueIdx, partition), job, partition); /* SBSW_CSM_WRITE_TO_QUEUE_API_REQ */

    Csm_IncQueueIdxOfQueueState(queueIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_API_REQ */
  }
  /* #10 Check if queue has two empty places, so that there is always one empty place for an active job,
   *     otherwise return QUEUE_FULL and report runtime error. */
  else if (Csm_GetQueueIdxOfQueueState(queueIdx, partition) < (Csm_GetQueueEndIdxOfQueueInfo(queueIdx) - 1u))
  {
    for (i = Csm_GetQueueStartIdxOfQueueInfo(queueIdx); i <= Csm_GetQueueIdxOfQueueState(queueIdx, partition); i++) /* FETA_CSM_STRICTLY_INCREASING_BY_ONE_WITH_PRE_CHECK */
    {
      /* #11 If end of queue is reached, new job has the highest priority. Simply add new job at the end. */
      if (i == Csm_GetQueueIdxOfQueueState(queueIdx, partition))
      {
        Csm_SetQueue(i, job, partition); /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */
      }
      /* #12 If job priority of job is lower or equal to the one in the queue,
       *     or if the one in the queue is active, take its place. */
      else if ((job->jobPriority <= Csm_GetQueue(i, partition)->jobPriority) || (Csm_GetJobState(Csm_GetQueue(i, partition)->jobId, partition) == CSM_JOB_STATE_ACTIVE))
      {
        /* Place found in which job needs to be inserted */
        /* #13 Move all remaining jobs, which are already sorted, by one place to the right. */
        for (j = Csm_GetQueueIdxOfQueueState(queueIdx, partition); j > i; j--) /* FETA_CSM_STRICTLY_DECREASING_BY_ONE */
        {
          Csm_SetQueue(j, Csm_GetQueue(j - 1u, partition), partition); /* PRQA S 2841 */ /* MD_CSM_QUEUE_BY_QUEUEIDX */ /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */
        }

        /* #14 Insert new job into the freed place. */
        Csm_SetQueue(i, job, partition); /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */
        break;
      }
      else
      {
        /* Priority of job to insert is higher, check next slot of queue. */
      }
    }

    Csm_SetJobState(job->jobId, CSM_JOB_STATE_QUEUED, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
    Csm_IncQueueIdxOfQueueState(queueIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_API_REQ */
  }
  else
  {
    retVal = CSM_QUEUE_FULL_RETVAL;
#  if (CSM_DEV_ERROR_REPORT == STD_ON)
    (void)Det_ReportRuntimeError(CSM_MODULE_ID, CSM_INSTANCE_ID, CSM_GENERIC_JOB_PROCESSING_ID, CSM_E_QUEUE_FULL);
#  endif
  }

#  if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */

  return retVal;
} /* Csm_EnqueueJob() */

/**********************************************************************************************************************
 *  Csm_DequeueJob()
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
CSM_LOCAL FUNC(void, CSM_CODE) Csm_DequeueJob(
  Csm_QueueInfoIterType queueIdx,
  uint32 jobId,
  Csm_SizeOfPartitionIdentifiersType partition)
{
  Csm_QueueIterType i, j;

  /* #10 Iterate over enqueued jobs. */
  for (i = Csm_GetQueueIdxOfQueueState(queueIdx, partition); i > Csm_GetQueueStartIdxOfQueueInfo(queueIdx); i--) /* FETA_CSM_STRICTLY_DECREASING_BY_ONE */
  {
    if (jobId == Csm_GetQueue(i - 1u, partition)->jobId)
    {
      /* #20 Fill the possible gap by shifting remaining jobs to the left. */
      for (j = i; j < Csm_GetQueueIdxOfQueueState(queueIdx, partition); j++) /* FETA_CSM_STRICTLY_INCREASING_BY_ONE */
      {
        Csm_SetQueue(j - 1u, Csm_GetQueue(j, partition), partition); /* PRQA S 2841 */ /* MD_CSM_QUEUE_BY_QUEUEIDX */ /* SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX */
      }

      /* #21 Decrease queue size to finalize dequeuing. */
      Csm_DecQueueIdxOfQueueState(queueIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */

      break;
    }
  }

#  if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */

} /* Csm_DequeueJob() */

/**********************************************************************************************************************
 *  Csm_TriggerAsynchJobProcessing()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CSM_LOCAL FUNC(void, CSM_CODE) Csm_TriggerAsynchJobProcessing(Csm_QueueInfoIterType queueIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
  uint32 jobId;
  Std_ReturnType retVal;
  Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionConfigIdxOfQueueInfo(queueIdx);

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Check if queue has entries and processing is possible (queue not locked due to an active request): */
  if ((Csm_GetLockOfQueueState(queueIdx, partition) == 0u) && (Csm_GetQueueIdxOfQueueState(queueIdx, partition) != Csm_GetQueueStartIdxOfQueueInfo(queueIdx)))
  {
    job = Csm_GetQueue(Csm_GetQueueIdxOfQueueState(queueIdx, partition) - 1u, partition);
    jobId = job->jobId;

#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
    Csm_SetRetriggeringOfQueueState(queueIdx, CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_PARTITION_DEPENDENT */
#  endif

    if (jobId < Csm_GetSizeOfJobState(partition))
    {
      /* #20 Lock queue temporarily and pass job to underlying crypto. */
      Csm_SetJobState(jobId, CSM_JOB_STATE_PROGRESSING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
      Csm_IncLockOfQueueState(queueIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      retVal = Csm_HandleJobProcessing(Csm_GetChannelIdOfQueueInfo(queueIdx), job, partition); /* SBSW_CSM_PASS_JOB */
      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
      Csm_DecLockOfQueueState(queueIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */

      /* #31 If Job was successfully passed to lower layer remove it from queue. */
      if (retVal == E_OK)
      {
        Csm_DequeueJob(queueIdx, jobId, partition);
        /* #32 Check if the job is still in state PROGRESSING or callback already occurred. */
        if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_PROGRESSING)
        {
          Csm_SetJobState(jobId, CSM_JOB_STATE_WAITING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
        }
        else
        {
          /* Callback already occurred but was not ready to process the application callbacks - do that now */
          SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
          Csm_CallbackNotification(job, Csm_GetJobState(jobId, partition) & CSM_JOB_STATE_CALLBACK_OCCURRED_CLEAR_MASK); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
          SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        }
      }
      /* #33 If job cannot be processed because of busy driver or full queue. Let job in queue and try again later. */
      else if ((retVal == CRYPTO_E_BUSY) || (retVal == CRYPTO_E_QUEUE_FULL))
      {
        /* #34 Append job redirection to job object again to overwrite changes that were made during last processing attempt */
        Csm_AppendRedirectionToJob(job, partition); /* SBSW_CSM_PASS_JOB_POINTER_AND_PARTITION_API_REQUIREMENT */

        Csm_SetJobState(jobId, CSM_JOB_STATE_QUEUED, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
      }
      else
      {
        /* #40 If lower layer returns an error remove job from queue and notify application. */
        Csm_DequeueJob(queueIdx, jobId, partition);
        /* change to state waiting for correct handling in callback notification */
        Csm_SetJobState(jobId, CSM_JOB_STATE_WAITING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_CallbackNotification(job, retVal); /* SBSW_CSM_PASS_JOB_POINTER_LOCAL */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
      }
    }
    else
    {
      /* job invalid - throw it away */
      Csm_DecQueueIdxOfQueueState(queueIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE */
    }
  }
} /* Csm_TriggerAsynchJobProcessing() */

/**********************************************************************************************************************
 *  Csm_HandleCryIfCancelResultInStateCanceling()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CSM_LOCAL FUNC(boolean, CSM_CODE) Csm_HandleCryIfCancelResultInStateCanceling(
  Csm_SizeOfPartitionIdentifiersType partition,
  uint32 jobId,
  Std_ReturnType cryIfRetVal,
  uint8 cancelFailedState)
{
  boolean notifyByCallback = FALSE;

  if (cryIfRetVal == E_OK)
  {
    notifyByCallback = TRUE;
  }
  else if (cryIfRetVal == CRYPTO_E_JOB_CANCELED)
  {
    /* No immediate cancellation in lower layer possible, wait for next Csm_CallbackNotification */
    Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELED, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
  }
  else
  {
    /* Canceling failed, restore state */
    Csm_SetJobState(jobId, cancelFailedState, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
  }

#  if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */

  return notifyByCallback;
}

#  if (CSM_CANCELATION_DURING_PROCESSING == STD_ON)
/**********************************************************************************************************************
 *  Csm_HandleCryIfCancelResultStateHasChanged()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleCryIfCancelResultStateHasChanged(
  Csm_SizeOfPartitionIdentifiersType partition,
  uint32 jobId,
  Std_ReturnType cryIfRetVal)
{

  if (cryIfRetVal == E_OK)
  {
    /* Nothing to do, callback already occurred */
  }
  else if (cryIfRetVal == CRYPTO_E_JOB_CANCELED)
  {
    /* No immediate cancellation in lower layer possible, wait for next Csm_CallbackNotification */
    Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELED, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
  }
  else
  {
    /* Job State has changed due to callback. Leave the state as it is. */
  }

#   if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#   endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */

}
#  endif /* (CSM_CANCELATION_DURING_PROCESSING == STD_ON) */

/**********************************************************************************************************************
 *  Csm_CancelAsynchronousJob()
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
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_CancelAsynchronousJob(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job,
  Csm_SizeOfPartitionIdentifiersType partition)
{
  uint32 jobId = job->jobId;
  Std_ReturnType retVal = E_NOT_OK;
  Csm_QueueInfoIterType queueInfoIdx = Csm_GetQueueInfoIdxOfJobTable(jobId);

  boolean notifyByCallback = FALSE;

#  if (CSM_CANCELATION_DURING_PROCESSING == STD_ON)
  if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_WAITING)
  {
    /* #10 Object is in driver and could call Csm_Notification at any time. Even if lower layer returns,
     *     driver could have already invoked the Csm_CallbackNotification function or be on the way. */

    Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

    /* #12 Cancel job in lower layer */
    retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(queueInfoIdx), job); /* SBSW_CSM_PASS_JOB */

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();

    if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_CANCELING)
    {
      notifyByCallback = Csm_HandleCryIfCancelResultInStateCanceling(partition, jobId, retVal, CSM_JOB_STATE_WAITING);
    }
    else
    {
      Csm_HandleCryIfCancelResultStateHasChanged(partition, jobId, retVal);
    }
  }
  else
#  endif /* (CSM_CANCELATION_DURING_PROCESSING == STD_ON) */
    if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_QUEUED)
    {
      /* #21 If Job is not started yet, cancellation can be done in Csm only, otherwise cancellation in lower layer required. */
      if ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_START) == CRYPTO_OPERATIONMODE_START)
      {
        retVal = E_OK;
      }
      else
      {
        /* Job was already started and waits for input in driver - cancel it in lower layer too */
        /* #23 Pause processing when canceling in lower layer. Otherwise job could be passed to lower layer in between */
        Csm_IncLockOfQueueState(queueInfoIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(queueInfoIdx), job); /* SBSW_CSM_PASS_JOB */
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_DecLockOfQueueState(queueInfoIdx, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ */
      }

      if (retVal == E_OK)
      {
        /* #24 Remove job from queue in case of successful cancellation */
        Csm_DequeueJob(queueInfoIdx, jobId, partition);
        notifyByCallback = TRUE;
      }
    }
    else if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_ACTIVE)
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

      /* #32 Cancel job in lower layer */
      retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(queueInfoIdx), job); /* SBSW_CSM_PASS_JOB */

      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();

      if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_CANCELING)
      {
        notifyByCallback = Csm_HandleCryIfCancelResultInStateCanceling(partition, jobId, retVal, CSM_JOB_STATE_ACTIVE);
      }
      else
      {
        /* Job State was modified in between so Csm_CallbackNotification already occurred */
        retVal = E_OK;
      }
    }
    else
    {
      /* Job is in mode which cannot be cancelled - retVal already set */
    }

  /* #40 If job was canceled, call configured callback notification function */
  if (notifyByCallback)
  {
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    Csm_CallbackNotification(job, CRYPTO_E_JOB_CANCELED); /* SBSW_CSM_FORWARDING_PTR */
    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
  }

  return retVal;
} /* Csm_CancelAsynchronousJob() */

/**********************************************************************************************************************
 *  Csm_HandleCallbackNotification()
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
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleCallbackNotification(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  Crypto_ResultType result,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorIdPtr,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) runtimeErrorIdPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType resultCopy = result;
  boolean notify = TRUE;

  /* Keep jobId local to still have it availabe after clearing the job object in case of a shared job object */
  uint32 jobId = job->jobId;
  Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
  Csm_SizeOfJobType jobObjId = Csm_GetSizeOfJob(partition);
#  endif

  SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
#  if (CSM_CANCELATION_DURING_PROCESSING == STD_ON)
  /* #21 Check if job was cancelled in between and update return value accordingly */
  if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_CANCELED)
  {
    resultCopy = CRYPTO_E_JOB_CANCELED;
    Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
  }
  else
#  endif /* (CSM_CANCELATION_DURING_PROCESSING == STD_ON) */
    if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_IDLE)
    {
      /* Job is not running - Illegal function call, ignore it */
      notify = FALSE;
      *runtimeErrorIdPtr = CSM_E_UNEXPECTED_CALLBACK; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
    }
  /* #22 If not yet in state waiting but in state progressing due to preemption,
   *     save result and call callback in initial call. */
    else if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_PROGRESSING)
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_CALLBACK_OCCURRED | resultCopy, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
      notify = FALSE;
    }
  /* #23 Update state to accept new request directly triggered in callback of application.
         Set new state to idle or active depending on whether operation mode contains operation mode finish. */
    else if ((resultCopy == E_OK) && ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) != CRYPTO_OPERATIONMODE_FINISH))
    {
      Csm_SetJobState(jobId, CSM_JOB_STATE_ACTIVE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
    }
    else
    {
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
      /* Get Csm object since passed job object could be a copy made by crypto driver */
      jobObjId = Csm_GetJobToObjMap(jobId, partition);
#  endif
      Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
    }

  SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

  /* #24 Check wether user is already to be notified and shared object is to be freed */
  if (notify)
  {
#  if (CSM_CALLOUT == STD_ON)
    Csm_CallPostJobCallout(job, &resultCopy, partition); /* SBSW_CSM_PASS_JOB_STACK_AND_PARTITION */
#  endif /* (CSM_CALLOUT == STD_ON) */

      /* #30 Call user callback function */
    Csm_HandleApplicationCallback(job, resultCopy, errorIdPtr, partition); /* SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT_AND_STACK */

    /* #31 Free object if job finishes or is rejected if job has no dedicated object */
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
    if ((jobObjId < Csm_GetSizeOfJob(partition))
#   if (CSM_JOBIDXOFJOBTABLE == STD_ON)
      && (!(Csm_IsJobUsedOfJobTable(jobId)))
#   endif
      )
    {
      Csm_GetAddrJob(jobObjId, partition)->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
    }
#  endif /* (CSM_JOBSHARINGOFQUEUEINFO == STD_ON) */

#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
    {
      /* #40 Trigger new asynchronous job. Derive the retrigger state depending on whether getting queue lock succeeded
        *     or failed. */
      Csm_QueueInfoIterType queueInfoIdx;
      queueInfoIdx = Csm_GetQueueInfoIdxOfJobTable(jobId);

      if (Csm_IsTriggerAsynchJobsInCallbackOfQueueInfo(queueInfoIdx)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
        Csm_SetRetriggeringOfQueueState(queueInfoIdx, CSM_ASYNC_JOB_RETRIGGER_STATE_PENDING, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_PARTITION_DEPENDENT */
        Csm_TriggerAsynchJobProcessing(queueInfoIdx); /* SBSW_CSM_PASS_QUEUESTATEINDEX */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      }
    }
#  endif /* (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON) */
  }
#  if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */

} /* Csm_HandleCallbackNotification() */

/**********************************************************************************************************************
 *  Csm_HandleApplicationCallback()
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
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleApplicationCallback(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  Std_ReturnType result,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  Csm_SizeOfPartitionIdentifiersType partition)
{
  /* #10 Call configured user callback function */
#  if (CSM_CALLBACK_START_NOTIFICATION == STD_OFF)
  if ((((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_UPDATE) == CRYPTO_OPERATIONMODE_UPDATE)
          && Csm_IsPrimitiveCallbackUpdateNotificationOfJobTable(job->jobId))
      || ((job->jobPrimitiveInputOutput.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH)
      || (result == CRYPTO_E_JOB_CANCELED))
#  endif /* (CSM_CALLBACK_START_NOTIFICATION == STD_OFF) */
  {
#  if (CSM_CALLBACKFUNC43OFBSWCALLBACKS == STD_ON)
    if (Csm_IsBswCallbacksUsedOfJobTable(job->jobId) && (Csm_GetCallbackFunc43OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId)) != NULL_PTR))
    {
      Csm_GetCallbackFunc43OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId))(job, result); /* SBSW_CSM_FUNCTION_POINTER */
    }
    else
#  endif /* (CSM_CALLBACKFUNC43OFBSWCALLBACKS == STD_ON) */

#  if(CSM_CALLBACKFUNC44OFBSWCALLBACKS == STD_ON)
    if (Csm_IsBswCallbacksUsedOfJobTable(job->jobId) && (Csm_GetCallbackFunc44OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId)) != NULL_PTR))
    {
      Csm_GetCallbackFunc44OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId))(job->jobId, result); /* SBSW_CSM_FUNCTION_POINTER */
    }
    else
#  endif /* (CSM_CALLBACKFUNC44OFBSWCALLBACKS == STD_ON) */

#  if(CSM_CALLBACKFUNC45OFBSWCALLBACKS == STD_ON)
    if (Csm_IsBswCallbacksUsedOfJobTable(job->jobId) && (Csm_GetCallbackFunc45OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId)) != NULL_PTR))
    {
      Csm_GetCallbackFunc45OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId))(job, result); /* SBSW_CSM_FUNCTION_POINTER */
    }
    else
#  endif /* (CSM_CALLBACKFUNC45OFBSWCALLBACKS == STD_ON) */

#  if(CSM_CALLBACKFUNC46OFBSWCALLBACKS == STD_ON)
    /* Coming to this line Csm_GetCallbackFunc46OfBswCallbacks is always != NULL_PTR */
    if (Csm_IsBswCallbacksUsedOfJobTable(job->jobId))
    {
      Csm_GetCallbackFunc46OfBswCallbacks(Csm_GetBswCallbacksIdxOfJobTable(job->jobId))(job->jobId, result); /* SBSW_CSM_FUNCTION_POINTER */
    }
    else
#  endif /* (CSM_CALLBACKFUNC46OFBSWCALLBACKS == STD_ON) */

#  if (CSM_RTECALLBACK == STD_ON)
    if (Csm_IsRteCallbackUsedOfJobTable(job->jobId)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      Csm_SetRteResult(Csm_GetRteCallbackIdxOfJobTable(job->jobId), result, partition); /* SBSW_CSM_WRITE_TO_RTERESULT_BY_JOBID */
      Csm_SetRteCallbackOccurred(TRUE, partition); /* SBSW_CSM_CSL_VAR_ACCESS */
    }
    else
#  endif /* (CSM_RTECALLBACK == STD_ON) */

    {
      *errorId = CSM_E_PARAM_METHOD_INVALID; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
      /* Notification was called with an invalid job */
      CSM_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
      CSM_DUMMY_STATEMENT(result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
      CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
    }
  }
} /* Csm_HandleApplicationCallback() */

#  if (CSM_RTECALLBACK == STD_ON)
/**********************************************************************************************************************
 *  Csm_HandleRteCallbacks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CSM_LOCAL_INLINE FUNC(void, CSM_CODE) Csm_HandleRteCallbacks(Csm_SizeOfPartitionIdentifiersType partition)
{
  /* Rte Notifications */
  if (Csm_IsRteCallbackOccurred(partition))
  {
    Csm_RteCallbackIterType callbackIdx;
    Std_ReturnType retVal;

    Csm_SetRteCallbackOccurred(FALSE, partition); /* SBSW_CSM_CSL_VAR_ACCESS */
    for (callbackIdx = 0u; callbackIdx < Csm_GetSizeOfRteCallback(partition); callbackIdx++)
    {
      retVal = Csm_GetRteResult(callbackIdx, partition);
      if (retVal != CSM_CALLBACK_IDLE)
      {
        Csm_SetRteResult(callbackIdx, CSM_CALLBACK_IDLE, partition); /* SBSW_CSM_WRITE_TO_RTERESULT_BY_SIZE_API_REQ */
        (void)Csm_GetRteCallback(callbackIdx, partition)(retVal); /* SBSW_CSM_FUNCTION_POINTER */
      }
    }
  }

  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
} /* Csm_HandleRteCallbacks() */
#  endif /* (CSM_RTECALLBACK == STD_ON) */

# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */

# if (CSM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 *  Csm_VerifyJobParam()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CSM_LOCAL FUNC(Std_ReturnType, CSM_CODE) Csm_VerifyJobParam(
  uint32 jobId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId,
  Crypto_ServiceInfoType service)
{
  uint8 retVal = E_NOT_OK;

  /* Check if the provided job id is in range of the configured jobs */
  if (jobId >= Csm_GetSizeOfJobTable())
  {
    *errorId = CSM_E_PARAM_HANDLE; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
  }
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service != service)
  {
    *errorId = CSM_E_SERVICE_TYPE; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
  }
  else
  {
    retVal = Csm_VerifyInitAndPartition(jobId, errorId); /* SBSW_CSM_FORWARDING_PTR */
  }

  return retVal;
} /* Csm_VerifyJobParam() */

/**********************************************************************************************************************
 *  Csm_VerifyInitAndPartition()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CSM_LOCAL_INLINE FUNC(Std_ReturnType, CSM_CODE) Csm_VerifyInitAndPartition(
  uint32 jobId,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) errorId)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* Job id is reliable - Get the partition from the job */
  Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

  /* Check the initialization state of the partition */
  if (Csm_GetPartitionDataInitialized(partition) != CSM_INITIALIZED)
  {
    *errorId = CSM_E_UNINIT; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
  }
#  if (CSM_MULTI_PARTITION_RUNTIME_CHECKS == STD_ON)
  /* Check if the currently executing partition matches the configured partition */
  else if (Csm_GetPartitionIdx() != partition)
  {
    *errorId = CSM_E_INVALID_PARTITION; /* SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE */
  }
#  endif
  else
  {
    retVal = E_OK;
  }

#  if (CSM_NUMBER_OF_PARTITIONS <= 1u)
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */

  return retVal;
} /* Csm_VerifyInitAndPartition() */
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */

/**********************************************************************************************************************
 *  Csm_GetPartitionOfJob()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CSM_LOCAL_INLINE FUNC(Csm_SizeOfPartitionIdentifiersType, CSM_CODE) Csm_GetPartitionOfJob(uint32 jobId)
{
  return Csm_GetPartitionConfigIdxOfQueueInfo(Csm_GetQueueInfoIdxOfJobTable(jobId));
}
#endif /* (CSM_JOB == STD_ON) */

#if ((CSM_DEV_ERROR_DETECT == STD_ON) || (CSM_JOB == STD_ON))
/**********************************************************************************************************************
 *  Csm_GetPartitionIdx()
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
CSM_LOCAL_INLINE FUNC(Csm_SizeOfPartitionIdentifiersType, CSM_CODE) Csm_GetPartitionIdx(void)
{
  Csm_SizeOfPartitionIdentifiersType partIdx = Csm_GetSizeOfPartitionIdentifiers();

# if (CSM_NUMBER_OF_PARTITIONS > 1u)
  ApplicationType currentApplicationId = GetApplicationID();

  /* Iterate over all available partitions */
  Csm_SizeOfPartitionIdentifiersType index = 0u;

  for (index = 0u; index < Csm_GetSizeOfPartitionIdentifiers(); index++)
  {
    /* Check if the application id match with one of the configured application id's */
    if (currentApplicationId == Csm_GetPartitionSNVOfPartitionIdentifiers(index))
    {
      partIdx = index;
      break;
    }
  }
# else /* !(CSM_NUMBER_OF_PARTITIONS > 1u) */
  partIdx = 0u;
# endif
  /* Return the index of the configured partition */
  return partIdx;
} /* Csm_GetPartitionIdx() */
#endif /* ((CSM_DEV_ERROR_DETECT == STD_ON) || (CSM_JOB == STD_ON)) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Csm_InitMemory_Local()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, CSM_CODE) Csm_InitMemory_Local(Csm_SizeOfPartitionIdentifiersType partition)
{
  /* ----- Implementation ----------------------------------------------- */
#if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* Initialize the initialization flag of the calling partition. */
  Csm_SetPartitionDataInitialized(CSM_UNINIT, partition); /* SBSW_CSM_WRITE_PARTITION_DATA */
#endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
} /* Csm_InitMemory_Local() */

/**********************************************************************************************************************
 *  Csm_Init()
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
/* PRQA S 6010, 6030, 6080 1 */ /* MD_MSR_STPTH */ /* MD_MSR_STCYC */ /* MD_MSR_STMIF */
FUNC(void, CSM_CODE) Csm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

#if ((CSM_DEV_ERROR_DETECT == STD_ON) || (CSM_JOB == STD_ON))
  Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionIdx();
#endif
  /* ----- Development Error Checks ------------------------------------- */
#if (CSM_DEV_ERROR_DETECT == STD_ON)
# if (CSM_NUMBER_OF_PARTITIONS > 1u)
  /* #10 Check initialization state */
  if (partition >= Csm_GetSizeOfPartitionIdentifiers())
  {
    errorId = CSM_E_INVALID_PARTITION;
  }
  else
# endif /* (CSM_NUMBER_OF_PARTITIONS > 1u) */
  if (Csm_GetPartitionDataInitialized(partition) == (uint8)CSM_INITIALIZED)
  {
    errorId = CSM_E_ALREADY_INITIALIZED;
  }
  else
#endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
#if (CSM_JOB == STD_ON)
    uint32_least index;
    P2VAR(Crypto_JobType, AUTOMATIC, AUTOMATIC) job;
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize job input/output buffer */
    for (index = 0u; index < Csm_GetSizeOfJob(partition); index++)
    {
      job = Csm_GetAddrJob(index, partition);
      job->jobState = CRYPTO_JOBSTATE_IDLE; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.inputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.inputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.secondaryInputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.secondaryInputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.tertiaryInputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.tertiaryInputLength = 0u; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.outputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.outputLengthPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.secondaryOutputPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.secondaryOutputLengthPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.verifyPtr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
# if (CSM_JOB_TYPE_LAYOUT_INPUT64 == STD_ON)
      job->jobPrimitiveInputOutput.input64 = 0u; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
# endif
# if (CSM_JOB_TYPE_LAYOUT_OUTPUT64_PTR == STD_ON)
      job->jobPrimitiveInputOutput.output64Ptr = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
# endif
      job->jobPrimitiveInputOutput.mode = 0u; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->cryptoKeyId = 0u; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPriority = 0u; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
# if (CSM_JOB_TYPE_LAYOUT_ASR430_COMPATIBILITY == STD_ON)
      job->PrimitiveInputOutput = job->jobPrimitiveInputOutput; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->state = CRYPTO_JOBSTATE_IDLE; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
# endif

      /* The following initialization is not necessarily needed and done for consistency. */
      job->jobPrimitiveInputOutput.cryIfKeyId = 0u;   /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInputOutput.targetCryIfKeyId = 0u; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->jobPrimitiveInfo = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
      job->targetCryptoKeyId = 0u;  /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */

# if (CSM_JOB_TYPE_LAYOUT_REDIRECTION_INFO_REF == STD_ON) && (CSM_JOB_TYPE_LAYOUT_FIELD_ORDERING == CSM_ASR_VERSION_R21_11)
      job->jobRedirectionInfoRef = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
# endif
# if (CSM_JOB_TYPE_LAYOUT_REDIRECTION_INFO_REF == STD_ON) && (CSM_JOB_TYPE_LAYOUT_FIELD_ORDERING == CSM_ASR_VERSION_4_03)
      job->jobRedirectionInfoRef = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
# endif
# if (CSM_JOB_TYPE_LAYOUT_JOB_INFO_PTR == STD_ON)
      job->jobInfo = NULL_PTR; /* SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION */
# endif
    }

    for (index = 0u; index < Csm_GetSizeOfJobTable(); index++)
    {
# if (CSM_NUMBER_OF_PARTITIONS > 1u)
      /* #21 Check if the job relates to the current application */
      if (Csm_GetPartitionOfJob((uint32)index) == partition)
# endif
      {
# if (CSM_JOBIDXOFJOBTABLE == STD_ON)
#  if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
        if (Csm_IsJobUsedOfJobTable(index) == TRUE)
#  endif
        {
          job = Csm_GetAddrJob(Csm_GetJobIdxOfJobTable(index), partition);
          job->jobPrimitiveInfo = Csm_GetAddrJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(index)); /* SBSW_CSM_WRITE_TO_JOB_BY_JOBTABLE */
#  if (CSM_JOB_TYPE_LAYOUT_JOB_INFO_PTR == STD_ON)
          job->jobInfo = Csm_GetAddrJobInfo(Csm_GetJobInfoIdxOfJobTable(index)); /* SBSW_CSM_WRITE_TO_JOB_BY_JOBTABLE */
#  endif
          job->jobId = (uint32)index; /* SBSW_CSM_WRITE_TO_JOB_BY_JOBTABLE */
          job->jobPriority = Csm_GetPriorityOfJobTable(index); /* SBSW_CSM_WRITE_TO_JOB_BY_JOBTABLE */
        }
# endif /* (CSM_JOBIDXOFJOBTABLE == STD_ON) */
        Csm_SetJobState(index, CSM_JOB_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
      }
    }

    /* #30 Init queues */
    for (index = 0u; index < Csm_GetSizeOfQueueInfo(); index++)
    {
# if (CSM_NUMBER_OF_PARTITIONS > 1u)
      /* Check if the queue relates to the current application */
      if (Csm_GetPartitionConfigIdxOfQueueInfo(index) == partition)
# endif
      {
        Csm_SetQueueIdxOfQueueState(index, Csm_GetQueueStartIdxOfQueueInfo(index), partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_PARTITION_DEPENDENT_SEARCHED */
        Csm_SetLockOfQueueState(index, 0u, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_PARTITION_DEPENDENT_SEARCHED */

# if ((CSM_ASYNC_PROCESSING == STD_ON) && (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON))
        if (Csm_IsTriggerAsynchJobsInCallbackOfQueueInfo(index)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
        {
          Csm_SetRetriggeringOfQueueState(index, CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_QUEUESTATE_PARTITION_DEPENDENT_SEARCHED */
        }
# endif
      }
    }

# if (CSM_CALLOUT == STD_ON)
    /* Initialize Callout State */
    for (index = 0u; index < Csm_GetSizeOfCalloutState(partition); index++)
    {
      Csm_SetCalloutStateOfCalloutState(index, CSM_CALLOUT_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION */
    }
# endif /* (CSM_CALLOUT == STD_ON) */

# if (CSM_RTECALLBACK == STD_ON)
    Csm_SetRteCallbackOccurred(FALSE, partition); /* SBSW_CSM_CSL_VAR_ACCESS */
    for (index = 0u; index < Csm_GetSizeOfRteResult(partition); index++)
    {
      Csm_SetRteResult(index, CSM_CALLBACK_IDLE, partition); /* SBSW_CSM_WRITE_TO_RTERESULT_BY_SIZE */
    }
# endif /* (CSM_RTECALLBACK == STD_ON) */

#endif /* (CSM_JOB == STD_ON) */

#if (CSM_DEV_ERROR_DETECT == STD_ON)
    /* #40 Set CSM initialization state to initialized */
    Csm_SetPartitionDataInitialized(CSM_INITIALIZED, partition); /* SBSW_CSM_WRITE_PARTITION_DATA */
#endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */

  }
  /* ----- Development Error Report --------------------------------------- */
#if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError(CSM_MODULE_ID, CSM_INSTANCE_ID, CSM_INIT_ID, errorId);
  }
#else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */
#if ((CSM_DEV_ERROR_DETECT == STD_ON) || (CSM_JOB == STD_ON))
  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* Csm_Init() */

#if (CSM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Csm_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CSM_CODE) Csm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CSM_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  else
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (uint16)CSM_VENDOR_ID; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->moduleID = (uint8)CSM_MODULE_ID; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_major_version = (uint8)CSM_SW_MAJOR_VERSION; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_minor_version = (uint8)CSM_SW_MINOR_VERSION; /* SBSW_CSM_GETVERSIONINFO */
    versioninfo->sw_patch_version = (uint8)CSM_SW_PATCH_VERSION; /* SBSW_CSM_GETVERSIONINFO */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)CSM_GETVERSIONINFO_ID, errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */
} /* Csm_GetVersionInfo() */
#endif /* (CSM_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  Csm_MainFunction_Local()
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
/* PRQA S 6080, 6030 1 */ /* MD_MSR_STMIF */ /* MD_MSR_STCYC */
FUNC(void, CSM_CODE) Csm_MainFunction_Local(Csm_SizeOfPartitionIdentifiersType partition)
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  if (partition >= Csm_GetSizeOfPartitionIdentifiers())
  {
    errorId = CSM_E_INVALID_PARTITION;
  }
  /* #10 Check initialization state */
  else if (Csm_GetPartitionDataInitialized(partition) != CSM_INITIALIZED)
  {
    errorId = CSM_E_UNINIT;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
# if (CSM_ASYNC_PROCESSING == STD_ON)

    Csm_QueueInfoIterType queueInfoIdx;
#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
    Csm_QueueIterType triggerCounter;
#  endif

    /* ----- Implementation ----------------------------------------------- */

    /* #20 Loop over configured queues and trigger asynchronous job processing */
    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    for (queueInfoIdx = 0u; queueInfoIdx < Csm_GetSizeOfQueueInfo(); queueInfoIdx++)
    {
#  if (CSM_NUMBER_OF_PARTITIONS > 1u)
      if (Csm_GetPartitionConfigIdxOfQueueInfo(queueInfoIdx) == partition)
#  endif
      {
#  if (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON)
        if (Csm_IsTriggerAsynchJobsInCallbackOfQueueInfo(queueInfoIdx)) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
        {
          /* #30 Check whether retriggering of asynchronous job processing has halted. If so, trigger it again. */
          /* The following difference is calculated from generated table values and assumed to be always greater or equal than 0 */
          for (triggerCounter = 0u; triggerCounter < ((Csm_QueueIterType)(Csm_GetQueueEndIdxOfQueueInfo(queueInfoIdx)) - (Csm_QueueIterType)(Csm_GetQueueStartIdxOfQueueInfo(queueInfoIdx))); triggerCounter++) /* FETA_CSM_STRICTLY_INCREASING_BY_ONE */
          {
            Csm_TriggerAsynchJobProcessing(queueInfoIdx);

            if (Csm_GetRetriggeringOfQueueState(queueInfoIdx, partition) == CSM_ASYNC_JOB_RETRIGGER_STATE_IDLE)
            {
              break;
            }
          }
        }
        else
#  endif /* (CSM_TRIGGERASYNCHJOBSINCALLBACKOFQUEUEINFO == STD_ON) */
        {
          Csm_TriggerAsynchJobProcessing(queueInfoIdx);
        }
      }
    }
    SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

    /* #40 Handle callback notifications if processing has been aborted by callouts */
#  if (CSM_CALLOUT == STD_ON)
    {
      Csm_CalloutInfoIterType calloutInfoIdx;
      for (calloutInfoIdx = 0u; calloutInfoIdx < Csm_GetSizeOfCalloutInfo(); calloutInfoIdx++)
      {
        if (Csm_GetCalloutStateOfCalloutState(calloutInfoIdx, partition) == CSM_CALLOUT_STATE_PROCESSING_ABORTED_BY_CALLOUT)
        {
          Csm_CallbackNotification(Csm_GetAddrJob(Csm_GetJobToObjMap(Csm_GetJobTableIdxOfCalloutInfo(calloutInfoIdx), partition), partition), E_NOT_OK); /* SBSW_CSM_PASS_JOB_POINTER_CSL03 */
        }
      }
    }
#  endif /* (CSM_CALLOUT == STD_ON) */

#  if (CSM_RTECALLBACK == STD_ON)
    Csm_HandleRteCallbacks(partition);
#  endif

# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MAINFUNCTION_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */
#endif /* (CSM_JOB == STD_ON) */

  CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
} /* Csm_MainFunction_Local() */

/**********************************************************************************************************************
 *  Csm_CallbackNotification()
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
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CSM_CODE) Csm_CallbackNotification(
  P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  Crypto_ResultType result)
{
#if (CSM_JOB == STD_ON) && (CSM_ASYNC_PROCESSING == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CSM_E_NO_ERROR;
  uint8 runtimeErrorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* Verify the job pointer */
  if (job == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
  /* Ignore not used sharing jobs */
  if (job->jobId == CSM_EMPTY_JOB)
  {
    /* Ignore Call, job is not used anymore */
    runtimeErrorId = CSM_E_UNEXPECTED_CALLBACK;
  }
  else
# endif
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* Verify provided jobId */
  if (job->jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  /* Verify initialization state */
  else if (Csm_VerifyInitAndPartition(job->jobId, &errorId) != E_OK) /* SBSW_CSM_PASS_JOBID_AND_STACK */
  {
    /* errorId already set in subfunction */
  }
  else if ((result & CSM_JOB_STATE_CALLBACK_OCCURRED) != 0u)
  {
    errorId = CSM_E_PARAM_METHOD_INVALID;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    Csm_HandleCallbackNotification(job, result, &errorId, &runtimeErrorId);  /* SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT_AND_STACK */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CALLBACKNOTIFICATION_ID), errorId);
  }

  if (runtimeErrorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportRuntimeError(CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CALLBACKNOTIFICATION_ID), runtimeErrorId);
  }
# else
  CSM_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(runtimeErrorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */
#else /* !(CSM_JOB == STD_ON) && (CSM_ASYNC_PROCESSING == STD_ON) */
  CSM_DUMMY_STATEMENT(job); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif /* (CSM_JOB == STD_ON) && (CSM_ASYNC_PROCESSING == STD_ON) */

} /* Csm_CallbackNotification() */

/* Csm Key Management Functions */
/**********************************************************************************************************************
 *  Csm_KeyElementSet()
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
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSet(
  uint32 keyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
  uint32 keyLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check for null pointers */
  if (keyPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyElementSet function */
    retVal = CryIf_KeyElementSet(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, keyPtr, keyLength); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTSET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT_CONST(keyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyElementSet() */

/**********************************************************************************************************************
 *  Csm_KeySetValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetValid(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check key range */
  if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeySetValid function */
    retVal = CryIf_KeySetValid(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYSETVALID_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeySetValid() */

/**********************************************************************************************************************
 *  Csm_KeySetInvalid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetInvalid(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check key range */
  if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeySetInvalid function */
    retVal = CryIf_KeySetInvalid(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYSETINVALID_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeySetInvalid() */

/**********************************************************************************************************************
 *  Csm_KeyGetStatus()
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
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGetStatus(
  uint32 keyId,
  P2VAR(Crypto_KeyStatusType, AUTOMATIC, CSM_APPL_VAR) keyStatusPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check for null pointers */
  if (keyStatusPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyGetStatus function */
    retVal = CryIf_KeyGetStatus(Csm_GetCryIfKeyIdOfKey(keyId), keyStatusPtr); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYGETSTATUS_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyStatusPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyGetStatus() */

/**********************************************************************************************************************
 *  Csm_KeyElementGet()
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
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGet(
  uint32 keyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check for null pointers */
  if ((keyPtr == NULL_PTR)
         || (keyLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyElementGet function */
    retVal = CryIf_KeyElementGet(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, keyPtr, keyLengthPtr); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTGET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyElementGet() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopy(
  uint32 keyId,
  uint32 keyElementId,
  uint32 targetKeyId,
  uint32 targetKeyElementId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check key range */
  if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyElementCopy function */
    retVal = CryIf_KeyElementCopy(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, Csm_GetCryIfKeyIdOfKey(targetKeyId), targetKeyElementId);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTCOPY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyElementCopy() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopyPartial()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopyPartial(
  uint32 keyId,
  uint32 keyElementId,
  uint32 keyElementSourceOffset,
  uint32 keyElementTargetOffset,
  uint32 keyElementCopyLength,
  uint32 targetKeyId,
  uint32 targetKeyElementId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check key range */
  if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyElementCopyPartial function */
    retVal = CryIf_KeyElementCopyPartial(Csm_GetCryIfKeyIdOfKey(keyId), keyElementId, keyElementSourceOffset, keyElementTargetOffset, keyElementCopyLength, Csm_GetCryIfKeyIdOfKey(targetKeyId), targetKeyElementId);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYELEMENTCOPYPARTIAL_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementSourceOffset); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementTargetOffset); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(keyElementCopyLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyElementId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_KEY == STD_ON) */
} /* Csm_KeyElementCopyPartial() */

/**********************************************************************************************************************
 *  Csm_KeyCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopy(
  uint32 keyId,
  uint32 targetKeyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check key range */
  if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyCopy function */
    retVal = CryIf_KeyCopy(Csm_GetCryIfKeyIdOfKey(keyId), Csm_GetCryIfKeyIdOfKey(targetKeyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYCOPY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyCopy() */

/**********************************************************************************************************************
 *  Csm_RandomSeed()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomSeed(
  uint32 keyId,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
  uint32 seedLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check for null pointers */
  if (seedPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_RandomSeed function */
    retVal = CryIf_RandomSeed(Csm_GetCryIfKeyIdOfKey(keyId), seedPtr, seedLength); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_RANDOMSEED_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT_CONST(seedPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(seedLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_RandomSeed() */

/**********************************************************************************************************************
 *  Csm_KeyGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerate(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check key range */
  if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyGenerate function */
    retVal = CryIf_KeyGenerate(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyGenerate() */

/**********************************************************************************************************************
 *  Csm_KeyDerive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDerive(
  uint32 keyId,
  uint32 targetKeyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check key range */
  if ((keyId >= Csm_GetSizeOfKey()) || (targetKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_KeyDerive function */
    retVal = CryIf_KeyDerive(Csm_GetCryIfKeyIdOfKey(keyId), Csm_GetCryIfKeyIdOfKey(targetKeyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYDERIVE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(targetKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyDerive() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubVal()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubVal(
  uint32 keyId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check for null pointers */
  if ((publicValuePtr == NULL_PTR)
         || (publicValueLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyExchangeCalcPubVal function */
    retVal = CryIf_KeyExchangeCalcPubVal(Csm_GetCryIfKeyIdOfKey(keyId), publicValuePtr, publicValueLengthPtr); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYEXCHANGECALCPUBVAL_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(publicValuePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(publicValueLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyExchangeCalcPubVal() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecret()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecret(
  uint32 keyId,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
  uint32 partnerPublicValueLength)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check for null pointers */
  if (partnerPublicValuePtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_KeyExchangeCalcSecret function */
    retVal = CryIf_KeyExchangeCalcSecret(Csm_GetCryIfKeyIdOfKey(keyId), partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_KEYEXCHANGECALCSECRET_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(partnerPublicValuePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(partnerPublicValueLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_KeyExchangeCalcSecret() */

/**********************************************************************************************************************
 *  Csm_CertificateParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParse(uint32 keyId)
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check key range */
  if (keyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Call of CryIf_CertificateParse function */
    retVal = CryIf_CertificateParse(Csm_GetCryIfKeyIdOfKey(keyId));
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CERTIFICATEPARSE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_CertificateParse() */

/**********************************************************************************************************************
 *  Csm_CertificateVerify()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerify(
  uint32 keyId,
  uint32 verifyKeyId,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_KEY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check for null pointers */
  if (verifyPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #30 Check key range */
  else if ((keyId >= Csm_GetSizeOfKey()) || (verifyKeyId >= Csm_GetSizeOfKey()))
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #40 Call of CryIf_CertificateVerify function */
    retVal = CryIf_CertificateVerify(Csm_GetCryIfKeyIdOfKey(keyId), Csm_GetCryIfKeyIdOfKey(verifyKeyId), verifyPtr); /* SBSW_CSM_FORWARDING_PTR */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CERTIFICATEVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_KEY == STD_ON) */
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyKeyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_KEY == STD_ON) */
} /* Csm_CertificateVerify() */

/**********************************************************************************************************************
 *  Csm_CancelJob()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJob(
  uint32 jobId,
  Crypto_OperationModeType mode)
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided jobId */
  if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  /* #20 Verify initialization state */
  else if (Csm_VerifyInitAndPartition(jobId, &errorId) != E_OK) /* SBSW_CSM_PASS_JOBID_AND_STACK */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);
    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();

    if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_IDLE)
    {
      /* #30 If Job is idle  - No canceling needed */
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      retVal =  E_OK;
    }
    else
    {
      /* Job is running, object exists */
      P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job = Csm_GetAddrJob(Csm_GetJobToObjMap(jobId, partition), partition);

# if (CSM_ASYNC_PROCESSING == STD_ON)
      if (Csm_IsAsynchronousOfJobTable(jobId))
      {
        retVal = Csm_CancelAsynchronousJob(job, partition); /* SBSW_CSM_PASS_JOB_AND_PARTITION */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      }
      else
# endif /* (CSM_ASYNC_PROCESSING == STD_ON) */
      if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_ACTIVE)
      {
        Csm_SetJobState(jobId, CSM_JOB_STATE_CANCELING, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();

        /* #55 Cancel Job in lower layer */
        retVal = CryIf_CancelJob(Csm_GetChannelIdOfQueueInfo(Csm_GetQueueInfoIdxOfJobTable(jobId)), job); /* SBSW_CSM_PASS_JOB */

        if (retVal == E_OK)
        {
          /* #56 Canceling successful - Call post-job callout and free job oject */
# if (CSM_CALLOUT == STD_ON)
          /* retVal only acts as information for post job callout that job was cancelled.
           * Set it back to E_OK in each case as canceling was already performed. */
          retVal = CRYPTO_E_JOB_CANCELED;
          Csm_CallPostJobCallout(job, &retVal, partition); /* SBSW_CSM_PASS_JOB_STACK_AND_PARTITION */
          retVal = E_OK;
# endif
# if (CSM_JOBSHARINGOFQUEUEINFO == STD_ON)
          job->jobId = CSM_EMPTY_JOB; /* SBSW_CSM_WRITE_TO_JOB_POINTER */
# endif
          Csm_SetJobState(jobId, CSM_JOB_STATE_IDLE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
        }
        else
        {
          /* #57 Canceling failed, set active again */
          Csm_SetJobState(jobId, CSM_JOB_STATE_ACTIVE, partition); /* SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION */
        }
      }
      else
      {
        /* #60 Job cancelation is not possible as the job is currently in or on the way to the lower layer.
         *     If job is currently on the way to the lower layer, Crypto would return E_OK as it does not know the job yet
         *     Due to this, Csm cannot rely on the return value.
         */
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
        /* retVal already set to E_NOT_OK */
      }
    }
# if (CSM_NUMBER_OF_PARTITIONS <= 1u)
    CSM_DUMMY_STATEMENT(partition); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* (CSM_NUMBER_OF_PARTITIONS <= 1u) */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_CANCELJOB_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return retVal;
#else /* !(CSM_JOB == STD_ON) */

  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_JOB == STD_ON) */
} /* Csm_CancelJob() */

/* Csm Service Functions */

/**********************************************************************************************************************
 *  Csm_Hash()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_Hash(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_HASH == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_HASH) != E_OK)  /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_HASH_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_HASH == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_HASH == STD_ON) */
} /* Csm_Hash() */

/**********************************************************************************************************************
 *  Csm_MacGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerate(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_MACGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_MACGENERATE) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = macPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = macLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MACGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_MACGENERATE == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(macPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(macLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_MACGENERATE == STD_ON) */
} /* Csm_MacGenerate() */

/**********************************************************************************************************************
 *  Csm_MacVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerify(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
  uint32 macLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_MACVERIFY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_MACVERIFY) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryInputPtr = macPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryInputLength = macLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_MACVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_MACVERIFY == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(macPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(macLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_MACVERIFY == STD_ON) */
} /* Csm_MacVerify() */

/**********************************************************************************************************************
 *  Csm_Encrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_Encrypt(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_ENCRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_ENCRYPT) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_ENCRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_ENCRYPT == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_ENCRYPT == STD_ON) */
} /* Csm_Encrypt() */

/**********************************************************************************************************************
 *  Csm_Decrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_Decrypt(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_DECRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_DECRYPT) != E_OK)  /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_DECRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_DECRYPT == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_DECRYPT == STD_ON) */
} /* Csm_Decrypt() */

/**********************************************************************************************************************
 *  Csm_AEADEncrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncrypt(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
  uint32 plaintextLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
  uint32 associatedDataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,  /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,                /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr)         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_AEADENCRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_AEADENCRYPT) != E_OK)  /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = plaintextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = plaintextLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryInputPtr = associatedDataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryInputLength = associatedDataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = ciphertextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = ciphertextLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryOutputPtr = tagPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryOutputLengthPtr = tagLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_AEADENCRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_AEADENCRYPT == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(plaintextPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(plaintextLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(associatedDataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(associatedDataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(ciphertextPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(ciphertextLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(tagPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(tagLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_AEADENCRYPT == STD_ON) */
} /* Csm_AEADEncrypt() */

/**********************************************************************************************************************
 *  Csm_AEADDecrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecrypt(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
  uint32 ciphertextLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
  uint32 associatedDataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
  uint32 tagLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,                /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_AEADDECRYPT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_AEADDECRYPT) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = ciphertextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = ciphertextLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryInputPtr = associatedDataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryInputLength = associatedDataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.tertiaryInputPtr = tagPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.tertiaryInputLength = tagLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = plaintextPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = plaintextLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_AEADDECRYPT_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_AEADDECRYPT == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(ciphertextPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(ciphertextLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(associatedDataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(associatedDataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(tagPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(tagLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(plaintextPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(plaintextLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_AEADDECRYPT == STD_ON) */
} /* Csm_AEADDecrypt() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerate(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_SIGNATUREGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_SIGNATUREGENERATE) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_SIGNATUREGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_SIGNATUREGENERATE == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_SIGNATUREGENERATE == STD_ON) */
} /* Csm_SignatureGenerate() */

/**********************************************************************************************************************
 *  Csm_SignatureVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerify(
  uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
  uint32 signatureLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_SIGNATUREVERIFY == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_SIGNATUREVERIFY) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = mode; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = dataPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = dataLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryInputPtr = signaturePtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.secondaryInputLength = signatureLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.verifyPtr = verifyPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_SIGNATUREVERIFY_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_SIGNATUREVERIFY == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(dataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(signaturePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(signatureLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(verifyPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* !(CSM_EXIST_SIGNATUREVERIFY == STD_ON) */
} /* Csm_SignatureVerify() */

/**********************************************************************************************************************
 *  Csm_RandomGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerate(
  uint32 jobId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_RANDOMGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_RANDOMGENERATE) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = resultPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = resultLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_RANDOMGENERATE_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_RANDOMGENERATE == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(resultLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_RANDOMGENERATE == STD_ON) */
} /* Csm_RandomGenerate() */

/**********************************************************************************************************************
 *  Csm_JobKeySetValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeySetValid( uint32 jobId )
{
#if (CSM_EXIST_JOBKEYSETVALID == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_KEYSETVALID) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.cryIfKeyId = job->jobPrimitiveInfo->cryIfKeyId; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYSETVALID_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_JOBKEYSETVALID == STD_ON) */
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYSETVALID == STD_ON) */
} /* Csm_JobKeySetValid() */

/**********************************************************************************************************************
 *  Csm_JobKeySetValid44x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeySetValid44x( uint32 jobId, uint32 keyId )
{
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return Csm_JobKeySetValid(jobId);
} /* Csm_JobKeySetValid44x() */

/**********************************************************************************************************************
 *  Csm_JobKeySetInvalid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeySetInvalid(uint32 jobId)
{
#if (CSM_EXIST_JOBKEYSETINVALID == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_KEYSETINVALID) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.cryIfKeyId = job->jobPrimitiveInfo->cryIfKeyId; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYSETINVALID_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_JOBKEYSETINVALID == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYSETVALID == STD_ON) */
} /* Csm_JobKeySetInvalid() */

/**********************************************************************************************************************
 *  Csm_JobKeySetInvalid44x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeySetInvalid44x( uint32 jobId, uint32 keyId )
{
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return Csm_JobKeySetInvalid(jobId);
} /* Csm_JobKeySetInvalid44x() */

/**********************************************************************************************************************
 *  Csm_JobRandomSeed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobRandomSeed(
  uint32 jobId, P2CONST( uint8, AUTOMATIC, CSM_APPL_VAR ) seedPtr,
  uint32 seedLength)
{
#if (CSM_EXIST_JOBRANDOMSEED == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_RANDOMSEED) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.cryIfKeyId = job->jobPrimitiveInfo->cryIfKeyId; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = seedPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = seedLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBRANDOMSEED_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_JOBRANDOMSEED == STD_ON) */
  CSM_DUMMY_STATEMENT_CONST( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT_CONST( seedPtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( seedLength ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBRANDOMSEED == STD_ON) */
} /* Csm_JobRandomSeed() */

/**********************************************************************************************************************
 *  Csm_JobRandomSeed44x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobRandomSeed44x(
  uint32 jobId,
  uint32 keyId,
  P2CONST( uint8, AUTOMATIC, CSM_APPL_VAR ) seedPtr,
  uint32 seedLength )
{
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return Csm_JobRandomSeed(jobId, seedPtr, seedLength); /* SBSW_CSM_FORWARDING_PTR */
} /* Csm_JobRandomSeed44x() */

/**********************************************************************************************************************
 *  Csm_JobKeyGenerate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyGenerate( uint32 jobId )
{
#if (CSM_EXIST_JOBKEYGENERATE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_KEYGENERATE) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.cryIfKeyId = job->jobPrimitiveInfo->cryIfKeyId; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYGENERATE_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_JOBKEYGENERATE == STD_ON) */
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYGENERATE == STD_ON) */
} /* Csm_JobKeyGenerate() */

/**********************************************************************************************************************
 *  Csm_JobKeyGenerate44x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyGenerate44x( uint32 jobId, uint32 keyId )
{
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return Csm_JobKeyGenerate(jobId);
} /* Csm_JobKeyGenerate44x() */

/**********************************************************************************************************************
 *  Csm_JobKeyDerive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyDerive(
  uint32 jobId,
  uint32 targetKeyId)
{
#if (CSM_EXIST_JOBKEYDERIVE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_KEYDERIVE) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else if (targetKeyId >= Csm_GetSizeOfKey())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.cryIfKeyId = job->jobPrimitiveInfo->cryIfKeyId; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.targetCryIfKeyId = Csm_GetCryIfKeyIdOfKey( targetKeyId ); /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYDERIVE_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_JOBKEYDERIVE == STD_ON) */
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( targetKeyId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYDERIVE == STD_ON) */
} /* Csm_JobKeyDerive() */

/**********************************************************************************************************************
 *  Csm_JobKeyDerive44x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyDerive44x( uint32 jobId, uint32 keyId, uint32 targetKeyId )
{
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return Csm_JobKeyDerive(jobId, targetKeyId);
} /* Csm_JobKeyDerive44x() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubVal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyExchangeCalcPubVal(
  uint32 jobId,
  P2VAR( uint8, AUTOMATIC, CSM_APPL_VAR ) publicValuePtr,         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR( uint32, AUTOMATIC, CSM_APPL_VAR ) publicValueLengthPtr ) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_EXIST_JOBKEYEXCHANGECALCPUBVAL == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_KEYEXCHANGECALCPUBVAL) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.cryIfKeyId = job->jobPrimitiveInfo->cryIfKeyId; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputPtr = publicValuePtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.outputLengthPtr = publicValueLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYEXCHANGECALCPUBVAL_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_JOBKEYEXCHANGECALCPUBVAL == STD_ON) */
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( publicValuePtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( publicValueLengthPtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYEXCHANGECALCPUBVAL == STD_ON) */
} /* Csm_JobKeyExchangeCalcPubVal() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubVal44x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyExchangeCalcPubVal44x(
  uint32 jobId,
  uint32 keyId,
  P2VAR( uint8, AUTOMATIC, CSM_APPL_VAR ) publicValuePtr,         /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR( uint32, AUTOMATIC, CSM_APPL_VAR ) publicValueLengthPtr ) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return Csm_JobKeyExchangeCalcPubVal(jobId, publicValuePtr, publicValueLengthPtr); /* SBSW_CSM_FORWARDING_PTR */
} /* Csm_JobKeyExchangeCalcPubVal44x() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecret()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 */ /* MD_MSR_STMIF */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyExchangeCalcSecret(
  uint32 jobId,
  P2CONST( uint8, AUTOMATIC, CSM_APPL_VAR ) partnerPublicValuePtr,
  uint32 partnerPublicValueLength )
{
#if (CSM_EXIST_JOBKEYEXCHANGECALCSECRET == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id, service and partition */
  if (Csm_VerifyJobParam(jobId, &errorId, CRYPTO_KEYEXCHANGECALCSECRET) != E_OK) /* SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST */
  {
    /* errorId already set in subfunction */
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SINGLECALL; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.cryIfKeyId = job->jobPrimitiveInfo->cryIfKeyId; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = partnerPublicValuePtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = partnerPublicValueLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError( (uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_JOBKEYEXCHANGECALCSECRET_ID), errorId );
  }
# else
  CSM_DUMMY_STATEMENT( errorId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_EXIST_JOBKEYEXCHANGECALCSECRET == STD_ON) */
  CSM_DUMMY_STATEMENT( jobId ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( partnerPublicValuePtr ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT( partnerPublicValueLength ); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_EXIST_JOBKEYEXCHANGECALCSECRET == STD_ON) */
} /* Csm_JobKeyExchangeCalcSecret() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecret44x()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC( Std_ReturnType, CSM_CODE ) Csm_JobKeyExchangeCalcSecret44x(
  uint32 jobId,
  uint32 keyId,
  P2CONST( uint8, AUTOMATIC, CSM_APPL_VAR ) partnerPublicValuePtr,
  uint32 partnerPublicValueLength )
{
  CSM_DUMMY_STATEMENT(keyId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

  return Csm_JobKeyExchangeCalcSecret(jobId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_FORWARDING_PTR */
} /* Csm_JobKeyExchangeCalcSecret44x() */

/**********************************************************************************************************************
 *  Csm_SaveContextJob()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_SaveContextJob(
  uint32 jobId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) contextBufferLengthPtr ) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided job id */
  if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  /* #11 Verify initialization states */
  else if (Csm_VerifyInitAndPartition(jobId, &errorId) != E_OK) /* SBSW_CSM_PASS_JOBID_AND_STACK */
  {
    /* Error already set in subfunction */
  }
  /* #12 Check for Null Pointer */
  else if ((contextBufferPtr == NULL_PTR) || (contextBufferLengthPtr == NULL_PTR))
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #13 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service > CRYPTO_SIGNATUREVERIFY)
  {
    errorId = CSM_E_SERVICE_TYPE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    /* #14 Check if the job state is active */
    if (Csm_GetJobState(jobId, partition) == CSM_JOB_STATE_IDLE)
    {
      /* retVal is already set to E_NOT_OK */
    }
    else
    {
      SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();

      /* #20 Get and fill job object */
      retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
      if (retVal == E_OK)
      {
        job = Csm_GetAddrJob(jobObjId, partition);

        job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_SAVE_CONTEXT; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
        job->jobPrimitiveInputOutput.outputPtr = contextBufferPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
        job->jobPrimitiveInputOutput.outputLengthPtr = contextBufferLengthPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

        /* #21 Process job */
        retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
        /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
      }
      else
      {
        SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
      }
    }

  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_SAVECONTEXTJOB_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_JOB == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(contextBufferPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(contextBufferLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_JOB == STD_ON) */
} /* Csm_SaveContextJob() */

/**********************************************************************************************************************
 *  Csm_RestoreContextJob()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CSM_CODE) Csm_RestoreContextJob(
  uint32 jobId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  uint32 contextBufferLength )
{
#if (CSM_JOB == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CSM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CSM_DEV_ERROR_DETECT == STD_ON)
  /* #10 Verify provided jobId */
  if (jobId >= Csm_GetSizeOfJobTable())
  {
    errorId = CSM_E_PARAM_HANDLE;
  }
  /* #11 Verify initialization states */
  else if (Csm_VerifyInitAndPartition(jobId, &errorId) != E_OK) /* SBSW_CSM_PASS_JOBID_AND_STACK */
  {
    /* Error already set in subfunction */
  }
  /* #12 Check for Null Pointer */
  else if (contextBufferPtr == NULL_PTR)
  {
    errorId = CSM_E_PARAM_POINTER;
  }
  /* #13 Check if the correct API was called for the corresponding service */
  else if (Csm_GetJobPrimitiveInfo(Csm_GetJobPrimitiveInfoIdxOfJobTable(jobId)).primitiveInfo->service > CRYPTO_SIGNATUREVERIFY)
  {
    errorId = CSM_E_SERVICE_TYPE;
  }
  else
# endif /* (CSM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    P2VAR(Crypto_JobType, AUTOMATIC, CSM_APPL_VAR) job;
    Csm_SizeOfJobType jobObjId = 0u;
    Csm_SizeOfPartitionIdentifiersType partition = Csm_GetPartitionOfJob(jobId);

    SchM_Enter_Csm_CSM_EXCLUSIVE_AREA_0();
    /* #20 Get and fill job object */
    retVal = Csm_GetJobObj(jobId, partition, &jobObjId); /* SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE */
    if (retVal == E_OK)
    {
      job = Csm_GetAddrJob(jobObjId, partition);

      job->jobPrimitiveInputOutput.mode = CRYPTO_OPERATIONMODE_RESTORE_CONTEXT; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputPtr = contextBufferPtr; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */
      job->jobPrimitiveInputOutput.inputLength = contextBufferLength; /* SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ */

      /* #21 Process job */
      retVal = Csm_ProcessJob(job, partition); /* SBSW_CSM_PASS_JOB */
      /* SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0 is left in Csm_ProcessJob */
    }
    else
    {
      SchM_Exit_Csm_CSM_EXCLUSIVE_AREA_0();
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CSM_E_NO_ERROR)
  {
    (void)Det_ReportError((uint16)CSM_MODULE_ID, CSM_INSTANCE_ID, (uint8)(CSM_RESTORECONTEXTJOB_ID), errorId);
  }
# else
  CSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* !(CSM_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
#else /* !(CSM_JOB == STD_ON) */
  CSM_DUMMY_STATEMENT(jobId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(contextBufferPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CSM_DUMMY_STATEMENT(contextBufferLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif /* (CSM_JOB == STD_ON) */
} /* Csm_RestoreContextJob() */

#define CSM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Module specific MISRA deviations
 *********************************************************************************************************************/
/* Module specific MISRA deviations:
  MD_CSM_QUEUE_BY_QUEUEIDX:
    Reason:     If Queue variable has only a size of 1, accessing with any index + 1 causes this message.
    Risk:       Access could be executed causing an invalid memory access.
    Prevention: During runtime, loop is only entered if Queue is at least two since index is checked against localQueueIndex. Covered by code review.

  MD_CSM_0316:
    Reason:     Cast is needed since upper layer Rte expects super type as parameter.
    Risk:       Wrong type is casted or size does not match.
    Prevention: Covered by code review.

  MD_CSM_CALLBACK_DECLARATIONS:
    Description:  Rule 8.5: An external object or function shall be declared once in one and only one file
    Reason:       MICROSAR Csm declares its callbacks and pre and post job callouts in its Cfg.h. Modules that also declares them see a double declaration.
    Risk:         Compile warning or error due to declaration mismatch.
    Prevention:   Not necessary
 */

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_CSM_GETVERSIONINFO
    \DESCRIPTION The function Csm_GetVersionInfo writes to the object referenced by parameter VersionInfoPtr
    \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameter VersionInfoPtr is valid.

  \ID SBSW_CSM_FUNCTION_POINTER
    \DESCRIPTION    The API call is made using a function pointer which is read using ComStackLib macros.
                    Pointer might be forwarded.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       The pointer are forwarded and assumed to be valid.

  \ID SBSW_CSM_FORWARDING_PTR
    \DESCRIPTION    Passing received pointer to called function.
    \COUNTERMEASURE \N The pointer is forwarded and assumed to be valid.

  \ID SBSW_CSM_PASS_UNCHECKED_JOBID_STACKPTR_AND_CONST
    \DESCRIPTION    The function passes an unchecked job identifier to a subfunction for verification. Since an unchecked job identifier
                    is expected, this is safe. Furthermore a pointer to a stack variable of expected size is passed to a synchronous function.
                    The const value of the expected size but only used for comparision.
    \COUNTERMEASURE \N The job id is verified in the called function. The pointer to the stack variable is of expected size.
                       [CM_CSM_STACK_VARIABLE]

  \ID SBSW_CSM_PASS_CHECKED_JOBID_PARTITION_AND_STACK_VARIABLE
    \DESCRIPTION    The function passes a checked job id and a partition read by using job id (CSL03).
                    Furthermore, a pointer to a stack variable of expected size is passed to the synchronous function.
    \COUNTERMEASURE \R The job id was checked and the partition is read using the passed job id and ComStackLib macros (CSL03).
                       [CM_CSM_STACK_VARIABLE]

  \ID SBSW_CSM_PASS_JOB
    \DESCRIPTION    A job pointer is passed to the function. This pointer was originally determined by the given and
                    checked job id. This is done by accessing the job structure from the global job array via
                    indirection over JobTable or using one of the shared job objects. The shared job objects are
                    determined by accessing the QueueInfo via indirection over the JobTable. Having the QueueInfo index,
                    accesses to the global job strucure is done via indirection of QueueInfo. Therefore the pointer always
                    points to a valid job structure, that is safe to pass to an asynchronous function.
    \COUNTERMEASURE \N The job pointer is set using the passed Id and ComStackLib macros (CSL03).

  \ID SBSW_CSM_PASS_JOB_AND_PARTITION
    \DESCRIPTION    A job pointer is passed to the function. This pointer was originally determined by the given and
                    checked job id. This is done by accessing the job structure from the global job array via
                    indirection over JobTable or using one of the shared job objects. The shared job objects are
                    determined by accessing the QueueInfo via indirection over the JobTable. Having the QueueInfo index,
                    accesses to the global job strucure is done via indirection of QueueInfo. Therefore the pointer always
                    points to a valid job structure, that is safe to pass to an asynchronous function
                    The partition is derived from the job id.
    \COUNTERMEASURE \N The job pointer is set using the passed Id and ComStackLib macros (CSL03).
                    The index for the Partition specific data is either fixed (for single partition use case) or provided by searching the current application id
                    in the PartitionIdentifiers and using its corresponding partitionIdx (Qualified use-case CSL05 of ComStackLib).
                    If no partitionIdx can be found, usage of an invalid index is prevented by checking the index to be in acceptable range.
                    Therefore it is always guaranteed to be in the acceptable range.

  \ID SBSW_CSM_PASS_JOBID_AND_STACK
    \DESCRIPTION    The function passes a checked job id and a pointer to a stack variable of expected size to a synchronous function.
    \COUNTERMEASURE \R The job id was checked if it is in range.
                       [CM_CSM_STACK_VARIABLE]

  \ID SBSW_CSM_PASS_JOB_STACK_AND_PARTITION
    \DESCRIPTION    The function passes a checked job pointer (see [SBSW_CSM_PASS_JOB]), a pointer to a stack
                    variable of expected size and a checked partition index to an synchronous function.
    \COUNTERMEASURE \N The function pointer is read using the passed Id and ComStackLib macros (CSL03).
                       [CM_CSM_STACK_VARIABLE]

  \ID SBSW_CSM_PASS_JOB_POINTER_LOCAL
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been initialized and points to a valid job object.
    \COUNTERMEASURE \N The pointer is initialized from a local queue. The queue is only filled with valid job objects and therefore always contains valid jobs except when it is empty. In this case, the function is not called.

  \ID SBSW_CSM_PASS_JOB_POINTER_AND_PARTITION_API_REQUIREMENT
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been passed by the caller. The API requires that the referenced job is valid.
                    The function passes a partition index. The passed partition index has been passed by the caller. The API requires that the partition index is valid.
    \COUNTERMEASURE \N The caller ensures that the pointer passed as parameter is valid and references a valid object.
                       The caller ensures that the partition index passed as parameter is valid and references a valid object.

   \ID SBSW_CSM_PASS_JOB_POINTER_PARTITION_AND_STACK_API_REQUIREMENT
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been passed by the caller. The API requires that the referenced job is valid.
                    The function passes a pointer to a stack variable of expected size to an synchronous function.
                    The function passes a partition index. The passed partition index has been passed by the caller. The API requires that the partition index is valid.
    \COUNTERMEASURE \N The caller ensures that the pointer passed as parameter is valid and references a valid object.
                       The passed stack variable is of expected size
                       The caller ensures that the partition index passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT_AND_STACK
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been passed by the caller. The API requires that the referenced job is valid.
                    The function also passes a pointer to a stack variable of expected size to an synchronous function.
    \COUNTERMEASURE \N The caller ensures that the pointer passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_PASS_JOB_POINTER_API_REQUIREMENT
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been passed by the caller. The API requires that the referenced job is valid.
    \COUNTERMEASURE \N The caller ensures that the pointer passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_PASS_JOB_POINTER_CSL03
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer is valid since it is an indirection to Job via CalloutInfo.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_CSM_PASS_CHANNEL_JOB_POINTER_AND_PARTITION_API_REQUIREMENT
    \DESCRIPTION    The function passes a pointer referencing a job. The passed pointer has been passed by the caller. The API requires that the referenced job is valid.
                    The function passes a partition index. The passed partition index has been passed by the caller. The API requires that the partition index is valid.
                    The channel id is retrieved by the ComStackLib from the qualified job, passed by the caller.
    \COUNTERMEASURE \N The caller ensures that the pointer passed as parameter is valid and references a valid object.
                       The caller ensures that the partition index passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_PASS_QUEUESTATEINDEX
    \DESCRIPTION    The function passes a queue state index qualified by size.
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_JOB_POINTER
    \DESCRIPTION    The function does a write access on the job pointer. See [SBSW_CSM_PASS_JOB] for checking validity of job pointer.
    \COUNTERMEASURE \R The pointer is checked when it was passed to the service.

  \ID SBSW_CSM_WRITE_TO_JOB_POINTER_FROM_JOBOBJ
    \DESCRIPTION    The function does a write access on the job pointer. The job pointer is retrieved from a job object by the ComStackLib.
                    The job object is qualified by a function.
                    The used partition index is retrieved by the ComStackLib.
    \COUNTERMEASURE \R Qualified use-case CSL 05 of ComStackLib
                    The job object is verified in the function "Csm_GetJobObj" and inside verified by CSL 02.

  \ID SBSW_WRITE_TO_PASSED_LOCAL_STACK_VARIABLE
    \DESCRIPTION    The function writes to a pointer which is passed by local caller and located on the stack.
    \COUNTERMEASURE \R The calling function ensures that the pointer passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE
    \DESCRIPTION Access of CalloutState using ComStackLib
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_CALLOUT_STATE_BY_SIZE_AND_PARTITION
    \DESCRIPTION Access of CalloutState using ComStackLib
                 The used partition index has been passed by the caller. The API requires that the partition index is valid.
    \COUNTERMEASURE \N Qualified use-case CSL02, CSL05 of ComStackLib
                 The caller ensures that the partition index passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_WRITE_TO_JOB_BY_SIZE_OF_PARTITION
    \DESCRIPTION  Access of Job using ComStackLib
                  Write of partition specific data using ComStackLib.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib
                  The index for the partition specific data is either fixed (for single partition use case) or provided by searching the current application id
                  in the partitionIdentifiers and using its corresponding partitionIdx (Qualified use-case CSL05 of ComStackLib).
                  If no partitionIdx can be found, usage of an invalid index is prevented by checking the index to be in acceptable range.
                  Therefore it is always guaranteed to be in the acceptable range.

  \ID SBSW_CSM_WRITE_TO_JOB_BY_JOBTABLE
    \DESCRIPTION Access to Job via indirection over JobTable.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_QUEUESTATE_BY_SIZE
    \DESCRIPTION Access of QueueState using ComStackLib
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_QUEUESTATE_API_REQ
    \DESCRIPTION Access of QueueState using passed parameters from the caller. The API requires those parameters to be valid.
    \COUNTERMEASURE \N  The caller ensures that the parameters passed are valid.

  \ID SBSW_CSM_WRITE_TO_QUEUESTATE_BY_JOBID_AND_PARTITION_API_REQ
    \DESCRIPTION  Access to QueueState via indirection over JobTable.
                  The used partition index has been passed by the caller. The API requires that the partition index is valid.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib
                  The caller ensures that the partition index passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_WRITE_TO_QUEUESTATE_PARTITION_DEPENDENT
    \DESCRIPTION Access of QueueState using multi-partition indirection over QueueInfo
    \COUNTERMEASURE \N  Qualified use-case CSL05 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_QUEUESTATE_PARTITION_DEPENDENT_SEARCHED
    \DESCRIPTION  Access of QueueState using ComStackLib
                  Write of partition specific data using ComStackLib.
    \COUNTERMEASURE \N  Qualified use-case CSL01 of ComStackLib for the queue state index
                  The index for the partition specific data is either fixed (for single partition use case) or provided by searching the current application id
                  in the partitionIdentifiers and using its corresponding partitionIdx (Qualified use-case CSL05 of ComStackLib).
                  If no partitionIdx can be found, usage of an invalid index is prevented by checking the index to be in acceptable range.
                  Therefore it is always guaranteed to be in the acceptable range.

  \ID SBSW_CSM_WRITE_TO_RTERESULT_BY_SIZE_API_REQ
    \DESCRIPTION  Access of RteCallback and RteResult with same index using ComStackLib
                  The used partition index has been passed by the caller. The API requires that the partition index is valid.
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib
                  The caller ensures that the partition index passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_WRITE_TO_RTERESULT_BY_SIZE
    \DESCRIPTION Access of RteCallback and RteResult with same index using ComStackLib
                 Write of partition specific data using ComStackLib.
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib
                 The index for the partition specific data is either fixed (for single partition use case) or provided by searching the current application id
                 in the partitionIdentifiers and using its corresponding partitionIdx (Qualified use-case CSL05 of ComStackLib).
                 If no partitionIdx can be found, usage of an invalid index is prevented by checking the index to be in acceptable range.
                 Therefore it is always guaranteed to be in the acceptable range.

  \ID SBSW_CSM_WRITE_TO_RTERESULT_BY_JOBID
    \DESCRIPTION Access to RteResult via indirection over JobTable.
                 The partition id is retrieved from the jobId by the ComStackLib
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_CSM_WRITE_TO_QUEUE_BY_LOCALIDX
    \DESCRIPTION Access to Queue via localQueueIndex which is always checked against its size via indirection over QueueInfo.
                 The used partition index has been passed by the caller. The API requires that the partition index is valid.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib
                 The caller ensures that the partition index passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_WRITE_TO_QUEUE_API_REQ
    \DESCRIPTION Access of the queue using passed parameters from the caller. The API requires those parameters to be valid.
    \COUNTERMEASURE \N The caller ensures that the parameters passed are valid.

  \ID SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION_API_REQ
    \DESCRIPTION  Access of JobState and JobTable with same index using ComStackLib
                  The used partition index has been passed by the caller. The API requires that the partition index is valid.
    \COUNTERMEASURE \N Qualified use-case CSL02, CSL05 of ComStackLib
                  The caller ensures that the partition index passed as parameter is valid and references a valid object.

  \ID SBSW_CSM_WRITE_TO_JOBSTATE_BY_JOBID_AND_PARTITION
    \DESCRIPTION  Access of JobState and JobTable with same index using ComStackLib
                  The used partition index is qualified to be in range of 0 to (Csm_GetSizeOfPartitionIdentifiers() - 1)
    \COUNTERMEASURE \N Qualified use-case CSL02, CSL05 of ComStackLib

  \ID SBSW_WRITE_TO_OBJECTMAP_BY_JOBID
    \DESCRIPTION  Access of ObjectMap and JobTable with same index using ComStackLib
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_CSM_CSL_VAR_ACCESS
    \DESCRIPTION  A ComStackLib generated variable is accessed.
                  The used partition index has been passed by the caller. The API requires that the partition index is valid.
    \COUNTERMEASURE \N The variable is written using a generated macro of the CSL. It immediately dereferences the variables address without pointer arithmetic.

  \ID SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER
    \DESCRIPTION    The API call is forwarded to an existing API function.
                    The API is called with one or more pointers passed from the wrapper API function.
    \COUNTERMEASURE \N The called functions in the wrapper functions are API functions that are supposed to be silent.

  \ID SBSW_CSM_WRITE_PARTITION_DATA
    \DESCRIPTION    Write of partition specific data using ComStackLib.
    \COUNTERMEASURE \N  The index for the partition specific data is either fixed (for single partition use case) or provided by searching the current application id
                        in the partitionIdentifiers and using its corresponding partitionIdx.
                        Therefore the index is guaranteed to be in the acceptable range.
SBSW_JUSTIFICATION_END */

/* COUNTERMEASURES_BEGIN

  \CM CM_CSM_STACK_VARIABLE The address of the stack variable is always valid for synchronous call context.

COUNTERMEASURES_END */

/* COV_JUSTIFICATION_BEGIN
  \ID COV_CSM_USER_DEFINES
    \ACCEPT TX
    \ACCEPT XF
    \REASON Ifndef is present for redefinition prevention. This allows the user or RTE to manually override this value
            while also ensuring that the constant is present if the user or RTE does not override it.

  \ID COV_CSM_LOCAL_DEFINE
    \ACCEPT TX
    \REASON CSM_LOCAL is not defined externally.

    \ID COV_CSM_ASR_COMPATIBILITY_DEFINE
    \ACCEPT XF
    \ACCEPT TX
    \REASON The behavior was changed in AUTOSAR. To support older AUTOSAR versions, the the defines can be overwritten.
            Standard is the behavior of latest AUTOSAR version.

    \ID COV_CSM_ASR_COMPATIBILITY_FUNCTIONS
    \ACCEPT XF
    \ACCEPT TX
    \REASON The Csm header provides function declarations based on the configured AUTOSAR version for the including
            module. When Csm implementation itself includes the header, always the newest supported version is set.

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  FETA JUSTIFICATIONS
 *********************************************************************************************************************/

/* FETA_JUSTIFICATION_BEGIN

  \ID FETA_CSM_STRICTLY_INCREASING_BY_ONE
  \DESCRIPTION Loop is strictly monotonically increasing. Loop variable is only increased by one in each iteration.
               Types of loop variable and upper bound are of same same size and overflow is excluded by not
               including the highest possible upper bound value in the comparison (e.g. usage of < 255 and not <= 255).
  \COUNTERMEASURE \N No countermeasure necessary, loop is terminating.

  \ID FETA_CSM_STRICTLY_DECREASING_BY_ONE
  \DESCRIPTION Loop is strictly monotonically decreasing. Loop variable is only decreased by one in each iteration.
               Types of loop variable and lower bound are of same same size and overflow is excluded by not
               including the lowest possible lower bound value in the comparison (e.g. usage of > 0 and not >= 0).
  \COUNTERMEASURE \N No countermeasure necessary, loop is terminating.

  \ID FETA_CSM_STRICTLY_INCREASING_BY_ONE_WITH_PRE_CHECK
  \DESCRIPTION Loop is strictly monotonically increasing. Loop variable is only increased by one in each iteration.
               Types of loop variable and upper bound are of same same size but overflow is possible due to including
               the highest possible value in the comparison (e.g. <= 255).
  \COUNTERMEASURE \R A runtime check before the loop limits the upper boundary.

FETA_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: Csm.c
 *********************************************************************************************************************/
