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
 *  \defgroup   Dem_ObdFreezeFrameMemory MemPermanent
 *  \{
 *  \file       Dem_ObdFreezeFrameMemory_Interface.h
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

#if !defined (DEM_OBDFREEZEFRAMEMEMORY_INTERFACE_H)
#define DEM_OBDFREEZEFRAMEMEMORY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_ObdFreezeFrameMemory_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ObdFreezeFrameMemory_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
* Dem_Memories_ObdFreezeFrameMemory_GetFreezeFrameIndexMode02
*****************************************************************************/
/*!
* \brief         Returns the index of OBD Freeze Frame index visible in mode 02.
*
* \details       Returns the index of OBD Freeze Frame index visible in mode 02.
*
* \return        Index
*
* \pre           -
* \config        -
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_GetFreezeFrameIndexMode02(
  void
  );

/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_GetObdOnUdsSRecNumberByFreezeFrameType
 *****************************************************************************/
 /*!
 * \brief         Gets SRec number based on OBD on UDS FF type.
 *
 * \details       Gets SRec number based on OBD on UDS FF type.
 *
 * \param[in]     FreezeFrameType
 *                0 OBD FreezeFrame 0x00
 *                1 OBD FreezeFrame 0xF0
 *
 * \return        SRec Number of provided OBD on UDS FF type.
 * \pre           -
 * \config        -
 *
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_GetObdOnUdsSRecNumberByFreezeFrameType(
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
);


/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_GetSize
 *****************************************************************************/
/*!
 * \brief         Get the number of stored OBD-II or
 *                OBD on UDS freeze frame slots
 *
 * \details       Get the number of stored OBD-II or
 *                OBD on UDS freeze frame slots in variant.
 *                Depending on which version is supported by the configuration
 *                or variant.
 *
 * \return        The number of OBD-II or OBD on UDS freeze frame slots
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_GetSize(
  void
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
 * \defgroup Dem_ObdFreezeFrameMemory_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_ProcessAgingFinished
 *****************************************************************************/
/*!
 * \brief         Processes OBD FF relevant effects of finished aging.
 *
 * \details       -
 *
 * \pre           Function is only called for events which can age (event supports
 *                aging and has an aging target cycle count (aging for all DTCs 
 *                supported or event is stored))
 * \config        Feature OBD II, OBDonUDS is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_ProcessAgingFinished(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_RestoreMemory
 *****************************************************************************/
/*!
 * \brief         Initializes OBD Freeze Frame memory
 *
 * \details       This function verifies the integrity of OBD freeze frame entries and
 *                links them up into the runtime control structures.
 *
 * \pre           NvM must have restored NV mirrors
 * \config        Feature OBD II, OBDonUDS or WWH-OBD is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_RestoreMemory(
  void
  );


#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_SelectNewestLowPrioFF
 *****************************************************************************/
/*!
 * \brief         Selects the index of the newest Obd freeze frame with low prio 
 *                readiness group
 *
 * \details       If there are multiple low prio Obd freeze frames stored 
 *                select the one with most recent timestamp
 *
 * \param[in]     SelectVisible
 *                TRUE:  Select newest low prio visible freeze frame
 *                FALSE: Select newest low prio invisible freeze frame
 *
 * \return        Freeze Frame Index of the event if found, otherwise
 *                Dem_Memories_ObdFreezeFrameMemory_GetSize()
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SelectNewestLowPrioFF(
  CONST(boolean, AUTOMATIC)  SelectVisible
  );
#endif


#if (DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_STORAGE_FREEZE_FRAME == STD_ON) && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON)
/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_SelectOldestLowPrioFF
 *****************************************************************************/
/*!
 * \brief         Selects the index of the oldest Obd freeze frame with high
 *                prio readiness group
 *
 * \details       If there are multiple high prio Obd freeze frames stored
 *                select the one with oldest timestamp
 *
 * \param[in]     SelectVisible
 *                TRUE:  Select oldest high prio visible freeze frame
 *                FALSE: Select oldest high prio invisible freeze frame
 *
 * \return        Freeze Frame Index of the event if found, otherwise
 *                Dem_Memories_ObdFreezeFrameMemory_GetSize()
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SelectOldestLowPrioFF(
  CONST(boolean, AUTOMATIC)  SelectVisible
  );
#endif

/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_FindOBDFreezeFrameSlot
 *****************************************************************************/
/*!
 * \brief         Finds an OBD Freeze Frame storage slot.
 *                Determines, if freeze frame visibility must be updated.
 *
 * \details       This function tries to find an OBD freeze frame storage slot
 *                for the given event. It also determines, if an invisible
 *                freeze frame related to a misfire or fuel system event must
 *                be made visible immediately.
 *
 * \param[in,out] DTCContext
 *                DTC processing context. This function updates the QualifyDTC
 *                member.
 *                TRUE: invisible freeze frame related to a misfire or fuel
 *                system event must be set visible before DCY qualification
 *                FALSE: no visibility change required
 * \param[in]     DisplacePendingFreezeFrame
 *                True if freeze frame displacement is enabled
 *                and the corresponding event is confirmed.
 *
 * \return        Freeze frame storage slot for the given event.
 *
 * \pre           The event must be an OBD relevant event.
 * \config        -
 *
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_FindOBDFreezeFrameSlot(
  CONSTP2VAR(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext,
  CONST(boolean, AUTOMATIC) DisplacePendingFreezeFrame
  );

/* ****************************************************************************
* Dem_Memories_ObdFreezeFrameMemory_UpdateObdOnUdsFreezeFrame
*****************************************************************************/
/*!
* \brief         Stores or updates an OBD on UDS Freeze Frame.
*
* \details       Stores or updates the OBD on UDS freeze frame data.
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
* \param[in]     FreezeFrameSlot
*                FreezeFrame slot corresponding to the event.
*
* \pre           The event must be an OBD relevant event.
*                Can only be called in OBD on UDS configs or variants.
* \config        -
*
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_UpdateObdOnUdsFreezeFrame(
  Dem_EventIdType EventId,
  uint8 FreezeFrameSlot
);

/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_InitRestoreMemoryObd
 *****************************************************************************/
/*!
 * \brief         Initializes OBD II and OBD on UDS Freeze Frame
 *
 * \details       This function verifies the integrity of OBD memory FF entries
 *                and links them up into the runtime control structures.
 *
 * \pre           Feature OBD II or OBDonUDS is enabled. NvM must have restored NV mirrors
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_InitRestoreMemoryObd(
  void
);


/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Selects the freeze frame to be reported in Obd mode 02
 *
 * \details       Selects the oldest freeze frame of a misfire or fuel system 
 *                monitor if available, otherwise selects the oldest visible 
 *                freeze frame
 *
 * \return        Index of the freeze frame to display in mode 02, if no 
 *                candidate is found, return Dem_Memories_ObdFreezeFrameMemory_GetSize()
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame(
  void
  );

#if ((DEM_CFG_SUPPORT_OBDII == STD_ON) \
  && (DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON))
/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_SetFFOfEventVisible
 *****************************************************************************/
/*!
 * \brief         Marks a freeze frame slot related to an event as visible
 *
 * \details       Searches for a freeze frame slot related to the EventId
 *                and marks it as visible
 *
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDII == STD_ON
 *                && DEM_CFG_SUPPORT_CONFIRMED_VISIBLE_FREEZE_FRAME == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_SetFFOfEventVisible(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );
#endif

/* ****************************************************************************
* Dem_Memories_ObdFreezeFrameMemory_UpdateOBDIIFreezeFrame
*****************************************************************************/
/*!
* \brief         Stores or updates an OBDII Freeze Frame.
*
* \details       Stores or updates the OBDII freeze frame data, visibility and
*                time stamp depending on RequiredUpdates.
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
* \param[in]     FreezeFrameSlot
*                FreezeFrame slot corresponding to the event.
* \param[in]     RequiredUpdates
*                Required freeze frame updates.
*
* \return        EventId of the event whose freeze frame was updated
*
* \pre           The event must be an OBD relevant event.
*                Can only be called in OBDII configs or variants.
* \config        -
*
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_UpdateOBDIIFreezeFrame(
  Dem_EventIdType EventId,
  uint8 FreezeFrameSlot,
  uint8 RequiredUpdates
);

/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_ClearOBDFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Clears OBD FreezeFrame data
 *
 * \details       Clears OBD FreezeFrame data if stored by passed event and 
 *                if OBDII or OBD on UDS is enabled
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_ClearOBDFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON) || (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) )
/* ****************************************************************************
 * Dem_Memories_ObdFreezeFrameMemory_EventDisplaced
 *****************************************************************************/
/*!
 * \brief         Remove a freeze frame due to event displacement
 *
 * \details       This function clears a freeze frame stored for the passed
 *                Event if OBDII or OBD on UDS is enabled
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Only valid if OBDII is supported in variant.
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON ||
 *                DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_ObdFreezeFrameMemory_EventDisplaced(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_OBDFREEZEFRAMEMEMORY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ObdFreezeFrameMemory_Interface.h
 *********************************************************************************************************************/
