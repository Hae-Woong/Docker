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
/*! \addtogroup Dem_RingBuffer
 *  \{
 *  \file       Dem_RingBuffer_Implementation.h
 *  \brief      Manages ring buffer.
 *  \details    Manages access to ring buffers and ring buffer state handling.
 *  \entity     RingBuffer
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_RINGBUFFER_IMPLEMENTATION_H)
#define DEM_RINGBUFFER_IMPLEMENTATION_H

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

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_RingBuffer_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FaultMemory_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_RINGBUFFER_IMPLEMENTATION_FILENAME "Dem_RingBuffer_Implementation.h"

 /* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_RingBuffer_PrivateProperties Private Properties
 * \{
 */
 
/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_GetBufferCount
 *****************************************************************************/
/*!
 * \brief         Get the number of buffers for the specified sampling rate.
 * 
 * \details       Get the number of buffers for the specified sampling rate.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \return        Number of buffers
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_GetBufferCount(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_SetSampleCount
 *****************************************************************************/
/*!
 * \brief         Set number of stored samples in the ring buffer of the
 *                given sampling rate
 *
 * \details       Set number of stored samples in the ring buffer of the
 *                given sampling rate
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * \param[in]     SampleCount
 *                Sample count to write for the resp. ring buffer
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_SetSampleCount(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 SampleCount
  );

 /* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_SetCurrentIndex
 *****************************************************************************/
/*!
 * \brief         Set the index of the current sample for the ring buffer
 *                of the given sampling rate
 *
 * \details       Set the index of the current sample for the ring buffer
 *                of the given sampling rate
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * \param[in]     Index
 *                The index to which the current sample index of the resp.
 *                ring buffer is set to
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_SetCurrentIndex(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 Index
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_GetBufferCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_GetBufferCount(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  )
{
  uint8 lBufferCount = 1u;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if (Dem_GlobalDiagnostics_RingBuffer_ArePastSamplesSupported(SamplingProfileId) == TRUE)
  {
    lBufferCount = Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(SamplingProfileId);
  }
#endif
  return lBufferCount;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_SetCurrentIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_SetCurrentIndex(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 Index
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if(SamplingProfileId >= Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable())
  {
    Dem_Error_RunTimeCheckFailed(DEM_RINGBUFFER_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_RingBuffer.CurrentIndex[SamplingProfileId] = Index;                                                                  /* SBSW_DEM_ARRAY_WRITE_RINGBUFFERINDICES */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Index)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

 /* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_SetSampleCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
 DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
 Dem_GlobalDiagnostics_RingBuffer_SetSampleCount(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
   Dem_Cfg_SamplingProfileIterType SamplingProfileId,
   uint8 SampleCount
 )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if(SamplingProfileId >= Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable())
  {
    Dem_Error_RunTimeCheckFailed(DEM_RINGBUFFER_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_RingBuffer.SampleCount[SamplingProfileId] = SampleCount;                                                             /* SBSW_DEM_ARRAY_WRITE_RINGBUFFERINDICES */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SampleCount)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
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
 * \addtogroup Dem_RingBuffer_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_GetSampleCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_GetSampleCount(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  return Dem_Cfg_RingBuffer.SampleCount[SamplingProfileId];
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return 0u;
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_GetCurrentIndex
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_GetCurrentIndex(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  return Dem_Cfg_RingBuffer.CurrentIndex[SamplingProfileId];                                                                     /* PRQA S 2842 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return 0U;
#endif
}

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
 * \defgroup Dem_RingBuffer_Private Private Methods
 * \{
 */

/*********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

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
 * \addtogroup Dem_RingBuffer_Public
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_GetRingBufferSlot
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_GetRingBufferSlot(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8  BufferIndex
)
{
  Dem_DataPtrType lReturnValue;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((SamplingProfileId >= Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable())
    || (BufferIndex >= Dem_GlobalDiagnostics_RingBuffer_GetBufferCount(SamplingProfileId)))
  {
    Dem_Error_RunTimeCheckFailed(DEM_RINGBUFFER_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_Cfg_GetRingBufferPtrOfTimeSeriesSamplingProfileTable(0)[0]);
  }
  else
#endif
  {
    uint32 lBufferSize = Dem_Cfg_GetSampleSizeOfTimeSeriesSamplingProfileTable(SamplingProfileId);
    lReturnValue = &Dem_Cfg_GetRingBufferPtrOfTimeSeriesSamplingProfileTable(SamplingProfileId)[lBufferSize * BufferIndex];
  }

  DEM_IGNORE_UNUSED_ARGUMENT(SamplingProfileId);                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lReturnValue;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_ReadSlotChronological
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
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_ReadSlotChronological(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 DataIndexChrono
)
{
  uint8 lActualIndex = DataIndexChrono;
  if (Dem_GlobalDiagnostics_RingBuffer_ArePastSamplesSupported(SamplingProfileId) == TRUE)
  {
    uint8 lConfiguredSampleCount = Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(SamplingProfileId);
    uint8 lCurrentIndex = Dem_GlobalDiagnostics_RingBuffer_GetCurrentIndex(SamplingProfileId);
    if (Dem_GlobalDiagnostics_RingBuffer_GetSampleCount(SamplingProfileId) >= lConfiguredSampleCount)
    {
      lActualIndex = (lCurrentIndex + lActualIndex) % lConfiguredSampleCount;
    }
  }
  return Dem_GlobalDiagnostics_RingBuffer_GetRingBufferSlot(SamplingProfileId, lActualIndex);
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_Rotate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_Rotate(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  if (Dem_GlobalDiagnostics_RingBuffer_ArePastSamplesSupported(SamplingProfileId) == TRUE)
  {
    /* update current index of ring buffer */
    uint8 lConfiguredSampleCount = Dem_GlobalDiagnostics_RingBuffer_GetBufferCount(SamplingProfileId);
    uint8 lNewIndex = ((Dem_GlobalDiagnostics_RingBuffer_GetCurrentIndex(SamplingProfileId) + 1u) % lConfiguredSampleCount);
    Dem_GlobalDiagnostics_RingBuffer_SetCurrentIndex(SamplingProfileId, lNewIndex);

    /* update number of written samples in ring buffer */
    if (Dem_GlobalDiagnostics_RingBuffer_GetSampleCount(SamplingProfileId) < lConfiguredSampleCount)
    {
      Dem_GlobalDiagnostics_RingBuffer_SetSampleCount(SamplingProfileId,
        Dem_GlobalDiagnostics_RingBuffer_GetSampleCount(SamplingProfileId) + 1u);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_Init(
  void
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId;
  for(lSamplingProfileId = 0; lSamplingProfileId < Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable(); lSamplingProfileId++)
  {
    Dem_GlobalDiagnostics_RingBuffer_SetSampleCount(lSamplingProfileId, 0);
    Dem_GlobalDiagnostics_RingBuffer_SetCurrentIndex(lSamplingProfileId, 0);
  }
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_RingBuffer_ArePastSamplesSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_ArePastSamplesSupported(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId);
#if (DEM_CFG_SUPPORT_TIME_SERIES_PAST == STD_ON)
  return (boolean) (0u < Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(SamplingProfileId));
#else
  return FALSE;
#endif
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_RINGBUFFER_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_RingBuffer_Implementation.h
 *********************************************************************************************************************/
