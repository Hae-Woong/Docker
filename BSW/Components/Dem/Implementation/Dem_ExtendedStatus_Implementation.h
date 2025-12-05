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
/*! \addtogroup Dem_ExtendedStatus
 *  \{
 *  \file       Dem_ExtendedStatus_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     ExtendedStatus
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EXTENDEDSTATUS_IMPLEMENTATION_H)
#define DEM_EXTENDEDSTATUS_IMPLEMENTATION_H

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
#include "Dem_ExtendedStatus_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_InternalStatus_Interface.h"
#include "Dem_DTC_Interface.h"
#include "Dem_UDSStatus_Interface.h"
#include "Dem_EventIF_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_Infrastructure_Interface.h"

                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_EXTENDEDSTATUS_IMPLEMENTATION_FILENAME "Dem_ExtendedStatus_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ExtendedStatus_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/


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
 * \addtogroup Dem_ExtendedStatus_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Event_ExtendedStatus_GetEventStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_ExtendedStatus_StatusType, DEM_CODE)
Dem_Event_ExtendedStatus_GetEventStatus(
  Dem_EventIdType EventId
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON) || (DEM_CFG_SUPPORT_SIMILAR_CONDITIONS == STD_ON)
  /* Return extended status of event  */
  return Dem_Cfg_GetStatusData().ExtendedEventStatus[EventId];                                                                   /* PRQA S 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */
#else
  return DEM_EXT_STATUS_INITIALIZE;
#endif
}

/* ****************************************************************************
 % Dem_Event_ExtendedStatus_SetEventStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ExtendedStatus_SetEventStatus(
  Dem_EventIdType EventId,
  Dem_ExtendedStatus_StatusType Status
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Status)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON) || (DEM_CFG_SUPPORT_SIMILAR_CONDITIONS == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EventId >= Dem_Cfg_EventCountOfStatusBlock())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EXTENDEDSTATUS_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    /* Set extended status of event */
    Dem_Cfg_GetStatusData().ExtendedEventStatus[EventId] = Status;                                                               /* SBSW_DEM_ARRAY_WRITE_STATUSDATA_EXTENDEDEVENTSTATUS */
  }
#endif
}

/* ****************************************************************************
 % Dem_Event_ExtendedStatus_SetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_ExtendedStatus_StatusType, DEM_CODE)
Dem_Event_ExtendedStatus_SetBit(
  Dem_ExtendedStatus_StatusType ExtendedStatus,
  Dem_ExtendedStatus_ExtendedStatusBitsType BitIndex
  )
{
  return (Dem_ExtendedStatus_StatusType)Dem_Infrastructure_Utility_SetBitAtIndex(ExtendedStatus, (uint8)BitIndex, TRUE);
}

/* ****************************************************************************
 % Dem_Event_ExtendedStatus_ResetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_ExtendedStatus_StatusType, DEM_CODE)
Dem_Event_ExtendedStatus_ResetBit(
  Dem_ExtendedStatus_StatusType ExtendedStatus,
  Dem_ExtendedStatus_ExtendedStatusBitsType BitIndex
  )
{
  return (Dem_ExtendedStatus_StatusType)Dem_Infrastructure_Utility_SetBitAtIndex(ExtendedStatus, (uint8)BitIndex, FALSE);
}


/* ****************************************************************************
 % Dem_Event_ExtendedStatus_TestBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_ExtendedStatus_TestBit(
  Dem_ExtendedStatus_StatusType ExtendedStatus,
  Dem_ExtendedStatus_ExtendedStatusBitsType BitIndex
  )
{
  return Dem_Infrastructure_Utility_TestBitAtIndex(ExtendedStatus, (uint8)BitIndex);
}

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
 * \defgroup Dem_ExtendedStatus_Private Private Methods
 * \{
 */



/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/



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
 * \addtogroup Dem_ExtendedStatus_Public
 * \{
 */

 /* ****************************************************************************
 % Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC
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
Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
  if (Dem_GlobalDiagnostics_IsReadinessCalculationSupported() == TRUE)
  {
    if (Dem_GlobalDiagnostics_TestEventHasReadinessGroup(EventContext->EventId) == TRUE)
    {
      /* If event combination is supported and event is in a combined group */
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
      if (EventContext->GroupIndex != DEM_CFG_COMBINED_GROUP_INVALID)
      {
        Dem_UDSStatus_StatusType lGroupStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(EventContext->GroupIndex);
        /* If all combined events are tested this cycle and no one is currently test failed */
        if ((Dem_Event_UDSStatus_TestBit(lGroupStatus, Dem_UdsStatus_TF) == FALSE)
          && (Dem_Event_UDSStatus_TestBit(lGroupStatus, Dem_UdsStatus_TNCTOC) == FALSE)
          )
        {
          Dem_ExtendedStatus_StatusType lExtendedStatus = Dem_Event_ExtendedStatus_GetEventStatus(EventContext->EventId);
          /* If TestPassedSinceLastClear is not set already */
          if (Dem_Event_ExtendedStatus_TestBit(lExtendedStatus, Dem_ExtendedStatus_TPSLC) == FALSE)
          {
            Dem_Cfg_CombinedGroupIterType lCombinedIter;

            /* Set all events of combined group to TestPassedSinceLastClear */
            for (Dem_Cfg_CombinedGroupIterInit(EventContext->GroupIndex, &lCombinedIter);                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
              Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
              Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
            {
              Dem_EventIdType lSubEventId;
              lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
              /* If event is available */
              if (Dem_Event_TestEventUnavailable(lSubEventId) == FALSE)
              {
                /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
                Dem_EnterCritical_DiagMonitor();
                lExtendedStatus = Dem_Event_ExtendedStatus_GetEventStatus(lSubEventId);
                lExtendedStatus = Dem_Event_ExtendedStatus_SetBit(lExtendedStatus, Dem_ExtendedStatus_TPSLC);
                Dem_Event_ExtendedStatus_SetEventStatus(lSubEventId, lExtendedStatus);

                Dem_LeaveCritical_DiagMonitor();
                /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
              }
            }
            Dem_GlobalDiagnostics_ProcessEventTPSLCEnabled(EventContext->EventId);
          }
        }
      }
      else
# endif
      {
        /* Otherwise */
        Dem_ExtendedStatus_StatusType lExtendedStatus = Dem_Event_ExtendedStatus_GetEventStatus(EventContext->EventId);
        if (Dem_Event_ExtendedStatus_TestBit(lExtendedStatus, Dem_ExtendedStatus_TPSLC) == FALSE)
        {
          /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
          Dem_EnterCritical_DiagMonitor();

          /* Set extended status of event to TestPassedSinceLastClear */
          lExtendedStatus = Dem_Event_ExtendedStatus_SetBit(lExtendedStatus, Dem_ExtendedStatus_TPSLC);
          Dem_Event_ExtendedStatus_SetEventStatus(EventContext->EventId, lExtendedStatus);

          Dem_LeaveCritical_DiagMonitor();
          /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

          Dem_GlobalDiagnostics_ProcessEventTPSLCEnabled(EventContext->EventId);
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_EXTENDEDSTATUS_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ExtendedStatus_Implementation.h
 *********************************************************************************************************************/
