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
/*!
 *  \defgroup   Dem_EventEntry
 *  \{
 *  \file       Dem_EventEntry_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Interface of the Event Entry unit which handles access to and from event entries.
 *  \entity     EventEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EVENTENTRY_INTERFACE_H)
#define DEM_EVENTENTRY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_EventEntry_Types.h"
#include "Dem_Core_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_EventEntry_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetEventId
 *****************************************************************************/
/*!
 * \brief         Get the Event Id of the event entry
 *
 * \details       Get the Event Id of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The Event Id of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetEventId(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetEventId
 *****************************************************************************/
/*!
 * \brief         Set the event Id in the event entry
 *
 * \details       Set the event Id in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     EventId
 *                The event Id to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetEventId(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_Free
 *****************************************************************************/
/*!
 * \brief         Free event entry present at the given index
 *
 * \details       Free event entry present at the given index
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_Free(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetTimestamp
 *****************************************************************************/
/*!
 * \brief         Get the timestamp of the event entry
 *
 * \details       Get the timestamp of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The timestamp of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetTimestamp(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetTimestamp
 *****************************************************************************/
/*!
 * \brief         Set the timestamp in the event entry
 *
 * \details       Set the timestamp in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     Timestamp
 *                The timestamp to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetTimestamp(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint32, AUTOMATIC) Timestamp
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetMaxFdcSlc
 *****************************************************************************/
/*!
 * \brief         Get the maximum fault delection counter since last clear
 *                from the event entry.
 *
 * \details       -
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The maximum fault detection counter since last clear
 *
 * \pre           Event is stored.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetMaxFdcSlc(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetMaxFdcSlc
 *****************************************************************************/
/*!
 * \brief         Sets the maximum fault detection counter since last clear
 *                in the event entry.
 *
 * \details       -
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     FdcValue
 *                The fault detection counter value to set the Max FDC SLC to.
 *
 * \pre           Event is stored.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetMaxFdcSlc(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  sint8 FdcValue
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetState
 *****************************************************************************/
/*!
 * \brief         Get the state of the event entry
 *
 * \details       Get the state of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The state of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetState(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetState
 *****************************************************************************/
/*!
 * \brief         Set the state in the event entry
 *
 * \details       Set the state in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     State
 *                The state to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetState(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) State
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetAgingTargetCycle
 *****************************************************************************/
/*!
 * \brief         Get the aging target cycle of the event entry
 *
 * \details       Get the aging target cycle of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The aging target cycle of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetAgingTargetCycle(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetAgingTargetCycle
 *****************************************************************************/
/*!
 * \brief         Set the aging target cycle in the event entry
 *
 * \details       Set the aging target cycle in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     TargetCycle
 *                The aging target cycle to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetAgingTargetCycle(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint16 TargetCycle
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetAgingTimer
 *****************************************************************************/
/*!
 * \brief         Get the aging timer of the event entry
 *
 * \details       Get the aging timer of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The aging timer of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetAgingTimer(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetAgingTimer
 *****************************************************************************/
/*!
 * \brief         Set the aging timer in the event entry
 *
 * \details       Set the aging timer in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     AgingTimer
 *                The aging timer to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetAgingTimer(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint32, AUTOMATIC) AgingTimer
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetFailedCycleCounter
 *****************************************************************************/
/*!
 * \brief         Get the 'Failed Cycle Counter' of the event entry
 *
 * \details       Get the 'Failed Cycle Counter' of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The 'Failed Cycle Counter' of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetFailedCycleCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );


/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetConsecutiveFailedCycleCounter
 *****************************************************************************/
/*!
 * \brief         Get the 'Consecutive Failed Cycle Counter' of the memory 
 *                entry
 *
 * \details       Get the 'Consecutive Failed Cycle Counter' of the memory 
 *                entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The 'Consecutive Failed Cycle Counter' of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetConsecutiveFailedCycleCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetConsecutiveFailedCycleCounter
 *****************************************************************************/
/*!
 * \brief         Set the 'Consecutive Failed Cycle Counter' in the memory 
 *                entry
 *
 * \details       Set the 'Consecutive Failed Cycle Counter' in the memory 
 *                entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     ConsecutiveFailedCycleCounter
 *                The 'Consecutive Failed Cycle Counter' to be set in the 
 *                event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetConsecutiveFailedCycleCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) ConsecutiveFailedCycleCounter
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetCyclesSinceFirstFailed
 *****************************************************************************/
/*!
 * \brief         Get the 'Cycles Since First Failed' of the event entry
 *
 * \details       Get the 'Cycles Since First Failed' of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The 'Cycles Since First Failed' of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetCyclesSinceFirstFailed(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetCyclesSinceFirstFailed
 *****************************************************************************/
/*!
 * \brief         Set the 'Cycles Since First Failed' in the event entry
 *
 * \details       Set the 'Cycles Since First Failed' in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     CyclesSinceFirstFailed
 *                The 'Cycles Since First Failed' to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetCyclesSinceFirstFailed(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint16, AUTOMATIC) CyclesSinceFirstFailed
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetTestedCyclesSinceFirstFailed
 *****************************************************************************/
/*!
 * \brief         Get the 'Tested Cycles Since First Failed' of the memory 
 *                entry
 *
 * \details       Get the 'Tested Cycles Since First Failed' of the memory 
 *                entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The 'Tested Cycles Since First Failed' of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetTestedCyclesSinceFirstFailed(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetTestedCyclesSinceFirstFailed
 *****************************************************************************/
/*!
 * \brief         Set the 'Tested Cycles Since First Failed' in the memory 
 *                entry
 *
 * \details       Set the 'Tested Cycles Since First Failed' in the memory 
 *                entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     TestedCyclesSinceFirstFailed
 *                The 'Tested Cycles Since First Failed' to be set in the 
 *                event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetTestedCyclesSinceFirstFailed(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) TestedCyclesSinceFirstFailed
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetCyclesSinceLastFailed
 *****************************************************************************/
/*!
 * \brief         Get the 'Cycles Since Last Failed' of the event entry
 *
 * \details       Get the 'Cycles Since Last Failed' of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The 'Cycles Since Last Failed' of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetCyclesSinceLastFailed(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetCyclesSinceLastFailed
 *****************************************************************************/
/*!
 * \brief         Set the 'Cycles Since Last Failed' in the event entry
 *
 * \details       Set the 'Cycles Since Last Failed' in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     CyclesSinceLastFailed
 *                The 'Cycles Since Last Failed' to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetCyclesSinceLastFailed(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint16, AUTOMATIC) CyclesSinceLastFailed
  );

/* ****************************************************************************
* Dem_MemoryEntry_EventEntry_GetWarmUpCyclesSinceLastFailed
*****************************************************************************/
/*!
* \brief         Get the 'WarmUp Cycles Since Last Failed' of the event entry
*
* \details       Get the 'WarmUp Cycles Since Last Failed' of the event entry
*
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
*
* \return        The 'WarmUp Cycles Since Last Failed' of the event entry
*
* \pre           -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetWarmUpCyclesSinceLastFailed(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
* Dem_MemoryEntry_EventEntry_SetWarmUpCyclesSinceLastFailed
*****************************************************************************/
/*!
* \brief          Set the 'WarmUp Cycles Since Last Failed' in the event entry
*
* \details        Set the 'WarmUp Cycles Since Last Failed' in the event entry
*
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
* \param[in]      WarmUpCyclesSinceLastFailed
*                 The 'WarmUp Cycles Since Last Failed' to be set in the memory
*                 entry
*
* \pre            -
*
* \context        TASK|ISR2
* \synchronous    TRUE
* \reentrant      TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetWarmUpCyclesSinceLastFailed(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint16, AUTOMATIC) WarmUpCyclesSinceLastFailed
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus
 *****************************************************************************/
/*!
 * \brief         Get the preconfirmed status of the event entry
 *
 * \details       Get the preconfirmed status of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The preconfirmed status of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus
 *****************************************************************************/
/*!
 * \brief         Set the preconfirmed status in the event entry
 *
 * \details       Set the preconfirmed status in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     StatusIndicator
 *                The preconfirmed status to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) StatusIndicator
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetOccurrenceCounter1
 *****************************************************************************/
/*!
 * \brief         Get the occurrence counter #1 of the event entry
 *
 * \details       Get the occurrence counter #1 of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The occurrence counter #1 of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter1(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetOccurrenceCounter1
 *****************************************************************************/
/*!
 * \brief         Set the occurrence counter #1 in the event entry
 *
 * \details       Set the occurrence counter #1 in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     OccurrenceCounter1
 *                The occurrence counter #1 to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter1(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter1
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetOccurrenceCounter2
 *****************************************************************************/
/*!
 * \brief         Get the occurrence counter #2 of the event entry
 *
 * \details       Get the occurrence counter #2 of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The occurrence counter #2 of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter2(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetOccurrenceCounter2
 *****************************************************************************/
/*!
 * \brief         Set the occurrence counter #2 in the event entry
 *
 * \details       Set the occurrence counter #2 in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     OccurrenceCounter2
 *                The occurrence counter #2 to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter2(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter2
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetOccurrenceCounter3
 *****************************************************************************/
/*!
 * \brief         Get the occurrence counter #3 of the event entry
 *
 * \details       Get the occurrence counter #3 of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The occurrence counter #3 of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter3(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetOccurrenceCounter3
 *****************************************************************************/
/*!
 * \brief         Set the occurrence counter #3 in the event entry
 *
 * \details       Set the occurrence counter #3 in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     OccurrenceCounter3
 *                The occurrence counter #3 to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter3(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter3
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetOccurrenceCounter4
 *****************************************************************************/
/*!
 * \brief         Get the occurrence counter #4 of the event entry
 *
 * \details       Get the occurrence counter #4 of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The occurrence counter #4 of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter4(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetOccurrenceCounter4
 *****************************************************************************/
/*!
 * \brief         Set the occurrence counter #4 in the event entry
 *
 * \details       Set the occurrence counter #4 in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     OccurrenceCounter4
 *                The occurrence counter #4 to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter4(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter4
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetOccurrenceCounter7
 *****************************************************************************/
/*!
 * \brief         Get the occurrence counter #7 of the event entry
 *
 * \details       Get the occurrence counter #7 of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The occurrence counter #7 of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter7(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetOccurrenceCounter7
 *****************************************************************************/
/*!
 * \brief         Set the occurrence counter #7 in the event entry
 *
 * \details       Set the occurrence counter #7 in the event entry
 * 
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     OccurrenceCounter7
 *                The occurrence counter #7 to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter7(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter7
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetStdEnvData
 *****************************************************************************/
/*!
 * \brief         Get the requested data element of the standard environmental 
 *                data of the event entry
 *
 * \details       Get the requested data element of the standard environmental 
 *                data of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     DataIndex
 *                The index of the standard environmental data
 *
 * \return        The requested data element of the standard environmental 
 *                data of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetStdEnvData(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  DataIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetStdEnvData
 *****************************************************************************/
/*!
 * \brief         Set the requested data element of the standard environmental 
 *                data in the event entry
 *
 * \details       Set the requested data element of the standard environmental 
 *                data in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     DataIndex
 *                The index of the standard environmental data element
 * \param[in]     DataValue
 *                The data value to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetStdEnvData(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  DataIndex,
  CONST(uint8, AUTOMATIC)  DataValue
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetSnapshotHeader
 *****************************************************************************/
/*!
 * \brief         Get the snapshot header of the event entry
 *
 * \details       Get the snapshot header of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The snapshot header of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetSnapshotHeader(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetSnapshotHeader
 *****************************************************************************/
 /*!
 * \brief         Set the snapshot header in the event entry
 *
 * \details       Set the snapshot header in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     SnapshotHeader
 *                The snapshot header to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetSnapshotHeader(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) SnapshotHeader
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo
 *****************************************************************************/
/*!
 * \brief         Get the Fifo snapshot header of the event entry
 *
 * \details       Get the Fifo snapshot header of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The Fifo snapshot header of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo(
  Dem_Cfg_EntryIndexType EventEntryIndex
);

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetSnapshotHeaderFifo
 *****************************************************************************/
/*!
 * \brief         Set the Fifo snapshot header in the event entry
 *
 * \details       Set the Fifo snapshot header in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     SnapshotHeaderFifo
 *                The Fifo snapshot header to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetSnapshotHeaderFifo(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint8  SnapshotHeaderFifo
);

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetCombinedDTCSnapshotHeader
 *****************************************************************************/
/*!
 * \brief         Get the combined DTC snapshot header of the event entry
 *
 * \details       Get the combined DTC snapshot header of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The combined DTC snapshot header of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetCombinedDTCSnapshotHeader(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetCombinedDTCSnapshotHeader
 *****************************************************************************/
 /*!
 * \brief         Set the combined DTC snapshot header in the event entry
 *
 * \details       Set the combined DTC snapshot header in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     CombinedDTCSnapshotHeader
 *                The combined DTC snapshot header to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetCombinedDTCSnapshotHeader(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint32, AUTOMATIC) CombinedDTCSnapshotHeader
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetCombinedDTCExtendedHeader
 *****************************************************************************/
/*!
 * \brief         Get the combined DTC extended header of the event entry
 *
 * \details       Get the combined DTC extended header of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The combined DTC extended header of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetCombinedDTCExtendedHeader(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetCombinedDTCExtendedHeader
 *****************************************************************************/
 /*!
 * \brief         Set the combined DTC extended header in the event entry
 *
 * \details       Set the combined DTC extended header in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     CombinedDTCExtendedHeader
 *                The combined DTC extended header to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetCombinedDTCExtendedHeader(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint32, AUTOMATIC) CombinedDTCExtendedHeader
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetExtendedHeader
 *****************************************************************************/
/*!
 * \brief         Get the extended header of the event entry
 *
 * \details       Get the extended header of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The extended header of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetExtendedHeader(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetExtendedHeader
 *****************************************************************************/
/*!
 * \brief         Set the extended header in the event entry
 *
 * \details       Set the extended header in the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     ExtendedHeader
 *                The extended header to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetExtendedHeader(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) ExtendedHeader
  );

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the indexed snapshot data buffer of the event entry
 *
 * \details       Get the indexed snapshot data buffer of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     SRecIndex
 *                Index of the snapshot data record
 *
 * \return        Pointer to the indexed snapshot data buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataPtr(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  SRecIndex
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataConstPtr
 *****************************************************************************/
 /*!
 * \brief         Get the indexed snapshot data buffer of the event entry
 *
 * \details       Get the indexed snapshot data buffer of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     SRecIndex
 *                Index of the snapshot data record
 *
 * \return        Pointer to the unmodifiable (const) indexed snapshot data
 *                buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataConstPtr(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(Dem_Cfg_SRecIndexType, AUTOMATIC)  SRecIndex
  );
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetExtendedRecordDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the indexed extended data buffer of the event entry
 *
 * \details       Get the indexed extended data buffer of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     ERecIndex
 *                Index of the extended data record
 *
 * \return        Pointer to the indexed extended data buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetExtendedRecordDataPtr(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  ERecIndex
  );
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetExtendedRecordDataConstPtr
 *****************************************************************************/
/*!
 * \brief         Get the indexed extended data buffer of the event entry
 *
 * \details       Get the indexed extended data buffer of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     ERecIndex
 *                Index of the extended data record
 *
 * \return        Pointer to the unmodifiable (const) indexed extended data
 *                buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetExtendedRecordDataConstPtr(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  ERecIndex
  );
#endif

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetWwhObdFreezeFrameDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the WWH-OBD freeze frame data buffer of the event entry
 *
 * \details       Get the WWH-OBD freeze frame data buffer of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        Pointer to the WWH-OBD freeze frame data buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_WWHOBD == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetWwhObdFreezeFrameDataPtr(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

#if (DEM_CFG_SUPPORT_J1939_FREEZEFRAME == STD_ON) || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetJ1939FreezeFramePtr
 *****************************************************************************/
/*!
 * \brief         Get the J1939 freeze frame data buffer of the event entry
 *
 * \details       Get the J1939 freeze frame data buffer of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        Pointer to the J1939 freeze frame data buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_J1939_FREEZEFRAME == STD_ON ||
 *                DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetJ1939FreezeFramePtr(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetGlobalSnapshotRecordDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the global snapshot record data buffer of the event entry
 *
 * \details       Get the global snapshot record data buffer of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        Pointer to the global snapshot record data buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetGlobalSnapshotRecordDataPtr(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_TestGlobalSnapshotRecordStored
 *****************************************************************************/
/*!
 * \brief         Checks whether the global snapshot record of the event in 
 *                memory is stored.
 *
 * \details       Checks whether the global snapshot record of the event in 
 *                memory is stored.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                Global snapshot record is stored.
 * \return        FALSE
 *                Otherwise
 *                
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestGlobalSnapshotRecordStored(                                                                                  
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetGlobalSnapshotRecordStored
 *****************************************************************************/
/*!
 * \brief         Sets the global snapshot record stored state of an event in 
 *                memory.
 *
 * \details       Sets the global snapshot record stored state of an event in 
 *                memory.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     StoredState
 *                The global snapshot record stored state.                
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetGlobalSnapshotRecordStored(                                                                                   
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(boolean, AUTOMATIC) StoredState
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_TestJ1939FFStored
 *****************************************************************************/
/*!
 * \brief         Checks whether the J1939(expanded) Freeze Frame of the event 
 *                is stored in memory.
 *
 * \details       Checks whether the J1939(expanded) Freeze Frame of the event 
 *                is stored in memory.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                J1939(expanded) Freeze Frame is stored.
 * \return        FALSE
 *                Otherwise
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestJ1939FFStored(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetJ1939FFStoredStatus
 *****************************************************************************/
/*!
 * \brief         Sets the stored status of J1939(expanded) Freeze Frame of an 
 *                event in memory.
 *
 * \details       Sets the stored status of J1939(expanded) Freeze Frame of an 
 *                event in memory.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     StoredState
 *                The stored state of J1939(expanded) Freeze Frame.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetJ1939FFStoredStatus(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  boolean StoredState
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_EventEntry_Public Public Methods
 * \{
 */


#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_TestGlobalSnapshotStored
 *****************************************************************************/
/*!
 * \brief         Test if global snapshot is stored in event entry
 *
 * \details       Test if global snapshot is stored in event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE, global snapshot stored
 *                FALSE, global snapshot not stored
 *
 * \pre           -
   \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestGlobalSnapshotStored(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetOccurrenceCounter
 *****************************************************************************/
/*!
 * \brief         Get the occurrence counter of the event entry
 *
 * \details       Get the occurrence counter of the event entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The occurrence counter of the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventEntry_OccurrenceCounterType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_Init
 *****************************************************************************/
/*!
  * \brief        Initializes the event entry
  *
  * \details      Initializes the event entry
  *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
  *
  * \pre          -
  *
  * \context      TASK|ISR2
  * \synchronous  TRUE
  * \reentrant    TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_Init(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_CopyEntry
 *****************************************************************************/
/*!
 * \brief         Copies the content of an event entry to another event entry
 *
 * \details       Copies the content of an event entry to another event entry
 *
 * \param[in]     DestinationEntryIndex
 *                The event entry handle of the destination event entry
 * \param[in]     SourceEntryIndex
 *                The event entry handle of the source event entry
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_CopyEntry(
  Dem_Cfg_EntryIndexType DestinationEntryIndex,
  Dem_Cfg_EntryIndexType SourceEntryIndex
  );
#endif

#if (DEM_FEATURE_NEED_OCC_COUNTER == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_IncrementOccurrenceCounter
 *****************************************************************************/
/*!
 * \brief         Increment the occurrence counter in the event entry.
 *
 * \details       Increment the occurrence counter in the event entry, if it
 *                is lower than the maximum value (latch at maximum value).
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                The occurrence counter was incremented successfully.
 *                FALSE
 *                The occurrence counter was not incremented.
 * \config        DEM_FEATURE_NEED_OCC_COUNTER == STD_ON
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_IncrementOccurrenceCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_IncrementFailedCycleCounter
 *****************************************************************************/
/*!
 * \brief         Increment the 'Failed Cycle Counter' in the event entry.
 *
 * \details       Increment the 'Failed Cycle Counter' in the event entry, if
 *                it is lower than the maximum value (latch at maximum value).
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                The 'Failed Cycle Counter' was incremented successfully.
 *                FALSE
 *                The 'Failed Cycle Counter' was not incremented.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_IncrementFailedCycleCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_IncrementConsecutiveFailedCycleCounter
 *****************************************************************************/
/*!
 * \brief         Increment the 'Consecutive Failed Cycle Counter' in the 
 *                event entry
 *
 * \details       Increment the 'Consecutive Failed Cycle Counter' in the 
 *                event entry, if it is lower than the maximum value (latch at
 *                maximum value).
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                The 'Consecutive Failed Cycle Counter' was incremented 
 *                successfully
 *                FALSE
 *                The 'Consecutive Failed Cycle Counter' was not incremented
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_IncrementConsecutiveFailedCycleCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_InitializeEntry
 *****************************************************************************/
/*!
 * \brief         Initializes an event entry.
 *
 * \details       Initialize the different elements of an event's MemoryEntry 
 *                struct.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_InitializeEntry(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_GetEntryIndexToNvBlockIndex
 *****************************************************************************/
/*!
 * \brief         Returns the Nv Block Index referencing the event entry.
 *
 * \details       -
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        NV Block Id
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_NvBlockIndexType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetEntryIndexToNvBlockIndex(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry
 *****************************************************************************/
/*!
 * \brief         Checks whether the event entry is an aging only entry.
 *
 * \details       Checks whether the event entry is an aging only entry.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                Aging only entry
 * \return        FALSE
 *                Otherwise
 *                
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_TestEntryForEvent
 *****************************************************************************/
/*!
 * \brief         Test if given event occupies event entry linked to given index
 *
 * \details       Test if given event occupies event entry linked to given index
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * 
 * \return        TRUE
 *                Event is present at given entry
 * \return        FALSE
 *                Event is not present at given entry
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestEntryForEvent(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_TestUpdatable
 *****************************************************************************/
/*!
 * \brief         Test if the event entry is not an 'aging only' or 
 *                'retained in memory' entry.
 *
 * \details       Test if the event entry is not an 'aging only' or 
 *                'retained in memory' entry.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                If entry is not of type 'aging only' or 'retained in memory'
 * \return        FALSE
 *                If entry is of type 'aging only' or 'retained in memory'
 *
 * \pre           The event must be stored
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestUpdatable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_EVENTENTRY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EventEntry_Interface.h
 *********************************************************************************************************************/
