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
/*! \addtogroup Dem_PermanentEntry
 *  \{
 *  \file       Dem_PermanentEntry_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Implementation of the Permanent Entry unit which handles access to and from permanent entries.
 *  \entity     PermanentEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PERMANENTENTRY_IMPLEMENTATION_H)
#define DEM_PERMANENTENTRY_IMPLEMENTATION_H

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
#include "Dem_PermanentEntry_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Cfg_Declarations.h"
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FaultMemory_Interface.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_PERMANENTENTRY_IMPLEMENTATION_FILENAME "Dem_PermanentEntry_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_START_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_STOP_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PermanentEntry_PrivateProperties Private Properties
 * \{
 */



/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

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
 * \addtogroup Dem_PermanentEntry_PublicProperties
 * \{
 */


/* ****************************************************************************
 % Dem_MemoryEntry_PermanentEntry_GetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_GetEventId(
  CONST(uint8, AUTOMATIC)  Index
  )
{
  Dem_EventIdType lPermanentEntryEventId = DEM_EVENT_INVALID;
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  lPermanentEntryEventId = Dem_Cfg_PermanentData.PermanentDataEntry[Index].EventId;                                              /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  return lPermanentEntryEventId;
}

/* ****************************************************************************
 % Dem_MemoryEntry_PermanentEntry_GetStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_GetStatus(
  CONST(uint8, AUTOMATIC)  Index
  )
{
  uint8 lPermanentStatus = DEM_ESM_PERMANENT_NONE;
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  lPermanentStatus = Dem_Cfg_PermanentData.PermanentDataEntry[Index].Status;                                                     /* PRQA S 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  return lPermanentStatus;
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
 * \defgroup Dem_PermanentEntry_Private Private Methods
 * \{
 */



/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

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
 * \addtogroup Dem_PermanentEntry_Public
 * \{
 */


#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PermanentEntry_Free
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
Dem_MemoryEntry_PermanentEntry_Free(
  CONST(uint8, AUTOMATIC)  PermanentIndex
  )
{
  /* Free the given permanent slot */
  Dem_MemoryEntry_PermanentEntry_SetEventId(PermanentIndex, DEM_EVENT_INVALID);

  Dem_Internal_AssertReturnVoid(Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) > 0, DEM_E_INCONSISTENT_STATE)

  Dem_FaultMemory_Memories_SetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, Dem_FaultMemory_Memories_GetCurrentSize(DEM_CFG_MEMORYID_PERMANENT) - 1u);
  Dem_MemoryEntry_PermanentEntry_SetStatus(PermanentIndex, DEM_ESM_PERMANENT_NONE);
  Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT), 
                              DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
}                                                                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_PermanentEntry_SetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_SetEventId(                                                                                       /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint8, AUTOMATIC)  Index,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= Dem_Cfg_GlobalPermanentMaxCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PERMANENTENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_PermanentData.PermanentDataEntry[Index].EventId = EventId;                                                           /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */  /* SBSW_DEM_ARRAY_WRITE_PERMANENTDATA_PERMANENTDATAENTRY */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_PermanentEntry_SetStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_SetStatus(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(uint8, AUTOMATIC)  Status
  )
{
#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= Dem_Cfg_GlobalPermanentMaxCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PERMANENTENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                          /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_PermanentData.PermanentDataEntry[Index].Status = Status;                                                             /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */  /* SBSW_DEM_ARRAY_WRITE_PERMANENTDATA_PERMANENTDATAENTRY */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Status)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint8 lPermanentIndex;

  for (lPermanentIndex = 0u; lPermanentIndex < Dem_Cfg_GlobalPermanentMaxCount(); ++lPermanentIndex)
  {
    if (Dem_MemoryEntry_PermanentEntry_GetEventId(lPermanentIndex) == EventId)
    {
      break;
    }
  }
  return lPermanentIndex;
}
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_PermanentEntry_UpdateVisiblity
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_UpdateVisiblity(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
 CONST(Dem_EventIdType, AUTOMATIC)  EventId
 )
{
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) && (DEM_CFG_SUPPORT_PERMANENTDTC_IMMEDIATE_VISIBLE == STD_ON)
  uint8 lPermanentIndex;
  uint8 lPermanentState;
  Dem_EventIdType lMasterEventId;

  lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);

  /* scan through the permanent memory if the given event is available */
  lPermanentIndex = Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(lMasterEventId);
  if (lPermanentIndex != Dem_Cfg_GlobalPermanentMaxCount())
  {
    lPermanentState = Dem_MemoryEntry_PermanentEntry_GetStatus(lPermanentIndex);
    if (DEM_ESM_TEST_PERMANENT_VISIBLE(lPermanentState) == FALSE)
    {
      lPermanentState = DEM_ESM_SET_PERMANENT_VISIBLE(lPermanentState);
      Dem_MemoryEntry_PermanentEntry_SetStatus(lPermanentIndex, lPermanentState);

      /* persist status change in NVM */
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_PERMANENT),
        DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_PERMANENTENTRY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PermanentEntry_Implementation.h
 *********************************************************************************************************************/
