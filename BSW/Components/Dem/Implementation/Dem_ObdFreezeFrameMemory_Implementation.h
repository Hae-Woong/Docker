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
/*! \addtogroup Dem_ObdFreezeFrameMemory
 *  \{
 *  \file       Dem_ObdFreezeFrameMemory_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Implementation of the Permanent Memory subcomponent which manages storage of permanent DTCs.
 *  \entity     ObdFreezeFrameMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_OBDFREEZEFRAMEMEMORY_IMPLEMENTATION_H)
#define DEM_OBDFREEZEFRAMEMEMORY_IMPLEMENTATION_H

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
#include "Dem_ObdFreezeFrameMemory_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Cfg_Declarations.h"
#include "Dem_ObdFreezeFrameEntry_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_OBDFREEZEFRAMEMEMORY_IMPLEMENTATION_FILENAME "Dem_ObdFreezeFrameMemory_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Freeze frame index visible in mode 0x02 */
DEM_LOCAL VAR(uint8, DEM_VAR_NO_INIT)          Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02;                                  /* PRQA S 3218 */ /* MD_DEM_8.7 */

#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ObdFreezeFrameMemory_PrivateProperties Private Properties
 * \{
 */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
* Dem_Memories_ObdFreezeFrameMemory_IsFreezeFrameVisibleInMode02
*****************************************************************************/
/*!
* \brief         Tests if a freeze frame is visible in mode 02.
*
* \details       Tests if a freeze frame is visible in mode 02
*
* \return        TRUE:  Freeze frame visible in mode 02
*                FALSE: No freeze frame visible in mode 02
*
* \pre           -
* \config        DEM_CFG_SUPPORT_OBDII == STD_ON
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_IsFreezeFrameVisibleInMode02(                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  );
#endif

/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_IsPendingStorageSupported
 *****************************************************************************/
/*!
 * \brief        Tests if storage of OBD Freeze Frame at pending is supported.
 *
 * \details      Tests if storage of OBD Freeze Frame at pending is supported.
 *
 *
 * \return       TRUE
 *               storage of OBD Freeze Frame at pending is supported.
 * \return       FALSE
 *               storage of OBD Freeze Frame at pending is not supported
 * \pre          -
 * \config       -
 *
 * \context      TASK
 * \synchronous  TRUE
 * \reentrant    TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_IsPendingStorageSupported(
  void
  );

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_IsFreezeFrameVisibleInMode02
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_IsFreezeFrameVisibleInMode02(                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
void
)
{
  return (Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 != Dem_Memories_ObdFreezeFrameMemory_GetSize());
}
#endif

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_IsPendingStorageSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_IsPendingStorageSupported(
  void
  )
{
  return DEM_CFG_SUPPORT_PENDING_STORAGE_FREEZE_FRAME == STD_ON;
}


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
 * \addtogroup Dem_ObdFreezeFrameMemory_PublicProperties
 * \{
 */

/* ****************************************************************************
% Dem_Memories_ObdFreezeFrameMemory_GetFreezeFrameIndexMode02
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_GetFreezeFrameIndexMode02(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  void
  )
{
#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_OBDII == STD_ON)
  return Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02;
# else
  return 0u;
# endif
}

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_GetObdOnUdsSRecNumberByFreezeFrameType
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_GetObdOnUdsSRecNumberByFreezeFrameType(                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
)
{
  uint8 lSRecNumber;
  if (FreezeFrameType == Dem_RecordReader_0x1904_OBDonUDSFF_0xF0)
  {
    lSRecNumber = DEM_RECORDREADER_SNAPSHOTDATARECORD_OBDONUDS_UPDATEABLE;
  }
  else
  {
    lSRecNumber = DEM_RECORDREADER_SNAPSHOTDATARECORD_OBD;
  }

  return lSRecNumber;
}


/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_GetSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_GetSize(
  void
  )
{
  uint8 lMemorySize = 0u;
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
  lMemorySize = Dem_Cfg_GetMaxNumberEventEntryOBDFreezeFrame();
#endif
  return lMemorySize;
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
 * \defgroup Dem_ObdFreezeFrameMemory_Private Private Methods
 * \{
 */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
* Dem_Memories_ObdFreezeFrameMemory_FindDisplaceablePendingFreezeFrame
*****************************************************************************/
/*!
* \brief         Selects a displaceable Obd Freeze Frame entry
*
* \details       This function checks for pending freeze frames which can be
*                displaced by a confirmed freeze frame
*
* \param[in]     ReplaceMisfireOrFuelSys
*                TRUE: Replace freeze frames of misfire or fuel system events
*                FALSE: Do not replace freeze frames of misfire or fuel system
*                        events
*
* \return        Index of the displaceable entry. If displacing of pending
*                freeze frames is disabled, the number of configured
*                freeze frames will be returned.
*
* \pre           -
* \config        DEM_CFG_SUPPORT_OBDII == STD_ON && DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_FindDisplaceablePendingFreezeFrame(
CONST(boolean, AUTOMATIC)  ReplaceMisfireOrFuelSys
);
#endif

/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_TestPendingStorageSupported
 *****************************************************************************/
/*!
 * \brief         Tests if freeze frame storage is valid for a certain event.
 *
 * \details       Tests if freeze frame storage is valid for a certain event,
 *                depending on obd freeze frame storage configuration.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE:  Freeze frame storage for event is valid
 *                FALSE: Freeze frame storage for event is invalid
 *
 * \pre           Only invoked if OBDII or OBDOnUDS is enabled in variant
 * \config        -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_TestPendingStorageSupported(
  Dem_EventIdType  EventId
  );


#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_FindDisplaceableInvisibleFreezeFrameForVisibleEvent
 *****************************************************************************/
/*!
 * \brief         Selects a displaceable Obd Freeze Frame entry for the freeze
 *                frame of an immediately visible event.
 *                Determines, if freeze frame visibility must be updated.
 *
 * \details       This function checks, if the freeze frame of a visible event
 *                is allowed to displace an invisible freeze frame related to 
 *                a non-misfire and non-fuel system event.
 *                It also determines, if an invisible freeze frame related to
 *                a misfire or fuel system event must be made visible
 *                immediately.
 *
 * \param[out]    SetHighPrioFFVisibleBeforeDcyQualified
 *                TRUE: invisible freeze frame related to a misfire or fuel
 *                system event must be set visible before DCY qualification
 *                FALSE: no visibility change required
 *
 * \return        Index of the displaceable entry. If no index is found, the
 *                number of configured freeze frames will be returned.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON &&
 *                DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON &&
 *                DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_FindDisplaceableInvisibleFreezeFrameForVisibleEvent(
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) SetHighPrioFFVisibleBeforeDcyQualified
  );
#endif


#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_SetOldestHighPrioFreezeFrameVisible
 *****************************************************************************/
/*!
 * \brief         Marks the oldest freeze frame related to a misfire or fuel
 *                system event as visible
 *
 * \details       Searches for the oldest invisible freeze frame slot related
 *                to a misfire and fuel system event and marks it as visible

 * \return        EventId of the event whose slot was marked as visible
 * \return        DEM_EVENT_INVALID if no invisible freeze frame related to a
 *                misfire or fuel system event is stored
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON &&
 *                DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SetOldestHighPrioFreezeFrameVisible(
  void
  );
#endif


/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_SelectFFDisplaceIndex
 *****************************************************************************/
/*!
 * \brief         Selects Obd freeze frame index for displacement.
 *                Determines, if freeze frame visibility must be updated
 *
 * \details       Selects lowest prior Obd freeze frame index for displacement.
 *                It also determines, if an invisible freeze frame related to a
 *                misfire or fuel system event must be made visible immediately.
 *                If no less prior entry than Event Id is found,
 *                an invalid index is returned.
 *
 * \param[in]     EventId
 *                EventId of the freeze frame that needs to be stored
 * \param[in]     DisplacePendingFreezeFrame
 *                TRUE: trigger and configuration for displacing a pending 
 *                      freeze frame is given
 *                FALSE: displacing pending freeze frames with confirmed is
 *                       either excluded by configuration or the trigger is not
 *                       confirmed
 * \param[out]    SetHighPrioFFVisibleBeforeDcyQualified
 *                TRUE: invisible freeze frame related to a misfire or fuel
 *                      system event must be set visible before DCY
 *                      qualification
 *                FALSE: no visibility change required
 *
 * \return        Index of the freeze frame to displace if found, otherwise
 *                Dem_Memories_ObdFreezeFrameMemory_GetSize()
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SelectFFDisplaceIndex(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  DisplacePendingFreezeFrame,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) SetHighPrioFFVisibleBeforeDcyQualified
  );

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_FindDisplaceablePendingFreezeFrame
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_FindDisplaceablePendingFreezeFrame(
  CONST(boolean, AUTOMATIC)  ReplaceMisfireOrFuelSys
  )
{
  uint8 lSelectedIndex;
  Dem_EventIdType lEventId;

  lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(0);

  if ((Dem_Event_UDSStatus_TestBit(Dem_Core_Event_GetInternalUdsStatus(lEventId), Dem_UdsStatus_CDTC) == FALSE)
    && ((ReplaceMisfireOrFuelSys == TRUE)
    || ( (ReplaceMisfireOrFuelSys == FALSE)
      && (Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(lEventId) == FALSE))
    ))
  {
    lSelectedIndex = 0u;
  }
  else
  {
    lSelectedIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  }

# if (DEM_CFG_GLOBAL_FREEZEFRAME_SIZE > 1u)                                                                                      /* COV_DEM_UNSUPPORTED_FEATURE_COMBINATION XF */
  /* Function is designed for a single freeze frame slot only. */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif

  return lSelectedIndex;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_TestPendingStorageSupported
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_TestPendingStorageSupported(
  Dem_EventIdType EventId
  )
{
  Dem_DTC_UDSStatusType lDTCStatus;
  boolean lIsFreezeFrameStorageValid = FALSE;
  lDTCStatus = Dem_Core_Event_GetInternalUdsStatus(EventId);

  if ((Dem_Event_UDSStatus_TestBit(lDTCStatus, Dem_UdsStatus_CDTC) == TRUE))
  {
    /* If the CDTC Bit is set the freeze frame storage is always valid */
    lIsFreezeFrameStorageValid = TRUE;
  }
  else
  {
    if ( (Dem_Memories_ObdFreezeFrameMemory_IsPendingStorageSupported() == TRUE) 
      || (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE) )
    { 
      /* For freeze frame storage at pending (always for OBDonUDS, configurable for OBDII) 
      the freeze frame storage is only valid if PDTC is set */
      if (Dem_Event_UDSStatus_TestBit(lDTCStatus, Dem_UdsStatus_PDTC) == TRUE)
      {
        lIsFreezeFrameStorageValid = TRUE;
      }
    }
  }
  
  return lIsFreezeFrameStorageValid;
}


#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
% Dem_Memories_ObdFreezeFrameMemory_FindDisplaceableInvisibleFreezeFrameForVisibleEvent
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_FindDisplaceableInvisibleFreezeFrameForVisibleEvent(
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) SetHighPrioFFVisibleBeforeDcyQualified
  )
{
  uint8 lSelectedIndex;
  lSelectedIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();

  if (Dem_Memories_ObdFreezeFrameMemory_IsFreezeFrameVisibleInMode02() == FALSE)
  {
    lSelectedIndex = Dem_Memories_ObdFreezeFrameMemory_SelectNewestLowPrioFF(FALSE);
    if (lSelectedIndex == Dem_Memories_ObdFreezeFrameMemory_GetSize())
    {
      *SetHighPrioFFVisibleBeforeDcyQualified = TRUE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }

  return lSelectedIndex;
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
% Dem_Memories_ObdFreezeFrameMemory_SetOldestHighPrioFreezeFrameVisible
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
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SetOldestHighPrioFreezeFrameVisible(
void
)
{
  Dem_EventIdType lEventId;
  uint8 lFreezeFrameIndex;

  lEventId = DEM_EVENT_INVALID;
  lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_SelectOldestLowPrioFF(FALSE);

  if (lFreezeFrameIndex != Dem_Memories_ObdFreezeFrameMemory_GetSize())
  {
    lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex);
    if (lEventId != DEM_EVENT_INVALID)
    {
      Dem_MemoryEntry_ObdFreezeFrameEntry_SetVisibility(lFreezeFrameIndex, TRUE);
    }
  }

  return lEventId;
}
#endif

/* ****************************************************************************
% Dem_Memories_ObdFreezeFrameMemory_SelectFFDisplaceIndex
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SelectFFDisplaceIndex(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  DisplacePendingFreezeFrame,
  CONSTP2VAR(boolean, AUTOMATIC, AUTOMATIC) SetHighPrioFFVisibleBeforeDcyQualified                                               /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  uint8 lBestCandidate;
# if (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
  boolean lReplaceMisfireOrFuelSys;
#endif
  lBestCandidate = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacePendingFreezeFrame);                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SetHighPrioFFVisibleBeforeDcyQualified);                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  if (Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(EventId) == TRUE)
  {
    lBestCandidate = Dem_Memories_ObdFreezeFrameMemory_SelectNewestLowPrioFF(FALSE);
    if (lBestCandidate == Dem_Memories_ObdFreezeFrameMemory_GetSize())
    {
      lBestCandidate = Dem_Memories_ObdFreezeFrameMemory_SelectNewestLowPrioFF(TRUE);
# if (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
      if ((lBestCandidate != Dem_Memories_ObdFreezeFrameMemory_GetSize())
        && (DEM_OPERATIONCYCLE_TEST_DCY_QUALIFIED(Dem_OperationCycle_GetObdCycleStates()) == FALSE))                             
      {
        *SetHighPrioFFVisibleBeforeDcyQualified = TRUE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
#endif
    }
# if (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
    lReplaceMisfireOrFuelSys = TRUE;
#endif
  }
# if (DEM_CFG_SUPPORT_PENDING_DISPLACE_FREEZE_FRAME == STD_ON)
  else
  {
    lReplaceMisfireOrFuelSys = FALSE;
  }
  if ( (lBestCandidate == Dem_Memories_ObdFreezeFrameMemory_GetSize())
    && (DisplacePendingFreezeFrame == TRUE)
    )
  {
    /* All OBD freeze frame slots occupied by high prio monitors */
    lBestCandidate = Dem_Memories_ObdFreezeFrameMemory_FindDisplaceablePendingFreezeFrame(lReplaceMisfireOrFuelSys);
  }
#endif
# if (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
  if( ( lBestCandidate == Dem_Memories_ObdFreezeFrameMemory_GetSize())
    && (Dem_Event_TestObdVisibilityDelayed(EventId) == FALSE))
  {
    lBestCandidate = Dem_Memories_ObdFreezeFrameMemory_FindDisplaceableInvisibleFreezeFrameForVisibleEvent(SetHighPrioFFVisibleBeforeDcyQualified); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
#endif

  return lBestCandidate;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacePendingFreezeFrame);                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SetHighPrioFFVisibleBeforeDcyQualified);                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  return Dem_Memories_ObdFreezeFrameMemory_GetSize();
#endif
}

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
 * \addtogroup Dem_ObdFreezeFrameMemory_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_ProcessAgingFinished
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_ProcessAgingFinished(
  Dem_EventIdType EventId
)
{
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
    Dem_Memories_ObdFreezeFrameMemory_ClearOBDFreezeFrame(EventId);
    Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 = Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame();
  }
}

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_RestoreMemory
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_RestoreMemory(
  void
)
{
  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    {
      /* Delegate OBD Freeze Frames restore */
      Dem_Memories_ObdFreezeFrameMemory_InitRestoreMemoryObd();

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
      if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
      {
        /* set the freeze frame index for active slot in Mode02 */
        Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 = Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame();
      }
#endif
    }
  }
}

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_SelectNewestLowPrioFF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SelectNewestLowPrioFF(
  CONST(boolean, AUTOMATIC)  SelectVisible
  )
{
  uint8 lBestCandidate;
  uint8 lFreezeFrameIndex;
  uint32 lHighestTimestamp;

  lBestCandidate = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  lHighestTimestamp = 0u;


  while (lFreezeFrameIndex != 0U)
  {
    Dem_EventIdType lEventId;

    lFreezeFrameIndex--;
    lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex);

    if (Dem_MemoryEntry_ObdFreezeFrameEntry_TestVisible(lFreezeFrameIndex) == SelectVisible)
    {
      if ((Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(lEventId) == FALSE)
        && (Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(lFreezeFrameIndex) > lHighestTimestamp))
      {
        lBestCandidate = lFreezeFrameIndex;
        lHighestTimestamp = Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(lFreezeFrameIndex);
      }
      else
      {
        /* no candidate found, lBestCandidate is still Dem_Memories_ObdFreezeFrameMemory_GetSize()*/
      }
    }
  }
  return lBestCandidate;
}
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
% Dem_Memories_ObdFreezeFrameMemory_SelectOldestLowPrioFF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SelectOldestLowPrioFF(
  CONST(boolean, AUTOMATIC)  SelectVisible
  )
{
  uint8 lBestCandidate;
  uint8 lFreezeFrameIndex;
  uint32 lLowestTimestamp;

  lBestCandidate = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  lLowestTimestamp = 0xFFFFFFFFU;


  while (lFreezeFrameIndex != 0U)
  {
    Dem_EventIdType lEventId;

    lFreezeFrameIndex--;
    lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex);

    if (Dem_MemoryEntry_ObdFreezeFrameEntry_TestVisible(lFreezeFrameIndex) == SelectVisible)
    {
      if ((Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(lEventId) == TRUE)
        && (Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(lFreezeFrameIndex) < lLowestTimestamp))                                             
      {
        lBestCandidate = lFreezeFrameIndex;
        lLowestTimestamp = Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(lFreezeFrameIndex);
      }
      else
      {
        /* no candidate found, lBestCandidate is still Dem_Memories_ObdFreezeFrameMemory_GetSize()*/
      }
    }
  }
  return lBestCandidate;
}
#endif

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_FindOBDFreezeFrameSlot
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_FindOBDFreezeFrameSlot(
  CONSTP2VAR(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext,
  CONST(boolean, AUTOMATIC) DisplacePendingFreezeFrame
  )
{
  Dem_EventIdType lMemoryRepresentativeEventId;
  uint8 lSelectedIndex;
  uint8 lFreezeFrameIndex;

  lMemoryRepresentativeEventId = Dem_Cfg_GetMemoryRepresentative(DTCContext->EventId);

  lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  lSelectedIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();

  /* Find the stored freeze frame for the event (using the master event in
      case of event combination) */
  while (lFreezeFrameIndex != 0U)                                                                                                /* PRQA S 2994 */ /* MD_DEM_2994 */
  {
    Dem_EventIdType lEventId;

    lFreezeFrameIndex--;
    lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex);

    if (lEventId == lMemoryRepresentativeEventId)
    {
      lSelectedIndex = (uint8)lFreezeFrameIndex;
      break;
    }
    if (lEventId == DEM_EVENT_INVALID)
    {
      lSelectedIndex = (uint8)lFreezeFrameIndex;
    }
  }

  if (( lSelectedIndex == Dem_Memories_ObdFreezeFrameMemory_GetSize()) && (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE))
  {
    /* If configured and no empty slot or stored freeze frame for the event was found,
       search for displaceable freeze frame */
    lSelectedIndex = Dem_Memories_ObdFreezeFrameMemory_SelectFFDisplaceIndex(lMemoryRepresentativeEventId,                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
                                                                 DisplacePendingFreezeFrame,
                                                                 &(DTCContext->QualifyDTC));
  }

  return lSelectedIndex;
}                                                                                                                                /* PRQA S 6050, 6010 */ /* MD_MSR_STCAL, MD_MSR_STPTH */

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_UpdateObdOnUdsFreezeFrame
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_UpdateObdOnUdsFreezeFrame(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_EventIdType EventId,
  uint8 FreezeFrameSlot 
  )
{
#if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)
  Dem_EventIdType lMemoryRepresentativeEventId;

  lMemoryRepresentativeEventId = DEM_EVENT_INVALID;

  if (FreezeFrameSlot != Dem_Memories_ObdFreezeFrameMemory_GetSize())
  {
    lMemoryRepresentativeEventId = Dem_Cfg_GetMemoryRepresentative(EventId);
    if (Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(FreezeFrameSlot) != lMemoryRepresentativeEventId)
    {
     /* Initial storage of OBD FF */
     Dem_MemoryEntry_ObdFreezeFrameEntry_SetEventId(FreezeFrameSlot, lMemoryRepresentativeEventId);
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
      if (Dem_StorageManager_DataStorage_PrestorageApplyObdFF(
        Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0x00),        /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
        Dem_Cfg_GlobalObdFreezeFrameMaxSize()) == FALSE)
# endif
      {
        Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame(EventId,
          Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0x00),      /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
          Dem_Cfg_GlobalObdFreezeFrameMaxSize());
      }

      /* Create a copy of 0x00 OBD FreezeFrame for 0xF0 OBD FreezeFrame */
      Dem_MemCpy(Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0xF0), /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
        Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0x00),
        Dem_Cfg_GlobalObdFreezeFrameMaxSize());
    }
    else
    {
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
      if (Dem_StorageManager_DataStorage_PrestorageApplyObdFF(
        Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0xF0),        /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
        Dem_Cfg_GlobalObdFreezeFrameMaxSize()) == FALSE)
# endif
      {
        /* On update */
        Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame(EventId,
          Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(FreezeFrameSlot, Dem_RecordReader_0x1904_OBDonUDSFF_0xF0),      /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
          Dem_Cfg_GlobalObdFreezeFrameMaxSize());
      }
    } 

    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
                                DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
  }

#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameSlot)
#endif
}                                                                                                                                /* PRQA S 6050, 6010 */ /* MD_MSR_STCAL, MD_MSR_STPTH */

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_InitRestoreMemoryObd
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
Dem_Memories_ObdFreezeFrameMemory_InitRestoreMemoryObd(
  void
)
{
  /* If the primary entry of a freeze frame event is lost, also remove the
  freeze frame. The most probably scenario to get this state is to clear
  DTCs and NvM restores the previous freeze frame due to CRC errors */
  uint8 lFreezeFrameIndex;
  Dem_EventIdType lEventId;

  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)                                                                     /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();
    while (lFreezeFrameIndex != 0U)                                                                                              /* PRQA S 2994 */ /* MD_DEM_2994 */
    {
      lFreezeFrameIndex--;
      lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex);
      if (lEventId != DEM_EVENT_INVALID)
      {
        if ((Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY) < lEventId)    /* Omitted first event check due to coverage, case covered by DEM_EVENT_INVALID. */
          || (Dem_Cfg_GetMemoryRepresentative(lEventId) != lEventId)
          || (Dem_Memories_ObdFreezeFrameMemory_TestPendingStorageSupported(lEventId) == FALSE))                                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        {
          /* Invalidate the freeze frame */
          Dem_MemoryEntry_ObdFreezeFrameEntry_Free(lFreezeFrameIndex);
          Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
            DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame
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
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame(                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
  )
{
  uint8 lBestCandidate;
  uint8 lFreezeFrameIndex;
  uint32 lLowestTimestamp;

  lBestCandidate = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  lLowestTimestamp = 0xFFFFFFFFU;

  for (lFreezeFrameIndex = 0u; lFreezeFrameIndex < Dem_Memories_ObdFreezeFrameMemory_GetSize(); lFreezeFrameIndex++)             /* PRQA S 2994 */ /* MD_DEM_2994 */
  {
    if (Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex) == DEM_EVENT_INVALID)
    { /* Freeze Frame slot not in use, continue with next slot */
      continue;
    }

    if (Dem_MemoryEntry_ObdFreezeFrameEntry_TestVisible(lFreezeFrameIndex) == FALSE)
    { /* Freeze Frame not visible, continue with next slot */
      continue;
    }

    if (Dem_Core_DTC_IsObdRelated(Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex)) == FALSE)
    { /* Event of stored Freeze Frame is not OBD related (anymore), continue with next slot */
      continue;
    }

    if (Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex)) == TRUE)
    {
      if ((Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lBestCandidate)) == FALSE)
        || (Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(lFreezeFrameIndex) < lLowestTimestamp))
      {/* current candidate has higher prio readiness group than best candidate
          or has same prio readiness group but is older than best candidate or
          best candidate is not set yet -> set new best candidate and continue
          with next slot */
        lBestCandidate = lFreezeFrameIndex;
        lLowestTimestamp = Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(lBestCandidate);
      }
    }
    /* Best candidate has low prio readiness group or is not set yet */
    else if (Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lBestCandidate)) == FALSE)
    {
      if (Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(lFreezeFrameIndex) < lLowestTimestamp)
      { /* current and best candidate have the same priority so if timestamp of current index is older than best candidate-> set new best candidate and continue with next slot */
        lBestCandidate = lFreezeFrameIndex;
        lLowestTimestamp = Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(lBestCandidate);
      }
    }
    else
    {
      /* no candidate found, lBestCandidate is still Dem_Memories_ObdFreezeFrameMemory_GetSize()*/
    }
  }

  return lBestCandidate;
}

#if ((DEM_CFG_SUPPORT_OBDII == STD_ON) \
  && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON))
/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_SetFFOfEventVisible
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SetFFOfEventVisible(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  )
{
  uint8 lFreezeFrameIndex;
  Dem_EventIdType lMemoryRepresentativeEvent;

  lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);

  while (lFreezeFrameIndex != 0U)
  {
    lFreezeFrameIndex--;

    if (Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex) == lMemoryRepresentativeEvent)
    {
      Dem_MemoryEntry_ObdFreezeFrameEntry_SetVisibility(lFreezeFrameIndex, TRUE);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
        DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  }
}
#endif

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_UpdateOBDIIFreezeFrame
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
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_UpdateOBDIIFreezeFrame(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_EventIdType EventId,
  uint8 FreezeFrameSlot,
  uint8 RequiredUpdates
  )
{
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  Dem_EventIdType lMemoryRepresentativeEventId;

  lMemoryRepresentativeEventId = DEM_EVENT_INVALID;

  if ((RequiredUpdates & DEM_DATA_FF_UPDATE_REQUIRED) != 0u)
  {
    if (FreezeFrameSlot != Dem_Memories_ObdFreezeFrameMemory_GetSize())
    {
      lMemoryRepresentativeEventId = Dem_Cfg_GetMemoryRepresentative(EventId);
      if ((RequiredUpdates & DEM_DATA_FF_UPDATE_DATA) != 0u)
      {
        if (Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(FreezeFrameSlot) != lMemoryRepresentativeEventId)
        {
          /* initial storage of OBD FF, no update*/
          Dem_MemoryEntry_ObdFreezeFrameEntry_SetVisibility(FreezeFrameSlot, FALSE);
          Dem_MemoryEntry_ObdFreezeFrameEntry_SetEventId(FreezeFrameSlot, lMemoryRepresentativeEventId);
        }
        Dem_MemoryEntry_ObdFreezeFrameEntry_SetTimestamp(FreezeFrameSlot, Dem_FaultMemory_Memories_GetCurrentTimestamp());
        Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
# if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
        if (Dem_StorageManager_DataStorage_PrestorageApplyObdFF(Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdIIDataPtr(FreezeFrameSlot), /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
          Dem_Cfg_GlobalObdFreezeFrameMaxSize()) == FALSE)
# endif
        {
          Dem_StorageManager_DataStorage_CollectObdIIFreezeFrame(EventId,
            Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdIIDataPtr(FreezeFrameSlot),                                                /* SBSW_DEM_POINTER_OBD_FREEZEFRAME_BUFFER */
            Dem_Cfg_GlobalObdFreezeFrameMaxSize());
        }
      }
      else
      {
        /* No data update, check if time stamp update is needed by visibility
           change at confirmation */
        if ((RequiredUpdates & DEM_DATA_FF_UPDATE_TIMESTAMP) != 0u)
        {
          Dem_MemoryEntry_ObdFreezeFrameEntry_SetTimestamp(FreezeFrameSlot, Dem_FaultMemory_Memories_GetCurrentTimestamp());
          Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
        }
      }
      if ((RequiredUpdates & DEM_DATA_FF_UPDATE_VISIBILITY) != 0u)
      {
        Dem_MemoryEntry_ObdFreezeFrameEntry_SetVisibility(FreezeFrameSlot, TRUE);
      }

    }
# if (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
    else
    {
      if ((RequiredUpdates & DEM_DATA_FF_UPDATE_VISIBILITY) != 0u)
      {
        lMemoryRepresentativeEventId = Dem_Memories_ObdFreezeFrameMemory_SetOldestHighPrioFreezeFrameVisible();
      }
    }
# endif
    Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 = Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame();
  }

  if (RequiredUpdates != 0u)
  {
    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
                                DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
  }

  return lMemoryRepresentativeEventId;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameSlot)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(RequiredUpdates)
  return DEM_EVENT_INVALID;
#endif
}                                                                                                                                /* PRQA S 6050, 6010 */ /* MD_MSR_STCAL, MD_MSR_STPTH */

/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_ClearOBDFreezeFrame
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
Dem_Memories_ObdFreezeFrameMemory_ClearOBDFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  if ((Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE) &&
      (Dem_Core_DTC_IsObdRelated(EventId) == TRUE))
  { 
    /* Clear the OBD FreezeFrame (if available) */
    uint8 lSelectedIndex;
    lSelectedIndex = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEntryIndexOfEvent(EventId);

    if (lSelectedIndex != Dem_Memories_ObdFreezeFrameMemory_GetSize())
    { /* No need to clear the complete FreezeFrame entry as it is only valid if the EventId is valid */
      Dem_MemoryEntry_ObdFreezeFrameEntry_Free(lSelectedIndex);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_OBDFREEZEFRAME),
        DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
      /* if this event is visible in Mode02, clear this cache also */
      if ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE) && (Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 == lSelectedIndex))
      {
        Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 = Dem_Memories_ObdFreezeFrameMemory_GetSize();
      }
#endif
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON) || (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) )
/* ****************************************************************************
 % Dem_Memories_ObdFreezeFrameMemory_EventDisplaced
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
Dem_Memories_ObdFreezeFrameMemory_EventDisplaced(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  {
    if ( (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
      || ( (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE) 
        && (Dem_Event_UDSStatus_TestBit(Dem_Core_Event_GetInternalUdsStatus(EventId), Dem_UdsStatus_CDTC) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(Dem_Core_Event_GetInternalUdsStatus(EventId), Dem_UdsStatus_PDTC) == FALSE) ) )
    {
      Dem_Memories_ObdFreezeFrameMemory_ClearOBDFreezeFrame(EventId);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_OBDFREEZEFRAMEMEMORY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ObdFreezeFrameMemory_Implementation.h
 *********************************************************************************************************************/
