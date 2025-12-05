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
/*! \addtogroup Dem_0x1956
 *  \{
 *  \file       Dem_0x1956_Implementation.h
 *  \brief      Subcomponent for handling Service 0x19 Subfunction 0x56.
 *  \details    Subcomponent for handling Service 0x19 Subfunction 0x56, which filters DTCs and their UDS status that are part of a client requested readiness group.
 *  \entity     0x1956
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_0X1956_IMPLEMENTATION_H)
#define DEM_0X1956_IMPLEMENTATION_H

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
#include "Dem_0x1956_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FilterDataIF_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_0X1956_IMPLEMENTATION_FILENAME "Dem_0x1956_Implementation.h"

 /* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_0x1956_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

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
 * \addtogroup Dem_0x1956_PublicProperties
 * \{
 */

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
 * \defgroup Dem_0x1956_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

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
 * \addtogroup Dem_0x1956_Public
 * \{
 */

 /* ****************************************************************************
 % Dem_FilterData_0x1956_IsService0x1956Enabled
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FilterData_0x1956_IsService0x1956Enabled(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_SVC19_56 == STD_ON);
}


/* ****************************************************************************
 % Dem_FilterData_0x1956_SetDtcFilterByReadinessGroup
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FilterData_0x1956_SetDtcFilterByReadinessGroup(
  Dem_FilterData_HandleType DTCFilterId,
  Dem_EventOBDReadinessGroupType ReadinessGroupId
)
{
  Dem_Cfg_FilterInfoTablePtrType lFilterDataPtr;
  lFilterDataPtr = Dem_Client_FilterData_GetFilterDataPtrSafe(DTCFilterId);

  lFilterDataPtr->FilterDtcStrategy = Dem_FilterData_FilterEventBased_ReadinessGroup;                                            /* SBSW_DEM_ARRAY_POINTER_FILTERDATA */
  lFilterDataPtr->TableIndex.ReadinessGroupId = ReadinessGroupId;                                                                /* SBSW_DEM_ARRAY_POINTER_FILTERDATA */
  Dem_GlobalDiagnostics_InitReadinessEventIter(ReadinessGroupId, &lFilterDataPtr->Iterator.ReadinessGroupEventIter);             /* SBSW_DEM_POINTER_FILTERDATA_STRUCT_MEMBER_ARGUMENT */
  Dem_Client_FilterData_InitCombinedGroupProcessed(lFilterDataPtr);                                                              /* SBSW_DEM_ARRAY_POINTER_FILTERDATA_ARGUMENT */
}


/* ****************************************************************************
 % Dem_FilterData_0x1956_GetNumberOfFilteredDTCsByReadinessGroup
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
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FilterData_0x1956_GetNumberOfFilteredDTCsByReadinessGroup(                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventOBDReadinessGroupType ReadinessGroupId
)
{
  uint16 lCount;
  Dem_Cfg_ComplexIterType lReadinessEventIter;
  Dem_Cfg_ProcessedCombinedDTCGroupType lProcessedCombinedDTCGroups;
  
  lCount = 0;
  Dem_DTC_InitCombinedGroupProcessed(lProcessedCombinedDTCGroups);                                                               /* SBSW_DEM_POINTER_FORWARD_STACK */

  for (Dem_GlobalDiagnostics_InitReadinessEventIter(ReadinessGroupId, &lReadinessEventIter);                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    (Dem_Cfg_ComplexIterExists(&lReadinessEventIter) == TRUE);                                                                   /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lReadinessEventIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lEventId = Dem_GlobalDiagnostics_ReadinessEventIterGetEventId(&lReadinessEventIter);                         /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if ( (Dem_DTC_TestCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lEventId), lProcessedCombinedDTCGroups) == FALSE )       /* PRQA S 3415 */ /* MD_DEM_13.5_volatile */ /* SBSW_DEM_POINTER_FORWARD_STACK */
      && (Dem_Cfg_GetMemoryRepresentative(lEventId) == lEventId)
      && (Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
      && (Dem_DTC_TestSuppressionByRepresentiveEvent(lEventId) == FALSE))                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      Dem_DTC_SetCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lEventId), lProcessedCombinedDTCGroups);                      /* SBSW_DEM_POINTER_FORWARD_STACK */
      lCount++;
    }
  }

  return lCount;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_FilterData_0x1956_FilterDTCByReadinessGroup
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
DEM_LOCAL FUNC(Dem_EventIdType, DEM_CODE)
Dem_FilterData_0x1956_FilterDTCByReadinessGroup(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_FilterData_InfoType, AUTOMATIC, AUTOMATIC)  DTCFilterPtr                                                    /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  Dem_EventIdType lReturnEventIdValue = DEM_EVENT_INVALID;

  for (/*Already initialized on SetFilter - */;                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    (Dem_Cfg_ComplexIterExists(&DTCFilterPtr->Iterator.ReadinessGroupEventIter) == TRUE);                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&DTCFilterPtr->Iterator.ReadinessGroupEventIter))                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {                                                                                                                              
    Dem_EventIdType lEventId = Dem_GlobalDiagnostics_ReadinessEventIterGetEventId(&DTCFilterPtr->Iterator.ReadinessGroupEventIter); /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if ( (Dem_Client_FilterData_TestCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lEventId), DTCFilterPtr) == FALSE)         /* PRQA S 3415 */ /* MD_DEM_13.5_volatile */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      && (Dem_Cfg_GetMemoryRepresentative(lEventId) == lEventId)
      && (Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
      && (Dem_DTC_TestSuppressionByRepresentiveEvent(lEventId) == FALSE))                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      lReturnEventIdValue = lEventId;   
      Dem_Client_FilterData_SetCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lEventId), DTCFilterPtr);                       /* PRQA S 2469 */ /* MD_DEM_14.2_FalsePositive */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      Dem_Cfg_ComplexIterNext(&DTCFilterPtr->Iterator.ReadinessGroupEventIter);                                                  /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      break;
    }
  }

  return lReturnEventIdValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_0X1956_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_0x1956_Implementation.h
 *********************************************************************************************************************/
