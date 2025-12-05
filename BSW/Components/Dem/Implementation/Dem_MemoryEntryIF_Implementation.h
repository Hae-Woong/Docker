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
/*! \addtogroup Dem_MemoryEntryIF
 *  \{
 *  \file       Dem_MemoryEntryIF_Implementation.h
 *  \brief      Abstraction for multiple event memories.
 *  \details    Implementation of MemoryIF subcomponent which is facade for all memory related tasks.
 *  \entity     MemoryEntryIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMORYENTRYIF_IMPLEMENTATION_H)
#define DEM_MEMORYENTRYIF_IMPLEMENTATION_H

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
#include "Dem_MemoryEntryIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/ - units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_MEMORYENTRYIF_IMPLEMENTATION_FILENAME "Dem_MemoryEntryIF_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
/*!
 * \defgroup Dem_Memory_PrivateProperties Private Properties
 * \{
 */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"

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
 * \addtogroup Dem_Memory_PublicProperties
 * \{
 */


#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*!
 * \}
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_Private Private Methods
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
 * \addtogroup Dem_Memory_Public
 * \{
 */

/* ****************************************************************************
 % Dem_FaultMemory_MemoryEntry_ChronoIter_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_Init(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId,
  CONSTP2VAR(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  Dem_Internal_AssertReturnVoid(TranslatedMemoryId != DEM_CFG_MEMORYID_PERMANENT, DEM_E_INCONSISTENT_STATE)

  IterPtr->mIdx = 0u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->TranslatedMemoryId = TranslatedMemoryId;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_FaultMemory_MemoryEntry_ChronoIter_Resume
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_Resume(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId,
  CONST(Dem_Cfg_EntryIndexType, AUTOMATIC)  StoredChronoIter,
  CONSTP2VAR(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  Dem_Internal_AssertReturnVoid(TranslatedMemoryId != DEM_CFG_MEMORYID_PERMANENT, DEM_E_INCONSISTENT_STATE)
  IterPtr->mIdx = StoredChronoIter;                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->TranslatedMemoryId = TranslatedMemoryId;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_FaultMemory_MemoryEntry_ChronoIter_Next
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_Next(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2VAR(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx += 1u;                                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_FaultMemory_MemoryEntry_ChronoIter_Exists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_Exists(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (boolean)(IterPtr->mIdx < IterPtr->mEndIdx);
}


/* ****************************************************************************
 % Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return Dem_FaultMemory_Memories_GetChronology(IterPtr->TranslatedMemoryId,
         (uint8)((IterPtr->mEndIdx - 1u) - IterPtr->mIdx ));
}

/* ****************************************************************************
 % Dem_FaultMemory_MemoryEntry_ChronoIter_GetCurrentIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_GetCurrentIndex(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return (Dem_Cfg_EntryIndexType)IterPtr->mIdx;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMORYENTRYIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemoryEntryIF_Implementation.h
 *********************************************************************************************************************/
