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
/**        \file  Dcm_Svc86Schd.c
 *         \unit  Svc86Schd
 *        \brief  Contains the implementation of Service 0x86 Scheduler unit.
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
#define DCM_SVC86SCHD_SOURCE

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
#include "Dcm_Svc86Schd.h"
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_MSR_UT_OPTIONAL_UNIT */
# include "Dcm_Tsk.h"
# include "Dcm_Tmr.h"
# include "Dcm_Net.h"
# include "Dcm_NetPbCfg.h"
# include "Dcm_Repeater.h"
# include "Dcm_State.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Uti.h"
# include "Dcm_Diag.h"
# if ((DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON))
#  include "Dcm_DidMgr.h"
# endif
# if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
#  include "Dcm_VarMgr.h"
# endif

 /**********************************************************************************************************************
  *  VERSION CHECK
  *********************************************************************************************************************/

  /**********************************************************************************************************************
   *  LOCAL CONSTANT MACROS
   *********************************************************************************************************************/
/*! Indication that no events has occured yet for subfunction 0x03 or 0x07 */
# define DCM_SVC86_DID_CHANGE_EVENT_NO_WINNER                        DCM_SVC_86_MAX_NUM_CHANGE_OF_DID_EVENTS
# define DCM_SVC86_COMPARISON_VALUE_EVENT_NO_WINNER                  DCM_SVC_86_MAX_NUM_COMPARISON_OF_VALUE_EVENTS

/*! Comparison logic parameters for subfunction 0x07 */
# define DCM_SVC86_07_COMPARISON_LESS_THAN_MEASURED                  (1u)
# define DCM_SVC86_07_COMPARISON_MORE_THAN_MEASURED                  (2u)
# define DCM_SVC86_07_COMPARISON_EQUAL_MEASURED                      (3u)
# define DCM_SVC86_07_COMPARISON_UNEQUAL_MEASURED                    (4u)

/*! Maximum allowed Buffer size for comparing Data for subfunction 0x07 */
# define DCM_SVC86_07_MAX_SUPPORTED_COMPARE_LENGTH                   ((128u) + (DCM_SVC_86_MAX_SUPPORTED_DID_LENGTH))

/*! Mapping of current session context to NvM data. The context of any non-default session will never be stored into NvM */
# define Dcm_Svc86CurrentSession                                     Dcm_Svc86NvMData

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
 /*! Condition to determine if any element needs to be sent in the current scheduler cycle */
#  define Dcm_Svc86_09AnyElementToSend()                             ((Dcm_Svc86Schd_09DtcRingBuffer.NumElements == DCM_SVC_86_MAX_NUM_DTC_STATUS_CHANGED_EVENTS) || \
                                                                     (Dcm_Svc86Schd_09DtcRingBuffer.ReadIndex != Dcm_Svc86Schd_09DtcRingBuffer.ReadSchdIndex)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/*! Condition to determine if any element needs to be sent in the current scheduler cycle */
#  define Dcm_Svc86_09AnyElementToSend()                             ((Dcm_Svc86Schd_09DtcRingBuffer.NumElements == DCM_SVC_86_MAX_NUM_DTC_STATUS_CHANGED_EVENTS) || \
                                                                     (Dcm_Svc86Schd_09DtcRingBuffer.ReadIndex != Dcm_Svc86Schd_09DtcRingBuffer.ReadSchdIndex)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
 /*! Service 0x86 subfunction 0x03 scheduler data */
struct DCM_SVC86ONDIDCHANGESCHEDULERTYPE_TAG
{
  Dcm_DidMgrDidOpTypeContextType DidOpTypeContext;                            /*!< Information about operations on current DID */
  Dcm_MsgItemType                Buffer[DCM_SVC_86_MAX_SUPPORTED_DID_LENGTH]; /*!< Buffer for reading the DID data during scheduling */
  Dcm_MsgLenType                 ReadIndex;                                   /*!< Number of bytes that were already read */
  Dcm_OpStatusType               OpStatus;                                    /*!< Current operation status */
  uint8                          DidIndex;                                    /*!< Scheduler table entry that is currently processed */
  uint8                          DidIndexWinner;                              /*!< Index to scheduler table entry of the first detected event */
};
typedef struct DCM_SVC86ONDIDCHANGESCHEDULERTYPE_TAG Dcm_Svc86SchdOnDidChangeSchedulerType;
# endif

/*! Service 0x86 subfunction 0x07 scheduler data */
# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
struct DCM_SVC86SCHDONCOMPOFVALSCHEDULERTYPE_TAG
{
  Dcm_DidMgrDidOpTypeContextType DidOpTypeContext;                            /*!< Information about operations on current DID */
  Dcm_MsgItemType                Buffer[DCM_SVC86_07_MAX_SUPPORTED_COMPARE_LENGTH]; /*!< Buffer for reading the DID data during scheduling */
  Dcm_MsgLenType                 ReadIndex;                                   /*!< Number of bytes that were already read */
  Dcm_OpStatusType               OpStatus;                                    /*!< Current operation status */
  uint8                          DidIndex;                                    /*!< Scheduler table entry that is currently processed */
  uint8                          DidIndexWinner;                              /*!< Index to scheduler table entry of the first detected event */
};
typedef struct DCM_SVC86SCHDONCOMPOFVALSCHEDULERTYPE_TAG Dcm_Svc86SchdOnCompOfValSchedulerType;
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/*! Service 0x86 scheduler data */
struct DCM_SVC86SCHDSINGLETONCONTEXTTYPE_TAG
{
#  if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdOnCompOfValSchedulerType OnCompOfVal;  /*!< Service 0x86 subfunction 0x07 scheduler data */
#  endif
#  if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdOnDidChangeSchedulerType OnDidChange;  /*!< Service 0x86 subfunction 0x03 scheduler data */
#  endif
};
typedef struct DCM_SVC86SCHDSINGLETONCONTEXTTYPE_TAG Dcm_Svc86SchdSingletonContextType;
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
struct DCM_SVC86SCHDDTCRECORDINFORINGBUFFERTYPE_TAG
{
  uint32 DtcQueue[DCM_SVC_86_MAX_NUM_DTC_STATUS_CHANGED_EVENTS]; /*!< DTCStatusMask of the eventTypeRecord */
  uint8  ReadIndex;                                              /*!< The index of the DTC with which the next STRT is triggered */
  uint8  ReadSchdIndex;                                          /*!< The index of the DTC until which the STRTs are triggered in current scheduler call */
  uint8  WriteIndex;                                             /*!< The index in the buffer to which the next DTC will be written */
  uint8  NumElements;                                            /*!< Total number of DTC stored */
};
typedef struct DCM_SVC86SCHDDTCRECORDINFORINGBUFFERTYPE_TAG Dcm_Svc86SchdDtcRecordInfoRingBufferType;
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT()
 *********************************************************************************************************************/
/*! \brief          Triggers an internal request if Svc86 scheduler is active.
 *  \details        -
 *  \return         DCM_E_OK        Trigger of STRT was successful
 *  \return         DCM_E_NOT_OK    Trigger of STRT was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT(
  PduIdType rxPduId,
  uint16 sourceAddress,
  Dcm_MsgType data,
  PduLengthType length
  );

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT_OnDTCStatusChange()
 *********************************************************************************************************************/
/*! \brief          Triggers an internal request whenever the configured DTC status change.
 *  \details        -
 *  \return         DCM_E_OK        Trigger of STRT was successful
 *  \return         DCM_E_NOT_OK    Trigger of STRT was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT_OnDTCStatusChange(
  void
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdCancel()
 *********************************************************************************************************************/
/*! \brief          Cancels all ongoing DID operations.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdCancel(
  void
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT_OnChangeOfDid()
 *********************************************************************************************************************/
/*! \brief          Triggers an internal request whenever a configured DID is changed.
 *  \details        -
 *  \return         DCM_E_OK        Trigger of STRT was successful
 *  \return         DCM_E_NOT_OK    Trigger of STRT was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT_OnChangeOfDid(
  void
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT_OnComparisonOfValues()
 *********************************************************************************************************************/
/*! \brief          Triggers an internal request whenever the comparison condition is met.
 *  \details        -
 *  \return         DCM_E_OK        Trigger of STRT was successful
 *  \return         DCM_E_NOT_OK    Trigger of STRT was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT_OnComparisonOfValues(
  void
  );
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT_DtcRecordInfo()
 *********************************************************************************************************************/
/*! \brief          Triggers an internal request on DTC change..
 *  \details        -
 *  \return         DCM_E_OK        Trigger of STRT was successful
 *  \return         DCM_E_NOT_OK    Trigger of STRT was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT_DtcRecordInfo(
  void
  );
# endif

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdTaskOnDTCStatusChange()
 *********************************************************************************************************************/
/*! \brief          Processes task events related to onDTCStatusChange
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdTaskOnDTCStatusChange(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdEventRecordInit()
 *********************************************************************************************************************/
/*! \brief          Initializes all event records.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdEventRecordInit(
  void
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SubFunSample()
 *********************************************************************************************************************/
/*! \brief          Samples multiple DID records for subfunction 0x03.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_03SubFunSample(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SubFunSample()
 *********************************************************************************************************************/
/*! \brief          Samples multiple DID records for subfunction 0x07.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_07SubFunSample(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdTaskOnChangeOfDID()
 *********************************************************************************************************************/
/*! \brief          Processes task events related to onChangeOfDataIdentifier
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdTaskOnChangeOfDID(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdTaskOnComparisonOfValues()
 *********************************************************************************************************************/
/*! \brief          Processes task events related to onComparisonOfValues
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdTaskOnComparisonOfValues(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdTaskDtcRecordInfo()
 *********************************************************************************************************************/
/*! \brief          Processes task events related to DtcRecordInfo
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdTaskDtcRecordInfo(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SubFuncInitOperation()
 *********************************************************************************************************************/
/*! \brief          Initialize scheduler for next to be read DID.
 *  \details        -
 *  \param[in]      did              The DID that is about to be read
 *  \return         DCM_E_OK         Continue processing
 *  \return         DCM_E_PENDING    DID is currently in use by another service
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_03SubFuncInitOperation(
  uint16 did
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SubFuncInitOperation()
 *********************************************************************************************************************/
/*! \brief          Initialize scheduler for next to be read DID.
 *  \details        -
 *  \param[in]      did              The DID that is about to be read
 *  \return         DCM_E_OK         Continue processing
 *  \return         DCM_E_PENDING    DID is currently in use by another service
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07SubFuncInitOperation(
  uint16 did
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SubFuncDetectChange()
 *********************************************************************************************************************/
/*! \brief          Compares the data read by the scheduler with the stored reference data for subfunction 0x03.
 *  \details        -
 *  \param[in]      pDidRecord    The record of the DID that was read
 *  \return         DCM_E_OK      A change of the data was detected and the reference is updated
 *  \return         DCM_E_NOT_OK  The data is equal
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_03SubFuncDetectChange(
  Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SubFuncDetectChange()
 *********************************************************************************************************************/
/*! \brief          Compares the data read by the scheduler with the stored reference data for subfunction 0x07.
 *  \details        -
 *  \param[in]      pDidRecord    The record of the DID that was read
 *  \return         DCM_E_OK      A change of the data was detected and the reference is updated
 *  \return         DCM_E_NOT_OK  The data is equal
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07SubFuncDetectChange(
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07CompareLogicDidChange()
 *********************************************************************************************************************/
/*! \brief          Compares the data read by the scheduler with the stored reference data for subfunction 0x07.
 *  \details        -
 *  \param[in]      pDidRecord    The record of the DID that was read
 *  \return         DCM_E_OK      A change of the data was detected and the reference is updated
 *  \return         DCM_E_NOT_OK  The data is equal
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07CompareLogicDidChange(
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07UnsignedComparison()
 *********************************************************************************************************************/
/*! \brief         Compares the unsigned data read for subfunction 0x07.
 *  \details        -
 *  \param[in]      pDidRecord    The record of the DID that was read
 *  \return         DCM_E_OK      A change of the data was detected and the reference is updated
 *  \return         DCM_E_NOT_OK  The data is equal
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07UnsignedComparison(
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord,
  uint32  measuredData
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SignedComparison()
 *********************************************************************************************************************/
/*! \brief         Compares the signed data read for subfunction 0x07.
 *  \details        -
 *  \param[in]      pDidRecord    The record of the DID that was read
 *  \return         DCM_E_OK      A change of the data was detected and the reference is updated
 *  \return         DCM_E_NOT_OK  The data is equal
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07SignedComparison(
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord,
  uint32  measuredData,
  uint32  bitLengh
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SchedulerProcessEntry()
 *********************************************************************************************************************/
/*! \brief          Reads a specific DID and compares the data to the reference data for subfuction 0x03.
 *  \details        -
 *  \param[in]      pDidRecord      The record of the DID that is about to be read
 *  \return         DCM_E_OK        Continue processing
 *  \return         DCM_E_PENDING   Processing was postponed
 *  \return         DCM_E_NOT_OK    Something went wrong or DID data has not changed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_03SchedulerProcessEntry(
  Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SchedulerProcessEntry()
 *********************************************************************************************************************/
/*! \brief          Reads a specific DID and compares the data to the reference data for subfunction 0x07.
 *  \details        -
 *  \param[in]      pDidRecord      The record of the DID that is about to be read
 *  \return         DCM_E_OK        Continue processing
 *  \return         DCM_E_PENDING   Processing was postponed
 *  \return         DCM_E_NOT_OK    Something went wrong or DID data has not changed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07SchedulerProcessEntry(
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord
  );
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferPrepareReqMsg()
*********************************************************************************************************************/
/*! \brief          Read the DTC and copy the DTC to the STRT request buffer to trigger STRT.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_09RingBufferPrepareReqMsg(
  void
  );
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferRemoveElement()
 *********************************************************************************************************************/
/*! \brief          Remove the DTC from the ring buffer with which the STRT has already been triggered.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_09RingBufferRemoveElement(
  void
  );
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09TaskTriggerIntmResponse()
 *********************************************************************************************************************/
/*! \brief          Send intermediate response to tester before triggering STRT.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_09TaskTriggerIntmResponse(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09TaskProcessEvents()
 *********************************************************************************************************************/
/*! \brief          Remove the DTC from the ring buffer with which the STRT has already been triggered.
 *  \details        -
 *  \param[in]      pEventContext     The events of the Svc86 task context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_09TaskProcessEvents(
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SubFuncCancel()
 *********************************************************************************************************************/
/*! \brief          Cancels the scheduled DID for subfunction 0x03.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_03SubFuncCancel(
  void
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SubFuncCancel()
 *********************************************************************************************************************/
/*! \brief          Cancels the scheduled DID for subfunction 0x07.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_07SubFuncCancel(
  void
  );
# endif

# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Public RoE context used by NvM for later restoring */
VAR(Dcm_Svc86NvMDataType, DCM_VAR_NOINIT) Dcm_Svc86NvMData;

/*! RoE context in default session */
DCM_LOCAL VAR(Dcm_Svc86NvMDataType, DCM_VAR_NOINIT) Dcm_Svc86DefaultSession;                                                                         /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/*! Scheduler for periodic sampling */
DCM_LOCAL VAR(Dcm_Svc86SchdSingletonContextType, DCM_VAR_NOINIT) Dcm_Svc86SchdSingletonContext;
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/*! The ring buffer for storing the Dtc related to Svc86 09 triggered events */
DCM_LOCAL VAR(Dcm_Svc86SchdDtcRecordInfoRingBufferType, DCM_VAR_NOINIT) Dcm_Svc86Schd_09DtcRingBuffer;
# endif
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CONST_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/*! Default session context */
CONST(Dcm_Svc86NvMDataType, DCM_CONST) Dcm_Svc86DefaultNvMData = { 0u };                                                                             /* PRQA S 1514 */ /* MD_Dcm_ObjectOnlyAccessedOnce */
# endif
# define DCM_STOP_SEC_CONST_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_VAR_NO_INIT_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Activation State of scheduler in case the application wants to pause the scheduling */
DCM_LOCAL VAR(boolean, DCM_VAR_NOINIT) Dcm_Svc86SchdActivationState;
# define DCM_STOP_SEC_VAR_NO_INIT_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT(
  PduIdType rxPduId,
  uint16 sourceAddress,
  Dcm_MsgType data,
  PduLengthType length
  )
{
  Std_ReturnType lResult = DCM_E_OK;

  if (Dcm_Svc86SchdActivationState == TRUE)
  {
    lResult =  Dcm_NetRxIndInternal(rxPduId
                                   ,sourceAddress
                                   ,data
                                   ,length);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    /* Do nothing */
  }

  return lResult;
}

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT_OnDTCStatusChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT_OnDTCStatusChange(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  return Dcm_Svc86SchdSendSTRT(Dcm_Svc86CurrentSession.RxPduId
                              ,Dcm_Svc86CurrentSession.ClientSrcAddr
                              ,Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord.STRT
                              ,Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord.STRTLength);                                                /* SBSW_DCM_POINTER_READ_86DTCRECORD */
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT_OnChangeOfDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT_OnChangeOfDid(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord;

  /* ----- Implementation ----------------------------------------------- */
  pDidRecord = Dcm_Svc86Schd_03GetDidRecord(Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndexWinner);

  return Dcm_Svc86SchdSendSTRT(Dcm_Svc86CurrentSession.RxPduId
                              ,Dcm_Svc86CurrentSession.ClientSrcAddr
                              ,pDidRecord->STRT
                              ,pDidRecord->STRTLength);                                                                                              /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT_OnComparisonOfValues()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT_OnComparisonOfValues(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord;

  /* ----- Implementation ----------------------------------------------- */
  pDidRecord = Dcm_Svc86Schd_07GetDidRecord(Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndexWinner);

  return Dcm_Svc86SchdSendSTRT(Dcm_Svc86CurrentSession.RxPduId
                              ,Dcm_Svc86CurrentSession.ClientSrcAddr
                              ,pDidRecord->STRT
                              ,pDidRecord->STRTLength);                                                                                              /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdSendSTRT_DtcRecordInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SchdSendSTRT_DtcRecordInfo(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  return Dcm_Svc86SchdSendSTRT(Dcm_Svc86CurrentSession.RxPduId
                              ,Dcm_Svc86CurrentSession.ClientSrcAddr
                              ,Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRT
                              ,Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRTLength);                                              /* SBSW_DCM_POINTER_READ_86DTCRECORD */
}
# endif

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdTaskOnDTCStatusChange()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdTaskOnDTCStatusChange(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_DETECTED))
  {
    if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_SCHEDULER_RATE_ELAPSED))
    {
      /* Scheduler rate elapsed, trigger STRT */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_SEND_STRT);                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* Scheduler rate not yet elapsed, postpone sending */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_DETECTED);                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_SEND_STRT))
  {
    Std_ReturnType lStdResult;

    lStdResult = Dcm_Svc86SchdSendSTRT_OnDTCStatusChange();

    if (lStdResult == DCM_E_PENDING)
    {
      /* Set STRT state to SENDING and retry sending next task cycle */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_SEND_STRT);                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SubFunSample()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_03SubFunSample(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                       lStdReturn;
  Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord;

  DCM_IGNORE_UNREF_PARAM(pContext);

  /* ----- Implementation ----------------------------------------------- */
  do
  {
    pDidRecord = Dcm_Svc86Schd_03GetDidRecord(Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndex);

    lStdReturn = Dcm_Svc86Schd_03SchedulerProcessEntry(pDidRecord);                                                                                  /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */

    if (lStdReturn == DCM_E_PENDING)
    {
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_03_DID_SAMPLE_AND_COMPARE);                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    }

    if (lStdReturn == DCM_E_OK)
    {
      if (Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndexWinner == DCM_SVC86_DID_CHANGE_EVENT_NO_WINNER)
      {
        Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndexWinner = Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndex;
      }
    }

    ++Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndex;

    Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndex %= Dcm_Svc86SchdGetOnDidChangeNumRecords();
  }
  while (Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndex != 0u);

  if ( (lStdReturn != DCM_E_PENDING)
     && (Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndexWinner != DCM_SVC86_DID_CHANGE_EVENT_NO_WINNER))
  {
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_ROE_86_03_DID_SEND_STRT);                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SubFunSample()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_07SubFunSample(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                       lStdReturn;
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord;

  DCM_IGNORE_UNREF_PARAM(pContext);

  /* ----- Implementation ----------------------------------------------- */
  do
  {
    pDidRecord = Dcm_Svc86Schd_07GetDidRecord(Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndex);

    lStdReturn = Dcm_Svc86Schd_07SchedulerProcessEntry(pDidRecord);                                                                                  /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */

    if (lStdReturn == DCM_E_PENDING)
    {
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_07_DID_SAMPLE_AND_COMPARE);                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    if (lStdReturn == DCM_E_OK)
    {
      if (Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndexWinner == DCM_SVC86_COMPARISON_VALUE_EVENT_NO_WINNER)
      {
        Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndexWinner = Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndex;
        Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndex = 0u;
      }
    }

    if ((lStdReturn == DCM_E_PENDING) || (Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndexWinner != DCM_SVC86_COMPARISON_VALUE_EVENT_NO_WINNER))
    {
      break;
    }

    ++Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndex;

    Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndex %= Dcm_Svc86CurrentSession.EventRecord.OnCompOfVal.NumDidRecords;
  } while (Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndex != 0u);

  if ( (lStdReturn != DCM_E_PENDING)
     && (Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndexWinner != DCM_SVC86_COMPARISON_VALUE_EVENT_NO_WINNER) )
  {
    Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_ROE_86_07_DID_SEND_STRT);                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdTaskOnChangeOfDID()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdTaskOnChangeOfDID(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_SCHEDULER_RATE_ELAPSED))
  {
    if ( (Dcm_Svc86CurrentSession.EventRecord.OnDidChange.NumDidRecords != 0u) && (Dcm_Svc86SchdActivationState == TRUE) )
    {
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_ROE_86_03_DID_SAMPLE_AND_COMPARE);                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_03_DID_SAMPLE_AND_COMPARE))
  {
    Dcm_Svc86Schd_03SubFunSample(pContext, pEventContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_03_DID_SEND_STRT))
  {
    Std_ReturnType lStdResult;

    lStdResult = Dcm_Svc86SchdSendSTRT_OnChangeOfDid();

    if (lStdResult != DCM_E_PENDING)
    {
      Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndexWinner = DCM_SVC86_DID_CHANGE_EVENT_NO_WINNER;
    }
    else
    {
      /* Set STRT state to SENDING and retry sending next task cycle */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_03_DID_SEND_STRT);                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdTaskOnComparisonOfValues()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdTaskOnComparisonOfValues(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_SCHEDULER_RATE_ELAPSED))
  {
    if ( (Dcm_Svc86CurrentSession.EventRecord.OnCompOfVal.NumDidRecords != 0u) && (Dcm_Svc86SchdActivationState == TRUE) )
    {
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_ROE_86_07_DID_SAMPLE_AND_COMPARE);                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_07_DID_SAMPLE_AND_COMPARE))
  {
    Dcm_Svc86Schd_07SubFunSample(pContext, pEventContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_07_DID_SEND_STRT))
  {
    Std_ReturnType lStdResult;

    lStdResult = Dcm_Svc86SchdSendSTRT_OnComparisonOfValues();

    if (lStdResult != DCM_E_PENDING)
    {
      Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndexWinner = DCM_SVC86_COMPARISON_VALUE_EVENT_NO_WINNER;
    }
    else
    {
      /* Set STRT state to SENDING and retry sending next task cycle */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_07_DID_SEND_STRT);                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdTaskDtcRecordInfo()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdTaskDtcRecordInfo(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_SCHEDULER_RATE_ELAPSED))
  {
    /* Update the ring buffer read progress */
    Dcm_Svc86Schd_09DtcRingBuffer.ReadSchdIndex = Dcm_Svc86Schd_09DtcRingBuffer.WriteIndex;

    if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_BUFFER_FULL))
    {
      /* Trigger intermediate response if the buffer is full and some events are lost */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_INTM_RESP);                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_DETECTED))
    {
      /* Start triggering STRTs right away when intermediate responses are not expected */
      Dcm_TskSetLocalEvent(pEventContext->Ev, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_STRT);                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* Intentionally left blank */
    }
  }
  else
  {
    /* Postpone intermediate response and STRT to next scheduler call */
    Dcm_TskTaskEvMemType lPostEv = ((pEventContext->Ev) &
      (DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_DETECTED |
        DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_BUFFER_FULL));

    Dcm_TskSetLocalEvent(pEventContext->PostEv, lPostEv);                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_INTM_RESP))
  {
    if (Dcm_Svc86SchdActivationState == TRUE)
    {
      Dcm_Svc86Schd_09TaskTriggerIntmResponse(pContext, pEventContext);                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      if (Dcm_Svc86_09AnyElementToSend())
      {
        Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_STRT);                                               /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
  }

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_STRT))
  {
    Dcm_Svc86Schd_09TaskProcessEvents(pEventContext);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SubFuncInitOperation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_03SubFuncInitOperation(
  uint16 did
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult;

  /* ----- Implementation ----------------------------------------------- */
  lStdResult = Dcm_RsrcMgrGetDidLock(DCM_INITIAL, did, DCM_RSRCMGR_DIDLOCK_OWNER_SID86_03);

  if (lStdResult == DCM_E_OK)
  {
    Dcm_Svc86SchdSingletonContext.OnDidChange.OpStatus = DCM_INITIAL;
    Dcm_Svc86SchdSingletonContext.OnDidChange.ReadIndex = 0u;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SubFuncInitOperation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07SubFuncInitOperation(
  uint16 did
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult;

  /* ----- Implementation ----------------------------------------------- */
  lStdResult = Dcm_RsrcMgrGetDidLock(DCM_INITIAL, did, DCM_RSRCMGR_DIDLOCK_OWNER_SID86_07);

  if (lStdResult == DCM_E_OK)
  {
    Dcm_Svc86SchdSingletonContext.OnCompOfVal.OpStatus = DCM_INITIAL;
    Dcm_Svc86SchdSingletonContext.OnCompOfVal.ReadIndex = 0u;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SubFuncDetectChange()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_03SubFuncDetectChange(
  Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  lStdResult = DCM_E_NOT_OK;
  boolean         lUpdateRef = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if (pDidRecord->RefState == DCM_SVC86_DID_REFERENCE_STATE_INVALID)
  {
    pDidRecord->RefState = DCM_SVC86_DID_REFERENCE_STATE_VALID;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    lUpdateRef = TRUE;
  }
  else
  {
    Dcm_DidMgrDidLengthType lIndex;

    for (lIndex = 0; lIndex < pDidRecord->DidInfoContext.DidLength; ++lIndex)
    {
      if (pDidRecord->RefDidData[lIndex] != Dcm_Svc86SchdSingletonContext.OnDidChange.Buffer[lIndex])
      {
        lUpdateRef = TRUE;
        lStdResult = DCM_E_OK;
        break;
      }
    }
  }

  if (lUpdateRef == TRUE)
  {
    /* Change detected, update reference */
    Dcm_UtiMemCopySafe(Dcm_Svc86SchdSingletonContext.OnDidChange.Buffer
      , pDidRecord->RefDidData
      , 0u
      , DCM_SVC_86_MAX_SUPPORTED_DID_LENGTH
      , pDidRecord->DidInfoContext.DidLength);                                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SubFuncDetectChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07SubFuncDetectChange(
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  lStdResult;

  /* ----- Implementation ----------------------------------------------- */
  lStdResult = Dcm_Svc86Schd_07CompareLogicDidChange(pDidRecord);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

  return lStdResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07CompareLogicDidChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07CompareLogicDidChange(
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32          lMeasuredData;
  uint32          lTempVal = 0u;
  Std_ReturnType  lStdResult = DCM_E_NOT_OK;
  uint8           lTempMask = 0u;
  uint8           lStartByteOffset;
  uint8           lEndByteOffset;
  uint8           lShiftIteration;

  /* ----- Implementation ----------------------------------------------- */
  /* Offset calculation */
  lStartByteOffset = (uint8)(pDidRecord->Offset / 8u);
  lEndByteOffset = lStartByteOffset + 4u;
  lShiftIteration = (uint8)(pDidRecord->Offset % 8u);

  lMeasuredData = Dcm_UtiMake32Bit((Dcm_Svc86SchdSingletonContext.OnCompOfVal.Buffer[lStartByteOffset + 0u])                                         /* PRQA S 2985 */ /* MD_Dcm_Redundant_2986 */
                                  ,(Dcm_Svc86SchdSingletonContext.OnCompOfVal.Buffer[lStartByteOffset + 1u])
                                  ,(Dcm_Svc86SchdSingletonContext.OnCompOfVal.Buffer[lStartByteOffset + 2u])
                                  ,(Dcm_Svc86SchdSingletonContext.OnCompOfVal.Buffer[lStartByteOffset + 3u]));

  lMeasuredData = (lMeasuredData << lShiftIteration);

  /* extract the last shifted bits from the end byte offset */
  if (pDidRecord->Offset != 0u)
  {
    lTempMask = (0xFFu << (8u - lShiftIteration));
    lTempVal = (uint32)(Dcm_Svc86SchdSingletonContext.OnCompOfVal.Buffer[lEndByteOffset]) & (uint32)(lTempMask);
    lTempVal = lTempVal >> (8u - lShiftIteration);
    lMeasuredData |= lTempVal;
  }

  /* Bit length check */
  if (pDidRecord->BitLength != 0u)
  {
    lMeasuredData = (lMeasuredData >> (32u - (pDidRecord->BitLength)));
  }

  /* Sign check */
  if (pDidRecord->Signedness == TRUE) /* signed comparison */
  {
    lStdResult = Dcm_Svc86Schd_07SignedComparison(pDidRecord, lMeasuredData, pDidRecord->BitLength);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    lStdResult = Dcm_Svc86Schd_07UnsignedComparison(pDidRecord, lMeasuredData);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07UnsignedComparison()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07UnsignedComparison(                                                                  /* PRQA S 6030 */ /* MD_MSR_STCYC */
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord,
  uint32  measuredData
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType         lStdResult = DCM_E_NOT_OK;
  uint32                 lHysteresisVal;

  /* ----- Implementation ----------------------------------------------- */

  /* Hysteresis calculation */
  if (pDidRecord->RawRefCompVal == 0u)
  {
    lHysteresisVal = (uint32)(pDidRecord->HysteresisPercentage); /* absolute value */
  }
  else
  {
    lHysteresisVal = (uint32)(((pDidRecord->HysteresisPercentage) * (pDidRecord->RawRefCompVal)) / (100u));
  }

  switch (pDidRecord->ComparisonLogic)
  {
    case DCM_SVC86_07_COMPARISON_LESS_THAN_MEASURED:
    {
      if ((pDidRecord->TriggerActive == TRUE)
        && (pDidRecord->RawRefCompVal < measuredData))
      {
        pDidRecord->TriggerActive = FALSE;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_OK;
      }
      else /* Hysteresis is not active or measured value is less than the comparison value limit */
      {
        if ((pDidRecord->RawRefCompVal - lHysteresisVal) > measuredData) /* measured data below Hysteresis value */
        {
          pDidRecord->TriggerActive = TRUE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
        }
      }
      break;
    }
    case DCM_SVC86_07_COMPARISON_MORE_THAN_MEASURED:
    {
      if ((pDidRecord->TriggerActive == TRUE)
        && (pDidRecord->RawRefCompVal > measuredData))
      {
        pDidRecord->TriggerActive = FALSE;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_OK;
      }
      else /* Hysteres is not active or measured value is greater than the comparison value limit */
      {
        if ((pDidRecord->RawRefCompVal + lHysteresisVal) < measuredData)
        {
          pDidRecord->TriggerActive = TRUE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
        }
      }
      break;
    }
    case DCM_SVC86_07_COMPARISON_EQUAL_MEASURED:
      if (pDidRecord->RawRefCompVal == measuredData)
      {
        lStdResult = DCM_E_OK;
      }
      break;
    case DCM_SVC86_07_COMPARISON_UNEQUAL_MEASURED:
      if (pDidRecord->RawRefCompVal != measuredData)
      {
        lStdResult = DCM_E_OK;
      }
      break;
    default:                                                                                                                                         /*   COV_DCM_RTM_UNREACHABLE X */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
      break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SignedComparison()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07SignedComparison(                                                                    /* PRQA S 6030 */ /* MD_MSR_STCYC */
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord,
  uint32  measuredData,
  uint32  bitLengh
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  sint32         lMeasuredData;
  sint32         lRawRefCompVal;
  uint32         lHysteresisVal;

  /* ----- Implementation ----------------------------------------------- */
  if (bitLengh == 8u)
  {
    lMeasuredData = (sint8)measuredData;
    lRawRefCompVal = (sint8)pDidRecord->RawRefCompVal;
  }
  else if (bitLengh == 16u)
  {
    lMeasuredData = (sint16)measuredData;
    lRawRefCompVal = (sint16)pDidRecord->RawRefCompVal;
  }
  else
  {
    /* 32 bits */
    lMeasuredData = (sint32)measuredData;
    lRawRefCompVal = (sint32)pDidRecord->RawRefCompVal;
  }

  /* Hysteresis calculation */
  if (lRawRefCompVal == 0)
  {
    lHysteresisVal = pDidRecord->HysteresisPercentage; /* absolute value */
  }
  else
  {
    sint32 lAbsRawRefCompVal = Dcm_UtiMathAbs(lRawRefCompVal);

    lHysteresisVal = ((uint32)lAbsRawRefCompVal * pDidRecord->HysteresisPercentage) / 100u;
  }

  switch (pDidRecord->ComparisonLogic)
  {
    case DCM_SVC86_07_COMPARISON_LESS_THAN_MEASURED:
    {
      if ((pDidRecord->TriggerActive == TRUE)
        && (lRawRefCompVal < lMeasuredData))
      {
        pDidRecord->TriggerActive = FALSE;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_OK;
      }
      else /* Hysteresis is not active or measured value is less than the comparison value limit */
      {
        if ((lRawRefCompVal - (sint32)lHysteresisVal) > lMeasuredData) /* measured data below Hysteresis value */
        {
          pDidRecord->TriggerActive = TRUE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
        }
        else
        {
          /* Hysteresis limit is not reached yet */
        }
      }
      break;
    }
    case DCM_SVC86_07_COMPARISON_MORE_THAN_MEASURED:
    {
      if ((pDidRecord->TriggerActive == TRUE)
        && (lRawRefCompVal > lMeasuredData))
      {
        pDidRecord->TriggerActive = FALSE;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_OK;
      }
      else /* Hysteres is not active or measured value is greater than the comparison value limit */
      {
        if ((lRawRefCompVal + (sint32)lHysteresisVal) < lMeasuredData)
        {
          pDidRecord->TriggerActive = TRUE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
        }
        else
        {
          /* Hysteresis limit is not reached yet */
        }
      }
      break;
    }
    case DCM_SVC86_07_COMPARISON_EQUAL_MEASURED:
      if (lRawRefCompVal == lMeasuredData)
      {
        lStdResult = DCM_E_OK;
      }
      break;
    case DCM_SVC86_07_COMPARISON_UNEQUAL_MEASURED:
      if (lRawRefCompVal != lMeasuredData)
      {
        lStdResult = DCM_E_OK;
      }
      break;
    default:                                                                                                                                         /* COV_DCM_RTM_UNREACHABLE X */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
      break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SchedulerProcessEntry()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_03SchedulerProcessEntry(
  Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_03) == FALSE)
  {
    lStdResult = Dcm_Svc86Schd_03SubFuncInitOperation(pDidRecord->DidInfoContext.Did);
  }

  if (lStdResult == DCM_E_OK)
  {
    Dcm_DiagDataContextType      lDataContext;
    Dcm_NegativeResponseCodeType lNrc; /* Unused since no NR possible */

    Dcm_UtiInitDataContext(&lDataContext
                          ,Dcm_Svc86SchdSingletonContext.OnDidChange.Buffer
                          ,DCM_SVC_86_MAX_SUPPORTED_DID_LENGTH);                                                                                     /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

    Dcm_UtiCommitData(&lDataContext, Dcm_Svc86SchdSingletonContext.OnDidChange.ReadIndex);                                                           /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

    lStdResult = Dcm_DidMgrReadDid(Dcm_Svc86SchdSingletonContext.OnDidChange.OpStatus
                                  ,&lDataContext
                                  ,&(pDidRecord->DidInfoContext)
                                  ,&(Dcm_Svc86SchdSingletonContext.OnDidChange.DidOpTypeContext)
                                  ,&lNrc);                                                                                                           /* SBSW_DCM_COMB_PTR_FORWARD */

    switch (lStdResult)
    {
      case DCM_E_OK:
        lStdResult = Dcm_Svc86Schd_03SubFuncDetectChange(pDidRecord);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_03);
        break;
      case DCM_E_PENDING:
        Dcm_Svc86SchdSingletonContext.OnDidChange.OpStatus = DCM_PENDING;
        Dcm_Svc86SchdSingletonContext.OnDidChange.ReadIndex = lDataContext.Usage;
        break;
      default: /* DCM_E_NOT_OK */
        Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_03);
        break;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SchedulerProcessEntry()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Schd_07SchedulerProcessEntry(
  Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_07) == FALSE)
  {
    lStdResult = Dcm_Svc86Schd_07SubFuncInitOperation(pDidRecord->DidInfoContext.Did);
  }

  if (lStdResult == DCM_E_OK)
  {
    Dcm_DiagDataContextType      lDataContext;
    Dcm_NegativeResponseCodeType lNrc; /* Unused since no NR possible */

    Dcm_UtiInitDataContext(&lDataContext
                          ,Dcm_Svc86SchdSingletonContext.OnCompOfVal.Buffer
                          ,DCM_SVC86_07_MAX_SUPPORTED_COMPARE_LENGTH);                                                                               /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

    Dcm_UtiCommitData(&lDataContext, Dcm_Svc86SchdSingletonContext.OnCompOfVal.ReadIndex);                                                           /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

    lStdResult = Dcm_DidMgrReadDid(Dcm_Svc86SchdSingletonContext.OnCompOfVal.OpStatus
                                  ,&lDataContext
                                  ,&(pDidRecord->DidInfoContext)
                                  ,&(Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidOpTypeContext)
                                  ,&lNrc);                                                                                                           /* SBSW_DCM_COMB_PTR_FORWARD */

    switch (lStdResult)
    {
      case DCM_E_OK:
        lStdResult = Dcm_Svc86Schd_07SubFuncDetectChange(pDidRecord);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
        Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_07);
        break;
      case DCM_E_PENDING:
        Dcm_Svc86SchdSingletonContext.OnCompOfVal.OpStatus = DCM_PENDING;
        Dcm_Svc86SchdSingletonContext.OnCompOfVal.ReadIndex = lDataContext.Usage;
        break;
      default: /* DCM_E_NOT_OK */
        Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_07);
        break;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferPrepareReqMsg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_09RingBufferPrepareReqMsg(
  void
  )
{
  uint32 lEventTypeDTC = Dcm_Svc86Schd_09DtcRingBuffer.DtcQueue[Dcm_Svc86Schd_09DtcRingBuffer.ReadIndex];

  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRT[2u] = Dcm_UtiGetHiLoByte(lEventTypeDTC);                                          /* SBSW_DCM_POINTER_WRITE_86DTCRECORDSTRT */
  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRT[3u] = Dcm_UtiGetLoHiByte(lEventTypeDTC);                                          /* SBSW_DCM_POINTER_WRITE_86DTCRECORDSTRT */
  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRT[4u] = Dcm_UtiGetLoLoByte(lEventTypeDTC);                                          /* SBSW_DCM_POINTER_WRITE_86DTCRECORDSTRT */
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferRemoveElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_09RingBufferRemoveElement(
  void
  )
{
  Dcm_Svc86Schd_09DtcRingBuffer.ReadIndex++;

  if (Dcm_Svc86Schd_09DtcRingBuffer.ReadIndex == DCM_SVC_86_MAX_NUM_DTC_STATUS_CHANGED_EVENTS)
  {
    Dcm_Svc86Schd_09DtcRingBuffer.ReadIndex = 0u;
  }
  Dcm_UtiEnterCriticalSection();
  /*=================================*
   BEGIN CRITICAL SECTION
  *=================================*/
  --Dcm_Svc86Schd_09DtcRingBuffer.NumElements;
  /*=================================*
   END CRITICAL SECTION
  *=================================*/
  Dcm_UtiLeaveCriticalSection();
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09TaskProcessEvents()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_09TaskProcessEvents(
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Std_ReturnType lTriggerResult;

  Dcm_Svc86Schd_09RingBufferPrepareReqMsg();
  lTriggerResult = Dcm_Svc86SchdSendSTRT_DtcRecordInfo();

  /* If the trigger was successful or retry is not needed: */
  if (lTriggerResult != DCM_E_PENDING)
  {
    Dcm_Svc86Schd_09RingBufferRemoveElement();
  }

  if (Dcm_Svc86_09AnyElementToSend())
  {
    /* Set STRT state to SENDING and continue reading from buffer */
    Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_STRT);                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03SubFuncCancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_03SubFuncCancel(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_03) == TRUE)
  {
    Dcm_Svc86OnDidChangeDidRecordPtrType  pDidRecord;
    Dcm_NegativeResponseCodeType          lNrc;/* not evaluated after the API call */
    Dcm_DiagDataContextType               lDataContext;

    pDidRecord = Dcm_Svc86Schd_03GetDidRecord(Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndex);

    Dcm_UtiInitDataContext(&lDataContext
                          ,Dcm_Svc86SchdSingletonContext.OnDidChange.Buffer
                          ,DCM_SVC_86_MAX_SUPPORTED_DID_LENGTH);                                                                                     /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

/* Cancel ongoing operation immediately  */
    (void)Dcm_DidMgrReadDid(DCM_CANCEL
                           ,&lDataContext
                           ,&(pDidRecord->DidInfoContext)
                           ,&(Dcm_Svc86SchdSingletonContext.OnDidChange.DidOpTypeContext)
                           ,&lNrc);                                                                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */

    Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_03);
  }
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07SubFuncCancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_07SubFuncCancel(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  if (Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_07) == TRUE)
  {
    Dcm_Svc86OnCompOfValDidRecordPtrType  pDidRecord;
    Dcm_NegativeResponseCodeType          lNrc;/* not evaluated after the API call */
    Dcm_DiagDataContextType               lDataContext;

    pDidRecord = Dcm_Svc86Schd_07GetDidRecord(Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndex);

    Dcm_UtiInitDataContext(&lDataContext
                          ,Dcm_Svc86SchdSingletonContext.OnCompOfVal.Buffer
                          ,DCM_SVC86_07_MAX_SUPPORTED_COMPARE_LENGTH);                                                                               /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

/* Cancel ongoing operation immediately  */
    (void)Dcm_DidMgrReadDid(DCM_CANCEL
                           ,&lDataContext
                           ,&(pDidRecord->DidInfoContext)
                           ,&(Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidOpTypeContext)
                           ,&lNrc);                                                                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */

    Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID86_07);
  }
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdCancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdCancel(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
#  if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86Schd_03SubFuncCancel();
#  endif

#  if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86Schd_07SubFuncCancel();
#  endif
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdEventRecordInit()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86SchdEventRecordInit(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least lIndex;

  /* ----- Implementation ----------------------------------------------- */

#  if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
  for (lIndex = 0; lIndex < Dcm_Svc86CurrentSession.EventRecord.OnDidChange.NumDidRecords; ++lIndex)
  {
    Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord = Dcm_Svc86Schd_03GetDidRecord(lIndex);
    pDidRecord->RefState = DCM_SVC86_DID_REFERENCE_STATE_INVALID;                                                                                    /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
  }
#  endif

#  if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
  for (lIndex = 0; lIndex < Dcm_Svc86CurrentSession.EventRecord.OnCompOfVal.NumDidRecords; ++lIndex)
  {
    Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord = Dcm_Svc86Schd_07GetDidRecord(lIndex);
    pDidRecord->TriggerActive = TRUE;                                                                                                                /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
  }
#  endif
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09TaskTriggerIntmResponse()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86Schd_09TaskTriggerIntmResponse(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Dcm_NetConnRefMemType lConnHdl = Dcm_NetGetConnHdlByTesterAddress(Dcm_Svc86CurrentSession.ClientSrcAddr);

  if ((lConnHdl < DCM_NET_INVALID_CONNHDL)                                                                                                           /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#  if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
     && (((Dcm_NetIsGenericConnection(lConnHdl))                                                                                                     /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
      && (Dcm_Svc86CurrentSession.ClientSrcAddr <= DCM_NET_MAX_VAL_GENERIC_SRC_ADDRESS))
      || ((!Dcm_NetIsGenericConnection(lConnHdl))                                                                                                    /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
        && (Dcm_Svc86CurrentSession.ClientSrcAddr == Dcm_NetPbCfgGetConnTesterAddress(lConnHdl))))                                                   /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#  endif
    )
  {
    Dcm_NetTransportObjectPtrType pTranspObj; /* use a local copy to avoid collision with a RxIndication! */
    BufReq_ReturnType             lResult;

    lResult = Dcm_NetLockConnection(lConnHdl, &pTranspObj);                                                                                          /* SBSW_DCM_POINTER_FORWARD_STACK */

    if (lResult == BUFREQ_OK) /* try to lock the connection to this tester */
    {
      /* Source address of the request shall be the target address of the response. */
      pTranspObj->ClientSrcAddr = Dcm_Svc86CurrentSession.ClientSrcAddr;                                                                             /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
      pTranspObj->Flags = DCM_NET_TOBJ_FLAG_INTERNAL;                                                                                                /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
      pTranspObj->State = DCM_NET_TOBJ_STATE_PREPTX;                                                                                                 /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
      pTranspObj->ResType = DCM_NET_TOBJ_RESTYPE_ROE_INTMRSP;                                                                                        /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
      pTranspObj->BuffInfo.SduDataPtr = Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.IntmRspMsg;                                      /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */
      pTranspObj->BuffInfo.SduLength = Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.IntmRspMsgLength;                                 /* SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ */

      Dcm_DiagSetTranspObj(pContext->ThreadId, pTranspObj); /* store working context */                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);

      if (Dcm_Svc86_09AnyElementToSend())
      {
        Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_STRT);                                               /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
    else
    {
      /* The buffer is occupied. Retry at the next cycle */
      Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_INTM_RESP);                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
  }
}
# endif

# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86SchdInit(
  void
  )
{
#  if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndex = 0u;
  Dcm_Svc86SchdSingletonContext.OnDidChange.DidIndexWinner = DCM_SVC86_DID_CHANGE_EVENT_NO_WINNER;
#  endif

#  if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndex = 0u;
  Dcm_Svc86SchdSingletonContext.OnCompOfVal.DidIndexWinner = DCM_SVC86_COMPARISON_VALUE_EVENT_NO_WINNER;
#  endif
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc86SchdClearAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86SchdClearAll(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  Dcm_Svc86SchdStopAll();

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcEventState = DCM_SVC86_EVENT_STATE_CLEARED;
# endif
# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86CurrentSession.EventRecord.OnDidChange.NumDidRecords = 0u;
# endif
# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86CurrentSession.EventRecord.OnCompOfVal.NumDidRecords = 0u;
# endif
# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcEventState = DCM_SVC86_EVENT_STATE_CLEARED;
# endif
}

/**********************************************************************************************************************
 *  Dcm_Svc86SchdStopAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86SchdStopAll(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  Dcm_Svc86CurrentSession.RoEState = DCM_SVC86_ROE_STATE_STOPPED;

  Dcm_TmrStopTimer(DCM_TMR_ID_SVC86_SCHEDULER);

  Dcm_TskClrEvent(DCM_TSK_ID_ROE, DCM_TSK_EV_ALL_EVENTS);

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdCancel();
# endif
}
/**********************************************************************************************************************
 *  Dcm_Service86Init()
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
FUNC(void, DCM_CODE) Dcm_Service86Init(
  void
  )
{
  Dcm_Svc86SchdActivationState = TRUE;

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRT[0u] = 0x19u;                                                                      /* SBSW_DCM_POINTER_WRITE_86DTCRECORDSTRT */

  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.IntmRspMsg[0u] = 0xc6u;                                                                /* SBSW_DCM_POINTER_WRITE_86INTMRSPMSG */
  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.IntmRspMsg[1u] = 0x09u;                                                                /* SBSW_DCM_POINTER_WRITE_86INTMRSPMSG */
  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.IntmRspMsg[2u] = 0xFFu;                                                                /* SBSW_DCM_POINTER_WRITE_86INTMRSPMSG */
# endif

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
  if ((Dcm_Svc86CurrentSession.MagicNumber == DCM_CFG_FINAL_MAGIC_NUMBER)
#  if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
    && (Dcm_Svc86CurrentSession.CfgVariantId == Dcm_VarMgrGetActiveCfgVariantId())                                                                   /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#  endif
    && (Dcm_Svc86CurrentSession.RoEState == DCM_SVC86_ROE_STATE_STARTED_PERSISTENTLY))
  {
    Dcm_Svc86SchdStartAll();
  }
  else
# endif
  {
    /* Just clear RoE. Initalization of the scheduler will be done when RoE is started */
    Dcm_Svc86SchdClearAll();

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
    Dcm_Svc86CurrentSession.MagicNumber = DCM_CFG_FINAL_MAGIC_NUMBER;
#  if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
    Dcm_Svc86CurrentSession.CfgVariantId = Dcm_VarMgrGetActiveCfgVariantId();
#  endif
# endif
  }
}

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86Schd_09RingBufferInit(
  void
  )
{
  Dcm_Svc86Schd_09DtcRingBuffer.ReadIndex = 0u;
  Dcm_Svc86Schd_09DtcRingBuffer.ReadSchdIndex = 0u;

  Dcm_Svc86Schd_09DtcRingBuffer.WriteIndex = 0u;
  Dcm_Svc86Schd_09DtcRingBuffer.NumElements = 0u;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc86Task()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86Task(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext
  )
{
# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdTaskOnDTCStatusChange(pContext, pEventContext);                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdTaskOnChangeOfDID(pContext, pEventContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdTaskOnComparisonOfValues(pContext, pEventContext);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdTaskDtcRecordInfo(pContext, pEventContext);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif
}

/**********************************************************************************************************************
 *  Dcm_OnTimeoutSvc86Scheduler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutSvc86Scheduler(
  Dcm_ThreadIdMemType threadId
  )
{
  DCM_IGNORE_UNREF_PARAM(threadId);

  Dcm_TskSetEvent(DCM_TSK_ID_ROE, DCM_TSK_EV_ROE_SCHEDULER_RATE_ELAPSED);

  return DCM_SVC_86_SCHEDULER_RATE;
}

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferAddElement()
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
FUNC(void, DCM_CODE) Dcm_Svc86Schd_09RingBufferAddElement(
  uint32 DTC
  )
{
  if (Dcm_DebugDetectRuntimeError(Dcm_Svc86Schd_09DtcRingBuffer.WriteIndex >= DCM_SVC_86_MAX_NUM_DTC_STATUS_CHANGED_EVENTS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    Dcm_Svc86Schd_09RingBufferInit();
  }
  else
  {
    Dcm_Svc86Schd_09DtcRingBuffer.DtcQueue[Dcm_Svc86Schd_09DtcRingBuffer.WriteIndex] = DTC;                                                          /* SBSW_DCM_POINTER_WRITE_86RINGBUFFER */
    ++Dcm_Svc86Schd_09DtcRingBuffer.WriteIndex;

    if (Dcm_Svc86Schd_09DtcRingBuffer.WriteIndex == DCM_SVC_86_MAX_NUM_DTC_STATUS_CHANGED_EVENTS)
    {
      Dcm_Svc86Schd_09DtcRingBuffer.WriteIndex = 0u;
    }
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    ++Dcm_Svc86Schd_09DtcRingBuffer.NumElements;
    /*=================================*
     END CRITICAL SECTION
    *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
 /**********************************************************************************************************************
  *  Dcm_Svc86Schd_09RingBufferGetNumElements()
  *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_Svc86Schd_09RingBufferGetNumElements(
  void
  )
{
  return Dcm_Svc86Schd_09DtcRingBuffer.NumElements;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc86SchdGetSessionContext()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
FUNC(void, DCM_CODE) Dcm_Svc86SchdGetSessionContext(
  Dcm_ExtSvc86NvMDataPtrType pRepContextNvMData
  )
{
  *pRepContextNvMData = Dcm_Svc86CurrentSession;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_Svc86SchdSetSessionContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86SchdSetSessionContext(
  Dcm_ExtSvc86NvMDataConstPtrType pSessionContext
  )
{
  Dcm_Svc86CurrentSession.ClientSrcAddr = pSessionContext->ClientSrcAddr;
  Dcm_Svc86CurrentSession.RoEState      = pSessionContext->RoEState;
  Dcm_Svc86CurrentSession.RxPduId       = pSessionContext->RxPduId;
}

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03GetDidRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_Svc86OnDidChangeDidRecordPtrType, DCM_CODE) Dcm_Svc86Schd_03GetDidRecord(
  uint8_least index
  )
{
  Dcm_Svc86OnDidChangeDidRecordPtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= DCM_SVC_86_MAX_NUM_CHANGE_OF_DID_EVENTS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_Svc86CurrentSession.EventRecord.OnDidChange.DidRecord[0];
  }
  else
  {
    lResult = &Dcm_Svc86CurrentSession.EventRecord.OnDidChange.DidRecord[index];
  }

  return lResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07GetDidRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_Svc86OnCompOfValDidRecordPtrType, DCM_CODE) Dcm_Svc86Schd_07GetDidRecord(
  uint8_least index
  )
{
  Dcm_Svc86OnCompOfValDidRecordPtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= DCM_SVC_86_MAX_NUM_COMPARISON_OF_VALUE_EVENTS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_Svc86CurrentSession.EventRecord.OnCompOfVal.DidRecord[0];
  }
  else
  {
    lResult = &Dcm_Svc86CurrentSession.EventRecord.OnCompOfVal.DidRecord[index];
  }

  return lResult;
}
# endif

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdGetOnDtcChangeEventRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc86OnDtcChangeEventRecordPtrType, DCM_CODE) Dcm_Svc86SchdGetOnDtcChangeEventRecord(
  void
  )
{
  return &Dcm_Svc86CurrentSession.EventRecord.OnDtcChange;
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09GetDtcRecordInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc86DtcRecordInfoEventRecordPtrType, DCM_CODE) Dcm_Svc86Schd_09GetDtcRecordInfo(
  void
  )
{
  return &Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo;
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09TriggerOnDTCStatus()
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
FUNC(void, DCM_CODE) Dcm_Svc86Schd_09TriggerOnDTCStatus(
  uint32 DTC,
  uint8 DTCStatusOld,
  uint8 DTCStatusNew
  )
{
  uint8 NumOfDTCElements = 0;
  if ( (Dcm_Svc86CurrentSession.RoEState != DCM_SVC86_ROE_STATE_STOPPED) && (Dcm_Svc86SchdActivationState== TRUE) )
  {
    if ((Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcEventState == DCM_SVC86_EVENT_STATE_ACTIVE)
       && (((DTCStatusOld ^ DTCStatusNew) & Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.DTCStatusMask) != 0u))
    {
      Dcm_UtiEnterCriticalSection();
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
      if ((Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcEventState == DCM_SVC86_EVENT_STATE_ACTIVE)
         && (((DTCStatusOld ^ DTCStatusNew) & Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.DTCStatusMask) != 0u))
      {
        /*=================================*
         END CRITICAL SECTION
        *=================================*/
        Dcm_UtiLeaveCriticalSection();
        NumOfDTCElements = Dcm_Svc86Schd_09RingBufferGetNumElements();
        /* Max number of events allowed is used instead of the actual queue size, so that no events are triggered when no STRTs are allowed */
        if (NumOfDTCElements < DCM_SVC_86_MAX_NUM_DTC_STATUS_CHANGED_EVENTS)
        {
          Dcm_TskSetEvent(DCM_TSK_ID_ROE, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_DETECTED);
          Dcm_Svc86Schd_09RingBufferAddElement(DTC);
        }
        else
        {
          Dcm_TskSetEvent(DCM_TSK_ID_ROE, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_BUFFER_FULL);
        }
      }
      else
      {
        /*=================================*
         END CRITICAL SECTION
        *=================================*/
        Dcm_UtiLeaveCriticalSection();
      }
    }
  }
}
# endif

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_01TriggerOnDTCStatus()
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
FUNC(void, DCM_CODE) Dcm_Svc86Schd_01TriggerOnDTCStatus(
  uint8 DTCStatusOld,
  uint8 DTCStatusNew
  )
{
  if ( (Dcm_Svc86CurrentSession.RoEState != DCM_SVC86_ROE_STATE_STOPPED) && (Dcm_Svc86SchdActivationState == TRUE) )
  {
    if ((Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcEventState == DCM_SVC86_EVENT_STATE_ACTIVE)
       && (((DTCStatusOld ^ DTCStatusNew) & Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord.DTCStatusMask) != 0u))
    {
      Dcm_UtiEnterCriticalSection();
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
      if ((Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcEventState == DCM_SVC86_EVENT_STATE_ACTIVE)
         && (((DTCStatusOld ^ DTCStatusNew) & Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord.DTCStatusMask) != 0u))
      {
        /*=================================*
         END CRITICAL SECTION
        *=================================*/
        Dcm_UtiLeaveCriticalSection();
        Dcm_TskSetEvent(DCM_TSK_ID_ROE, DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_DETECTED);
      }
      else
      {
        /*=================================*
         END CRITICAL SECTION
        *=================================*/
        Dcm_UtiLeaveCriticalSection();
      }

    }
  }
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdGetOnDidChangeNumRecords()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_Svc86SchdGetOnDidChangeNumRecords(
  void
  )
{
  return Dcm_Svc86CurrentSession.EventRecord.OnDidChange.NumDidRecords;
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdIncOnDidChangeNumRecords()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86SchdIncOnDidChangeNumRecords(
  void
  )
{
  ++Dcm_Svc86CurrentSession.EventRecord.OnDidChange.NumDidRecords;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Dcm_Svc86SchdGetOnCompOfValNumRecords()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_Svc86SchdGetOnCompOfValNumRecords(
  void
  )
{
  return Dcm_Svc86CurrentSession.EventRecord.OnCompOfVal.NumDidRecords;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Dcm_Svc86SchdIncOnCompOfValNumRecords()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86SchdIncOnCompOfValNumRecords(
  void
  )
{
  ++Dcm_Svc86CurrentSession.EventRecord.OnCompOfVal.NumDidRecords;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc86SchdGetRoEState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc86RoeStateType, DCM_CODE) Dcm_Svc86SchdGetRoEState(
  void
  )
{
  return Dcm_Svc86CurrentSession.RoEState;
}

/**********************************************************************************************************************
 *  Dcm_Svc86SchdSetRoEState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86SchdSetRoEState(
  Dcm_Svc86RoeStateType RoeState
  )
{
  Dcm_Svc86CurrentSession.RoEState = RoeState;
  return;
}

/**********************************************************************************************************************
 *  Dcm_Svc86SchdOnChangeSession()
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
FUNC(void, DCM_CODE) Dcm_Svc86SchdOnChangeSession(
  Dcm_CfgStateGroupOptType oldSession,
  Dcm_CfgStateGroupOptType newSession
  )
{
  /* If it is NOT a transition from default to default session */
  if ((oldSession != DCM_STATE_VALUE_SESSION_INIT) || (newSession != DCM_STATE_VALUE_SESSION_INIT))
  {
    if (oldSession == DCM_STATE_VALUE_SESSION_INIT)
    {
      /* Default session is left; save context */
      Dcm_Svc86DefaultSession = Dcm_Svc86CurrentSession;
    }

    /* Stop RoE, clear task events and cancel any ongoing DID scheduling. Clear RoE setup. */
    Dcm_Svc86SchdClearAll();

    if (newSession == DCM_STATE_VALUE_SESSION_INIT)
    {
      /* Restore default session context always */
      Dcm_Svc86CurrentSession = Dcm_Svc86DefaultSession;

      if (Dcm_Svc86DefaultSession.RoEState != DCM_SVC86_ROE_STATE_STOPPED)
      {
        Dcm_Svc86SchdStartAll();
      }
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc86SchdStartAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc86SchdStartAll(
  void
  )
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
     (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86SchdEventRecordInit();
  Dcm_Svc86SchdInit();
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86Schd_09RingBufferInit();
# endif

  Dcm_TskSetEvent(DCM_TSK_ID_ROE, DCM_TSK_EV_ROE_SCHEDULER_RATE_ELAPSED);

  Dcm_TmrStartTimer(DCM_TMR_ID_SVC86_SCHEDULER, DCM_SVC_86_SCHEDULER_RATE);
}

# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Dcm_SetRoeActivationState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetRoeActivationState(
  boolean enabled
  )
{
  Std_ReturnType lResult = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  /*  Check that the module is initialized. */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  /* Check for valid parameter. */
  else if (Dcm_DebugDetectError((enabled != FALSE) && (enabled != TRUE)))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_Svc86SchdActivationState = enabled;

    lResult = DCM_E_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETROEACTIVATIONSTATE, lErrorId);

  return lResult;
}

# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc86Schd.c
 *********************************************************************************************************************/
