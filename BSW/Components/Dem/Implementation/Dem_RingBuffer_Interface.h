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
 *  \defgroup   Dem_RingBuffer RingBuffer
 *  \{
 *  \file       Dem_RingBuffer_Interface.h
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

#if !defined (DEM_RINGBUFFER_INTERFACE_H)
#define DEM_RINGBUFFER_INTERFACE_H

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
#include "Dem_RingBuffer_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_RingBuffer_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_GetSampleCount
 *****************************************************************************/
/*!
 * \brief         Get the number of samples currently stored in the ring
 *                buffer.
 *
 * \details       Get the number of samples currently stored in the ring
 *                buffer for the specified sampling profile
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \return        Sample count in the respective buffer.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_GetSampleCount(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_GetCurrentIndex
 *****************************************************************************/
 /*!
  * \brief         Return index to current sample slot in ring buffer
  *
  * \details       Return index to current sample slot in ring buffer for given
  *                sampling rate
  *
  * \param[in]     SamplingProfileId
  *                Specifies a sampling profile
  *
  * \return        Index to current sample slot in ring buffer
  *
  * \pre           -
  *
  * \context       TASK|ISR2
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_GetCurrentIndex(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_RingBuffer_Public Public Methods
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_GetRingBufferSlot
 *****************************************************************************/
/*!
 * \brief         Returns pointer to the indexed ringbuffer slot from time
 *                series DIDs.
 *
 * \details       Checks if passed sampling profile Id and buffer index is
 *                valid and returns pointer to the requested indexed ringbuffer
 *                slot from time series DIDs
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * \param[in]     BufferIndex
 *                Index of the buffer
 *
 * \return        Always returns a valid pointer. If the passed sampling profile Id
 *                or buffer index is invalid, a pointer to the first index of the
 *                first ring buffer is returned (if time series feature is enabled,
 *                at least one ringbuffer must exist).
 *
 * \pre           Time series feature must be enabled
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_GetRingBufferSlot(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8  BufferIndex
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_ReadSlotChronological
 *****************************************************************************/
/*!
 * \brief         Returns ring buffer entry at the requested index for the
 *                specified sampling rate.
 *
 * \details       Returns ring buffer entry at the requested index. The index is
 *                in chronological order ranging from [0..number of stored samples[.
 *                Thus, index 0 referes to the oldest element currently saved in
 *                the ring buffer for the given sampling rate.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * \param[in]     DataIndexChrono
 *                Index of the entry to read. The index is in chronological order
 *                with 0 being the oldest element. The index must be in range
 *                [0..Dem_GlobalDiagnostics_RingBuffer_GetSampleCount(SamplingProfileId)[
 *
 * \return        Pointer to the buffer
 *
 * \pre           Time series feature must be enabled for a return value other
 *                than NULL_PTR.
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_ReadSlotChronological(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 DataIndexChrono
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_Rotate
 *****************************************************************************/
/*!
 * \brief         Shift ring buffer internally to next slot.
 *
 * \details       Shift ring buffer internally to next slot, if time series
 *                past samples are configured for given sampling profile.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_Rotate(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  );
#endif

/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_Init
 *****************************************************************************/
/*!
 * \brief         Initializes time series ring buffer
 *
 * \details       Initializes time series ring buffer
 *
 * \pre           NvM must have restored NV mirrors
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_Init(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_RingBuffer_ArePastSamplesSupported
 *****************************************************************************/
/*!
 * \brief         Returns whether past samples are supported for the given
 *                sampling rate
 * 
 * \details       Returns whether past samples are supported for the given
 *                sampling rate
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \return        TRUE
 *                Past samples are supported
 * \return        FALSE
 *                Past samples are not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_RingBuffer_ArePastSamplesSupported(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_RINGBUFFER_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_RingBuffer_Interface.h
 *********************************************************************************************************************/
