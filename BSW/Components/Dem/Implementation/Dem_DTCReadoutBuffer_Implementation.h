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
/*! \addtogroup Dem_DTCReadoutBuffer
 *  \{
 *  \file       Dem_DTCReadoutBuffer_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) Implementation file
 *  \details    Buffer event specific data after Dem_DisableDTCRecordUpdate() call.
 *  \entity     DTCReadoutBuffer
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DTCREADOUTBUFFER_IMPLEMENTATION_H)
#define DEM_DTCREADOUTBUFFER_IMPLEMENTATION_H

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
#include "Dem_DTCReadoutBuffer_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Statistics_Interface.h"
#include "Dem_0x1904_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_DTCREADOUTBUFFER_IMPLEMENTATION_FILENAME "Dem_DTCReadoutBuffer_Implementation.h"

/*!
 * \addtogroup  Dem_DTCReadoutBuffer_StatusType
 * \{
 */
#define DEM_DTCREADOUTBUFFER_STATUS_UNUSED            0u  /*!< Initialization value, buffer is unused and contains invalid data */
#define DEM_DTCREADOUTBUFFER_STATUS_QUEUED            1u  /*!< Buffer contains EventId, MemoryId configuration - waiting for MainFunction to copy the data */
#define DEM_DTCREADOUTBUFFER_STATUS_IN_USE            2u  /*!< Buffer contains configuration and event data */
#define DEM_DTCREADOUTBUFFER_STATUS_NOT_STORED        3u  /*!< Buffer contains EventId, MemoryId configuration - no data found to copy */
/*!
 * \}
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 * \defgroup Dem_DTCReadoutBuffer_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_DTCReadoutBuffer_GetReadoutBuffer()
 *****************************************************************************/
/*!
 * \brief         Returns pointer to the readout buffer entry
 *
 * \details       Asserts that the readout buffer id is valid and returns
 *                pointer to the readout buffer entry
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 *
 * \return        Pointer to the readout buffer entry
 *
 * \pre           -
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_ReadoutBufferEntryPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetReadoutBuffer(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId
);

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_GetState
 *****************************************************************************/
/*!
 * \brief         Get internal state of readout buffer.
 *
 * \details       Get internal state of readout buffer.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer, derived from the ClientId.
 *
 * \return        DEM_DTCREADOUTBUFFER_UNUSED
 *                The readout buffer is unused and contains no (i.e. invalid)
 *                data.
 * \return        DEM_DTCREADOUTBUFFER_QUEUED
 *                The readout buffer contains EventId, MemoryId values, and is
 *                waiting for the MainFunction to copy the Data.
 * \return        DEM_DTCREADOUTBUFFER_IN_USE
 *                The readout buffer contains configuration and event's data.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCReadoutBuffer_StatusType, DEM_CODE)
Dem_DTCReadoutBuffer_GetState(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  );
#endif

/* **************************************************************************** 
 * Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe
 *****************************************************************************/
/*!
 * \brief         Returns pointer to the corresponding event entry of the
 *                readout buffer
 *
 * \details       Checks if passed readout buffer Id and event entry index is
 *                valid and returns pointer to the requested event entry
 *                of the readout buffer
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     ReadoutBufferDataIndex
 *                Index to ReadoutBufferData inside the ReadoutBuffer
 *
 * \return        Valid pointer to the readout buffer's event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_ReadoutBufferDataPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  );



#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_GetReadoutBufferSafe()
 *****************************************************************************/
/*!
 * \brief         Returns pointer to the readout buffer entry
 *
 * \details       Checks if passed handle is valid and returns pointer to the
 *                readout buffer entry
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 *
 * \return        Valid pointer to the readout buffer entry
 *
 * \pre           -
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_ReadoutBufferEntryPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetReadoutBufferSafe(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_GetSnapshotIteratorPtr
 *****************************************************************************/
/*!
 * \brief         Returns pointer to the snapshot iterator
 *
 * \details       Returns pointer to the snapshot iterator of the given readout buffer
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     ReadoutBufferDataIndex
 *                Index to ReadoutBufferData inside the ReadoutBuffer
 *
 * \return        Pointer to the snapshot iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetSnapshotIteratorPtr(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  );
#endif


/* ****************************************************************************
 * Dem_DTCReadoutBuffer_GetReadoutBufferData()
 *****************************************************************************/
/*!
 * \brief         Returns pointer to the corresponding event entry of the
 *                readout buffer
 *
 * \details       Returns pointer to the requested event entry
 *                of the readout buffer without any verification
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     ReadoutBufferDataIndex
 *                Index to ReadoutBufferData inside the ReadoutBuffer
 *
 * \return        Pointer to the readout buffer's event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_ReadoutBufferDataPtrType, DEM_CODE)
  Dem_DTCReadoutBuffer_GetReadoutBufferData(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  );


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_GetExtendedIteratorPtr
 *****************************************************************************/
/*!
 * \brief         Returns pointer to the extended iterator
 *
 * \details       Returns pointer to the extended iterator of the given readout buffer
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     ReadoutBufferDataIndex
 *                Index to ReadoutBufferData inside the ReadoutBuffer
 *
 * \return        Pointer to the extended iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/ 
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetExtendedIteratorPtr(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
);
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetExtendedRecordNumber
 *****************************************************************************/
/*!
 * \brief         Sets the extended data record number
 *
 * \details       Sets the given extended data record number as 
 *                SelectedExtendedDataNumber in ReadoutBuffer
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     SelectedExtendedRecordNumber
 *                Selected extended data record number
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetExtendedRecordNumber(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(uint8, AUTOMATIC)  SelectedExtendedRecordNumber
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetSnapshotRecordSelected
 *****************************************************************************/
/*!
 * \brief         Marks the selection state of a snapshot record in a readout 
 *                buffer
 *
 * \details       Marks the selection state of a snapshot record in a readout 
 *                buffer
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     SnapshotSelected
 *                Selection state of the snapshot record
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotRecordSelected(                                                                                  
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(boolean, AUTOMATIC) SnapshotSelected
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetExtendedRecordSelected
 *****************************************************************************/
/*!
 * \brief         Marks the selection state of an extended data record in a readout 
 *                buffer
 *
 * \details       Marks the selection state of an extended data record in a readout 
 *                buffer
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     ExtendedSelected
 *                Selection state of the extended data record
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetExtendedRecordSelected(                                                                                  
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(boolean, AUTOMATIC) ExtendedSelected
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetSnapshotRecordNumber
 *****************************************************************************/
/*!
 * \brief         Sets the selected snapshot record number
 *
 * \details       Sets the selected snapshot record number
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     SelectedSnapshotRecordNumber
 *                Record number of the selected snapshot record
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotRecordNumber(                                                                                    
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(uint8, AUTOMATIC)  SelectedSnapshotRecordNumber
);
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetEventId
 *****************************************************************************/
/*!
 * \brief         Set EventId of ReadoutBufferData.
 *
 * \details       Set EventId of ReadoutBufferData in ReadoutBuffer.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer, derived from the ClientId.
 * \param[in]     ReadoutBufferDataIndex
 *                Index to ReadoutBufferData inside the ReadoutBuffer
 * \param[in]     EventId
 *                The EventId of the readout buffer
 *
 * \pre           Dem_DTCReadoutBuffer_GetState() != DEM_DTCREADOUTBUFFER_UNUSED
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetEventId(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );
#endif


#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetMasterEventId
 *****************************************************************************/
/*!
 * \brief         Set Master EventId of readout buffer.
 *
 * \details       Set Master EventId of readout buffer.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer, derived from the ClientId.
 * \param[in]     MasterEventId
 *                Master event Id of the combined DTC group.
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON) 
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetMasterEventId(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_EventIdType, AUTOMATIC) MasterEventId
  );
#endif


#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetNumberOfOccupiedSlots
 *****************************************************************************/
/*!
 * \brief         Set number of occupied ReadoutBufferData entries.
 *
 * \details       Set number of occupied ReadoutBufferData entries.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer, derived from the ClientId.
 * \param[in]     NumOccupiedSlots
 *                Number of occupied memory slots of readout buffer.
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON) 
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetNumberOfOccupiedSlots(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(uint8, AUTOMATIC) NumOccupiedSlots
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetMemoryId
 *****************************************************************************/
/*!
 * \brief         Set the memory location
 *
 * \details       Set the memory locations of the given record
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer, derived from the ClientId.
 * \param[in]     MemoryId
 *                Memory Id to be set within the readout buffer.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetMemoryId(                                                                                                
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(uint16, AUTOMATIC) MemoryId
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetOBDData_IsStored
 *****************************************************************************/
/*!
 * \brief         Sets the stored status of OBD FreezeFrame
 *
 * \details       Sets the stored status of OBD FreezeFrame for requested
 *                ReadoutBufferId if ObdII with readout of Obd freeze frame is 
 *                enabled or OBD on UDS is enabled
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer, derived from the ClientId.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON 
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetOBDData_IsStored(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(boolean, AUTOMATIC) IsStored
  );
#endif

/* ****************************************************************************
 * Dem_RecordReader_DTCReadoutBuffer_InitCustomTriggerData
 *****************************************************************************/
/*!
 * \brief         Initialize the custom trigger data in readout buffer.
 *
 * \details       -
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer, derived from the ClientId.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_InitCustomTriggerData(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId
  );


/* ****************************************************************************
 * Dem_RecordReader_DTCReadoutBuffer_GetEventIdInCustomTriggerEntry
 *****************************************************************************/
/*!
 * \brief         Returns the evend id of the custom triggered data stored in the 
 *                readout buffer
 *
 * \details       -
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 *
 * \return        EventId
 *                Event Id in the custom trigger data
 *                DEM_EVENT_INVALID
 *                No custom triggered data is stored in the readout buffer
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different ReadoutBuffer Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetEventIdInCustomTriggerEntry(
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId
  );


/* ****************************************************************************
 * Dem_RecordReader_DTCReadoutBuffer_GetUnusedFutureSamplesCustomTrigger
 *****************************************************************************/
/*!
 * \brief         Returns the captured event's unused future samples
 *                in the readout buffer custom trigger data
 *
 * \details       -
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 *
 * \return        Unused future samples of custom triggered time series in readout buffer
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetUnusedFutureSamplesCustomTrigger(                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId
);


/* ****************************************************************************
 * Dem_RecordReader_DTCReadoutBuffer_GetUnusedPastSamplesCustom
 *****************************************************************************/
/*!
 * \brief         Returns the captured event's unused past samples
 *                in the readout buffer custom trigger data
 *
 * \details       -
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 *
 * \return        Unused past samples of custom triggered time series in readout buffer
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetUnusedPastSamplesCustom(                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId
);


/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetTimeSeriesEntryIndex
 *****************************************************************************/
/*!
 * \brief         Set the time series entry index in readout buffer.
 *
 * \details       Set the time series entry index in readout buffer.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetTimeSeriesEntryIndex(
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId,
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
);

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetTimeSeriesEntryUnusedFutureSamples
 *****************************************************************************/
/*!
 * \brief         Set the unused future samples for time series in readout buffer.
 *
 * \details       Set the unused future samples for time series in readout buffer.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetTimeSeriesEntryUnusedFutureSamples(                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId,
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
);
#endif
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetUnusedFutureSamplesInTssSet
 *****************************************************************************/
/*!
 * \brief         Set the unused future samples for the specified series
 *                in readout buffer for the Tss set.
 *
 * \details       -
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     SeriesId
 *                Specifies a series of the relevant event
 * \param[in]     SetId
 *                Tss set index
 * \param[in]     Samples
 *                Number of unused future samples
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetUnusedFutureSamplesInTssSet(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId,
  uint8 Samples
);


/* ****************************************************************************
 * Dem_DTCReadoutBuffer_GetCustomTriggerEntryIndex
 *****************************************************************************/
/*!
 * \brief         Returns the index of the readout buffer in Dem_Cfg_CustomTriggerEntryPtr
 *
 * \details       -
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 *
 * \return        Index of the readout buffer in Dem_Cfg_CustomTriggerEntryPtr
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_DTCReadoutBuffer_GetCustomTriggerEntryIndex(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetReadoutBuffer()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_ReadoutBufferEntryPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetReadoutBuffer(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId
  )
{
  Dem_Cfg_ReadoutBufferEntryPtrType lResult;

  Dem_Internal_AssertContinue(ReadoutBufferId < Dem_Cfg_GetSizeOfReadoutBuffer(), DEM_E_INCONSISTENT_STATE)
  lResult = Dem_Cfg_GetAddrReadoutBuffer(ReadoutBufferId);                                                                       /* PRQA S 2932 */ /* MD_DEM_18.1_ComputingOfInvalidPointer_ReadoutBuffer */

  return lResult;
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DTCReadoutBuffer_StatusType, DEM_CODE)
Dem_DTCReadoutBuffer_GetState(
CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
)
{
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->State;
}
#endif

/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_ReadoutBufferDataPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  Dem_Cfg_ReadoutBufferDataPtrType lResult;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((ReadoutBufferId >= Dem_Cfg_GetSizeOfReadoutBuffer())
    || (ReadoutBufferDataIndex >= Dem_Cfg_SizeOfReadOutBufferData()))
  {
    Dem_Error_RunTimeCheckFailed(DEM_DTCREADOUTBUFFER_IMPLEMENTATION_FILENAME, __LINE__);                                        /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lResult = &(Dem_Cfg_GetReadoutBuffer(0).ReadOutBufferData[0]);
  }
  else
# endif
  {
    lResult = &(Dem_Cfg_GetReadoutBuffer(ReadoutBufferId).ReadOutBufferData[ReadoutBufferDataIndex]);                            /* PRQA S 2931 */ /* MD_DEM_18.1_ComputingOfInvalidPointer */
  }

  return lResult;
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetReadoutBufferSafe()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_ReadoutBufferEntryPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetReadoutBufferSafe(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId
  )
{
  Dem_Cfg_ReadoutBufferEntryPtrType lResult;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (ReadoutBufferId >= Dem_Cfg_GetSizeOfReadoutBuffer())
  {
    Dem_Error_RunTimeCheckFailed(DEM_DTCREADOUTBUFFER_IMPLEMENTATION_FILENAME, __LINE__);                                        /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lResult = &Dem_Cfg_GetReadoutBuffer(0);
  }
  else
# endif
  {
    lResult = Dem_Cfg_GetAddrReadoutBuffer(ReadoutBufferId);
  }

  return lResult;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetSnapshotIteratorPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetSnapshotIteratorPtr(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorPtrType lReturnValue;

  Dem_Internal_AssertContinue(ReadoutBufferDataIndex < Dem_Cfg_SizeOfReadOutBufferData(), DEM_E_INCONSISTENT_STATE)
  lReturnValue = &(Dem_Cfg_GetReadoutBuffer(ReadoutBufferId).ReadOutBufferData[ReadoutBufferDataIndex].SnapshotIterator);

  return lReturnValue;
}
#endif



/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetReadoutBufferData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_ReadoutBufferDataPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetReadoutBufferData(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  Dem_Cfg_ReadoutBufferDataPtrType lResult;

  Dem_Internal_AssertContinue(ReadoutBufferDataIndex < Dem_Cfg_SizeOfReadOutBufferData(), DEM_E_INCONSISTENT_STATE)
  lResult = &(Dem_Cfg_GetReadoutBuffer(ReadoutBufferId).ReadOutBufferData[ReadoutBufferDataIndex]);

  return lResult;
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetExtendedIteratorPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetExtendedIteratorPtr(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorPtrType lReturnValue;

  Dem_Internal_AssertContinue(ReadoutBufferDataIndex < Dem_Cfg_SizeOfReadOutBufferData(), DEM_E_INCONSISTENT_STATE)

  lReturnValue = &(Dem_Cfg_GetReadoutBuffer(ReadoutBufferId).ReadOutBufferData[ReadoutBufferDataIndex].ExtendedIterator);

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetExtendedRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetExtendedRecordNumber(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(uint8, AUTOMATIC)  SelectedExtendedRecordNumber
)
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->SelectedExtendedDataNumber = SelectedExtendedRecordNumber;         /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetSnapshotRecordSelected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotRecordSelected(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(boolean, AUTOMATIC) SnapshotSelected
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->SnapshotSelected = SnapshotSelected;                               /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetExtendedRecordSelected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetExtendedRecordSelected(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(boolean, AUTOMATIC) ExtendedSelected
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->ExtendedRecordSelected = ExtendedSelected;                         /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetSnapshotRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotRecordNumber(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(uint8, AUTOMATIC)  SelectedSnapshotRecordNumber
)
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->SelectedSnapshotRecordNumber = SelectedSnapshotRecordNumber;       /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetEventId(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(ReadoutBufferId, ReadoutBufferDataIndex)->EventId = EventId;                     /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER_READOUTBUFFERDATA */
}
#endif

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetMasterEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetMasterEventId(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_EventIdType, AUTOMATIC) MasterEventId
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MasterEventId = MasterEventId;                                     /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetNumberOfOccupiedSlots
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetNumberOfOccupiedSlots(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(uint8, AUTOMATIC) NumOccupiedSlots
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->NumOccupiedSlots = NumOccupiedSlots;                               /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetMemoryId(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(uint16, AUTOMATIC) MemoryId
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MemoryId = MemoryId;                                               /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetOBDData_IsStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetOBDData_IsStored(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(boolean, AUTOMATIC) IsStored
  )
{
#if (((DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_OBDII_FREEZEFRAME_IN_SVC19 == STD_ON)) || (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)) 
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->OBDFreezeFrameData.IsStored = IsStored;                            /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IsStored)                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}
#endif

/* ****************************************************************************
 % Dem_RecordReader_DTCReadoutBuffer_InitCustomTriggerData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_InitCustomTriggerData(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId
  )
{
#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)) 
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->CustomTriggerData.EventId = DEM_EVENT_INVALID;                     /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_RecordReader_DTCReadoutBuffer_GetEventIdInCustomTriggerEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetEventIdInCustomTriggerEntry(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId
  )
{
  Dem_EventIdType lEventId = DEM_EVENT_INVALID;
#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON))
  lEventId = Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->CustomTriggerData.EventId;
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */

  return lEventId;
}


/* ****************************************************************************
 % Dem_RecordReader_DTCReadoutBuffer_GetUnusedFutureSamplesCustomTrigger
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetUnusedFutureSamplesCustomTrigger(                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId
  )
{
  uint8 lUnusedFutureSamples = 0u;
# if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON))
  lUnusedFutureSamples = Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->CustomTriggerData.UnusedFutureSamples;
# endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lUnusedFutureSamples;
}


/* ****************************************************************************
 % Dem_RecordReader_DTCReadoutBuffer_GetUnusedPastSamplesCustom
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetUnusedPastSamplesCustom(                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId
  )
{
  uint8 lUnusedPastSamples = 0u;
# if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON))
  lUnusedPastSamples = Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->CustomTriggerData.UnusedPastSamples;
# endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lUnusedPastSamples;
}


/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetTimeSeriesEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetTimeSeriesEntryIndex(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId,
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->TimeSeriesEntryIndex = TimeSeriesEntryIndex;                       /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
# else 
  DEM_IGNORE_UNUSED_ARGUMENT(ReadoutBufferId)                                                                                    /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(TimeSeriesEntryIndex)                                                                               /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
# endif
}

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetTimeSeriesEntryUnusedFutureSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetTimeSeriesEntryUnusedFutureSamples(                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId,
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
#if(DEM_CFG_SUPPORT_DCM == STD_ON)
  Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  Dem_EventIdType lEventId = Dem_DTCReadoutBuffer_GetEventId(ReadoutBufferId, DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT);

  uint8 lMaxTssSets = Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(lEventId);
  Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(lEventId);
  Dem_TimeSeriesIdType lSeriesId;
  for (lSeriesId = 0; lSeriesId < lNumberOfSeries; lSeriesId++)
  {

    uint8 lSetId;
    for(lSetId = 0; lSetId < lMaxTssSets; lSetId++)
    {
      Dem_DTCReadoutBuffer_SetUnusedFutureSamplesInTssSet(ReadoutBufferId, lSeriesId, lSetId,
        Dem_MemoryEntry_TimeSeriesEntry_GetUnusedFutureSamples(lEntry, lSeriesId, lSetId));                                      /* SBSW_DEM_POINTER_READ_TIMESERIES_BASEENTRY */
    } 
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimeSeriesEntryIndex)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetUnusedFutureSamplesInTssSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetUnusedFutureSamplesInTssSet(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId,
  uint8 Samples
)
{
#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((SeriesId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES) || (SetId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS))
  {
    Dem_Error_RunTimeCheckFailed(DEM_DTCREADOUTBUFFER_IMPLEMENTATION_FILENAME, __LINE__);                                        /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->UnusedFutureSamples[SeriesId][SetId] = Samples;                  /* SBSW_DEM_ARRAY_WRITE_READOUTBUFFER_UNUSEDFUTURESAMPLES */
  }
#else 
  DEM_IGNORE_UNUSED_ARGUMENT(ReadoutBufferId);                                                                                   /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(SeriesId);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(SetId);                                                                                             /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(Samples);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
}


/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetCustomTriggerEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_DTCReadoutBuffer_GetCustomTriggerEntryIndex(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId
  )
{
  return (DEM_CFG_GLOBAL_CUSTOM_TRIGGERED_SNAPSHOTS_SIZE + (Dem_Cfg_EntryIndexType)ReadoutBufferId);
}

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
 * \addtogroup Dem_DTCReadoutBuffer_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetTimeSeriesEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesIdType, DEM_CODE)
Dem_DTCReadoutBuffer_SnapshotSource_GetCurrentTimeSeriesId(                                                                      /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId
  )
{
# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->CurrentTimeSeriesId;
# else 
  DEM_IGNORE_UNUSED_ARGUMENT(ReadoutBufferId)                                                                                    /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  return 0;
# endif
}

/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SnapshotSource_SetCurrentTimeSeriesId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SnapshotSource_SetCurrentTimeSeriesId(                                                                      /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  Dem_TimeSeriesIdType Value
  )
{
# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->CurrentTimeSeriesId = Value;                                           /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
# else 
  DEM_IGNORE_UNUSED_ARGUMENT(Value);                                                                                             /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(ReadoutBufferId)                                                                                    /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
# endif
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetTimeSeriesEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_DTCReadoutBuffer_GetTimeSeriesEntryIndex(
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId
  )
{
# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->TimeSeriesEntryIndex;
# else 
  DEM_IGNORE_UNUSED_ARGUMENT(ReadoutBufferId)                                                                                    /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  return DEM_CFG_ENTRYINDEX_INVALID;
# endif
}
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetUnusedFutureSamplesNormal
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTCReadoutBuffer_GetUnusedFutureSamples(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_DTCReadoutBuffer_HandleType  ReadoutBufferId,
  Dem_TimeSeriesIdType SeriesId, 
  uint8 SetId
  )
{
  uint8 lUnusedFutureSamples = 0u;
  lUnusedFutureSamples = Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->UnusedFutureSamples[SeriesId][SetId];
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SetId)                                                                                        /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lUnusedFutureSamples;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_TestDataRequested
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTCReadoutBuffer_TestDataRequested(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  return (boolean)(Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) != DEM_DTCREADOUTBUFFER_STATUS_UNUSED);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_TestResultReady
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTCReadoutBuffer_TestResultReady(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  Dem_DTCReadoutBuffer_StatusType lBufferStatus;
  lBufferStatus = Dem_DTCReadoutBuffer_GetState(ReadoutBufferId);
  return (boolean)( (lBufferStatus == DEM_DTCREADOUTBUFFER_STATUS_IN_USE)
                          || (lBufferStatus == DEM_DTCREADOUTBUFFER_STATUS_NOT_STORED) );
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_TestStoredDataAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTCReadoutBuffer_TestStoredDataAvailable(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  return (boolean)(Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_IN_USE);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetExtendedEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTCReadoutBuffer_GetExtendedEntryIndex(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  return Dem_DTCReadoutBuffer_GetExtendedIteratorPtr(ReadoutBufferId, ReadoutBufferDataIndex)->ExtendedEntryIndex;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetExtendedEntryIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_ExtendedEntry_IterType, DEM_CODE)
Dem_DTCReadoutBuffer_GetExtendedEntryIterator(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  return Dem_DTCReadoutBuffer_GetExtendedIteratorPtr(ReadoutBufferId, ReadoutBufferDataIndex)->ExtendedEntryIter;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetExtendedEntryIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetExtendedEntryIterator(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  CONST(Dem_ExtendedEntry_IterType, AUTOMATIC)  ExtendedEntryIter
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(                                                                                 /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER_READOUTBUFFERDATA */
    ReadoutBufferId, ReadoutBufferDataIndex)->ExtendedIterator.ExtendedEntryIter = ExtendedEntryIter;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_IncExtendedEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_IncExtendedEntryIndex(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  CONST(uint8, AUTOMATIC)  ERecType
  )
{
# if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
  if (ERecType == DEM_CFG_EREC_TYPE_USER)
  {
    uint8 lExtendedEntryIndex;

    lExtendedEntryIndex = Dem_DTCReadoutBuffer_GetExtendedEntryIndex(
                            ReadoutBufferId, ReadoutBufferDataIndex) + 1u;

    Dem_DTCReadoutBuffer_SetExtendedEntryIndex(
      ReadoutBufferId, ReadoutBufferDataIndex, lExtendedEntryIndex);
  }
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ERecType)                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferDataIndex)                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetExtendedRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTCReadoutBuffer_GetExtendedRecordNumber(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId
  )
{
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->SelectedExtendedDataNumber;
}
#endif



#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_TestSnapshotRecordSelected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTCReadoutBuffer_TestSnapshotRecordSelected(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->SnapshotSelected;
}
#endif



#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_TestExtendedRecordSelected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTCReadoutBuffer_TestExtendedRecordSelected(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->ExtendedRecordSelected;
}
#endif



#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetSnapshotEntryIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SnapshotEntry_IterType, DEM_CODE)
Dem_DTCReadoutBuffer_GetSnapshotEntryIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
)
{
  return Dem_DTCReadoutBuffer_GetSnapshotIteratorPtr(ReadoutBufferId, ReadoutBufferDataIndex)->SnapshotEntryIter;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetSnapshotEntryIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotEntryIterator(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC)  SnapshotEntryIter
)
{
  Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(                                                                                 /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER_READOUTBUFFERDATA */
    ReadoutBufferId, ReadoutBufferDataIndex)->SnapshotIterator.SnapshotEntryIter = SnapshotEntryIter;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetSnapshotRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTCReadoutBuffer_GetSnapshotRecordNumber(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId
)
{
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->SelectedSnapshotRecordNumber;
}
#endif



#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetSnapshotRecordSource
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DTCReadoutBuffer_SnapshotSourceType, DEM_CODE)
Dem_DTCReadoutBuffer_GetSnapshotRecordSource(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId
)
{
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->Source;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetSnapshotRecordSource
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotRecordSource(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_SnapshotSourceType, AUTOMATIC)  Source
)
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->Source = Source;                                                   /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetSnapshotSourceIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_ComplexIterType, DEM_CODE)
Dem_DTCReadoutBuffer_GetSnapshotSourceIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
)
{
  return Dem_DTCReadoutBuffer_GetSnapshotIteratorPtr(ReadoutBufferId, ReadoutBufferDataIndex)->SourceIterator;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetSnapshotSourceIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotSourceIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  CONST(Dem_Cfg_ComplexIterType, AUTOMATIC)  SourceIterator
)
{
  Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(ReadoutBufferId,                                                                 /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER_READOUTBUFFERDATA */
    ReadoutBufferDataIndex)->SnapshotIterator.SourceIterator = SourceIterator;
}
#endif

/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetEventEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_DTCReadoutBuffer_GetEventEntryIndex(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  return (DEM_EVENTENTRY_HANDLE_READOUTBUFFER + ((Dem_Cfg_EntryIndexType)ReadoutBufferId * Dem_Cfg_SizeOfReadOutBufferData()) + ReadoutBufferDataIndex);
}

/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_DTCReadoutBuffer_GetEventId(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  Dem_EventIdType lReturnVal;

  lReturnVal = (Dem_EventIdType)Dem_DTCReadoutBuffer_GetReadoutBufferData(
                                ReadoutBufferId, ReadoutBufferDataIndex)->EventId;

  return lReturnVal;
}


#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetMasterEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_DTCReadoutBuffer_GetMasterEventId(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  Dem_EventIdType lReturnVal;
  lReturnVal = (Dem_EventIdType)Dem_DTCReadoutBuffer_GetReadoutBuffer(
                                ReadoutBufferId)->MasterEventId;

  return lReturnVal;
}
#endif



#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetNumberOfOccupiedSlots
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTCReadoutBuffer_GetNumberOfOccupiedSlots(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  uint8 lReturnVal;
  lReturnVal = Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->NumOccupiedSlots;

  return lReturnVal;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_DTCReadoutBuffer_GetMemoryId(                                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->MemoryId;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetOBDData_IsStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTCReadoutBuffer_GetOBDData_IsStored(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
#if (((DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_OBDII_FREEZEFRAME_IN_SVC19 == STD_ON)) || (DEM_CFG_SUPPORT_OBDONUDS == STD_ON))
  return Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->OBDFreezeFrameData.IsStored;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return FALSE;
#endif
}
#endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && (DEM_CFG_SUPPORT_OBDII == STD_ON) \
   && (DEM_CFG_SUPPORT_OBDII_FREEZEFRAME_IN_SVC19 == STD_ON) )
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetPtrToOBDIIFreezeFrameBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetPtrToOBDIIFreezeFrameBuffer(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  return &(Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->OBDFreezeFrameData.ObdFreezeFrame[0][0]);
}
#endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && (DEM_CFG_SUPPORT_OBDONUDS == STD_ON) )
/* ****************************************************************************
% Dem_RecordReader_DTCReadoutBuffer_GetPtrToObdOnUdsFreezeFrameBuffer
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetPtrToObdOnUdsFreezeFrameBuffer(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  Dem_1904_ObdOnUdsFreezeFrameType FreezeFrameType
)
{
  Dem_DataPtrType lReturnValue;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((uint8)FreezeFrameType >= DEM_CFG_MAX_NUM_OBDFREEZEFRAME)
  {
    Dem_Error_RunTimeCheckFailed(DEM_DTCREADOUTBUFFER_IMPLEMENTATION_FILENAME, __LINE__);                                        /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->OBDFreezeFrameData.ObdFreezeFrame[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->OBDFreezeFrameData.ObdFreezeFrame[FreezeFrameType][0]);
  }

  return lReturnValue;
}
#endif

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_CUSTOMTRIGGER_SREC == STD_ON))
/* ****************************************************************************
% Dem_RecordReader_DTCReadoutBuffer_GetCustomTriggerSnapshotConstPtr
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetCustomTriggerSnapshotConstPtr(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId
)
{
  return &(Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->CustomTriggerData.SnapshotData[0]);
}
#endif

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON))
/* ****************************************************************************
 % Dem_RecordReader_DTCReadoutBuffer_GetCustomTriggerTSSnapshotConstPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetCustomTriggerTSSnapshotConstPtr(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  uint8 TimeSeriesSampleIndex
)
{
  Dem_ConstSharedDataPtrType lTSSnapshotDataPtr;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (TimeSeriesSampleIndex >= Dem_Memories_CustomTriggerMemory_MaxTssSamplesGet())
  {
    Dem_Error_RunTimeCheckFailed(DEM_DTCREADOUTBUFFER_IMPLEMENTATION_FILENAME, __LINE__);                                        /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lTSSnapshotDataPtr = 
       (Dem_ConstSharedDataPtrType)(&(Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->CustomTriggerData.TSSnapshotData[0][0]));
  }
  else
# endif
  {
    lTSSnapshotDataPtr = (Dem_ConstSharedDataPtrType)
       &(Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->CustomTriggerData.TSSnapshotData[TimeSeriesSampleIndex][0]);
  }

  return lTSSnapshotDataPtr;
}
#endif


/* ****************************************************************************
 % Dem_RecordReader_DTCReadoutBuffer_CustomTriggeredRecordIsStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_CustomTriggeredRecordIsStored(                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  uint8  RecordNumber
)
{
  boolean lIsReadable = FALSE;
#if (DEM_CFG_SUPPORT_DCM == STD_ON)
  Dem_EventIdType lEventId = Dem_RecordReader_DTCReadoutBuffer_GetEventIdInCustomTriggerEntry(ReadoutBufferId);

  if (lEventId != DEM_EVENT_INVALID)
  {
    if (Dem_Memories_CustomTriggerMemory_TestRecordNumberIsSRec(lEventId, RecordNumber) == TRUE)
    {
      lIsReadable = TRUE;
    }
    else if (Dem_Memories_CustomTriggerMemory_TestRecordNumberIsTSSRec(lEventId, RecordNumber) == TRUE)                          /* COV_DEM_ROBUSTNESS TX */
    {
      uint8 lTimeSeriesSampleIndex = Dem_Memories_CustomTriggerMemory_GetTimeSeriesSampleIndex(lEventId, RecordNumber);
      uint8 lPastSamples = Dem_Memories_CustomTriggerMemory_NumPastTssSamples(lEventId);
      uint8 lFutureSamples = Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(lEventId);

      if (lTimeSeriesSampleIndex >= lPastSamples)
      {
        /* Future Sample */
        lTimeSeriesSampleIndex -= lPastSamples;
        lIsReadable = (lTimeSeriesSampleIndex < (lFutureSamples - Dem_RecordReader_DTCReadoutBuffer_GetUnusedFutureSamplesCustomTrigger(ReadoutBufferId)));
      }
      else
      {
        /* Past Sample */
        lIsReadable = (lTimeSeriesSampleIndex >= Dem_RecordReader_DTCReadoutBuffer_GetUnusedPastSamplesCustom(ReadoutBufferId));
      }
    }
    else
    {
      /* MISRA else unreachable code */
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    }
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(RecordNumber)                                                                                 /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return lIsReadable;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
% Dem_RecordReader_DTCReadoutBuffer_GetNumberofStoredCustomTriggeredRecords
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_GetNumberofStoredCustomTriggeredRecords(                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId
)
{
  uint8 lCountOfRecords = 0;
#if (DEM_CFG_SUPPORT_DCM == STD_ON)
  Dem_EventIdType lEventId = Dem_RecordReader_DTCReadoutBuffer_GetEventIdInCustomTriggerEntry(ReadoutBufferId);

  if (lEventId != DEM_EVENT_INVALID)
  {
    if (Dem_Core_TestCustomTriggeredSrecConfigured(lEventId) == TRUE)
    {
      lCountOfRecords += 1u;
    }
# if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
    if (Dem_Core_TestCustomTriggeredTssrConfigured(lEventId) == TRUE)
    {
      uint8 lStoredPastSamples = Dem_Memories_CustomTriggerMemory_NumPastTssSamples(lEventId) 
                            - Dem_RecordReader_DTCReadoutBuffer_GetUnusedPastSamplesCustom(ReadoutBufferId);
      uint8 lStoredFutureSamples = Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(lEventId) 
                            - Dem_RecordReader_DTCReadoutBuffer_GetUnusedFutureSamplesCustomTrigger(ReadoutBufferId);
      lCountOfRecords += lStoredPastSamples + lStoredFutureSamples;
    }
# endif
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return lCountOfRecords;
}


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetExtendedEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetExtendedEntryIndex(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  CONST(uint8, AUTOMATIC)  ExtendedEntryIndex
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(                                                                                 /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER_READOUTBUFFERDATA */
    ReadoutBufferId, ReadoutBufferDataIndex)->ExtendedIterator.ExtendedEntryIndex = ExtendedEntryIndex;
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
 * \defgroup Dem_DTCReadoutBuffer_Private Private Methods
 * \{
 */

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
 /* ****************************************************************************
 * Dem_DTCReadoutBuffer_CollectMemIndependentCounter()
 *****************************************************************************/
 /*!
 * \brief         Copy memory independent counters to readout buffer for event.
 *
 * \details       Copy memory independent counters to readout buffer for event.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     EventId
 *                Unique identifier of an event
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 *
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different ReadoutBuffer Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_CollectMemIndependentCounter(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_EventIdType EventId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetReadoutBufferEntry()
 *****************************************************************************/
/*!
 * \brief         Sets the values of different parameters in a readout buffer 
 *                entry.
 *
 * \details       Set the EventId, MemoryId, State, Snapshot record source, 
 *                Snapshot record number, snapshot record selection state, 
 *                ExtendedIterator and SnapshotIterator in the indicated
 *                readout buffer.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     MasterEventId
 *                EventId of master event to be set
 * \param[in]     MemoryId
 *                MemoryId to be set
 * \param[in]     State
 *                State to be set
 * \param[in]     CombinedEventSelected
 *                Combined event of type 2 is selected
 *
 * \pre           For event combination type 1 and 2 passed EventId must be the 
 *                MasterEventId of the combined group
 *
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different ReadoutBuffer Ids
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) 
Dem_DTCReadoutBuffer_SetReadoutBufferEntry(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(uint16, AUTOMATIC)  MasterEventId,
  CONST(uint16, AUTOMATIC)  MemoryId,
  CONST(uint8, AUTOMATIC)  State,
  CONST(boolean, AUTOMATIC)  CombinedEventSelected
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_InitializeSnapshotIterator()
 *****************************************************************************/
/*!
 * \brief         Initializes contents of snapshot iterator.
 *
 * \details       Initializes contents of snapshot iterator within the indexed 
 *                data entry of readout buffer to the initial value.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     ReadoutBufferDataIndex
 *                Index to ReadoutBufferData inside the ReadoutBuffer
 *
 * \pre           -
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different ReadoutBuffer Id and ReadoutBufferDataIndex
 *                combinations
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_InitializeSnapshotIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_ExtendedDataRecord_Init
 *****************************************************************************/
/*!
 * \brief         Initialize the extended data iterator
 *
 * \details       Clear all members.
 *
 * \param[in]     ExtendedDataIterator
 *                Pointer to the extended data iterator to be initialized
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_ExtendedDataRecord_Init(
  Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorPtrType ExtendedDataIterator
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SnapshotRecord_Init
 *****************************************************************************/
/*!
 * \brief         Initialize the snapshot record iterator
 *
 * \details       Clear all members.
 *
 * \param[in]     SnapshotDataIterator
 *                Pointer to the snapshot data iterator to be initialized
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SnapshotRecord_Init(
  Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorPtrType SnapshotDataIterator
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetExtendedIterator
 *****************************************************************************/
/*!
 * \brief         Sets the extended iterator
 *
 * \details       Sets the extended iterator of the given readout buffer
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     ReadoutBufferDataIndex
 *                Index to ReadoutBufferData inside the ReadoutBuffer
 * \param[in]     ExtendedDataIterator
 *                Pointer to the new value of the extended data iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetExtendedIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorConstPtrType  ExtendedDataIterator
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetSnapshotIterator
 *****************************************************************************/
/*!
 * \brief         Sets the snapshot iterator
 *
 * \details       Sets the snapshot iterator of the given readout buffer
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     ReadoutBufferDataIndex
 *                Index to ReadoutBufferData inside the ReadoutBuffer
 * \param[in]     SnapshotIterator
 *                Pointer to the new value of the snapshot data iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorConstPtrType SnapshotIterator
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_SetState
 *****************************************************************************/
/*!
 * \brief         Set internal state of readout buffer.
 *
 * \details       Set internal state of readout buffer.
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer, derived from the ClientId.
 *
 * \param[in]     State
 *                New internal state to be set
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetState(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(uint8, AUTOMATIC)  State
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_DTCReadoutBuffer_CopyOBDIIFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Copies OBD FreezeFrame
 *
 * \details       Copies OBD FreezeFrame from memory buffer to
 *                OBD FreezeFrame Buffer in ReadOutBuffer if 
 *                ObdII and readout of Obd freeze frame is 
 *                enabled
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     EventId
 *                EventId which stores the OBDII Freeze frame
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_CopyOBDIIFreezeFrame(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_RecordReader_DTCReadoutBuffer_CopyObdOnUdsFreezeFrame
 *****************************************************************************/
 /*!
 * \brief         Copies OBD on UDS FreezeFrames
 *
 * \details       Copies OBD on UDS FreezeFrames from memory buffer to
 *                OBD FreezeFrame Buffer in ReadOutBuffer if
 *                Obd on UDS is enabled
 *
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     EventId
 *                EventId which stores the OBDII Freeze frame
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_CopyObdOnUdsFreezeFrame(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  Dem_EventIdType EventId
);
#endif

/* ****************************************************************************
 * Dem_RecordReader_DTCReadoutBuffer_CopyCustomTriggerData
 *****************************************************************************/
 /*!
 * \brief         Copies custom trigger entry into readout buffer
 *
 * \details       -
 *
 *
 * \param[in]     ReadoutBufferId
 *                Index of the ReadoutBuffer
 * \param[in]     EventId
 *                EventId which stores the custom triggered data
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_CopyCustomTriggerData(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  Dem_EventIdType EventId
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetReadoutBufferEntry()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetReadoutBufferEntry(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(uint16, AUTOMATIC)  MasterEventId,
  CONST(uint16, AUTOMATIC)  MemoryId,
  CONST(uint8, AUTOMATIC)  State,
  CONST(boolean, AUTOMATIC)  CombinedEventSelected
  )
{
  uint8 lSizeOfReadoutBufferDataEntries;
  Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorType lSnapshotIterator;
  Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorType lExtendedIterator;

  Dem_DTCReadoutBuffer_SnapshotRecord_Init(&lSnapshotIterator);                                                                  /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_DTCReadoutBuffer_ExtendedDataRecord_Init(&lExtendedIterator);                                                              /* SBSW_DEM_POINTER_FORWARD_STACK */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CombinedEventSelected)                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */

# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  Dem_DTCReadoutBuffer_SetMasterEventId(ReadoutBufferId, MasterEventId);
  Dem_DTCReadoutBuffer_SetNumberOfOccupiedSlots(ReadoutBufferId, 0x00U);
  if (CombinedEventSelected == TRUE)
  {
    /* Initialize all readout buffer data entries */
    lSizeOfReadoutBufferDataEntries = Dem_Cfg_SizeOfReadOutBufferData();
  }
  else
# endif
  {
    /* Initialize default readout buffer data entries */
    lSizeOfReadoutBufferDataEntries = 0x01U;
  }

  {
    Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType lReadoutBufferDataIndex;

    for (lReadoutBufferDataIndex = 0x00U;
         lReadoutBufferDataIndex < lSizeOfReadoutBufferDataEntries;
         lReadoutBufferDataIndex++)
    {
      Dem_DTCReadoutBuffer_SetSnapshotIterator(ReadoutBufferId,
                                               lReadoutBufferDataIndex,
                                               &lSnapshotIterator);                                                              /* SBSW_DEM_POINTER_FORWARD_STACK */
      Dem_DTCReadoutBuffer_SetExtendedIterator(ReadoutBufferId,
                                               lReadoutBufferDataIndex,
                                               &lExtendedIterator);                                                              /* SBSW_DEM_POINTER_FORWARD_STACK */
    }
  }

  if ((Dem_RecordReader_0x1904_IsObdIIFreezeFrameInService19EnabledInVariant() == TRUE) || (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE))
  {
    Dem_DTCReadoutBuffer_SetOBDData_IsStored(ReadoutBufferId, FALSE);
  }

  Dem_DTCReadoutBuffer_SetEventId(ReadoutBufferId, DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT, MasterEventId);
  Dem_DTCReadoutBuffer_SetMemoryId(ReadoutBufferId, MemoryId);
  Dem_DTCReadoutBuffer_SetSnapshotRecordSource(ReadoutBufferId, Dem_DTCReadoutBuffer_SnapshotSource_Invalid);
  Dem_DTCReadoutBuffer_SnapshotSource_SetCurrentTimeSeriesId(ReadoutBufferId, Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber(MasterEventId, 0));
  Dem_DTCReadoutBuffer_SetSnapshotRecordNumber(ReadoutBufferId, 0x00u);
  Dem_DTCReadoutBuffer_SetSnapshotRecordSelected(ReadoutBufferId, FALSE);
  Dem_DTCReadoutBuffer_SetExtendedRecordSelected(ReadoutBufferId, FALSE);
  Dem_DTCReadoutBuffer_SetExtendedRecordNumber(ReadoutBufferId, 0x00u);
  Dem_DTCReadoutBuffer_SetState(ReadoutBufferId, State);

  Dem_RecordReader_DTCReadoutBuffer_InitCustomTriggerData(ReadoutBufferId);
}                                                                                                                                /* PRQA S 6050, 6060 */ /* MD_MSR_STCAL, MD_MSR_STPAR */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_InitializeSnapshotIterator()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_InitializeSnapshotIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorType lSnapshotDataIterator;

  lSnapshotDataIterator = *Dem_DTCReadoutBuffer_GetSnapshotIteratorPtr(
                            ReadoutBufferId,
                            ReadoutBufferDataIndex);

# if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  {
    Dem_EventIdType lEventId = Dem_DTCReadoutBuffer_GetEventId(ReadoutBufferId, ReadoutBufferDataIndex);

    Dem_MemoryEntry_SnapshotEntry_IteratorInit(lEventId,
                                   Dem_DTCReadoutBuffer_GetEventEntryIndex(
                                     ReadoutBufferId,
                                     ReadoutBufferDataIndex),
                                   &(lSnapshotDataIterator.SnapshotEntryIter));                                                  /* SBSW_DEM_POINTER_FORWARD_STACK */
  }
# endif

  Dem_Cfg_ComplexIterClear(&(lSnapshotDataIterator.SourceIterator));                                                             /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_DTCReadoutBuffer_SetSnapshotIterator(ReadoutBufferId,                                                                      /* SBSW_DEM_POINTER_FORWARD_STACK */
                                           ReadoutBufferDataIndex,
                                           &lSnapshotDataIterator);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_ExtendedDataRecord_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_ExtendedDataRecord_Init(
  Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorPtrType ExtendedDataIterator                                                      /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  Dem_MemoryEntry_ExtendedEntry_IterInit(DEM_EVENT_INVALID, &(ExtendedDataIterator->ExtendedEntryIter));                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT_STRUCT_MEMBER */
  ExtendedDataIterator->ExtendedEntryIndex = 0u;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SnapshotRecord_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SnapshotRecord_Init(
  Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorPtrType SnapshotDataIterator
  )
{
  Dem_Cfg_ComplexIterClear(&(SnapshotDataIterator->SourceIterator));                                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT_STRUCT_MEMBER */
# if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  Dem_MemoryEntry_SnapshotEntry_IteratorInit(DEM_EVENT_INVALID, DEM_CFG_ENTRYINDEX_INVALID, &(SnapshotDataIterator->SnapshotEntryIter)); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT_STRUCT_MEMBER */
# endif
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetExtendedIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetExtendedIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorConstPtrType  ExtendedDataIterator
  )
{
# if (DEM_CFG_SUPPORT_ERECS == STD_ON)
  Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(                                                                                 /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER_READOUTBUFFERDATA */
    ReadoutBufferId, ReadoutBufferDataIndex)->ExtendedIterator = *ExtendedDataIterator;
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferDataIndex)                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ExtendedDataIterator)                                                                         /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetSnapshotIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetSnapshotIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex,
  Dem_Cfg_DTCReadoutBuffer_SnapshotDataIteratorConstPtrType  SnapshotIterator
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferDataSafe(                                                                                 /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER_READOUTBUFFERDATA */
    ReadoutBufferId, ReadoutBufferDataIndex)->SnapshotIterator = *SnapshotIterator;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetState()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetState(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONST(uint8, AUTOMATIC)  State
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->State = State;                                                     /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_CopyOBDIIFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_CopyOBDIIFreezeFrame(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
# if ((DEM_CFG_SUPPORT_OBDII == STD_ON) && (DEM_CFG_SUPPORT_OBDII_FREEZEFRAME_IN_SVC19 == STD_ON))
  uint8 lSelectedIndex;

  lSelectedIndex = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEntryIndexOfEvent(EventId);
  if (lSelectedIndex < Dem_Memories_ObdFreezeFrameMemory_GetSize())
  {
    Dem_ConstSharedDataPtrType lSourceEntry;
    Dem_DataPtrType lDestinationEntry;
    uint16 lObdFreezeFrameMaxSize;

    lSourceEntry = Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdIIDataPtr(lSelectedIndex);
    lDestinationEntry = Dem_DTCReadoutBuffer_GetPtrToOBDIIFreezeFrameBuffer(ReadoutBufferId);
    lObdFreezeFrameMaxSize = Dem_Cfg_GlobalObdFreezeFrameMaxSize();
    Dem_MemCpy((Dem_SharedDataPtrType)lDestinationEntry,                                                                         /* PRQA S 0311, 0315, 0602 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_POINTER_FORWARD_READOUTBUFFER_OBDFREEZEFRAME */
               (Dem_ConstSharedDataPtrType)lSourceEntry,
               lObdFreezeFrameMaxSize);
    Dem_DTCReadoutBuffer_SetOBDData_IsStored(ReadoutBufferId, TRUE);
  }
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 % Dem_RecordReader_DTCReadoutBuffer_CopyObdOnUdsFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_CopyObdOnUdsFreezeFrame(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  Dem_EventIdType EventId
  )
{
# if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON) 
  uint8 lSelectedIndex;

  lSelectedIndex = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEntryIndexOfEvent(EventId);
  if (lSelectedIndex < Dem_Memories_ObdFreezeFrameMemory_GetSize())
  {
    Dem_ConstSharedDataPtrType lSourceEntry;
    Dem_DataPtrType lDestinationEntry;
    uint16 lObdFreezeFrameMaxSize;
    lObdFreezeFrameMaxSize = Dem_Cfg_GlobalObdFreezeFrameMaxSize();

    lSourceEntry = Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(lSelectedIndex, Dem_RecordReader_0x1904_OBDonUDSFF_0x00);
    lDestinationEntry = Dem_RecordReader_DTCReadoutBuffer_GetPtrToObdOnUdsFreezeFrameBuffer(ReadoutBufferId, Dem_RecordReader_0x1904_OBDonUDSFF_0x00);
    Dem_MemCpy((Dem_SharedDataPtrType)lDestinationEntry,                                                                         /* PRQA S 0311, 0315, 0602 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_POINTER_FORWARD_READOUTBUFFER_OBDFREEZEFRAME */
               (Dem_ConstSharedDataPtrType)lSourceEntry,
                lObdFreezeFrameMaxSize);

    lSourceEntry = Dem_MemoryEntry_ObdFreezeFrameEntry_GetObdOnUdsDataPtr(lSelectedIndex, Dem_RecordReader_0x1904_OBDonUDSFF_0xF0);
    lDestinationEntry = Dem_RecordReader_DTCReadoutBuffer_GetPtrToObdOnUdsFreezeFrameBuffer(ReadoutBufferId, Dem_RecordReader_0x1904_OBDonUDSFF_0xF0);
    Dem_MemCpy((Dem_SharedDataPtrType)lDestinationEntry,                                                                         /* PRQA S 0311, 0315, 0602 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_POINTER_FORWARD_READOUTBUFFER_OBDFREEZEFRAME */
      (Dem_ConstSharedDataPtrType)lSourceEntry,
      lObdFreezeFrameMaxSize);

    Dem_DTCReadoutBuffer_SetOBDData_IsStored(ReadoutBufferId, TRUE);
  }
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_RecordReader_DTCReadoutBuffer_CopyCustomTriggerData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_RecordReader_DTCReadoutBuffer_CopyCustomTriggerData(                                                                         /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  Dem_EventIdType EventId
  )
{
#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON))
  if (Dem_Core_TestCustomTriggerConfigured(EventId))
  {
    Dem_Cfg_EntryIndexType lSelectedIndex = Dem_Memories_CustomTriggerMemory_FindEntryIndex(EventId);
    if (lSelectedIndex != DEM_CFG_ENTRYINDEX_INVALID)
    {
      Dem_Cfg_ConstSharedCustomTriggerEntryPtrType lSourceEntry = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lSelectedIndex);
      Dem_Cfg_CustomTriggerEntryPtrType lDestinationEntry = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(Dem_DTCReadoutBuffer_GetCustomTriggerEntryIndex(ReadoutBufferId));

      Dem_MemCpy((Dem_SharedDataPtrType)lDestinationEntry,                                                                       /* PRQA S 0311, 0315, 0602 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_MEMCPY_CUSTOMTRIGGERENTRY */
        (Dem_ConstSharedDataPtrType)lSourceEntry,
        sizeof(Dem_Cfg_CustomTriggerEntryType));
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
 /* ****************************************************************************
 % Dem_DTCReadoutBuffer_CollectMemIndependentCounter()
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_CollectMemIndependentCounter(
  Dem_DTCReadoutBuffer_HandleType ReadoutBufferId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_EventIdType EventId
)
{
# if (DEM_FEATURE_NEED_COUNTER_READOUT_BUFFER == STD_ON)
  Dem_EventIdType RepresentativeEvent;
  RepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);
#  if (DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON)
    Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MemIndepedentCounter.FaultPendingCounter =                       /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
      Dem_Statistics_CalculateFaultPendingCounter(EventEntryIndex, RepresentativeEvent);
#  endif

#  if (DEM_CFG_DATA_FAILED_CYCLES == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_ON)
    Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MemIndepedentCounter.FailedCycles =                              /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
      Dem_Statistics_CalculateFailedCycleCounter(EventEntryIndex, RepresentativeEvent);
#  endif

#  if (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_MAX_FDC_SLC == STD_ON)
    Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MemIndepedentCounter.MaxFDCSinceLastClear =                      /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
      Dem_Statistics_CalculateMaxFdcSinceLastClear(RepresentativeEvent);
#  endif

#  if (DEM_CFG_DATA_AGINGCTR == STD_ON) && (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_ON)
    Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MemIndepedentCounter.AgingCounter =                              /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
      Dem_Statistics_CalculateUpwardsAgingCounter(EventEntryIndex, RepresentativeEvent);
#  endif

#  if (DEM_CFG_DATA_AGINGCTR_INVERTED == STD_ON) && (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_ON)
    Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MemIndepedentCounter.AgingCounterInvert =                        /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
      Dem_Statistics_CalculateDownwardsAgingCounter(EventEntryIndex, RepresentativeEvent);
#  endif

#  if (DEM_CFG_DATA_CYCLES_TESTED_SINCE_LAST_FAILED == STD_ON)
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MemIndepedentCounter.CyclesTestSinceLastFailed =                   /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
    Dem_Statistics_CalculateCyclesTestedSinceLastFailed(RepresentativeEvent);
#  endif

#  if (DEM_CFG_DATA_HEALINGCTR_DOWNCNT == STD_ON)
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->MemIndepedentCounter.DownwardHealingCounter =                      /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
    Dem_Statistics_CalculateInvertedHealingCounter(RepresentativeEvent);
#  endif
# endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
}
#endif

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
 * \addtogroup Dem_DTCReadoutBuffer_Public
 * \{
 */

#if ( (DEM_FEATURE_NEED_STATISTICAL_DATA == STD_ON) || (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) )
 /* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetMemIndependentCounterPtr
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_ReadoutMemIndeCounterConstPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetMemIndependentCounterPtr(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
)
{
  Dem_Cfg_ReadoutMemIndeCounterConstPtrType lPtr;
# if (DEM_FEATURE_NEED_COUNTER_READOUT_BUFFER == STD_ON)
  if (ReadoutBufferId < DEM_CFG_READOUTBUFFER_INVALID)
  {
    lPtr = &Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->MemIndepedentCounter;
  }
  else
# endif
  {
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
    lPtr = NULL_PTR;
  }

  return lPtr;
}
#endif

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_TestCombinedEventSelected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTCReadoutBuffer_TestCombinedEventSelected(                                                                                  /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH  */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  boolean lReturnValue;
  lReturnValue = FALSE;

  if (Dem_Cfg_EventCombinedGroup(Dem_DTCReadoutBuffer_GetMasterEventId(ReadoutBufferId))
        != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif

#if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetSnapShotHeaderBitMaskIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_DTCReadoutBuffer_GetSnapShotHeaderBitMaskIndex(
  CONSTP2CONST(Dem_DTCReadoutBuffer_RecordNumberIteratorType, AUTOMATIC, AUTOMATIC)  IterPtr,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC) CombinedGroupIdx
  )
{
  uint16_least lCurrentIndex;
  uint16_least lStartIndex;

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CombinedGroupIdx)                                                                             /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lCurrentIndex = IterPtr->mIdx;
  lStartIndex = Dem_Cfg_GetSnapshotDataRecordsOfCombinedDTCs_DTC2FF_IndStartIdxOfCombinedGroups(CombinedGroupIdx);

  /* The difference can only go from 0 to 31. Hence downcasting does not result in data loss */
  return (uint8)(lCurrentIndex - lStartIndex);
}
#endif

#if ((DEM_CFG_SUPPORT_DCM == STD_ON)\
     && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)\
     && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_ERECS == STD_ON)))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetRecordNumberIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DTCReadoutBuffer_RecordNumberIteratorType, DEM_CODE)
Dem_DTCReadoutBuffer_GetRecordNumberIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  Dem_DTCReadoutBuffer_RecordNumberIteratorType lReturnValue;

  lReturnValue = Dem_DTCReadoutBuffer_GetReadoutBuffer(ReadoutBufferId)->RecordNumberIterator;

  return lReturnValue;
}
#endif

#if ((DEM_CFG_SUPPORT_DCM == STD_ON)\
     && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)\
     && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_ERECS == STD_ON)))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SetRecordNumberIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SetRecordNumberIterator(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC)  ReadoutBufferId,
  CONSTP2CONST(Dem_DTCReadoutBuffer_RecordNumberIteratorType, AUTOMATIC, AUTOMATIC) RecordIterPtr
  )
{
  Dem_DTCReadoutBuffer_GetReadoutBufferSafe(ReadoutBufferId)->RecordNumberIterator = *RecordIterPtr;                             /* SBSW_DEM_ARRAY_POINTER_READOUTBUFFER */
}
#endif

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetGlobalSnapshotDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_DTCReadoutBuffer_GetGlobalSnapshotDataPtr(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  return &(Dem_DTCReadoutBuffer_GetReadoutBufferData(
             ReadoutBufferId, ReadoutBufferDataIndex)->Data.GlobalSnapshotData[0]);
}
#endif


/* ****************************************************************************
 % Dem_DTCReadoutBuffer_TestGlobalSnapshotStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_DTCReadoutBuffer_TestGlobalSnapshotStored(                                                                                   /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH  */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, AUTOMATIC)  ReadoutBufferDataIndex
  )
{
  boolean lGlobalSRecStored = FALSE;

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
  lGlobalSRecStored = Dem_DTCReadoutBuffer_GetReadoutBufferData(
                        ReadoutBufferId, ReadoutBufferDataIndex)->Data.GlobalSnapshotRecordStored;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferId)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ReadoutBufferDataIndex)                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif

  return lGlobalSRecStored;
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_Init(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId
  )
{
  boolean lCombinedDTCSelected;

#if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  lCombinedDTCSelected = TRUE;
#else
  lCombinedDTCSelected = FALSE;
#endif

  Dem_DTCReadoutBuffer_SetReadoutBufferEntry(ReadoutBufferId,
                                             DEM_EVENT_INVALID,
                                             DEM_CFG_MEMORYID_INVALID,
                                             DEM_DTCREADOUTBUFFER_STATUS_UNUSED,
                                             lCombinedDTCSelected);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_CheckAndSetState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_DTCReadoutBuffer_CheckAndSetState(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_EventIdType, AUTOMATIC)                 EventId,
  CONST(uint16, AUTOMATIC)                          MemoryId,
  CONST(Dem_DTCSelector_HandleType, AUTOMATIC)      DTCSelector
  )
{
  Std_ReturnType lReturnValue;
  boolean lCombinedDTCSelected;
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  Dem_Cfg_CombinedGroupIndexType lGroupId;
# endif

  if (Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_UNUSED)
  {
    /* new request */
    Dem_DTCSelector_LockSelectDTC(DTCSelector);

# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
    if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      lCombinedDTCSelected = TRUE;
    }
    else
# endif
    {
      lCombinedDTCSelected = FALSE;
    }

    Dem_DTCReadoutBuffer_SetReadoutBufferEntry(ReadoutBufferId,
                                               EventId,
                                               MemoryId,
                                               DEM_DTCREADOUTBUFFER_STATUS_QUEUED,
                                               lCombinedDTCSelected);

    /* Scheduler task needs to be enabled last. This ensures readout buffer data are
       written back to memory before executing the task.*/
    Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_FilterEventData);
    lReturnValue = DEM_PENDING;
  }
  else
  if (
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
      (EventId == Dem_DTCReadoutBuffer_GetMasterEventId(ReadoutBufferId))
# else
      (EventId == Dem_DTCReadoutBuffer_GetEventId(ReadoutBufferId, DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT))
# endif
     && (MemoryId == Dem_DTCReadoutBuffer_GetMemoryId(ReadoutBufferId))                                                          /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
     )
  {
    /* repeated request with identical parameters */
    if (Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_QUEUED)
    {
      /* request is still queued, copying of data is not finished */
      lReturnValue = DEM_PENDING;
    }
    else
    {
      /* copying of data is finished */
      lReturnValue = E_OK;
    }
  }
  else
  {
    /* repeated request with different parameters */
    lReturnValue = E_NOT_OK;
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_FillData_NormalEvent
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_FillData_NormalEvent(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCSelector_HandleType, AUTOMATIC)      DTCSelector
)
{
  if (ReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)
  {
    /* copy event entry only for readout buffer in state QUEUED */
    if (Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_QUEUED)
    {
      Dem_EventIdType lEventId = Dem_DTCReadoutBuffer_GetEventId(ReadoutBufferId, DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT);
      Dem_Cfg_EntryIndexType lEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(lEventId, FALSE);
      Dem_Cfg_EntryIndexType lReadoutBufferEventEntryIndex = Dem_DTCReadoutBuffer_GetEventEntryIndex(ReadoutBufferId, DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT);

      if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
      {
        Dem_RecordReader_DTCReadoutBuffer_CopyObdOnUdsFreezeFrame(ReadoutBufferId, lEventId);
      }
      else if (Dem_RecordReader_0x1904_IsObdIIFreezeFrameInService19EnabledInVariant() == TRUE)
      {
        Dem_DTCReadoutBuffer_CopyOBDIIFreezeFrame(ReadoutBufferId, lEventId);
      }
      else
      {
        /* MISRA Case */
      }

      Dem_RecordReader_DTCReadoutBuffer_CopyCustomTriggerData(ReadoutBufferId, lEventId);

      if (lEntryIndex == DEM_CFG_ENTRYINDEX_INVALID)
      {
        Dem_MemoryEntry_EventEntry_SetEventId(lReadoutBufferEventEntryIndex, DEM_EVENT_INVALID);
        Dem_MemoryEntry_EventEntry_InitializeEntry(lReadoutBufferEventEntryIndex);
        Dem_EnterCritical_DcmApi();
        if (Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_QUEUED)
        {
          Dem_DTCReadoutBuffer_SetState(ReadoutBufferId, DEM_DTCREADOUTBUFFER_STATUS_NOT_STORED);
          Dem_DTCSelector_ReleaseSelectDTC(DTCSelector);
        }
        Dem_LeaveCritical_DcmApi();
      }
      else
      {
        Dem_MemoryEntry_EventEntry_CopyEntry(lReadoutBufferEventEntryIndex, lEntryIndex);
        Dem_EnterCritical_DcmApi();
        if (Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_QUEUED)
        {
          Dem_DTCReadoutBuffer_SetState(ReadoutBufferId, DEM_DTCREADOUTBUFFER_STATUS_IN_USE);
          Dem_DTCSelector_ReleaseSelectDTC(DTCSelector);
        }
        Dem_LeaveCritical_DcmApi();
      }
      Dem_DTCReadoutBuffer_CollectMemIndependentCounter(ReadoutBufferId, lReadoutBufferEventEntryIndex, lEventId);

# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
      lEntryIndex = Dem_Memories_TimeSeriesMemory_FindEntryIndex(lEventId);
      if (lEntryIndex == DEM_CFG_ENTRYINDEX_INVALID)
      {
        /* no time series entry for event - set invalid time series entry index in readout buffer */
        Dem_DTCReadoutBuffer_SetTimeSeriesEntryIndex(ReadoutBufferId, lEntryIndex);
      }
      else
      {
        /* Lock time series entry. An valid entry index indicates that the entry has a state of UNLOCKED or LOCKED, so that the precondition for locking is fulfilled */
        Dem_MemoryEntry_TimeSeriesEntry_Lock(lEntryIndex);
        /* Store the found time series entry index in readout buffer */
        Dem_DTCReadoutBuffer_SetTimeSeriesEntryIndex(ReadoutBufferId, lEntryIndex);
        Dem_DTCReadoutBuffer_SetTimeSeriesEntryUnusedFutureSamples(ReadoutBufferId, lEntryIndex);
      }
# endif
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_FillData_CombinedEvent
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_FillData_CombinedEvent(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCSelector_HandleType, AUTOMATIC)      DTCSelector
)
{ /* copy event entry only for valid readout buffer in state QUEUED */
  if ((ReadoutBufferId != DEM_CFG_READOUTBUFFER_INVALID)
      && (Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_QUEUED))                                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
  {
    Dem_Cfg_CombinedGroupIterType lCombinedIter;
    Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType lReadoutBufferDataIndex = 0u;
    uint8 lReadoutBufferState = DEM_DTCREADOUTBUFFER_STATUS_NOT_STORED;
    Dem_Cfg_CombinedGroupIndexType lGroupId
      = Dem_Cfg_EventCombinedGroup(Dem_DTCReadoutBuffer_GetMasterEventId(ReadoutBufferId));

    /* Iterate through all sub-events */
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEventId;
      lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if (Dem_RecordReader_0x1904_IsObdIIFreezeFrameInService19EnabledInVariant() == TRUE)
      {
        /* Only one subevent can have OBDII freeze frame stored */
        if ( (Dem_Core_DTC_IsObdRelated(lSubEventId) == TRUE)
          && (Dem_DTCReadoutBuffer_GetOBDData_IsStored(ReadoutBufferId) == FALSE) )                                              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        {
          Dem_DTCReadoutBuffer_CopyOBDIIFreezeFrame(ReadoutBufferId, lSubEventId);
        }
      }

      if (Dem_Event_InternalStatus_TestEventStored(lSubEventId) == TRUE)
      {
        Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(lSubEventId, FALSE);
        if ( (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
          && (lReadoutBufferDataIndex < Dem_Cfg_SizeOfReadOutBufferData()))                                                      
        {
          Dem_Cfg_EntryIndexType lReadoutBufferEventEntryIndex;
          lReadoutBufferEventEntryIndex = Dem_DTCReadoutBuffer_GetEventEntryIndex(
                                          ReadoutBufferId,
                                          lReadoutBufferDataIndex);
          Dem_MemoryEntry_EventEntry_CopyEntry(lReadoutBufferEventEntryIndex, lEventEntryIndex);
          Dem_DTCReadoutBuffer_SetEventId(ReadoutBufferId,
                                          lReadoutBufferDataIndex,
                                          lSubEventId);
          lReadoutBufferDataIndex++;
        }
        else
        {
          /* Either there are more number of subevents stored
              OR generated arrays are smaller than expected
              OR a subevent entry was deleted during copying. */
          Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE)
        }
      }
    }
    Dem_DTCReadoutBuffer_SetNumberOfOccupiedSlots(ReadoutBufferId, lReadoutBufferDataIndex);
    if (lReadoutBufferDataIndex > 0u)
    {
      lReadoutBufferState = DEM_DTCREADOUTBUFFER_STATUS_IN_USE;
    }

    /* Initialize all unused event entries */
    while ((lReadoutBufferDataIndex < Dem_Cfg_SizeOfReadOutBufferData()))
    {
      Dem_Cfg_EntryIndexType lReadoutBufferEventEntryIndex = Dem_DTCReadoutBuffer_GetEventEntryIndex(ReadoutBufferId, lReadoutBufferDataIndex);
      Dem_MemoryEntry_EventEntry_InitializeEntry(lReadoutBufferEventEntryIndex);
      lReadoutBufferDataIndex++;
    }

    /* Set new ReadoutBuffer state and release SelectDTC*/
    Dem_EnterCritical_DcmApi();
    if (Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_QUEUED)
    {
      Dem_DTCReadoutBuffer_SetState(ReadoutBufferId, lReadoutBufferState);
      Dem_DTCSelector_ReleaseSelectDTC(DTCSelector);
    }
    Dem_LeaveCritical_DcmApi();
  }
}                                                                                                                                /* PRQA S 6010, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_FreeBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_FreeBuffer(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_DTCSelector_HandleType, AUTOMATIC)      DTCSelector
  )
{
# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
  if(Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_IN_USE)
  {
    lTimeSeriesEntryIndex = Dem_DTCReadoutBuffer_GetTimeSeriesEntryIndex(ReadoutBufferId);
  }
  if (lTimeSeriesEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    Dem_Memories_TimeSeriesMemory_ReleaseEntry(lTimeSeriesEntryIndex);
  }
# endif

/* >>>> -------------------------------- Enter Critical Section: DcmApi */
  Dem_EnterCritical_DcmApi();
  if (Dem_DTCReadoutBuffer_GetState(ReadoutBufferId) == DEM_DTCREADOUTBUFFER_STATUS_QUEUED)
  {
    Dem_DTCSelector_ReleaseSelectDTC(DTCSelector);
  }

  Dem_DTCReadoutBuffer_SetState(ReadoutBufferId, DEM_DTCREADOUTBUFFER_STATUS_UNUSED);
  Dem_LeaveCritical_DcmApi();
/* <<<< -------------------------------- Leave Critical Section: DcmApi */

}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SelectExtendedDataRecord
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SelectExtendedDataRecord(
CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
CONST(uint8, AUTOMATIC) ExtendedDataNumber
)
{
# if (DEM_CFG_SUPPORT_ERECS == STD_ON)
  Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType lReadoutBufferDataIndex;
  uint8 lNumOccupiedReadoutBufferSlots;

#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  if (Dem_DTCReadoutBuffer_TestCombinedEventSelected(ReadoutBufferId) == TRUE)
  {
    lNumOccupiedReadoutBufferSlots = Dem_DTCReadoutBuffer_GetNumberOfOccupiedSlots(ReadoutBufferId);
  }
  else
#  endif
  {
    lNumOccupiedReadoutBufferSlots = 0x01U;
  }

  lReadoutBufferDataIndex = DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT;
  for ( /* Initialized to default slot */;
       lReadoutBufferDataIndex < lNumOccupiedReadoutBufferSlots;
       lReadoutBufferDataIndex++)
  {
    Dem_EventIdType lEventId;
    Dem_Cfg_DTCReadoutBuffer_ExtendedDataIteratorType lExtendedDataIterator;

    lEventId = Dem_DTCReadoutBuffer_GetEventId(ReadoutBufferId, lReadoutBufferDataIndex);
    lExtendedDataIterator = *Dem_DTCReadoutBuffer_GetExtendedIteratorPtr(ReadoutBufferId, lReadoutBufferDataIndex);
    Dem_MemoryEntry_ExtendedEntry_IterInit(lEventId, &(lExtendedDataIterator.ExtendedEntryIter));                                /* SBSW_DEM_POINTER_FORWARD_STACK */
    lExtendedDataIterator.ExtendedEntryIndex = 0u;
    Dem_DTCReadoutBuffer_SetExtendedIterator(ReadoutBufferId, lReadoutBufferDataIndex, &lExtendedDataIterator);                  /* SBSW_DEM_POINTER_FORWARD_STACK */
  }

#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  /* Initialize the DTC specific ERec Iterators */
  if (Dem_DTCReadoutBuffer_TestCombinedEventSelected(ReadoutBufferId) == TRUE)
  {
    Dem_DTCReadoutBuffer_RecordNumberIteratorType lReadoutBufferRecordNumberIter;

    Dem_Cfg_DTCExtendedDataRecordIterInit(&lReadoutBufferRecordNumberIter,                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
                                          Dem_Cfg_EventCombinedGroup(Dem_DTCReadoutBuffer_GetMasterEventId(ReadoutBufferId)));
    Dem_DTCReadoutBuffer_SetRecordNumberIterator(ReadoutBufferId, &lReadoutBufferRecordNumberIter);                              /* SBSW_DEM_POINTER_FORWARD_STACK */
  }
#  endif
# endif

  Dem_DTCReadoutBuffer_SetExtendedRecordNumber(ReadoutBufferId, ExtendedDataNumber);
  Dem_DTCReadoutBuffer_SetExtendedRecordSelected(ReadoutBufferId, TRUE);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_SelectSnapshotRecord
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_DTCReadoutBuffer_SelectSnapshotRecord(
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(uint8, AUTOMATIC) RecordNumber
)
{
  Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType lReadoutBufferDataIndex = DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT;
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  uint8 lNumOccupiedReadoutBufferSlots;

  if (Dem_DTCReadoutBuffer_TestCombinedEventSelected(ReadoutBufferId) == TRUE)
  {
    lNumOccupiedReadoutBufferSlots = Dem_DTCReadoutBuffer_GetNumberOfOccupiedSlots(ReadoutBufferId);
    for ( /* Initialized to default slot */;
         lReadoutBufferDataIndex < lNumOccupiedReadoutBufferSlots;
         lReadoutBufferDataIndex++)
    {
      Dem_DTCReadoutBuffer_InitializeSnapshotIterator(ReadoutBufferId, lReadoutBufferDataIndex);
    }
  }
  else
# endif
  {
    Dem_DTCReadoutBuffer_InitializeSnapshotIterator(ReadoutBufferId, lReadoutBufferDataIndex);
  }

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  {
    Dem_EventIdType lEventId = Dem_DTCReadoutBuffer_GetEventId(ReadoutBufferId, DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT);
    Dem_DTCReadoutBuffer_SnapshotSource_SetCurrentTimeSeriesId(ReadoutBufferId, Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber(lEventId, RecordNumber));
  }
#endif

  Dem_DTCReadoutBuffer_SetSnapshotRecordNumber(ReadoutBufferId, RecordNumber);
  Dem_DTCReadoutBuffer_SetSnapshotRecordSource(ReadoutBufferId, Dem_DTCReadoutBuffer_SnapshotSource_Invalid);
  Dem_DTCReadoutBuffer_SetSnapshotRecordSelected(ReadoutBufferId, TRUE);
}
#endif

#if ((DEM_CFG_SUPPORT_ERECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_DTCReadoutBuffer_GetEventDataIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType, DEM_CODE)
Dem_DTCReadoutBuffer_GetEventDataIndex(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_DTCReadoutBuffer_HandleType, AUTOMATIC) ReadoutBufferId,
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  boolean lEventFound;
  uint8 lNumOccupiedReadoutBufferSlots;
  Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType lReadoutBufferDataIndex;

  lEventFound = FALSE;
  lReadoutBufferDataIndex = DEM_DTCREADOUTBUFFER_DATAINDEX_DEFAULT;
  lNumOccupiedReadoutBufferSlots = Dem_DTCReadoutBuffer_GetNumberOfOccupiedSlots(ReadoutBufferId);

  for ( /* Initialized to default slot */;
      lReadoutBufferDataIndex < lNumOccupiedReadoutBufferSlots;
      lReadoutBufferDataIndex++)
  {
    if (Dem_DTCReadoutBuffer_GetEventId(ReadoutBufferId, lReadoutBufferDataIndex) == EventId)
    {
      lEventFound = TRUE;
    }

    /* In an ordered list, no point to search for entries greater than the Event Id.
       This means the Event Id is not stored in memory */
    if (Dem_DTCReadoutBuffer_GetEventId(ReadoutBufferId, lReadoutBufferDataIndex) >= EventId)
    {
      break;
    }
  }

  if (lEventFound == FALSE)
  {
    lReadoutBufferDataIndex = (Dem_DTCReadoutBuffer_ReadoutBufferDataIndexType)Dem_Cfg_SizeOfReadOutBufferData();
  }

  return lReadoutBufferDataIndex;
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_DTCREADOUTBUFFER_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_DTCReadoutBuffer_Implementation.h
 *********************************************************************************************************************/
