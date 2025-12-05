/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Crypto_30_vHsm_Ipc.c
 *        \brief  MICROSAR vHsm Crypto Driver (Crypto)
 *
 *      \details  Inter-processor communication between Host and Vector Hardware Security Module Firmware.
 *
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CRYPTO_30_VHSM_IPC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Crypto_30_vHsm.h"
#include "Crypto_30_vHsm_Ipc.h"
#include "Crypto_30_vHsm_Jobs.h"
#include "CryIf_Cbk.h"
#include "SchM_Crypto_30_vHsm.h"
#include "vstdlib.h"

#if (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#if !defined (CRYPTO_30_VHSM_LOCAL) /* COV_CRYPTO_30_VHSM_LOCAL_DEFINE */
# define CRYPTO_30_VHSM_LOCAL                                         static
#endif

#if !defined (CRYPTO_30_VHSM_LOCAL_INLINE) /* COV_CRYPTO_30_VHSM_LOCAL_DEFINE */
# define CRYPTO_30_VHSM_LOCAL_INLINE                                  LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#define CRYPTO_30_VHSM_START_SEC_VAR_NOINIT_32BIT
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the IPC memory */
P2VAR(uint32, CRYPTO_30_VHSM_VAR_NOINIT, CRYPTO_30_VHSM_VAR_NOINIT) Crypto_30_vHsm_Ipc_MemoryPtrOfPartition[CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES];

#define CRYPTO_30_VHSM_STOP_SEC_VAR_NOINIT_32BIT
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CRYPTO_30_VHSM_START_SEC_CODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_SendRequestToIpc()
 *********************************************************************************************************************/
/*!
 * \brief       Serialize the jobrequest
 * \details     -
 * \param[in]   jobRequestId         Valid Id of the jobrequest
 * \param[in]   jobRequestBuffer     Valid buffer pointer where the job shall be serialized.
 *                                   Must at least be of size CRYPTO_30_VHSM_IPC_TX_CHANNEL_MEMORY_SIZE.
 * \return      TRUE if the job request was correctly serialized, otherwise E_NOT_OK
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_VHSM_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_SendRequestToIpc(
  Crypto_30_vHsm_JobRequestIterType jobRequestId,
  Crypto_30_vHsm_JobRequestBufferPtrType jobRequestBuffer);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_GetResponseFromIpc()
 *********************************************************************************************************************/
/*!
 * \brief       Deserialize the job response
 * \details     -
 * \param[in]   partitionIdx          Valid partition index
 * \param[in]   rxChannelId           Valid Channel where the response is allocated
 * \param[in]   writeIdx              WriteIdx of the channel which will be used to acknowledge the response in the IPC.
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
CRYPTO_30_VHSM_LOCAL_INLINE FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_GetResponseFromIpc(
  Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx,
  Crypto_30_vHsm_ChannelIdType rxChannelId,
  uint32 writeIdx);

#define CRYPTO_30_VHSM_STOP_SEC_CODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define CRYPTO_30_VHSM_START_SEC_CODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_SendRequestToIpc()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
CRYPTO_30_VHSM_LOCAL_INLINE FUNC(Std_ReturnType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_SendRequestToIpc(
  Crypto_30_vHsm_JobRequestIterType jobRequestId,
  Crypto_30_vHsm_JobRequestBufferPtrType jobRequestBuffer)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  Crypto_30_vHsm_JobKindType jobRequestKind;
#if (CRYPTO_30_VHSM_REDIRECTION == STD_ON)
  Crypto_JobRedirectionInfoType Crypto_JobRedirectionInfoDummy;
  P2VAR(Crypto_JobRedirectionInfoType, AUTOMATIC, CRYPTO_30_VHSM_APPL_DATA) Crypto_JobRedirectionInfoPtr;
  boolean isCryptoJob = FALSE;
#endif
  uint32 jobInformation;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the kind of the job request */
  jobRequestKind = Crypto_30_vHsm_GetKindOfJobRequest(jobRequestId);

  if ((jobRequestKind == CRYPTO_30_VHSM_CRYPTO_JOBKIND)
#if (CRYPTO_30_VHSM_IPC_MAC_JOB_OPTIMIZATION == STD_ON)
    || (jobRequestKind == CRYPTO_30_VHSM_CRYPTO_MACGEN_JOBKIND)
    || (jobRequestKind == CRYPTO_30_VHSM_CRYPTO_MACVER_JOBKIND)
#endif
    )
  {
#if (CRYPTO_30_VHSM_REDIRECTION == STD_ON)
    isCryptoJob = TRUE;
#endif
  }
  /* #21 Add job request header. */
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u)
  Crypto_30_vHsm_SetCounterOfJobRequest(jobRequestId, (Crypto_30_vHsm_CounterOfJobRequestType)(Crypto_30_vHsm_GetCounterOfJobRequest(jobRequestId) + 1u)); /* SBSW_CRYPTO_30_VHSM_CSL02_OBJECTINFO */
  /* still some free flags... redirection flags are only 5 bit currently */
  jobInformation = (((uint32)(Crypto_30_vHsm_GetCounterOfJobRequest(jobRequestId)) << CRYPTO_30_VHSM_JOBREQUEST_COUNTER_BITPOSITION)
    | ((uint32)(Crypto_30_vHsm_GetHsmObjIdOfObjectInfo(jobRequestId)) << CRYPTO_30_VHSM_JOBREQUEST_OBJECTID_BITPOSITION)
    | jobRequestId);
#else
  jobInformation = ((((uint32)(Crypto_30_vHsm_GetHsmObjIdOfObjectInfo(jobRequestId)) << CRYPTO_30_VHSM_JOBREQUEST_OBJECTID_BITPOSITION)) | jobRequestId);
#endif /* !(CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u) */
#if (CRYPTO_30_VHSM_REDIRECTION == STD_ON)
  if ((isCryptoJob == TRUE)
    && (Crypto_30_vHsm_GetCryptoJobOfJobRequest(jobRequestId)->jobRedirectionInfoRef != NULL_PTR)) /* PRQA S 3415 */ /* MD_CRYPTO_30_VHSM_3415_LOGICAL_OPERATOR */
  {
    jobInformation |= (((uint32)Crypto_30_vHsm_GetCryptoJobOfJobRequest(jobRequestId)->jobRedirectionInfoRef->redirectionConfig) << CRYPTO_30_VHSM_JOBREQUEST_REDIRECTION_BITPOSITION);
    Crypto_JobRedirectionInfoPtr = Crypto_30_vHsm_GetCryptoJobOfJobRequest(jobRequestId)->jobRedirectionInfoRef;
  }
  else
  {
    Crypto_JobRedirectionInfoDummy.redirectionConfig = 0u;
    Crypto_JobRedirectionInfoPtr = &Crypto_JobRedirectionInfoDummy;
  }
#endif /* (CRYPTO_30_VHSM_REDIRECTION == STD_ON) */

#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 2u)
  /* Buffer size */
  Crypto_30_vHsm_Serialize_Uint32(CRYPTO_30_VHSM_IPC_TX_CHANNEL_ENTRY_MEMORY_SIZE, jobRequestBuffer); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_IPC_SERIALIZE_UINT32 */
  /* Header */
  Crypto_30_vHsm_Serialize_Uint32(jobInformation, &jobRequestBuffer[4]); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_IPC_SERIALIZE_UINT32 */
  Crypto_30_vHsm_Serialize_Uint32(jobRequestKind, &jobRequestBuffer[8]); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_IPC_SERIALIZE_UINT32 */
  Crypto_30_vHsm_Serialize_Uint32((uint32)Crypto_30_vHsm_IsCancelOfJobRequest(jobRequestId), &jobRequestBuffer[12]); /* PRQA S 0310, 3305, 4304 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_IPC_SERIALIZE_UINT32 */
#else /* !(CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 2u) */
  /* Header */
  Crypto_30_vHsm_Serialize_Uint32(jobInformation, &jobRequestBuffer[0]); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_IPC_SERIALIZE_UINT32 */
  Crypto_30_vHsm_Serialize_Uint32(jobRequestKind, &jobRequestBuffer[4]); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_IPC_SERIALIZE_UINT32 */
#endif

  /* Set init values inside jobRequest */
  Crypto_30_vHsm_SetUsedBufferOfJobRequest(jobRequestId, 0u); /* SBSW_CRYPTO_30_VHSM_CSL02_OBJECTINFO */
#if (CRYPTO_30_VHSM_DEV_ERROR_DETECT == STD_ON)
  Crypto_30_vHsm_SetTrimmedOutputBufferOfJobRequest(jobRequestId, FALSE); /* SBSW_CRYPTO_30_VHSM_CSL02_OBJECTINFO */
#endif

  /* #22 Call job request transmission function. */
  switch (jobRequestKind)
  {
#if (CRYPTO_30_VHSM_IPC_MAC_JOB_OPTIMIZATION == STD_ON)
  case CRYPTO_30_VHSM_CRYPTO_MACVER_JOBKIND:
    retVal = Crypto_30_vHsm_SerializeRequest_CryptoJob_MacVer( /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_SerializeRequest_CryptoJob_MacVer */
      jobRequestId,
      Crypto_30_vHsm_GetCryptoJobOfJobRequest(jobRequestId),
      &jobRequestBuffer[16]
# if (CRYPTO_30_VHSM_REDIRECTION == STD_ON)
      , Crypto_JobRedirectionInfoPtr
# endif
    );
    break;
  case CRYPTO_30_VHSM_CRYPTO_MACGEN_JOBKIND:
    retVal = Crypto_30_vHsm_SerializeRequest_CryptoJob_MacGen( /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_SerializeRequest_CryptoJob_MacGen */
      jobRequestId,
      Crypto_30_vHsm_GetCryptoJobOfJobRequest(jobRequestId),
      &jobRequestBuffer[16]
# if (CRYPTO_30_VHSM_REDIRECTION == STD_ON)
      , Crypto_JobRedirectionInfoPtr
# endif
    );
    break;
#endif /* (CRYPTO_30_VHSM_IPC_MAC_JOB_OPTIMIZATION == STD_ON) */
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u)
# if (CRYPTO_30_VHSM_KEY_DERIVE_JOB_SUPPORT == STD_ON)
  case CRYPTO_30_VHSM_KEYMANAGEMENT_KEYDERIVEJOB_JOBKIND:
# endif
#endif
  case CRYPTO_30_VHSM_CRYPTO_JOBKIND:
    retVal = Crypto_30_vHsm_SerializeRequest_CryptoJob( /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_SerializeRequest_CryptoJob */
      jobRequestId,
      Crypto_30_vHsm_GetCryptoJobOfJobRequest(jobRequestId),
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u)
      &jobRequestBuffer[8]
#else
      &jobRequestBuffer[16]
#endif
#if (CRYPTO_30_VHSM_REDIRECTION == STD_ON)
      , Crypto_JobRedirectionInfoPtr
#endif
    );
    break;
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u)
  case CRYPTO_30_VHSM_KEYMANAGEMENT_COPY_PARTIAL_JOBKIND:
#endif
  case CRYPTO_30_VHSM_KEYMANAGEMENT_JOBKIND:
    retVal = Crypto_30_vHsm_SerializeRequest_KeyMJob( /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_SerializeRequest_KeyMJob */
      jobRequestId,
      Crypto_30_vHsm_GetKeyMJobOfJobRequest(jobRequestId),
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u)
      &jobRequestBuffer[8]
#else
      &jobRequestBuffer[16]
#endif
      );
    break;
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 2u)
  case CRYPTO_30_VHSM_KEYMANAGEMENT_COPY_PARTIAL_JOBKIND:
    retVal = Crypto_30_vHsm_SerializeRequest_KeyMJob_CopyPartial( /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_SerializeRequest_KeyMJob_CopyPartial */
      jobRequestId,
      Crypto_30_vHsm_GetKeyMJobOfJobRequest(jobRequestId),
      &jobRequestBuffer[16]);
    break;
# if (CRYPTO_30_VHSM_KEY_DERIVE_JOB_SUPPORT == STD_ON)
  case CRYPTO_30_VHSM_KEYMANAGEMENT_KEYDERIVEJOB_JOBKIND:
    retVal = Crypto_30_vHsm_SerializeRequest_CryptoJob_KeyDerive( /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_SerializeRequest_KeyMJob_KeyDerive */
      jobRequestId,
      Crypto_30_vHsm_GetCryptoJobOfJobRequest(jobRequestId),
      &jobRequestBuffer[16]);
    break;
# endif /* (CRYPTO_30_VHSM_KEY_DERIVE_JOB_SUPPORT == STD_ON) */
#endif /* (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 2u) */
  default: /* COV_CRYPTO_30_VHSM_MISRA_ADDITIONAL_STATEMENT */ /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    break;
  }

  return retVal;
} /* Crypto_30_vHsm_SendRequestToIpc() */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_GetResponseFromIpc()
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
CRYPTO_30_VHSM_LOCAL_INLINE FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_GetResponseFromIpc(
  Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx,
  Crypto_30_vHsm_ChannelIdType rxChannelId,
  uint32 writeIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CRYPTO_E_PARAM_VALUE;

  Crypto_30_vHsm_JobRequestIterType jobRequestId;
  uint32 jobResponseData;
  Crypto_30_vHsm_JobKindType jobResponseKind;
  Crypto_30_vHsm_JobResponseBufferPtrType jobResponseBuffer = (Crypto_30_vHsm_JobResponseBufferPtrType)Crypto_30_vHsm_Ipc_GetRxChannelBuffer(partitionIdx, rxChannelId); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */

    /* #20 Parse and remove job response header. */
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 2u)
  jobResponseData = Crypto_30_vHsm_Deserialize_Uint32(&jobResponseBuffer[4]);  /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_STACK_VARIABLE_AS_PTR_WITH_BUFFER */
  jobResponseKind = (Crypto_30_vHsm_JobKindType)Crypto_30_vHsm_Deserialize_Uint32(&jobResponseBuffer[8]); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_STACK_VARIABLE_AS_PTR_WITH_BUFFER */
#else
  jobResponseData = Crypto_30_vHsm_Deserialize_Uint32(&jobResponseBuffer[0]);  /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_SERIALIZATION_MACRO */ /* SBSW_CRYPTO_30_VHSM_STACK_VARIABLE_AS_PTR_WITH_BUFFER */
#endif
  jobRequestId = jobResponseData & CRYPTO_30_VHSM_JOBRESPONSE_ID_MASK;

  /* #30 Check if jobRequestKind is valid. */
  if ((jobRequestId >= Crypto_30_vHsm_GetSizeOfJobRequest()))
  {
    /* Remove the invalid response from the IPC */
    Crypto_30_vHsm_Ipc_GetRxChannelReadIdx(partitionIdx, rxChannelId) = writeIdx; /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_SET_VALUE_IN_IPC */
  }
#if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES > 1u)
  /* Check if the received job belongs to the correct partition */
  else if (partitionIdx != Crypto_30_vHsm_GetCurrentPartitionIdxOfObjectInfo(jobRequestId))
  {
    /* Remove the invalid response from the IPC */
    Crypto_30_vHsm_Ipc_GetRxChannelReadIdx(partitionIdx, rxChannelId) = writeIdx; /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_SET_VALUE_IN_IPC */
  }
#endif /* (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES > 1u) */
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u)
  /* Check if this is the response we expect */
  else if (((Crypto_30_vHsm_CounterOfJobRequestType)((jobResponseData & CRYPTO_30_VHSM_JOBRESPONSE_COUNTER_MASK) >> CRYPTO_30_VHSM_JOBRESPONSE_COUNTER_BITPOSITION) != Crypto_30_vHsm_GetCounterOfJobRequest(jobRequestId)))
  {
    /* Remove the invalid response from the IPC */
    Crypto_30_vHsm_Ipc_GetRxChannelReadIdx(partitionIdx, rxChannelId) = writeIdx; /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_SET_VALUE_IN_IPC */
  }
#else /* !(CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u) */
  /* Check if the jobRequestKind inside the request is the same as in the jobRequestList */
  else if (Crypto_30_vHsm_GetKindOfJobRequest(jobRequestId) != jobResponseKind)
  {
    Crypto_30_vHsm_Ipc_GetRxChannelReadIdx(partitionIdx, rxChannelId) = writeIdx; /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_SET_VALUE_IN_IPC */
  }
#endif
  /* check if jobrequest still waits for an answer */
  else if (Crypto_30_vHsm_GetStateOfJobRequest(jobRequestId) != CRYPTO_30_VHSM_JOBREQUEST_STATE_PROCESSING)
  {
    /* Remove the invalid response from the IPC */
    Crypto_30_vHsm_Ipc_GetRxChannelReadIdx(partitionIdx, rxChannelId) = writeIdx; /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_SET_VALUE_IN_IPC */
  }
  else
  {
    jobResponseKind = Crypto_30_vHsm_GetKindOfJobRequest(jobRequestId);
    /* #50 Call job response receive function for processing. */
    if ((jobResponseKind == CRYPTO_30_VHSM_KEYMANAGEMENT_JOBKIND)
      || (jobResponseKind == CRYPTO_30_VHSM_KEYMANAGEMENT_COPY_PARTIAL_JOBKIND))
    {
      Crypto_30_vHsm_ReceiveResponse_KeyMJob( /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_ReceiveResponse_KeyMJob */
        jobRequestId,
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u)
        jobResponseData,
        &jobResponseBuffer[4]);
#else
        &jobResponseBuffer[12]);
#endif
    }
    else
    {
      Crypto_30_vHsm_ReceiveResponse_CryptoJob( /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_ReceiveResponse_CryptoJob */
        jobRequestId,
#if (CRYPTO_30_VHSM_IPC_PROTOCOL_VERSION == 3u)
        jobResponseData,
        &jobResponseBuffer[4]);
#else
        &jobResponseBuffer[12]);
#endif
    }

    /* #61 Commit the reception of the job response to the IPC */
    Crypto_30_vHsm_Ipc_GetRxChannelReadIdx(partitionIdx, rxChannelId) = writeIdx; /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_SET_VALUE_IN_IPC */

    /* #62 Indicate that the channel can now be used by other jobs */
    Crypto_30_vHsm_SetChannelState(rxChannelId, CRYPTO_30_VHSM_IPC_CHANNELSTATE_FREE, partitionIdx); /* SBSW_CRYPTO_30_VHSM_CSL01_CHANNELSTATE */

    /* #63 State processing of job request is done and job results are available. */
    Crypto_30_vHsm_SetStateOfJobRequest(jobRequestId, CRYPTO_30_VHSM_JOBREQUEST_STATE_RESULT_AVAILABLE); /* SBSW_CRYPTO_30_VHSM_CSL02_OBJECTINFO */

#if (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON)
    errorId = CRYPTO_E_NO_ERROR;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_E_NO_ERROR)
  {
    /* #100 Call Det if error occurred */
    (void)Det_ReportError(CRYPTO_30_VHSM_MODULE_ID, CRYPTO_30_VHSM_INSTANCE_ID, CRYPTO_30_VHSM_SID_COPYJOBRESPONSERXDATA, errorId);
  }
#else
  CRYPTO_30_VHSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif /* !(CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON) */

#if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES < 2u)
  CRYPTO_30_VHSM_DUMMY_STATEMENT(partitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

} /* Crypto_30_vHsm_GetResponseFromIpc() */

#define CRYPTO_30_VHSM_STOP_SEC_CODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CRYPTO_30_VHSM_START_SEC_CODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_TransmitRequest()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_TransmitRequest(
  Crypto_30_vHsm_ChannelIdType txChannelId,
  Crypto_30_vHsm_JobRequestIterType jobRequestId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  uint8 errorId = CRYPTO_30_VHSM_IPC_E_NO_ERROR;
  uint32 readIdx;
  uint32 writeIdx;
  Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx = Crypto_30_vHsm_GetCurrentPartitionIdxOfObjectInfo(jobRequestId);

  /* Use local copy of read index to prevent concurrency issues. */
  readIdx = Crypto_30_vHsm_Ipc_GetTxChannelReadIdx(partitionIdx, txChannelId); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */
  writeIdx = Crypto_30_vHsm_Ipc_GetTxChannelWriteIdx(partitionIdx, txChannelId); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Return BUSY when channel is full. */
  if ((writeIdx - readIdx) > 0u)
  {
    /* Error: Tx Channel is full. */
    retVal = CRYPTO_E_BUSY;
#if (CRYPTO_30_VHSM_DEV_ERROR_DETECT == STD_ON)
    /* Error: Cannot allocate enough space for job request in Tx channel. */
    errorId = CRYPTO_30_VHSM_IPC_E_CANNOT_ALLOCATE_JOBREQUEST_BUFFER;
#endif
  }
  else  /* (writeIdx - readIdx) < CRYPTO_30_VHSM_IPC_CHANNEL_SIZE */
  {
    /* #30 Copy job request data from driver into Tx channel for transmission to vHsm. */
    retVal = Crypto_30_vHsm_SendRequestToIpc(jobRequestId, (Crypto_30_vHsm_JobRequestBufferPtrType)Crypto_30_vHsm_Ipc_GetTxChannelBuffer(partitionIdx, txChannelId)); /* PRQA S 0310 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_ForwardingIpcBufferAsPtr_SendRequestToIpc */
#if (CRYPTO_30_VHSM_DEV_ERROR_DETECT == STD_ON)
    if (E_OK != retVal)
    {
      /* Error: Copying job request data for transmission failed. */
      errorId = CRYPTO_30_VHSM_IPC_E_CANNOT_COPY_JOBREQUEST_DATA;
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CRYPTO_30_VHSM_IPC_E_NO_ERROR)
  {
    /* #100 Report Det if error occurred */
    (void)Det_ReportError(CRYPTO_30_VHSM_MODULE_ID,
      CRYPTO_30_VHSM_INSTANCE_ID, CRYPTO_30_VHSM_IPC_SID_TRANSMITJOBREQUEST, errorId);
  }
#else /* !(CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON) */
  CRYPTO_30_VHSM_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

#if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES < 2u)
  CRYPTO_30_VHSM_DUMMY_STATEMENT(partitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
} /* Crypto_30_vHsm_Ipc_TransmitRequest() */

#define CRYPTO_30_VHSM_STOP_SEC_CODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define CRYPTO_30_VHSM_START_SEC_RAMCODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_ReceiveResponse()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_ReceiveResponse(
  Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx,
  Crypto_30_vHsm_ChannelIdType rxChannelId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 readIdx;
  uint32 writeIdx;

  /* ----- Implementation ----------------------------------------------- */
  readIdx = Crypto_30_vHsm_Ipc_GetRxChannelReadIdx(partitionIdx, rxChannelId); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */
  /* Use local copy of write index to prevent concurrency issues. */
  writeIdx = Crypto_30_vHsm_Ipc_GetRxChannelWriteIdx(partitionIdx, rxChannelId); /* PRQA S 0310, 3305 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */

  /* #10 Receive job responses from Rx channel if present. */
  if (readIdx != writeIdx)
  {
    /* #40 Copy job response data to driver for processing. */
    /* When a job response was invalid it is marked as read and will be discared! */
    Crypto_30_vHsm_GetResponseFromIpc(partitionIdx, rxChannelId, writeIdx);
  }
} /* Crypto_30_vHsm_Ipc_ReceiveResponse() */

#define CRYPTO_30_VHSM_STOP_SEC_RAMCODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define CRYPTO_30_VHSM_START_SEC_CODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_GetDet()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_GetDet(
  Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx,
  P2VAR(uint16, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) moduleId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) instanceId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) apiId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) errorId)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the values of the Det stored inside the Ipc */
  /* PRQA S 0310, 3305 4 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */
  *moduleId = Crypto_30_vHsm_Ipc_GetDetModuleId(partitionIdx); /* SBSW_CRYPTO_30_VHSM_VARIABLE_ACCESS_PTR_FROM_STACK */
  *instanceId = Crypto_30_vHsm_Ipc_GetDetInstanceId(partitionIdx); /* SBSW_CRYPTO_30_VHSM_VARIABLE_ACCESS_PTR_FROM_STACK */
  *apiId = Crypto_30_vHsm_Ipc_GetDetApiId(partitionIdx); /* SBSW_CRYPTO_30_VHSM_VARIABLE_ACCESS_PTR_FROM_STACK */
  *errorId = Crypto_30_vHsm_Ipc_GetDetErrorId(partitionIdx); /* SBSW_CRYPTO_30_VHSM_VARIABLE_ACCESS_PTR_FROM_STACK */

# if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES < 2u)
  CRYPTO_30_VHSM_DUMMY_STATEMENT(partitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

} /* Crypto_30_vHsm_Ipc_GetDet() */
#endif /* (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON) */

/*********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_ProcessChannels()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_ProcessChannels(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  Crypto_30_vHsm_ChannelStateIterType rxChannel;

  /* #10 Process the job response available in the IPC */
  for (rxChannel = 0u; rxChannel  < Crypto_30_vHsm_GetSizeOfChannelState(partitionIdx); rxChannel ++)
  {
    if (Crypto_30_vHsm_GetChannelState(rxChannel, partitionIdx) == CRYPTO_30_VHSM_IPC_CHANNELSTATE_ASYNC)
    {
      Crypto_30_vHsm_Ipc_ReceiveResponse(partitionIdx, (Crypto_30_vHsm_ChannelIdType)rxChannel);
    }
  }
} /* Crypto_30_vHsm_Ipc_ProcessChannels() */

/*********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_ProcessCallbacks()
 *********************************************************************************************************************/
/*!
 *
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
 */
 /* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_ProcessCallbacks(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Crypto_30_vHsm_JobRequestIterType jobRequestId;
  Std_ReturnType jobRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over job request slots */
  for (jobRequestId = 0u; jobRequestId < Crypto_30_vHsm_GetSizeOfJobRequest(); jobRequestId++)
  {
#if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES > 1u)
    /* #11 Check if the jobRequestId mapped to the current partition */
    if (Crypto_30_vHsm_GetPartitionIdxOfObjectInfo(jobRequestId) == partitionIdx)
#endif
    {
      /* #21 Check if the job is an async job */
      if (Crypto_30_vHsm_GetProcessingTypeOfJobRequest(jobRequestId) == CRYPTO_PROCESSING_ASYNC)
      {
        /* #22 Check if the job request lock is still set */
        if (Crypto_30_vHsm_IsLockOfJobRequest(jobRequestId))
        {
          /* #23 Check if the job request is finished */
          if (Crypto_30_vHsm_GetStateOfJobRequest(jobRequestId) == CRYPTO_30_VHSM_JOBREQUEST_STATE_RESULT_AVAILABLE)
          {
            /* #30 Get the corresponding job */
            P2VAR(Crypto_JobType, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) tmpJob = Crypto_30_vHsm_GetCryptoJobOfJobRequest(jobRequestId);

            /* #40 Set the state of the job */
            tmpJob->CRYPTO_30_VHSM_JOB_STATE_MEMBER = Crypto_30_vHsm_GetJobStateOfJobRequest(jobRequestId); /* SBSW_CRYPTO_30_VHSM_CSL02_OBJECTINFO_JOB_PTR */

            jobRetVal = (Std_ReturnType)Crypto_30_vHsm_GetResultOfJobRequest(jobRequestId);
            Crypto_30_vHsm_SetStateOfJobRequest(jobRequestId, CRYPTO_30_VHSM_JOBREQUEST_STATE_IDLE); /* SBSW_CRYPTO_30_VHSM_CSL02_OBJECTINFO */

#if (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON)
            Crypto_30_vHsm_TimeoutDetect_End_Callout( /* PRQA S 0404 */ /* MD_CRYPTO_30_VHSM_0404_VOLATILE_READ */
              CRYPTO_30_VHSM_CALLOUT_ASYNC,
              Crypto_30_vHsm_GetHsmObjIdOfObjectInfo(jobRequestId),
              Crypto_30_vHsm_GetContextClassOfJobRequest(jobRequestId),
              Crypto_30_vHsm_GetContextIdOfJobRequest(jobRequestId));
#endif

            /* #50 Free the job request lock */
            Crypto_30_vHsm_SetLockOfJobRequest(jobRequestId, FALSE); /* SBSW_CRYPTO_30_VHSM_CSL02_OBJECTINFO */

            /* #60 Check if the mode was finish */
            if ((jobRetVal != E_OK)
              || ((tmpJob->CRYPTO_30_VHSM_JOB_PRIMITIVE_INPUT_OUTPUT_MEMBER.mode & CRYPTO_OPERATIONMODE_FINISH) == CRYPTO_OPERATIONMODE_FINISH))
            {
              /* #70 Remove the lock from the driver object to accept new jobs */
              SchM_Enter_Crypto_30_vHsm_CRYPTO_30_VHSM_EXCLUSIVE_AREA_3();
              Crypto_30_vHsm_SetDriverObjectLock(jobRequestId, CRYPTO_30_VHSM_ACCEPT_ANY_JOBID); /* SBSW_CRYPTO_30_VHSM_CSL02_OBJECTINFO */
              SchM_Exit_Crypto_30_vHsm_CRYPTO_30_VHSM_EXCLUSIVE_AREA_3();
            }

            /* #80 Call the CryIf callback */
            CryIf_CallbackNotification(
              tmpJob, /* SBSW_CRYPTO_30_VHSM_PTR_ACCESS_WITH_DET_CHECK */
              jobRetVal);
          }
        }
      }
    }
  }

#if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES < 2u)
  CRYPTO_30_VHSM_DUMMY_STATEMENT(partitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

} /* Crypto_30_vHsm_Ipc_ProcessCallbacks() */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_MainFunctionHandler()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_MainFunctionHandler(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx)
{
#if (CRYPTO_30_VHSM_INTERRUPT_MODE == STD_OFF)
  /* #10 Process the job response available in the IPC */
  Crypto_30_vHsm_Ipc_ProcessChannels(partitionIdx);

  /* #20 Process the callbacks of async jobs */
  Crypto_30_vHsm_Ipc_ProcessCallbacks(partitionIdx);
#endif

  /* #30 Check Det if error occurred */
  Crypto_30_vHsm_Ipc_HandleDet(partitionIdx); /* PRQA S 2987 */ /* MD_CRYPTO_30_VHSM_2987_HANDLEDET */
} /* Crypto_30_vHsm_MainFunctionHandler() */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_InitMemoryHandler()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_InitMemoryHandler(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the mode to uninitialized */
#if (CRYPTO_30_VHSM_DEV_ERROR_DETECT == STD_ON)
  Crypto_30_vHsm_SetPartitionInitialized((uint8)CRYPTO_30_VHSM_UNINIT, partitionIdx); /* SBSW_CRYPTO_30_VHSM_WRITE_PARTITION_DATA */
#endif
  CRYPTO_30_VHSM_DUMMY_STATEMENT(partitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
} /* Crypto_30_vHsm_InitMemoryHandler() */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_HandleDet()
 *********************************************************************************************************************/
/*!
 *
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
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_HandleDet(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx) /* PRQA S 3206 */ /* MD_CRYPTO_30_VHSM_3206_SINGLE_PARTITION_IPC_ACCESS */
{
  if (Crypto_30_vHsm_Ipc_GetDetReportFlag(partitionIdx) == TRUE) /* PRQA S 0310, 3305, 4340 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */
  {
#if (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON)
    /* #10 Check if a Det error has been reported */
    SchM_Enter_Crypto_30_vHsm_CRYPTO_30_VHSM_EXCLUSIVE_AREA_1();
    if (Crypto_30_vHsm_Ipc_GetDetReportFlag(partitionIdx) == TRUE) /* PRQA S 0310, 3305, 4340 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* COV_CRYPTO_30_VHSM_DOUBLE_CHECK */
    {
      uint16 moduleId;
      uint8 instanceId, apiId, errorId;

      /* #20 Get the Det parameters */
      Crypto_30_vHsm_Ipc_GetDet(partitionIdx, &moduleId, &instanceId, &apiId, &errorId); /* SBSW_CRYPTO_30_VHSM_STACK_VARIABLE_AS_PTR */

      /* #30 Acknowledge the forwarding of the Det Error */
      Crypto_30_vHsm_Ipc_SetDetReportFlag(partitionIdx, FALSE); /* PRQA S 0310, 3305, 4404 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_SET_VALUE_IN_IPC */
      SchM_Exit_Crypto_30_vHsm_CRYPTO_30_VHSM_EXCLUSIVE_AREA_1();

# if (CRYPTO_30_VHSM_DET_INSTANCE_MASK != 0)
      /* #35 Modify the InstanceId of the Det to distinguish between DET from vHsm and DET from application core */
      instanceId |= CRYPTO_30_VHSM_DET_INSTANCE_MASK;
# endif

      /* #40 Call the Det */
      (void)Det_ReportError(moduleId, instanceId, apiId, errorId);
    }
    else
    {
      SchM_Exit_Crypto_30_vHsm_CRYPTO_30_VHSM_EXCLUSIVE_AREA_1();
    }
#else /* !(CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON) */
    /* #50 Acknowledge the forwarding of the Det Error */
    Crypto_30_vHsm_Ipc_SetDetReportFlag(partitionIdx, FALSE); /* PRQA S 0310, 3305, 4404 */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* SBSW_CRYPTO_30_VHSM_SET_VALUE_IN_IPC */
#endif

#if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES < 2u)
    CRYPTO_30_VHSM_DUMMY_STATEMENT(partitionIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  }
} /* Crypto_30_vHsm_Ipc_HandleDet() */

/*********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_ResponseIsrHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_ResponseIsrHandler(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx) /* PRQA S 3206 */ /* MD_CRYPTO_30_VHSM_3206_SINGLE_PARTITION_IPC_ACCESS */
{
  /* ----- Implementation ----------------------------------------------- */

  /* #10 Clear interrupt flag if not already done by OS */
  Crypto_30_vHsm_ClearInterruptOfPartition(partitionIdx); /* PRQA S 0303 */ /* MD_CRYPTO_30_VHSM_REGISTER_ACCESS */ /* SBSW_CRYPTO_30_VHSM_REGISTER_ACCESS */ /*lint !e522 */

#if (CRYPTO_30_VHSM_INTERRUPT_MODE == STD_ON)
  /* #20 Process the job response available in the IPC */
  Crypto_30_vHsm_Ipc_ProcessChannels(partitionIdx);

  /* #30 Process the callbacks of async jobs */
  Crypto_30_vHsm_Ipc_ProcessCallbacks(partitionIdx);
#endif
} /* Crypto_30_vHsm_Ipc_ResponseIsrHandler() */

#define CRYPTO_30_VHSM_STOP_SEC_CODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define CRYPTO_30_VHSM_START_SEC_RAMCODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_WaitForHsmRamForPartition()
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_WaitForHsmRamForPartition(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx)
{
  /* #10 Get the offset of the ready flag */
  volatile uint32 readyFlagOffsetTmp = Crypto_30_vHsm_Ipc_MemoryLayout.readyFlagOffset;

#if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES > 1u)
  Crypto_30_vHsm_Ipc_MemoryPtrOfPartition[partitionIdx] = &Crypto_30_vHsm_GetIpcMemoryPtr(partitionIdx)[0U]; /* SBSW_CRYPTO_30_VHSM_IPC_MEMORY_PTR_ARRAY */
#else
  Crypto_30_vHsm_Ipc_MemoryPtrOfPartition[partitionIdx] = &Crypto_30_vHsm_Ipc_Memory[0U]; /* SBSW_CRYPTO_30_VHSM_IPC_MEMORY_PTR_ARRAY */
#endif

#if (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON)
  Crypto_30_vHsm_TimeoutDetect_Start_Callout(
    CRYPTO_30_VHSM_CALLOUT_HSMREADY,
    0u,
    CRYPTO_30_VHSM_CALLOUT_NOT_DEFINED,
    0u);
#endif
#if (CRYPTO_30_VHSM_IS_VTT==STD_OFF)
  /* #20 Wait until the ready flag has been set by the vHsm */
  while (Crypto_30_vHsm_CheckForHsmReady()) /* PRQA S 0303, 0310, 3305 */ /* MD_CRYPTO_30_VHSM_REGISTER_ACCESS */ /* MD_CRYPTO_30_VHSM_IPC_MACRO */ /* FETA_CRYPTO_30_VHSM_WAIT_FOR_HSM_WITH_CALLOUT */
  {
# if (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON)
    Crypto_30_vHsm_TimeoutDetect_Loop_Callout(
      CRYPTO_30_VHSM_CALLOUT_HSMREADY,
      0u,
      CRYPTO_30_VHSM_CALLOUT_NOT_DEFINED,
      0u);
# endif
  }
#endif /* (CRYPTO_30_VHSM_IS_VTT==STD_OFF) */

#if (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON)
  Crypto_30_vHsm_TimeoutDetect_End_Callout(
    CRYPTO_30_VHSM_CALLOUT_HSMREADY,
    0u,
    CRYPTO_30_VHSM_CALLOUT_NOT_DEFINED,
    0u);
#endif

#if (CRYPTO_30_VHSM_IPC_INITIALIZATION == STD_ON)
  /* Check if Ipc has already been initialized before */
# if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES > 1u)
  if ((*(volatile uint32*)CRYPTO_30_VHSM_HOST2_HSM_ADDRESS & ((uint32)1u << (uint32)Crypto_30_vHsm_GetIpcInstanceId(partitionIdx))) != ((uint32)1u << (uint32)Crypto_30_vHsm_GetIpcInstanceId(partitionIdx))) /* PRQA S 0303 */ /* MD_CRYPTO_30_VHSM_REGISTER_ACCESS */
# else
  if (Crypto_30_vHsm_IsIpcNotInitialized()) /* PRQA S 0303 */ /* MD_CRYPTO_30_VHSM_REGISTER_ACCESS */
# endif
  {
    /* Initialize IPC */
    VStdLib_MemClr(Crypto_30_vHsm_Ipc_MemoryPtrOfPartition[partitionIdx], (uint32)CRYPTO_30_VHSM_IPC_MEMORY_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CRYPTO_30_VHSM_INIT_IPC_MEMORY */

    /* Indicate that IPC has been initialized. HSM will now be able to use the IPC. */
    Crypto_30_vHsm_IndicateInitializedIpcOfPartition(partitionIdx); /* PRQA S 0303 */ /* MD_CRYPTO_30_VHSM_REGISTER_ACCESS */ /* SBSW_CRYPTO_30_VHSM_REGISTER_ACCESS */
    Crypto_30_vHsm_TriggerInterruptOfPartition(partitionIdx); /* PRQA S 0303 */ /* MD_CRYPTO_30_VHSM_TRIGGER_INTERRUPT */ /* SBSW_CRYPTO_30_VHSM_TRIGGER_INTERRUPT */ /*lint !e522 */
  }
#endif /* (CRYPTO_30_VHSM_IPC_INITIALIZATION == STD_ON) */
#if (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON)
  Crypto_30_vHsm_TimeoutDetect_Start_Callout(
    CRYPTO_30_VHSM_CALLOUT_IPCINIT,
    0u,
    CRYPTO_30_VHSM_CALLOUT_NOT_DEFINED,
    0u);
#endif
  while ((Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, readyFlagOffsetTmp)) != 0xDEADBEEFu) /* PRQA S 0310, 3305, 0404 */ /* MD_CRYPTO_30_VHSM_0404_VOLATILE_READ */ /* FETA_CRYPTO_30_VHSM_WAIT_FOR_HSM_WITH_CALLOUT */
  {
#if (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON)
    Crypto_30_vHsm_TimeoutDetect_Loop_Callout(
      CRYPTO_30_VHSM_CALLOUT_IPCINIT,
      0u,
      CRYPTO_30_VHSM_CALLOUT_NOT_DEFINED,
      0u);
#endif
  }
#if (CRYPTO_30_VHSM_TIMEOUT_CALLOUT == STD_ON)
  Crypto_30_vHsm_TimeoutDetect_End_Callout(
    CRYPTO_30_VHSM_CALLOUT_IPCINIT,
    0u,
    CRYPTO_30_VHSM_CALLOUT_NOT_DEFINED,
    0u);
#endif
} /* Crypto_30_vHsm_Ipc_WaitForHsmRamForPartition() */

#define CRYPTO_30_VHSM_STOP_SEC_RAMCODE
#include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_vHsm_Ipc.c
 *********************************************************************************************************************/
