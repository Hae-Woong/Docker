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
/*! \addtogroup Dem_PermanentMemory
 *  \{
 *  \file       Dem_PermanentMemory_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Implementation of the Permanent Memory subcomponent which manages storage of permanent DTCs.
 *  \entity     PermanentMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PERMANENTMEMORY_IMPLEMENTATION_H)
#define DEM_PERMANENTMEMORY_IMPLEMENTATION_H

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
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_PermanentMemory_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Memories_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_Cfg_Declarations.h"
#include "Dem_PermanentEntry_Interface.h"
#include "Dem_FaultMemory_Interface.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_PERMANENTMEMORY_IMPLEMENTATION_FILENAME "Dem_PermanentMemory_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/*! Number of stored entries in confirmed chronology */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)                    Dem_PermanentMemory_ConfirmedChronoOverflow;
#endif

#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_START_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/*! Number of stored entries in confirmed chronology */
DEM_LOCAL VAR(uint16, DEM_VAR_NO_INIT)                    Dem_PermanentMemory_ConfirmedChronoCurrentCount;
#endif

#define DEM_STOP_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */




/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PermanentMemory_PrivateProperties Private Properties
 * \{
 */

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_SetOverflown
 *****************************************************************************/
/*!
 * \brief         Mark the confirmed chronology as overflown.
 *
 * \details       Mark the confirmed chronology as overflown.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_SetOverflown( 
  void
  );
#endif

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize
 *****************************************************************************/
/*!
 * \brief         Returns the max size of the permanent confirmed chronology.
 *
 * \details       Returns the max size of the permanent confirmed chronology.
 *
 * \return        Size of the confirmed chronology array.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize(
  void
  );



#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex
 *****************************************************************************/
/*!
 * \brief         Get the event Id in confirmed chronology
 *
 * \details       Get the event Id in confirmed chronology
 *
 * \param[in]     ConfirmedChronologyIndex
 *                Index in Dem_Cfg_StatusData.ConfirmedChronology[]
 *
 * \return        EventId
 *                Unique handle of the event.
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(
  CONST(uint16, AUTOMATIC)  ConfirmedChronologyIndex
  );
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex
 *****************************************************************************/
/*!
 * \brief         Set the event Id in confirmed chronology
 *
 * \details       Set the event Id in confirmed chronology
 *
 * \param[in]     ConfirmedChronologyIndex
 *                Index in Dem_Cfg_StatusData.ConfirmedChronology[]
 * \param[in]     EventId
 *                Unique handle of the event.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(
  CONST(uint16, AUTOMATIC)  ConfirmedChronologyIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize
 *****************************************************************************/
/*!
 * \brief         Get the number of occupied entries in the confirmed chronology.
 *
 * \details       Get the number of occupied entries in the confirmed chronology.
 *
 *
 * \return        Size
 *                Number of occupied entries in the confirmed chronology.
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize(
  void
  );
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize
 *****************************************************************************/
/*!
 * \brief         Set the number of occupied entries in the confirmed chronology.
 *
 * \details       Set the number of occupied entries in the confirmed chronology.
 *
 * \param[in]     Size
 *                New number of occupied entries in the confirmed chronology.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize(
  CONST(uint16, AUTOMATIC)  Size
  );
#endif

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_SetOverflown
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_SetOverflown( 
  void
  )
{
  Dem_PermanentMemory_ConfirmedChronoOverflow = TRUE;
}
#endif

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
)
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  return DEM_CFG_MAX_NUMBER_PERMANENT_CHRONOLOGY;
#else
  return 0u;
#endif
}

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(
  CONST(uint16, AUTOMATIC)  ConfirmedChronologyIndex
  )
{
  return Dem_Cfg_GetStatusData().ConfirmedChronology[ConfirmedChronologyIndex];                                                  /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperationLoopExit */
}
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(
  CONST(uint16, AUTOMATIC)  ConfirmedChronologyIndex,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (ConfirmedChronologyIndex >= Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PERMANENTMEMORY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_GetStatusData().ConfirmedChronology[ConfirmedChronologyIndex] = EventId;                                             /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */  /* SBSW_DEM_ARRAY_WRITE_STATUSDATA_CONFIRMEDCHRONOLOGY */
  }
}
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize(
  void
  )
{
  return Dem_PermanentMemory_ConfirmedChronoCurrentCount;
}
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize(
  CONST(uint16, AUTOMATIC)  Size
  )
{
  Dem_PermanentMemory_ConfirmedChronoCurrentCount = Size;
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_PermanentMemory_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_SetEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_SetEnabled(
  boolean State
)
{
#if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
  Dem_Cfg_AdminData.PermanentActivationState = State;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(State);
#endif
}

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_TestEnabled
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PermanentMemory_TestEnabled(
  void
  )
{
  boolean lReturnValue = FALSE;
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  lReturnValue = TRUE;
#if DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON
  lReturnValue = (boolean)Dem_Cfg_AdminData.PermanentActivationState;
#endif
#endif
  return lReturnValue;
}

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_SetInitPattern
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_SetInitPattern(
  CONST(uint16, AUTOMATIC)  InitPattern
  )
{
  Dem_Cfg_PermanentData.InitPattern = InitPattern;
}
#endif

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_GetInitPattern
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_PermanentMemory_GetInitPattern(
  void
  )
{
  return Dem_Cfg_PermanentData.InitPattern;
}
#endif

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_ResetOverflown
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_ResetOverflown(
  void
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_PermanentMemory_ConfirmedChronoOverflow = FALSE;
#endif
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PermanentMemory_Private Private Methods
 * \{
 */

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_FillUpFromPrimary
 *****************************************************************************/
/*!
 * \brief         Fills up the permanent memory after healing some entries
 *
 * \details       This function checks for OBD relevant DTCs that currently
 *                trigger the MIL but are not stored in permanent memory. Up to
 *                all empty permanent slots are filled with such events.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_FillUpFromPrimary(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
 /* ****************************************************************************
  * Dem_Memories_PermanentMemory_FillUpAddEvent
  *****************************************************************************/
  /*!
   * \brief         Add an event to the permanent memory.
   *
   * \details       Add an event to the permanent memory. Set the event to 'active '
   *                and 'visible' if it's WIR-bit is qualified.
   *
   * \param[in]     EventId
   *                Unique handle of the event.
   * \pre           -
   * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
   *
   * \context       TASK
   * \synchronous   TRUE
   * \reentrant     FALSE
   *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_FillUpAddEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_FillUpFromConfirmedChrono
 *****************************************************************************/
 /*!
  * \brief         Fill up the permanent memory from entries of the
  *                confirmed chronology.
  *
  * \details       Fill up the permanent memory from entries of the
  *                confirmed chronology.
  *
  * \pre           -
  * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
  *
  * \return        E_OK
  *                The permanent memory is completely filled.
  *
  * \return        E_NOT_OK
  *                The permanent memory is not completely filled.
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     FALSE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PermanentMemory_FillUpFromConfirmedChrono(
  void
);
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_TestEventStored
 *****************************************************************************/
 /*!
  * \brief         Test if event is already stored in confirmed chronology.
  *
  * \details       Test if event is already stored in confirmed chronology.
  *
  * \param[in]     EventId
  *                Unique handle of the event.
  * \pre           -
  * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     FALSE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_TestEventStored(
  CONST(Dem_EventIdType, AUTOMATIC)  Event
);
#endif
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_FreeAtIndex
 *****************************************************************************/
 /*!
  * \brief         Frees the entry in confirmed chronology at forwarded index position
  *
  * \details       Move all entries that are newer than the entry to delete
  *                one step forward in the chronology array until the entry
  *                at the forwarded index position is overwritten.
  *
  * \param[in]     ConfirmedChronologyIndex
  *                Index in Dem_Cfg_StatusData.ConfirmedChronology[]
  *
  * \pre           -
  * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
  *
  * \context       TASK|ISR2
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_FreeAtIndex(
  CONST(uint16, AUTOMATIC)  ConfirmedChronologyIndex
);
#endif

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_FillUpFromPrimary
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
Dem_Memories_PermanentMemory_FillUpFromPrimary(
  void
  )
{
  Dem_EventIdType lEventId;

  /* scan for events which support the MIL. */
  for (lEventId = Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);                                              /* PRQA S 0771 */ /* MD_DEM_15.4_opt */
    (lEventId <= Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY));
       ++lEventId)
  {
    /* Only process master events (subevents are handled with the master) */
    if (Dem_Cfg_GetMasterEvent(lEventId) != lEventId)
    {
      continue;                                                                                                                  
    }

    if (Dem_DTC_TestEventStoresPermanentDTC(lEventId) == TRUE)
    {
      if (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() < Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize())
      {
        if (Dem_Event_UDSStatus_TestBit(Dem_DTC_GetInternaUDSStatus(lEventId), Dem_UdsStatus_CDTC) == TRUE)
        { /* Confirmed event found */
          Dem_Memories_PermanentMemory_ConfirmedChrono_AddEvent(lEventId, TRUE);
        }
      }
      else
      {
        if (Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) >= Dem_FaultMemory_Memories_GetMaxSize(DEM_CFG_MEMORYID_PERMANENT))
        {
          /*Confirmed chronology and permanent memory are full*/
          break;
        }
      }

      if (Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) < Dem_FaultMemory_Memories_GetMaxSize(DEM_CFG_MEMORYID_PERMANENT))
      {
        if (Dem_DTC_TestStatusForPermanentStorage(lEventId) == TRUE)
        {
          Dem_Memories_PermanentMemory_FillUpAddEvent(lEventId);
        }
      }
      else
      {
        if (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() >= Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize())
        {
          /*Confirmed chronology and permanent memory are full*/
          break;
        }
      }
    }
  }
  if (lEventId > Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY))
  {
    /*All primary events have been evaluated*/
    if (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() < Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize())
    {
      /* Chronology has still empty entries - remember confirmed chronology is not overflown*/
      Dem_Memories_PermanentMemory_ConfirmedChrono_ResetOverflown();
    }
  }
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#endif 

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_FillUpAddEvent
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_FillUpAddEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint8 lPermanentIndex;

  lPermanentIndex = Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(EventId);
  if (Dem_FaultMemory_Memories_GetMaxSize(DEM_CFG_MEMORYID_PERMANENT) == lPermanentIndex)
  {
    uint8 lPermanentState;

    lPermanentIndex = Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(DEM_EVENT_INVALID);
    lPermanentState = DEM_ESM_PERMANENT_NONE;

    Dem_MemoryEntry_PermanentEntry_SetEventId(lPermanentIndex, EventId);
    lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_ACTIVE);

    {
      if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
      {
        /* Only a qualified WIR is immediately visible */
        uint8 lQualifyState;
        lQualifyState = Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId);
        if (Dem_Event_InternalStatus_QualifyStatus_TestBit(lQualifyState, Dem_QualifyStatus_WIR) == TRUE)
        {
          lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
        }
      }
      else
      {
        lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
      }
    }
    Dem_FaultMemory_Memories_SetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) + 1u);
    Dem_MemoryEntry_PermanentEntry_SetStatus(lPermanentIndex, lPermanentState);
    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT),
      DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif 

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_TestEventStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_TestEventStored(
  CONST(Dem_EventIdType, AUTOMATIC)  Event
  )
{
  boolean lEventStored;
  uint16 lChronoIndex;
  lChronoIndex = 0u;
  lEventStored = FALSE;
  while (lChronoIndex < Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize())
  {
    if (Event == Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(lChronoIndex))
    {
      lEventStored = TRUE;
      break;
    }
    lChronoIndex++;
  }
  return lEventStored;
}
#endif 

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_FillUpFromConfirmedChrono
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
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PermanentMemory_FillUpFromConfirmedChrono(
  void
  )
{
  Std_ReturnType lReturnValue;
  uint16 lChronoIndex;

  lChronoIndex = 0u;
  lReturnValue = E_NOT_OK;

  while (lChronoIndex < Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize())
  {
    Dem_EventIdType lEventCandidate;
    lEventCandidate = Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(lChronoIndex);

    if (Dem_DTC_TestStatusForPermanentStorage(lEventCandidate) == TRUE)
    {
      Dem_Memories_PermanentMemory_FillUpAddEvent(lEventCandidate);

      if (Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) >= Dem_FaultMemory_Memories_GetMaxSize(DEM_CFG_MEMORYID_PERMANENT))
      {
        /* Permanent memory is completely filled up */
        lReturnValue = E_OK;
        break;
      }
    }
    lChronoIndex++;
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif 

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_FreeAtIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_FreeAtIndex(
  CONST(uint16, AUTOMATIC)  ConfirmedChronologyIndex
  )
{
  Dem_EventIdType lTempEventId;
  uint16 lChronoIterator;

  Dem_Internal_AssertReturnVoid(ConfirmedChronologyIndex < Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize(), 
    DEM_E_INCONSISTENT_STATE)

  lChronoIterator = Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize();
  --lChronoIterator;
  Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize(lChronoIterator);

  lTempEventId = Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(lChronoIterator);
  Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(lChronoIterator, DEM_EVENT_INVALID);
  while (lChronoIterator != ConfirmedChronologyIndex)
  {
    /* go backwards through the chronological list and move down all entries, until the entry to delete is overwritten */
    Dem_EventIdType lSwap;

    lSwap = lTempEventId;

    --lChronoIterator;
    lTempEventId = Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(lChronoIterator);
    Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(lChronoIterator, lSwap);
    Dem_Nvm_SetStatusBlockChanged();
  }
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_PermanentMemory_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_RestoreMemory
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
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_RestoreMemory(
  void
)
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  if ((Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE) && (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE))
  {
    uint8 lPermanentIndex;

    Dem_FaultMemory_Memories_SetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, 0);
    lPermanentIndex = Dem_FaultMemory_Memories_GetMaxSize(DEM_CFG_MEMORYID_PERMANENT);
    while (lPermanentIndex != 0u)
    {
      Dem_EventIdType lEventId;
      --lPermanentIndex;

      lEventId = Dem_MemoryEntry_PermanentEntry_GetEventId(lPermanentIndex);

      if (lEventId != DEM_EVENT_INVALID)
      {
        Dem_FaultMemory_Memories_SetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) + 1u);

        if ((Dem_Cfg_GetMasterEvent(lEventId) != lEventId)
          || (Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PRIMARY) > lEventId)
          || (Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY) < lEventId))
        {
          /*The configuration has changed somehow - remove entry */
          Dem_MemoryEntry_PermanentEntry_Free(lPermanentIndex);
        }
        else
        {
          if (DEM_ESM_GET_PERMANENT_STATE(Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex)) == DEM_ESM_PERMANENT_ACTIVE)
          {
            if ((Dem_DTC_TestEventStoresPermanentDTC(lEventId) == TRUE)
              && (Dem_DTC_TestStatusForPermanentStorage(lEventId) == FALSE))                                                     /* PRQA S 3415 */ /* MD_DEM_13.5_volatile */
            {
              if ((Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
                && (Dem_Cfg_EventIsCombined(lEventId) == FALSE))
              {
                uint8 lEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(lEventId);

                lEventStatus = Dem_Event_UDSStatus_SetBit(lEventStatus, Dem_UdsStatus_WIR);
                if (Dem_DTC_IsEventCombinationType2Enabled() == FALSE)
                {
                  lEventStatus = Dem_Event_UDSStatus_SetBit(lEventStatus, Dem_UdsStatus_CDTC);
                }
                Dem_Event_UDSStatus_SetInternalEventUdsStatus(lEventId, lEventStatus);

                Dem_Core_StatusIndicator_UpdateOnObdMemoryRestoreSetWIRSLC(lEventId);
              }
              else
              {
                Dem_Memories_PermanentMemory_ClearEvent(lEventId, FALSE);
              }
            }
          }
        }
      }
    }
    /* Assume worst case */
    Dem_Memories_PermanentMemory_ConfirmedChrono_SetOverflown();

    Dem_Memories_PermanentMemory_ConfirmedChrono_Restore();
  }
#endif
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_UpdatePermanentEntryOBD
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_UpdatePermanentEntryOBD(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus,
  CONST(uint8, AUTOMATIC)  NewDtcStatus
  )
{
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) && (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  uint8 lPermanentIndex;

  /* If the event has a permanent DTC */
  lPermanentIndex = Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(EventId);
  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    boolean lExitPassive;
    boolean lEntryRemoved;
    uint8 lPermanentStateNew;
    uint8 lPermanentStateOld;

    lExitPassive = FALSE;
    lPermanentStateNew = Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex);
    lPermanentStateOld = lPermanentStateNew;

    /* Mark the permanent entry as visible */
    lPermanentStateNew = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentStateNew);

    switch (DEM_ESM_GET_PERMANENT_STATE(lPermanentStateOld))
    {
    /* Permanent memory state last cycle was PERMANENT_ACTIVE:
       This implies that the WIR bit must have also been set, the check is kept for robustness */
    case DEM_ESM_PERMANENT_ACTIVE:
      if ( (Dem_Event_UDSStatus_TestBit(OldDtcStatus, Dem_UdsStatus_WIR) == TRUE)                                                /* COV_DEM_ROBUSTNESS TF tx tf */
        && (Dem_Event_UDSStatus_TestBit(NewDtcStatus, Dem_UdsStatus_WIR) == FALSE))
      {
        Dem_MemoryEntry_PermanentEntry_Free(lPermanentIndex);
      }
      break;

    /* Permanent memory state last cycle was PERMANENT_PASSIVE: */
    case DEM_ESM_PERMANENT_PASSIVE:
# if (DEM_CFG_SUPPORT_PERMANENT_MANDATORY_PFC == STD_OFF)
      /* Unless MANDATORY_PFC is configured, leave the PASSIVE state if the Event requests the MIL */
      if (Dem_Event_UDSStatus_TestBit(NewDtcStatus, Dem_UdsStatus_WIR) == TRUE)
      {
        lExitPassive = TRUE;
      }
      else
# endif
      /* Otherwise, if the event was not tested failed this cycle */
      if (Dem_Event_UDSStatus_TestBit(OldDtcStatus, Dem_UdsStatus_TFTOC) == FALSE)
      {
        /* Test if the PCF conditions are met, if so set the PFC condition for the permanent entry */
        if (DEM_OPERATIONCYCLE_TEST_PFC_CYCLE(Dem_OperationCycle_GetObdCycleStates()) == TRUE)
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
        }
        /* Test for a qualified passed test result, if so set the TESTED condition for the permanent entry */
        if (Dem_Event_UDSStatus_TestBit(OldDtcStatus, Dem_UdsStatus_TNCTOC) == FALSE)
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        }
        /* If all conditions are met for the permanent entry, exit the passive state */
        if ( (DEM_ESM_TEST_PERMANENT_CONDITION_TESTED(lPermanentStateNew) == TRUE)
          && (DEM_ESM_TEST_PERMANENT_CONDITION_PFC(lPermanentStateNew) == TRUE)
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
          && (DEM_ESM_TEST_PERMANENT_CONDITION_DCY(lPermanentStateNew) == TRUE)
# endif
          )
        {
          lExitPassive = TRUE;
        }
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
        /* Otherwise, if DCY qualification is configured as permanent clear condition, set the DCY condition
         * for the permanent entry */
        else
        {
          lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentStateNew);
        }
# endif
      }
      else
      {
        /* If the event was tested failed, reset the PFC and TESTED conditions for the permanent entry */
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
        /* If DCY qualification is configured as permanent clear condition, also set the DCY condition for the
         * permanent entry */
        lPermanentStateNew = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentStateNew);
# endif
      }
      break;

    default:                                                                                                                     /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      /* Do nothing */
      break;
    } /* end switch */

    /* On exit of state PASSIVE */
    lEntryRemoved = FALSE;
    if (lExitPassive == TRUE)
    {
      if (Dem_Event_UDSStatus_TestBit(NewDtcStatus, Dem_UdsStatus_WIR) == FALSE)
      { /* If the event no longer requests the MIL, remove the permanent entry. */
        Dem_MemoryEntry_PermanentEntry_Free(lPermanentIndex);
        lEntryRemoved = TRUE;
      }
      else
      { /* Otherwise, go to ACTIVE state and reset the PFC and TESTED conditions for the permanent entry. */
        lPermanentStateNew = DEM_ESM_SET_PERMANENT_STATE(lPermanentStateNew, DEM_ESM_PERMANENT_ACTIVE);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentStateNew);
        lPermanentStateNew = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentStateNew);
      }
    }

    /* If the permanent entry was changed, mark it for immediate NV synchronization */
    if ((lPermanentStateNew != lPermanentStateOld) && (lEntryRemoved == FALSE))
    {
      Dem_MemoryEntry_PermanentEntry_SetStatus(lPermanentIndex, lPermanentStateNew);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT), 
                                  DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  } /* (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount()) */
#else
 DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
 DEM_IGNORE_UNUSED_CONST_ARGUMENT(OldDtcStatus)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
 DEM_IGNORE_UNUSED_CONST_ARGUMENT(NewDtcStatus)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_UpdatePermanentEntryWWHOBD
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
Dem_Memories_PermanentMemory_UpdatePermanentEntryWWHOBD(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus
  )
{
#if (DEM_FEATURE_NEED_PERMANENT_FOR_WWHOBD == STD_ON)
  /* If the event has a permanent DTC */
  uint8 lPermanentIndex = Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(EventId);

  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    uint8 lPermanentState = Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex);

    if (DEM_ESM_GET_PERMANENT_STATE(lPermanentState) == DEM_ESM_PERMANENT_PASSIVE)
    {
      /* Otherwise, if the event was not tested failed this cycle */
      if (Dem_Event_UDSStatus_TestBit(OldDtcStatus, Dem_UdsStatus_TFTOC) == FALSE)
      {
        /* Test for a qualified passed test result, if so set the TESTED condition for the permanent entry */
        if (Dem_Event_UDSStatus_TestBit(OldDtcStatus, Dem_UdsStatus_TNCTOC) == FALSE)
        {
          Dem_MemoryEntry_PermanentEntry_Free(lPermanentIndex);
        }
      }
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OldDtcStatus)                                                                                 /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */



/* ****************************************************************************
 % Dem_Memories_PermanentMemory_AddEvent
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
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_AddEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_EventIdType lMasterEventId;
  uint8   lPermanentIndex;

  lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);

  /* scan through the permanent memory if the given event is available */
  lPermanentIndex = Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(lMasterEventId);

  if (Dem_FaultMemory_Memories_GetMaxSize(DEM_CFG_MEMORYID_PERMANENT) == lPermanentIndex)
  { /* Event not stored, try to find an empty slot */
    lPermanentIndex = Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(DEM_EVENT_INVALID);

    if (lPermanentIndex != Dem_FaultMemory_Memories_GetMaxSize(DEM_CFG_MEMORYID_PERMANENT))
    {
      uint8 lPermanentState;

      lPermanentState = DEM_ESM_PERMANENT_NONE;
      lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_ACTIVE);

      /* OBD-II or OBDonUDS enabled in this (PBS) variant */
      if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
      {
        /* PDTC is immediately visible if the Event is a 0-Trip event and OBD Visibility is not delayed. 
           If DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE is enabled, it is sufficient that the WIR bit is
           externally visible to set the permanent entry visible. */
        if (
# if (DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE == STD_ON)
          (Dem_Event_InternalStatus_QualifyStatus_TestBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_WIR) == TRUE)
# else
          (Dem_Cfg_EventTripTarget(EventId) == 0U)
          && (Dem_Event_TestObdVisibilityDelayed(EventId) == FALSE)
# endif
           )
        {
          lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
        }
      }
      /* WWH-OBD enabled in this (PBS) variant */
      else
      {
        lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
      }
      Dem_MemoryEntry_PermanentEntry_SetEventId(lPermanentIndex, lMasterEventId);
      Dem_MemoryEntry_PermanentEntry_SetStatus(lPermanentIndex, lPermanentState);

      Dem_FaultMemory_Memories_SetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) + 1u );

      /* persist status change in NVM */
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT), 
                                  DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
    else
    { /* Entry was not available and no free slot detected --> set overflow indication */
      Dem_FaultMemory_Memories_SetOverflow(DEM_CFG_MEMORYID_PERMANENT);
    }
  }
  else
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    /* Permanent DTC of event already stored */
    uint8 lPermanentStateNew;
    uint8 lPermanentStateOld;

    lPermanentStateNew = Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex);
    lPermanentStateOld = lPermanentStateNew;
    lPermanentStateNew = DEM_ESM_SET_PERMANENT_STATE(lPermanentStateNew, DEM_ESM_PERMANENT_ACTIVE);

    /* If the permanent entry was changed, mark it for immediate NV synchronization */
    if (lPermanentStateNew != lPermanentStateOld)
    {
      Dem_MemoryEntry_PermanentEntry_SetStatus(lPermanentIndex, lPermanentStateNew);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT), 
                                  DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  }
  else
  {
    /* MISRA case */
  }
#else /* (DEM_CFG_SUPPORT_PERMANENT== STD_ON) */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ClearEvent
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
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ClearEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)   ProcessClearDTC
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  uint8 lPermanentIndex;

  lPermanentIndex = Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(EventId);
  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    uint8 lPermanentState;
    lPermanentState = Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex);

    /* Go to PASSIVE state and reset the conditions to remove the PDTC. Keep its visibility state. */
    lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_PASSIVE);
    lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_TESTED(lPermanentState);

    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    {
      if (Dem_DTC_TestPfcRequired(EventId) == TRUE)
      {
        lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_PFC(lPermanentState);
      }
      else
      {
        lPermanentState = DEM_ESM_SET_PERMANENT_CONDITION_PFC(lPermanentState);
      }
# if (DEM_CFG_SUPPORT_PERMANENT_CONDITION_DCY == STD_ON)
      /* If DCY qualification is configured as permanent clear condition */
      if (DEM_OPERATIONCYCLE_TEST_DCY_QUALIFIED(Dem_OperationCycle_GetObdCycleStates()) == TRUE)
      { /* If the DCY is qualified, RESET the DCY condition for the permanent entry */
        lPermanentState = DEM_ESM_RESET_PERMANENT_CONDITION_DCY(lPermanentState);
      }
      else
      { /* Otherwise, set the DCY condition for the permanent entry */
        lPermanentState = DEM_ESM_SET_PERMANENT_CONDITION_DCY(lPermanentState);
      }
# endif
    }

    /* If the permanent entry was changed, mark it for cleared and immediate NV synchronization */
    if (lPermanentState != Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex))
    {
      Dem_MemoryEntry_PermanentEntry_SetStatus(lPermanentIndex, lPermanentState);
      if (ProcessClearDTC == TRUE)
      {
        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT),
          DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
      }
      else
      {
        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT),
          DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
      }
    }
  }
#else /* (DEM_CFG_SUPPORT_PERMANENT== STD_ON) */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ProcessClearDTC)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_Memories_PermanentMemory_FillUp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_FillUp(
  void
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Std_ReturnType lReturnValue;

  if (Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) < Dem_FaultMemory_Memories_GetMaxSize(DEM_CFG_MEMORYID_PERMANENT))
  {
    lReturnValue = Dem_Memories_PermanentMemory_FillUpFromConfirmedChrono();

    if ((lReturnValue == E_NOT_OK) && (Dem_PermanentMemory_ConfirmedChronoOverflow == TRUE))
    {
      Dem_Memories_PermanentMemory_FillUpFromPrimary();
    }
  }
#endif
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/* ****************************************************************************
 % Dem_Memories_PermanentMemory_EnableStorage
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PermanentMemory_EnableStorage(
  void
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant())                                                                             /* COV_DEM_ROBUSTNESS TX */
  {
    boolean lCurrentPermanentActivationState = FALSE;

    /* >>>> -------------------------------- Enter Critical Section: DcmApi
     * Critical section to read current value and write new value. This guarantees Dem_Memories_PermanentMemory_FillUp is called
     * only once.  */
    Dem_EnterCritical_DcmApi();

    /* PDTC storage may be activated from task level or by a diagnostic request */
    lCurrentPermanentActivationState = Dem_Memories_PermanentMemory_TestEnabled();
    if (lCurrentPermanentActivationState == FALSE)
    {
      /* permanent storage is not yet active */
      Dem_Memories_PermanentMemory_SetEnabled(TRUE);
    }

    Dem_LeaveCritical_DcmApi();
    /* <<<< -------------------------------- Leave Critical Section: DcmApi */

    if (lCurrentPermanentActivationState == FALSE)
    {
      Dem_Memories_PermanentMemory_FillUp();
    } /* else: permanent storage is already active; nothing to do */
    lReturnValue = E_OK;
  }
  return lReturnValue;
}


/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ProcessActivationMode4Enabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ProcessActivationMode4Enabled(
  void
  )
{
  if (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
  {
    uint8 lPermanentIndex;
    boolean lEntryChanged = FALSE;
    Dem_EventIdType lMasterEventId;

    /* This loop handles reconfirmation of passive DTCs. WWHOBD related DTCs may only become passive due to a clear operation.
       Reconfirming a DTC always results in both its CDTC and TF bit to be set. However, the check is keps for robustness. */
    for (lPermanentIndex = 0u; lPermanentIndex < Dem_Cfg_GlobalPermanentMaxCount(); ++lPermanentIndex)                           /* PRQA S 2994 */ /* MD_DEM_2994 */
    {
      lMasterEventId = Dem_MemoryEntry_PermanentEntry_GetEventId(lPermanentIndex);
      if (lMasterEventId != DEM_EVENT_INVALID)
      {
        if ((DEM_ESM_GET_PERMANENT_STATE(Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex)) == DEM_ESM_PERMANENT_PASSIVE))
        {
          if ((Dem_Core_DTC_IsClassB1(lMasterEventId) == TRUE)
             && (Dem_Event_InternalStatus_QualifyStatus_TestBit(Dem_DTC_GetQualifyStatus(lMasterEventId), Dem_QualifyStatus_CDTC) == TRUE)
             && (Dem_Event_UDSStatus_TestBit(Dem_DTC_GetInternaUDSStatus(lMasterEventId), Dem_UdsStatus_TF) == TRUE) )           /* COV_DEM_ROBUSTNESS TF tf tf tx */
          {
            uint8 lPermanentState = Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex);
            lPermanentState = DEM_ESM_SET_PERMANENT_STATE(lPermanentState, DEM_ESM_PERMANENT_ACTIVE);

            Dem_MemoryEntry_PermanentEntry_SetStatus(lPermanentIndex, lPermanentState);
            lEntryChanged = TRUE;
          }
        }
      }
    }

    Dem_Memories_PermanentMemory_FillUp();

    if (lEntryChanged == TRUE)
    {
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT),
        DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_Init(
  void
  )
{
  uint16 lConfirmedChronologyIndex = Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize();
  while (lConfirmedChronologyIndex > 0u)
  {
    lConfirmedChronologyIndex--;
    Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(lConfirmedChronologyIndex, DEM_EVENT_INVALID);
  }
  /* Initialize with worst case scenario */
  Dem_PermanentMemory_ConfirmedChronoOverflow = FALSE;
}
#endif

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_Restore
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_Restore(
  void
  )
{
  uint16 lConfirmedChronoIndex;
  Dem_EventIdType lEventId;

  /*Initialize current size of chronology*/
  Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize(0);
  
  /*Start at the end of the chronology array*/
  lConfirmedChronoIndex = Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize();
  while (lConfirmedChronoIndex != 0u)
  {
    --lConfirmedChronoIndex;
    lEventId = Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(lConfirmedChronoIndex);

    if (lEventId != DEM_EVENT_INVALID)
    {
      if (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() == 0u)
      {
        Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize(lConfirmedChronoIndex + 1u);
      }

      if ((Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PERMANENT) > lEventId)
        || (Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PERMANENT) < lEventId)
        || (Dem_Cfg_GetMasterEvent(lEventId) != lEventId)                                                                        /* COV_DEM_GENERATED_EVENTID */
         )
      {
        /*The configuration has changed somehow, event no longer belongs to primary memory - remove entry */
        Dem_Memories_PermanentMemory_ConfirmedChrono_FreeAtIndex(lConfirmedChronoIndex);
      }
      else
      {
        if (Dem_Event_UDSStatus_TestBit(Dem_DTC_GetInternaUDSStatus(lEventId), Dem_UdsStatus_CDTC) == FALSE)
        {
          /*Confirmed bit no longer set - remove entry */
          Dem_Memories_PermanentMemory_ConfirmedChrono_FreeAtIndex(lConfirmedChronoIndex);
        }
      }
    }
    else
    {
      if (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() != 0u)
      {
        /*There is an empty entry in the chronology although we already found a newer event -> fix chronology by clearing this entry*/
        Dem_Memories_PermanentMemory_ConfirmedChrono_FreeAtIndex(lConfirmedChronoIndex);
      }
    }
  } /*End while*/
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_FreeEvent
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_FreeEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_EventIdType lTempEventId;
  Dem_EventIdType lSwap;
  uint16 lChronoIterator;
  boolean lEventFound;
  lEventFound = FALSE;

  lChronoIterator = 0u;
  lTempEventId = Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(lChronoIterator);

  if (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() > 1u)
  {
    /* Go forwards through the chronological list and search for the event to free.
    If found shift all entries one entry backwards beginning at the entry to free  */
    do
    {
      lSwap = Dem_Memories_PermanentMemory_ConfirmedChrono_GetEventAtIndex(lChronoIterator + 1u);
      if ((lTempEventId == EventId) || (lEventFound == TRUE))
      {
        lEventFound = TRUE;
        Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(lChronoIterator, lSwap);
      }
      lTempEventId = lSwap;
      ++lChronoIterator;
    }
    while (lChronoIterator < (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() - 1u));
    
    if ((lSwap == EventId) || (lEventFound == TRUE))
    {
      /* Free last entry */
      Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(lChronoIterator, DEM_EVENT_INVALID);
      if (lEventFound == FALSE)
      {
        lEventFound = TRUE;
      }
    }
  }
  else
  {
    if (lTempEventId == EventId)
    {
     /* Just free the one existing entry */
      lEventFound = TRUE;
      Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(lChronoIterator, DEM_EVENT_INVALID);
    }
  }

  if (lEventFound == TRUE)
  {
    if (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() > 0u)
    {
      Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize(Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() - 1u);
    }
    else
    {
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);
#endif
}

/* ****************************************************************************
 % Dem_Memories_PermanentMemory_ConfirmedChrono_AddEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_AddEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  ConfirmedEvent,
  CONST(boolean, AUTOMATIC) SearchExistingEvent
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  uint16 lChronoIndex;
  boolean lEventStored;

  lEventStored = FALSE;
  if (SearchExistingEvent == TRUE)
  {
    lEventStored = Dem_Memories_PermanentMemory_ConfirmedChrono_TestEventStored(ConfirmedEvent);
  }

  if (lEventStored == FALSE)
  {
    lChronoIndex = Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize();
    if (Dem_Memories_PermanentMemory_ConfirmedChrono_GetCurrentSize() < Dem_Memories_PermanentMemory_ConfirmedChrono_GetMaxSize())
    {
      Dem_Memories_PermanentMemory_ConfirmedChrono_SetCurrentSize(lChronoIndex + 1u);
      Dem_Memories_PermanentMemory_ConfirmedChrono_SetEventAtIndex(lChronoIndex, ConfirmedEvent);
      Dem_Nvm_SetStatusBlockChanged();
    }
    else
    {
      /*Confirmed chronology overflow -> Configuration error: Size of confirmed chronology is too small*/
      Dem_PermanentMemory_ConfirmedChronoOverflow = TRUE;
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ConfirmedEvent);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SearchExistingEvent);
#endif
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_PERMANENTMEMORY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PermanentMemory_Implementation.h
 *********************************************************************************************************************/
