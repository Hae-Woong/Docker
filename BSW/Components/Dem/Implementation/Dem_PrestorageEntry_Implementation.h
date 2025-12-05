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
/*! \addtogroup Dem_PrestorageEntry
 *  \{
 *  \file       Dem_PrestorageEntry_Implementation.h
 *  \brief      Stores and manages single entries of 'prestored freezeframes'.
 *  \details    Implementation of the Prestorage Entry unit which handles access to and from prestored freezeframe entries.
 *  \entity     PrestorageEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PRESTORAGEENTRY_IMPLEMENTATION_H)
#define DEM_PRESTORAGEENTRY_IMPLEMENTATION_H

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
#include "Dem_PrestorageEntry_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Cfg_Declarations.h"
#include "Dem_Infrastructure_Interface.h"


/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_PRESTORAGEENTRY_IMPLEMENTATION_FILENAME "Dem_PrestorageEntry_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PrestorageEntry_PrivateProperties Private Properties
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
 * \addtogroup Dem_PrestorageEntry_PublicProperties
 * \{
 */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_Checked
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_Checked(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  Dem_Cfg_PrestorageIndexType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (PrestorageEntry > Dem_Cfg_GlobalPrestorageSize())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PRESTORAGEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    /* return index of the temporary buffer */
    lReturnValue = Dem_Cfg_GlobalPrestorageSize();
  }
  else
# endif
  {
    lReturnValue = PrestorageEntry;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_GetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetEventId(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  return Dem_Cfg_GetPrestorage()[PrestorageEntry].EventId;
}
#endif


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_SetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_SetEventId(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_GetPrestorage()[Dem_MemoryEntry_PrestorageEntry_Checked(PrestorageEntry)].EventId = EventId;                           /* SBSW_DEM_ARRAY_WRITE_PRESTORAGE */
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_GetGlobalSRecDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetGlobalSRecDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  Dem_DataPtrType lPrestorageSnapshot;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (PrestorageEntry > Dem_Cfg_GlobalPrestorageSize())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PRESTORAGEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lPrestorageSnapshot = &(Dem_Cfg_GetPrestorage()[0].GlobalSnapshotData[0]);
  }
  else
# endif
  {
    lPrestorageSnapshot = &(Dem_Cfg_GetPrestorage()[PrestorageEntry].GlobalSnapshotData[0]);
  }

  return lPrestorageSnapshot;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_GetSRecDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetSRecDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  Dem_DataPtrType lPrestorageSnapshot;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (PrestorageEntry > Dem_Cfg_GlobalPrestorageSize())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PRESTORAGEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lPrestorageSnapshot = &(Dem_Cfg_GetPrestorage()[0].SnapshotData[0]);
  }
  else
# endif
  {
    lPrestorageSnapshot = &(Dem_Cfg_GetPrestorage()[PrestorageEntry].SnapshotData[0]);
  }

  return lPrestorageSnapshot;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_GetObdFFDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetObdFFDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  Dem_DataPtrType lPrestorageOBDFreezeFrame;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (PrestorageEntry > Dem_Cfg_GlobalPrestorageSize())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PRESTORAGEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lPrestorageOBDFreezeFrame = &(Dem_Cfg_GetPrestorage()[0].ObdFreezeFrame[0]);
  }
  else
# endif
  {
    lPrestorageOBDFreezeFrame = &(Dem_Cfg_GetPrestorage()[PrestorageEntry].ObdFreezeFrame[0]);
  }

  return lPrestorageOBDFreezeFrame;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && ((DEM_CFG_SUPPORT_J1939_FREEZEFRAME == STD_ON) || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_GetJ1939FFDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetJ1939FFDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  Dem_DataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (PrestorageEntry > Dem_Cfg_GlobalPrestorageSize())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PRESTORAGEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_Cfg_GetPrestorage()[0].J1939FreezeFrame[0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_Cfg_GetPrestorage()[PrestorageEntry].J1939FreezeFrame[0]);
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_GetWwhObdFFDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetWwhObdFFDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  Dem_DataPtrType lPrestorageSnapshot;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (PrestorageEntry > Dem_Cfg_GlobalPrestorageSize())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PRESTORAGEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lPrestorageSnapshot = &(Dem_Cfg_GetPrestorage()[0].ObdFreezeFrameData[0]);
  }
  else
# endif
  {
    lPrestorageSnapshot = &(Dem_Cfg_GetPrestorage()[PrestorageEntry].ObdFreezeFrameData[0]);
  }
  return lPrestorageSnapshot;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_GetState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetState(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  return Dem_Cfg_GetPrestorage()[PrestorageEntry].State;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_SetState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_SetState(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry,
  CONST(uint8, AUTOMATIC)  State
  )
{
  Dem_Cfg_GetPrestorage()[ Dem_MemoryEntry_PrestorageEntry_Checked(PrestorageEntry) ].State = State;                             /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_PRESTORAGE */
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_SetEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_SetEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_Cfg_EventPrestorageIndex(EventId) >= Dem_Cfg_GetSizeOfPrestorageIndex())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PRESTORAGEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_SetPrestorageIndex(Dem_Cfg_EventPrestorageIndex(EventId), PrestorageEntry);                                          /* SBSW_DEM_ARRAY_WRITE_PRESTORAGEINDEX */
  }
}
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_EventHasEntryAssigned
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_EventHasEntryAssigned(
  Dem_EventIdType EventId
)
{
#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  Dem_Cfg_PrestorageIndexType lPrestorageEntry;
#endif

  boolean lReturnValue = FALSE;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  lPrestorageEntry = Dem_MemoryEntry_PrestorageEntry_GetEntry(EventId);

  if (lPrestorageEntry < DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
  {
    lReturnValue = TRUE;
  }
#endif

  return lReturnValue;
}


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_PrestorageEntry_GetEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Internal_AssertReturnValue(DEM_PRESTORAGEMEMORY_NOT_CONFIGURED_FOR_EVENT != Dem_Cfg_EventPrestorageIndex(EventId), DEM_E_PARAM_DATA, DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
  return Dem_Cfg_GetPrestorageIndex(Dem_Cfg_EventPrestorageIndex(EventId));
}
#endif


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
 * \defgroup Dem_PrestorageEntry_Private Private Methods
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
 * \addtogroup Dem_PrestorageEntry_Public
 * \{
 */


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_PRESTORAGEENTRY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PrestorageEntry_Implementation.h
 *********************************************************************************************************************/
