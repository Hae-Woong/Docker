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
/*! \addtogroup ObdFreezeFrameEntry
 *  \{
 *  \file       Dem_ObdFreezeFrameEntry_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of the Obd Freeze Frame Memory subcomponent which manages storage and erasing of the 
 *              legislative Obd Freeze Frame
 *  \entity     ObdFreezeFrameEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_H)
#define DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_H

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
#include "Dem_ObdFreezeFrameEntry_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Client_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_FILENAME "Dem_ObdFreezeFrameEntry_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
/*!
 * \defgroup Dem_ObdFreezeFrameEntry_PrivateProperties Private Properties
 * \{
 */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_GetVisibility
 *****************************************************************************/
/*!
 * \brief         Get the state of the visibility flag of the freeze frame at
 *                position 'Index'.
 *
 * \details       Get the state of the visibility flag of the freeze frame at
 *                position 'Index'.
 *
 * \param[in]     Index
 *                Index of the freeze frame
 *
 * \return        The visibility flag of the freeze frame
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *                && DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetVisibility(
  CONST(uint8, AUTOMATIC)  Index
  );
#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_GetVisibility
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetVisibility(
  CONST(uint8, AUTOMATIC)  Index
  )
{
  return Dem_Cfg_FreezeFrameData[Index].IsVisible;
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
 * \addtogroup Dem_ObdFreezeFrameEntry_PublicProperties
 * \{
 */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON) 
/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_SetVisibility
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_SetVisibility(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(boolean, AUTOMATIC)  Visible
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= DEM_CFG_GLOBAL_FREEZEFRAME_SIZE)
  {
    Dem_Error_RunTimeCheckFailed(DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_FreezeFrameData[Index].IsVisible = Visible;                                                                          /* PRQA S 2842 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_FREEZEFRAMEDATA */
  }
}
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_TestVisible
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_TestVisible(
  CONST(uint8, AUTOMATIC) Index
  )
{
  boolean lVisible = FALSE;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index);
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
# if (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
  lVisible = Dem_MemoryEntry_ObdFreezeFrameEntry_GetVisibility(Index);
# else
  lVisible = TRUE;
# endif
#endif
  return lVisible;
}

/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(
  uint8 Index
)
{
  Dem_EventIdType lEventId = DEM_EVENT_INVALID;
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
  if (Index < Dem_Memories_ObdFreezeFrameMemory_GetSize())
  {
    lEventId = Dem_Cfg_FreezeFrameData[Index].EventId;                                                                           /* PRQA S 2841, 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */ 
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index);
#endif
  return lEventId;
}

/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_SetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_SetEventId(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= DEM_CFG_GLOBAL_FREEZEFRAME_SIZE)
  {
    Dem_Error_RunTimeCheckFailed(DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_FreezeFrameData[Index].EventId = EventId;                                                                            /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */  /* SBSW_DEM_ARRAY_WRITE_FREEZEFRAMEDATA */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  Index
  )
{
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  return Dem_Cfg_FreezeFrameData[Index].Timestamp;                                                                               /* PRQA S 2842 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)
  return 0;
#endif
}

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_SetTimestamp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_SetTimestamp(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  Index,
  CONST(uint32, AUTOMATIC)  Timestamp
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= DEM_CFG_GLOBAL_FREEZEFRAME_SIZE)
  {
    Dem_Error_RunTimeCheckFailed(DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_FreezeFrameData[Index].Timestamp = Timestamp;                                                                        /* SBSW_DEM_ARRAY_WRITE_FREEZEFRAMEDATA */
  }
}
#endif


#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdIIDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdIIDataPtr(
  uint8 Index
  )
{
  Dem_SharedDataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= DEM_CFG_GLOBAL_FREEZEFRAME_SIZE)
  {
    Dem_Error_RunTimeCheckFailed(DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_Cfg_FreezeFrameData[0].FreezeFrameData[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_Cfg_FreezeFrameData[Index].FreezeFrameData[0][0]);
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(
  uint8 Index,
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
  )
{
  Dem_SharedDataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((Index >= DEM_CFG_GLOBAL_FREEZEFRAME_SIZE)
    || ((uint8)FreezeFrameType >= DEM_CFG_MAX_NUM_OBDFREEZEFRAME))
  {
    Dem_Error_RunTimeCheckFailed(DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_Cfg_FreezeFrameData[0].FreezeFrameData[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_Cfg_FreezeFrameData[Index].FreezeFrameData[FreezeFrameType][0]);
  }
  return lReturnValue;
}
#endif

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
 * \defgroup Dem_ObdFreezeFrameEntry_Private Private Methods
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
 * \addtogroup Dem_ObdFreezeFrameEntry_Public
 * \{
 */

/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_Free
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_Free(
  CONST(uint8, AUTOMATIC) Index
  )
{
  Dem_MemoryEntry_ObdFreezeFrameEntry_SetEventId(Index, DEM_EVENT_INVALID);
}

/* ****************************************************************************
 % Dem_MemoryEntry_ObdFreezeFrameEntry_GetEntryIndexOfEvent
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetEntryIndexOfEvent(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint8 lSelectedIndex;

  lSelectedIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
    Dem_EventIdType lMemoryRepresentativeEvent;
    uint8 lFreezeFrameIndex;

    lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);
    lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();

    while (lFreezeFrameIndex != 0U)                                                                                              /* PRQA S 2994 */ /* MD_DEM_2994 */
    {
      lFreezeFrameIndex--;

      if (Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex) == lMemoryRepresentativeEvent)
      { /* Requested event has a FreezeFrame stored */
        lSelectedIndex = lFreezeFrameIndex;
        break;
      }
    }
  }

  return lSelectedIndex;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_OBDFREEZEFRAMEENTRY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ObdFreezeFrameEntry_Implementation.h
 *********************************************************************************************************************/
