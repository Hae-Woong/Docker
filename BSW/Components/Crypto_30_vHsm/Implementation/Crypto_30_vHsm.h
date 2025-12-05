/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*      \file  Crypto_30_vHsm.h
 *      \brief  MICROSAR vHsm Crypto Driver (Crypto)
 *
 *      \details  Core of the Vector Hardware Security Module Firmware driver.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2017-06-23  vistof  -             Initial creation for ASR 4.3
 *                        vismsl                Concept Work
 *  01.01.00  2018-01-09  vistof  STORYC-4214   Feature Completion:
 *                                              Fixed handling code flash write access.
 *                                              Added Det forwarding from vHsm
 *                                              Added support for key element deletion
 *                                              Removed not needed DET checks for cryptoKeyId
 *                                              Fixed serialization of secondaryInputLength during MacVerify
 *                                              Fixed Addresses of memory sections to support more derivatives.
 *                                              Added support for cancel
 *                                              Added Address of global structure is now configurable
 *                                              Added asynchronous job processing
 *                                              Fixed global RAM buffer handling. Each object has now its own buffer.
 *                                              Added option to use vHsm without interrupts
 *                        visfpt                Added padding modes (PKCS7, None, Zero) for hardware AES.
 *                        vismsl                Rework of Ipc
 * 01.02.00  2018-05-04   visfpt  STORYC-5257   Release of Component
 *                        vistof  ESCAN00099366 Fixed: Missing compiler abstractions
 * 01.03.00  2018-06-20   visjhi  STORYC-5741   Added Hardware independent memory area configuration
 * 02.00.00  2018-08-21   vistof  STORYC-6297   Requests will immediately return CRYPTO_E_BUSY when vHsm Firmware is in RAM loop.
 *                                STORYC-6296   Added check if provided output buffer is too big to be buffered to the shared RAM.
 *                                STORYC-6295   Ensure alignment of driver object buffers
 *                                STORYC-6294   Add callout during waiting loops
 *                                ESCAN00100450 Blocking of driver object for other jobs when job is canceled.
 *           2018-09-13   visrpp  STORYC-6521   MISRA-C:2012 Compliance
 *                                ESCAN00100842 Fixed: Possibility of an endless loop in polling mode
 * 02.01.00  2018-12-19   vistof  STORYC-7341   Support fast MAC Verify/Generate processing
 *                                STORYC-7143   Support redirection of input/output buffers from/to key elements
 *                                STORYC-7339   Support partial key element copy
 *                                ESCAN00101843 Compiler option -Ospeed does not work in interrupt mode because
 *                                              Crypto_30_vHsm_JobRequest is not volatile
 * 02.01.01  2019-05-09   visjhi  STORYC-8370   Remove VTT switches
 *                                ESCAN00103101 MacVerify: NULL_PTR as verifyPtr not accepted in START/UPDATE mode (streaming)
 * 02.02.00  2019-08-06   vistof  ESCAN00103942 Fixed: Det error CRYPTO_E_PARAM_POINTER during Fast MAC Generation
 *                                HSM-432       Added DET when configured global ram buffer is too small
 *                                HSM-432       Rework buffer handling
 * 02.02.01  2020-01-28   vistof  ESCAN00105494 Compiler error: unresolved external symbol Det_ReportError
 *                                              when DEV_ERROR_REPORT and DEV_ERROR_DETECT have different values
 * 02.03.00  2020-01-30   vistof  HSM-629       Trigger Host to HSM ISR
 *                                HSM-619       Asynchronous Key Handling
 *                                HSM-160       Make DET errors from vHsm distinguishable from application core DET errors
 *                                HSM-699       Initialize IPC and inform vHsm
 * 02.03.01  2020-07-14   vistof  ESCAN00106838 Execution of a job switches from E_OK to continuously returning E_NOT_OK
 *                                ESCAN00106857 Async jobs may return wrong values in polling mode
 * 02.04.00  2020-08-07   vistof  HSM-984       Harmonize Crypto_JobType according to different ASR version
 * 02.05.00  2020-10-02   vistof  HSM-1193      Allow parallel Key Management operations
 *                                HSM-1190      Improve Exclusive area design
 *                                HSM-1225      Discard job inputs/outputs which are not relevant for the service
 * 02.05.01  2020-10-27   vistof  ESCAN00107729 Compiler error: CRYPTO_KEYSETVALID not defined
 *                                              when Csm according to ASR 4.3 instead of ASR 4.4 is used
 *                                ESCAN00107730 Undefined behaviour when calling key management APIs
 * 02.06.00  2021-01-05   vistof  HSM-1296      Support NoBuffering driver objects
 *                                ESCAN00108351 Use interrupts only for async jobs
 * 03.00.00  2021-01-25   vistof  ESCAN00108526 Race condition in IPC processing leads to undefined behaviour
 *                                ESCAN00108682 Cancel for async jobs is processed async and retVal of Callback is E_OK instead of CRYPTO_E_CANCELED
 *                                HSM-1324      Add support for IPCv3, Remove support for IPCv1
 *                                HSM-943       Improve IPCv3 via shifts
 *                                HSM-1318      Improve IPCv3 via exchanging pointers with values
 *                        visrpp  HSM-270       Custom Modes according ASR 20-11
 * 03.00.01  2021-03-15   vistof  ESCAN00108847 Buffer inside GlobalRamBuffer is not allocated correctly during runtime
 *                                HSM-1608      Remove not needed global variables
 * 03.01.00  2021-07-26   visebj  HSM-1595      Introduce access to HSM2HOST and HOST2HSM registers via KeyElement APIs
 *                                HSM-1785      Support save/restore context
 *                                HSM-1833      Indicate initialized IPC via interrupt
 * 03.02.00  2022-07-07   vistof  HSM-2091      Usage of <Mip>_MemMap.h
 *                                ESCAN00112312 Driver object is still locked after vHsm has returned an error during processing of a crypto job
 * 03.03.00  2022-10-24   vistof  HSM-3092      Support for special for Interrupt handling
 * 03.04.00  2023-02-13   viskju  HSM-3377      FETA analysis, Rework loop callout
 * 04.00.00  2023-04-14   vistof  HSM-3734      Support Multi Partition Use-Case
 *                                ESCAN00114445 Save/Restore Context only works for services which have both input and output pointers
 *                                ESCAN00114875 Fix Compiler warning: Not initialized variable is mapped to ZERO_INIT section
 * 04.00.01  2023-06-06   vistof  ESCAN00114796 Compiler warning: unreferenced formal parameter 'partitionIdx'
 *                                ESCAN00114814 IpcInstance has wrong mapping to partitions
 *                                ESCAN00114934 Runtime exception for Vtt Use-Case during triggering and clearing an interrupt
 *                                HSM-4392      Rework InitMemory for Multicore
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_VHSM_H)
# define CRYPTO_30_VHSM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Crypto_30_vHsm_Types.h"
# include "Crypto_30_vHsm_Cfg.h"
# include "Crypto_30_vHsm_KeyManagement.h"
# include "Crypto_30_vHsm_Custom.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define CRYPTO_30_VHSM_VENDOR_ID                                     (30u)
# define CRYPTO_30_VHSM_MODULE_ID                                     (114u)
# define CRYPTO_30_VHSM_INSTANCE_ID                                   (0u)

/* AUTOSAR Software specification version information */
# define CRYPTO_30_VHSM_AR_RELEASE_MAJOR_VERSION                      (0x04u)
# define CRYPTO_30_VHSM_AR_RELEASE_MINOR_VERSION                      (0x03u)
# define CRYPTO_30_VHSM_AR_RELEASE_REVISION_VERSION                   (0x01u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define CRYPTO_30_VHSM_MAJOR_VERSION                                 (4u)
# define CRYPTO_30_VHSM_MINOR_VERSION                                 (0u)
# define CRYPTO_30_VHSM_PATCH_VERSION                                 (1u)

/* ----- API service IDs ----- */
# define CRYPTO_30_VHSM_SID_INIT                                      (0x00u) /*!< Service ID: Crypto_30_vHsm_Init() */
# define CRYPTO_30_VHSM_SID_GET_VERSION_INFO                          (0x01u) /*!< Service ID: Crypto_30_vHsm_GetVersionInfo() */
# define CRYPTO_30_VHSM_SID_PROCESS_JOB                               (0x03u) /*!< Service ID: Crypto_30_vHsm_ProcessJob() */
# define CRYPTO_30_VHSM_SID_KEY_ELEMENT_SET                           (0x04u) /*!< Service ID: Crypto_30_vHsm_KeyElementSet() */
# define CRYPTO_30_VHSM_SID_KEY_VALID_SET                             (0x05u) /*!< Service ID: Crypto_30_vHsm_KeyValidSet() */
# define CRYPTO_30_VHSM_SID_KEY_ELEMENT_GET                           (0x06u) /*!< Service ID: Crypto_30_vHsm_KeyElementGet() */
# define CRYPTO_30_VHSM_SID_KEY_GENERATE                              (0x07u) /*!< Service ID: Crypto_30_vHsm_KeyGenerate() */
# define CRYPTO_30_VHSM_SID_KEY_DERIVE                                (0x08u) /*!< Service ID: Crypto_30_vHsm_KeyDerive() */
# define CRYPTO_30_VHSM_SID_KEY_EXCHANGE_CALC_PUB_VAL                 (0x09u) /*!< Service ID: Crypto_30_vHsm_KeyExchangeCalcPubVal() */
# define CRYPTO_30_VHSM_SID_KEY_EXCHANGE_CALC_SECRET                  (0x0Au) /*!< Service ID: Crypto_30_vHsm_KeyExchangeCalcSecret() */
# define CRYPTO_30_VHSM_SID_CERTIFICATE_PARSE                         (0x0Bu) /*!< Service ID: Crypto_30_vHsm_CertificateParse() */
# define CRYPTO_30_VHSM_SID_MAIN_FUNCTION                             (0x0Cu) /*!< Service ID: Crypto_30_vHsm_MainFunction() */
# define CRYPTO_30_VHSM_SID_RANDOM_SEED                               (0x0Du) /*!< Service ID: Crypto_30_vHsm_RandomSeed() */
# define CRYPTO_30_VHSM_SID_CANCEL_JOB                                (0x0Eu) /*!< Service ID: Crypto_30_vHsm_CancelJob() */
# define CRYPTO_30_VHSM_SID_KEY_ELEMENT_COPY                          (0x0Fu) /*!< Service ID: Crypto_30_vHsm_KeyElementCopy() */
# define CRYPTO_30_VHSM_SID_KEY_COPY                                  (0x10u) /*!< Service ID: Crypto_30_vHsm_KeyCopy() */
# define CRYPTO_30_VHSM_SID_KEY_ELEMENT_IDS_GET                       (0x11u) /*!< Service ID: Crypto_30_vHsm_KeyElementIdsGet() */
# define CRYPTO_30_VHSM_SID_CERTIFICATE_VERIFY                        (0x12u) /*!< Service ID: Crypto_30_vHsm_CertificateVerify() */
# define CRYPTO_30_VHSM_SID_KEY_ELEMENT_COPY_PARTIAL                  (0x13u) /*!< Service ID: Crypto_30_vHsm_KeyElementCopyPartial() */
# define CRYPTO_30_VHSM_SID_COPYJOBREQUESTTXDATA                      (0x20u) /*!< Service ID: Crypto_30_vHsm_CopyJobRequestTxData() */
# define CRYPTO_30_VHSM_SID_COPYJOBRESPONSERXDATA                     (0x21u) /*!< Service ID: Crypto_30_vHsm_GetResponseFromIpc() */
# define CRYPTO_30_VHSM_SID_PROCESS_JOB_REQUEST                       (0x22u) /*!< Service ID: Crypto_30_vHsm_ProcessJobRequest() */

/* ----- Module States ----- */
/* State: Module is uninitialized */
# define CRYPTO_30_VHSM_UNINIT                                        (0x00u)
/* State: Module is initialized */
# define CRYPTO_30_VHSM_INITIALIZED                                   (0x01u)

/* Development Error Types [SWS_Crypto_00040] */
# if !defined (CRYPTO_E_NO_ERROR) /* COV_CRYPTO_30_VHSM_CSM_DEFINES */
#  define CRYPTO_E_NO_ERROR                                           (255u)
# endif
# if !defined (CRYPTO_E_UNINIT) /* COV_CRYPTO_30_VHSM_CSM_DEFINES */
#  define CRYPTO_E_UNINIT                                             (0u)
# endif
# if !defined (CRYPTO_E_INIT_FAILED) /* COV_CRYPTO_30_VHSM_CSM_DEFINES */
#  define CRYPTO_E_INIT_FAILED                                        (1u)
# endif
# if !defined (CRYPTO_E_PARAM_POINTER) /* COV_CRYPTO_30_VHSM_CSM_DEFINES */
#  define CRYPTO_E_PARAM_POINTER                                      (2u)
# endif
# if !defined (CRYPTO_E_PARAM_HANDLE) /* COV_CRYPTO_30_VHSM_CSM_DEFINES */
#  define CRYPTO_E_PARAM_HANDLE                                       (4u)
# endif
# if !defined (CRYPTO_E_PARAM_VALUE) /* COV_CRYPTO_30_VHSM_CSM_DEFINES */
#  define CRYPTO_E_PARAM_VALUE                                        (5u)
# endif
# if !defined (CRYPTO_E_GLOBAL_BUFFER_TOO_SMALL) /* COV_CRYPTO_30_VHSM_CSM_DEFINES */
#  define CRYPTO_E_GLOBAL_BUFFER_TOO_SMALL                            (100u)
# endif

# define CRYPTO_30_VHSM_JOBREQUEST_STATE_IDLE                         (0x00u)
# define CRYPTO_30_VHSM_JOBREQUEST_STATE_PROCESSING                   (0x01u)
# define CRYPTO_30_VHSM_JOBREQUEST_STATE_RESULT_AVAILABLE             (0x02u)

# define CRYPTO_30_VHSM_HSM2HOST_ACTIVE                               (0u)
# define CRYPTO_30_VHSM_HSM2HOST_READY                                (1u)
# define CRYPTO_30_VHSM_HSM2HOST_UPDATERRUNNING                       (15u)

/* Macros for Job access */
# if !defined (CRYPTO_30_VHSM_JOB_STATE_MEMBER) /* COV_CRYPTO_30_VHSM_ASR_COMPATIBILITY */
#  define CRYPTO_30_VHSM_JOB_STATE_MEMBER                             jobState
# endif

# if !defined (CRYPTO_30_VHSM_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER) /* COV_CRYPTO_30_VHSM_ASR_COMPATIBILITY */
#  define CRYPTO_30_VHSM_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER            jobPrimitiveInputOutput
# endif

/* Multi Partition */
# if (CRYPTO_30_VHSM_NUMBER_OF_PARTITIONS > 1u)
#  define Crypto_30_vHsm_GetCurrentPartitionIdx()                       Crypto_30_vHsm_GetPartitionIdx()
#  define Crypto_30_vHsm_GetCurrentPartitionIdxOfObjectInfo(objectId)   Crypto_30_vHsm_GetPartitionIdxOfObjectInfo(objectId)
# else
#  define Crypto_30_vHsm_GetCurrentPartitionIdx()                       0u
#  define Crypto_30_vHsm_GetCurrentPartitionIdxOfObjectInfo(objectId)   0u
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_VHSM_START_SEC_CODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the Crypto Driver
 *  \details     This function initializes the module Crypto_30_vHsm. It initializes all variables and sets the
 *               module state to initialized.
 *  \pre         Interrupts are disabled.
 *               Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Specification of module initialization
 *  \trace       CREQ-131331
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Init(void);

# if (CRYPTO_30_VHSM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_vHsm_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Function returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Valid pointer to where to store the version information.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CRYPTO_30_VHSM_VERSION_INFO_API == STD_ON
 *  \trace       CREQ-131329
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) versioninfo);
# endif /* (CRYPTO_30_VHSM_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_ProcessJob()
 *********************************************************************************************************************/
/*! \brief         Processes the received job
 *  \details       Performs the crypto primitive, that is configured in the job parameter.
 *  \param[in]     objectId                Valid Id of the Crypto Driver Object.
 *  \param[in,out] job                     Valid pointer to the configuration of the job. Contains structures with job and
 *                                         primitive relevant information but also pointer to result buffers.
 *  \return        E_OK                         Request successful.
 *                 E_NOT_OK                     Request failed.
 *                 CRYPTO_E_BUSY                Request failed, Crypto Driver Object is busy.
 *                 CRYPTO_E_KEY_NOT_VALID       Request failed, the key is not valid.
 *                 CRYPTO_E_KEY_SIZE_MISMATCH   Request failed, a key element has the wrong size.
 *                 CRYPTO_E_QUEUE_FULL          Request failed, the queue is full.
 *                 CRYPTO_E_ENTROPY_EXHAUSTION  Request failed, the entropy is exhausted
 *                 CRYPTO_E_SMALL_BUFFER        Request failed, the provided buffer is too small to store the result.
 *                 CRYPTO_E_COUNTER_OVERFLOW    The counter is overflowed.
 *                 CRYPTO_E_JOB_CANCELED        The service request failed because the synchronous Job has been canceled.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE|FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_ProcessJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) job);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_CancelJob()
 *********************************************************************************************************************/
/*! \brief         Cancels the received job
 *  \details       This interface removes the provided job from the queue and cancels the processing of the job if possible.
 *  \param[in]     objectId                Valid identifier of the Crypto Driver Object.
 *  \param[in,out] job                     Valid pointer to the configuration of the job. Contains structures with user and
 *                                         primitive relevant information.
 *  \return        E_OK                    Request successful, job has been removed.
 *                 E_NOT_OK                Request failed, job could not be removed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_CancelJob(
  uint32 objectId,
  P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) job);

# if (CRYPTO_30_VHSM_NUMBER_OF_PARTITIONS > 1u)
/**********************************************************************************************************************
 *  Crypto_30_vHsm_GetPartitionIdx()
 *********************************************************************************************************************/
/*! \brief         Returns the current active partition idx.
 *  \details       -
 *  \return        Current partition idx
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Crypto_30_vHsm_PartitionIdentifiersIterType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_GetPartitionIdx(void);
# endif /* (CRYPTO_30_VHSM_NUMBER_OF_PARTITIONS > 1u) */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_TriggerInterruptOfPartition()
 *********************************************************************************************************************/
/*! \brief         Trigger an interrupt to the HSM
 *  \details       -
 *  \param[in]     partitionIdx                Valid identifier of the partition.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_TriggerInterruptOfPartition(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_ClearInterruptOfPartition()
 *********************************************************************************************************************/
/*! \brief         Clear the response interrupt flag
 *  \details       -
 *  \param[in]     partitionIdx                Valid identifier of the partition.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_ClearInterruptOfPartition(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_IndicateInitializedIpcOfPartition()
 *********************************************************************************************************************/
/*! \brief         Indicate that the IPC has been initialized to the HSM
 *  \details       For multi partition use-case, a spinlock is needed to synchronize the access to the HOST2HSM register when IPC initailization shall be done by the driver.
 *  \param[in]     partitionIdx                Valid identifier of the partition.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_IndicateInitializedIpcOfPartition(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

# define CRYPTO_30_VHSM_STOP_SEC_CODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_VHSM_START_SEC_RAMCODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_ProcessJobRequest()
 *********************************************************************************************************************/
/*! \brief       Process the job request
 *  \details     -
 *  \param[in]   jobRequestId        Valid Id of the job request (in range of Crypto_30_vHsm_GetSizeOfJobRequest())
 *  \param[in]   partitionIdx        Valid identifier of the partition.
 *  \return      E_OK                Request successful.
 *               E_NOT_OK            Request failed.
 *  \pre         jobRequestId < Crypto_30_vHsm_GetSizeOfJobRequest()
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE|FALSE          Sync or Async, depends on the job configuration
 *  \trace       CREQ-Crypto-WaitLoopCallout, CREQ-Crypto-AsyncCallout
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_ProcessJobRequest(
  Crypto_30_vHsm_JobRequestIterType jobRequestId,
  Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

#if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES < 2u)
/**********************************************************************************************************************
 *  Crypto_30_vHsm_WaitForHsmRam()
 *********************************************************************************************************************/
/*!
 * \brief       Waits in the RAM for the vHsm Ready flag.
 * \details     This is only needed because the vHsm has no control over the application core startup when a debugger is attached.
 *              When a debugger is attached, all cores will startup immediately.
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 * \trace       CREQ-Crypto-WaitLoopCallout
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_WaitForHsmRam(void);
#else /* !(CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES < 2u) */
/**********************************************************************************************************************
 *  Crypto_30_vHsm_WaitForHsmRamForApplication()
 *********************************************************************************************************************/
/*!
 * \brief       Waits in the RAM for the vHsm Ready flag.
 * \details     This is only needed because the vHsm has no control over the application core startup when a debugger is attached.
 *              When a debugger is attached, all cores will startup immediately.
 * \param[in]   applicationId          Valid ApplicationId
 * \return      E_OK                   Request successful.
 *              E_NOT_OK               Request failed.
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 * \trace       CREQ-Crypto-WaitLoopCallout
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_WaitForHsmRamForApplication(ApplicationType applicationId);
#endif /* !(CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES < 2u) */

# define CRYPTO_30_VHSM_STOP_SEC_RAMCODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_VHSM_START_SEC_APPL_CODE
# include "Crypto_30_vHsm_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */

# if (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_vHsm_TimeoutDetect_Start_Callout()
 *********************************************************************************************************************/
/*! \brief       Notification just before waiting for a response from the vHsm Firmware.
 *  \details     Implementation has to be done in the application.
 *  \param[in]   calloutId          Identifies the loop.
 *  \param[in]   objectId           Id of the Crypto Driver Object of the current context. This is only relevant if calloutId is
 *                                  CRYPTO_30_VHSM_CALLOUT_JOBREQUEST or CRYPTO_30_VHSM_CALLOUT_ASYNC.
 *  \param[in]   contextClass       Type of the current context. Only relevant if calloutId is CRYPTO_30_VHSM_CALLOUT_JOBREQUEST
 *                                  or CRYPTO_30_VHSM_CALLOUT_ASYNC.
 *  \param[in]   contextId          If contextClass is CRYPTO_30_VHSM_CALLOUT_CRYPTOJOB, this parameter is the crypto job id,
 *                                  if contextClass is CRYPTO_30_VHSM_CALLOUT_KEYMJOB, this parameter is the key id.
 *                                  Only relevant if calloutId is CRYPTO_30_VHSM_CALLOUT_JOBREQUEST.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_APPL_CODE) Crypto_30_vHsm_TimeoutDetect_Start_Callout(Crypto_30_vHsm_CalloutIdType calloutId,
  uint32 objectId, Crypto_30_vHsm_CalloutContextClassType contextClass, uint32 contextId);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_TimeoutDetect_Loop_Callout()
 *********************************************************************************************************************/
/*! \brief       Notification about ongoing waiting for a response from the vHsm Firmware.
 *  \details     Implementation has to be done in the application.
 *  \param[in]   calloutId          Identifies the loop.
 *  \param[in]   objectId           Id of the Crypto Driver Object of the current context. This is only relevant if calloutId is
 *                                  CRYPTO_30_VHSM_CALLOUT_JOBREQUEST or CRYPTO_30_VHSM_CALLOUT_ASYNC.
 *  \param[in]   contextClass       Type of the current context. Only relevant if calloutId is CRYPTO_30_VHSM_CALLOUT_JOBREQUEST
 *                                  or CRYPTO_30_VHSM_CALLOUT_ASYNC.
 *  \param[in]   contextId          If contextClass is CRYPTO_30_VHSM_CALLOUT_CRYPTOJOB, this parameter is the crypto job id,
 *                                  if contextClass is CRYPTO_30_VHSM_CALLOUT_KEYMJOB, this parameter is the key id.
 *                                  Only relevant if calloutId is CRYPTO_30_VHSM_CALLOUT_JOBREQUEST.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_APPL_CODE) Crypto_30_vHsm_TimeoutDetect_Loop_Callout(Crypto_30_vHsm_CalloutIdType calloutId,
  uint32 objectId, Crypto_30_vHsm_CalloutContextClassType contextClass, uint32 contextId);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_TimeoutDetect_End_Callout()
 *********************************************************************************************************************/
/*! \brief       Notification just after waiting for a response from the vHsm Firmware.
 *  \details     Implementation has to be done in the application.
 *  \param[in]   calloutId          Identifies the loop.
 *  \param[in]   objectId           Id of the Crypto Driver Object of the current context. This is only relevant if calloutId is
 *                                  CRYPTO_30_VHSM_CALLOUT_JOBREQUEST or CRYPTO_30_VHSM_CALLOUT_ASYNC.
 *  \param[in]   contextClass       Type of the current context. Only relevant if calloutId is CRYPTO_30_VHSM_CALLOUT_JOBREQUEST
 *                                  or CRYPTO_30_VHSM_CALLOUT_ASYNC.
 *  \param[in]   contextId          If contextClass is CRYPTO_30_VHSM_CALLOUT_CRYPTOJOB, this parameter is the crypto job id,
 *                                  if contextClass is CRYPTO_30_VHSM_CALLOUT_KEYMJOB, this parameter is the key id.
 *                                  Only relevant if calloutId is CRYPTO_30_VHSM_CALLOUT_JOBREQUEST.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_APPL_CODE) Crypto_30_vHsm_TimeoutDetect_End_Callout(Crypto_30_vHsm_CalloutIdType calloutId,
  uint32 objectId, Crypto_30_vHsm_CalloutContextClassType contextClass, uint32 contextId);
# endif /* (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON) */

#  if (CRYPTO_30_VHSM_INTERRUPT_SET_CALLOUT_AVAILABLE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_vHsm_RequestInterruptSet_Callout()
 *********************************************************************************************************************/
/*! \brief         Function which is called to set the request interrupt flag.
 *  \details       Usually the driver provides the capability to handle setting the interrupt flag by itself. However new
 *                 platforms or use-cases may require more specific handling which can be implemented in this callout.
 *  \param[in]     address         Address configured in the Cfg5 for the response interrupt clearing
 *  \param[in]     mask            Mask configured in the Cfg5 for the response interrupt clearing
 *  \param[in]     value           Value configured in the Cfg5 for the response interrupt clearing
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_APPL_CODE)  Crypto_30_vHsm_RequestInterruptSet_Callout(uint32 address, uint32 mask);
#  endif /* (CRYPTO_30_VHSM_INTERRUPT_SET_CALLOUT_AVAILABLE == STD_ON) */

#  if (CRYPTO_30_VHSM_INTERRUPT_CLEAR_CALLOUT_AVAILABLE == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_vHsm_ResponseInterruptClear_Callout()
 *********************************************************************************************************************/
/*! \brief         Function which is called to clear the response interrupt flag.
 *  \details       Usually the driver provides the capability to handle clearing the interrupt flag by itself. However new
 *                 platforms or use-cases may require more specific handling which can be implemented in this callout.
 *  \param[in]     address         Address configured in the Cfg5 for the response interrupt clearing
 *  \param[in]     mask            Mask configured in the Cfg5 for the response interrupt clearing
 *  \param[in]     value           Value configured in the Cfg5 for the response interrupt clearing
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_APPL_CODE) Crypto_30_vHsm_ResponseInterruptClear_Callout(uint32 address, uint32 mask, uint32 value);
#  endif /* (CRYPTO_30_VHSM_INTERRUPT_CLEAR_CALLOUT_AVAILABLE == STD_ON) */

# define CRYPTO_30_VHSM_STOP_SEC_APPL_CODE
# include "Crypto_30_vHsm_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */

#endif /* !defined (CRYPTO_30_VHSM_H) */

/*!
 * \exclusivearea CRYPTO_30_VHSM_EXCLUSIVE_AREA_0
 * Protects global data from inconsistencies
 * \protects Crypto_30_vHsm_JobRequestMap, Crypto_30_vHsm_DriverObjectList, Crypto_30_vHsm_JobRequestList
 * \usedin Crypto_30_vHsm_ProcessJob, Crypto_30_vHsm_CancelJob, Crypto_30_vHsm_CertificateParse,
 * Crypto_30_vHsm_CertificateVerify, Crypto_30_vHsm_KeyCopy, Crypto_30_vHsm_KeyDerive, Crypto_30_vHsm_KeyElementCopy,
 * Crypto_30_vHsm_KeyElementGet, Crypto_30_vHsm_KeyElementIdsGet, Crypto_30_vHsm_KeyElementSet,
 * Crypto_30_vHsm_KeyExchangeCalcPubVal, Crypto_30_vHsm_KeyExchangeCalcSecret, Crypto_30_vHsm_KeyGenerate,
 * Crypto_30_vHsm_KeyValidSet Crypto_30_vHsm_RandomSeed
 * \exclude All functions provided by Crypto.
 * \length SHORT The access to the protected variables is kept short
 * \endexclusivearea
 */

/*!
 * \exclusivearea CRYPTO_30_VHSM_EXCLUSIVE_AREA_1
 * Protects that a forwarded DET from the vHsm to be processed only once. (Flag is located in the IPC memory)
 * \protects The IPC memory referenced by Crypto_30_vHsm_Ipc_MemoryPtr
 * \usedin Crypto_30_vHsm_ProcessJobRequest, Crypto_30_vHsm_MainFunction
 * \exclude Crypto_30_vhsm_GetVersionInfo
 * \length SHORT The access to the protected variables is kept short
 * \endexclusivearea
 */

/*!
 * \exclusivearea CRYPTO_30_VHSM_EXCLUSIVE_AREA_2
 * Protects workspace locking resources when it can not be ensured that uint32 write access is atomic.
 * If atomic uint32 write access can be guaranteed, this critical section can be empty.
 * \protects Crypto_30_vHsm_DriverObjectLock
 * \usedin Crypto_30_vHsm_ProcessCryptoJob
 * \exclude All functions provided by Crypto.
 * \length SHORT The access to the protected variables is kept short and is only a uint32 write operation.
 * \endexclusivearea
 */

/*!
 * \exclusivearea CRYPTO_30_VHSM_EXCLUSIVE_AREA_3
 * Protects workspace locking resources when it can not be ensured that uint32 write access is atomic.
 * If atomic uint32 write access can be guaranteed, this critical section can be empty.
 * If interrupt mode of driver is used and ISRs can't be interrupted by the same interrupt, this critical section does not need to be protected.
 * \protects Crypto_30_vHsm_DriverObjectLock
 * \usedin Crypto_30_vHsm_Ipc_ProcessCallbacks
 * \exclude All functions provided by Crypto.
 * \length SHORT The access to the protected variables is kept short and is only a uint32 write operation.
 * \endexclusivearea
 */

/*!
 * \exclusivearea CRYPTO_30_VHSM_EXCLUSIVE_AREA_4
 * Protects HOST2HSM register if multiple partitions are configured because multiple cores need to write to the register.
 * This needs to be a spinlock for multi core configurations
 * \protects HOST2HSM register
 * \usedin Crypto_30_vHsm_IndicateInitializedIpcOfPartition
 * \exclude All functions provided by Crypto.
 * \length SHORT The access to the protected variables is kept short and is only a uint32 write operation.
 * \endexclusivearea
 */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_vHsm.h
 *********************************************************************************************************************/
