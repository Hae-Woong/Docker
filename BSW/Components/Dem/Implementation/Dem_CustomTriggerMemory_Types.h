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
/*! \addtogroup Dem_CustomTriggerMemory
 *  \{
 *  \file       Dem_CustomTriggerMemory_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Type definition of the Custom Trigger Memory subcomponent which manages the dedicated memory for custom triggered
 *              snapshot data and time series snapshot data.
 *  \entity     CustomTriggerMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CUSTOMTRIGGERMEMORY_TYPES_H)
#define DEM_CUSTOMTRIGGERMEMORY_TYPES_H

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
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/
 
/*!
 * \defgroup  Dem_CustomTriggerMemory_QueueStates  Macros encoding prestorage queue states
 * Macros encoding custom triggered freeze frame queue state
 * \{
 */
# define DEM_CUSTOMTRIGGERMEMORY_QUEUE_INITIAL                  ((uint8)(0x00U))  /*!< Initial state */
# define DEM_CUSTOMTRIGGERMEMORY_QUEUE_STOREFF                  ((uint8)(0x10U))  /*!< Custom triggered Freeze Frame shall be stored/updated */

typedef uint8 Dem_Memories_CustomTriggerMemory_QueueActionType;

/*! Invalid record number for custom triggered snapshot record */
# define DEM_CUSTOMTRIGGERMEMORY_SREC_NUMBER_INVALID                             ((uint8)0xffU)
/*!
 * \}
 */
 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#endif /* DEM_CUSTOMTRIGGERMEMORY_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CustomTriggerMemory_Types.h
 *********************************************************************************************************************/
