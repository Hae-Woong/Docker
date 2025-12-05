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
 *  \defgroup   Dem_SnapshotEntry SnapshotEntry
 *  \{
 *  \file       Dem_SnapshotEntry_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Encapsulates the list of snapshot record buffers stored in the event entry
 *  \entity     SnapshotEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_SNAPSHOTENTRY_INTERFACE_H)
#define DEM_SNAPSHOTENTRY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_SnapshotEntry_Types.h"
#include "Dem_MemoryEntryIF_Types.h"
#include "Dem_EventMemory_Types.h"

/* ********************************************************************************************************************
*  SUBCOMPONENT CONSTANT MACROS
*********************************************************************************************************************/

/*! Index indicating an invalid value */
#define DEM_SNAPSHOTENTRY_CONFIGURED_SREC_INDEX_INVALID    32U


/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_SnapshotEntry_Public Public Methods
 * \{
 */

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)\
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_SRecIsSupportedByAvailableCombinedEvent
 *****************************************************************************/
/*!
 * \brief         Tests if an available SubEvent has the snapshot configured.
 *
 * \details       Tests if an available SubEvent has the snapshot configured.
 *
 * \param[in]     CombinedDTCGroup
 *                Combined DTC group identifier
 * \param[in]     SRecId
 *                Identifier of the snapshot data record
 * \return        TRUE
 *                At least one SubEvent that has the snapshot configured is available
 * \return        FALSE
 *                No SubEvent that has the snapshot configured is available
 *
 * \pre           -
 * \config        ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_SRecIsSupportedByAvailableCombinedEvent(
CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup,
CONST(uint8, AUTOMATIC)  SRecId
);
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorInvalidate
 *****************************************************************************/
/*!
 * \brief         Sets snapshot entry iterator to state 'invalidated'
 *
 * \details       Sets snapshot entry iterator to state 'invalidated'
 *
 * \param[in,out] SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorInvalidate(
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorInit
 *****************************************************************************/
/*!
 * \brief         Initializes the snapshot entry iterator
 *
 * \details       Initializes the iterator to enumerate snapshot entries in 
 *                an event entry
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[out]    SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorInit(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorNext
 *****************************************************************************/
/*!
 * \brief         Move the snapshot entry iterator to the next element
 *
 * \details       Move the snapshot entry iterator to the next element
 *
 * \param[in,out] SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           SnapshotEntryIter has been initialized
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorNext(
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorExists
 *****************************************************************************/
/*!
 * \brief         Test if the snapshot entry iterator points to a valid element
 *
 * \details       Test if the snapshot entry iterator points to a valid element
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        TRUE
 *                SnapshotEntryIter points to a valid element
 * \return        FALSE
 *                SnapshotEntryIter doesn't point to a valid element
 *
 * \pre           SnapshotEntryIter has been initialized
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorExists(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorInit
 *****************************************************************************/
/*!
 * \brief         Initializes the combined snapshot entry iterator
 *
 * \details       Initializes the iterator to enumerate combined snapshot entries 
 *                in an event entry
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[out]    SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *                && DEM_CFG_SUPPORT_SRECS == STD_ON
 *                && DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorInit(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
);
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorNext
 *****************************************************************************/
/*!
 * \brief         Move the combined snapshot entry iterator to the next element
 *
 * \details       Move the combined snapshot entry iterator to the next element
 *
 * \param[in,out] SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           SnapshotEntryIter has been initialized
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON 
 *                && DEM_CFG_SUPPORT_SRECS == STD_ON
 *                && DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorNext(
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
);
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorExists
 *****************************************************************************/
/*!
 * \brief         Test if the combined snapshot entry iterator points to a valid element
 *
 * \details       Test if the combined snapshot entry iterator points to a valid element
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        TRUE
 *                SnapshotEntryIter points to a valid element
 * \return        FALSE
 *                SnapshotEntryIter doesn't point to a valid element
 *
 * \pre           SnapshotEntryIter has been initialized
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *                && DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorExists(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
);
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordIndex
 *****************************************************************************/
/*!
 * \brief         Get the snapshot record index 
 *
 * \details       Get the snapshot record index pointed to by the iterator
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        Handle to the snapshot record descriptor table
 *
 * \pre           SnapshotEntryIter has been initialized
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordIndex(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotEntryIndex
 *****************************************************************************/
/*!
 * \brief         Get the snapshot record index 
 *
 * \details       Get the snapshot record index pointed to by the iterator
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        Index of the snapshot data buffer in the event entry
 *
 * \pre           SnapshotEntryIter has been initialized
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotEntryIndex(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
* Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex
*****************************************************************************/
/*!
* \brief         Get the snapshot record fifo index
*
* \details       Get the snapshot record fifo index pointed to by the iterator
*
* \param[in]     SnapshotEntryIter
*                Pointer to the snapshot entry iterator
*
* \return        Fifo index of the snapshot data, when calculated fifo enabled
*                Otherwise always 0.
*
* \pre           SnapshotEntryIter has been initialized
* \config        DEM_CFG_SUPPORT_SRECS == STD_ON
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
);
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordDataConstPtr
 *****************************************************************************/
/*!
 * \brief         Get the snapshot data buffer
 *
 * \details       Get the snapshot data buffer at the index pointed to by the 
 *                iterator
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        Pointer to the unmodifiable (const) snapshot data buffer
 *
 * \pre           SnapshotEntryIter has been initialized
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordDataConstPtr(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorIsSnapshotRecordStored
 *****************************************************************************/
/*!
 * \brief         Tests whether a snapshot record is currently stored
 *
 * \details       Tests whether a snapshot record pointed to by the iterator 
 *                is currently stored
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        TRUE
 *                The snapshot record is stored
 * \return        FALSE
 *                The snapshot record is not stored
 *
 * \pre           SnapshotEntryIter->EventEntryIndex must be inside valid range
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorIsSnapshotRecordStored(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorTestSnapshotRecordFifoStorageRequired
 *****************************************************************************/
/*!
 * \brief         Tests whether a Calculated FIFO snapshot record need to be stored
 *
 * \details       Tests whether a Calculated FIFO snapshot record pointed to by the iterator
 *                need to be stored
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        TRUE
 *                The snapshot record need to be stored
 * \return        FALSE
 *                The snapshot record does not need to be stored
 *
 * \pre           SnapshotEntryIter->EventEntryIndex must be inside valid range
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorTestSnapshotRecordFifoStorageRequired(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
);
#endif /* DEM_CFG_SUPPORT_SRECS == STD_ON */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorUpdateSnapshotEntry
 *****************************************************************************/
/*!
 * \brief         Set the 'stored' flag for a snapshot entry
 *
 * \details       If configured records are used, set the corresponding bit in
 *                the SnapshotHeader of the event entry.
 *                If calculated records are used, increment the SnapshotHeader
 *                value of the event entry.
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           SnapshotEntryIter->EventEntryIndex must be inside valid range
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorUpdateSnapshotEntry(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

# if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)\
      && (DEM_CFG_SUPPORT_SRECS == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_SetSRecStorageInDTCHeader
 *****************************************************************************/
/*!
 * \brief         Set the 'stored' flag for a snapshot record
 *
 * \details       Set the 'stored' flag for a snapshot record within the DTC
 *                specific snapshot record header in the event entry.
 *
 * \param[in]     SRecId
 *                Identifier of the snapshot record
 * \param[in]     CombinedDTCGroup
 *                Combined DTC group identifier
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 *
 * \config        ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)\
 *                 && (DEM_CFG_SUPPORT_SRECS == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_SetSRecStorageInDTCHeader(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup,    
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

# if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_GetSRecStorageStatus
 *****************************************************************************/
/*!
 * \brief         Get the 'stored' status for a snapshot record within the 
 *                subevent entry.
 *
 * \details       Get the 'stored' status for a snapshot record within the 
 *                subevent entry.
 *
 * \param[in]     SRecId
 *                Identifier of the snapshot record
 * \param[in]     SnapshotHeaderBitMaskIndex
 *                Index pointing to position of SRecId inside the DTC specific
 *                Record Number list
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        Storage status of the snapshot record
 *
 * \pre           -
 *
 * \config        (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) 
 *                && (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetSRecStorageStatus(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(uint8, AUTOMATIC)  SnapshotHeaderBitMaskIndex,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

# if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_FindSRecIndexInCombinedDTCSRecList
 *****************************************************************************/
/*!
 * \brief         Function searches for an returns the index of a snapshot
 *                record Id within the list of snapshot records corresponding
 *                to a combined DTC group.
 *
 * \details       Function searches for an returns the index of a snapshot
 *                record Id within the list of snapshot records corresponding
 *                to a combined DTC group.
 *
 * \param[in]     SRecId
 *                Identifier of the snapshot record
 * \param[in]     SRecNumerationType
 *                Enumeration type used for the snapshot record
 * \param[in]     CombinedDTCGroup
 *                Combined DTC group identifier
 *
 * \pre           -
 * \return        Index of a snapshot record Id.
 *                If the snapshot record is not found within the list, function
 *                returns 'DEM_SNAPSHOTENTRY_CONFIGURED_SREC_INDEX_INVALID'
 *
 * \config        (DEM_CFG_SUPPORT_SRECS == STD_ON) 
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_FindSRecIndexInCombinedDTCSRecList(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(Dem_Cfg_SRecNumerationType, AUTOMATIC)  SRecNumerationType,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorCopySnapshotEntry
 *****************************************************************************/
/*!
 * \brief         Copy the snapshot entry at the given index to the given 
 *                snapshot entry
 *
 * \details       Copy the snapshot entry at the given index to the snapshot 
 *                entry pointed to by the iterator
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 * \param[in]     SnapshotRecordIndex
 *                Index of the snapshot entry to be copied
 *
 * \pre           SnapshotEntryIter->EventEntryIndex and EntryIndex must be
 *                inside valid range
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorCopySnapshotEntry(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter,
  uint8 SnapshotRecordIndex
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorFindLastStoredEntry
 *****************************************************************************/
/*!
 * \brief         Move the snapshot entry iterator to the index of the last 
 *                stored snapshot entry.
 *
 * \details       Move the snapshot entry iterator to the index of the last 
 *                stored snapshot entry. The snapshot entry pointer always 
 *                points to a valid snapshot entry afterwards.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[out]    SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           SnapshotEntryIter->EventEntryIndex must be inside valid range
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorFindLastStoredEntry(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorFindEntryWithRecordNumber
 *****************************************************************************/
/*!
 * \brief         Move the snapshot entry iterator to the index of the
 *                snapshot entry with requested record number.
 *
 * \details       Move the snapshot entry iterator to the index of the snapshot 
 *                entry with requested record number. The snapshot entry pointer 
 *                always points to a valid snapshot entry afterwards.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     RecordNumber
 *                Snapshot record number.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[out]    SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           SnapshotEntryIter->EventEntryIndex must be inside valid range
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorFindEntryWithRecordNumber(
  Dem_EventIdType EventId,
  uint8 RecordNumber,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_SearchStoredEntry
 *****************************************************************************/
/*!
 * \brief         Move the snapshot entry iterator to the index of the next
 *                stored snapshot entry.
 *
 * \details       Move the snapshot entry iterator to the index of the next 
 *                stored snapshot entry. The iterator only points to the index
 *                of a valid and stored entry, if the function returns TRUE.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in,out] SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        TRUE
 *                SnapshotEntryIter points to the index of a stored entry.
 *                FALSE
 *                No stored entry was found. 
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON &&
 *                DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_SearchStoredEntry(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorGetStoredRecordNumber
 *****************************************************************************/
/*!
 * \brief         Returns the snapshot record number, pointed by the iterator.
 *
 * \details       Returns the snapshot record number, pointed by the iterator.
 *
 * \param[out]    SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        TRUE
 *                SnapshotEntryIter points to the index of a stored entry.
 *                FALSE
 *                No stored entry was found.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON &&
 *                DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetStoredRecordNumber(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
);
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredSnapshotRecords
 *****************************************************************************/
/*!
 * \brief         Counts the number of snapshot records stored in a memory
 *                entry
 *
 * \details       Counts the number of snapshot records stored in a memory
 *                entry
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        The number of stored snapshot records
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON &&
 *                DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredSnapshotRecords(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredCombinedSnapshotRecords
 *****************************************************************************/
/*!
 * \brief         Counts the number of snapshot records stored in a memory
 *                entry and additional combined DTC event entries.
 *
 * \details       Counts the number of snapshot records stored in a memory
 *                entry and additional combined DTC event entries.
 *
 * \param[in]     EventEntryIter
 *                Entry iterator
 * \param[in]     GroupId
 *                CombinedDTC Group Identifier
 *
 * \return        The number of stored snapshot records
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON &&
 *                DEM_CFG_SUPPORT_SRECS == STD_ON &&
 *                DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredCombinedSnapshotRecords(
  Dem_Cfg_EntryIterType EventEntryIter,
  Dem_Cfg_CombinedGroupIndexType GroupId
);
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_SNAPSHOTENTRY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_SnapshotEntry_Interface.h
 *********************************************************************************************************************/
