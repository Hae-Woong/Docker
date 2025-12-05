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
 *  \defgroup   Dem_DataStorage DataStorage
 *  \{
 *  \file       Dem_DataStorage_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     DataStorage
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DATASTORAGE_INTERFACE_H)
#define DEM_DATASTORAGE_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_DataStorage_Types.h"
#include "Dem_Data_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_DataStorage_Public Public Methods
 * \{
 */

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_PrestorageApplyGlobalSRec
 *****************************************************************************/
/*!
 * \brief         Copies a prestored global snapshot for an event
 *
 * \details       This function copies the prestored data only if a prestored
 *                entry has been locked for the event. Otherwise, this function
 *                does not modify the destination buffer.
 *
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \return        TRUE
 *                Data has been copied
 * \return        FALSE
 *                No data has been copied
 *
 * \pre           A Prestorage entry has been locked
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON &&
 *                DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplyGlobalSRec(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_PrestorageApplySRec
 *****************************************************************************/
/*!
 * \brief         Copies a prestored snapshot for an event
 *
 * \details       This function copies the prestored data only if a prestored
 *                entry has been locked for the event. Otherwise, this function
 *                does not modify the destination buffer.
 *
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \return        TRUE
 *                Data has been copied
 * \return        FALSE
 *                No data has been copied
 *
 * \pre           A Prestorage entry has been locked
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON &&
 *                DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplySRec(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif

#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_PrestorageApplyObdFF
 *****************************************************************************/
/*!
 * \brief         Copies a prestored OBD freeze frame for an event
 *
 * \details       This function copies the prestored data only if a prestored
 *                entry has been locked for the event. Otherwise, this function
 *                does not modify the destination buffer.
 *
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \return        TRUE
 *                Data has been copied
 * \return        FALSE
 *                No data has been copied
 *
 * \pre           A Prestorage entry has been locked.
 *                Function can only be called in OBD on UDS or OBDII variant.
 * \config        DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON &&
 *                DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplyObdFF(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_PrestorageApplyWwhObdFF
 *****************************************************************************/
/*!
 * \brief         Copies a prestored WWH-OBD freeze frame for an event
 *
 * \details       This function copies the prestored data only if a prestored
 *                entry has been locked for the event. Otherwise, this function
 *                does not modify the destination buffer.
 *
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \return        TRUE
 *                Data has been copied
 * \return        FALSE
 *                No data has been copied
 *
 * \pre           A Prestorage entry has been locked
 * \config        DEM_CFG_SUPPORT_WWHOBD == STD_ON &&
 *                DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplyWwhObdFF(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_PrestorageApplyJ1939FF
 *****************************************************************************/
/*!
 * \brief         Copies a prestored J1939 freeze frame for an event (both
 *                normal and expanded)
 *
 * \details       This function copies the prestored data only if a prestored
 *                entry has been locked for the event. Otherwise, this function
 *                does not modify the destination buffer.
 *
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \return        TRUE
 *                Data has been copied
 * \return        FALSE
 *                No data has been copied
 *
 * \pre           A Prestorage entry has been locked
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON &&
 *                DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplyJ1939FF(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_CollectObdIIFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Samples an OBD freeze frame for an event.
 *
 * \details       Collect each PID configured for the given event's OBD-II
 *                freeze frame.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \pre           DEM_CFG_SUPPORT_OBDII == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectObdIIFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif


#if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON) || (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Samples a OBD freeze frame for an event.
 *
 * \details       Collect each DID configured for the given event's
 *                OBD FreezeFrame.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_OBDONUDS == STD_ON
 *                || DEM_CFG_SUPPORT_WWHOBD == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame(
  Dem_EventIdType EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  uint16 BufferSize
  );
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_CollectJ1939FreezeFrame
 *****************************************************************************/
/*!
 * \brief         Samples J1939 freeze frame(s) for an event
 *
 * \details       Collect each DID configured for the given event's J1939
 *                freeze frame(s) (both normal and expanded).
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 * 
 * \pre           -
 * \config        DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectJ1939FreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_CollectGlobalSnapshot
 *****************************************************************************/
/*!
 * \brief         Samples global snapshot record of an event
 *
 * \details       Samples global snapshot record of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectGlobalSnapshot(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_CollectSnapshot
 *****************************************************************************/
/*!
 * \brief         Samples a snapshot for an event
 *
 * \details       Samples a snapshot for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] DestinationBuffer
 *                Pointer to the buffer to store the data
 * \param[in]     BufferSize
 *                Size of the destination buffer
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectSnapshot(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  );
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_CollectERec
 *****************************************************************************/
/*!
 * \brief         Samples extended data record for an event
 *
 * \details       Samples extended data record for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     DidIndex
 *                Unique handle of the DID
 * \param[in,out] DestinationBuffer
 *                Pointer to the destination buffer structure to store the data
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_USER_ERECS == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectERec(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_DidIndexType,AUTOMATIC) DidIndex,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  );
#endif

/* ****************************************************************************
 * Dem_StorageManager_DataStorage_CollectTimeSeriesSamples
 *****************************************************************************/
/*!
 * \brief         Store DIDs for the specified sampling profile in given buffer
 *
 * \details       Store DIDs for the specified sampling profile in given buffer
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 *
 * \pre           timer for sampling profile has expired
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectTimeSeriesSamples(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  Dem_Data_DestinationBufferPtrType DestinationBuffer
  );

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_GetSamplingProfileDidInfoTableId
 *****************************************************************************/
/*!
 * \brief         Find the did info table id of a specified did for a specified time series
 *
 * \details       Find the did info table id of a specified did for a specified time series
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * \param[in]     DidIndex
 *                Specifies a DID
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_StorageManager_DataStorage_GetSamplingProfileDidInfoTableId(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  Dem_Cfg_DidIndexType DidIndex
  );
#endif

/* ****************************************************************************
 * Dem_StorageManager_DataStorage_GetDidNVSize
 *****************************************************************************/
/*!
 * \brief         Sums up size of all non-volatile stored data elements
 *
 * \details       Sums up size of all non-volatile stored data elements
 *
 * \param[in]     DidIndex
 *                Unique handle of the DID
 *
 * \return        Number of bytes used in NV-Ram for the DID
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_StorageManager_DataStorage_GetDidNVSize(
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_DATASTORAGE_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_DataStorage_Interface.h
 *********************************************************************************************************************/
