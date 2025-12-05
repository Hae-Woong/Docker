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
/*! \addtogroup Dem_DTCIF
 *  \{
 *  \file       Dem_DTCIF_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of DTCIF subcomponent
 *  \entity     DTCIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DTCIF_IMPLEMENTATION_H)
#define DEM_DTCIF_IMPLEMENTATION_H

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
#include "Dem_DTCIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Satellite_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Event_Interface.h"
#include "Dem_OperationCycle_Interface.h"
#include "Dem_StatusIndicator_Interface.h"
#include "Dem_Event_Interface.h"
#include "Dem_Aging_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_DTCIF_IMPLEMENTATION_FILENAME "Dem_DTCIF_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/*! Array of qualification processing states of combined DTCs */
DEM_LOCAL VAR(Dem_Cfg_ProcessedCombinedDTCGroupType, DEM_VAR_NO_INIT) Dem_DTC_QualificationProcessedCombinedDTCs;                /* PRQA S 3218 */ /* MD_DEM_8.7 */
#endif

#define DEM_STOP_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_DTC_PrivateProperties Private Properties
 * \{
 */


 /* ****************************************************************************
 * Dem_DTC_isRetryStorageEnabled
 *****************************************************************************/
 /*!
 * \brief         Returns true if feature 'retry storage' is enabled.
 * \details       -
 *
 * \return        TRUE
 *                Support for 'retry storage'
 * \return        FALSE
 *                No support for 'retry storage'
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_isRetryStorageEnabled(
  void
);


#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
/* ****************************************************************************
 * Dem_DTC_SetAgedCounter
 *****************************************************************************/
/*!
 * \brief         Sets the aged counter for the respective DTC.
 *
 * \details       Sets the aged counter for the respective DTC.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \param[in]     AgedCounter
 *                The aged counter to be set.
 *
 * \config        DEM_CFG_DATA_AGED_COUNTER == STD_ON
 *
 * \pre           Event has a dedicated entry in the aged counter array.
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_SetAgedCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC) AgedCounter
  );
#endif

/* ****************************************************************************
 * Dem_Core_DTC_IsClassA
 *****************************************************************************/
 /*!
  * \brief        Test if event's DTC has severity class A
  *
  * \details      -
  *
  * \param[in]    EventId
  *               Unique handle of the event
  *
  * \return       TRUE: Event's DTC has severity class A
  *               FALSE: Event's DTC has not severity class A
  *
  * \pre           -
  *
  * \context       TASK|ISR2
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_DTC_IsClassA(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_DTC_IsDTCFormatJ1939Enabled
 *****************************************************************************/
/*!
 * \brief         Check if DTC format J1939 is supported
 *
 * \details       Check if DTC format J1939 is supported
 *
 * \return        TRUE
 *                Support for DTC Format J1939
 * \return        FALSE
 *                No support for DTC Format J1939
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsDTCFormatJ1939Enabled(
  void
);

/* ****************************************************************************
 * Dem_DTC_IsObdUdsDTCSeparationEnabled
 *****************************************************************************/
/*!
 * \brief         Check if feature OBD/UDS DTC separation is enabled
 *
 * \details       Check if feature OBD/UDS DTC separation is enabled
 *
 * \return        TRUE
 *                DTC separation is enabled
 * \return        FALSE
 *                DTC separation is not enabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsObdUdsDTCSeparationEnabled(
  void
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

 /* ****************************************************************************
 % Dem_DTC_isRetryStorageEnabled
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_isRetryStorageEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_RETRY_STORAGE == STD_ON);
}


#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
/* ****************************************************************************
 % Dem_DTC_SetAgedCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_SetAgedCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC) AgedCounter
  )
{
  uint16 lAgedCounterIndex;
  lAgedCounterIndex = Dem_Cfg_GetAgedCounterIndexOfEventTable(EventId);

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (lAgedCounterIndex >= DEM_CFG_MAX_NUMBER_AGED_DTCS)
  {
    Dem_Error_RunTimeCheckFailed(DEM_DTCIF_IMPLEMENTATION_FILENAME, __LINE__);                                                   /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_GetAgingData().AgedCounter[lAgedCounterIndex] = AgedCounter;                                                         /* SBSW_DEM_ARRAY_WRITE_AGINGDATA_AGEDCOUNTER */
  }
}
#endif

/* ****************************************************************************
 % Dem_Core_DTC_IsClassA
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_DTC_IsClassA(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  uint8 lWwhObdDtcClass;
  lWwhObdDtcClass = Dem_Core_DTC_GetWwhObdDtcClass(EventId);
  return (boolean)(lWwhObdDtcClass == DEM_ACTIVATIONMODE_DTCCLASS_A);
}

/* ****************************************************************************
 % Dem_DTC_IsDTCFormatJ1939Enabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsDTCFormatJ1939Enabled(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void)
{
  return (DEM_CFG_SUPPORT_J1939 == STD_ON);
}

/* ****************************************************************************
 % Dem_DTC_IsObdUdsDTCSeparationEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsObdUdsDTCSeparationEnabled(
  void)
{
  return (DEM_CFG_SUPPORT_OBD_UDS_DTC_SEPARATION == STD_ON);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_DTC_PublicProperties
 * \{
 */

/* ****************************************************************************
% Dem_DTC_IsMemIndependentCycleCounterEnabled
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsMemIndependentCycleCounterEnabled(
  void
)
{
  return (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON);
}

/* ****************************************************************************
 % Dem_DTC_IsEventCombinationType2Enabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsEventCombinationType2Enabled(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
)
{
  return DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON;
}

/* ****************************************************************************
 % Dem_Core_DTC_IsObdOnUds3ByteDTCSupportedInVariant
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_DTC_IsObdOnUds3ByteDTCSupportedInVariant(
  void)
{
  return (boolean)((Dem_DTC_IsObdUdsDTCSeparationEnabled() == TRUE) && (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE));
}

/*****************************************************************************
 % Dem_DTC_IsDTCFormatValidWithOrigin
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsDTCFormatValidWithOrigin(
  Dem_DTCFormatType  DTCFormat,
  Dem_DTCOriginType  DTCOrigin,
  Dem_DTCFormat_ContextType  Context)
{
  boolean lValid = TRUE;

  switch (Context)
  {
  case Dem_DTCFormat_Context_SelectDTC:
    if ( (DTCFormat == DEM_DTC_FORMAT_OBD_3BYTE)
      && (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) )
    {
      lValid = FALSE;
    }
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }

  return lValid;
}

/*****************************************************************************
 % Dem_DTC_IsDTCFormatValidInVariant
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsDTCFormatValidInVariant(
  Dem_DTCFormatType  DTCFormat,
  Dem_DTCFormat_ContextType  Context)
{
  boolean lValid = FALSE;
  
  switch (DTCFormat)
  {
    case DEM_DTC_FORMAT_UDS:
      if (Context == Dem_DTCFormat_Context_SetFilterByReadinessGroup)
      {
        lValid = (Dem_Core_DTC_IsObdOnUds3ByteDTCSupportedInVariant() == FALSE);
      }
      else
      {
        lValid = TRUE;
      }
      break;

    case DEM_DTC_FORMAT_OBD:
      if (Context == Dem_DTCFormat_Context_GetDTCOfOBDFF)
      {
        lValid = TRUE;
      }
      else
      if (Context == Dem_DTCFormat_Context_SetFFRecFilter)
      {
        lValid = Dem_Cfg_IsObdIIExclusivelySupportedInVariant();
      }
      else
      if ( (Context == Dem_DTCFormat_Context_SelectDTC)
        || (Context == Dem_DTCFormat_Context_SetDTCFilter) )
      {
        lValid = (Dem_Cfg_IsObdOnUdsSupportedInVariant() == FALSE);
      }
      else
      {
        /* Not Valid in current context */
      }
      break;

    case DEM_DTC_FORMAT_OBD_3BYTE:
      if ( (Context == Dem_DTCFormat_Context_SetDTCFilter)
        || (Context == Dem_DTCFormat_Context_SelectDTC)
        || (Context == Dem_DTCFormat_Context_SetFilterByERecNumber)
        || (Context == Dem_DTCFormat_Context_SetFilterByReadinessGroup)
         )
      {
        lValid = Dem_Core_DTC_IsObdOnUds3ByteDTCSupportedInVariant();
      }
      break;

    case DEM_DTC_FORMAT_J1939:
      if (Context == Dem_DTCFormat_Context_SelectDTC)
      {
        lValid = TRUE;
      }
      else
      if (Context == Dem_DTCFormat_Context_GetDTCOfOBDFF)
      {
        lValid = Dem_DTC_IsDTCFormatJ1939Enabled();
      }
      else
      {
        /* Not Valid in current context */
      }
      break;

    default:
      /* Format not valid */
      break;
  }

  return lValid;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_DTC_SetQualifyStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_SetQualifyStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  Status
  )
{
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;
  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);

  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_Cfg_CombinedGroupIterType lCombinedIter;
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      /* Ignore event availability to prevent inconsistent qualification states later on */
      Dem_EventIdType lSubEventId;
      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(lSubEventId, Status);
    }
  }
  else
# endif
  {
    Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId, Status);
  }
}


#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
/* ****************************************************************************
 % Dem_DTC_GetAgedCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTC_GetAgedCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint16 lAgedCounterIndex;

  lAgedCounterIndex = Dem_Cfg_GetAgedCounterIndexOfEventTable(EventId);
  return Dem_Cfg_GetAgingData().AgedCounter[lAgedCounterIndex];
}
#endif

/* ****************************************************************************
 % Dem_Core_DTC_IsClassB1
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_DTC_IsClassB1(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  uint8 lWwhObdDtcClass;
  lWwhObdDtcClass = Dem_Core_DTC_GetWwhObdDtcClass(EventId);
  return (boolean)(lWwhObdDtcClass == DEM_ACTIVATIONMODE_DTCCLASS_B1);
}

/* ****************************************************************************
 % Dem_Core_DTC_HasJ1939DTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_DTC_HasJ1939DTC(
  Dem_EventIdType EventId
)
{
  uint32 J1939DTC;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3206, 3112 */ /* MD_DEM_3206, MD_DEM_14.2 */

# if (DEM_CFG_SUPPORT_J1939 == STD_ON)
  J1939DTC = Dem_Cfg_EventJ1939Dtc(EventId);
#else
  J1939DTC = DEM_CFG_DTC_J1939_INVALID;
#endif

  return (boolean) (J1939DTC != DEM_CFG_DTC_J1939_INVALID);
}

/* ****************************************************************************
 % Dem_Core_DTC_IsObdRelated
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_DTC_IsObdRelated(
  Dem_EventIdType EventId
)
{
  boolean lReturnValue;

  lReturnValue = FALSE;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3206, 3112 */ /* MD_DEM_3206, MD_DEM_14.2 */

#if (DEM_FEATURE_NEED_OBD == STD_ON)
# if (DEM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
    lReturnValue = (Dem_Cfg_GetFunctionalGroupIdentifierOfDtcNumberTable(Dem_Cfg_GetDtcNumberTableIdxOfEventTable(EventId)) == DEM_FGID_EMISSION_SYSTEM_GROUP);
# else
    lReturnValue = (Dem_Cfg_GetFunctionalGroupIdentifierOfDtcTable(Dem_Cfg_GetDtcTableIdxOfEventTable(EventId)) == DEM_FGID_EMISSION_SYSTEM_GROUP);
# endif
#endif

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Core_DTC_GetWwhObdDtcClass
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
  */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Core_DTC_GetWwhObdDtcClass(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  uint8 lReturnValue;
  Dem_DTCSeverityType lSeverity = Dem_Cfg_EventSeverity(EventId);

  switch (lSeverity & 0x1fU) /* ignore the UDS severity bits */
  {
  case DEM_SEVERITY_WWHOBD_CLASS_A:
    lReturnValue = DEM_ACTIVATIONMODE_DTCCLASS_A;
    break;
  case DEM_SEVERITY_WWHOBD_CLASS_B1:
    lReturnValue = DEM_ACTIVATIONMODE_DTCCLASS_B1;
    break;
  case DEM_SEVERITY_WWHOBD_CLASS_B2:
    lReturnValue = DEM_ACTIVATIONMODE_DTCCLASS_B2;
    break;
  case DEM_SEVERITY_WWHOBD_CLASS_C:
    lReturnValue = DEM_ACTIVATIONMODE_DTCCLASS_C;
    break;
  case DEM_SEVERITY_WWHOBD_CLASS_NO_CLASS:
    lReturnValue = DEM_ACTIVATIONMODE_DTCCLASS_NO;
    break;
  default:
    Dem_Error_ReportError(DEM_INTERNAL_APIID, DEM_E_INCONSISTENT_STATE);
    lReturnValue = 0x00U;
    break;
  }
  return lReturnValue;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_DTC_Private Private Methods
 * \{
 */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 /* ****************************************************************************
 * Dem_DTC_SetFdcToc
 *****************************************************************************/
 /*!
 * \brief         Sets Fdc trip flag for the DTC
 *
 * \details       Sets Fdc trip flag of the DTC in the DTC internal status
 *                field.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING is off
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventIds
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_SetFdcToc(
  Dem_EventIdType EventId
  );
#endif

 /* ****************************************************************************
 * Dem_DTC_IsMemIndependentFaultPendingCounterEnabled
 *****************************************************************************/
 /*!
 * \brief         Returns true if feature 'memory independent fault pending counter' is enabled.
 * \details       -
 *
 * \return        TRUE
 *                Feature supported
 * \return        FALSE
 *                Feature not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsMemIndependentFaultPendingCounterEnabled(
  void
);

/* ****************************************************************************
* Dem_DTC_IsMemIndependentFailedCycleCounterEnabled
*****************************************************************************/
/*!
* \brief         Returns true if feature 'memory independent failed cycle counter' is enabled.
* \details       -
*
* \return        TRUE
*                Feature supported
* \return        FALSE
*                Feature not supported
*
* \pre           -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsMemIndependentFailedCycleCounterEnabled(
  void
);

#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_GenerateExternalOnlyType1CombinedStatus
 *****************************************************************************/
/*!
 * \brief         Calculate externally visible DTC status for a Type 1 combined
 *                event
 *
 * \details       This function calculates an externally visible DTC status byte
 *                for a Type 1 combined event taking user controlled WIR bit,
 *                visibility of CDTC and WIR bits into account.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     DtcStatus
 *                Unmodified DTC status byte
 *
 * \return        The resulting DTC status
 *
 * \pre           Event belongs to a Combined DTC group
 *
 * \config        DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_GenerateExternalOnlyType1CombinedStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_UDSStatusType, AUTOMATIC)  DtcStatus
  );
#endif

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_GenerateExternalOnlyType2CombinedStatus
 *****************************************************************************/
/*!
 * \brief         Calculate externally visible DTC status for a combined event
 *
 * \details       This function calculates an externally visible DTC status byte
 *                for the combined event taking user controlled WIR bit,
 *                visibility of CDTC and WIR bits into account.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     DtcStatus
 *                Unmodified DTC status byte
 *
 * \return        The resulting DTC status
 *
 * \pre           Event belongs to a Combined DTC group
 *
 * \config        DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_GenerateExternalOnlyType2CombinedStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_UDSStatusType, AUTOMATIC)  DtcStatus
  );
#endif


# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_ProcessOperation
 *****************************************************************************/
/*!
 * \brief         Function processes the 'Requested effects' for a normal
 *                or combined event and recalculates the statuses.
 *
 * \details       Function processes the 'Requested effects' for a normal
 *                or combined event and recalculates the statuses.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     FuncPtr
 *                Operation to perform on all subevents
 * \param[in]     ArgPtr
 *                Arguments passed to the operation.
 *
 * \pre           -
 *
 * \config        (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_ProcessOperation(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Util_ForEachCombinedFuncType, AUTOMATIC)  FuncPtr,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );
#endif


#if ((DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF) \
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_DTC_CycleQualifyDcy_EventCombType2_StoredOnly
 *****************************************************************************/
/*!
 * \brief         Qualifies the OBD driving cycle for a configuration supporting
 *                event combination Type 2 in case event confirmation
 *                is coupled with event entry creation.
 *
 * \details       When the driving cycle is qualified, the environmental data,
 *                ConfirmedDTC bit and WIR bit become visible for external
 *                testers and the application.
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
 *                && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcy_EventCombType2_StoredOnly(
  void
  );
#endif



/* ****************************************************************************
 * Dem_DTC_TestGroupAvailable
 *****************************************************************************/
/*!
 * \brief         Tests if a combined event group is available based on runtime
 *                calibration
 *
 * \details       Tests if a combined event group is available based on runtime
 *                calibration
 *
 * \param[in]     GroupId
 *                Unique identifier of the combined event group
 *
 * \return        TRUE
 *                The group is available
 * \return        FALSE
 *                The group is not available
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestGroupAvailable(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId
  );

/* ****************************************************************************
 * Dem_DTC_TestGroupAvailableInVariant
 *****************************************************************************/
/*!
 * \brief         Tests if a combined event group is available based on the PB-S
 *                variant
 *
 * \details       Tests if a combined event group is available based on the PB-S
 *                variant
 *
 * \param[in]     GroupId
 *                Unique identifier of the combined event group
 *
 * \return        TRUE
 *                The group is available in the selected variant
 * \return        FALSE
 *                The group is not available in the selected variant
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestGroupAvailableInVariant(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId
  );


/* ****************************************************************************
 * Dem_DTC_TestGroupSuppression
 *****************************************************************************/
/*!
 * \brief         Tests if a combined event group is suppressed for reporting API
 *
 * \details       Tests if a combined event group is suppressed for reporting API
 *
 * \param[in]     GroupId
 *                Unique identifier of the combined event group
 *
 * \return        TRUE
 *                The group is suppressed
 * \return        FALSE
 *                The group is not suppressed
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestGroupSuppression(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId
  );


/* ****************************************************************************
 * Dem_DTC_ApplyUserControlledWIR
 *****************************************************************************/
/*!
 * \brief         Apply the user controlled WIR bit to the DTC's status
 *
 * \details       This function applies a user controlled WIR bit from the
 *                given internal status to the given DTC status byte.
 *
 *                The resulting WIR status cannot be reset by this function.
 *
 * \param[in]     DtcStatus
 *                Unmodified DTC status byte
 *
 * \param[in]     InternalStatus
 *                Internal event status information
 *
 * \return        The resulting DTC status
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_ApplyUserControlledWIR(
  CONST(Dem_DTC_UDSStatusType, AUTOMATIC)  DtcStatus,
  CONST(Dem_DTC_InternalStatusType, AUTOMATIC)  InternalStatus
  );


#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_CombinedGroupGetFDC
 *****************************************************************************/
/*!
 * \brief         Calculates the fault detection counter of a combined event
 *
 * \details       This function iterates through all sub-events, and takes the
 *                largest fault detection counter to represent the combined
 *                FDC.
 *                This is not stable against preemptive modification of the
 *                set of FDCs concerned. This is not prevented as it would
 *                lead to a large runtime within a critical section, without
 *                actually increasing the expected quality of the result.
 *
 *                Explanation: If all sub-events report a fairly stable result
 *                (the FDCs count fairly linearly), the combined FDC is also
 *                reliable if it is collected without critical section.
 *                If the sub-events change their result often and chaotically,
 *                the combined FDC also behaves chaotically, based on report
 *                order and task priorities. Collecting the FDCs within a
 *                critical section will not improve the result.
 *
 * \param[in]     GroupId
 *                Unique identifier of the combined event group
 * \param[out]    FaultDetectionCounter
 *                Pointer to receive the FDC value
 *
 * \return        E_OK
 *                The combined FDC was returned in FaultDetectionCounter
 * \return        E_NOT_OK
 *                The combined FDC could not be determined.
 *
 * \pre           RTE is started (if RTE is used)
 * \config        ((DEM_CFG_SUPPORT_DCM == STD_ON) || (DEM_CFG_DATA_CURRENT_FDC == STD_ON))
 *                && (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON) )
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different events
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_DTC_CombinedGroupGetFDC(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId,
  CONSTP2VAR(sint8, AUTOMATIC, DEM_APPL_DATA)  FaultDetectionCounter
  );
#endif


#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON) && (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_Disconnect_Combined
 *****************************************************************************/
/*!
 * \brief         Marks a combined event unavailable
 *
 * \details       This function sets a sub-event unavailable and re-calculates
 *                the availability and suppression states for the combined
 *                DTC.
 *
 *                A stored DTC may not be set to unavailable, so this function
 *                performs no actions on stored DTCs.
 *
 *                Do not call directly, use Dem_DTC_Disconnect instead.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Must be called in critical section DiagMonitor
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON &&
 *                DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_Disconnect_Combined(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON) && (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_Reconnect_Combined
 *****************************************************************************/
/*!
 * \brief         Marks a combined event available
 *
 * \details       This function sets a sub-event available and re-calculates
 *                the availability and suppression states for the combined
 *                DTC.
 *
 *                Do not call directly, use Dem_DTC_Reconnect instead.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Must be called in critical section DiagMonitor
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON &&
 *                DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_Reconnect_Combined(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 *  Dem_DTC_CustomizedAging
 *****************************************************************************/
/*!
 * \brief         Update event due to aging event (Customized trigger)
 *
 * \details       Update event due to aging event (Customized trigger)
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
 Dem_DTC_CustomizedAging(
  Dem_EventIdType EventId
  );
#endif

/* ****************************************************************************
 * Dem_DTC_ForEachCombinedSubEvent
 *****************************************************************************/
/*!
 * \brief         Executes a combined operation.
 *
 * \details       Due to excessive code duplication it was necessary to fall
 *                back to higher order functions. This function takes a
 *                function pointer to an operation to be executed for all
 *                sub-events of a combined event.
 *                For normal events, the operation is executed for that event
 *                only, so the caller does not need to distinguish between
 *                combined and non-combined events too much.
 *
 *                The caller is still responsible to call this function only
 *                once per combined event, e.g. by calling it only for the
 *                master event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     FuncPtr
 *                Operation to perform on all subevents
 * \param[in]     ArgPtr
 *                Arguments passed to the operation.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_ForEachCombinedSubEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Util_ForEachCombinedFuncType, AUTOMATIC)  FuncPtr,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );


#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_InitGroupInternalStatus
 *****************************************************************************/
/*!
 * \brief         Initializes the internal event status of a given combined
 *                event group
 *
 * \details       Initializes the internal event status of a given combined
 *                event group
 *
 * \param[in]     GroupId
 *                Unique identifier of the combined event group
 *
 * \pre           May be called only during module initialization
 * \config        DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_InitGroupInternalStatus(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId
  );
#endif


#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
/* ****************************************************************************
 * Dem_DTC_CycleQualifyDcy_AllDtc
 *****************************************************************************/
/*!
 * \brief         Qualifies the OBD driving cycle in case event confirmation
 *                is independent from event entry creation.
 *
 * \details       When the driving cycle is qualified, the environmental data,
 *                ConfirmedDTC bit and WIR bit become visible for external
 *                testers and the application.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_EMISSIONOBD == STD_ON &&
 *                DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcy_AllDtc(
  void
  );
#endif


#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
   && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF) \
   && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_OFF)
/* ****************************************************************************
 * Dem_DTC_CycleQualifyDcy_StoredOnly
 *****************************************************************************/
/*!
 * \brief         Qualifies the OBD driving cycle in case event confirmation
 *                is coupled with event entry creation.
 *
 * \details       When the driving cycle is qualified, the environmental data,
 *                ConfirmedDTC bit and WIR bit become visible for external
 *                testers and the application.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_EMISSIONOBD == STD_ON &&
 *                DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF &&
 *                DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_OFF
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcy_StoredOnly(
  void
  );
#endif


#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_SetDtcSuppression
 *****************************************************************************/
/*!
 * \brief         Marks an event's DTC suppressed
 *
 * \details       This function sets an event's DTC suppressed.
 *                For combined events, it does not matter which sub-event is
 *                passed to this function.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_SetDtcSuppression(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


/* ****************************************************************************
 * Dem_DTC_EventEntryReInit
 *****************************************************************************/
/*!
 * \brief         Reclaim an event entry
 *
 * \details       This function reclaims an event entry for
 *                the given event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           The event is stored already
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_EventEntryReInit(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );


/* ****************************************************************************
 * Dem_DTC_EventCreateMemory
 *****************************************************************************/
/*!
 * \brief         Allocate an event entry
 *
 * \details       This function tries to allocate an event entry for
 *                the given event. This can displace existing event entries
 *                if the memory is already full.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Entry index in the allocated memory. If no entry was
 *                allocated, DEM_CFG_ENTRYINDEX_INVALID is returned.
 *
 * \pre           The event may not be stored already
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_DTC_EventCreateMemory(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_DTC_SetFaultPendingCounter
 *****************************************************************************/
/*!
 * \brief         Set the memory independent fault pending counter of 
 *                the respective DTC.
 *
 * \details       Set the memory independent fault pending counter of 
 *                the respective DTC.
 *
 * \param[in]     FaultPendingCounterIndex
 *                Index to array FaultPendingCounter.
 *                The Index must be in range [1..DEM_CFG_MAX_NUMBER_FAULT_PENDING_DTCS[.
 * \param[in]     FaultPendingCounter
 *                Value to set.
 *
 * \pre           Feature memory independent fault pending counter is enabled.
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_SetFaultPendingCounter(
  Dem_Cfg_FaultPendingCounterIndexOfEventTableType FaultPendingCounterIndex,
  uint8 FaultPendingCounter
  );

/* ****************************************************************************
* Dem_DTC_SetFailedCycleCounter
*****************************************************************************/
/*!
* \brief         Set the memory independent failed cycle counter of
*                the respective DTC.
*
* \details       Set the memory independent failed cycle counter of
*                the respective DTC.
*
* \param[in]     FailedCycleCounterIndex
*                Index to array FailedCycleCounter.
*                The Index must be in range [1..DEM_CFG_MAX_NUMBER_FAILED_CYCLE_DTCS[.
* \param[in]     FailedCycleCounter
*                Value to set.
*
* \pre           Feature memory independent failed cycle counter is enabled.
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_SetFailedCycleCounter(
  Dem_Cfg_FailedCycleCounterIndexOfEventTableType FailedCycleCounterIndex,
  uint8 FailedCycleCounter
  );

/* ****************************************************************************
 * Dem_DTC_ResetFailedCycleCounter
 *****************************************************************************/
/*!
 * \brief         Reset the memory independent failed cycle counter of 
 *                the respective DTC.
 *
 * \details       Reset the memory independent failed cycle counter of 
 *                the respective DTC.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Feature memory independent failed cycle counter is supported.
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_ResetFailedCycleCounter(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
* Dem_DTC_IncrementFailedCycleCounter
*****************************************************************************/
/*!
* \brief         Increments the memory independent failed cycle counter of
*                the respective DTC.
*
* \details       Increments the memory independent failed cycle counter of
*                the respective DTC.
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
*
* \pre           Feature memory independent failed cycle counter is supported.
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_IncrementFailedCycleCounter(
  Dem_EventIdType EventId
);

/* ****************************************************************************
* Dem_DTC_IncrementFaultPendingCounter
*****************************************************************************/
/*!
* \brief         Increment the memory independent fault pending counter of
*                the respective DTC.
*
* \details       Increment the memory independent fault pending counter of
*                the respective DTC.
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
*
* \pre           Feature memory independent fault cycle counter is supported.
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_IncrementFaultPendingCounter(
  Dem_EventIdType  EventId
);

#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
/* ****************************************************************************
 * Dem_DTC_ResetAgedCounter
 *****************************************************************************/
/*!
 * \brief         Reset the aged counter of the respective DTC.
 *
 * \details       Reset the aged counter of the respective DTC.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \config        DEM_CFG_DATA_AGED_COUNTER == STD_ON
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_ResetAgedCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
/* ****************************************************************************
 * Dem_DTC_IncrementAgedCounter
 *****************************************************************************/
/*!
 * \brief         Increments the aged counter for the respective DTC.
 *
 * \details       Increments the aged counter for the respective DTC.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \config        DEM_CFG_DATA_AGED_COUNTER == STD_ON
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_IncrementAgedCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && \
    ((DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON) || (DEM_CFG_SUPPORT_TRIGGER_FDC == STD_ON) || (DEM_CFG_STORAGE_AT_FDC == STD_ON))
/* ****************************************************************************
 * Dem_DTC_CalcFdcUpdateFlags
 *****************************************************************************/
/*!
 * \brief         Test if the Fdc threshold is reached for the DTC for the first
 *                time in current operation cycle.
 *
 * \details       Test if the Fdc threshold is reached for the DTC for the first
 *                time in current operation cycle.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The Fdc Update Flag
 *                DEM_CFG_TRIGGER_FDC_FIRST_IN_CYCLE: Fdc threshold reached
 *                for the first time for DTC
 *                DEM_CFG_TRIGGER_FDC: Fdc threshold reached was reached for DTC
 *                before
 *
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF &&
 *                (DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON ||
 *                 DEM_CFG_SUPPORT_TRIGGER_FDC == STD_ON ||
 *                 DEM_CFG_STORAGE_AT_FDC == STD_ON)
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_StorageTriggerType, DEM_CODE)
Dem_DTC_CalcFdcUpdateFlags(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


/* ****************************************************************************
 * Dem_DTC_FreeEventAgingOnly
 *****************************************************************************/
/*!
 * \brief         Ends aging process of event with 'aging only' entry.
 *
 * \details       Free the event entry, resets DTC internal status and
 *                mark the corresponding NV block as 'Dirty'
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 *
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_FreeEventAgingOnly(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );


/* ****************************************************************************
 * Dem_DTC_TestEventEntryExists
 *****************************************************************************/
/*!
 * \brief         Tests whether an event entry exists for the DTC of the given
 *                event. 
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Event entry for DTC exists.
 * \return        FALSE
 *                Event entry for DTC does not exist.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
  Dem_DTC_TestEventEntryExists(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_EventCombinationOnStorage_Cleared
 *****************************************************************************/
/*!
 * \brief         Clears 'event combination on storage' DTCs.
 *
 * \details       Clears 'event combination on storage' DTCs.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                At least one subevent has been cleared.
 * \return        FALSE
 *                No subevent could be cleared.
 *
 * \pre           -
 *
 * \config        DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_EventCombinationOnStorage_Cleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_DTC_EventCombinationOnRetrieval_Cleared
 *****************************************************************************/
/*!
 * \brief         Clears 'event combination on retrieval' DTCs.
 *
 * \details       Clears 'event combination on retrieval' DTCs.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                At least one subevent has been cleared.
 * \return        FALSE
 *                No subevent could be cleared.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_EventCombinationOnRetrieval_Cleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
/* ****************************************************************************
 * Dem_DTC_CycleQualifyDcyForEachSubevent
 *****************************************************************************/
/*!
 * \brief         Update the DTC's state due to dcy qualification
 *
 * \details       Update the DTC's state due to dcy qualification
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     ArgPtr
 *                Arguments passed to the operation.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_EMISSIONOBD == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcyForEachSubevent(
  Dem_EventIdType  EventId,
  Dem_DTC_ForEachCombinedArgPtrType  ArgPtr
  );
#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
/* ****************************************************************************
 % Dem_DTC_SetFdcToc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_SetFdcToc(
  Dem_EventIdType EventId
  )
{
#if (DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON)
  Dem_Event_InternalStatusType lInternalStatus;
  /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();

  lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
  lInternalStatus = Dem_Event_InternalStatus_SetBit(lInternalStatus, Dem_InternalStatus_FDCTOC);
  Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);

  Dem_LeaveCritical_DiagMonitor();
  /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

 /* ****************************************************************************
 % Dem_DTC_IsMemIndependentFaultPendingCounterEnabled
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsMemIndependentFaultPendingCounterEnabled(
  void
)
{
  return (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON) && (DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON);
}

/* ****************************************************************************
% Dem_DTC_IsMemIndependentFailedCycleCounterEnabled
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_IsMemIndependentFailedCycleCounterEnabled(
  void
)
{
  return (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON) && (DEM_CFG_DATA_FAILED_CYCLES == STD_ON);
}

 /* ****************************************************************************
 % Dem_DTC_ResetFailedCycleCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_ResetFailedCycleCounter(
  Dem_EventIdType EventId
)
{
  if(Dem_DTC_IsMemIndependentFailedCycleCounterEnabled() == TRUE)
  {
    Dem_Cfg_FailedCycleCounterIndexOfEventTableType lFailedCounterIndex;

    lFailedCounterIndex = Dem_Cfg_GetFailedCycleCounterIndexOfEventTable(EventId);

    if (lFailedCounterIndex != DEM_CFG_FAILEDCYCLECOUNTER_INDEX_INVALID)
    { /* memory independent failed cycle counter supported for event */
      Dem_DTC_SetFailedCycleCounter(lFailedCounterIndex, 0);

      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_CYCLECOUNTERDATA),
        DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
}

/* ****************************************************************************
 % Dem_DTC_SetFaultPendingCounter
*****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_SetFaultPendingCounter(
  Dem_Cfg_FaultPendingCounterIndexOfEventTableType FaultPendingCounterIndex,
  uint8 FaultPendingCounter
)
{
#if (DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON)
  if ((Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
    && (FaultPendingCounterIndex >= DEM_CFG_MAX_NUMBER_FAULT_PENDING_DTCS))
  {
    Dem_Error_RunTimeCheckFailed(DEM_DTCIF_IMPLEMENTATION_FILENAME, __LINE__);                                                   /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  {
    Dem_Cfg_CycleCounterData.FaultPendingCounter[FaultPendingCounterIndex] = FaultPendingCounter;                                /* SBSW_DEM_ARRAY_WRITE_CYCLECOUNTERDATA_FAULTPENDINGCOUNTER */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FaultPendingCounter)                                                                          /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FaultPendingCounterIndex)                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
}

/* ****************************************************************************
 % Dem_DTC_SetFailedCycleCounter
*****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_SetFailedCycleCounter(
  Dem_Cfg_FailedCycleCounterIndexOfEventTableType FailedCycleCounterIndex,
  uint8 FailedCycleCounter
)
{
#if (DEM_CFG_DATA_FAILED_CYCLES == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON)
    if ( (Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
      && (FailedCycleCounterIndex >= DEM_CFG_MAX_NUMBER_FAILED_CYCLE_DTCS))
    {
      Dem_Error_RunTimeCheckFailed(DEM_DTCIF_IMPLEMENTATION_FILENAME, __LINE__);                                                 /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
    else
    {
      Dem_Cfg_CycleCounterData.FailedCycleCounter[FailedCycleCounterIndex] = FailedCycleCounter;                                 /* SBSW_DEM_ARRAY_WRITE_CYCLECOUNTERDATA_FAILEDCYCLECOUNTER */
    }
#else
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(FailedCycleCounter)                                                                         /* PRQA S 3112 */ /* MD_DEM_14.2 */
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FailedCycleCounterIndex)                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
}

/* ****************************************************************************
% Dem_DTC_IncrementFailedCycleCounter
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
Dem_DTC_IncrementFailedCycleCounter(
  Dem_EventIdType EventId
)
{
  Dem_Cfg_FailedCycleCounterIndexOfEventTableType lFailedCounterIndex;

  if(Dem_DTC_IsMemIndependentFailedCycleCounterEnabled() == TRUE)
  {
    lFailedCounterIndex = Dem_Cfg_GetFailedCycleCounterIndexOfEventTable(EventId);

    if (lFailedCounterIndex != DEM_CFG_FAILEDCYCLECOUNTER_INDEX_INVALID)                                                         /* COV_DEM_ROBUSTNESS TX */
    { /* memory independent failed cycle counter supported for event */
      uint8 lCycleCounter;
      lCycleCounter = Dem_DTC_GetFailedCycleCounter(lFailedCounterIndex);
      if (lCycleCounter < 0xffU)
      {
        lCycleCounter = (uint8)(lCycleCounter + 1u);
        Dem_DTC_SetFailedCycleCounter(lFailedCounterIndex, lCycleCounter);

        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_CYCLECOUNTERDATA),
          DEM_NVM_BLOCKSTATE_DIRTY);
      }
    }
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
}

/* ****************************************************************************
 % Dem_DTC_IncrementFaultPendingCounter
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
Dem_DTC_IncrementFaultPendingCounter(
  Dem_EventIdType EventId
)
{
  Dem_Cfg_FaultPendingCounterIndexOfEventTableType lFaultPendingCounterIndex;

  if(Dem_DTC_IsMemIndependentFaultPendingCounterEnabled() == TRUE)
  {
    lFaultPendingCounterIndex = Dem_Cfg_GetFaultPendingCounterIndexOfEventTable(EventId);

    if (lFaultPendingCounterIndex != DEM_CFG_FAULTPENDINGCOUNTER_INDEX_INVALID)
    { /* memory independent fault pending counter supported for event */
      uint8 lCycleCounter;
      lCycleCounter = Dem_DTC_GetFaultPendingCounter(lFaultPendingCounterIndex);
      if (lCycleCounter < 0xffU)
      {
        lCycleCounter = (uint8)(lCycleCounter + 1u);
        Dem_DTC_SetFaultPendingCounter(lFaultPendingCounterIndex, lCycleCounter);

        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_CYCLECOUNTERDATA),
          DEM_NVM_BLOCKSTATE_DIRTY);
      }
    }
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
}

#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_GenerateExternalOnlyType1CombinedStatus
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
DEM_LOCAL_INLINE FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_GenerateExternalOnlyType1CombinedStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONST(Dem_DTC_UDSStatusType, AUTOMATIC)  DtcStatus
  )
{

  Dem_DTC_UDSStatusType lExternalDTCStatus;

  lExternalDTCStatus = DtcStatus;
  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    lExternalDTCStatus = (Dem_DTC_UDSStatusType)(lExternalDTCStatus & Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId));
  }

# if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
  {
    /* DTC has combined group */
    boolean lWirActive;
    Dem_Cfg_CombinedGroupIndexType lGroupId;
    Dem_Cfg_CombinedGroupIterType lCombinedIter;
    
    lWirActive = FALSE;
    lGroupId = Dem_Cfg_EventCombinedGroup(EventId);

    /* Scan through all active sub-events */
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId;

      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      if ((Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
        && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE))
      {
        lWirActive = (boolean)(lWirActive
          || (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_UserControlledWIR) == TRUE));
      }
    }

    if (lWirActive == TRUE)
    {
      lExternalDTCStatus = Dem_Event_UDSStatus_SetBit(lExternalDTCStatus, Dem_UdsStatus_WIR);
    }
  }
# endif

  lExternalDTCStatus = Dem_DTC_ApplyCombinedStatus(lExternalDTCStatus);

  return lExternalDTCStatus;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_GenerateExternalOnlyType2CombinedStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_GenerateExternalOnlyType2CombinedStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONST(Dem_DTC_UDSStatusType, AUTOMATIC)  DtcStatus
  )
{
  /* DTC has combined group*/
  Dem_DTC_UDSStatusType lExternalDTCStatus;

# if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DtcStatus)                                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
  lExternalDTCStatus = Dem_DTC_CalculateCombinedGroupType2_ExternalUdsStatus(EventId);
# else
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
    lExternalDTCStatus = Dem_DTC_CalculateCombinedGroupType2_ExternalUdsStatus(EventId);
  }
  else
  {
    lExternalDTCStatus = DtcStatus;
  }
# endif

  lExternalDTCStatus = Dem_DTC_ApplyCombinedStatus(lExternalDTCStatus);

  return lExternalDTCStatus;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_ProcessOperation
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
Dem_DTC_ProcessOperation(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Util_ForEachCombinedFuncType, AUTOMATIC)  FuncPtr,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  )
{
  Dem_Cfg_CombinedGroupIndexType lCurrentlyProcessingGroupId;

  lCurrentlyProcessingGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lCurrentlyProcessingGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    ArgPtr->Common.OldDtcStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(lCurrentlyProcessingGroupId);                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    (*FuncPtr)(EventId, ArgPtr);                                                                                                 /* SBSW_DEM_CALL_FUNCPTR_FOR_EVENT */
    Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(lCurrentlyProcessingGroupId);
    ArgPtr->Common.NewDtcStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(lCurrentlyProcessingGroupId);                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  else
  /* Event is not a combined event */
  {
    ArgPtr->Common.OldDtcStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    (*FuncPtr)(EventId, ArgPtr);                                                                                                 /* SBSW_DEM_CALL_FUNCPTR_FOR_EVENT */
    ArgPtr->Common.NewDtcStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ((DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF) \
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_DTC_CycleQualifyDcy_EventCombType2_StoredOnly
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcy_EventCombType2_StoredOnly(
  void
  )
{
  Dem_MemoryEntry_AllocMemEntriesIterType lStoredEventsIter;
  Dem_DTC_InitCombinedGroupProcessed(Dem_DTC_QualificationProcessedCombinedDTCs);                                                /* SBSW_DEM_POINTER_FORWARD_GLOBAL */

  for (Dem_FaultMemory_MemoryEntry_ChronoIter_Init(DEM_CFG_MEMORYID_PRIMARY, &lStoredEventsIter);                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_FaultMemory_MemoryEntry_ChronoIter_Exists(&lStoredEventsIter) == TRUE;                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_FaultMemory_MemoryEntry_ChronoIter_Next(&lStoredEventsIter))                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex;
    Dem_EventIdType lEventId;
    uint8 lOldExternalDtcUdsStatus;
    Dem_DTC_ForEachCombinedArgType lArgs;
    Dem_Cfg_CombinedGroupIndexType lGroupId;

    lEventEntryIndex = Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex(&lStoredEventsIter);                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
    lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);

    if (Dem_Event_TestValidHandle(lEventId) == FALSE)
    {
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE)
      continue;                                                                                                                  
    }

    /* Skip non OBD Events */
    if (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE)
    {
      continue;                                                                                                                  
    }

# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
    /* Skip aged Events */
    if (Dem_Event_InternalStatus_GetStoredStatus(lEventId) == Dem_InternalStatus_StoredStatus_Aged)
    {
      continue;                                                                                                                  
    }
#endif

    lGroupId = Dem_Cfg_EventCombinedGroup(lEventId);
    if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      if (Dem_DTC_TestCombinedGroupProcessed(lGroupId, Dem_DTC_QualificationProcessedCombinedDTCs) == TRUE)                      /* SBSW_DEM_POINTER_FORWARD_GLOBAL */
      {
        continue;                                                                                                                
      }
      else
      {
        lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(lEventId);
        Dem_Event_ProcessDCYQualificationCombinedDTC_StoredOnly(lEventId, &lArgs);                                               /* SBSW_DEM_POINTER_FORWARD_STACK */
      }
    }
    else
    {
      lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(lEventId);
      Dem_Event_ProcessDCYQualificationNonCombinedDTC_StoredOnly(lEventId, lEventEntryIndex, &lArgs);                            /* SBSW_DEM_POINTER_FORWARD_STACK */
    }

    lArgs.Common.NewDtcStatus = Dem_DTC_GenerateExternalOnlyStatus(lEventId,
                                                                   lArgs.Common.NewDtcStatus);

# if (DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE == STD_ON)
    if ( (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
      && (Dem_Event_UDSStatus_TestBit(lOldExternalDtcUdsStatus, Dem_UdsStatus_WIR) == FALSE)
      && (Dem_Event_UDSStatus_TestBit(lArgs.Common.NewDtcStatus, Dem_UdsStatus_WIR) == TRUE))
    {
      Dem_MemoryEntry_PermanentEntry_UpdateVisiblity(lEventId);
    }
# endif

    Dem_Notifications_DtcStatusChanged(lEventId,
                                       lOldExternalDtcUdsStatus,
                                       lArgs.Common.NewDtcStatus,
                                       DEM_NOTIFICATIONS_CYCLE_QUALIFIED);

    if (Dem_Event_UDSStatus_TestBit((uint8)(lOldExternalDtcUdsStatus ^ lArgs.Common.NewDtcStatus), Dem_UdsStatus_CDTC) == TRUE)
    {
      Dem_Notifications_EventDataChanged(lEventId);
    }
  }
}                                                                                                                                /* PRQA S 6010, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_DTC_TestGroupAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestGroupAvailable(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  boolean lReturnValue;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON) && (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
  lReturnValue = (boolean)(Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetCombinedGroupStatus(GroupId), Dem_InternalStatus_Disconnected) == FALSE);
#else
  lReturnValue = TRUE;
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_DTC_TestGroupAvailableInVariant
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestGroupAvailableInVariant(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  boolean lReturnValue;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  lReturnValue = Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetCombinedGroupStatus(GroupId), Dem_InternalStatus_AvailableInVariant);
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_DTC_TestGroupSuppression
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestGroupSuppression(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  boolean lReturnValue;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON) && (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
  lReturnValue = Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetCombinedGroupStatus(GroupId), Dem_InternalStatus_SuppressedDTC);
#else
  lReturnValue = FALSE;
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_DTC_ApplyUserControlledWIR
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_ApplyUserControlledWIR(
  CONST(Dem_DTC_UDSStatusType, AUTOMATIC)  DtcStatus,
  CONST(Dem_DTC_InternalStatusType, AUTOMATIC)  InternalStatus                                                                   /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  Dem_DTC_UDSStatusType lStatus = DtcStatus;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(InternalStatus)                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
  if (Dem_Event_InternalStatus_TestBit(InternalStatus, Dem_InternalStatus_UserControlledWIR) == TRUE)
  {
    lStatus = Dem_Event_UDSStatus_SetBit(lStatus, Dem_UdsStatus_WIR);
  }
#endif
  return lStatus;
}

#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CombinedGroupGetFDC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_DTC_CombinedGroupGetFDC(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId,
  CONSTP2VAR(sint8, AUTOMATIC, DEM_APPL_DATA)  FaultDetectionCounter
)
{
  Std_ReturnType lReturnValue;
  sint8 lMaxFdc;
  Dem_Cfg_CombinedGroupIterType lCombinedIter;

  /* In rare cases GroupId is not required */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lReturnValue = E_OK;
  lMaxFdc = -128;

  for (Dem_Cfg_CombinedGroupIterInit(GroupId, &lCombinedIter);                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lSubEventId;
    lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if ((Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)                                                                   
      && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE))
    {
      sint8 lFdc;
      lFdc = -128;
      lReturnValue = Dem_GetFaultDetectionCounter(lSubEventId, &lFdc);                                                           /* SBSW_DEM_POINTER_FORWARD_STACK */
      if ((lReturnValue == E_OK) && (lMaxFdc < lFdc))
      {
        lMaxFdc = lFdc;
      }
    }
    if (lReturnValue != E_OK)
    {
      break;
    }
  }

  if (lReturnValue == E_OK)
  {
    *FaultDetectionCounter = lMaxFdc;                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON) && (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_Disconnect_Combined
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
Dem_DTC_Disconnect_Combined(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  Dem_DTC_InternalStatusType lGroupInternalStatus = Dem_Event_InternalStatus_GetCombinedGroupStatus(lGroupId);

  /* Remove pending actions: time debouncing, FDC triggers and queue state  */
  Dem_SatelliteCore_ClearEvent(
    Dem_Cfg_EventSatelliteId(EventId),
    Dem_Cfg_EventSatelliteEventId(EventId));

  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  {
    Dem_Event_ExtendedStatus_SetEventStatus(EventId,
      Dem_Event_ExtendedStatus_ResetBit(Dem_Event_ExtendedStatus_GetEventStatus(EventId), Dem_ExtendedStatus_SimilarConditions)
    );
  }
  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, 0x00U);

  if (Dem_Event_InternalStatus_TestBit(lGroupInternalStatus, Dem_InternalStatus_Disconnected) == FALSE)
  { /* If all Events belonging to a DTC are disconnected, that DTC is disconnected as well */
    /* If all connected events are suppressed or disconnected, the DTC is suppressed as well */
    boolean lDoDisconnect = TRUE;
    Dem_Cfg_CombinedGroupIterType lCombinedIter;

    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      if (Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
      {
        Dem_Event_InternalStatusType lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(lSubEventId);
        if (Dem_Event_InternalStatus_TestBit(lInternalStatus, Dem_InternalStatus_Disconnected) == FALSE)
        {
          lDoDisconnect = FALSE;
          break;
        }
      }
    }
    if (lDoDisconnect == TRUE)
    {
      lGroupInternalStatus = Dem_Event_InternalStatus_SetBit(lGroupInternalStatus, Dem_InternalStatus_Disconnected);
    }

    Dem_Event_InternalStatus_SetCombinedGroupStatus(lGroupId, lGroupInternalStatus);
    Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(lGroupId);

# if ((DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON) && (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON))
    {
      uint8 lCombinedGroupStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(lGroupId);
      /* If all combined events are tested this cycle and no one is currently test failed */
      if ((Dem_GlobalDiagnostics_TestEventHasReadinessGroup(EventId) == TRUE)
        && (Dem_Event_UDSStatus_TestBit(lCombinedGroupStatus, Dem_UdsStatus_TF) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(lCombinedGroupStatus, Dem_UdsStatus_TNCTOC) == FALSE)
        )
      {
        for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          Dem_EventIdType lSubEventId;
          lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
          /* If event is available */
          if (Dem_Event_TestEventUnavailable(lSubEventId) == FALSE)
          {
            Dem_ExtendedStatus_StatusType lExtendedStatus = Dem_Event_ExtendedStatus_GetEventStatus(lSubEventId);
            lExtendedStatus = Dem_Event_ExtendedStatus_SetBit(lExtendedStatus, Dem_ExtendedStatus_TPSLC);
            Dem_Event_ExtendedStatus_SetEventStatus(lSubEventId, lExtendedStatus);
          }
        }
      }
    }
# endif
  }
}                                                                                                                                /* PRQA S 6010, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON) && (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_Reconnect_Combined
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
Dem_DTC_Reconnect_Combined(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  Dem_DTC_InternalStatusType lGroupInternalStatus = Dem_Event_InternalStatus_GetCombinedGroupStatus(lGroupId);

  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, 0x50U);
  if (Dem_Event_InternalStatus_TestBit(lGroupInternalStatus, Dem_InternalStatus_Disconnected) == TRUE)
  {
    /* Since at least one sub-events is connected now, set the combined group to connected */
    lGroupInternalStatus = Dem_Event_InternalStatus_ResetBit(lGroupInternalStatus, Dem_InternalStatus_Disconnected);

    Dem_Event_InternalStatus_SetCombinedGroupStatus(lGroupId, lGroupInternalStatus);
  }
  Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(lGroupId);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 %  Dem_DTC_CustomizedAging
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
 Dem_DTC_CustomizedAging(
  Dem_EventIdType EventId
)
{
  if (Dem_Cfg_EventSupportAging(EventId) == TRUE)
  {
    uint8 lEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);

    if ((Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_TF) == FALSE))
    { /* Stored, not active event */
      Dem_UdsStatusByteType lOldEventStatus;
      Dem_UdsStatusByteType lOldExternalEventStatus;
      Dem_DTC_UDSStatusType lPreconfirmedStatus;

      /* Update UDS status byte */
      /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
      Dem_EnterCritical_DiagMonitor();

      lEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
      lOldEventStatus = lEventStatus;
      lOldExternalEventStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lOldEventStatus);

      lEventStatus = Dem_Event_UDSStatus_ResetBit(lEventStatus, Dem_UdsStatus_CDTC);

      Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, lEventStatus);

      /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
      Dem_LeaveCritical_DiagMonitor();

      if (Dem_Event_UDSStatus_IsNvRelevantStatusChange(lOldEventStatus, lEventStatus) == TRUE)
      {
        Dem_Nvm_SetStatusBlockChanged();
      }

      {
        Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);
        /* Update SI.30 status byte */
        lPreconfirmedStatus = Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(lEventEntryIndex);
        lPreconfirmedStatus = DEM_DTC_SET_PC_STATUS_ADTC(lPreconfirmedStatus);
        lPreconfirmedStatus = DEM_DTC_RESET_PC_STATUS_TFSLC(lPreconfirmedStatus);
        Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus(lEventEntryIndex, lPreconfirmedStatus);
      }

      Dem_Notifications_EventStatusChanged(EventId,
                                           lOldExternalEventStatus,
                                           Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lEventStatus));
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_DTC_ForEachCombinedSubEvent
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_ForEachCombinedSubEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Util_ForEachCombinedFuncType, AUTOMATIC)  FuncPtr,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  )
{
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    /* DTC has combined group */
    Dem_Cfg_CombinedGroupIterType lCombinedIter;

    ArgPtr->Common.OldDtcStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(lGroupId);                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    /* Iterate through all active sub-events */
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if ( (Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)                                                                
        && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE) )
      {
        (*FuncPtr)(lSubEventId, ArgPtr);                                                                                         /* SBSW_DEM_CALL_FOREACH_FUNCPTR */
      }
    }
    Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(lGroupId);
    ArgPtr->Common.NewDtcStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(lGroupId);                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  else
#endif
  {
    /* EventId is not a combined event */

    ArgPtr->Common.OldDtcStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    (*FuncPtr)(EventId, ArgPtr);                                                                                                 /* SBSW_DEM_CALL_FOREACH_FUNCPTR */
    ArgPtr->Common.NewDtcStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_InitGroupInternalStatus
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
Dem_DTC_InitGroupInternalStatus(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
# if (DEM_FEATURE_NEED_EVENTAVAILABLE_DEFAULT == STD_ON)
  Dem_Cfg_CombinedGroupIterType lSubEventIter;
  Dem_Event_InternalStatusType lInternalStatus;

  lInternalStatus = Dem_Event_InternalStatus_GetCombinedGroupStatus(GroupId);
  lInternalStatus = Dem_Event_InternalStatus_SetBit(lInternalStatus, Dem_InternalStatus_Disconnected);

  for (Dem_Cfg_CombinedGroupIterInit(GroupId, &lSubEventIter);                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_CombinedGroupIterExists(&lSubEventIter) == TRUE;                                                                  /* PRQA S 2990 */ /* MD_DEM_2990 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_CombinedGroupIterNext(&lSubEventIter))                                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    if (Dem_Cfg_EventAvailableByDefault(Dem_Cfg_CombinedGroupIterGet(&lSubEventIter)) == TRUE)                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_Disconnected);
      break;
    }
  }
  Dem_Event_InternalStatus_SetCombinedGroupStatus(GroupId, lInternalStatus);
# endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CycleQualifyDcy_AllDtc
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcy_AllDtc(
  void
)
{
  Dem_EventIdType lEventId;

  for (lEventId = Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PRIMARY); 
       lEventId <= Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY); 
       lEventId++)
  {
    uint8 lOldExternalDtcUdsStatus;
    Dem_DTC_ForEachCombinedArgType lArgs;

    /* Skip non OBD Events */
    if (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE)
    {
      continue;                                                                                                                  
    }

# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
    /* Only process master events (subevents are handled with the master) */
    if (lEventId != Dem_Cfg_GetMasterEvent(lEventId))
    {
      continue;                                                                                                                  
    }
# endif

    lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(lEventId);

    Dem_DTC_CycleQualifyDcyForEachSubevent(lEventId, &lArgs);                                                                    /* SBSW_DEM_POINTER_FORWARD_STACK */

    lArgs.Common.NewDtcStatus = Dem_DTC_GenerateExternalOnlyStatus(lEventId, lArgs.Common.NewDtcStatus);

    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    {
      /* When DEM_CFG_SUPPORT_TYPE2_COMBINATION == ON, OBDII freeze frame type can only be type-1 */
# if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)

      if ( (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
        && (Dem_Event_UDSStatus_TestBit(lOldExternalDtcUdsStatus, Dem_UdsStatus_CDTC) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(lArgs.Common.NewDtcStatus, Dem_UdsStatus_CDTC) == TRUE))
      {
        Dem_Memories_ObdFreezeFrameMemory_SetFFOfEventVisible(lEventId);
        Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 = Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame();
      }
# endif

# if (DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE == STD_ON)
      if ((Dem_Event_UDSStatus_TestBit(lOldExternalDtcUdsStatus, Dem_UdsStatus_WIR) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(lArgs.Common.NewDtcStatus, Dem_UdsStatus_WIR) == TRUE))
      {
        Dem_MemoryEntry_PermanentEntry_UpdateVisiblity(lEventId);
      }
#endif
    }

    Dem_Notifications_DtcStatusChanged(lEventId,
                                       lOldExternalDtcUdsStatus,
                                       lArgs.Common.NewDtcStatus,
                                       DEM_NOTIFICATIONS_CYCLE_QUALIFIED);

    if (Dem_Event_UDSStatus_TestBit((uint8)(lOldExternalDtcUdsStatus ^ lArgs.Common.NewDtcStatus), Dem_UdsStatus_CDTC) == TRUE)
    {
      Dem_Notifications_EventDataChanged(lEventId);
    }

    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS),
                                DEM_NVM_BLOCKSTATE_DIRTY);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF) \
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_OFF)
/* ****************************************************************************
 % Dem_DTC_CycleQualifyDcy_StoredOnly
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcy_StoredOnly(
  void
  )
{
  Dem_MemoryEntry_AllocMemEntriesIterType lStoredEventsIter;

  for (Dem_FaultMemory_MemoryEntry_ChronoIter_Init(DEM_CFG_MEMORYID_PRIMARY, &lStoredEventsIter);                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_FaultMemory_MemoryEntry_ChronoIter_Exists(&lStoredEventsIter) == TRUE;                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_FaultMemory_MemoryEntry_ChronoIter_Next(&lStoredEventsIter))                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex;
    Dem_EventIdType lEventId;
    Dem_DTC_UDSStatusType lOldExternalDtcUdsStatus = DEM_UDS_STATUS_INITIALIZE;
    Dem_DTC_UDSStatusType lNewExternalDtcUdsStatus = DEM_UDS_STATUS_INITIALIZE;
    Dem_DTC_ForEachCombinedArgType lArgs;

    lEventEntryIndex = Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex(&lStoredEventsIter);                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
    lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);

    if (Dem_Event_TestValidHandle(lEventId) == FALSE)
    {
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE)
      continue;
    }

    /* Skip non OBD Events */
    if (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE)
    {
      continue;
    }

# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
    /* Skip aged Events */
    if (Dem_Event_InternalStatus_GetStoredStatus(lEventId) == Dem_InternalStatus_StoredStatus_Aged)
    {
      continue;                                                                                                                  
    }
#endif

    lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(lEventId);

    Dem_DTC_CycleQualifyDcyForEachSubevent(lEventId, &lArgs);                                                                    /* SBSW_DEM_POINTER_FORWARD_STACK */

    Dem_Event_UpdateQualificationStatus(lEventId, lEventEntryIndex);

    lNewExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(lEventId);

    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    {
# if ((DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_OBDII == STD_ON))
      if ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
        &&(Dem_Event_UDSStatus_TestBit(lOldExternalDtcUdsStatus, Dem_UdsStatus_CDTC) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(lNewExternalDtcUdsStatus, Dem_UdsStatus_CDTC) == TRUE))
      {
        Dem_Memories_ObdFreezeFrameMemory_SetFFOfEventVisible(lEventId);
        Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 = Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame();
      }
#endif
# if (DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE == STD_ON)
      if ((Dem_Event_UDSStatus_TestBit(lOldExternalDtcUdsStatus, Dem_UdsStatus_WIR) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(lNewExternalDtcUdsStatus, Dem_UdsStatus_WIR) == TRUE))
      {
        Dem_MemoryEntry_PermanentEntry_UpdateVisiblity(lEventId);
      }
#endif
    }
    Dem_Notifications_DtcStatusChanged(lEventId,
                                       lOldExternalDtcUdsStatus,
      lNewExternalDtcUdsStatus,
                                       DEM_NOTIFICATIONS_CYCLE_QUALIFIED);
    if (Dem_Event_UDSStatus_TestBit((uint8)(lOldExternalDtcUdsStatus ^ lNewExternalDtcUdsStatus), Dem_UdsStatus_CDTC) == TRUE)
    {
      Dem_Notifications_EventDataChanged(lEventId);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_SetDtcSuppression
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
Dem_DTC_SetDtcSuppression(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_DTC_InternalStatusType lInternalStatus;
# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;

  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    lInternalStatus = Dem_Event_InternalStatus_GetCombinedGroupStatus(lGroupId);
    lInternalStatus = Dem_Event_InternalStatus_SetBit(lInternalStatus, Dem_InternalStatus_SuppressedDTC);
    Dem_Event_InternalStatus_SetCombinedGroupStatus(lGroupId, lInternalStatus);

    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
  }
  else
# endif
  {
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
    lInternalStatus = Dem_Event_InternalStatus_SetBit(lInternalStatus, Dem_InternalStatus_SuppressedDTC);
    Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);

    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON) */

/* ****************************************************************************
 % Dem_DTC_EventEntryReInit
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
Dem_DTC_EventEntryReInit(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_MemoryEntry_EventEntry_InitializeEntry(EventEntryIndex);

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();
  Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_Active);
  Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

  if (Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE)
  {
    Dem_Nvm_SetEventEntryNvBlockState(EventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
  }
  else
  {
    Dem_Nvm_SetEventEntryNvBlockState(EventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_DTC_EventCreateMemory
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
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_DTC_EventCreateMemory(
  Dem_EventIdType  EventId
  )
{
  Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_FaultMemory_Memories_AllocateEventEntry(Dem_Core_Event_GetDestination(EventId), EventId);

  if (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  { 
    /* Created new entry */
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_Active);
    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

    if (Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE)
    {
      Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
    else
    {
      Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
  return lEventEntryIndex;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
/* ****************************************************************************
 % Dem_DTC_IncrementAgedCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_IncrementAgedCounter(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_EventSupportAgedCounter(EventId) == TRUE)
  {
    uint8 lAgedCounter;
    lAgedCounter = Dem_DTC_GetAgedCounter(EventId);
    if (lAgedCounter < DEM_DTC_AGED_COUNTER_MAX_VALUE)
    {
      Dem_DTC_SetAgedCounter(EventId, lAgedCounter + 1u);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_AGINGDATA),
        DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
}
#endif

/* ****************************************************************************
 % Dem_DTC_FreeEventAgingOnly
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_FreeEventAgingOnly(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
# if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  Dem_Memories_EventMemory_FreeEntryOfEvent(EventEntryIndex, EventId);
  /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();
  /* In critical section 'DiagMonitor', set the events stored status to 'AGING' */
  Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_None);
  Dem_LeaveCritical_DiagMonitor();
  /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
  Dem_Nvm_SetEventEntryNvBlockState(EventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
# endif
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
/* ****************************************************************************
 % Dem_DTC_ResetAgedCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_ResetAgedCounter(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Cfg_EventSupportAgedCounter(EventId) == TRUE)
  {
    if (Dem_DTC_GetAgedCounter(EventId) != DEM_DTC_AGED_COUNTER_INIT_VALUE)
    {
      Dem_DTC_SetAgedCounter(EventId, DEM_DTC_AGED_COUNTER_INIT_VALUE);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_AGINGDATA),
        DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
}
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && ((DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON) || (DEM_CFG_SUPPORT_TRIGGER_FDC == STD_ON) || (DEM_CFG_STORAGE_AT_FDC == STD_ON))
/* ****************************************************************************
 % Dem_DTC_CalcFdcUpdateFlags
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
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_StorageTriggerType, DEM_CODE)
Dem_DTC_CalcFdcUpdateFlags(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_StorageTriggerType lUpdateFlags;

#if (DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON)
#  if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;
#  endif

  lUpdateFlags = (Dem_Cfg_StorageTriggerType)DEM_CFG_TRIGGER_FDC_FIRST_IN_CYCLE;
#  if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)

  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_Cfg_CombinedGroupIterType lCombinedIter;
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId;
      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      if (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_FDCTOC) == TRUE)
      {
        lUpdateFlags = (Dem_Cfg_StorageTriggerType)DEM_CFG_TRIGGER_FDC;
        break;
      }
    }
  }
  else
#  endif /* DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON */
  {
    if (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(EventId), Dem_InternalStatus_FDCTOC) == TRUE)
    {
      lUpdateFlags = (Dem_Cfg_StorageTriggerType)DEM_CFG_TRIGGER_FDC;
    }
  }
# else /* (DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON) */
  lUpdateFlags = (Dem_Cfg_StorageTriggerType)DEM_CFG_TRIGGER_FDC;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif
  return lUpdateFlags;
}
#endif

/* ****************************************************************************
 % Dem_DTC_TestEventEntryExists
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestEventEntryExists(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  boolean lStoredStatus = FALSE;
#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);

  Dem_Cfg_CombinedGroupIterType lCombinedIter;
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      if (Dem_Event_InternalStatus_TestEventStored(lSubEventId) == TRUE)
      {
        lStoredStatus = TRUE;
        break;
      }
    }
  }
  else
#endif
  {
    lStoredStatus = Dem_Event_InternalStatus_TestEventStored(EventId);
  }
  return lStoredStatus;
}

#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_EventCombinationOnStorage_Cleared
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
Dem_DTC_EventCombinationOnStorage_Cleared(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  boolean lReturnValue;
  Dem_Cfg_CombinedGroupIterType lGroupIter;
# if (DEM_CFG_NOTIFY_CLEAR_ALLOWED == STD_ON)
  boolean lClearAllowed;
# endif
  lReturnValue = FALSE;

# if (DEM_CFG_NOTIFY_CLEAR_ALLOWED == STD_ON)
  lClearAllowed = TRUE;

  for (Dem_Cfg_CombinedGroupIterInit(Dem_Cfg_EventCombinedGroup(EventId), &lGroupIter);                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_CombinedGroupIterExists(&lGroupIter) == TRUE;                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_CombinedGroupIterNext(&lGroupIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lSubEventId;
    lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lGroupIter);                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    if (Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
    {
      if ((Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE)
        && (Dem_ClearTask_ClearEventAllowed(lSubEventId) == FALSE))                                                              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      {
        lClearAllowed = FALSE;
        break;
      }
    }
  }
  if (lClearAllowed == TRUE)
# endif
  {
    Dem_DTC_Cleared_Internal(EventId);
    lReturnValue = TRUE;
  }
  /* Always call init monitor callbacks */
  if (Dem_Cfg_EventCombinedGroup(EventId) != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    for (Dem_Cfg_CombinedGroupIterInit(Dem_Cfg_EventCombinedGroup(EventId), &lGroupIter);                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterExists(&lGroupIter) == TRUE;                                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterNext(&lGroupIter))                                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId;
      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lGroupIter);                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      if (Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
      {
        if (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE)
        {
          Dem_Notifications_InitMonitorForEvent(lSubEventId, DEM_INIT_MONITOR_CLEAR);
        }
      }
    }
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_EventCombinationOnRetrieval_Cleared
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_EventCombinationOnRetrieval_Cleared(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  boolean lReturnValue;
  Dem_Cfg_CombinedGroupIterType lGroupIter;
  lReturnValue = FALSE;

  for (Dem_Cfg_CombinedGroupIterInit(Dem_Cfg_EventCombinedGroup(EventId), &lGroupIter);                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_CombinedGroupIterExists(&lGroupIter) == TRUE;                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_CombinedGroupIterNext(&lGroupIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lSubEventId;
    lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lGroupIter);                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    if (Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
    {
      if (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE)
      {
        if(Dem_Event_ProcessCleared(lSubEventId) == TRUE)
        {
          lReturnValue = TRUE;
        }
      }
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CycleQualifyDcyForEachSubevent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcyForEachSubevent(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  Dem_DTC_ForEachCombinedArgPtrType  ArgPtr
  )
{
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
    ArgPtr->Specific.CycleQualifyDcy_Internal.EventQualificationStatus = 0x00;                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif

    /* Process the DTC status qualification of the stored event or combination group */
    Dem_DTC_ForEachCombinedSubEvent(EventId, Dem_Event_CycleQualifyDcy, ArgPtr);                                                 /* SBSW_DEM_CALL_FOREACH_COMBINED_EVENT_FORWARD_ARGUMENT */

# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
    Dem_DTC_SetQualifyStatus(EventId, (Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId) | ArgPtr->Specific.CycleQualifyDcy_Internal.EventQualificationStatus));
# endif
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_DTC_Public
 * \{
 */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CustomizedAgingStart
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CustomizedAgingStart(
  CONSTP2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext
)
{
  if (Dem_Event_InternalStatus_TestEventStored(DTCContext->EventId) == TRUE)
  {
    boolean lIsModified;
    boolean lEventAged = FALSE;
    Dem_DTC_UDSStatusType lOldPreconfirmedStatus;
    Dem_DTC_UDSStatusType lNewPreconfirmedStatus;
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(DTCContext->EventId, TRUE);

    lOldPreconfirmedStatus = Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(lEventEntryIndex);
    lNewPreconfirmedStatus = DEM_DTC_RESET_PC_STATUS_UDTC(lOldPreconfirmedStatus);
    Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus(lEventEntryIndex, lNewPreconfirmedStatus);

    lIsModified = (boolean)(lOldPreconfirmedStatus != lNewPreconfirmedStatus);

    if ( (DEM_DTC_TEST_PC_STATUS_UDTCTOC(lOldPreconfirmedStatus) == FALSE)
      && ((DTCContext->DtcEffects & DEM_CFG_TRIGGER_FIRSTTHISCYCLE) != 0x00U) )
    { /* OCC2 if preconfirmed level had not been reached */
      if (Dem_MemoryEntry_EventEntry_GetOccurrenceCounter2(lEventEntryIndex) < 0xffU)
      {
        Dem_MemoryEntry_EventEntry_SetOccurrenceCounter2(lEventEntryIndex, Dem_MemoryEntry_EventEntry_GetOccurrenceCounter2(lEventEntryIndex) + 1u);
        lIsModified = TRUE;
      }

      /* OCC7 once per cycle, and also consider FDC preconfirmed */
      if (Dem_MemoryEntry_EventEntry_GetOccurrenceCounter7(lEventEntryIndex) < 0xffU)
      {
        Dem_MemoryEntry_EventEntry_SetOccurrenceCounter7(lEventEntryIndex, Dem_MemoryEntry_EventEntry_GetOccurrenceCounter7(lEventEntryIndex) + 1u);
        lIsModified = TRUE;
      }

      /* Start aging at event passed */
      if (Dem_MemoryEntry_EventEntry_GetOccurrenceCounter2(lEventEntryIndex) == Dem_Core_Aging_GetAgingTarget(DTCContext->EventId))
      {
        Dem_DTC_CustomizedAging(DTCContext->EventId);
        lIsModified = TRUE;
        lEventAged = TRUE;
      }
    }

    if (lIsModified == TRUE)
    {
      if ((Dem_Cfg_EventSupportImmediateNv(DTCContext->EventId) == TRUE)
        && (lEventAged == TRUE)
        )
      {
        Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
      }
      else
      {
        Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON */

/* ****************************************************************************
 % Dem_Core_DTC_FindEventFromDtc
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
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Core_DTC_FindEventFromDtc(
  uint32 DTC,
  Dem_DTCFormatType DTCFormat,
  uint16 TranslatedMemoryId
  )
{
  Dem_EventIdType lLowerLimit = DEM_EVENT_INVALID;
  Dem_EventIdType lEventId = DEM_EVENT_INVALID;

  if (Dem_Memories_EventMemory_IsReferredByMemoryId(TranslatedMemoryId) == TRUE)
  {
    lLowerLimit = Dem_FaultMemory_Memories_GetFirstEvent(TranslatedMemoryId);
    lEventId = Dem_FaultMemory_Memories_GetLastEvent(TranslatedMemoryId);
  }
  else 
  if ((DTCFormat != DEM_DTC_FORMAT_J1939)
   && (TranslatedMemoryId == DEM_CFG_MEMORYID_PERMANENT))
  {
    lLowerLimit = Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PERMANENT);
    lEventId = Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PERMANENT);
  }
  else
  {
    /* Limits could not be determined, skip event lookup */
  }

  if (lLowerLimit != DEM_EVENT_INVALID)
  {
    for (; lEventId >= lLowerLimit; --lEventId)
    {
      boolean lDTCFound = FALSE;

      if ( (DTCFormat == DEM_DTC_FORMAT_UDS) 
        && (DTC == Dem_Cfg_EventUdsDtc(lEventId)) )
      {
        lDTCFound = TRUE;
      }
      else
      if ( (DTCFormat == DEM_DTC_FORMAT_OBD_3BYTE) 
        && (DTC == Dem_Core_Event_GetObdOnUds3ByteDtc(lEventId)) )
      {
        lDTCFound = TRUE;
      }
      else
      if ( (DTCFormat == DEM_DTC_FORMAT_J1939) 
        && (DTC == Dem_Cfg_EventJ1939Dtc(lEventId)) )
      {
        lDTCFound = TRUE;
      }
      else
      {
        /* Event does not have a matching DTC */
      }

      if (lDTCFound == TRUE)
      {
        break;
      }
    }

    if (lEventId < lLowerLimit)
    {
      lEventId = DEM_EVENT_INVALID;
    }
  }

  return lEventId;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */

 /* ****************************************************************************
 % Dem_DTC_InvokeInitMonitorCallbacks
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
Dem_DTC_InvokeInitMonitorCallbacks(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  Dem_Cfg_CombinedGroupIndexType lGroupId;

  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    /* DTC has combined group */
    Dem_Cfg_CombinedGroupIterType lCombinedIter;
    Dem_EventIdType lSubEventId;

    /* Iterate through all active sub-events */
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if ((Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)                                                                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE))
      {
        /* Call Init Monitor Callbacks */
        Dem_Notifications_InitMonitorForEvent(lSubEventId, DEM_INIT_MONITOR_RESTART);
      }
    } 
  }
  else
  {
    /* Call Init Monitor for Event Callbacks */
    Dem_Notifications_InitMonitorForEvent(EventId, DEM_INIT_MONITOR_RESTART);
  }

  /* Call Init Monitor for function Callbacks */
  Dem_Notifications_InitMonitorForFunction(EventId);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

 /* ****************************************************************************
 % Dem_DTC_ResetFaultPendingCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_ResetFaultPendingCounter(
  Dem_EventIdType EventId
)
{
  if (Dem_DTC_IsMemIndependentFaultPendingCounterEnabled() == TRUE)
  {
    Dem_Cfg_FaultPendingCounterIndexOfEventTableType lFailedCounterIndex;

    lFailedCounterIndex = Dem_Cfg_GetFaultPendingCounterIndexOfEventTable(EventId);

    if (lFailedCounterIndex != DEM_CFG_FAULTPENDINGCOUNTER_INDEX_INVALID)
    { /* memory independent fault pending counter supported for event */
      Dem_DTC_SetFaultPendingCounter(lFailedCounterIndex, 0);

      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_CYCLECOUNTERDATA),
        DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
}

 /* ****************************************************************************
 % Dem_DTC_GetFailedCycleCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTC_GetFailedCycleCounter(                                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_FailedCycleCounterIndexOfEventTableType FailedCycleCounterIndex
)
{
  uint8 lFailedCycleCounter;

#if (DEM_CFG_DATA_FAILED_CYCLES == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON)
  {
    lFailedCycleCounter = Dem_Cfg_CycleCounterData.FailedCycleCounter[FailedCycleCounterIndex];
  }
#else
  {
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(FailedCycleCounterIndex)                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    lFailedCycleCounter = 0;
  }
#endif
  return lFailedCycleCounter;
}


 /* ****************************************************************************
 % Dem_DTC_GetFaultPendingCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTC_GetFaultPendingCounter(
  Dem_Cfg_FaultPendingCounterIndexOfEventTableType FaultPendingCounterIndex
)
{
  uint8 lFaultPendingCounter;

#if (DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON)
  {
    lFaultPendingCounter = Dem_Cfg_CycleCounterData.FaultPendingCounter[FaultPendingCounterIndex];
  }
#else
  {
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(FaultPendingCounterIndex)                                                                   /* PRQA S 3112 */ /* MD_DEM_14.2 */
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    lFaultPendingCounter = 0;
  }
#endif
  return lFaultPendingCounter;
}

 /* ****************************************************************************
 % Dem_DTC_UpdateMemIndependentCycleCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *

 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_UpdateMemIndependentCycleCounter(
  Dem_EventIdType  EventId,
  Dem_Cfg_StorageTriggerType UpdateFlags
)
{
  if ((UpdateFlags & DEM_CFG_TRIGGER_TESTFAILEDTHISCYCLE) != 0u)
  {
    Dem_DTC_IncrementFailedCycleCounter(EventId);
    Dem_DTC_IncrementFaultPendingCounter(EventId);
  }
}

/* ****************************************************************************
 % Dem_DTC_RetryStorage
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_RetryStorage(
    Dem_EventIdType EventId
  )
{
  boolean lReturn;
  lReturn = (boolean)((Dem_DTC_isRetryStorageEnabled() == TRUE)
    && (Dem_Event_InternalStatus_TestEventStored(EventId) == FALSE));                                                            /* PRQA S 3415 */ /* MD_DEM_13.5_cf */

  return lReturn;
}

/* ****************************************************************************
 % Dem_DTC_Cleared_Internal
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTC_Cleared_Internal(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  Dem_DTC_ForEachCombinedArgType lArgs;
  if (Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE)
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);
    /* Remove the event from the memory */
    Dem_Memories_EventMemory_FreeEntryOfEvent(lEventEntryIndex, EventId);
    Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
  }

  Dem_Memories_TimeSeriesMemory_InvalidateEntryOfEvent(EventId, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
  Dem_Memories_CustomTriggerMemory_ClearEvent(EventId, DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);

  Dem_Memories_ObdFreezeFrameMemory_ClearOBDFreezeFrame(EventId);

  Dem_Core_Aging_ProcessOnClear(EventId);

#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
  Dem_DTC_ForEachCombinedSubEvent(EventId, Dem_Event_Cleared, &lArgs);                                                           /* SBSW_DEM_CALL_FOREACH_COMBINED_EVENT */
#elif (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  Dem_DTC_ProcessOperation(EventId, Dem_Event_Cleared, &lArgs);                                                                  /* SBSW_DEM_CALL_OPERATION_FOR_EVENT */
#else
  Dem_Event_Cleared(EventId, &lArgs);                                                                                            /* SBSW_DEM_POINTER_FORWARD_STACK */
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_DTC_InitCombinedGroupProcessed()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_InitCombinedGroupProcessed(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
VAR(Dem_Cfg_ProcessedCombinedDTCGroupType, AUTOMATIC) ProcessedCombinedDTCGroup
)
{
  uint16_least lBitfieldIterator;
  for (lBitfieldIterator = 0u;
    lBitfieldIterator < DEM_CFG_PROCESSEDCOMBINEDDTCGROUP_SIZE;
    lBitfieldIterator++)
  {
    ProcessedCombinedDTCGroup[lBitfieldIterator] = 0u;                                                                           /* SBSW_DEM_ARRAY_WRITE_PROCESSEDCOMBINEDDTCGROUP */
  }
}

/* ****************************************************************************
 % Dem_DTC_TestCombinedGroupProcessed()
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
Dem_DTC_TestCombinedGroupProcessed(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) GroupId,
  CONST(Dem_Cfg_ProcessedCombinedDTCGroupType, AUTOMATIC) ProcessedCombinedDTCGroup                                              /* PRQA S 3673 */ /* MD_DEM_8.13_Internal */
  )
{
  boolean lGroupProcessed;
  lGroupProcessed = FALSE;
  if (Dem_DTC_IsEventCombinationType2Enabled() == TRUE)
  {
    if (GroupId != DEM_CFG_COMBINED_GROUP_INVALID)
    {
     /* Check whether bit in the corresponding bit field is set */
      lGroupProcessed = Dem_Com_TestBitInBitfield(ProcessedCombinedDTCGroup,                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
                                                  DEM_CFG_PROCESSEDCOMBINEDDTCGROUP_SIZE,
                                                  GroupId);
    }
  }
  return lGroupProcessed;
}


/* ****************************************************************************
 % Dem_DTC_SetCombinedGroupProcessed()
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
Dem_DTC_SetCombinedGroupProcessed(                                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) GroupId,
  VAR(Dem_Cfg_ProcessedCombinedDTCGroupType, AUTOMATIC) ProcessedCombinedDTCGroup
  )
{
  if (Dem_DTC_IsEventCombinationType2Enabled() == TRUE)
  {
    if (GroupId != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      Dem_Com_SetBitInBitfield(ProcessedCombinedDTCGroup,                                                                        /* SBSW_DEM_ARRAY_FORWARD_PROCESSEDCOMBINEDDTCGROUP */
                               DEM_CFG_PROCESSEDCOMBINEDDTCGROUP_SIZE,
                               GroupId);
    }
  }
}


#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CalculateCombinedGroupUdsStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_CalculateCombinedGroupUdsStatus(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  GroupId                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  Dem_DTC_UDSStatusType lReturnValue = 0u;
  Dem_Cfg_CombinedGroupIterType lCombinedIter;

  /* In rare cases GroupId is not required */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  for (Dem_Cfg_CombinedGroupIterInit(GroupId, &lCombinedIter);                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    if ((Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
      && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE))
    {
      lReturnValue = (Dem_DTC_UDSStatusType)(lReturnValue | Dem_Event_UDSStatus_GetInternalEventUdsStatus(lSubEventId));
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CalculateCombinedGroupType2_ExternalUdsStatus
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
DEM_LOCAL FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_CalculateCombinedGroupType2_ExternalUdsStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_DTC_UDSStatusType lCombinedDTCStatus;
  Dem_DTC_UDSStatusType lCombinedSubEventStatus;
  Dem_Cfg_CombinedGroupIterType lCombinedIter;
  Dem_Cfg_CombinedGroupIndexType lGroupId;
# if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
  boolean lWirActive;
# endif

  lCombinedDTCStatus = 0x00U;
  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
# if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
  lWirActive = FALSE;
# endif

  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId;

      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      if ((Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
        && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE))
      {
        lCombinedSubEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(lSubEventId);
        if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
        {
          lCombinedSubEventStatus = (Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(lSubEventId) & lCombinedSubEventStatus);
        }
        lCombinedDTCStatus = (Dem_DTC_UDSStatusType)(lCombinedDTCStatus | lCombinedSubEventStatus);

# if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
        lWirActive = (boolean)(lWirActive
                     || (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_UserControlledWIR) == TRUE));
# endif
      }
    }

# if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
    if (lWirActive == TRUE)
    {
      lCombinedDTCStatus = Dem_Event_UDSStatus_SetBit(lCombinedDTCStatus, Dem_UdsStatus_WIR);
    }
# endif
  }

  return lCombinedDTCStatus;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_DTC_GetQualifyStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_GetQualifyStatus(                                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_DTC_UDSStatusType lDTCQualifyStatus;
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;

  lDTCQualifyStatus = 0x00;
  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);

  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_Cfg_CombinedGroupIterType lCombinedIter;
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId;

      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      if ((Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
        && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE))
      {
        lDTCQualifyStatus = (Dem_DTC_UDSStatusType)(lDTCQualifyStatus | Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(lSubEventId));
      }
    }
  }
  else
#endif
  {
    lDTCQualifyStatus =  Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId);
  }
  return lDTCQualifyStatus;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_DTC_TestStatusForPermanentStorage
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
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_DTC_TestStatusForPermanentStorage(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  boolean lReturnValue;

  lReturnValue = FALSE;

  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
    lReturnValue = (boolean)(Dem_Event_UDSStatus_TestBit(Dem_DTC_GetInternaUDSStatus(EventId), Dem_UdsStatus_WIR) == TRUE);
  }
  else if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    lReturnValue = (boolean)(Dem_GlobalDiagnostics_IsEventAndActivationMode4Active(EventId) == TRUE);
  }
  else
  {
    /* MISRA case */
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_DTC_TestEventStoresPermanentDTC
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
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_DTC_TestEventStoresPermanentDTC(                                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  )
{
  boolean lReturnValue;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lReturnValue = FALSE;

  if (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
  {
    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    {
      lReturnValue = Dem_GlobalDiagnostics_TestEventHasMilAssigned(EventId);
    }
    else if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
    {
      if (Dem_Cfg_EventWwhObdFreezeFrameExists(EventId) == TRUE)
      {
        lReturnValue = (Dem_Core_DTC_IsClassA(EventId) 
          || Dem_Core_DTC_IsClassB1(EventId));                                                                                   /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      }
    }
    else
    {
      /* MISRA case */
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_DTC_TestPfcRequired
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
Dem_DTC_TestPfcRequired(                                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  boolean lReturnValue;

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
#if (DEM_CFG_SUPPORT_CLEAR_PDTC_NO_PFC == STD_ON) && (DEM_CFG_SUPPORT_IUMPR == STD_ON)
    lReturnValue = FALSE;
# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
    {
      Dem_Cfg_CombinedGroupIndexType lGroupId;

      lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
      if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
      {
        Dem_Cfg_CombinedGroupIterType lCombinedIter;
        for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          Dem_EventIdType lSubEventId;
          lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
          if ((Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
            && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE))
          {
            if (Dem_Event_GetRatioIdx(lSubEventId) == DEM_CFG_RATIOINDEX_INVALID)
            {
              lReturnValue = TRUE;
              break;
            }
          }
        }
      }
      else
      {
        if (Dem_Event_GetRatioIdx(EventId) == DEM_CFG_RATIOINDEX_INVALID)
        {
          lReturnValue = TRUE;
        }
      }
    }
# else
    /* If event has a ratio attached, it can be cleared without respect the PFC cycle. So set PFC cycle to fulfilledPerm */
    if (Dem_Event_GetRatioIdx(EventId) == DEM_CFG_RATIOINDEX_INVALID)
    {
      lReturnValue = TRUE;
    }
# endif
#else
   lReturnValue = TRUE;
#endif
  }
  else
  {
    lReturnValue = TRUE;
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_DTC_TestDtcAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestDtcAvailable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;
  if (Dem_Cfg_EventIsCombined(EventId) == TRUE)
  {
    lReturnValue = Dem_DTC_TestGroupAvailable(Dem_Cfg_EventCombinedGroup(EventId));
  }
  else
  {
    lReturnValue = Dem_Event_TestEventRuntimeAvailable(EventId);
  }
  return lReturnValue;
}

/* ****************************************************************************
% Dem_DTC_TestSuppressionByRepresentiveEvent
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestSuppressionByRepresentiveEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  boolean lReturnValue;
  if (Dem_Event_CombinedStorage_IsSupported() == TRUE)
  {
    lReturnValue = Dem_DTC_TestSuppressionOnDtcRetrieval(EventId);
  }
  else
  {
    lReturnValue = Dem_Event_TestSuppressionOnEventRetrieval(EventId);
  }
  return lReturnValue;
}


/* ****************************************************************************
 % Dem_DTC_TestDtcAvailableInVariant
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestDtcAvailableInVariant(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;
  if (Dem_Cfg_EventIsCombined(EventId) == TRUE)
  {
    lReturnValue = Dem_DTC_TestGroupAvailableInVariant(Dem_Cfg_EventCombinedGroup(EventId));
  }
  else
  {
    lReturnValue = Dem_Cfg_EventAvailableByVariant(EventId);
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_DTC_TestDtcSuppression
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestDtcSuppression(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;

  if (Dem_Cfg_EventIsCombined(EventId) == TRUE)
  {
    lReturnValue = Dem_DTC_TestGroupSuppression(Dem_Cfg_EventCombinedGroup(EventId));
  }
  else
  {
    lReturnValue = Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(EventId), Dem_InternalStatus_SuppressedDTC);
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_DTC_TestSuppressionOnDtcRetrieval
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestSuppressionOnDtcRetrieval(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;
  lReturnValue = (boolean)
     ( (Dem_DTC_TestDtcSuppression(EventId) == TRUE)
    || (Dem_DTC_TestDtcAvailableInVariant(EventId) == FALSE)
    || ( (Dem_DTC_TestDtcAvailable(EventId) == FALSE)
      && (Dem_DTC_TestEventEntryExists(EventId) == FALSE) )                                                                      /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
     );
  return lReturnValue;
}

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_DTC_TestRemainingEventforDtc
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
Dem_DTC_TestRemainingEventforDtc(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  boolean lReturnValue;
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;
  Dem_DTC_InternalStatusType lInternalStatus;
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif

# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
  lReturnValue = FALSE;
  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);

  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_Cfg_CombinedGroupIterType lCombinedIter;

    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId;
      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(lSubEventId);
      if ( (lSubEventId != EventId)
        && (Dem_Event_InternalStatus_TestBit(lInternalStatus, Dem_InternalStatus_Disconnected) == FALSE) )
      {
        lReturnValue = TRUE;
        break;
      }
    }
  }
  else
# endif
  {
    lReturnValue = FALSE;
  }
  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_DTC_ResetFdcToc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_ResetFdcToc(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
#if (DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON)
  Dem_DTC_InternalStatusType lInternalStatus;
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();

  lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
  lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_FDCTOC);
  Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);

  Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_ResetDtcSuppression
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
Dem_DTC_ResetDtcSuppression(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;
  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_DTC_InternalStatusType lInternalStatus;
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    lInternalStatus = Dem_Event_InternalStatus_GetCombinedGroupStatus(lGroupId);
    lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_SuppressedDTC);
    Dem_Event_InternalStatus_SetCombinedGroupStatus(lGroupId, lInternalStatus);
    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
  }
  else
# endif
  {
    Dem_DTC_InternalStatusType lInternalStatus;
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
    lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_SuppressedDTC);
    Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);
    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON) */

#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_ChangeDtcSuppression
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_DTC_ChangeDtcSuppression(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  SuppressionStatus
  )
{
  Std_ReturnType lReturnValue;

  lReturnValue = E_NOT_OK;

  if (EventId != DEM_EVENT_INVALID)
  {
    if (SuppressionStatus == TRUE)
    {
      Dem_DTC_SetDtcSuppression(EventId);
    }
    else
    {
      Dem_DTC_ResetDtcSuppression(EventId);
    }

    Dem_Dtr_DTCSuppressionChanged(EventId);
    lReturnValue = E_OK;
  }

  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_DTC_GetInternaUDSStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_GetInternaUDSStatus(
  Dem_EventIdType EventId
  )
{
  Dem_DTC_UDSStatusType lDtcStatus;
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    lDtcStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(lGroupId);
  }
  else
#endif
  {
    lDtcStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  }
  return Dem_DTC_ApplyCombinedStatus(lDtcStatus);
}

/* ****************************************************************************
% Dem_DTC_GetExternalUDSStatus
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_GetExternalUDSStatus(
  Dem_EventIdType  EventId
)
{
  return Dem_DTC_GenerateExternalOnlyStatus(EventId, Dem_DTC_GetInternaUDSStatus(EventId));
}

/* ****************************************************************************
 % Dem_DTC_GetFaultDetectionCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_DTC_GetFaultDetectionCounter(                                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(sint8, AUTOMATIC, DEM_APPL_DATA)  FaultDetectionCounter
  )
{
  Std_ReturnType lGetFdcReturnValue;
# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;
  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    lGetFdcReturnValue = Dem_DTC_CombinedGroupGetFDC(lGroupId, FaultDetectionCounter);                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else
# endif
  {
    lGetFdcReturnValue = Dem_GetFaultDetectionCounter(EventId, FaultDetectionCounter);                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }

  return lGetFdcReturnValue;
}

/* ****************************************************************************
 % Dem_DTC_ApplyCombinedStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_ApplyCombinedStatus(
  CONST(Dem_DTC_UDSStatusType, AUTOMATIC)  DtcStatus
  )
{
  Dem_DTC_UDSStatusType lReturnValue;
  lReturnValue = DtcStatus;
# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  if (Dem_Event_UDSStatus_TestBit(lReturnValue, Dem_UdsStatus_TFSLC) == TRUE)
  {
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_TNCSLC);
  }
  if (Dem_Event_UDSStatus_TestBit(lReturnValue, Dem_UdsStatus_TFTOC) == TRUE)
  {
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_TNCTOC);
  }
# endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_DTC_GenerateExternalOnlyStatus
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
DEM_LOCAL FUNC(Dem_DTC_UDSStatusType, DEM_CODE)
Dem_DTC_GenerateExternalOnlyStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONST(Dem_DTC_UDSStatusType, AUTOMATIC)  DtcStatus
  )
{
  Dem_DTC_UDSStatusType lExternalDTCStatus;
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;
#endif

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
    lExternalDTCStatus = Dem_DTC_GenerateExternalOnlyType1CombinedStatus(EventId, DtcStatus);
#else
    lExternalDTCStatus = Dem_DTC_GenerateExternalOnlyType2CombinedStatus(EventId, DtcStatus);
# endif
  }
  else
#endif
  {
    lExternalDTCStatus = DtcStatus;

    /* EventId is not a combined event */
    if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
    {
      lExternalDTCStatus = (Dem_DTC_UDSStatusType)(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId) & lExternalDTCStatus);
    }
    lExternalDTCStatus = (Dem_DTC_UDSStatusType)(Dem_DTC_ApplyUserControlledWIR(lExternalDTCStatus, Dem_Event_InternalStatus_GetEventStatus(EventId)));
  }
  return lExternalDTCStatus;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_DTC_ProcessAgingFinished
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
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_ProcessAgingFinished(                                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  boolean SuppressNotifications
)
{
  Dem_DTC_UDSStatusType lOldExternalDtcUdsStatus;
  Dem_DTC_ForEachCombinedArgType lArgs;
  boolean lIsCDTCReset = FALSE;

  Dem_DTC_ResetFailedCycleCounter(EventId);
  Dem_DTC_ResetFaultPendingCounter(EventId);

  /* Set SI30 AgedDTC bit */
  Dem_Core_StatusIndicator_UpdateOnEventAged(EventId);

#if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
  if (EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    lArgs.Specific.Event_Aged.IsAgingOnlyEntry = Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry(EventEntryIndex);
  }
  else
  {
    lArgs.Specific.Event_Aged.IsAgingOnlyEntry = FALSE;
  }
#endif

  /* Process aging for each sub-event status */
  lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(EventId);
  lArgs.Specific.Event_Aged.SuppressNotifications = SuppressNotifications;

# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_OFF)
  Dem_DTC_ForEachCombinedSubEvent(EventId, Dem_Event_Aged, &lArgs);                                                              /* SBSW_DEM_CALL_FOREACH_COMBINED_EVENT */
# else
  Dem_DTC_ProcessOperation(EventId, Dem_Event_Aged, &lArgs);                                                                     /* SBSW_DEM_CALL_OPERATION_FOR_EVENT */
# endif

  /* Store in a variable if CDTC was set and is now reset in event status */
  lIsCDTCReset = (Dem_Event_UDSStatus_TestBit(lArgs.Common.OldDtcStatus, Dem_UdsStatus_CDTC) == TRUE)
              && (Dem_Event_UDSStatus_TestBit(lArgs.Common.NewDtcStatus, Dem_UdsStatus_CDTC) == FALSE);

  if (Dem_DTC_TestEventStoresPermanentDTC(EventId) == TRUE)
  {
    /* For event combination type 2, clear entry from permanent chronology
        only if the Confirmed DTC bit is reset in the DTC status */
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    if (lIsCDTCReset == TRUE)
# endif
    {
      /* CDTC bit is always reset on aging -> free unconditionally */
      Dem_Memories_PermanentMemory_ConfirmedChrono_FreeEvent(Dem_Cfg_GetMasterEvent(EventId));
    }
  }

  if (lIsCDTCReset == TRUE)
  {
    Dem_GlobalDiagnostics_DecrementConfirmedEventsIfNeeded(EventId);
    Dem_GlobalDiagnostics_OdometerData_ProcessConfirmedDTCChanged();
    Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_ProcessConfirmedDTCChanged();
  }

  if (EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    /* An event entry exists: It can only be in state AGED, if feature retain memory entry is enabled and the entry is not an aging only entry */
    if ((Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry(EventEntryIndex) == TRUE) || (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_OFF))
    {
      Dem_Memories_EventMemory_FreeEntryOfEvent(EventEntryIndex, EventId);
    }

    /* Only if event entry exists its block state can be 'dirty immediate'. Otherwise the block state remains 'dirty' as set in Dem_Event_Aged
        (or when setting the aging counter to AGED if it is stored in the event entry. */
    if (Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE)
    {
      Dem_Nvm_SetEventEntryNvBlockState(EventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }

    /* If configured, notify the DTC status change */
    if ((Dem_InitState_TestMasterInState(DEM_INITSTATE_INITIALIZED) == TRUE)
      && (SuppressNotifications == FALSE))
    {
      Dem_Notifications_DtcStatusChanged(EventId,
        lOldExternalDtcUdsStatus,
        Dem_DTC_GenerateExternalOnlyStatus(EventId, lArgs.Common.NewDtcStatus),
        DEM_NOTIFICATIONS_DTC_AGED);
    }
  }

#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
  /* For event combination type 2, increment the Aged Counter only if the
      Confirmed DTC bit is reset in the DTC status */
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  if (lIsCDTCReset == TRUE)
# endif
  {
    Dem_DTC_IncrementAgedCounter(EventId);
  }
#endif

  Dem_Memories_TimeSeriesMemory_InvalidateEntryOfEvent(EventId, DEM_NVM_BLOCKSTATE_DIRTY);
  Dem_Memories_CustomTriggerMemory_ClearEvent(EventId, DEM_NVM_BLOCKSTATE_DIRTY);
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON) || (DEM_CFG_SUPPORT_TRIGGER_PASSED == STD_ON))
/* ****************************************************************************
 % Dem_DTC_Passed_CalcDTCEffects
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
  Dem_DTC_Passed_CalcDTCEffects(
  CONSTP2VAR(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext,
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventEffects
  )
{
#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
  if (Dem_Cfg_EventCombinedGroup(DTCContext->EventId) != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_DTC_UDSStatusType lCurrentInternalDtcUdsStatus = Dem_DTC_GetInternaUDSStatus(DTCContext->EventId);
    if ( (Dem_Event_UDSStatus_TestBit(DTCContext->OldDtcStatus, Dem_UdsStatus_TF) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(lCurrentInternalDtcUdsStatus, Dem_UdsStatus_TF) == FALSE))
    {
      DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_PASSED);                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    if ( (Dem_Event_UDSStatus_TestBit(DTCContext->OldDtcStatus, Dem_UdsStatus_TNCTOC) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(lCurrentInternalDtcUdsStatus, Dem_UdsStatus_TNCTOC) == FALSE))
    {
      DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_FIRSTTHISCYCLE);            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  else
#endif
  {
    DTCContext->DtcEffects = EventEffects;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}
#endif

/* ****************************************************************************
% Dem_DTC_InitMemIndependentCycleCounterDataNv
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_InitMemIndependentCycleCounterDataNv(
  void
)
{
  if (Dem_DTC_IsMemIndependentCycleCounterEnabled() == TRUE)
  {
# if (DEM_CFG_DATA_FAILED_CYCLES == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON)
    Dem_MemSet((Dem_DataPtrType)&(Dem_Cfg_CycleCounterData.FailedCycleCounter[0]),                                               /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_ARRAY_WRITE_CYCLECOUNTERDATA_FAILEDCYCLECOUNTER */
      0x00u,
      DEM_CFG_MAX_NUMBER_FAILED_CYCLE_DTCS * sizeof(Dem_Cfg_CycleCounterData.FailedCycleCounter[0]));
# endif

# if (DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON)
    Dem_MemSet((Dem_DataPtrType)&(Dem_Cfg_CycleCounterData.FaultPendingCounter[0]),                                              /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_MEMSET_CYCLECOUNTERDATA_FAULTPENDINGCOUNTER */
      0x00u,
      DEM_CFG_MAX_NUMBER_FAULT_PENDING_DTCS * sizeof(Dem_Cfg_CycleCounterData.FaultPendingCounter[0]));
# endif
  }
}

/* ****************************************************************************
 % Dem_DTC_Cleared
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_Cleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;
  Dem_DTC_UDSStatusType lOldInternalDtcUdsStatus;
  Dem_DTC_UDSStatusType lNewInternalDtcUdsStatus;
  Dem_DTC_UDSStatusType lOldExternalDtcUdsStatus;
  Dem_DTC_UDSStatusType lNewExternalDtcUdsStatus;

  Dem_Event_DataUpdateStart(EventId);
  lOldInternalDtcUdsStatus = Dem_DTC_GetInternaUDSStatus(EventId);
  lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(EventId);
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  if (Dem_Cfg_EventCombinedGroup(EventId) != DEM_CFG_COMBINED_GROUP_INVALID)
  {
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
    lReturnValue = Dem_DTC_EventCombinationOnStorage_Cleared(EventId);
# else
    lReturnValue = Dem_DTC_EventCombinationOnRetrieval_Cleared(EventId);
# endif
  }
  else
#endif
  {
    lReturnValue = Dem_Event_ProcessCleared(EventId);
  }
  lNewInternalDtcUdsStatus = Dem_DTC_GetInternaUDSStatus(EventId);
  lNewExternalDtcUdsStatus = Dem_DTC_GenerateExternalOnlyStatus(EventId, lNewInternalDtcUdsStatus);

  if (Dem_DTC_TestEventStoresPermanentDTC(EventId) == TRUE)
  {
    Dem_Memories_PermanentMemory_ClearEvent(EventId, TRUE);

    /*CDTC bit could be reset already*/
    if ((Dem_Event_UDSStatus_TestBit(lOldInternalDtcUdsStatus, Dem_UdsStatus_CDTC) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(lNewInternalDtcUdsStatus, Dem_UdsStatus_CDTC) == FALSE))
    {
      Dem_Memories_PermanentMemory_ConfirmedChrono_FreeEvent(EventId);
    }
  }

  Dem_Notifications_InitMonitorForFunction(EventId);

  if (lReturnValue == TRUE)
  {
    Dem_DTC_ResetFailedCycleCounter(EventId);
    Dem_DTC_ResetFaultPendingCounter(EventId);
    Dem_Core_StatusIndicator_ResetSI30byte(EventId);
    Dem_Core_Event_SetEntryIndependentMaxFdcSlc(EventId, 0);

  if ((Dem_Event_UDSStatus_TestBit(lOldInternalDtcUdsStatus, Dem_UdsStatus_CDTC) == TRUE) 
    && (Dem_Event_UDSStatus_TestBit(lNewInternalDtcUdsStatus, Dem_UdsStatus_CDTC) == FALSE))
  {
    Dem_GlobalDiagnostics_DecrementConfirmedEventsIfNeeded(EventId);
    Dem_GlobalDiagnostics_OdometerData_ProcessConfirmedDTCChanged();
    Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_ProcessConfirmedDTCChanged();
  }

#if (DEM_CFG_DATA_AGED_COUNTER == STD_ON)
    Dem_DTC_ResetAgedCounter(EventId);
#endif

    Dem_Notifications_DtcStatusChanged(EventId,
      lOldExternalDtcUdsStatus,
      lNewExternalDtcUdsStatus,
      DEM_NOTIFICATIONS_DTC_CLEAR);
  }

  Dem_Event_DataUpdateFinish(EventId);
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
/* ****************************************************************************
 % Dem_DTC_Displaced
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

 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_Displaced(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_DTC_UDSStatusType lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(EventId);
  Dem_DTC_ForEachCombinedArgType lArgs;
  boolean lIsCDTCReset = FALSE;

# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_OFF)
  Dem_DTC_ForEachCombinedSubEvent(EventId, Dem_Event_Displaced, &lArgs);                                                         /* SBSW_DEM_CALL_FOREACH_COMBINED_EVENT */
# else
  Dem_DTC_ProcessOperation(EventId, Dem_Event_Displaced, &lArgs);                                                                /* SBSW_DEM_CALL_OPERATION_FOR_EVENT */
# endif

  /* Store in a variable if CDTC was set and is now reset in event status */
  lIsCDTCReset = (Dem_Event_UDSStatus_TestBit(lArgs.Common.OldDtcStatus, Dem_UdsStatus_CDTC) == TRUE)
              && (Dem_Event_UDSStatus_TestBit(lArgs.Common.NewDtcStatus, Dem_UdsStatus_CDTC) == FALSE);

  if (lIsCDTCReset == TRUE)
  {
# if (DEM_CFG_PROCESS_CDTC_STOREDONLY == STD_ON)
    if (Dem_DTC_TestEventStoresPermanentDTC(EventId) == TRUE)
    {
      Dem_EventIdType lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);
      Dem_Memories_PermanentMemory_ConfirmedChrono_FreeEvent(lMasterEventId);
    }
# endif
    Dem_GlobalDiagnostics_DecrementConfirmedEventsIfNeeded(EventId);
    Dem_GlobalDiagnostics_OdometerData_ProcessConfirmedDTCChanged();
    Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_ProcessConfirmedDTCChanged();
  }

  Dem_Core_Aging_ProcessOnDisplaced(EventId);
  /* Call status changed notifications (there is always a change) */
  Dem_Notifications_DtcStatusChanged(EventId,
                                     lOldExternalDtcUdsStatus,
                                     Dem_DTC_GenerateExternalOnlyStatus(EventId, lArgs.Common.NewDtcStatus),
                                     DEM_NOTIFICATIONS_DTC_DISPLACED);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CustomizedDisplacement
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
Dem_DTC_CustomizedDisplacement(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  SI30
  )
{
  Dem_DTC_UDSStatusType lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(EventId);
  Dem_DTC_ForEachCombinedArgType lArgs;

  lArgs.Specific.Event_CustomizedDisplacement_Internal.SI30 = SI30;
  Dem_DTC_ForEachCombinedSubEvent(EventId, Dem_Event_CustomizedDisplacement, &lArgs);                                            /* SBSW_DEM_CALL_FOREACH_COMBINED_EVENT */
  /* Call status changed notifications (there is always a change) */
  Dem_Notifications_DtcStatusChanged(EventId,
                                     lOldExternalDtcUdsStatus,
                                     Dem_DTC_GenerateExternalOnlyStatus(EventId, lArgs.Common.NewDtcStatus),
                                     DEM_NOTIFICATIONS_DTC_DISPLACED);
}
#endif

/* ****************************************************************************
 % Dem_DTC_InitDTCContext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_InitDTCContext(
CONST(Dem_EventIdType, AUTOMATIC)  EventId,
CONSTP2VAR(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext
)
{
  DTCContext->EventId = EventId;                                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  DTCContext->OldDtcStatus = Dem_DTC_GetInternaUDSStatus(EventId);                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  DTCContext->OldExternalDtcStatus = Dem_DTC_GetExternalUDSStatus(EventId);                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  DTCContext->DtcEffects = DEM_CFG_TRIGGER_NONE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#endif
  DTCContext->QualifyDTC = FALSE;                                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_DTC_CalculateFailedEffects
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
Dem_DTC_CalculateFailedEffects(
CONSTP2VAR(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext,
CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC)  EventEffects
)
{
#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
  if (Dem_Cfg_EventCombinedGroup(DTCContext->EventId) != DEM_CFG_COMBINED_GROUP_INVALID)
  {
# if (DEM_CFG_SUPPORT_RETRY_STORAGE == STD_ON)
    if (Dem_Event_InternalStatus_TestEventStored(DTCContext->EventId) == FALSE)
    {
      DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_TESTFAILED);                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_TESTFAILEDTHISCYCLE);       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_FIRSTTHISCYCLE);            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
# endif
    {
      if (Dem_Event_UDSStatus_TestBit(DTCContext->OldDtcStatus, Dem_UdsStatus_TF) == FALSE)
      {
        DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_TESTFAILED);              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
      if (Dem_Event_UDSStatus_TestBit(DTCContext->OldDtcStatus, Dem_UdsStatus_TFTOC) == FALSE)
      {
        DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_TESTFAILEDTHISCYCLE);     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_FIRSTTHISCYCLE);          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }
  }
  else
#endif
  {
    DTCContext->DtcEffects = EventEffects;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_DTC_CalculateFailedEffects_OnStorage
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
Dem_DTC_CalculateFailedEffects_OnStorage(
CONSTP2VAR(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext,
CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC)  EventEffects
)
{
#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
   if (Dem_Cfg_EventCombinedGroup(DTCContext->EventId) != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    if((EventEffects & DEM_CFG_TRIGGER_PENDING) != 0u)
    {
      if ((Dem_Event_UDSStatus_TestBit(DTCContext->OldDtcStatus, Dem_UdsStatus_PDTC) == FALSE)
# if (DEM_CFG_SUPPORT_RETRY_STORAGE == STD_ON) && (DEM_CFG_PROCESS_PDTC_ALL_DTC == STD_ON)
        || (Dem_Event_InternalStatus_TestEventStored(DTCContext->EventId) == FALSE)                                              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
# endif
        )
      {
        DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_PENDING);                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }

    if((EventEffects & DEM_CFG_TRIGGER_CONFIRMED) != 0u)
    {
      if ( (Dem_Event_UDSStatus_TestBit(DTCContext->OldDtcStatus, Dem_UdsStatus_CDTC) == FALSE)
# if (DEM_CFG_SUPPORT_RETRY_STORAGE == STD_ON) && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
        || (Dem_Event_InternalStatus_TestEventStored(DTCContext->EventId) == FALSE)                                              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
# endif
        )
      {
        DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_CONFIRMED);               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }

    if((EventEffects & DEM_CFG_TRIGGER_INDICATOR) != 0u)
    {
      if (Dem_Event_UDSStatus_TestBit(DTCContext->OldDtcStatus, Dem_UdsStatus_WIR) == FALSE)
      {
        DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_CFG_TRIGGER_INDICATOR);               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }
  }
  else
#endif
  {
    DTCContext->DtcEffects = EventEffects;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}                                                                                                                                /* PRQA S 6010 */ /* MD_MSR_STPTH */

/* ****************************************************************************
 % Dem_DTC_ProcessFailedEffects
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_ProcessFailedEffects(
CONSTP2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext
)
{
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
   Dem_EnterCritical_DiagMonitor();

   if (Dem_Event_InternalStatus_GetStoredStatus(DTCContext->EventId) == Dem_InternalStatus_StoredStatus_Aging)
   {
     Dem_Event_InternalStatus_SetStoredStatus(DTCContext->EventId, Dem_InternalStatus_StoredStatus_Active);
   }
    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
}

/* ****************************************************************************
 % Dem_DTC_Passed_ProcessDTCEffects
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_Passed_ProcessDTCEffects(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext,                                                          /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  CONSTP2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext
)
{
  Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC(EventContext);                                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
#if ((DEM_CFG_SUPPORT_TRIGGER_PASSED == STD_ON) && (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF))
  if ((Dem_Cfg_EventUdsDtc(DTCContext->EventId) != DEM_CFG_DTC_UDS_INVALID)
    && (Dem_GlobalDiagnostics_TestEventStorageConditionsSatisfied(DTCContext->EventId) == TRUE))
  {
    Dem_InternalStatus_StoredStatusType lStoredStatus = Dem_Event_InternalStatus_GetStoredStatus(DTCContext->EventId);

    if ( (lStoredStatus == Dem_InternalStatus_StoredStatus_Active)
      || (lStoredStatus == Dem_InternalStatus_StoredStatus_Aging))
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(DTCContext->EventId, TRUE);
#if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
      if (Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry(lEventEntryIndex) == FALSE)
#endif
      {
        EventContext->DataModified = DEM_DATA_TEST_USERDATA_CHANGED(                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
          Dem_FaultMemory_StorageManager_UpdateEventEntryOnPassed(DTCContext->EventId, DTCContext->DtcEffects, lEventEntryIndex));

        if (EventContext->DataModified == TRUE)
        {
          if (Dem_Cfg_EventSupportImmediateNv(DTCContext->EventId) == TRUE)
          {
            Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
          }
          else
          {
            Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
          }
        }
      }
    }
  }
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
    Dem_Memories_PrestorageMemory_StateSetDiscard();
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DTCContext);
  DEM_IGNORE_UNUSED_ARGUMENT(EventContext->DataModified)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_DTC_ProcessStorage
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_ProcessStorage(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext,
  CONSTP2VAR(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext
  )
{
  boolean lIsOccurrence = FALSE;
  uint8 lBlockModification = DEM_DATA_NO_CHANGES;
  Dem_Cfg_EntryIndexType lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;

  if (Dem_Event_InternalStatus_TestEventStored(DTCContext->EventId) == FALSE)
  { /* Not stored yet - If no entry can be created we need to discard some updates */
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
    /* Storage was not possible, discard changes to the status byte */
    EventContext->EventEffects = DEM_CFG_TRIGGER_NONE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    DTCContext->DtcEffects = DEM_CFG_TRIGGER_NONE;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#else
    if (Dem_Memories_EventMemory_TestStorageTriggerIsFulfilled(DTCContext->EventId, DTCContext->DtcEffects) == TRUE)
    { /* We have to store this event in memory */
      lEventEntryIndex = Dem_DTC_EventCreateMemory(DTCContext->EventId);
    }
    if (lEventEntryIndex == DEM_CFG_ENTRYINDEX_INVALID)
    { /* Storage was not possible, discard changes that require storage */
      EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects & DEM_ESM_EVENT_RESET_TRIGGER_MASK);  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects & DEM_ESM_EVENT_RESET_TRIGGER_MASK);          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_ESM_EVENT_CATCHUP_TRIGGER);     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_ESM_EVENT_CATCHUP_TRIGGER);             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

      Dem_DTC_SetFdcToc(DTCContext->EventId);
    }
#endif
  }
  else
  { /* Already stored - Update the existing entry if necessary */

    lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(DTCContext->EventId, TRUE);

    if (Dem_MemoryEntry_EventEntry_TestUpdatable(DTCContext->EventId, lEventEntryIndex) == FALSE)
    {
      if (Dem_Memories_EventMemory_TestStorageTriggerIsFulfilled(DTCContext->EventId, DTCContext->DtcEffects) == TRUE)
      { /* Reclaim event entry */
        Dem_DTC_EventEntryReInit(DTCContext->EventId, lEventEntryIndex);

        EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_ESM_EVENT_CATCHUP_TRIGGER);   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        DTCContext->DtcEffects = (Dem_Cfg_StorageTriggerType)(DTCContext->DtcEffects | DEM_ESM_EVENT_CATCHUP_TRIGGER);           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

        lIsOccurrence = TRUE;
      }
      else
      {
        if (Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry(lEventEntryIndex) == TRUE)
        {
          Dem_DTC_FreeEventAgingOnly(DTCContext->EventId, lEventEntryIndex);
        }
        lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
      }
    }
  }

  if (Dem_Core_DTC_IsObdRelated(DTCContext->EventId) == TRUE)
  { /* store OBD Freeze Frame depending on DTC triggers, no event entry required */
    Dem_FaultMemory_StorageManager_TriggerObdFreezeFrameStorage(DTCContext);                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    if ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE) && (DTCContext->QualifyDTC == TRUE))
    {
      /* Qualify status of event has to be updated */
      EventContext->QualifyEvent = TRUE;                                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }

  if (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
#if (DEM_CFG_USE_NVM == STD_ON) && (DEM_CFG_SUPPORT_NVM_WRITELIMIT == STD_ON)
    uint16 lOldOccurrenceCount;
#endif

    if ((DTCContext->DtcEffects & DEM_ESM_EVENT_OCCURRENCE_TRIGGER) != 0u)
    {
      lIsOccurrence = TRUE;
    }
    if (lIsOccurrence == TRUE)
    {
      /* event storage or update trigger matched */
      Dem_Memories_EventMemory_UpdateChronology(DTCContext->EventId, lEventEntryIndex);
      lBlockModification |= DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
    }

#if (DEM_CFG_USE_NVM == STD_ON) && (DEM_CFG_SUPPORT_NVM_WRITELIMIT == STD_ON)
    lOldOccurrenceCount = Dem_MemoryEntry_EventEntry_GetOccurrenceCounter(lEventEntryIndex);
#endif
#if (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON)
    /* initialize the CyclesSinceFirstFailed counter when not done yet, e.g. due event entry allocation via FDC */
    if (Dem_MemoryEntry_EventEntry_GetCyclesSinceFirstFailed(lEventEntryIndex) == DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID)
    {
      Dem_MemoryEntry_EventEntry_SetCyclesSinceFirstFailed(lEventEntryIndex, Dem_OperationCycle_GetCycleCounter(Dem_Cfg_EventOperationCycle(DTCContext->EventId)));
      lBlockModification |= DEM_DATA_SET_STATISTICS_CHANGED(lBlockModification);
    }
#endif

    if (Dem_Core_DTC_IsObdRelated(DTCContext->EventId) == TRUE)
    { /* OBD related Event, store Freeze Frame */
      lBlockModification |= Dem_FaultMemory_StorageManager_TriggerWwhObdFreezeFrameStorage(DTCContext->EventId, DTCContext->DtcEffects, lEventEntryIndex);
    }
    lBlockModification |= Dem_FaultMemory_StorageManager_UpdateEventEntryOnFailed(DTCContext->EventId, DTCContext->DtcEffects, lEventEntryIndex);

    if ((DEM_DATA_TEST_USERDATA_CHANGED(lBlockModification)
      || (lIsOccurrence == TRUE))
      && (Dem_Cfg_EventSupportImmediateNv(DTCContext->EventId) == TRUE))                                                         
    {
      /* If the event is configured for immediate NV updates, set the entries NV state to immediate update */
#if (DEM_CFG_USE_NVM == STD_ON) && (DEM_CFG_SUPPORT_NVM_WRITELIMIT == STD_ON)
      uint16 lNewOccurrenceCount = Dem_MemoryEntry_EventEntry_GetOccurrenceCounter(lEventEntryIndex);
      Dem_Nvm_TestAndSetBlockStateImmediate(Dem_MemoryEntry_EventEntry_GetEntryIndexToNvBlockIndex(lEventEntryIndex), lOldOccurrenceCount, lNewOccurrenceCount);
#else
    Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
#endif
    }
    else
    {
      if (lBlockModification != DEM_DATA_NO_CHANGES)
      {
        /* Otherwise set the entries NV state to 'DIRTY' */
        Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
      }
      else
      {
        /* MISRA */
      }
    }
    EventContext->DataModified = (boolean)DEM_DATA_TEST_USERDATA_CHANGED(lBlockModification);                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }

  /* store time series snapshot records if storage trigger is fulfilled */
  if ((Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(DTCContext->EventId) == TRUE)
    && (Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled() == FALSE))
  {
    Dem_BitField256Bits lSeriesTriggeredFlags;
    boolean lStorageTriggered = Dem_Memories_TimeSeriesMemory_SetBitsForTriggeredSeries(&lSeriesTriggeredFlags, DTCContext);     /* SBSW_DEM_POINTER_FORWARD_STACK */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    if(lStorageTriggered)
    {
      Dem_Memories_TimeSeriesMemory_StoreEntry(EventContext->EventId, &lSeriesTriggeredFlags);                                   /* SBSW_DEM_POINTER_FORWARD_STACK */
    }
  }
}                                                                                                                                /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && \
    ((DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON) || (DEM_CFG_SUPPORT_TRIGGER_FDC== STD_ON) || (DEM_CFG_STORAGE_AT_FDC == STD_ON))
/* ****************************************************************************
 % Dem_DTC_FdcTrip
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_FdcTrip(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  Dem_Cfg_EntryIndexType lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;

  Dem_Event_DataUpdateStart(EventId);
  if (Dem_Event_InternalStatus_TestEventStored(EventId) == FALSE)
  {
    if (Dem_Memories_EventMemory_TestStorageTriggerIsOnFdc(EventId) == TRUE)
    {
      /* FdcTrip is the storage trigger and the event is not yet stored */
      lEventEntryIndex = Dem_DTC_EventCreateMemory(EventId);

      /* store time series snapshot records when event gets stored */
      if ( (Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(EventId) == TRUE)
        && ((lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID))
        && (Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled() == TRUE))
      {
        Dem_BitField256Bits lSeriesTriggeredFlags;
        Dem_Memories_TimeSeriesMemory_SetAllBits(&lSeriesTriggeredFlags);                                                        /* SBSW_DEM_POINTER_FORWARD_STACK */
        Dem_Memories_TimeSeriesMemory_StoreEntry(EventId, &lSeriesTriggeredFlags);                                               /* SBSW_DEM_POINTER_FORWARD_STACK */
      }
    }
  }
  else
  {
    lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);

    if (Dem_MemoryEntry_EventEntry_TestUpdatable(EventId, lEventEntryIndex) == FALSE)
    {
      if (Dem_Memories_EventMemory_TestStorageTriggerIsOnFdc(EventId) == TRUE)
      {
         /* Storage trigger reached - reclaim event entry */
         Dem_DTC_EventEntryReInit(EventId, lEventEntryIndex);

         /* Store time series snapshot records when event gets re-stored - 
             Time Series entry is not retained in memory - no reinit required*/
         if ( (Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(EventId) == TRUE)
           && (Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled() == TRUE))
         {
           Dem_BitField256Bits lSeriesTriggeredFlags;
           Dem_Memories_TimeSeriesMemory_SetAllBits(&lSeriesTriggeredFlags);                                                     /* SBSW_DEM_POINTER_FORWARD_STACK */
           Dem_Memories_TimeSeriesMemory_StoreEntry(EventId, &lSeriesTriggeredFlags);                                            /* SBSW_DEM_POINTER_FORWARD_STACK */
         }
      }
      else
      {
        lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
      }
    }
  }

  if (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    uint8 lBlockModification;
    Dem_Cfg_StorageTriggerType lUpdateFlags;

    Dem_Core_Aging_ProcessOnFdcTrip(EventId);

    lUpdateFlags = Dem_DTC_CalcFdcUpdateFlags(EventId);

# if (DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON)

    /* Set internal status to FdcToc only when not yet set for event */
    if (( (lUpdateFlags & DEM_CFG_TRIGGER_FDC_FIRST_IN_CYCLE) != 0u)
#  if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
      || (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(EventId), Dem_InternalStatus_FDCTOC) == FALSE)
#  endif
      )
    {
      Dem_DTC_SetFdcToc(EventId);
    }
# endif

    lBlockModification = Dem_FaultMemory_StorageManager_UpdateEventEntryOnFailed(EventId, lUpdateFlags, lEventEntryIndex);

    if (DEM_DATA_TEST_USERDATA_CHANGED(lBlockModification))
    {
      if (Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE)
      {
        Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
      }
      else
      {
        Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
      }
    }

    if (DEM_DATA_TEST_USERDATA_CHANGED(lBlockModification) == TRUE)
    {
      Dem_Notifications_EventDataChanged(EventId);
    }
  }
  Dem_Event_DataUpdateFinish(EventId);
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  Dem_Memories_PrestorageMemory_StateSetDiscard();
# endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CustomizedFdcTrip
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
Dem_DTC_CustomizedFdcTrip(
  Dem_EventIdType EventId,
  boolean TCTOC,
  boolean FdcThresholdExceeded,
  boolean FdcMaximumExceeded
  )
{
  if ( (Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE) || (FdcThresholdExceeded == TRUE) )
  {
    Dem_Event_DataUpdateStart(EventId);
#if (DEM_FEATURE_NEED_HEALINGCOUNTER == STD_ON)
    if (Dem_Event_UDSStatus_TestBit(Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId), Dem_UdsStatus_PDTC) == FALSE)
    {
      Dem_Event_SetTripCount(EventId, 0);
    }
#endif

    /* Don't store internal events (i.e. events without a DTC attached) */
    if ( (Dem_Cfg_EventUdsDtc(EventId) != DEM_CFG_DTC_UDS_INVALID)
         /* Don't store with disabled storage conditions */
      && (Dem_GlobalDiagnostics_TestEventStorageConditionsSatisfied(EventId) == TRUE) )
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex;
      boolean lNewEntry;

      if (Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE)
      { /* Already stored - Find, *but don't update* the event entry */
        lNewEntry = FALSE;
        lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);
      }
      else
      { /* Not stored yet */
        lNewEntry = TRUE;
        lEventEntryIndex = Dem_DTC_EventCreateMemory(EventId);
      }

      if (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
      {
        if (FdcThresholdExceeded == TRUE)
        {
          Dem_DTC_UDSStatusType lOldPreconfirmedStatus;
          Dem_DTC_UDSStatusType lNewPreconfirmedStatus;
          Dem_Cfg_StorageTriggerType lEffects;
          uint8 lBlockModification;

          lOldPreconfirmedStatus = Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(lEventEntryIndex);

          if (DEM_DTC_TEST_PC_STATUS_ADTC(lOldPreconfirmedStatus) == TRUE)
          { /* Reclaim aged event */
            lNewEntry = TRUE;
            Dem_MemoryEntry_EventEntry_InitializeEntry(lEventEntryIndex);
            lOldPreconfirmedStatus = Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(lEventEntryIndex);
          }

          lNewPreconfirmedStatus = lOldPreconfirmedStatus;
          lNewPreconfirmedStatus = DEM_DTC_SET_PC_STATUS_UDTC(lNewPreconfirmedStatus);
          lNewPreconfirmedStatus = DEM_DTC_SET_PC_STATUS_UDTCTOC(lNewPreconfirmedStatus);
          lNewPreconfirmedStatus = DEM_DTC_SET_PC_STATUS_UDTCSLC(lNewPreconfirmedStatus);

          Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus(lEventEntryIndex, lNewPreconfirmedStatus);
          lEffects = DEM_CFG_TRIGGER_FDC;

          /* reset OCC1,2 (cycles without preconfirmed result) */
          {
            Dem_MemoryEntry_EventEntry_SetOccurrenceCounter1(lEventEntryIndex, 0);
            Dem_MemoryEntry_EventEntry_SetOccurrenceCounter2(lEventEntryIndex, 0);
          }

          if (DEM_DTC_TEST_PC_STATUS_UDTCTOC(lOldPreconfirmedStatus) == FALSE)
          { /* OCC4 at most once per cycle */
            if (Dem_MemoryEntry_EventEntry_GetOccurrenceCounter4(lEventEntryIndex) < 0xffU)
            {
              Dem_MemoryEntry_EventEntry_SetOccurrenceCounter4(lEventEntryIndex, Dem_MemoryEntry_EventEntry_GetOccurrenceCounter4(lEventEntryIndex) + 1u);
            }

            if ( /* Only increment existing counters */
              (lNewEntry == FALSE)
              /* If we are here, the preconfirmed limit has been exceeded on an already stored event. */
              /* Only increment if the event is not yet tested this cycle (exceeded the counter, not yet tested passed) */
              && ((Dem_Event_UDSStatus_TestBit(Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId), Dem_UdsStatus_TNCTOC) == TRUE)                                              
                 /* Or increment if it is THIS test that has reset the Tested-Bit (e.g. test not debounced) */
                || (TCTOC == TRUE)))
            {
              if (Dem_MemoryEntry_EventEntry_GetOccurrenceCounter7(lEventEntryIndex) < 0xffU)
              {
                Dem_MemoryEntry_EventEntry_SetOccurrenceCounter7(lEventEntryIndex, Dem_MemoryEntry_EventEntry_GetOccurrenceCounter7(lEventEntryIndex) + 1u);
              }
            }

            lEffects = (Dem_Cfg_StorageTriggerType)(lEffects | DEM_CFG_TRIGGER_FIRSTTHISCYCLE);
          }

          lBlockModification = Dem_FaultMemory_StorageManager_UpdateEventEntryOnFailed(EventId, lEffects, lEventEntryIndex);

          if (Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE)
          {
            Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
          }
          else
          {
            Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
          }

          if (DEM_DATA_TEST_USERDATA_CHANGED(lBlockModification) == TRUE)
          {
            Dem_Notifications_EventDataChanged(EventId);
          }
        }

        if (FdcMaximumExceeded == TRUE)
        {
          /* FDC 12 -> Max FDC since last clear */
          Dem_Core_Event_UpdateMaxFdcSlc(EventId);
        }
      }
      /* else: Storage was not possible */
    }
    Dem_Event_DataUpdateFinish(EventId);

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
    /* Prestored data is discarded unconditionally even when storage fails */
    Dem_Memories_PrestorageMemory_StateSetDiscard();
#endif
  }
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_DTC_Disconnect
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
Dem_DTC_Disconnect(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  if (Dem_Cfg_EventCombinedGroup(EventId) != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_DTC_Disconnect_Combined(EventId);
  }
  else
# endif
  {

    Dem_Event_Disconnect_Normal(EventId);
  }
}
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_DTC_Reconnect
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
  Dem_DTC_Reconnect(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  if (Dem_Cfg_EventCombinedGroup(EventId) != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_DTC_Reconnect_Combined(EventId);
  }
  else
# endif
  {
    Dem_Event_Reconnect_Normal(EventId);
  }
}
#endif

#if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
/* ****************************************************************************
 % Dem_DTC_GetWIRStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_GetWIRStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
 return (boolean)(Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(EventId), Dem_InternalStatus_UserControlledWIR));
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


#if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR == STD_ON)
/* ****************************************************************************
 % Dem_DTC_SetWIRStatus
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_DTC_SetWIRStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC) WIRStatus
  )
{
  Std_ReturnType lReturnValue;
  Dem_Event_InternalStatusType lInternalStatus;

  lReturnValue = E_NOT_OK;
# if (DEM_CFG_SUPPORT_USER_CONTROLLED_WIR_DURING_CDTCS == STD_OFF)
  if (Dem_GlobalDiagnostics_TestEnableConditionFulfilled(DEM_ENABLECONDITION_CONTROLDTCSETTING) == TRUE)
# endif
  {
    /* DTC setting is enabled, proceed */
    if (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(EventId), Dem_InternalStatus_UserControlledWIR) != WIRStatus)
    {
      uint8 lOldExternalDtcStatus = Dem_DTC_GetExternalUDSStatus(EventId);

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
      Dem_EnterCritical_DiagMonitor();

      /* update internal status bit */
      lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
      if (WIRStatus == FALSE)
      {
        lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_UserControlledWIR);
      }
      else
      {
        lInternalStatus = Dem_Event_InternalStatus_SetBit(lInternalStatus, Dem_InternalStatus_UserControlledWIR);
      }
      Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);

      Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

      Dem_Notifications_DtcStatusChanged(EventId,
                                         lOldExternalDtcStatus,
                                         Dem_DTC_GetExternalUDSStatus(EventId),
                                         DEM_NOTIFICATIONS_USER_WIR);
    }

    lReturnValue = E_OK;
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
/* ****************************************************************************
 % Dem_DTC_CycleQualifyDcy
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_CycleQualifyDcy(
  void
  )
{
# if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
  Dem_DTC_CycleQualifyDcy_AllDtc();
# else
#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  Dem_DTC_CycleQualifyDcy_EventCombType2_StoredOnly();
#  else
  Dem_DTC_CycleQualifyDcy_StoredOnly();
#  endif
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


/* ****************************************************************************
 % Dem_DTC_OpCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_OpCycle(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  CycleStatus
  )
{
  Dem_DTC_ForEachCombinedArgType lArgs;
  lArgs.Specific.EventOpCycle_Internal.CycleStatus = CycleStatus;
  Dem_DTC_ForEachCombinedSubEvent(EventId, Dem_Event_OpCycle, &lArgs);                                                           /* SBSW_DEM_CALL_FOREACH_COMBINED_EVENT */
}

/* ****************************************************************************
 % Dem_DTC_InitQualification
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_InitQualification(
  void
  )
{
#if(DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF)
  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    Dem_MemoryEntry_AllocMemEntriesIterType lStoredEventsIter;

    /* Scan the primary memory and extract qualified warningIndicatorRequested and confirmedDTC status */
    for (Dem_FaultMemory_MemoryEntry_ChronoIter_Init(DEM_CFG_MEMORYID_PRIMARY, &lStoredEventsIter);                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_FaultMemory_MemoryEntry_ChronoIter_Exists(&lStoredEventsIter) == TRUE;                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_FaultMemory_MemoryEntry_ChronoIter_Next(&lStoredEventsIter))                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex(&lStoredEventsIter);        /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_EventIdType lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);

      if ( (Dem_Event_TestValidHandle(lEventId) == TRUE)
        && (Dem_Core_DTC_IsObdRelated(lEventId) == TRUE))
      { /* only OBD related events supports the qualification of Confirmed and WIR bit */
        uint8 lEntryState = Dem_MemoryEntry_EventEntry_GetState(lEventEntryIndex);
        Dem_DTC_SetQualifyStatus(lEventId, DEM_MEM_SET_NON_QUALIFIED_STATUS_BITS(lEntryState));
      }
    }
  }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_DTC_Init
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
Dem_DTC_Init(
  void
  )
{
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;
  {
    lGroupId = Dem_Cfg_GlobalCombinedGroupCount();
    while (lGroupId > 0u)
    {
      --lGroupId;
      Dem_DTC_InitGroupInternalStatus(lGroupId);
      Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(lGroupId);
    }
  }
#endif

}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_DTC_PreInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_PreInit(
  void
  )
{
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  {
    Dem_Cfg_CombinedGroupIndexType lGroupId;
    for (lGroupId = 0u; lGroupId < Dem_Cfg_GlobalCombinedGroupCount(); ++lGroupId)
    {
      Dem_Event_InternalStatusType lInternalStatus;
      Dem_Cfg_CombinedGroupIterType lSubEventIter;

      lInternalStatus = Dem_Event_InternalStatus_SetBit(0u, Dem_InternalStatus_Disconnected);
      for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lSubEventIter);                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterExists(&lSubEventIter) == TRUE;                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterNext(&lSubEventIter))                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        if (Dem_Cfg_EventAvailableByVariant(Dem_Cfg_CombinedGroupIterGet(&lSubEventIter)) == TRUE)                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          lInternalStatus = Dem_Event_InternalStatus_SetBit(lInternalStatus, Dem_InternalStatus_AvailableInVariant);
        }
        if (Dem_Cfg_EventAvailableByDefault(Dem_Cfg_CombinedGroupIterGet(&lSubEventIter)) == TRUE)                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_Disconnected);
        }
      }
      Dem_Event_InternalStatus_SetCombinedGroupStatus(lGroupId, lInternalStatus);
    }
  }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTC_SetQualificationProcessedCombinedDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_SetQualificationProcessedCombinedDTC(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) GroupId
  )
{
  Dem_DTC_SetCombinedGroupProcessed(GroupId, Dem_DTC_QualificationProcessedCombinedDTCs);                                        /* SBSW_DEM_POINTER_FORWARD_GLOBAL */
}
#endif


/* ****************************************************************************
% Dem_DTC_QualifySingleDTC
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTC_QualifySingleDTC(                                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  const Dem_EventIdType EventId
)
{
# if (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
  uint8 lOldExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(EventId);
  Dem_DTC_ForEachCombinedArgType lArgs;

  Dem_DTC_CycleQualifyDcyForEachSubevent(EventId, &lArgs);                                                                       /* SBSW_DEM_POINTER_FORWARD_STACK */

# if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
  /* Qualification state is stored in Status Block */
  Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS),
    DEM_NVM_BLOCKSTATE_DIRTY);
#else
/* Qualification state is stored in event entry */
  if (Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE)
  { /* Update qualification status in event entry */
    Dem_Cfg_EntryIndexType lEventEntryIndex
      = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);

    Dem_Event_UpdateQualificationStatus(EventId, lEventEntryIndex);
    Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
  }
#endif

# if (DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE == STD_ON)
  if (Dem_Event_UDSStatus_TestBit(lArgs.Common.NewDtcStatus, Dem_UdsStatus_WIR) == TRUE)                                                            
  {
    Dem_MemoryEntry_PermanentEntry_UpdateVisiblity(EventId);
  }
#endif

  Dem_Notifications_DtcStatusChanged(EventId,
    lOldExternalDtcUdsStatus,
    lArgs.Common.NewDtcStatus,
    DEM_NOTIFICATIONS_CYCLE_QUALIFIED);
  if (Dem_Event_UDSStatus_TestBit((uint8)(lOldExternalDtcUdsStatus ^ lArgs.Common.NewDtcStatus), Dem_UdsStatus_CDTC) == TRUE)
  {
    Dem_Notifications_EventDataChanged(EventId);
  }

#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
% Dem_DTC_TestDtcActive
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestDtcActive(                                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  const Dem_EventIdType EventId
  )
{
  boolean lIsActive = FALSE;
  uint8 lExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(EventId);
  if (((Dem_Event_UDSStatus_TestBit(lExternalDtcUdsStatus, Dem_UdsStatus_TF) == TRUE) && (Dem_Event_UDSStatus_TestBit(lExternalDtcUdsStatus, Dem_UdsStatus_CDTC) == TRUE))
    || ((Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE) && (Dem_GlobalDiagnostics_TestEventHasMilAssigned(EventId) == TRUE)
    && (Dem_Event_UDSStatus_TestBit(lExternalDtcUdsStatus, Dem_UdsStatus_WIR) == TRUE))
     )
  {
    lIsActive = TRUE;
  }
  return lIsActive;
}

/* ****************************************************************************
% Dem_DTC_TestDtcPreviouslyActive
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTC_TestDtcPreviouslyActive(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  const Dem_EventIdType EventId
  )
{
  boolean lIsPreviouslyActive;
  uint8 lExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(EventId);
  if ((Dem_Event_UDSStatus_TestBit(lExternalDtcUdsStatus, Dem_UdsStatus_TF) == FALSE) && (Dem_Event_UDSStatus_TestBit(lExternalDtcUdsStatus, Dem_UdsStatus_CDTC) == TRUE)
    && ((Dem_Cfg_IsEmissionObdSupportedInVariant() == FALSE) || ((Dem_GlobalDiagnostics_TestEventHasMilAssigned(EventId) == FALSE)
    || (Dem_Event_UDSStatus_TestBit(lExternalDtcUdsStatus, Dem_UdsStatus_WIR) == FALSE))))
  {
    lIsPreviouslyActive = TRUE;
  }
  else
  {
    lIsPreviouslyActive = FALSE;
  }
  return lIsPreviouslyActive;
}

/* ****************************************************************************
 % Dem_DTC_GetFormatedDTCByEvent
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_DTC_GetFormatedDTCByEvent(                                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  Dem_DTCFormatType DTCFormat,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  switch (DTCFormat)
  {
  case DEM_DTC_FORMAT_UDS:
    {
      uint32 lTempDtc = Dem_Cfg_EventUdsDtc(EventId);
      if (lTempDtc != DEM_CFG_DTC_UDS_INVALID)
      {
        *DTC = lTempDtc;                                                                                                         /* SBSW_DEM_POINTER_WRITE_API */
        lReturnValue = E_OK;
      }
    }
    break;

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
  case DEM_DTC_FORMAT_J1939:
    {
      uint32 lTempDtc = Dem_Cfg_EventJ1939Dtc(EventId);
      if (lTempDtc != DEM_CFG_DTC_J1939_INVALID)
      {
        *DTC = lTempDtc;                                                                                                         /* SBSW_DEM_POINTER_WRITE_API */
        lReturnValue = E_OK;
      }
    }
    break;
#endif /* (DEM_CFG_SUPPORT_J1939 == STD_ON) */

  case DEM_DTC_FORMAT_OBD:
    if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
    {
      uint32 lTempDtc = Dem_Cfg_EventObdDtc(EventId);
      if (lTempDtc != DEM_CFG_DTC_OBD_INVALID)
      { /* copy valid DTC number to out parameter */
        *DTC = (uint32)(lTempDtc << 8u);                                                                                         /* SBSW_DEM_POINTER_WRITE_API */
        lReturnValue = E_OK;
      }
    }
    else if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
    {
      uint32 lTempDtc = Dem_Cfg_EventUdsDtc(EventId);
      if (DEM_CFG_DTC_UDS_INVALID != lTempDtc)
      {
        *DTC = lTempDtc;                                                                                                         /* SBSW_DEM_POINTER_WRITE_API */
        lReturnValue = E_OK;
      }
    }
    else
    {
      /* MISRA case */
      /* E_NOT_OK */
    }
    break;

  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    break;
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_DTCIF_IMPLEMENTATION_H */
/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_DTCIF_Implementation.h
 *********************************************************************************************************************/
