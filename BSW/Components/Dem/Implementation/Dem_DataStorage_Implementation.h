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
/*! \addtogroup Dem_DataStorage
 *  \{
 *  \file       Dem_DataStorage_Implementation.h
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

#if !defined (DEM_DATASTORAGE_IMPLEMENTATION_H)
#define DEM_DATASTORAGE_IMPLEMENTATION_H

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
#include "Dem_DataStorage_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_PrestorageMemory_Interface.h"
#include "Dem_PrestorageEntry_Interface.h"
#include "Dem_ExtDataElement_Interface.h"
#include "Dem_FaultMemory_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_DATASTORAGE_IMPLEMENTATION_FILENAME "Dem_DataStorage_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_DataStorageIF_Private Private Methods
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_TimeSeriesDidIterGet
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Did index
 *
 * \pre           IterPtr has been initialized
 * \config        DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_StorageManager_DataStorage_TimeSeriesDidIterGet(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_DataStorage_TimeSeriesDidIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate DIDs relevant for a 
 *                time series
 *
 * \param[out]    IterPtr
 *                Pointer to the iterator
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_TimeSeriesDidIterInit(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr,
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  );
#endif


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_TimeSeriesDidIterGet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_StorageManager_DataStorage_TimeSeriesDidIterGet(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetDataCollectionTableIdxOfSamplingProfileDidInfoTable(IterPtr->mIdx);
}
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_TimeSeriesDidIterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_TimeSeriesDidIterInit(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr,
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  )
{
  Dem_Cfg_SamplingProfileIterType samplingProfileDidIndirectionId =
    Dem_Cfg_GetSamplingProfileDidInfoIndirectionTableIdxOfTimeSeriesSamplingProfileTable(SamplingProfileId);
  DEM_IGNORE_UNUSED_ARGUMENT(SamplingProfileId);                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_VARIABLE(samplingProfileDidIndirectionId);                                                                   /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  
  IterPtr->mIdx = 
    Dem_Cfg_GetSamplingProfileDidInfoTableStartIdxOfSamplingProfileDidInfoIndirectionTable(samplingProfileDidIndirectionId);     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = 
    Dem_Cfg_GetSamplingProfileDidInfoTableEndIdxOfSamplingProfileDidInfoIndirectionTable(samplingProfileDidIndirectionId);       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
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
 * \addtogroup Dem_DataStorageIF_Public
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_GetSamplingProfileDidInfoTableId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_StorageManager_DataStorage_GetSamplingProfileDidInfoTableId(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  Dem_Cfg_DidIndexType DidIndex
  )
{
  Dem_Cfg_ComplexIterType lDidIter;
  for (Dem_StorageManager_DataStorage_TimeSeriesDidIterInit(&lDidIter, SamplingProfileId);                                       /* SBSW_DEM_POINTER_FORWARD_STACK */
      Dem_Cfg_ComplexIterExists(&lDidIter) == TRUE;                                                                              /* SBSW_DEM_POINTER_FORWARD_STACK */
      Dem_Cfg_ComplexIterNext(&lDidIter))                                                                                        /* SBSW_DEM_POINTER_FORWARD_STACK */
  {
    if(DidIndex == Dem_StorageManager_DataStorage_TimeSeriesDidIterGet(&lDidIter))                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      break;
    }
  }
  return lDidIter.mIdx;
}
#endif

/* ****************************************************************************
 % Dem_StorageManager_DataStorage_GetDidNVSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_StorageManager_DataStorage_GetDidNVSize(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_Cfg_DidIndexType, AUTOMATIC)  DidIndex
  )
{
  uint16 lDidTotalSize;

  Dem_Cfg_DataIndexType lDataIndex;
  Dem_Cfg_DidDataIterType lDataIter;

  lDidTotalSize = 0u;
  for (Dem_Cfg_DidDataIterInit(DidIndex, &lDataIter);                                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_DidDataIterExists(&lDataIter) == TRUE;                                                                            /* PRQA S 2994 */ /* MD_DEM_2994 */  /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_DidDataIterNext(&lDataIter))                                                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    lDataIndex = Dem_Cfg_DidDataIterGet(&lDataIter);                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */

    if (Dem_Cfg_DataIsStoredInNV(lDataIndex) == TRUE)
    {
      switch (Dem_Cfg_DataCallbackType(lDataIndex))
      {
      case DEM_CFG_DATA_FROM_ROOTCAUSE_EVENTID:
      case DEM_CFG_DATA_FROM_CS_PORT:
      case DEM_CFG_DATA_FROM_CS_PORT_WITH_EVENTID:
      case DEM_CFG_DATA_FROM_FCT_WITH_EVENTID_BUFFERLENGTH:
      case DEM_CFG_DATA_FROM_FCT_WITH_PROXY:
      case DEM_CFG_DATA_FROM_SR_PORT:
          lDidTotalSize += Dem_Cfg_DataSize(lDataIndex);
          break;

      default:                                                                                                                   /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
        /* Do nothing */
          break;
      }
    }
  }

  return lDidTotalSize;
}


#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_PrestorageApplyGlobalSRec
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplyGlobalSRec(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  boolean lReturnValue;
  Dem_Cfg_PrestorageIndexType lPrestoreLockedIndex;

  lReturnValue = FALSE;

  lPrestoreLockedIndex = Dem_Memories_PrestorageMemory_GetLockedIndex();
  if (DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY != lPrestoreLockedIndex)
  {
    Dem_MemCpy(DestinationBuffer,                                                                                                /* PRQA S 0310, 0315, 0602 */ /* MD_DEM_11.4_cpy, MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_WRITE_ARGUMENT_BUFFER */
               Dem_MemoryEntry_PrestorageEntry_GetGlobalSRecDataPtr(lPrestoreLockedIndex),
               BufferSize);

    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_PrestorageApplySRec
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplySRec(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  boolean lReturnValue;
  Dem_Cfg_PrestorageIndexType lPrestoreLockedIndex;

  lReturnValue = FALSE;

  lPrestoreLockedIndex = Dem_Memories_PrestorageMemory_GetLockedIndex();
  if (DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY != lPrestoreLockedIndex)
  {
    Dem_ConstDataPtrType lPrestorageBuffer;
    lPrestorageBuffer = Dem_MemoryEntry_PrestorageEntry_GetSRecDataPtr(lPrestoreLockedIndex);
    Dem_MemCpy(DestinationBuffer,                                                                                                /* PRQA S 0310, 0315, 0602, 0311 */ /* MD_DEM_11.4_cpy, MD_MSR_VStdLibCopy, MD_DEM_20.2, MD_DEM_11.8 */  /* SBSW_DEM_POINTER_WRITE_ARGUMENT_BUFFER */
               lPrestorageBuffer,
               BufferSize);

    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) */

#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_PrestorageApplyObdFF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplyObdFF(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  boolean lReturnValue;
  Dem_Cfg_PrestorageIndexType lPrestoreLockedIndex;
  
  lReturnValue = FALSE;


  lPrestoreLockedIndex = Dem_Memories_PrestorageMemory_GetLockedIndex();
  if (DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY != lPrestoreLockedIndex)
  {
    Dem_MemCpy(DestinationBuffer,                                                                                                /* PRQA S 0310, 0315, 0602 */ /* MD_DEM_11.4_cpy, MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_WRITE_ARGUMENT_BUFFER */
                Dem_MemoryEntry_PrestorageEntry_GetObdFFDataPtr(lPrestoreLockedIndex),
                BufferSize);

    lReturnValue = TRUE;
  }
  
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_PrestorageApplyWwhObdFF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplyWwhObdFF(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  boolean lReturnValue;
  Dem_Cfg_PrestorageIndexType lPrestoreLockedIndex;

  lReturnValue = FALSE;

  lPrestoreLockedIndex = Dem_Memories_PrestorageMemory_GetLockedIndex();
  if (DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY != lPrestoreLockedIndex)
  {
    Dem_MemCpy(DestinationBuffer,                                                                                                /* PRQA S 0310, 0315, 0602 */ /* MD_DEM_11.4_cpy, MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_WRITE_ARGUMENT_BUFFER */
               Dem_MemoryEntry_PrestorageEntry_GetWwhObdFFDataPtr(lPrestoreLockedIndex),
               BufferSize);

    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) && (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_PrestorageApplyJ1939FF
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_DataStorage_PrestorageApplyJ1939FF(
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  boolean lReturnValue;
  Dem_Cfg_PrestorageIndexType lPrestoreLockedIndex;

  lReturnValue = FALSE;

  lPrestoreLockedIndex = Dem_Memories_PrestorageMemory_GetLockedIndex();
  if (DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY != lPrestoreLockedIndex)
  {
    Dem_MemCpy(DestinationBuffer,                                                                                                /* PRQA S 0310, 0315, 0602 */ /* MD_DEM_11.4_cpy, MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_WRITE_ARGUMENT_BUFFER */
               Dem_MemoryEntry_PrestorageEntry_GetJ1939FFDataPtr(lPrestoreLockedIndex),
               BufferSize);

    lReturnValue = TRUE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_CollectObdIIFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectObdIIFreezeFrame(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  Dem_ExtDataElement_CollectDidInfoType lCollectDidInfo;
  Dem_Cfg_GlobalPidIterType lPidIter;
  Dem_Data_DestinationBuffer lDestinationBuffer;

  /* Initialize collection context */
  Dem_Data_InitDestinationBuffer(&lDestinationBuffer, DestinationBuffer, BufferSize);                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  Dem_ExtDataElement_InitCollectDidInfo(&lCollectDidInfo                                                                         /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */                                                          
                                         ,&lDestinationBuffer
                                         ,EventId);

  for (Dem_Cfg_GlobalPidIterInit(&lPidIter);                                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_GlobalPidIterExists(&lPidIter) == TRUE;                                                                           /* PRQA S 2994 */ /* MD_DEM_2994 */  /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_GlobalPidIterNext(&lPidIter))                                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_DidIndexType lDidIndex;
    lDidIndex = Dem_Cfg_GlobalPidIterGet(&lPidIter);                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_ExtDataElement_CollectStoredDid(&lCollectDidInfo, lDidIndex);                                                            /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO */
  }
}
#endif


#if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON) || (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame(
  Dem_EventIdType EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  uint16 BufferSize
  )
{
  Dem_ExtDataElement_CollectDidInfoType lCollectDidInfo;
  Dem_Data_DestinationBuffer lDestinationBuffer;
  Dem_Cfg_EventObdDidIterType lDidIter;

  /* Initialize collection context */
  Dem_Data_InitDestinationBuffer(&lDestinationBuffer, DestinationBuffer, BufferSize);                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  Dem_ExtDataElement_InitCollectDidInfo(&lCollectDidInfo                                                                         /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */  
                                         ,&lDestinationBuffer
                                         ,EventId);

  for (Dem_Cfg_EventObdDidIterInit(EventId, &lDidIter);                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventObdDidIterExists(&lDidIter) == TRUE;                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventObdDidIterNext(&lDidIter))                                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_DidIndexType lDidIndex;
    lDidIndex = Dem_Cfg_EventObdDidIterGet(&lDidIter);                                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_ExtDataElement_CollectStoredDid(&lCollectDidInfo, lDidIndex);                                                            /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO */
  }
}
#endif


#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_CollectJ1939FreezeFrame
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectJ1939FreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  Dem_Data_DestinationBuffer lDestinationBuffer;
  Dem_ExtDataElement_CollectDidInfoType lCollectDidInfo;

  boolean lBufferTooSmall = FALSE;
  Dem_Data_InitDestinationBuffer(&lDestinationBuffer, DestinationBuffer, BufferSize);                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  Dem_ExtDataElement_InitCollectDidInfo(&lCollectDidInfo                                                                         /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */  
                                         , &lDestinationBuffer
                                         , EventId);

  if (Dem_Cfg_EventJ1939FFExists(EventId) == FALSE)
  { /* skip the buffer area reserved for standard FreezeFrame */
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (Dem_Data_GetRemainingSizeOfDestinationBuffer(lCollectDidInfo.DestinationBuffer) < Dem_Cfg_GlobalJ1939FFRawSize())        /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO_DESTINATIONBUFFER */
    {
      Dem_Error_RunTimeCheckFailed(DEM_DATASTORAGE_IMPLEMENTATION_FILENAME, __LINE__);                                           /* SBSW_DEM_POINTER_RUNTIME_CHECK */
      lBufferTooSmall = TRUE;
    }
    else
# endif
    {
      Dem_Data_IncrementWriteIndexDestinationBuffer(lCollectDidInfo.DestinationBuffer, Dem_Cfg_GlobalJ1939FFRawSize());          /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO_DESTINATIONBUFFER */
    }
  }

  if (lBufferTooSmall == FALSE)
  {
    Dem_Cfg_EventSPNIterType lSpnIter;
    for (Dem_Cfg_EventSPNIterInit(EventId, &lSpnIter);                                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventSPNIterExists(&lSpnIter) == TRUE;                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_EventSPNIterNext(&lSpnIter))                                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_DidIndexType lDidIndex;
      lDidIndex = Dem_Cfg_EventSPNIterGet(&lSpnIter);                                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_ExtDataElement_CollectStoredDid(&lCollectDidInfo, lDidIndex);                                                          /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_CollectGlobalSnapshot
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectGlobalSnapshot(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  Dem_ExtDataElement_CollectDidInfoType lCollectDidInfo;
  Dem_Cfg_GlobalSnapshotDidIterType lDidIter;
  Dem_Data_DestinationBuffer lDestinationBuffer;

  /* Initialize collection context */
  Dem_Data_InitDestinationBuffer(&lDestinationBuffer, DestinationBuffer, BufferSize);                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  Dem_ExtDataElement_InitCollectDidInfo(&lCollectDidInfo                                                                         /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */
                                         ,&lDestinationBuffer
                                         ,EventId);

  for (Dem_Cfg_GlobalSnapshotDidIterInit(&lDidIter);                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_GlobalSnapshotDidIterExists(&lDidIter) == TRUE;                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_GlobalSnapshotDidIterNext(&lDidIter))                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_DidIndexType lDidIndex;
    lDidIndex = Dem_Cfg_GlobalSnapshotDidIterGet(&lDidIter);                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_ExtDataElement_CollectStoredDid(&lCollectDidInfo, lDidIndex);                                                            /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO */
  }
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_CollectSnapshot
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectSnapshot(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_SharedDataPtrType, AUTOMATIC)  DestinationBuffer,
  CONST(uint16, AUTOMATIC)  BufferSize
  )
{
  Dem_ExtDataElement_CollectDidInfoType lCollectDidInfo;
  Dem_Cfg_EventDidIterType lDidIter;
  Dem_Data_DestinationBuffer lDestinationBuffer;

  /* Initialize collection context */
  Dem_Data_InitDestinationBuffer(&lDestinationBuffer, DestinationBuffer, BufferSize);                                            /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */
  Dem_ExtDataElement_InitCollectDidInfo(&lCollectDidInfo                                                                         /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */
                                         ,&lDestinationBuffer
                                         ,EventId);

  for (Dem_Cfg_EventDidIterInit(EventId, &lDidIter);                                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventDidIterExists(&lDidIter) == TRUE;                                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_EventDidIterNext(&lDidIter))                                                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_DidIndexType lDidIndex;
    lDidIndex = Dem_Cfg_EventDidIterGet(&lDidIter);                                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_ExtDataElement_CollectStoredDid(&lCollectDidInfo, lDidIndex);                                                            /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO */
  }
}
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_CollectERec
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectERec(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_DidIndexType,AUTOMATIC) DidIndex,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
  )
{
  Dem_ExtDataElement_CollectDidInfoType lCollectDidInfo;

  Dem_ExtDataElement_InitCollectDidInfo(&lCollectDidInfo                                                                         /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */
                                         ,DestinationBuffer
                                         ,EventId);

  Dem_ExtDataElement_CollectStoredDid(&lCollectDidInfo, DidIndex);                                                               /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO */
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_DataStorage_CollectTimeSeriesSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_DataStorage_CollectTimeSeriesSamples(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  Dem_Data_DestinationBufferPtrType DestinationBuffer                                                                            /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  Dem_Cfg_ComplexIterType lDidIter;
  Dem_ExtDataElement_CollectDidInfoType lCollectDidInfo;

  Dem_ExtDataElement_InitCollectDidInfo(&lCollectDidInfo                                                                         /* SBSW_DEM_CALL_COLLECTDIDINFO_INIT */
                                         , DestinationBuffer
                                         , DEM_EVENT_INVALID);

  for (Dem_StorageManager_DataStorage_TimeSeriesDidIterInit(&lDidIter, SamplingProfileId);                                       /* SBSW_DEM_POINTER_FORWARD_STACK */
       Dem_Cfg_ComplexIterExists(&lDidIter) == TRUE;                                                                             /* SBSW_DEM_POINTER_FORWARD_STACK */
       Dem_Cfg_ComplexIterNext(&lDidIter))                                                                                       /* SBSW_DEM_POINTER_FORWARD_STACK */
  {
    Dem_Cfg_DidIndexType lDidIndex = Dem_StorageManager_DataStorage_TimeSeriesDidIterGet(&lDidIter);                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_ExtDataElement_CollectStoredDid(&lCollectDidInfo, lDidIndex);                                                            /* SBSW_DEM_POINTER_LOCAL_COLLECTDIDINFO */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_DATASTORAGE_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_DataStorage_Implementation.h
 *********************************************************************************************************************/
