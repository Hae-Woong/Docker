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
/*! \addtogroup Dem_StatusIndicator
 *  \{
 *  \file       Dem_StatusIndicator_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of StatusIndicator subcomponent
 *  \entity     StatusIndicator
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_STATUSINDICATOR_IMPLEMENTATION_H)
#define DEM_STATUSINDICATOR_IMPLEMENTATION_H

 /* ********************************************************************************************************************
  *  GLOBAL MISRA JUSTIFICATIONS
  *********************************************************************************************************************/
 /* PRQA S 2982 EOF */ /* MD_DEM_2982 */
 /* PRQA S 2991 EOF */ /* MD_DEM_2991 */
 /* PRQA S 2992 EOF */ /* MD_DEM_2992 */
 /* PRQA S 2995 EOF */ /* MD_DEM_2995 */
 /* PRQA S 2996 EOF */ /* MD_DEM_2996 */

 /* ********************************************************************************************************************
  * INCLUDES
  *********************************************************************************************************************/

                                                   /* Own subcomponent header */
 /* ------------------------------------------------------------------------- */
#include "Dem_StatusIndicator_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Satellite_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_DTC_Interface.h"
#include "Dem_Event_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/
/*! Filename declaration */
#define DEM_STATUSINDICATOR_IMPLEMENTATION_FILENAME "Dem_StatusIndicator_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_StatusIndicator_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Core_StatusIndicator_SetBit
 *****************************************************************************/
/*!
 * \brief         Sets a single bit in the status indicator status byte
 *
 * \details       -
 *
 * \param[in]     StatusIndicator
 *                Status indicator status byte
 * \param[in]     BitIndex
 *                Bit index to set in the status byte
 *
 * \return        Status byte with the set bit value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/

DEM_LOCAL_INLINE FUNC(Dem_StatusIndicator_StatusType, DEM_CODE)
Dem_Core_StatusIndicator_SetBit(
  Dem_StatusIndicator_StatusType StatusIndicator,
  Dem_StatusIndicator_StatusIndicatorBitsType BitIndex
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
 /*!
  * \defgroup Dem_StatusIndicator_Private Private Methods
  * \{
  */

/* ****************************************************************************
 * Dem_Core_StatusIndicator_SymptomAction
 *****************************************************************************/
/*!
 * \brief         Process the queued StatusIndicator symptom action.
 *
 * \details       Process the queued StatusIndicator symptom action.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_SymptomAction(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_StatusIndicator_ProcessSatelliteQueueActions
 *****************************************************************************/
/*!
 * \brief         Process queued StatusIndicator symptom actions.
 *
 * \details       Iterate over all events and process the queued StatusIndicator 
 *                symptom action.
 *
 * \param[in]     SatelliteId
 *                Index of the satellite.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_ProcessSatelliteQueueActions(
  Dem_Satellite_IdType SatelliteId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_StatusIndicator_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 % Dem_Core_StatusIndicator_ResetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_StatusIndicator_StatusType, DEM_CODE)
Dem_Core_StatusIndicator_ResetBit(
  Dem_StatusIndicator_StatusType StatusIndicator,
  Dem_StatusIndicator_StatusIndicatorBitsType BitIndex
  )
{
  return (Dem_StatusIndicator_StatusType)Dem_Infrastructure_Utility_SetBitAtIndex(StatusIndicator, (uint8)BitIndex, FALSE);
}

/* ****************************************************************************
 % Dem_Core_StatusIndicator_TestBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_StatusIndicator_TestBit(
  Dem_StatusIndicator_StatusType StatusIndicator,
  Dem_StatusIndicator_StatusIndicatorBitsType BitIndex
  )
{
  return Dem_Infrastructure_Utility_TestBitAtIndex(StatusIndicator, (uint8)BitIndex);
}

/* ****************************************************************************
 % Dem_Core_StatusIndicator_isStatusIndicatorEnabled()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_StatusIndicator_isStatusIndicatorEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_SI30_STATUS == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_StatusIndicator_IsExtendedFimEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_StatusIndicator_IsExtendedFimEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_EXTENDED_FIM == STD_ON);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h" 

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_StatusIndicator_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 % Dem_Core_StatusIndicator_SetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_StatusIndicator_StatusType, DEM_CODE)
Dem_Core_StatusIndicator_SetBit(
  Dem_StatusIndicator_StatusType StatusIndicator,
  Dem_StatusIndicator_StatusIndicatorBitsType BitIndex
  )
{
  return (Dem_StatusIndicator_StatusType)Dem_Infrastructure_Utility_SetBitAtIndex(StatusIndicator, (uint8)BitIndex, TRUE);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_StatusIndicator_Private Private Methods
 * \{
 */

/* ****************************************************************************
 % Dem_Core_StatusIndicator_SymptomAction
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_SymptomAction(
  Dem_EventIdType EventId
  )
{
  Dem_InternalStatus_StoredStatusType lStoredState = Dem_Event_InternalStatus_GetStoredStatus(EventId);

  if ((lStoredState != Dem_InternalStatus_StoredStatus_None) && (lStoredState != Dem_InternalStatus_StoredStatus_Aged))
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);
    
    if (Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry(lEventEntryIndex) == FALSE)
    {
      Dem_StatusIndicator_StatusType lOldStatusIndicatorByte = Dem_Core_StatusIndicator_Get(EventId);
      Dem_StatusIndicator_StatusType lStatusIndicatorByte = Dem_Core_StatusIndicator_SetBit(lOldStatusIndicatorByte, Dem_StatusIndicator_SSLC);
      if (lStatusIndicatorByte != lOldStatusIndicatorByte)
      {
        Dem_Core_StatusIndicator_Set(EventId, lStatusIndicatorByte);
        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS), DEM_NVM_BLOCKSTATE_DIRTY);
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_StatusIndicator_ProcessSatelliteQueueActions
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
Dem_Core_StatusIndicator_ProcessSatelliteQueueActions(
  Dem_Satellite_IdType SatelliteId
  )
{
  Dem_Satellite_MonitorIdType lMonitorId;

  for (lMonitorId = 0u; lMonitorId < Dem_Cfg_GetEventCountOfSatelliteInfo(SatelliteId); ++lMonitorId)
  {
    if (Dem_Satellite_PeekStatusIndicatorActionQueue(SatelliteId, lMonitorId) != DEM_STATUSINDICATOR_QUEUE_INITIAL)
    {
      Dem_StatusIndicator_QueueActionType lStatusIndicatorQueueStatus;
      Dem_EventIdType lEventId;

      lStatusIndicatorQueueStatus = Dem_Satellite_ConsumeStatusIndicatorActionQueue(SatelliteId, lMonitorId);
      lEventId = Dem_Satellite_GetEventId(SatelliteId, lMonitorId);
      if (lStatusIndicatorQueueStatus == DEM_STATUSINDICATOR_QUEUE_SYMPTOM_REACHED)
      {
        Dem_Core_StatusIndicator_SymptomAction(lEventId);
      }
    }
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/*!
 * \defgroup Dem_StatusIndicator_ApiFunctions Api Functions
 * \{
 */


/* ****************************************************************************
% Dem_Core_StatusIndicator_Get
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(Dem_StatusIndicator_StatusType, DEM_CODE)
Dem_Core_StatusIndicator_Get(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
Dem_EventIdType EventId
)
{
  Dem_StatusIndicator_StatusType lStatusIndicator = DEM_EXT_STATUS_INITIALIZE;

#if (DEM_CFG_SUPPORT_SI30_STATUS == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EventId >= Dem_Cfg_EventCountOfStatusBlock())
  {
    Dem_Error_RunTimeCheckFailed(DEM_STATUSINDICATOR_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    lStatusIndicator = Dem_Cfg_GetStatusData().SI30Status[EventId];                                                              /* PRQA S 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lStatusIndicator = 0u;
#endif

  return lStatusIndicator;
}

/* ****************************************************************************
 % Dem_Core_StatusIndicator_Set
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_Set(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
Dem_EventIdType EventId,
Dem_StatusIndicator_StatusType StatusIndicator
)
{
#if (DEM_CFG_SUPPORT_SI30_STATUS == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EventId >= Dem_Cfg_EventCountOfStatusBlock())
  {
    Dem_Error_RunTimeCheckFailed(DEM_STATUSINDICATOR_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_GetStatusData().SI30Status[EventId] = StatusIndicator;                                                               /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_STATUSDATA_SI30STATUS */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StatusIndicator)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
  % Dem_Core_StatusIndicator_Init
  *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_Init(
  void
  )
  {
    if (Dem_Core_StatusIndicator_IsExtendedFimEnabled() == TRUE)
    {
      Dem_Scheduler_EnableTask(Dem_Scheduler_Task_StatusIndicator);
    }
  }

/* ****************************************************************************
 % Dem_Core_StatusIndicator_MainFunction
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_MainFunction(
  void
  )
{
  Dem_Satellite_IdType lSatelliteId;

  for (lSatelliteId = 0u; lSatelliteId < Dem_Cfg_GlobalSatelliteCount(); ++lSatelliteId)
  {
    if (Dem_SatelliteCore_TestAndClearStatusIndicatorQueueUpdated(lSatelliteId) == TRUE)
    {
      Dem_Core_StatusIndicator_ProcessSatelliteQueueActions(lSatelliteId);
    }
  }
}

/* ****************************************************************************
 % Dem_Core_StatusIndicator_UpdateOnTestFailed
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_StatusIndicator_UpdateOnTestFailed(
  Dem_EventIdType EventId,
  Dem_Cfg_StorageTriggerType UpdateFlags
  )
{
  boolean lReturnValue = FALSE;

  if (Dem_Core_StatusIndicator_isStatusIndicatorEnabled() == TRUE)
  {
    Dem_StatusIndicator_StatusType lStatusIndicatorByte = Dem_Core_StatusIndicator_Get(EventId);
    if ((UpdateFlags & DEM_CFG_TRIGGER_INDICATOR) != 0u)
    {
      lStatusIndicatorByte = Dem_Core_StatusIndicator_SetBit(lStatusIndicatorByte, Dem_StatusIndicator_WIRSLC);

      lReturnValue = TRUE;
    }

    if ((UpdateFlags & DEM_CFG_TRIGGER_TESTFAILED) != 0u) 
    {
      lStatusIndicatorByte = Dem_Core_StatusIndicator_SetBit(lStatusIndicatorByte, Dem_StatusIndicator_TFSLC);

      if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
      {
        lStatusIndicatorByte = Dem_Core_StatusIndicator_SetBit(lStatusIndicatorByte, Dem_StatusIndicator_ERDTC);
      }

      lReturnValue = TRUE;
    }
    Dem_Core_StatusIndicator_Set(EventId, lStatusIndicatorByte);
    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS), DEM_NVM_BLOCKSTATE_DIRTY);
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_StatusIndicator_UpdateOnEventAged
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_UpdateOnEventAged(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  if (Dem_Core_StatusIndicator_isStatusIndicatorEnabled() == TRUE)
  { 
    Dem_Core_StatusIndicator_Set(EventId, Dem_Core_StatusIndicator_SetBit(DEM_STATUSINDICATOR_RESET_VALUE, Dem_StatusIndicator_ADTC));
    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS), DEM_NVM_BLOCKSTATE_DIRTY);
  }
}

/* ****************************************************************************
 % Dem_Core_StatusIndicator_UpdateOnMemoryInitRestoreMemory
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_UpdateOnMemoryInitRestoreMemory(
  Dem_EventIdType EventId
  )
{
  if (Dem_Core_StatusIndicator_isStatusIndicatorEnabled() == TRUE)
  {
    if (Dem_Core_Event_TestHasIndicator(EventId) == FALSE)
    {
      Dem_StatusIndicator_StatusType lStatusIndicatorByte;
      lStatusIndicatorByte = Dem_Core_StatusIndicator_Get(EventId);
      if (Dem_Core_StatusIndicator_TestBit(lStatusIndicatorByte, Dem_StatusIndicator_WIRSLC) == TRUE)
      {
        lStatusIndicatorByte = Dem_Core_StatusIndicator_ResetBit(lStatusIndicatorByte, Dem_StatusIndicator_WIRSLC);
        Dem_Core_StatusIndicator_Set(EventId, lStatusIndicatorByte);
        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS), DEM_NVM_BLOCKSTATE_DIRTY);
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_StatusIndicator_UpdateOnObdMemoryRestoreSetWIRSLC
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_UpdateOnObdMemoryRestoreSetWIRSLC(                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  if (Dem_Core_StatusIndicator_isStatusIndicatorEnabled() == TRUE)
  {
    Dem_StatusIndicator_StatusType lOldStatusIndicatorByte = Dem_Core_StatusIndicator_Get(EventId);
    Dem_StatusIndicator_StatusType lStatusIndicatorByte = Dem_Core_StatusIndicator_SetBit(lOldStatusIndicatorByte, Dem_StatusIndicator_WIRSLC);

    if (lStatusIndicatorByte != lOldStatusIndicatorByte)
    {
      Dem_Core_StatusIndicator_Set(EventId, lStatusIndicatorByte);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS), DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_StatusIndicator_GetStatusIndicator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Core_StatusIndicator_GetStatusIndicator(
  Dem_EventIdType EventId,
  P2VAR(uint8, AUTOMATIC, DEM_APPL_DATA) Status
  )
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_Core_StatusIndicator_IsExtendedFimEnabled() == TRUE)
  {
    Dem_StatusIndicator_StatusType lStatusIndicatorByte = Dem_Core_StatusIndicator_Get(EventId);
    *Status = lStatusIndicatorByte;                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lReturnValue = E_OK;
  }

  return lReturnValue;
}

/* ****************************************************************************
% Dem_Core_StatusIndicator_ResetSI30byte
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_StatusIndicator_ResetSI30byte(
Dem_EventIdType EventId
)
{
  if (Dem_Core_StatusIndicator_isStatusIndicatorEnabled() == TRUE)
  {
    Dem_Core_StatusIndicator_Set(EventId, DEM_STATUSINDICATOR_RESET_VALUE);
    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS), DEM_NVM_BLOCKSTATE_DIRTY);
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* DEM_STATUSINDICATOR_IMPLEMENTATION_H */

 /*!
  * \}
  */
 /* ********************************************************************************************************************
  *  END OF FILE: Dem_StatusIndicator_Implementation.h
  *********************************************************************************************************************/
