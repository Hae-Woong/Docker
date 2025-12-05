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
/*!
 *  \defgroup   Dem_Memories Dem Memories
 *  \{
 *  \file       Dem_Memories_Interface.h
 *  \brief      Abstraction for multiple event memories.
 *  \details    Public Interfaces of Memories subcomponent which is facade for all memory related tasks.
 *  \entity     Memories
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMORIES_INTERFACE_H)
#define DEM_MEMORIES_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_PermanentMemory_Interface.h"
#include "Dem_ObdFreezeFrameMemory_Interface.h"
#include "Dem_TimeSeriesMemory_Interface.h"
#include "Dem_CustomTriggerMemory_Interface.h"
#include "Dem_PrestorageMemory_Interface.h"
#include "Dem_EventMemory_Interface.h"
#include "Dem_MemoriesIF_Interface.h"

#endif /* DEM_MEMORIES_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Memories_Interface.h
 *********************************************************************************************************************/
