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
/*! \addtogroup Dem_ClientAccessIF
 *  \{
 *  \file       Dem_ClientAccessIF_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) Implementation file
 *  \details    Implementation of ClientAccess subcomponent
 *  \entity     ClientAccessIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CLIENTACCESSIF_IMPLEMENTATION_H)
#define DEM_CLIENTACCESSIF_IMPLEMENTATION_H

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
#include "Dem_ClientAccessIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_RecordReader_Interface.h"
#include "Dem_FilterData_Interface.h"
#include "Dem_FreezeFrameIterator_Interface.h"
#include "Dem_J1939ExpandedFF_Interface.h"
#include "Dem_DTCSelector_Interface.h"
#include "Dem_DTCReporting_Interface.h"
#include "Dem_ClearDTC_Interface.h"
#include "Dem_ClientData_Interface.h"
#include "Dem_Data_Interface.h"

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/*! Maximum PID value for Diagnostic Readiness 2 (DM21) */
#define DEM_J1939_DM21_MAX_PID_VALUE   (64255U)

#define DEM_START_SEC_CONST_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_STOP_SEC_CONST_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
 /*!
  * \defgroup Dem_ClientAccessIF_PrivateProperties Private Properties
  * \{
  */

/* ****************************************************************************
 * Dem_Client_ClientAccess_IsJ1939Readiness2Supported
 *****************************************************************************/
/*!
 * \brief         Returns True if feature 'J1939 Readiness2' is supported.
 *
 * \details       -
 *
 * \return        TRUE
 *                Support for 'J1939 Readiness2'
 * \return        FALSE
 *                No support for 'J1939 Readiness2'
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939Readiness2Supported(
  void
);

/* ****************************************************************************
 * Dem_Client_ClientAccess_IsJ1939Readiness1Supported
 *****************************************************************************/
/*!
 * \brief         Tests if J1939 Readiness 1 calculation is supported.
 *
 * \details       Tests if J1939 Readiness 1 calculation is supported.
 *
 * \return        TRUE
 *                J1939 Readiness 1 calculation is supported.
 * \return        FALSE
 *                J1939 Readiness 1 calculation is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939Readiness1Supported(
  void
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Client_ClientAccess_IsJ1939Readiness2Supported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939Readiness2Supported(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_J1939_READINESS2 == STD_ON);
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_IsJ1939Readiness1Supported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939Readiness1Supported(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_J1939_READINESS1 == STD_ON);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_ClientAccess_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Client_ClientAccess_TestDtcMatchesJ1939ClearFilter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestDtcMatchesJ1939ClearFilter(
  Dem_EventIdType EventId,
  uint8 DTCTypeFilter
)
{
  boolean lEventMatch;

  switch (DTCTypeFilter)
  {
  case DEM_J1939DTC_CLEAR_ACTIVE:
    lEventMatch = (Dem_DTC_TestDtcActive(EventId) == TRUE);
    break;

  case DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE:
    lEventMatch = (Dem_DTC_TestDtcPreviouslyActive(EventId) == TRUE);
    break;

  default:
    /* Clear active and previously active DTCs */
    lEventMatch = ((Dem_DTC_TestDtcActive(EventId) == TRUE) || (Dem_DTC_TestDtcPreviouslyActive(EventId) == TRUE));              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    break;
  }
  return lEventMatch;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_GlobalClientCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Client_ClientAccess_GlobalClientCount(
  void
  )
{
  return  Dem_Cfg_GetSizeOfClientTable();
}
       
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestSelectorValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestSelectorValid(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  return (boolean)(Dem_ClientData_GetDTCSelector(ClientId) != DEM_CFG_DTCSELECTOR_INVALID);
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_TestClientIdValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestClientIdValid(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
)
{
  return (boolean)(ClientId < Dem_Client_ClientAccess_GlobalClientCount());
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_SelectDTC
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
Dem_Client_ClientAccess_SelectDTC(
  uint8 ClientId,
  uint32 DTC,
  Dem_DTCFormatType DTCFormat,
  Dem_DTCOriginType DTCOrigin
  )
{
  Dem_DTCSelector_HandleType lDTCSelectorId;
  Dem_ClearDTC_HandleType lClearDTCHandle;

  lClearDTCHandle = Dem_ClientData_GetClearDTCStatusHandle(ClientId);
  /* Client is configured to clear DTCs */
  if (lClearDTCHandle != DEM_CLEARDTC_HANDLE_INVALID)
  {
    Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
  }

  lDTCSelectorId = Dem_ClientData_GetDTCSelector(ClientId);
  if (lDTCSelectorId != DEM_CFG_DTCSELECTOR_INVALID)
  {
    Dem_DTCOriginType lMappedOrigin;
    if (DTCOrigin == DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY)
    {
      lMappedOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
    }
    else
    {
      lMappedOrigin = DTCOrigin;
    }
    Dem_DTCSelector_SelectDTC(lDTCSelectorId, DTC, DTCFormat, lMappedOrigin);
  }
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetSelectionResult
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCSelector_ResultType, DEM_CODE)
Dem_Client_ClientAccess_GetSelectionResult(
  CONST(uint8, AUTOMATIC) ClientId
  )
{
  Dem_DTCSelector_HandleType lDTCSelectorId;
  Dem_DTCSelector_ResultType lReturnValue;

  lDTCSelectorId = Dem_ClientData_GetDTCSelector(ClientId);
  if (lDTCSelectorId != DEM_CFG_DTCSELECTOR_INVALID)
  {
    lReturnValue = Dem_DTCSelector_GetSelectionResult(lDTCSelectorId);
  }
  else
  {
    lReturnValue = DEM_DTCSELECTOR_SELECTION_INIT;
  }

  return lReturnValue;
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_GetSelectedMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClientAccess_GetSelectedMemoryId(
  CONST(uint8, AUTOMATIC) ClientId
  )
{
  Dem_DTCSelector_HandleType lDTCSelectorId;
  uint16 lTranslatedMemoryId;

  lDTCSelectorId = Dem_ClientData_GetDTCSelector(ClientId);
  if (lDTCSelectorId != DEM_CFG_DTCSELECTOR_INVALID)                                                                             /* COV_DEM_ROBUSTNESS TX */
  {
    lTranslatedMemoryId = Dem_DTCSelector_GetMemoryId(lDTCSelectorId);
  }
  else
  {
    lTranslatedMemoryId = DEM_CFG_MEMORYID_INVALID;
  }

  return lTranslatedMemoryId;
}
#endif

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetSelectedDTCFormat
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCFormatType, DEM_CODE)
Dem_Client_ClientAccess_GetSelectedDTCFormat(
  uint8 ClientId
  )
{
  Dem_DTCFormatType lDTCFormat = DEM_DTC_FORMAT_INVALID;
  Dem_DTCSelector_HandleType lDTCSelectorId = Dem_ClientData_GetDTCSelector(ClientId);

  if (lDTCSelectorId != DEM_CFG_DTCSELECTOR_INVALID)                                                                             /* COV_DEM_ROBUSTNESS TX */
  {
    lDTCFormat = Dem_DTCSelector_GetDTCFormat(lDTCSelectorId);
  }

  return lDTCFormat;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetSelectedEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Client_ClientAccess_GetSelectedEventId(
  CONST(uint8, AUTOMATIC) ClientId
  )
{
  Dem_DTCSelector_HandleType lDTCSelectorId;
  Dem_EventIdType lReturnValue;

  lDTCSelectorId = Dem_ClientData_GetDTCSelector(ClientId);
  if (lDTCSelectorId != DEM_CFG_DTCSELECTOR_INVALID)                                                                             /* COV_DEM_ROBUSTNESS TX */
  {
    lReturnValue = Dem_DTCSelector_GetEventId(lDTCSelectorId);
  }
  else
  {
    lReturnValue = DEM_EVENT_INVALID;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_TestSelectDTCLocked
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) 
Dem_Client_ClientAccess_TestSelectDTCLocked(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC) ClientId
  )
{
  Dem_DTCSelector_HandleType lDTCSelectorId;
  boolean lReturnValue;

  lDTCSelectorId = Dem_ClientData_GetDTCSelector(ClientId);
  if (lDTCSelectorId != DEM_CFG_DTCSELECTOR_INVALID)
  {
    lReturnValue = Dem_DTCSelector_TestSelectDTCLocked(lDTCSelectorId);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_TestFilterValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestFilterValid(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  return (boolean) (Dem_ClientData_GetFilterId(ClientId) != DEM_CFG_FILTER_INVALID);
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_SetDTCFilter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetDTCFilter(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONST(uint8, AUTOMATIC)  DTCStatusMask,
  CONST(Dem_DTCFormatType, AUTOMATIC)  DTCFormat,
  CONST(Dem_DTCOriginType, AUTOMATIC)  DTCOrigin,
  CONST(boolean, AUTOMATIC)  FilterWithSeverity,
  CONST(Dem_DTCSeverityType, AUTOMATIC)  DTCSeverityMask,
  CONST(boolean, AUTOMATIC)  FilterForFDC
  )
{
  Std_ReturnType lReturnValue = E_NOT_OK;
  Dem_FilterData_HandleType lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    Dem_DTCOriginType lMappedOrigin;
    boolean lFilterEmissionRelatedDtcs;
    uint16 lTranslatedMemoryId;

    if (DTCOrigin == DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY)
    {
      lMappedOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      lFilterEmissionRelatedDtcs = TRUE;
    }
    else
    {
      lMappedOrigin = DTCOrigin;
      lFilterEmissionRelatedDtcs = FALSE;
    }

    if (Dem_FaultMemory_Memories_TranslateDTCOrigin(lMappedOrigin, &lTranslatedMemoryId) == E_OK)                                /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      lReturnValue = Dem_Client_FilterData_SetDTCFilter(lFilterId,
                                                DTCStatusMask,
                                                DTCFormat,
                                                lTranslatedMemoryId,
                                                lFilterEmissionRelatedDtcs,
                                                FilterWithSeverity,
                                                DTCSeverityMask,
                                                FilterForFDC);
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNextFilteredDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextFilteredDTC(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(Dem_FilterData_ResultType, AUTOMATIC, AUTOMATIC)  FilterResult
  )
{
  Std_ReturnType lReturnValue;
  Dem_FilterData_HandleType lFilterId;

  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    lReturnValue = Dem_Client_FilterData_GetNextFilteredDTC(lFilterId, FilterResult);                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestDTCFilterSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestDTCFilterSet(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_FilterData_HandleType lFilterId;

  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    lReturnValue = Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_DTC);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestDTCFilterWithFDCSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
  Dem_Client_ClientAccess_TestDTCFilterWithFDCSet(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_FilterData_HandleType lFilterId;

  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    lReturnValue = (boolean)((Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_DTC) == TRUE)
                     && (Dem_Client_FilterData_TestUseFdc(lFilterId) == TRUE));                                                  /* PRQA S 3415 */ /* MD_DEM_13.5_cf */ 
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestDTCFilterWithSeveritySet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestDTCFilterWithSeveritySet(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_FilterData_HandleType lFilterId;

  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    lReturnValue = (boolean)((Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_DTC) == TRUE)
                    && (Dem_Client_FilterData_TestUseSeverity(lFilterId) == TRUE));                                              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */ 
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestJ1939DTCFilterSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939DTCFilterSet(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_FilterData_HandleType lFilterId;

  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    lReturnValue = Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_J1939DTC);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_Client_ClientAccess_TestJ1939RatioFilterSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939RatioFilterSet(
  uint8 ClientId
)
{
  boolean lReturnValue = FALSE;
  Dem_FilterData_HandleType lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    lReturnValue = Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_J1939RATIO);
  }

  return lReturnValue;
}

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestJ1939DTCFilterWithLampStatusSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939DTCFilterWithLampStatusSet(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_FilterData_HandleType lFilterId;

  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    lReturnValue = Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_J1939DTCLAMP);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestJ1939FreezeFrameFilterSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939FreezeFrameFilterSet(                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_FreezeFrameIterator_HandleType lFreezeFrameIteratorId;

  lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(ClientId);

  if (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID)
  {
    lReturnValue = Dem_FreezeFrameIterator_TestJ1939FreezeFrameFilterSet(lFreezeFrameIteratorId);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_Client_ClientAccess_TestJ1939FreezeFrameFilterFreezeFrameKind
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestJ1939FreezeFrameFilterFreezeFrameKind(                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint8  ClientId,
  Dem_J1939DcmSetFreezeFrameFilterType  FreezeFrameKind
  )
{
  Dem_FreezeFrameIterator_HandleType lFreezeFrameIteratorId;
  boolean lReturnValue = FALSE;

  lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(ClientId);

  if (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID)
  {
    if (FreezeFrameKind == Dem_FreezeFrameIterator_GetFreezeFrameKind(lFreezeFrameIteratorId))
    {
      lReturnValue = TRUE;
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_Init
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Client_ClientAccess_Init(
  void
  )
{
  uint8 lClientId;
  for (lClientId = 0u; 
       lClientId < Dem_Client_ClientAccess_GlobalClientCount(); 
       ++lClientId)
  {
    Dem_DTCSelector_HandleType lDTCSelectorId;
#if (DEM_CFG_SUPPORT_DCM == STD_ON)
    Dem_DTCReadoutBuffer_HandleType lReadoutBufferId;
#endif
#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) || (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON) )
    Dem_FilterData_HandleType lFilterId;
#endif
#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) ||(DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) )
    Dem_FreezeFrameIterator_HandleType lFreezeFrameIteratorId;
#endif
    Dem_Dcm_ERec_0x1916_RecordNumberFilter_HandleType lExtendedDataFilterId;


    lDTCSelectorId = Dem_ClientData_GetDTCSelector(lClientId);
#if (DEM_CFG_SUPPORT_DCM == STD_ON)
    lReadoutBufferId = Dem_ClientData_GetReadoutBufferId(lClientId);
#endif
#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) || (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON) )
    lFilterId = Dem_ClientData_GetFilterId(lClientId);
#endif
#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) ||(DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) )
    lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(lClientId);
#endif

    if (lDTCSelectorId != DEM_CFG_DTCSELECTOR_INVALID)
    {
      Dem_DTCSelector_InitDefault(lDTCSelectorId);
    }

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
    if (lReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)
    {
      Dem_DTCReadoutBuffer_Init(lReadoutBufferId);
    }
#endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) || (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON) )
    if (lFilterId != DEM_CFG_FILTER_INVALID)
    {
      Dem_Client_FilterData_InitDefault(lFilterId);
    }
#endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) ||(DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) )
    if (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID)
    {
      Dem_FreezeFrameIterator_InitDefault(lFreezeFrameIteratorId);
    }
#endif

    if (Dem_Dcm_ERec_0x1916_IsService19_16Enabled() == TRUE)
    {
      lExtendedDataFilterId = Dem_ClientData_GetExtendedDataFilterId(lClientId);
      if (lExtendedDataFilterId != DEM_CFG_FILTER_INVALID)
      {
        Dem_Dcm_ERec_0x1916_InitERecNumberFilter(lExtendedDataFilterId);
      }
    }
  }

  Dem_ClearDTC_Init();
  return;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestReadoutBufferValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestReadoutBufferValid(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  return (boolean) (Dem_ClientData_GetReadoutBufferId(ClientId) != DEM_CFG_READOUTBUFFER_INVALID);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_GetReadoutBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCReadoutBuffer_HandleType, DEM_CODE)
Dem_Client_ClientAccess_GetReadoutBuffer(
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  return Dem_ClientData_GetReadoutBufferId(ClientId);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestDTCRecordUpdateRequested
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestDTCRecordUpdateRequested(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_DTCReadoutBuffer_HandleType lReadoutBufferId;

  lReadoutBufferId = Dem_ClientData_GetReadoutBufferId(ClientId);

  if (lReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)
  {
    lReturnValue = Dem_DTCReadoutBuffer_TestDataRequested(lReadoutBufferId);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestDTCRecordUpdateDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestDTCRecordUpdateDisabled(
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_DTCReadoutBuffer_HandleType lReadoutBufferId;

  lReadoutBufferId = Dem_ClientData_GetReadoutBufferId(ClientId);

  if (lReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)
  {
    lReturnValue = Dem_DTCReadoutBuffer_TestResultReady(lReadoutBufferId);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestSnapshotRecordSelected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestSnapshotRecordSelected(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_DTCReadoutBuffer_HandleType lReadoutBufferId;

  lReadoutBufferId = Dem_ClientData_GetReadoutBufferId(ClientId);

  if (lReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)
  {
    lReturnValue = Dem_DTCReadoutBuffer_TestSnapshotRecordSelected(lReadoutBufferId);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestExtendedRecordSelected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestExtendedRecordSelected(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_DTCReadoutBuffer_HandleType lReadoutBufferId;

  lReadoutBufferId = Dem_ClientData_GetReadoutBufferId(ClientId);

  if (lReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)
  {
    lReturnValue = Dem_DTCReadoutBuffer_TestExtendedRecordSelected(lReadoutBufferId);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_DisableDTCRecordUpdate
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_DisableDTCRecordUpdate(
  CONST(uint8, AUTOMATIC) ClientId
  )
{
  Std_ReturnType lReturnValue;
  Dem_DTCSelector_ResultType lSelectionResult;

  lReturnValue = E_NOT_OK;
  lSelectionResult = Dem_Client_ClientAccess_GetSelectionResult(ClientId);
  if (lSelectionResult == DEM_DTCSELECTOR_SELECTION_DTC)
  {
    Dem_EventIdType lEventId;
    /* get event id from selectDTC */
    lEventId = Dem_Client_ClientAccess_GetSelectedEventId(ClientId);
    Dem_Internal_AssertReturnValue(lEventId != DEM_EVENT_INVALID, DEM_E_INCONSISTENT_STATE, E_NOT_OK)

    if (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE)
    {
      uint16 lMemoryId;
      Dem_DTCReadoutBuffer_HandleType lReadoutBufferId;

      lMemoryId = Dem_Client_ClientAccess_GetSelectedMemoryId(ClientId);
      Dem_Internal_AssertReturnValue(lMemoryId != DEM_CFG_MEMORYID_INVALID, DEM_E_INCONSISTENT_STATE, E_NOT_OK)

      lReadoutBufferId = Dem_ClientData_GetReadoutBufferId(ClientId);
      if (lReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)                                                                     /* COV_DEM_ROBUSTNESS TX */
      {
        Dem_DTCSelector_HandleType lSelector;
        lSelector = Dem_ClientData_GetDTCSelector(ClientId);
        lReturnValue = Dem_DTCReadoutBuffer_CheckAndSetState(lReadoutBufferId, lEventId, lMemoryId, lSelector);
      }
    }
    else
    {
      lReturnValue = DEM_WRONG_DTC;
    }
  }
  else
  {
    /* missing SelectDTC call or incorrect arguments in that call */
    switch (lSelectionResult)
    {
      case DEM_DTCSELECTOR_SELECTION_GROUP:
      case DEM_DTCSELECTOR_SELECTION_ALL_DTCS:
      case DEM_DTCSELECTOR_SELECTION_NO_MATCH:
        lReturnValue = DEM_WRONG_DTC;
        break;
      case DEM_DTCSELECTOR_SELECTION_WRONG_ORIGIN:
        lReturnValue = DEM_WRONG_DTCORIGIN;
        break;
      case DEM_DTCSELECTOR_SELECTION_INIT:
      default:                                                                                                                   /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
        /* ReturnValue E_NOT_OK */
        break;
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_EnableDTCRecordUpdate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_EnableDTCRecordUpdate(
  CONST(uint8, AUTOMATIC) ClientId
  )
{
  Std_ReturnType lReturnValue;
  Dem_DTCReadoutBuffer_HandleType lReadoutBufferId;
  Dem_DTCSelector_HandleType lSelector;

  lReturnValue = E_NOT_OK;

  lReadoutBufferId = Dem_ClientData_GetReadoutBufferId(ClientId);
  lSelector = Dem_ClientData_GetDTCSelector(ClientId);

  if (lReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)
  {
    Dem_DTCReadoutBuffer_FreeBuffer(lReadoutBufferId, lSelector);
    lReturnValue = E_OK;
  }

  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_Client_ClientAccess_FillDTCReadoutBufferData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Client_ClientAccess_FillDTCReadoutBufferData(
  void
  )
{
#if (DEM_CFG_SUPPORT_DCM == STD_ON)
  uint8 lClientId;

  for (lClientId = 0u; 
       lClientId < Dem_Client_ClientAccess_GlobalClientCount(); 
       ++lClientId)
  {
    Dem_DTCReadoutBuffer_HandleType lReadoutBufferId;
    Dem_DTCSelector_HandleType lSelector;

    lReadoutBufferId = Dem_ClientData_GetReadoutBufferId(lClientId);
    lSelector = Dem_ClientData_GetDTCSelector(lClientId);

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    if (Dem_DTCReadoutBuffer_TestCombinedEventSelected(lReadoutBufferId) == TRUE)
    {
      Dem_DTCReadoutBuffer_FillData_CombinedEvent(lReadoutBufferId, lSelector);
    }
    else
#endif
    {
      Dem_DTCReadoutBuffer_FillData_NormalEvent(lReadoutBufferId, lSelector);
    }
  }
#endif
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_ClearDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) Dem_Client_ClientAccess_ClearDTC(
  CONST(uint8, AUTOMATIC) ClientId
  )
{
  Std_ReturnType lReturnValue;
  Dem_ClearDTC_HandleType lClearDTCHandle;
  Dem_ClearDTC_StatusType lClearDTCStatus;

  lReturnValue = DEM_CLEAR_FAILED;

  lClearDTCHandle = Dem_ClientData_GetClearDTCStatusHandle(ClientId);
  /* Client is configured to clear DTCs */
  if (lClearDTCHandle != DEM_CLEARDTC_HANDLE_INVALID)
  {
    lClearDTCStatus = Dem_ClearDTC_GetClearStatus(lClearDTCHandle);
    switch (lClearDTCStatus)
    {
    case DEM_CLEARDTC_STATUS_IDLE:
    {
      Dem_DTCSelector_HandleType lSelector = Dem_ClientData_GetDTCSelector(ClientId);
      if (Dem_DTCSelector_GetDTCFormat(lSelector) != DEM_DTC_FORMAT_OBD_3BYTE)
      {
        lReturnValue = Dem_ClearDTC_StartClear(lClearDTCHandle, lSelector);
      }
      else
      {
        /* Clear not allowed if DTC format is set to OBD 3-Byte */
        lReturnValue = DEM_CLEAR_FAILED;
      }
    }
    break;
    case DEM_CLEARDTC_STATUS_PENDING:
      lReturnValue = DEM_PENDING;
      break;
    case DEM_CLEARDTC_STATUS_DONE:
      Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
      lReturnValue = E_OK;
      break;
    case DEM_CLEARDTC_STATUS_FAILED:
      Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
      lReturnValue = DEM_CLEAR_FAILED;
      break;
    case DEM_CLEARDTC_STATUS_NV_ERROR:
      Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
      lReturnValue = DEM_CLEAR_MEMORY_ERROR;
      break;
    default:                                                                                                                     /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      /* return DEM_CLEAR_FAILED */
      break;
    }
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_J1939ClearSingleDTC
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) 
Dem_Client_ClientAccess_J1939ClearSingleDTC(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 ClientId,
  uint32 J1939DTC,
  Dem_DTCOriginType DTCOrigin,
  uint8 DTCTypeFilter
  )
{
  Std_ReturnType lReturnValue = DEM_CLEAR_FAILED;
  Dem_ClearDTC_HandleType lClearDTCHandle = Dem_ClientData_GetClearDTCStatusHandle(ClientId);

  if (lClearDTCHandle != DEM_CLEARDTC_HANDLE_INVALID)
  {
    Dem_ClearDTC_StatusType lClearDTCStatus = Dem_ClearDTC_GetClearStatus(lClearDTCHandle);

    switch (lClearDTCStatus)
    {
    case DEM_CLEARDTC_STATUS_IDLE:
      Dem_Client_ClientAccess_SelectDTC(ClientId, J1939DTC, DEM_DTC_FORMAT_J1939, DTCOrigin);
      if (Dem_Client_ClientAccess_GetSelectionResult(ClientId) == DEM_DTCSELECTOR_SELECTION_DTC)
      {
        Dem_DTCSelector_HandleType lSelector = Dem_ClientData_GetDTCSelector(ClientId);
        /* EventId always valid for DEM_DTCSELECTOR_SELECTION_DTC */
        Dem_EventIdType lEventId = Dem_DTCSelector_GetEventId(lSelector);
        if (Dem_Client_ClientAccess_TestDtcMatchesJ1939ClearFilter(lEventId, DTCTypeFilter) == FALSE)
        {
          lReturnValue = DEM_WRONG_DTCTYPEFILTER;
        }
        else if ((Dem_Core_DTC_IsObdRelated(lEventId) == FALSE)
           && Dem_Client_ClientAccess_IsEventAssignedToNode(lEventId, Dem_Client_ClientAccess_GetNodeIndex(ClientId)))
        {
          lReturnValue = Dem_Client_ClearDTC_StartClearSingleDtcJ1939(lClearDTCHandle, lSelector, DTCTypeFilter,
                                                               Dem_Client_ClientAccess_GetNodeIndex(ClientId));
        }
        else
        {
          /* MISRA Case, lReturnValue is DEM_CLEAR_FAILED at this point */
        }
      }
      else
      {
        lReturnValue = DEM_WRONG_DTC;
      }
      break;
    case DEM_CLEARDTC_STATUS_PENDING:
      lReturnValue = DEM_PENDING;
      break;
    case DEM_CLEARDTC_STATUS_DONE:
      Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
      lReturnValue = E_OK;
      break;
    case DEM_CLEARDTC_STATUS_FAILED:
      Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
      lReturnValue = DEM_CLEAR_FAILED;
      break;
    case DEM_CLEARDTC_STATUS_NV_ERROR:
      Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
      lReturnValue = DEM_CLEAR_MEMORY_ERROR;
      break;
    default:                                                                                                                     /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
      break;
    }
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON) 
/* ****************************************************************************
 % Dem_Client_ClientAccess_J1939ClearDTC
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE) 
Dem_Client_ClientAccess_J1939ClearDTC(
  uint8 ClientId,
  uint8 DTCTypeFilter,
  Dem_DTCOriginType DTCOrigin
  )
{
  Std_ReturnType lReturnValue = DEM_CLEAR_FAILED;

  if ( (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
    && (DTCTypeFilter == DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE) )
  {
    /* Clearing previously active DTCs is not allowed if OBD is supported */
    lReturnValue = DEM_CLEAR_FAILED;
  }
  else
  if ( (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
    && (DTCOrigin != DEM_DTC_ORIGIN_PRIMARY_MEMORY) )
  {
    /* Clearing non-Primary DTCs is not allowed if OBD is supported */
    lReturnValue = DEM_WRONG_DTCORIGIN;
  }
  else
  {
    Dem_ClearDTC_HandleType lClearDTCHandle = Dem_ClientData_GetClearDTCStatusHandle(ClientId);
    /* Client is configured to clear DTCs */
    if (lClearDTCHandle != DEM_CLEARDTC_HANDLE_INVALID)
    {
      Dem_ClearDTC_StatusType lClearDTCStatus = Dem_ClearDTC_GetClearStatus(lClearDTCHandle);
      switch (lClearDTCStatus)
      {
      case DEM_CLEARDTC_STATUS_IDLE:
        {
          Dem_Client_ClientAccess_SelectDTC(ClientId, DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_FORMAT_J1939, DTCOrigin);
          lReturnValue = Dem_Client_ClearDTC_StartClearAllDtcsJ1939(lClearDTCHandle,
                                                             Dem_ClientData_GetDTCSelector(ClientId),
                                                             DTCTypeFilter,
                                                             Dem_Client_ClientAccess_GetNodeIndex(ClientId));
        }
        break;
      case DEM_CLEARDTC_STATUS_PENDING:
        lReturnValue = DEM_PENDING;
        break;
      case DEM_CLEARDTC_STATUS_DONE:
        Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
        lReturnValue = E_OK;
        break;
      case DEM_CLEARDTC_STATUS_FAILED:
        Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
        lReturnValue = DEM_CLEAR_FAILED;
        break;
      case DEM_CLEARDTC_STATUS_NV_ERROR:
        Dem_ClearDTC_ResetClearStatus(lClearDTCHandle);
        lReturnValue = DEM_CLEAR_MEMORY_ERROR;
        break;
      default:                                                                                                                   /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
        /* return DEM_CLEAR_FAILED */
        break;
      }
    }
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) || (DEM_CFG_SUPPORT_J1939 == STD_ON) )
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestFreezeFrameIteratorValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestFreezeFrameIteratorValid(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  return (boolean) (Dem_ClientData_GetFreezeFrameIteratorId(ClientId) != DEM_CFG_FREEZEFRAMEITERATOR_INVALID);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_TestFreezeFrameFilterSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestFreezeFrameFilterSet(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  boolean lReturnValue;
  Dem_FreezeFrameIterator_HandleType lFreezeFrameIteratorId;

  lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(ClientId);

  if (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID)
  {
    lReturnValue = Dem_FreezeFrameIterator_TestFilterSet(lFreezeFrameIteratorId);
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 % Dem_Client_ClientAccess_StartFreezeFrameIterator
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_StartFreezeFrameIterator(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONST(Dem_DTCFormatType, AUTOMATIC)  DTCFormat,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_DCM_DATA)  NumberOfFilteredRecords
)
{
  Std_ReturnType lReturnValue;
  Dem_FreezeFrameIterator_HandleType lFreezeFrameIteratorId;

  lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(ClientId);
  if (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID)
  {
    uint16                                lRecordCount;
    Dem_FreezeFrameIterator_DTCFormatType lDtcFormat;

    /* Count stored snapshot records in primary memory to prevent concurrent modification
    of the chrono index to have an effect on the iteration */
    lDtcFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT;

    if (DTCFormat == DEM_DTC_FORMAT_UDS)
    { /* Set filter for output format of DTCs to UDS */
      lDtcFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_UDS;
    }
    else
    if ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE) &&
        (DTCFormat == DEM_DTC_FORMAT_OBD))
    { /* Set filter for output format of DTCs to OBD */
      lDtcFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD;
    }
    else
    {
      /* DTC format is either J1939, OBD 3-Byte or 
         OBD but OBDII is not exclusively supported. Do nothing. */
    }

    if (lDtcFormat != DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT)
    {
      /* Count stored snapshot records */
      lRecordCount = Dem_FreezeFrameIterator_GetNumberOfFreezeFrameRecords();

      /* Prepare iterations to get the data of the records when later calling Dem_GetNextFilteredRecord. */
      Dem_FreezeFrameIterator_StartIterator(lFreezeFrameIteratorId, lDtcFormat);

      *NumberOfFilteredRecords = lRecordCount;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lReturnValue = E_OK;
    }
    else
    {
      lReturnValue = E_NOT_OK;
    }
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNextFreezeFrameRecord
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
Dem_Client_ClientAccess_GetNextFreezeFrameRecord(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,                                                                              /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber                                                                       /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  )
{
  Dem_FreezeFrameIterator_HandleType  lFreezeFrameIteratorId;
  boolean                             lReturnValue;
  lReturnValue = FALSE;

  lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(ClientId);
  if (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID)
  {
    if (Dem_FreezeFrameIterator_TestFilterSet(lFreezeFrameIteratorId) == TRUE)
    {
      lReturnValue = Dem_FreezeFrameIterator_GetNext(lFreezeFrameIteratorId, DTC, RecordNumber);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_GetDTCByOccurrenceTime
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
Dem_Client_ClientAccess_GetDTCByOccurrenceTime(
  CONST(Dem_DTCRequestType, AUTOMATIC)  DTCRequest,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC
  )
{
  boolean lReturnValue;
  Dem_EventIdType lEventId;

  lReturnValue = FALSE;
  lEventId = DEM_EVENT_INVALID;

  switch (DTCRequest)
  {
  case DEM_FIRST_FAILED_DTC:
    lEventId = Dem_Statistics_GetFirstFailedEvent();
    break;
  case DEM_MOST_RECENT_FAILED_DTC:
    lEventId = Dem_Statistics_GetMostRecentFailedEvent();
    break;
  case DEM_FIRST_DET_CONFIRMED_DTC:
    lEventId = Dem_Statistics_GetFirstConfirmedEvent();
    break;
  case DEM_MOST_REC_DET_CONFIRMED_DTC:
    lEventId = Dem_Statistics_GetMostRecentConfirmedEvent();
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    /* Do nothing */
    break;
  }

  if (lEventId != DEM_EVENT_INVALID)
  { /* Only EventIds with a valid DTC number should be stored in, so the EventId->DTC mapping will return always a valid DTC number */
    if ( (Dem_DTCReporting_TestObdHideOccurrences() == FALSE)
      || (Dem_Cfg_EventSignificance(lEventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)                                          
      || (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE))                                                                           
    {
      if (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE)
      {
        *DTC = Dem_Cfg_EventUdsDtc(lEventId);                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        lReturnValue = TRUE;
      }
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_FilterNumberMemory
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClientAccess_FilterNumberMemory(
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  uint16 lNumberOfDTCs;
  Dem_FilterData_HandleType lFilterId;
  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    lNumberOfDTCs =  Dem_Client_FilterData_CountMatchingDTCs(lFilterId);
  }
  else
  {
    lNumberOfDTCs = 0u;
  }
  return lNumberOfDTCs;
}                                                                                                                                /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF*/
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) */

#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_EventChangeDtcSuppression
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_EventChangeDtcSuppression(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONST(boolean, AUTOMATIC)  SuppressionStatus
  )
{
  Std_ReturnType lReturnValue = DEM_WRONG_DTC;

  if (Dem_Client_ClientAccess_GetSelectedDTCFormat(ClientId) != DEM_DTC_FORMAT_OBD_3BYTE)
  {
    Dem_EventIdType lEventId = Dem_Client_ClientAccess_GetSelectedEventId(ClientId);
    lReturnValue = Dem_DTC_ChangeDtcSuppression(lEventId, SuppressionStatus);
  }
  
  return lReturnValue;
}                                                                                                                                /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF*/
#endif /* (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON) */

#if (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_EventRetrieveDtcSuppression
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_EventRetrieveDtcSuppression(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  SuppressionStatus
  )
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_Client_ClientAccess_GetSelectedDTCFormat(ClientId) == DEM_DTC_FORMAT_OBD_3BYTE)
  {
    lReturnValue = DEM_WRONG_DTC;
  }
  else
  {
    Dem_EventIdType lEventId = Dem_Client_ClientAccess_GetSelectedEventId(ClientId);
    if (lEventId != DEM_EVENT_INVALID)                                                                                           /* COV_DEM_ROBUSTNESS TX */
    {
      *SuppressionStatus = Dem_DTC_TestDtcSuppression(lEventId);                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lReturnValue = E_OK;
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF*/
#endif /* (DEM_CFG_SUPPORT_DTCSUPPRESSION == STD_ON) */


/* ****************************************************************************
 % Dem_Client_ClientAccess_TestNodeValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestNodeValid(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
  return (boolean) (Dem_ClientData_GetNodeId(ClientId) != DEM_CFG_NMNODEID_INVALID);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ClientId)                                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return FALSE;
#endif
}


/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNodeIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Client_ClientAccess_GetNodeIndex(                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  return Dem_Cfg_FindJ1939NodeIndexFromNmNodeId(Dem_ClientData_GetNodeId(ClientId));
}

#if (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_SetFirstDTCWithLampStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Client_ClientAccess_SetFirstDTCWithLampStatus(
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  Dem_FilterData_HandleType lFilterId;

  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if ( (lFilterId != DEM_CFG_FILTER_INVALID) && (Dem_Client_ClientAccess_TestNodeValid(ClientId) == TRUE) )                             
  {
    Dem_Client_FilterData_SetFirstDTCWithLampStatus(lFilterId, Dem_Client_ClientAccess_GetNodeIndex(ClientId));
  }
}
#endif

#if (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) || (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNextDTCwithLampStatus
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
Dem_Client_ClientAccess_GetNextDTCwithLampStatus(
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_J1939DCM_DATA)  LampStatus,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter
  )
{
  Std_ReturnType lReturnValue;
  Dem_FilterData_HandleType lFilterId;

  lReturnValue = DEM_NO_SUCH_ELEMENT;
  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    if (Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_J1939DTCLAMP) == TRUE)
    {
      Dem_FilterData_ResultType lFilterResult;
      /* MISRA initialization */
      lFilterResult.DTC = 0u;                                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lFilterResult.OccurrenceCounter = 0u;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      (lFilterResult.LampStatus).LampStatus = 0u;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      (lFilterResult.LampStatus).FlashLampStatus = 0u;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

      lReturnValue = Dem_Client_FilterData_GetNextDTCWithLampStatus(lFilterId, &lFilterResult);                                  /* SBSW_DEM_POINTER_FORWARD_STACK */
      if (lReturnValue == E_OK)
      {
        *J1939DTC = lFilterResult.DTC;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        *OccurrenceCounter = lFilterResult.OccurrenceCounter;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        *LampStatus = lFilterResult.LampStatus;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }
  }
  return lReturnValue;
}
#endif /* (DEM_CFG_SUPPORT_J1939 == STD_ON) && (DEM_CFG_SUPPORT_J1939_DM31 == STD_ON) */

/* ****************************************************************************
 % Dem_Client_ClientAccess_SetJ1939DTCFilter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetJ1939DTCFilter(                                                                                       /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH  */
  uint8 ClientId,
  Dem_J1939DcmDTCStatusFilterType DTCStatusFilter,
  Dem_DTCKindType DTCKind,
  Dem_DTCOriginType DTCOrigin,
  CONSTP2VAR(Dem_J1939DcmLampStatusType, AUTOMATIC, DEM_J1939DCM_DATA)  LampStatus
  )
{
  Std_ReturnType lReturnValue;
  Dem_FilterData_HandleType lFilterId = Dem_ClientData_GetFilterId(ClientId);
  uint8 lNodeIndex = Dem_Client_ClientAccess_GetNodeIndex(ClientId);

  if ((lFilterId != DEM_CFG_FILTER_INVALID) && (Dem_Client_ClientAccess_TestNodeValid(ClientId) == TRUE))
  {
    lReturnValue = Dem_Client_FilterData_SetJ1939DTCFilter(lFilterId, 
                                                    DTCStatusFilter,
                                                    DTCKind,
                                                    DTCOrigin,
                                                    lNodeIndex);                                                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    if (lReturnValue == E_OK)
    {
      /* calculate node related lamp status */
      *LampStatus = Dem_GlobalDiagnostics_J1939CalculateNodeRelatedLampStatus(lNodeIndex);                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }

  return lReturnValue;
}

 /* ****************************************************************************
 % Dem_Client_ClientAccess_SetJ1939RatioFilter
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetJ1939RatioFilter(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) IgnitionCycleCounter,
  P2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) OBDMonitoringConditionsEncountered,
  uint8  ClientId
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if ( (Dem_Client_ClientAccess_TestFilterValid(ClientId) == TRUE)
    && (Dem_Client_ClientAccess_TestNodeValid(ClientId) == TRUE) )
  {
    uint16 lIgnitionCycleCounter;
    uint16 lGeneralDenominator;
    Dem_FilterData_HandleType lFilterId = Dem_ClientData_GetFilterId(ClientId);
    uint8 lNodeIndex = Dem_Client_ClientAccess_GetNodeIndex(ClientId);

    lReturnValue = Dem_Client_FilterData_SetJ1939RatioFilter(                                                                    /* SBSW_DEM_POINTER_FORWARD_STACK */
                    lFilterId,
                    lNodeIndex,
                    &lIgnitionCycleCounter,
                    &lGeneralDenominator);

    if (lReturnValue == E_OK)
    {
      *IgnitionCycleCounter = lIgnitionCycleCounter;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *OBDMonitoringConditionsEncountered = lGeneralDenominator;                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }

  return lReturnValue;
}

#if (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNumberOfFilteredJ1939DTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClientAccess_GetNumberOfFilteredJ1939DTC(
  CONST(uint8, AUTOMATIC)  ClientId
  )
{
  uint16 lNumberOfDTCs;
  Dem_FilterData_HandleType lFilterId;

  lNumberOfDTCs = 0u;
  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    if (Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_J1939DTC) == TRUE)
    {
      lNumberOfDTCs = Dem_Client_FilterData_GetNumberOfFilteredJ1939DTC(lFilterId);
    }
  }

  return lNumberOfDTCs;
}
#endif /* (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON) */

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNextFilteredJ1939DTC
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextFilteredJ1939DTC(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint8, AUTOMATIC)  ClientId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,                                                                    /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter                                                             /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;
#if (DEM_CFG_SUPPORT_J1939_READ_DTC == STD_ON)
  Dem_FilterData_HandleType lFilterId;

  lFilterId = Dem_ClientData_GetFilterId(ClientId);

  if (lFilterId != DEM_CFG_FILTER_INVALID)
  {
    if (Dem_Client_FilterData_TestFilterSet(lFilterId, DEM_FILTERDATA_FILTER_J1939DTC) == TRUE)
    {
      Dem_FilterData_ResultType lFilterResult;
      lReturnValue = Dem_Client_FilterData_GetNextFilteredJ1939DTC(lFilterId, &lFilterResult);                                   /* SBSW_DEM_POINTER_FORWARD_STACK */
      if (lReturnValue == E_OK)
      {
        *J1939DTC = lFilterResult.DTC;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        *OccurrenceCounter = lFilterResult.OccurrenceCounter;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ClientId);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(J1939DTC);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OccurrenceCounter);
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNextFilteredJ1939Ratio
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextFilteredJ1939Ratio(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA) SPN,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) Numerator,
  CONSTP2VAR(uint16, AUTOMATIC, DEM_J1939DCM_DATA) Denominator,
  uint8 ClientId
)
{
  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;

  if ( (Dem_Client_ClientAccess_TestFilterValid(ClientId) == TRUE)
    && (Dem_Client_ClientAccess_TestJ1939RatioFilterSet(ClientId) == TRUE) )
  {
    Dem_FilterData_ResultType lFilterResult;
    Dem_FilterData_HandleType lFilterId = Dem_ClientData_GetFilterId(ClientId);

    lReturnValue = Dem_Client_FilterData_GetNextFilteredJ1939Ratio(                                                              /* SBSW_DEM_POINTER_FORWARD_STACK */
                    lFilterId,
                    &lFilterResult);

    if (lReturnValue == E_OK)
    {
      *SPN = lFilterResult.SPN;                                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *Numerator = lFilterResult.Numerator;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *Denominator = lFilterResult.Denominator;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  return lReturnValue;
  
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_ReadDiagnosticReadiness1
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_ReadDiagnosticReadiness1(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8  ClientId,
  CONSTP2VAR(Dem_J1939DcmDiagnosticReadiness1Type, AUTOMATIC, DEM_J1939DCM_DATA)  DiagnosticReadiness
  )
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_Client_ClientAccess_IsJ1939Readiness1Supported() == TRUE)                                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_FilterData_HandleType lFilterId = Dem_ClientData_GetFilterId(ClientId);

    if ((lFilterId != DEM_CFG_FILTER_INVALID)
      && (Dem_Client_ClientAccess_TestNodeValid(ClientId) == TRUE))
    {
      uint8 lNodeIndex = Dem_Client_ClientAccess_GetNodeIndex(ClientId);
      Dem_Client_FilterData_GetDiagnosticReadiness1(lFilterId, lNodeIndex, DiagnosticReadiness);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      lReturnValue = E_OK;
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_ReadDiagnosticReadiness2
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_ReadDiagnosticReadiness2(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8  ClientId,
  CONSTP2VAR(Dem_J1939DcmDiagnosticReadiness2Type, AUTOMATIC, DEM_J1939DCM_DATA)  DiagnosticReadiness2                           /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_Client_ClientAccess_IsJ1939Readiness2Supported() == TRUE)                                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    if (Dem_Client_ClientAccess_TestNodeValid(ClientId) == TRUE)
    {
      uint16 lDistanceSinceMilOn = Dem_GlobalDiagnostics_GetDistanceTravelledSinceMilOn();
      uint16 lDistanceSinceLastClear = Dem_GlobalDiagnostics_GetDistanceTravelledSinceLastClear();
      uint16 lTimeSinceMilOn = Dem_GlobalDiagnostics_GetEngineRuntimeSinceMilOn();
      uint16 lTimeSinceLastClear = Dem_GlobalDiagnostics_GetEngineRuntimeSinceClear();
      lReturnValue = E_OK;

      DiagnosticReadiness2->DistanceTraveledWhileMILisActivated
        = Dem_Infrastructure_Utility_LatchUint16(lDistanceSinceMilOn, DEM_J1939_DM21_MAX_PID_VALUE);                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DiagnosticReadiness2->DistanceSinceDTCsCleared
        = Dem_Infrastructure_Utility_LatchUint16(lDistanceSinceLastClear, DEM_J1939_DM21_MAX_PID_VALUE);                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DiagnosticReadiness2->MinutesRunbyEngineWhileMILisActivated
        = Dem_Infrastructure_Utility_LatchUint16(lTimeSinceMilOn, DEM_J1939_DM21_MAX_PID_VALUE);                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DiagnosticReadiness2->TimeSinceDiagnosticTroubleCodesCleared
        = Dem_Infrastructure_Utility_LatchUint16(lTimeSinceLastClear, DEM_J1939_DM21_MAX_PID_VALUE);                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_ReadDiagnosticReadiness3
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_ReadDiagnosticReadiness3(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8  ClientId,
  CONSTP2VAR(Dem_J1939DcmDiagnosticReadiness3Type, AUTOMATIC, DEM_J1939DCM_DATA) DiagnosticReadiness3
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_Client_ClientAccess_IsJ1939Readiness3Supported() == TRUE)                                                              /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    if (Dem_Client_ClientAccess_TestNodeValid(ClientId) == TRUE)
    {
      Dem_Data_DestinationBuffer lDestinationBuffer;
      /* Initialize bytes 4-8 to static values (Byte 4 = 0x00, Byte 5 = 0x00, Byte 6 = 0x00, Byte 7 = 0x00, Byte 8 = 0x00) */
      uint8 lDm26Buffer[DEM_READINESS_DM26_SIZE] = { 0 };
      uint8 lPID30Value = 0u;
      uint32 lTimeSinceEngineStart = Dem_GlobalDiagnostics_GetEngineRuntimeInSeconds();

      /* Read engine runtime in seconds and latch it */
      if (lTimeSinceEngineStart > 64255u)
      {
        lTimeSinceEngineStart = 64255u;
      }
      DiagnosticReadiness3->TimeSinceEngineStart = (uint16) lTimeSinceEngineStart;                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

      /*Read data of PID30 and latch it */
      lPID30Value = Dem_GlobalDiagnostics_GetWarmUpCycleCountSinceClear();
      DiagnosticReadiness3->NumberofWarmupsSinceDTCsCleared = Dem_Infrastructure_Utility_LatchUint8(lPID30Value, 250U);          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

      Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(lDm26Buffer[0]), DEM_READINESS_DM26_SIZE);                           /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
      Dem_GlobalDiagnostics_ReadDataOfDM26(&lDestinationBuffer);                                                                 /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

      /* Copy the calculated support and status bytes for Readiness 3 */
      DiagnosticReadiness3->ContinuouslyMonitoredSystemsEnableCompletedStatus = lDm26Buffer[0];                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DiagnosticReadiness3->NonContinuouslyMonitoredSystemsEnableStatus5 = lDm26Buffer[1];                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DiagnosticReadiness3->NonContinuouslyMonitoredSystemsEnableStatus6 = lDm26Buffer[2];                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DiagnosticReadiness3->NonContinuouslyMonitoredSystems7 = lDm26Buffer[3];                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      DiagnosticReadiness3->NonContinuouslyMonitoredSystems8 = lDm26Buffer[4];                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

      lReturnValue = E_OK;
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_SetJ1939FreezeFrameIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetJ1939FreezeFrameIterator(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(uint8, AUTOMATIC)  ClientId,
  CONST(Dem_J1939DcmSetFreezeFrameFilterType, AUTOMATIC) FreezeFrameKind
  )
{
  Dem_FreezeFrameIterator_HandleType lFreezeFrameIteratorId;
  Std_ReturnType lReturnValue = E_NOT_OK;

  lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(ClientId);
  if ( (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID) && (Dem_Client_ClientAccess_TestNodeValid(ClientId) == TRUE) )
  {
    Dem_FreezeFrameIterator_StartJ1939Iterator(lFreezeFrameIteratorId, FreezeFrameKind, Dem_Client_ClientAccess_GetNodeIndex(ClientId));
    lReturnValue = E_OK;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNextJ1939FreezeFrame
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextJ1939FreezeFrame(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  uint8  ClientId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  DestBuffer,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  BufSize 
  )
{
  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;
  Dem_FreezeFrameIterator_HandleType lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(ClientId);

  if (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID)
  {
    if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
    {
      if (Dem_FreezeFrameIterator_GetFreezeFrameKind(lFreezeFrameIteratorId) == DEM_J1939DCM_EXPANDED_FREEZEFRAME)
      {
        lReturnValue = Dem_RecordReader_J1939ExpandedFF_GetOBDFreezeFrame(lFreezeFrameIteratorId,                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
                                                                          J1939DTC,
                                                                          OccurrenceCounter,
                                                                          DestBuffer,
                                                                          BufSize);
      }
    }
    else
# if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
    if (Dem_Cfg_IsEmissionObdSupportedInVariant() == FALSE)
    {
      lReturnValue = Dem_FreezeFrameIterator_GetNextJ1939FreezeFrame(lFreezeFrameIteratorId,                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
                                                                    J1939DTC,
                                                                    OccurrenceCounter,
                                                                    DestBuffer,
                                                                    BufSize);
    }
    else
# endif
    {
      /* Empty block for MISRA */
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNextSPNInFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_GetNextSPNInFreezeFrame(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  uint8  ClientId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA) SPN,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  SPNDataLength
  )
{
  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;

  if (Dem_Client_ClientAccess_IsJ1939ExpandedFreezeFrameSupported() == TRUE)                                                     /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_FreezeFrameIterator_HandleType lFreezeFrameIteratorId;
    lFreezeFrameIteratorId = Dem_ClientData_GetFreezeFrameIteratorId(ClientId);

    if (   (lFreezeFrameIteratorId != DEM_CFG_FREEZEFRAMEITERATOR_INVALID)
        && (Dem_Client_ClientAccess_TestNodeValid(ClientId) == TRUE))
    {
      lReturnValue = Dem_FreezeFrameIterator_GetNextSPNInFreezeFrame(lFreezeFrameIteratorId                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
                                                                     , SPN
                                                                     , SPNDataLength);
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_TestExtendedDataFilterValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestExtendedDataFilterValid(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint8 ClientId
)
{
  return (boolean)(Dem_ClientData_GetExtendedDataFilterId(ClientId) != DEM_CFG_FILTER_INVALID);
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_TestExtendedDataFilterSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_TestExtendedDataFilterSet(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint8 ClientId
)
{
  return Dem_Dcm_ERec_0x1916_TestFilterSet((Dem_Dcm_ERec_0x1916_RecordNumberFilter_HandleType)ClientId);
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_SetExtendedDataRecordFilter
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
Dem_Client_ClientAccess_SetExtendedDataRecordFilter(
  uint8 ClientId,
  uint8 RecordNumber
)
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_NOT_OK;

  if (Dem_Dcm_ERec_0x1916_IsService19_16Enabled() == TRUE)
  {
    Dem_Dcm_ERec_0x1916_RecordNumberFilter_HandleType lExtendedDataFilterId;
    lExtendedDataFilterId = Dem_ClientData_GetExtendedDataFilterId(ClientId);
    if (lExtendedDataFilterId != DEM_CFG_FILTER_INVALID)
    {
      lReturnValue = Dem_Dcm_ERec_0x1916_SetRecordFilter(lExtendedDataFilterId, RecordNumber);
    }
  }
  return lReturnValue;
}


/* ****************************************************************************
 % Dem_Client_ClientAccess_GetSizeOfFilteredExtendedDataRecords
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
Dem_Client_ClientAccess_GetSizeOfFilteredExtendedDataRecords(
  uint8 ClientId,
  P2VAR(uint16, AUTOMATIC, DEM_DCM_DATA)  NumberOfFilteredRecords,
  P2VAR(uint16, AUTOMATIC, DEM_DCM_DATA)  SizeOfRecords
)
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_NOT_OK;

  if (Dem_Dcm_ERec_0x1916_IsService19_16Enabled() == TRUE)
  {
    Dem_Dcm_ERec_0x1916_RecordNumberFilter_HandleType lExtendedDataFilterId;
    lExtendedDataFilterId = Dem_ClientData_GetExtendedDataFilterId(ClientId);
    if (lExtendedDataFilterId != DEM_CFG_FILTER_INVALID)
    {
      Dem_Dcm_ERec_0x1916_CalcSizeOfERecs(lExtendedDataFilterId, NumberOfFilteredRecords, SizeOfRecords);                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      lReturnValue = E_OK;
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetNextFilteredERecData
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
Dem_Client_ClientAccess_GetNextFilteredERecData(
  uint8 ClientId,
  P2VAR(uint32, AUTOMATIC, DEM_DCM_DATA) DTC,
  P2VAR(uint8,  AUTOMATIC, DEM_DCM_DATA) DTCStatus,
  P2VAR(uint8,  AUTOMATIC, DEM_DCM_DATA) DestBuffer,
  P2VAR(uint16, AUTOMATIC, DEM_DCM_DATA) BufSize
)
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_NOT_OK;
  
  if (Dem_Dcm_ERec_0x1916_IsService19_16Enabled() == TRUE)
  {
    Dem_Dcm_ERec_0x1916_RecordNumberFilter_HandleType lExtendedDataFilterId;
    lExtendedDataFilterId = Dem_ClientData_GetExtendedDataFilterId(ClientId);
    if (lExtendedDataFilterId != DEM_CFG_FILTER_INVALID)
    {
      lReturnValue = Dem_Dcm_ERec_0x1916_GetNextFilteredERecData(lExtendedDataFilterId, DTC, DTCStatus, DestBuffer, BufSize);    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }

  return lReturnValue;
}


/* ****************************************************************************
 % Dem_Client_ClientAccess_SetDtcFilterByReadinessGroup
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetDtcFilterByReadinessGroup(
  uint8 ClientId,
  Dem_DTCFormatType DTCFormat,
  Dem_EventOBDReadinessGroupType ReadinessGroup
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_FilterData_0x1956_IsService0x1956Enabled() == TRUE)
  {
    Dem_FilterData_HandleType lFilterId = Dem_ClientData_GetFilterId(ClientId);
    if (lFilterId != DEM_CFG_FILTER_INVALID)
    {
      Dem_Client_FilterData_SetDtcFilterByReadinessGroup(lFilterId, DTCFormat, ReadinessGroup);
      lReturnValue = E_OK;
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_SetDTCFilterByExtendedDataRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Client_ClientAccess_SetDTCFilterByExtendedDataRecordNumber(
  uint8 ClientId,
  Dem_DTCFormatType DTCFormat,
  uint8 ExtendedDataRecordNumber
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_FilterData_0x191A_IsService19_1AEnabled() == TRUE)
  {
    Dem_FilterData_HandleType lFilterId = Dem_ClientData_GetFilterId(ClientId);

    if (lFilterId != DEM_CFG_FILTER_INVALID)
    {
      lReturnValue = Dem_Client_FilterData_SetDTCFilterByExtendedDataRecordNumber(lFilterId, DTCFormat, ExtendedDataRecordNumber);
    }
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_IsEventAssignedToNode
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsEventAssignedToNode(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  NodeIndex
  )
{
  boolean lIsEventAssignedToNode = FALSE;
  Dem_Cfg_EventJ1939NodeIterType lNodeIter;


  for (Dem_Cfg_EventJ1939NodeIterInit(EventId, &lNodeIter);                                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventJ1939NodeIterExists(&lNodeIter) == TRUE;                                                                     /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventJ1939NodeIterNext(&lNodeIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    if (Dem_Cfg_EventJ1939NodeIterGet(&lNodeIter) == NodeIndex)                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      lIsEventAssignedToNode = TRUE;
      break;
    }
  }
  return lIsEventAssignedToNode;
} 

/* ****************************************************************************
 % Dem_Client_ClientAccess_GetEventJ1939LampStatus
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
Dem_Client_ClientAccess_GetEventJ1939LampStatus(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  NodeIndex,
  CONSTP2VAR(Dem_FilterData_ResultType, AUTOMATIC, AUTOMATIC)  FilterResult
)
{
  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;
  if (Dem_Client_ClientAccess_IsEventAssignedToNode(EventId, NodeIndex) == TRUE)
  {
    FilterResult->LampStatus = Dem_GlobalDiagnostics_J1939CalculateLampStatusOfEvent(EventId);                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lReturnValue = E_OK;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_IsJ1939FreezeFrameSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939FreezeFrameSupported(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_J1939_FREEZEFRAME == STD_ON);
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_IsJ1939ExpandedFreezeFrameSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939ExpandedFreezeFrameSupported(
  void
  )
{
  return (boolean)(   (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON) 
                   || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME_FOR_OBDII == STD_ON));
}

/* ****************************************************************************
 % Dem_Client_ClientAccess_IsJ1939Readiness3Supported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClientAccess_IsJ1939Readiness3Supported(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_J1939_READINESS3 == STD_ON);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* DEM_CLIENTACCESSIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ClientAccessIF_Implementation.h
 *********************************************************************************************************************/
