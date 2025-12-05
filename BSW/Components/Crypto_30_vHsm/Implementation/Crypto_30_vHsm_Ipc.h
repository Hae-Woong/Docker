/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Crypto_30_vHsm_Ipc.h
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

#if !defined (CRYPTO_30_VHSM_IPC_H)
# define CRYPTO_30_VHSM_IPC_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Crypto_30_vHsm_Cfg.h"
# include "Crypto_30_vHsm_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* API services IDs */
# define CRYPTO_30_VHSM_IPC_SID_TRANSMITJOBREQUEST                    (0x80u)
# define CRYPTO_30_VHSM_IPC_SID_RECEIVEJOBRESPONSES                   (0x81u)

/* Development error types */
# define CRYPTO_30_VHSM_IPC_E_NO_ERROR                                (0xFFu)
# define CRYPTO_30_VHSM_IPC_E_PARAM_POINTER                           (0x81u)
# define CRYPTO_30_VHSM_IPC_E_INVALID_CHANNEL                         (0x82u)
# define CRYPTO_30_VHSM_IPC_E_CANNOT_ALLOCATE_JOBREQUEST_BUFFER       (0x83u)
# define CRYPTO_30_VHSM_IPC_E_CANNOT_COPY_JOBREQUEST_DATA             (0x84u)

# define CRYPTO_30_VHSM_MANAGEMENT_JOBREQUEST_ID                      Crypto_30_vHsm_GetManagementDriverObjectOfvHsm()
# define CRYPTO_30_VHSM_ACCEPT_ANY_JOBID                              (0xFFFFFFFFu)

# define CRYPTO_30_VHSM_JOBREQUEST_ID_BITPOSITION                     (0u)
# define CRYPTO_30_VHSM_JOBREQUEST_OBJECTID_BITPOSITION               (8u)
# define CRYPTO_30_VHSM_JOBREQUEST_REDIRECTION_BITPOSITION            (16u)
# define CRYPTO_30_VHSM_JOBREQUEST_COUNTER_BITPOSITION                (24u)

# define CRYPTO_30_VHSM_JOBRESPONSE_ID_BITPOSITION                    (0u)
# define CRYPTO_30_VHSM_JOBRESPONSE_STATE_BITPOSITION                 (8u)
# define CRYPTO_30_VHSM_JOBRESPONSE_RESULT_BITPOSITION                (16u)
# define CRYPTO_30_VHSM_JOBRESPONSE_COUNTER_BITPOSITION               (24u)

# define CRYPTO_30_VHSM_JOBRESPONSE_ID_MASK                           (0x000000FFu)
# define CRYPTO_30_VHSM_JOBRESPONSE_COUNTER_MASK                      (0xFF000000u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/* IPC */
/* PRQA S 3453 IpcMacros3543 */ /* MD_CRYPTO_30_VHSM_3453_IPC_MACRO */
# define Crypto_30_vHsm_Ipc_IsHsmReady(partitionIdx)                  (Crypto_30_vHsm_Ipc_GetReadyFlag(partitionIdx) == 0xdeadbeefu)
# define Crypto_30_vHsm_Ipc_EndRamExec(partitionIdx)                  Crypto_30_vHsm_Ipc_SetEndRamExecFlag(partitionIdx, TRUE)
# define Crypto_30_vHsm_Ipc_IsRamExec(partitionIdx)                   Crypto_30_vHsm_Ipc_GetRamExecFlag(partitionIdx)
# define Crypto_30_vHsm_Ipc_IsDetReported(partitionIdx)               Crypto_30_vHsm_Ipc_GetDetReportFlag(partitionIdx)

# if (CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES<2)
#  define Crypto_30_vHsm_GetPartitionIdxOptimized(partitionIdx)       0u
# else
#  define Crypto_30_vHsm_GetPartitionIdxOptimized(partitionIdx)       (partitionIdx)
# endif

# define Crypto_30_vHsm_Ipc_GetMemoryPtr8(partitionIdx, offset)       (&(((P2VAR(uint8, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR))Crypto_30_vHsm_Ipc_MemoryPtrOfPartition[Crypto_30_vHsm_GetPartitionIdxOptimized(partitionIdx)])[offset]))
# define Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, offset)    (*(volatile uint32*)&(((P2VAR(uint8, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR))Crypto_30_vHsm_Ipc_MemoryPtrOfPartition[Crypto_30_vHsm_GetPartitionIdxOptimized(partitionIdx)])[offset]))

# define Crypto_30_vHsm_Ipc_GetTxChannelBuffer(partitionIdx, txChannelId) (Crypto_30_vHsm_Ipc_GetMemoryPtr8(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.txChannelMemoryLayout[txChannelId].bufferOffset))

# define Crypto_30_vHsm_Ipc_GetTxChannelWriteIdx(partitionIdx, txChannelId) (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.txChannelMemoryLayout[txChannelId].writeIdxOffset))

# define Crypto_30_vHsm_Ipc_GetTxChannelReadIdx(partitionIdx, txChannelId) (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.txChannelMemoryLayout[txChannelId].readIdxOffset))

# define Crypto_30_vHsm_Ipc_GetRxChannelBuffer(partitionIdx, rxChannelId) (Crypto_30_vHsm_Ipc_GetMemoryPtr8(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.rxChannelMemoryLayout[rxChannelId].bufferOffset))

# define Crypto_30_vHsm_Ipc_GetRxChannelWriteIdx(partitionIdx, rxChannelId) (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.rxChannelMemoryLayout[rxChannelId].writeIdxOffset))

# define Crypto_30_vHsm_Ipc_GetRxChannelReadIdx(partitionIdx, rxChannelId) (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.rxChannelMemoryLayout[rxChannelId].readIdxOffset))

# define Crypto_30_vHsm_Ipc_GetRamExecFlag(partitionIdx)              ((boolean)Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.ramExecFlagOffset))
# define Crypto_30_vHsm_Ipc_SetEndRamExecFlag(partitionIdx, value)    (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.endRamExecFlagOffset) = (value))

# define Crypto_30_vHsm_Ipc_GetReadyFlag(partitionIdx)                (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.readyFlagOffset))

# define Crypto_30_vHsm_Ipc_GetDetReportFlag(partitionIdx)            ((boolean)Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.detReportFlagOffset))
# define Crypto_30_vHsm_Ipc_SetDetReportFlag(partitionIdx, value)     (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.detReportFlagOffset) = (value))
# define Crypto_30_vHsm_Ipc_GetDetModuleId(partitionIdx)              ((uint16)Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.detModuleIdOffset))
# define Crypto_30_vHsm_Ipc_GetDetInstanceId(partitionIdx)            ((uint8)Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.detInstanceIdOffset))
# define Crypto_30_vHsm_Ipc_GetDetApiId(partitionIdx)                 ((uint8)Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.detApiIdOffset))
# define Crypto_30_vHsm_Ipc_GetDetErrorId(partitionIdx)               ((uint8)Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.detErrorIdOffset))

# define Crypto_30_vHsm_Ipc_vHsmProtocolVersion(partitionIdx)         (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.protocolInfoOffset))
# define Crypto_30_vHsm_Ipc_DriverProtocolVersion(partitionIdx)       (Crypto_30_vHsm_Ipc_GetMemoryValue32(partitionIdx, Crypto_30_vHsm_Ipc_MemoryLayout.protocolInfoOffset+4u))

/* PRQA L:IpcMacros3543 */

# define CRYPTO_30_VHSM_IPC_CHANNELSTATE_FREE                         (0x00u)
# define CRYPTO_30_VHSM_IPC_CHANNELSTATE_ASYNC                        (CRYPTO_PROCESSING_ASYNC + 1u)
# define CRYPTO_30_VHSM_IPC_CHANNELSTATE_SYNC                         (CRYPTO_PROCESSING_SYNC + 1u)

/*! Serialize a uint32 to the IPC buffer with the offset and increment the offset by 4 afterwards */
# define Crypto_30_vHsm_Serialize_Uint32(value, bufferPtr)            (*(P2VAR(uint32, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR))(bufferPtr)) = (uint32)(value) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Serialize a pointer to the IPC buffer with the offset and increment the offset by 4 afterwards */
# define Crypto_30_vHsm_Serialize_Ptr32(value, bufferPtr)             (*(P2VAR(uint32, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR))(bufferPtr)) = (uint32)(value) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Deserialize a uint32 to the IPC buffer with the offset and increment the offset by 4 afterwards */
# define Crypto_30_vHsm_Deserialize_Uint32(bufferPtr)                 (*(P2CONST(uint32, AUTOMATIC, CRYPTO_30_VHSM_APPL_DATA))(bufferPtr)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct Crypto_30_vHsm_Ipc_Channel_MemoryLayoutTypeStruct
{
  uint32 writeIdxOffset;    /*!< Shared memory offset to buffer reserved for write index (value equals index of next free entry) of channel. */
  uint32 readIdxOffset;     /*!< Shared memory offset to buffer reserved for read index (value equals index of next pending entry) of channel. */
  uint32 bufferOffset;      /*!< Shared memory offset to buffer reserved for all channel entries. */
} Crypto_30_vHsm_Ipc_Channel_MemoryLayoutType;

typedef struct Crypto_30_vHsm_Ipc_MemoryLayoutTypeStruct
{
  uint32 protocolInfoOffset;              /*!< Shared memory offsets to buffers reserved for vHSM protocol info. */
  Crypto_30_vHsm_Ipc_Channel_MemoryLayoutType txChannelMemoryLayout[CRYPTO_30_VHSM_IPC_CHANNELS];  /*!< Describes the memory layout of all TX channels. */
  Crypto_30_vHsm_Ipc_Channel_MemoryLayoutType rxChannelMemoryLayout[CRYPTO_30_VHSM_IPC_CHANNELS];  /*!< Describes the memory layout of all RX channels. */
  uint32 stackProtectionCanaryPE1Offset;  /*!< Shared memory offsets to buffers reserved for vHSM stack protection canary Appl Core 1. */
  uint32 stackProtectionCanaryPE2Offset;  /*!< Shared memory offsets to buffers reserved for vHSM stack protection canary Appl Core 2. */
  uint32 seedOffset;                      /*!< Shared memory offset to buffer reserved for vHSM random seed. */
  uint32 detModuleIdOffset;               /*!< Shared memory offsets to buffers reserved for vHSM development error detection (DET module). */
  uint32 detInstanceIdOffset;             /*!< Shared memory offsets to buffers reserved for vHSM development error detection (DET instance id). */
  uint32 detApiIdOffset;                  /*!< Shared memory offsets to buffers reserved for vHSM development error detection (DET api id). */
  uint32 detErrorIdOffset;                /*!< Shared memory offsets to buffers reserved for vHSM development error detection (DET error id). */
  uint32 readyFlagOffset;                 /*!< Shared memory offset to buffer reserved for vHSM Ready flag. */
  uint32 endRamExecFlagOffset;            /*!< Shared memory offset to buffer reserved for vHSM End-RAM-Execution flag. */
  uint32 ramExecFlagOffset;               /*!< Shared memory offset to buffer reserved for vHSM Is-RAM-Execution flag. */
  uint32 detReportFlagOffset;             /*!< Shared memory offset to buffer reserved for vHSM Det-Report flag. */

} Crypto_30_vHsm_Ipc_MemoryLayoutType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_VHSM_START_SEC_IPC_MEMORY_VAR_NOINIT_32BIT
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_VHSM_STOP_SEC_IPC_MEMORY_VAR_NOINIT_32BIT
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_VHSM_START_SEC_VAR_NOINIT_32BIT
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the IPC memory */
extern P2VAR(uint32, CRYPTO_30_VHSM_VAR_NOINIT, CRYPTO_30_VHSM_VAR_NOINIT) Crypto_30_vHsm_Ipc_MemoryPtrOfPartition[CRYPTO_30_VHSM_NUMBER_OF_IPC_INSTANCES];

# define CRYPTO_30_VHSM_STOP_SEC_VAR_NOINIT_32BIT
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_VHSM_START_SEC_CONST_UNSPECIFIED
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Layout of the IPC memory */
extern CONST(Crypto_30_vHsm_Ipc_MemoryLayoutType, CRYPTO_30_VHSM_CONST) Crypto_30_vHsm_Ipc_MemoryLayout;

# define CRYPTO_30_VHSM_STOP_SEC_CONST_UNSPECIFIED
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_VHSM_START_SEC_CODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_TransmitRequest()
 *********************************************************************************************************************/
/*!
 * \brief       Transmit job request
 * \details     -
 * \param[in]   txChannelId            Valid TX channel Id
 * \param[in]   jobRequestId           Valid job request Id
 * \return      TRUE if the job request was correctly transmitted, otherwise E_NOT_OK
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_TransmitRequest(
  Crypto_30_vHsm_ChannelIdType txChannelId,
  Crypto_30_vHsm_JobRequestIterType jobRequestId);

# define CRYPTO_30_VHSM_STOP_SEC_CODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_VHSM_START_SEC_RAMCODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_ReceiveResponse()
 *********************************************************************************************************************/
/*!
 * \brief       Receive job responses
 * \details     -
 * \param[in]   partitionIdx           Valid partition index
 * \param[in]   rxChannelId            Valid RX channel Id
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_ReceiveResponse(
  Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx,
  Crypto_30_vHsm_ChannelIdType rxChannelId);

# define CRYPTO_30_VHSM_STOP_SEC_RAMCODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_VHSM_START_SEC_CODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON)
/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_GetDet()
 *********************************************************************************************************************/
/*!
 * \brief       Get DET error
 * \details     -
 * \param[in]    partitionIdx        Valid partitionIdx
 * \param[out]   moduleId            Valid pointer to hold module ID
 * \param[out]   instanceId          Valid pointer to hold instance ID
 * \param[out]   apiId               Valid pointer to hold API ID
 * \param[out]   errorId             Valid pointer to hold error ID
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_GetDet(
  Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx,
  P2VAR(uint16, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) moduleId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) instanceId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) apiId,
  P2VAR(uint8, AUTOMATIC, CRYPTO_30_VHSM_APPL_VAR) errorId);
# endif /* (CRYPTO_30_VHSM_DEV_ERROR_REPORT == STD_ON) */

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_ProcessChannels()
 *********************************************************************************************************************/
/*!
 * \brief       Process IPC channels
 * \details     -
 * \param[in]   partitionIdx           Valid partition index
 * \pre         -
 * \context     ANY
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_ProcessChannels(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_ProcessCallbacks()
 *********************************************************************************************************************/
/*!
 * \brief       Process IPC callbacks
 * \details     -
 * \param[in]   partitionIdx            Valid partitionIdx
 * \pre         -
 * \context     ANY
 * \reentrant   FALSE
 * \synchronous TRUE
 * \trace       CREQ-Crypto-AsyncCallout
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_ProcessCallbacks(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_HandleDet()
 *********************************************************************************************************************/
/*!
 * \brief       Checks if vHSM invokes a DET error and forwards it to Det module
 * \details     Gets parameters from IPC, acknowledges the handling and calls Det_ReportError
 * \param[in]   partitionIdx            Valid partitionIdx
 * \pre         -
 * \context     ANY
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_HandleDet(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_MainFunctionHandler()
 *********************************************************************************************************************/
/*!
 * \brief       Main function handling the IPC
 * \details     -
 * \param[in]   partitionIdx            Valid partitionIdx
 * \pre         -
 * \context     ANY
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_MainFunctionHandler(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_InitMemoryHandler()
 *********************************************************************************************************************/
/*! \brief       The function initializes partition specific variables, which cannot be initialized with the startup code.
 *  \details     Initialize component variables at power up.
 *  \param[in]   partitionIdx                Valid identifier of the partition.
 *  \pre         Module is uninitialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_InitMemoryHandler(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_ResponseIsrHandler()
 *********************************************************************************************************************/
/*!
 * \brief       Response isr handler called by the ISR
 * \details     -
 * \param[in]   partitionIdx            Valid partitionIdx
 * \pre         -
 * \context     ANY
 * \reentrant   FALSE
 * \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_ResponseIsrHandler(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

# define CRYPTO_30_VHSM_STOP_SEC_CODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CRYPTO_30_VHSM_START_SEC_RAMCODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Crypto_30_vHsm_Ipc_WaitForHsmRamForPartition()
 *********************************************************************************************************************/
/*!
 * \brief       Waits in the RAM for the vHsm Ready flag.
 * \details     This is only needed because the vHsm has no control over the application core startup when a debugger is attached.
 *              When a debugger is attached, all cores will startup immediately.
 * \param[in]   partitionIdx          Valid Partition Index
 * \pre         -
 * \context     ANY
 * \reentrant   TRUE
 * \synchronous TRUE
 * \trace       CREQ-Crypto-WaitLoopCallout
 *********************************************************************************************************************/
FUNC(void, CRYPTO_30_VHSM_CODE) Crypto_30_vHsm_Ipc_WaitForHsmRamForPartition(Crypto_30_vHsm_PartitionIdentifiersIterType partitionIdx);

# define CRYPTO_30_VHSM_STOP_SEC_RAMCODE
# include "Crypto_30_vHsm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* !defined (CRYPTO_30_VHSM_IPC_H) */

/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_vHsm_Ipc.h
 *********************************************************************************************************************/
