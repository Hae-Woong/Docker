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
/**        \file  Dcm_Svc86Schd.h
 *         \unit  Svc86Schd
 *        \brief  Contains public types, function declarations and inline function definitions of Service 0x86 Scheduler unit.
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
#if !defined(DCM_SVC86SCHD_H)
# define DCM_SVC86SCHD_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Svc86SchdTypes.h"
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                          /* COV_MSR_UT_OPTIONAL_UNIT */
#  include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! DID reference states */
#  define DCM_SVC86_DID_REFERENCE_STATE_INVALID                      ((Dcm_Svc86DidReferenceStateType)0x00u)
#  define DCM_SVC86_DID_REFERENCE_STATE_VALID                        ((Dcm_Svc86DidReferenceStateType)0x01u)

/*! States of the RoE service handler */
#  define DCM_SVC86_ROE_STATE_STOPPED                                ((Dcm_Svc86RoeStateType)0x00)
#  define DCM_SVC86_ROE_STATE_STARTED                                ((Dcm_Svc86RoeStateType)0x01)
#  define DCM_SVC86_ROE_STATE_STARTED_PERSISTENTLY                   ((Dcm_Svc86RoeStateType)0x02)

/*! Event setup states */
#  define DCM_SVC86_EVENT_STATE_CLEARED                              ((Dcm_Svc86EventStateType)0x00u)
#  define DCM_SVC86_EVENT_STATE_ACTIVE                               ((Dcm_Svc86EventStateType)0x01u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#  if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON) || \
      (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdInit()
 *********************************************************************************************************************/
/*! \brief          Initializes the scheduler.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdInit(
  void
  );
#  endif

#  if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferAddElement()
 *********************************************************************************************************************/
/*! \brief          Add the DTC to the ring buffer so that it can be used to trigger STRT at scheduler call later.
 *  \details        -
 *  \context        ISR1|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86Schd_09RingBufferAddElement(
  uint32 DTC
  );
#  endif

#  if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferGetNumElements()
 *********************************************************************************************************************/
/*! \brief          Return the total number of DTC stored.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_Svc86Schd_09RingBufferGetNumElements(
  void
  );
#  endif

#  if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_03GetDidRecord()
 *********************************************************************************************************************/
/*! \brief          Returns a pointer to a Did record of the given index.
 *  \details        -
 *  \param[in]      index    Index to a Did record
 *  \return         The Did record
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc86OnDidChangeDidRecordPtrType, DCM_CODE) Dcm_Svc86Schd_03GetDidRecord(
  uint8_least index
  );
#  endif

#  if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_07GetDidRecord()
 *********************************************************************************************************************/
/*! \brief          Returns a pointer to a Did record of the given index.
 *  \details        -
 *  \param[in]      index    Index to a Did record
 *  \return         The Did record
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc86OnCompOfValDidRecordPtrType, DCM_CODE) Dcm_Svc86Schd_07GetDidRecord(
  uint8_least index
  );
#  endif

#  if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86SchdGetOnDtcChangeEventRecord()
 *********************************************************************************************************************/
/*! \brief          Returns a pointer to DTC record for sub-function 0x01.
 *  \details        -
 *  \return         The DTC record on DTC change for sub-function 0x01
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc86OnDtcChangeEventRecordPtrType, DCM_CODE) Dcm_Svc86SchdGetOnDtcChangeEventRecord(
  void
  );
#  endif

#  if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09GetDtcRecordInfo()
 *********************************************************************************************************************/
/*! \brief          Returns a pointer to DTC record info for sub-function 0x09.
 *  \details        -
 *  \return         The DTC record info for sub function 0x09
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc86DtcRecordInfoEventRecordPtrType, DCM_CODE) Dcm_Svc86Schd_09GetDtcRecordInfo(
  void
  );
#  endif

#  if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09RingBufferInit()
 *********************************************************************************************************************/
/*! \brief          Reset the state machine in the Dtc Ring buffer.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86Schd_09RingBufferInit(
  void
  );
#  endif

#  if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_09TriggerOnDTCStatus()
 *********************************************************************************************************************/
/*! \brief          Triggers a ROE event for sub-service DtcRecordInfo if the requested DTCStatusMask is matched.
 *  \details        -
 *  \param[in]      DTC               This is the DTC the change is assigned to
 *  \param[in]      DTCStatusOld      DTC status before change
 *  \param[in]      DTCStatusNew      DTC status after change
 *  \context        ISR1|ISR2
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86Schd_09TriggerOnDTCStatus(
  uint32 DTC,
  uint8 DTCStatusOld,
  uint8 DTCStatusNew
  );
#  endif

#  if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86Schd_01TriggerOnDTCStatus()
 *********************************************************************************************************************/
/*! \brief          Triggers a ROE event for sub-service OnDTCStatusChanged if the requested DTCStatusMask is matched.
 *  \details        -
 *  \param[in]      DTCStatusOld      DTC status before change
 *  \param[in]      DTCStatusNew      DTC status after change
 *  \context        ISR1|ISR2
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86Schd_01TriggerOnDTCStatus(
  uint8 DTCStatusOld,
  uint8 DTCStatusNew
  );
#  endif

/**********************************************************************************************************************
*  Dcm_Svc86SchdOnChangeSession()
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
FUNC(void, DCM_CODE) Dcm_Svc86SchdOnChangeSession(
  Dcm_CfgStateGroupOptType oldSession,
  Dcm_CfgStateGroupOptType newSession
  );

#  if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Dcm_Svc86SchdGetOnDidChangeNumRecords()
*********************************************************************************************************************/
/*! \brief          Get the current session number of DID events records for sub-function 03.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_Svc86SchdGetOnDidChangeNumRecords(
  void
  );
#  endif

#  if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
*  Dcm_Svc86SchdIncOnDidChangeNumRecords()
*********************************************************************************************************************/
/*! \brief          Increment the current session number of DID events records for sub-function 03.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdIncOnDidChangeNumRecords(
  void
  );
#  endif

/**********************************************************************************************************************
*  Dcm_Svc86SchdGetOnCompOfValNumRecords()
*********************************************************************************************************************/
/*! \brief          Get the current session number of DID events records for sub-function 07.
 *  \details        -
 *  \return         Return the number of DID event records for sub-function 0x07
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_Svc86SchdGetOnCompOfValNumRecords(
  void
  );

/**********************************************************************************************************************
*  Dcm_Svc86SchdIncOnCompOfValNumRecords()
*********************************************************************************************************************/
/*! \brief          Increase the current session number of DID events records for sub-function 07.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdIncOnCompOfValNumRecords(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc86SchdGetRoEState()
 *********************************************************************************************************************/
/*! \brief          Get the current session response on event state
 *  \details        -
 *  \return         Return the new RoE state
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_Svc86RoeStateType, DCM_CODE) Dcm_Svc86SchdGetRoEState(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc86SchdGetSessionContext()
 *********************************************************************************************************************/
/*! \brief          copy the current session data.
 *  \details        -
 *  \param[in]      pRepContext    Current repeater proxy context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdGetSessionContext(
  Dcm_ExtSvc86NvMDataPtrType pRepContextNvMData
  );

/**********************************************************************************************************************
 *  Dcm_Svc86SchdSetSessionContext()
 *********************************************************************************************************************/
/*! \brief          Set Svc 0x86 current session received request data.
 *  \details        -
 *  \param[in]      pSessionContext    Pointer to Svc0x86 repeater context session data.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdSetSessionContext(
  Dcm_ExtSvc86NvMDataConstPtrType pSessionContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc86SchdSetRoEState()
 *********************************************************************************************************************/
 /*! \brief          Set the current session RX PDU ID.
  *  \details        -
  *  \param[in]      RoeState    The new RoE state
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdSetRoEState(
  Dcm_Svc86RoeStateType RoeState
  );

/**********************************************************************************************************************
 *  Dcm_Svc86SchdStopAll()
 *********************************************************************************************************************/
/*! \brief          Stops all activities regarding event processing and scheduling.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdStopAll(
  void
  );

/**********************************************************************************************************************
*  Dcm_Svc86SchdStartAll()
*********************************************************************************************************************/
/*! \brief          Starts the scheduling.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdStartAll(
  void
  );

/**********************************************************************************************************************
 *  Dcm_Svc86SchdClearAll()
 *********************************************************************************************************************/
/*! \brief          Clears all setup events.
 *  \details        Stops all activities regarding event processing and scheduling before clearing.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc86SchdClearAll(
  void
  );

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
#endif /* !defined(DCM_SVC86SCHD_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc86Schd.h
 *********************************************************************************************************************/
