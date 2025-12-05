/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_Completion
 *  \{
 *  \file       Dem_Completion_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     Completion
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_COMPLETION_IMPLEMENTATION_H)
#define DEM_COMPLETION_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_Completion_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Pto_Interface.h"

#include "Dem_ReadinessIF_Interface.h"

                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_COMPLETION_IMPLEMENTATION_FILENAME "Dem_Completion_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/


#define DEM_START_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
 /*! Pending updates of readiness groups */
DEM_LOCAL volatile VAR(uint32, DEM_VAR_NO_INIT)     Dem_Completion_UpdateFlag;
#define DEM_STOP_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */



/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Completion_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete
 *****************************************************************************/
/*!
 * \brief         Tests if misfire monitoring shall always be reported as
 *                complete for compression ignition engines.
 *
 * \details       Tests if misfire monitoring shall always be reported as
 *                complete for compression ignition engines.
 *
 * \return        TRUE
 *                Misfire monitoring shall always be reported as complete
 *                for compression ignition engines.
 * \return        FALSE
 *                Misfire monitor completion is dynamically calculated for
 *                compression ignition engines.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_IsFuelSystemReportedAsComplete
 *****************************************************************************/
/*!
 * \brief         Tests if fuel system monitoring shall always be reported as
 *                complete.
 *
 * \details       Tests if fuel system monitoring shall always be reported as
 *                complete.
 *
 * \return        TRUE
 *                Fuel system monitoring shall always be reported as complete
 *                complete.
 * \return        FALSE
 *                Fuel system monitor completion is dynamically calculated.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_IsFuelSystemReportedAsComplete(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Set the update flag for Readiness Completion Calculation
 *
 * \details       Set the update flag for Readiness Completion Calculation,
 *                on the Dem MainFunction.
 *
 * \param[in]     UpdateFlag
 *                Value to be set
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(
  uint32 UpdateFlag
);

/* ****************************************************************************
 * Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Get the global update flags for readiness completion calculation.
 *
 * \details       Get the global update flags for readiness completion calculation.
 *                The flags indicate, if an update of a readiness group
 *                completion state is necessary during the main function.
 *
 * \return        Readiness completion update flags
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag(
  void
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete(
  void
)
{
  return DEM_CFG_PROCESS_COMPRESSION_MISFIRE_ALWAYS_COMPLETE == STD_ON;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_IsFuelSystemReportedAsComplete
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_IsFuelSystemReportedAsComplete(
  void
)
{
  return DEM_CFG_PROCESS_FUELSYSTEM_ALWAYS_COMPLETE == STD_ON;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(
  uint32 UpdateFlag
)
{
  uint32 lOldValue;
  uint32 lNewValue;

  do
  {
    lOldValue = Dem_Completion_UpdateFlag;
    lNewValue = UpdateFlag;
  } while (Dem_Com_SyncCompareAndSwap(&Dem_Completion_UpdateFlag, lOldValue, lNewValue) == FALSE);                               /* SBSW_DEM_CALL_SYNC_COMPARE_AND_SWAP */
}

/* ****************************************************************************
 % Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag(
  void
)
{
  return Dem_Completion_UpdateFlag;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Completion_PublicProperties
 * \{
 */


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Completion_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Reset the update flags for readiness completion calculation.
 *
 * \details       Reset the update flags for readiness completion calculation,
 *                and return the previous values.
 *
 * \return        Update flag values before the reset
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag(
  void
  );

/* ****************************************************************************
 * Dem_Readiness_Completion_SetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Marks the Readiness Completion of a given readiness group to
 *                be updated.
 *
 * \details       Marks the Readiness Completion of a given readiness group to
 *                be updated.
 *
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupUpdateFlag(
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Completion_GetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Tests if Readiness Completion of the given readiness group has
 *                to be updated
 *
 * \details       Tests if Readiness Completion of the given readiness group has
 *                to be updated
 *
 * \param[in]     State
 *                Current readiness state as 32 bit bitmap
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \return        TRUE
 *                Readiness Completion of given readiness group has to be updated
 * \return        FALSE
 *                Readiness Completion of given readiness group doesn't have to
 *                be updated
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(
  uint32 State,
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetReadinessGroupCompletedState
 *****************************************************************************/
/*!
 * \brief         Sets the stored completion value of the given readiness 
 *                group to the given value.
 *
 * \details       Sets the stored completion value of the given readiness 
 *                group to the given value.
 *
 * \param[in]     Completed
 *                TRUE    Readiness group is set to completed
 *                FALSE   Readiness group is set to not completed
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupCompletedState(
  boolean Completed,
  Dem_EventOBDReadinessGroupType ReadinessGroup);

/* ****************************************************************************
 * Dem_Readiness_Completion_CheckEventPreventsCompletion
 *****************************************************************************/
/*!
 * \brief         Check if event prevents its assigned readiness group from 
 *                being completed.
 *
 * \details       If the event prevents its assigned readiness group from
 *                being completed, the flag for updating the readiness completion
 *                is reset for this readiness group.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 * \param[in]     OldReadinessUpdateFlag
 *                Old update flag values of all readiness groups
 *
 * \return        The new update flag values of all readiness groups
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_CheckEventPreventsCompletion(
  Dem_EventIdType EventId,
  uint32 OldReadinessUpdateFlag
);

/* ****************************************************************************
 * Dem_Readiness_Completion_CheckReadinessRecalculationNeeded
 *****************************************************************************/
/*!
 * \brief         Check if recalcuation of event's readiness group is needed.
 *
 * \details       Check if recalcuation of event's readiness group is needed
 *                and reset update flag for this readiness group, if no 
 *                recalculation is required.
 *
 * \param[in]     ReadinessGroupId
 *                Unique identifier of the readiness group.
 * \param[in,out] ReadinessUpdateFlag
 *                Update flag values of all readiness groups
 *
 * \return        TRUE
 *                Recalcuation of event's readiness group is needed.
 * \return        FALSE
 *                No recalcuation of event's readiness group is needed.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_CheckReadinessRecalculationNeeded(
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) ReadinessUpdateFlag
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted
 *****************************************************************************/
/*!
 * \brief         Set all marked readiness groups to completed.
 *
 * \details       Set all readiness groups that are marked to be recalculated
 *                to completed.
 *
 * \param[in]     ReadinessUpdateFlag
 *                Update flag values of all readiness groups
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted(
  uint32 ReadinessUpdateFlag
);

/* ****************************************************************************
 * Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Removes the mark for updating the readiness completion of the
 *                given readiness group.
 *
 * \details       Removes the mark for updating the readiness completion of the
 *                given readiness group.
 *
 * \param[in]     State
 *                Current readiness state as 32 bit bitmap
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \return        New readiness state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(
  uint32 State,
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag(
  void
)
{
  uint32 lOldValue;
  uint32 lNewValue;

  do
  {
    lOldValue = Dem_Completion_UpdateFlag;
    lNewValue = 0u; 
  } while (Dem_Com_SyncCompareAndSwap(&Dem_Completion_UpdateFlag, lOldValue, lNewValue) == FALSE);                               /* SBSW_DEM_CALL_SYNC_COMPARE_AND_SWAP */

  /* Old value always contains the value before it was cleared */
  return lOldValue;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupUpdateFlag(
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    Dem_Com_SyncSetBitInBitfield(&Dem_Completion_UpdateFlag, DEM_READINESS_GROUP_WORDSIZE, ReadinessGroup);                      /* SBSW_DEM_POINTER_FORWARD_COMPLETIONUPDATEFLAG */
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_GetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(
  uint32 State,
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  boolean lReturnValue = FALSE;
  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    if ((State & (uint32)((uint32)1u << (ReadinessGroup))) != 0u)
    {
      lReturnValue = TRUE;
    }
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetReadinessGroupCompletedState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupCompletedState(
  boolean Completed,
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  Dem_EventOBDReadinessGroupType lReadinessGroup = ReadinessGroup;

  if (lReadinessGroup != DEM_CFG_READINESS_NONE)
  {
#if (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON)
    if (Completed == TRUE)
    {
      Dem_Com_SetBitInBitfield(&Dem_Cfg_StatusData.ReadinessGroupCompletedStates, DEM_READINESS_GROUP_WORDSIZE, ReadinessGroup); /* SBSW_DEM_POINTER_FORWARD_READINESSGROUPCOMPLETEDSTATES */
    }
    else
    {
      Dem_Com_ResetBitInBitfield(&Dem_Cfg_StatusData.ReadinessGroupCompletedStates, DEM_READINESS_GROUP_WORDSIZE, ReadinessGroup); /* SBSW_DEM_POINTER_FORWARD_READINESSGROUPCOMPLETEDSTATES */
    }
#endif
  }
  DEM_IGNORE_UNUSED_ARGUMENT(Completed)                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
}


/* ****************************************************************************
 % Dem_Readiness_Completion_CheckEventPreventsCompletion
 *****************************************************************************/
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_CheckEventPreventsCompletion(
  Dem_EventIdType EventId,
  uint32 OldReadinessUpdateFlag
)
{
  uint32 lNewReadinessUpdateFlag;

  lNewReadinessUpdateFlag = OldReadinessUpdateFlag;
  /* If event is available */
  if (Dem_Event_TestEventUnavailable(EventId) == FALSE)
  {
    /* If event is pending or event was not tested passed since last clear or event has PTO timer latched */
    if ( (Dem_Core_TestEventExtendedStatusBit(EventId, Dem_ExtendedStatus_TPSLC) == FALSE)
      || (Dem_Core_TestInternalEventUdsStatusBit(EventId, Dem_UdsStatus_PDTC) == TRUE)
      || (Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched(Dem_Core_Event_GetActivationTimerIndex(EventId)) == TRUE))
    {
      if (Dem_Core_TestEventExtendedStatusBit(EventId, Dem_ExtendedStatus_MonitorDisabledTOC) == FALSE)
      {
        /* Reset flag of this readiness group  */
        lNewReadinessUpdateFlag = Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(lNewReadinessUpdateFlag,
          Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId));
      }
    }
  }
  return lNewReadinessUpdateFlag;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Readiness_Completion_CheckReadinessRecalculationNeeded
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_CheckReadinessRecalculationNeeded(
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) ReadinessUpdateFlag
)
{
  uint32 lReadinessUpdateFlag = *ReadinessUpdateFlag;
  boolean lRecalculationNeeded;

  /* If update flag for event readiness group is not set no recalcuation of readiness group is needed */
  if (Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(lReadinessUpdateFlag, ReadinessGroupId) == FALSE)
  {
    lRecalculationNeeded = FALSE;
  }
  /* data context 'DID F501' is representant for data contexts PID01, DM05 and DID F501 as from these data contexts
     there is only a special handling for DID F501 in called function */
  else if (Dem_Readiness_Completion_TestGroupSupportsStaticCompletion(ReadinessGroupId, DEM_READINESS_DATACONTEXT_DIDF501) == TRUE)
  {
    /* Reset flag for this group, no recalcuation of readiness group is needed */
    lReadinessUpdateFlag = Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(lReadinessUpdateFlag, ReadinessGroupId);
    lRecalculationNeeded = FALSE;
  }
  /* If event readiness group is already completed  */
  else if (Dem_Readiness_Completion_TestGroupCalculatedCompleted(ReadinessGroupId) == TRUE)
  {
    /* Reset flag for this group, no recalcuation of readiness group is needed */
    lReadinessUpdateFlag = Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(lReadinessUpdateFlag, ReadinessGroupId);
    lRecalculationNeeded = FALSE;
  }
  else
  {
    lRecalculationNeeded = TRUE;
  }

  *ReadinessUpdateFlag = lReadinessUpdateFlag;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  return lRecalculationNeeded;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted(
  uint32 ReadinessUpdateFlag
)
{
  Dem_Cfg_ComplexIterType lReadinessGroupIter;
  uint32 lReadinessUpdateFlag = ReadinessUpdateFlag;

  for (Dem_GlobalDiagnostics_Readiness_GroupIterInit(&lReadinessGroupIter);                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
      (Dem_Cfg_ComplexIterExists(&lReadinessGroupIter) == TRUE) && (lReadinessUpdateFlag != 0u);                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */ /* PRQA S 2994 */ /* MD_DEM_2994 */
       Dem_Cfg_ComplexIterNext(&lReadinessGroupIter))                                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventOBDReadinessGroupType lReadinessGroupId = Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId(&lReadinessGroupIter); /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if (Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(lReadinessUpdateFlag, lReadinessGroupId) == TRUE)
    {
      Dem_Readiness_Completion_SetReadinessGroupCompletedState(TRUE, lReadinessGroupId);                                         /* SBSW_DEM_POINTER_FORWARD_GLOBAL */

      /* Reset flag of this readiness group */
      lReadinessUpdateFlag = Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(lReadinessUpdateFlag, lReadinessGroupId);
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(
  uint32 State,
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  uint32 lReturnValue;
  lReturnValue = State;
  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    lReturnValue &= ~((uint32)((uint32)1U << (uint32)ReadinessGroup));
  }
  return lReturnValue;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Completion_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Readiness_Completion_TestGroupSupportsStaticCompletion
 *****************************************************************************/
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
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestGroupSupportsStaticCompletion(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8  DataContext
)
{
  boolean lReturnValue;
  boolean lSupportIgnitionSpark = (DEM_CFG_SUPPORT_IGNITION_SPARK == STD_ON);
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_FLSYS:
    lReturnValue = TRUE;
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    if ((Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501() == TRUE)
      && (DataContext == DEM_READINESS_DATACONTEXT_DIDF501))
    {
      lReturnValue = TRUE;
    }
    else if ( (Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInRdyThisDcy() == TRUE)
           && ( (DataContext == DEM_READINESS_DATACONTEXT_PID41) || (DataContext == DEM_READINESS_DATACONTEXT_DM26) ) )
    {
      lReturnValue = TRUE;
    }
    else if ((DataContext == DEM_READINESS_DATACONTEXT_PID01) || (DataContext == DEM_READINESS_DATACONTEXT_DM05))
    {
      /* Always complete, see DSGN-Dem-SupportOfPID01 and DSGN-Dem-ReadReadinessJ1939 */
      lReturnValue = TRUE;
    }
    else
    {
      lReturnValue = FALSE;
    }
    break;
  case DEM_CFG_READINESS_MISF:
    if (DataContext != DEM_READINESS_DATACONTEXT_DIDF501)
    {
      lReturnValue = ((lSupportIgnitionSpark == TRUE) || (Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete() == TRUE));
    }
    else
    {
      lReturnValue = FALSE;
    }
    break;
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    if (Dem_Readiness_Completion_IsFuelSystemReportedAsComplete() == TRUE)
    {
      lReturnValue = TRUE;
    }
    else
    {
      lReturnValue = FALSE;
    }
    break;
  default:
    lReturnValue = FALSE;
    break;
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */


/* ****************************************************************************
 % Dem_Readiness_Completion_ProcessUpdateFlag
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_ProcessUpdateFlag(
  void
)
{
  if ( (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE) && (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE) ) /* COV_DEM_ROBUSTNESS TX */
  {
    uint32 lReadinessUpdateFlag;
    
    /* Get and reset all update flags for every readiness group to recalculate monitor completion state*/
    lReadinessUpdateFlag = Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag();
    
    /* If a flag of a readiness group is set  */
    if (lReadinessUpdateFlag != 0u)
    {
      Dem_Cfg_ComplexIterType lReadinessGroupIter;
      for (Dem_GlobalDiagnostics_Readiness_GroupIterInit(&lReadinessGroupIter);                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIterExists(&lReadinessGroupIter) == TRUE;                                                                 /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIterNext(&lReadinessGroupIter))                                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_EventOBDReadinessGroupType lReadinessGroupId = Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId(&lReadinessGroupIter); /* SBSW_DEM_CALL_ITERATOR_POINTER */

        if (Dem_Readiness_Completion_CheckReadinessRecalculationNeeded(lReadinessGroupId, &lReadinessUpdateFlag) == TRUE)        /* SBSW_DEM_POINTER_FORWARD_STACK */
        {
          Dem_Cfg_ComplexIterType lEventsOfReadinessGroupIter;
          for (Dem_GlobalDiagnostics_Readiness_EventIterInit(lReadinessGroupId, &lEventsOfReadinessGroupIter);                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
            Dem_Cfg_ComplexIterExists(&lEventsOfReadinessGroupIter) == TRUE;                                                     /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
            Dem_Cfg_ComplexIterNext(&lEventsOfReadinessGroupIter))                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
          {
            Dem_EventIdType lEventId = Dem_GlobalDiagnostics_Readiness_EventIterGetEventId(&lEventsOfReadinessGroupIter);        /* SBSW_DEM_CALL_ITERATOR_POINTER */

            /* If all flags are reset stop iterating events */
            if (lReadinessUpdateFlag == 0u)
            {
              break;
            }
            lReadinessUpdateFlag = Dem_Readiness_Completion_CheckEventPreventsCompletion(lEventId, lReadinessUpdateFlag);
          }
        }
      }
      
      Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted(lReadinessUpdateFlag);
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/* ****************************************************************************
 % Dem_Readiness_Completion_ResetReadinessGroupCompletedStates
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_ResetReadinessGroupCompletedStates(
  void
)
{
#if (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON)
  Dem_Cfg_StatusData.ReadinessGroupCompletedStates = 0u;
#endif
}

/* ****************************************************************************
 % Dem_Readiness_Completion_UpdateCompletionStatesAfterClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_UpdateCompletionStatesAfterClear(
  void
)
{
  if (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
  {
    /* all readiness groups are set to not complete */
    Dem_Readiness_Completion_ResetReadinessGroupCompletedStates();

    /* set flag for all readiness group supported by engine type to recalculate monitor state -> not supported groups will be
       set to N/A = completed*/
       /* although event states are reset on clear recalculation is necessary as availability of events (independent from clear
          request) has to be considered for completion states */
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_COMPRESSION);
    }
    else
    {
      Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_SPARK);
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_UpdateCompletionStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_UpdateCompletionStatus(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  boolean Complete
)
{
  if ( (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
    && (Dem_Readiness_Completion_TestGroupSupportsStaticCompletion(ReadinessGroup, DEM_READINESS_DATACONTEXT_DIDF501) == FALSE))
  {
    Dem_Readiness_Completion_SetReadinessGroupCompletedState(Complete, ReadinessGroup);
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_TriggerCompletionRecalculation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_TriggerCompletionRecalculation(
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  if (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
  {
    if (Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag(), ReadinessGroup) == FALSE)
    {
      Dem_Readiness_Completion_SetReadinessGroupUpdateFlag(ReadinessGroup);
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_TriggerCompletionRecalculationAll
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_TriggerCompletionRecalculationAll(
  void
)
{
  if (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
  {
    /* set flag for all readiness group supported by engine type to recalculate monitor state -> not supported groups will be set to N/A = completed*/
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_COMPRESSION);
    }
    else
    {
      Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_SPARK);
    }
    Dem_Scheduler_EnableTask(Dem_Scheduler_Task_ReadinessCompletion);
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_TestGroupCalculatedCompleted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestGroupCalculatedCompleted(
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  boolean lReturnValue = FALSE;

  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
#if (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON)
    lReturnValue = Dem_Com_TestBitInBitfield(&Dem_Cfg_StatusData.ReadinessGroupCompletedStates,                                  /* SBSW_DEM_POINTER_FORWARD_READINESSGROUPCOMPLETEDSTATES */
      DEM_READINESS_GROUP_WORDSIZE, ReadinessGroup);
#else
    DEM_IGNORE_UNUSED_ARGUMENT(ReadinessGroup)                                                                                   /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_COMPLETION_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Completion_Implementation.h
 *********************************************************************************************************************/
