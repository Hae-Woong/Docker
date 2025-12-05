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
 *  \defgroup   Dem_PermanentEntry
 *  \{
 *  \file       Dem_PermanentEntry_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Interface of the Permanent Entry unit which handles access to and from permanent entries.
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

#if !defined (DEM_PERMANENTENTRY_INTERFACE_H)
#define DEM_PERMANENTENTRY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_PermanentEntry_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PermanentEntry_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_MemoryEntry_PermanentEntry_GetEventId
 *****************************************************************************/
/*!
 * \brief         Get the stored EventId of the permanent entry at position
 *                'Index'.
 *
 * \details       Get the stored EventId of the permanent entry at position
 *                'Index'.
 *
 * \param[in]     Index
 *                Index of the permanent entry
 *
 * \return        The stored EventId of the permanent entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_GetEventId(
  CONST(uint8, AUTOMATIC)  Index
  );

/* ****************************************************************************
 * Dem_MemoryEntry_PermanentEntry_GetStatus
 *****************************************************************************/
/*!
 * \brief         Get the stored status of the permanent entry at position
 *                'Index'.
 *
 * \details       Get the stored status of the permanent entry at position
 *                'Index'.
 *
 * \param[in]     Index
 *                Index of the permanent entry
 *
 * \return        The stored status of the permanent entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_GetStatus(
  CONST(uint8, AUTOMATIC)  Index
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PermanentEntry_Public Public Methods
 * \{
 */


#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PermanentEntry_Free
 *****************************************************************************/
/*!
 * \brief         Frees a permanent memory index
 *
 * \details       Frees a permanent memory index
 *
 * \param[in]     PermanentIndex
 *                Index of a permanent entry
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_Free(
  CONST(uint8, AUTOMATIC)  PermanentIndex
  );
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_PermanentEntry_SetEventId
 *****************************************************************************/
/*!
 * \brief         Set the EventId of the permanent entry at position 'Index'.
 *
 * \details       Set the EventId of the permanent entry at position 'Index'.
 *
 * \param[in]     Index
 *                Index of the permanent entry
 * \param[in]     EventId
 *                New EventId of permanent entry at position 'index'
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_SetEventId(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_MemoryEntry_PermanentEntry_SetStatus
 *****************************************************************************/
/*!
 * \brief         Set the status of the permanent entry at position 'Index'.
 *
 * \details       Set the status of the permanent entry at position 'Index'.
 *
 * \param[in]     Index
 *                Index of the permanent entry
 * \param[in]     Status
 *                New status of permanent entry at position 'index'
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_SetStatus(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(uint8, AUTOMATIC)  Status
  );

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent
 *****************************************************************************/
/*!
 * \brief         Searches an eventId in the permanent memory
 *
 * \details       Searches an eventId in the permanent memory
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Index of the permanent entry used by the event. If no entry
 *                is found, Dem_Cfg_GlobalPermanentMaxCount() is returned.
 *
 * \pre           The passed event must be a master event
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_GetEntryIndexOfEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_PermanentEntry_UpdateVisiblity
 *****************************************************************************/
/*!
 * \brief         Mark permanent entry of event VISIBLE
 *
 * \details       Mark permanent entry of event VISIBLE
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PermanentEntry_UpdateVisiblity(
 CONST(Dem_EventIdType, AUTOMATIC)  EventId
 );


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_PERMANENTENTRY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PermanentEntry_Interface.h
 *********************************************************************************************************************/
