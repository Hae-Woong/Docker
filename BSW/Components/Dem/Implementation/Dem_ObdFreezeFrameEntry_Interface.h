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
 *  \defgroup   Dem_ObdFreezeFrameEntry Dem_ObdFreezeFrameEntry
 *  \{
 *  \file       Dem_ObdFreezeFrameEntry_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the Obd Freeze Frame Memory subcomponent which manages storage and erasing of the 
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

#if !defined (DEM_OBDFREEZEFRAMEENTRY_INTERFACE_H)
#define DEM_OBDFREEZEFRAMEENTRY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_ObdFreezeFrameEntry_Types.h"


/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ObdFreezeFrameMemory_PublicProperties Public Properties
 * \{
 */

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_SetVisibility
 *****************************************************************************/
/*!
 * \brief         Set the state of the visibility flag of the freeze frame at
 *                position 'Index'.
 *
 * \details       Set the state of the visibility flag of the freeze frame at
 *                position 'Index'.
 *
 * \param[in]     Index
 *                Index of the freeze frame
 * \param[in]     Visible
 *                TRUE: Set the visibility flag of the freeze frame entry
 *                FALSE: Reset the visibility flag of the freeze frame entry
 *
 * \return        The stored EventId of the freeze frame
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_SetVisibility(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(boolean, AUTOMATIC)  Visible
  );  
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_TestVisible
 *****************************************************************************/
/*!
 * \brief         Tests whether a given freeze frame slot contains a visible
 *                freeze frame
 *
 * \details       Tests whether a given freeze frame slot contains a visible
 *                freeze frame
 *
 * \param[in]     Index
 *                Index of the freeze frame
 * \return        TRUE: Freeze frame stored at index is visible
 *                FALSE: Freeze frame stored at index is not visible
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_TestVisible(
  CONST(uint8, AUTOMATIC) Index
  );

/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId
 *****************************************************************************/
 /*!
  * \brief         Get the stored EventId of the freeze frame at position
  *                'Index'.
  *
  * \details       If the given slot is occupied, returns the event Id
  *                otherwise returns the invalid event Id
  *
  * \param[in]     Index
  *                Index of the freeze frame
  *
  * \return        The stored EventId of the freeze frame
  * \pre           -
  * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(
  uint8 Index
);

/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_SetEventId
 *****************************************************************************/
/*!
 * \brief         Set the EventId of the freeze frame at position 'Index'.
 *
 * \details       Set the EventId of the freeze frame at position 'Index'.
 *
 * \param[in]     Index
 *                Index of the freeze frame
 * \param[in]     EventId
 *                New EventId of freeze frame at position 'index'
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_SetEventId(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp
 *****************************************************************************/
/*!
 * \brief         Get the stored timestamp of the freeze frame at position
 *                'Index'.
 *
 * \details       Get the stored timestamp of the freeze frame at position
 *                'Index'.
 *
 * \param[in]     Index
 *                Index of the freeze frame
 *
 * \return        The stored timestamp of the freeze frame
 *
 * \pre           Should only be called in OBDII configurations or variants
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetTimestamp(
  CONST(uint8, AUTOMATIC)  Index
  );



#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_SetTimestamp
 *****************************************************************************/
/*!
 * \brief         Set the timestamp of the freeze frame at position 'Index'.
 *
 * \details       Set the timestamp of the freeze frame at position 'Index'.
 *
 * \param[in]     Index
 *                Index of the freeze frame
 * \param[in]     Timestamp
 *                New timestamp of freeze frame at position 'index'
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_SetTimestamp(
  CONST(uint8, AUTOMATIC)  Index,
  CONST(uint32, AUTOMATIC)  Timestamp
  );
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdIIDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the data pointer for OBDII FreezeFrame data of an OBD
 *                freezeframe entry
 *
 * \details       Get the data pointer for OBDII FreezeFrame data of an OBD
 *                freezeframe entry
 *
 * \param[in]     Index
 *                Index of the freeze frame data
 *
 * \return        Pointer to OBD FreezeFrame data
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdIIDataPtr(
  uint8  Index
  );
#endif

#if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the data pointer for OBD on UDS FreezeFrame data of an OBD
 *                freezeframe entry
 *
 * \details       Get the data pointer for OBD on UDS FreezeFrame data of an OBD
 *                freezeframe entry
 *
 * \param[in]     Index
 *                Index of the freeze frame data
 * \param[in]     FreezeFrameType
 *                0 OBD FreezeFrame 0x00
 *                1 OBD FreezeFrame 0xF0
 *
 * \return        Pointer to OBD FreezeFrame data
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDONUDS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(
  uint8 Index,
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
);
#endif

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
 * \defgroup Dem_ObdFreezeFrameEntry_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_Free
 *****************************************************************************/
/*!
 * \brief         Marks freeze frame entry as unused
 *
 * \details       Reset eventId of a freeze frame entry
 *
 * \param[in]     Index
 *                Index of the freeze frame
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_Free(
  CONST(uint8, AUTOMATIC) Index
  );  

/* ****************************************************************************
 * Dem_MemoryEntry_ObdFreezeFrameEntry_GetEntryIndexOfEvent
 *****************************************************************************/
/*!
 * \brief         Find the freeze frame index for the given event
 *
 * \details       This function scans the available freeze frame slots looking
 *                for the given event. If the event does not have a stored
 *                freeze frame, the maximum size for the freeze frame memory is
 *                returned.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Freeze Frame Index of the event if stored, otherwise
 *                Dem_Memories_ObdFreezeFrameMemory_GetSize()
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_ObdFreezeFrameEntry_GetEntryIndexOfEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_OBDFREEZEFRAMEENTRY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ObdFreezeFrameEntry_Interface.h
 *********************************************************************************************************************/
