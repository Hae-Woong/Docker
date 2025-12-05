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
/*! \addtogroup Dem_MemoriesIF
 *  \{
 *  \file       Dem_MemoriesIF_Types.h
 *  \brief      Abstraction for multiple event memories.
 *  \details    Type definitions of MemoriesIF subcomponent which is facade for all memory related tasks which can be
 *              abstracted.
 *  \entity     MemoriesIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMORIESIF_TYPES_H)
#define DEM_MEMORIESIF_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/* Entry->State currently used bits: 0x89 */
/*! Set not qualified status bits  */
#define DEM_MEM_SET_NON_QUALIFIED_STATUS_BITS(Reg)             ((uint8)((Reg) | 0x77U))
/*! Applies qualified status to event entry status byte */
#define DEM_MEM_APPLY_QUALIFIED_STATUS(State, QualifyStatus)   ((uint8)(((State) & 0x77U) | ((QualifyStatus) & 0x88U)))

#if (DEM_FEATURE_NEED_MIL_GROUPS)
/*! Initialize a MIL group trip counter */
# define Dem_Mem_InitMilGroupTripCount(MilGroupIdx)            (Dem_Event_InternalStatus_SetMilGroupTripCount(MilGroupIdx, 0x00U))
/*! Get a MIL group trip counter */
# define Dem_Mem_GetMilGroupTripCount(MilGroupIdx)             (Dem_Event_InternalStatus_GetMilGroupTripCountRaw(MilGroupIdx) & 0x7Fu)
/*! Increment a MIL group trip counter */
# define Dem_Mem_IncrementMilGroupTripCount(MilGroupIdx)       (Dem_Event_InternalStatus_SetMilGroupTripCount(MilGroupIdx, (uint8)(Dem_Event_InternalStatus_GetMilGroupTripCountRaw(MilGroupIdx) + 1u)))
/*! Set a MIL group trip counter to 0 */
# define Dem_Mem_ResetMilGroupTripCount(MilGroupIdx)           (Dem_Event_InternalStatus_SetMilGroupTripCount(MilGroupIdx, (Dem_Event_InternalStatus_GetMilGroupTripCountRaw(MilGroupIdx) & 0x80U)))

/*! Get MIL group incremented this DCY */
# define Dem_Mem_GetMilGroupIncremented(MilGroupIdx)           (Dem_Event_InternalStatus_GetMilGroupTripCountRaw(MilGroupIdx) & 0x80U)
/*! Set MIL group incremented this DCY */
# define Dem_Mem_SetMilGroupIncremented(MilGroupIdx)           (Dem_Event_InternalStatus_SetMilGroupTripCount(MilGroupIdx, (Dem_Event_InternalStatus_GetMilGroupTripCountRaw(MilGroupIdx) | 0x80U)))
/*! Reset MIL group incremented this DCY */
# define Dem_Mem_ResetMilGroupIncremented(MilGroupIdx)         (Dem_Event_InternalStatus_SetMilGroupTripCount(MilGroupIdx, (Dem_Event_InternalStatus_GetMilGroupTripCountRaw(MilGroupIdx) & 0x7fU)))

/*! Increment pending DTC count */
# define Dem_Mem_IncrementMilGroupPendingCount(MilGroupIdx)    (Dem_Event_InternalStatus_SetMilGroupPendingCount(MilGroupIdx, (uint16)(Dem_Event_InternalStatus_GetMilGroupPendingCount(MilGroupIdx) + 1u)))
/*! Decrement pending DTC count */
# define Dem_Mem_DecrementMilGroupPendingCount(MilGroupIdx)    (Dem_Event_InternalStatus_SetMilGroupPendingCount(MilGroupIdx, (uint16)(Dem_Event_InternalStatus_GetMilGroupPendingCount(MilGroupIdx) - 1u)))
#endif
/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#endif /* DEM_MEMORIESIF_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemoriesIF_Types.h
 *********************************************************************************************************************/
