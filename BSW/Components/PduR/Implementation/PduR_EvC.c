/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  PduR_EvC.c
 *         \unit  EvC
 *        \brief  Pdu Router Event Cache source file
 *      \details  This is the implementation of the MICROSAR Pdu Router module.
 *                The basic software module is based on the AUTOSAR PduR specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 3679 EOF */ /* MD_MSR_Rule8.13 */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_EVC_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_EvC.h"
#include "SchM_PduR.h"
#include "PduR_Reporting.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_Put
 *********************************************************************************************************************/
/*! \brief       Write Id into the event queue.
 *  \details     -
 *  \param[in]   deferredEventCacheIdx    Id of the deferred event cache.
 *  \param[in]   rmGDestRomIdx            Index to the global destination Pdu to be queued
 *  \param[in]   memIdx                   memory section index
 *  \return      E_OK                     Write to the event cache was successful.
 *               E_NOT_OK                 Write to the event cache was not successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_EvC_Put(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_RmGDestRomIterType rmGDestRomIdx, PduR_MemIdxType memIdx);
#endif

# if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_Get
 *********************************************************************************************************************/
/*! \brief       Read the next rmGDestRomIdx from the event queue, if available
 *  \details     -
 *  \param[in]   deferredEventCacheIdx    Id of the deferred event cache.
 *  \param[out]  rmGDestRomIdxPtr         the read rmGDestRomIdx - Caller must ensure validity of the pointer.
 *  \param[in]   memIdx                   memory section index
 *  \return      E_OK                     Read was successful.
 *               E_NOT_OK                 Read was not successful. Queue is empty.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_EvC_Get(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, P2VAR(PduR_RmGDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmGDestRomIdxPtr, PduR_MemIdxType memIdx);
# endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_ReadFlush
 *********************************************************************************************************************/
/*! \brief       Flush the event queue by setting the read pointer equal to the write pointer
 *  \details     -
 *  \param[in]   deferredEventCacheIdx    Id of the deferred event cache.
 *  \param[in]   memIdx                   memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        This function must only be called by the reader core!
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_EvC_ReadFlush(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_GetDeferredEventCacheArrayRamReadIdxSafe
 *********************************************************************************************************************/
/*! \brief       Gets the current read idx for the queue with optional error checking.
 *  \details     -
 *  \param[in]   deferredEventCacheIdx    Id of the deferred event cache.
 *  \param[in]   memIdx                   memory section index
 *  \return      the read index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_DeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRamType, PDUR_CODE) PduR_EvC_GetDeferredEventCacheArrayRamReadIdxSafe(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_GetDeferredEventCacheArrayRamWriteIdxSafe
 *********************************************************************************************************************/
/*! \brief       Gets the current write idx for the queue with optional error checking.
 *  \details     -
 *  \param[in]   deferredEventCacheIdx    Id of the deferred event cache.
 *  \param[in]   memIdx                   memory section index
 *  \return      the write index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_DeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRamType, PDUR_CODE) PduR_EvC_GetDeferredEventCacheArrayRamWriteIdxSafe(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_IsFull
 *********************************************************************************************************************/
/*! \brief       Checks if the queue is full.
 *  \details -
 *  \param[in]   deferredEventCacheIdx    Id of the deferred event cache.
 *  \param[in]   memIdx                   memory section index
 *  \return      true                     Event cache is full.
 *               false                    Event cache is not full.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_EvC_IsFull(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_MemIdxType memIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_IsAnyEvCOverflowed
 *********************************************************************************************************************/
/*! \brief       Checks if any EvC for the destination application is overflowed.
 *  \details -
 *  \param[in]   destApplicationManagerRomIdx    Id of the destination application
 *  \return      true                     Any EvC is overflowed.
 *               false                    No EvC is overflowed.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_EvC_IsAnyEvCOverflowed(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_FlushAllEvCOfDestApplication
 *********************************************************************************************************************/
/*! \brief       Flushes all EvCs of the destination application.
 *  \details     -
 *  \param[in]   destApplicationManagerRomIdx    Id of the destination application
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_EvC_FlushAllEvCOfDestApplication(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_ReadAndProcessEvCOfDestApplication
 *********************************************************************************************************************/
/*! \brief       Read and process the event cache of this destination application
 *  \details     -
 *  \param[in]   destApplicationManagerRomIdx    Id of the destination application
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_EvC_ReadAndProcessEvCOfDestApplication(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_IsRmGDestIdxValidOnApplication
 *********************************************************************************************************************/
/*! \brief       Checks if this global destination handle is assigned to the provided application.
 *  \details -
 *  \param[in]   rmGDestRomIdx    Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]   applicationId    Valid DestApplicationManagerRom table index. Internal handle of a OsApplication.
 *  \return      true             rmGDestRomIdx is assigned to applicationId
 *               false            rmGDestRomIdx is not assigned to applicationId
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_EvC_IsRmGDestIdxValidOnApplication(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationId);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_EvC_Init(PduR_MemIdxType memIdx)
{
  PduR_DeferredEventCacheRomIterType deferredEventCacheRomIdx;
  for(deferredEventCacheRomIdx = 0u; deferredEventCacheRomIdx < PduR_GetSizeOfDeferredEventCacheRom(memIdx); deferredEventCacheRomIdx++)  /* FETA_PDUR_01 */
  {
    PduR_DeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRomType startIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRom(deferredEventCacheRomIdx, memIdx);

    PduR_SetDeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRam(deferredEventCacheRomIdx, startIdxOfBuffer, memIdx);
    PduR_SetDeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRam(deferredEventCacheRomIdx, startIdxOfBuffer, memIdx);
  }
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_Put
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_EvC_Put(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_RmGDestRomIterType rmGDestRomIdx, PduR_MemIdxType memIdx)
{
  Std_ReturnType retVal = E_NOT_OK;

  SchM_Enter_PduR_PDUR_EXCLUSIVE_AREA_0();
  {
    PduR_DeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRamType readIdx = PduR_EvC_GetDeferredEventCacheArrayRamReadIdxSafe(deferredEventCacheIdx, memIdx);
    PduR_DeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRamType writeIdx = PduR_EvC_GetDeferredEventCacheArrayRamWriteIdxSafe(deferredEventCacheIdx, memIdx);

    {
      PduR_DeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRomType startIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx);
      PduR_DeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRomType endIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx) - 1u;    /* ComStackLibs EndIdx is actually the first out of bounds index */

      /* If event cache is full, return E_NOT_OK */
      if(PduR_EvC_IsFull(deferredEventCacheIdx, memIdx) == TRUE)
      {
        /* Buffer is full */
        retVal = E_NOT_OK;
      }
      /* else if write index < read index, put rmGDestRomIdx into cache and increase write idx */
      else if(writeIdx < readIdx)       /* write is possible until read Idx is reached. Could be too small though */
      {
        PduR_SetRmGDestRomIdxOfDeferredEventCacheArrayRam(writeIdx, (PduR_RmGDestRomIdxOfDeferredEventCacheArrayRamType) rmGDestRomIdx, memIdx);
        PduR_SetDeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRam(deferredEventCacheIdx, writeIdx + 1u, memIdx);
        retVal = E_OK;
      }
      /* else */
      else
      { /* readIdx < writeIdx OR readIdx == writeIdx */
        /* put rmGDestRomIdx into cache */
        PduR_SetRmGDestRomIdxOfDeferredEventCacheArrayRam(writeIdx, (PduR_RmGDestRomIdxOfDeferredEventCacheArrayRamType) rmGDestRomIdx, memIdx);

        /* Wrap around write index if required or increase write index otherwise */
        if(writeIdx == endIdxOfBuffer)
        {
          PduR_SetDeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRam(deferredEventCacheIdx, startIdxOfBuffer, memIdx);
        }
        else
        {
          PduR_SetDeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRam(deferredEventCacheIdx, writeIdx + 1u, memIdx);
        }
        retVal = E_OK;
      }
    }
  }
  SchM_Exit_PduR_PDUR_EXCLUSIVE_AREA_0();

  return retVal;
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_WriteDeferredEventCache
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_EvC_WriteDeferredEventCache(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext)
{
  /* Put destination handle into the deferred event queue */
  PduR_DestApplicationManagerRomIterType destApplicationIdx = PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_SrcApplicationRomIterType dest2srcApplicationRomIdx = PduR_GetSrcApplicationRomStartIdxOfDestApplicationManagerRom(destApplicationIdx) + (PduR_SrcApplicationRomIterType) applicationContext;

  return PduR_EvC_Put(PduR_GetDeferredEventCacheRomIdxOfSrcApplicationRom(dest2srcApplicationRomIdx), rmGDestRomIdx, PduR_GetDeferredEventCacheRomPartitionIdxOfSrcApplicationRom(dest2srcApplicationRomIdx));
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_Get
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_EvC_Get(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, P2VAR(PduR_RmGDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmGDestRomIdxPtr, PduR_MemIdxType memIdx)
{
  Std_ReturnType retVal = E_NOT_OK;
  SchM_Enter_PduR_PDUR_EXCLUSIVE_AREA_0();
  {
    PduR_DeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRamType readIdx = PduR_EvC_GetDeferredEventCacheArrayRamReadIdxSafe(deferredEventCacheIdx, memIdx);
    PduR_DeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRamType writeIdx = PduR_EvC_GetDeferredEventCacheArrayRamWriteIdxSafe(deferredEventCacheIdx, memIdx);

    /* If buffer is empty, return E_NOT_OK */
    if(readIdx == writeIdx)
    {
      retVal = E_NOT_OK;
    }
    /* Otherwise */
    else
    {
      PduR_DeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRomType startIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx);
      PduR_DeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRomType endIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx) - 1u;    /* ComStackLibs EndIdx is actually the first out of bounds index */

      /* Read RmGDestRomIdx from Cache */
      PduR_RmGDestRomIterType localReadRmGDestRomIdx = PduR_GetRmGDestRomIdxOfDeferredEventCacheArrayRam(readIdx, memIdx);

      if(localReadRmGDestRomIdx < PduR_GetSizeOfRmGDestRom())
      {
        *rmGDestRomIdxPtr = localReadRmGDestRomIdx;
        retVal = E_OK;
      }

      /* Increase read index to next element. Wrap around if required. */
      if(readIdx == endIdxOfBuffer)
      {
        PduR_SetDeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRam(deferredEventCacheIdx, startIdxOfBuffer, memIdx);
      }
      else
      {
        PduR_SetDeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRam(deferredEventCacheIdx, readIdx + 1u, memIdx);
      }
    }
  }
  SchM_Exit_PduR_PDUR_EXCLUSIVE_AREA_0();
  /*@ assert retVal != E_OK || *rmGDestRomIdxPtr < PduR_GetSizeOfRmGDestRom(); */
  return retVal;
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_ReadFlush
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_EvC_ReadFlush(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_MemIdxType memIdx)
{
  PduR_SetDeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRam(deferredEventCacheIdx, PduR_EvC_GetDeferredEventCacheArrayRamWriteIdxSafe(deferredEventCacheIdx, memIdx), memIdx);
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_GetDeferredEventCacheArrayRamReadIdxSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_DeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRamType, PDUR_CODE) PduR_EvC_GetDeferredEventCacheArrayRamReadIdxSafe(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_MemIdxType memIdx)
{
  PduR_DeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRamType readIdx = PduR_GetDeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRam(deferredEventCacheIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_DeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRomType startIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx);
  PduR_DeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRomType endIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx);

  if(readIdx >= endIdxOfBuffer)
  {
    readIdx = endIdxOfBuffer - 1u;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETEVCREADIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
  if(readIdx < startIdxOfBuffer)
  {
    readIdx = startIdxOfBuffer;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETEVCREADIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif
/*@ assert (readIdx  < PduR_GetSizeOfDeferredEventCacheArrayRam(memIdx)); */
  return readIdx;
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_GetDeferredEventCacheArrayRamWriteIdxSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_DeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRamType, PDUR_CODE) PduR_EvC_GetDeferredEventCacheArrayRamWriteIdxSafe(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_MemIdxType memIdx)
{
  PduR_DeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRamType writeIdx = PduR_GetDeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRam(deferredEventCacheIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_DeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRomType startIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx);
  PduR_DeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRomType endIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx);

  if(writeIdx >= endIdxOfBuffer)
  {
    writeIdx = endIdxOfBuffer - 1u;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETEVCWRITEIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
  if(writeIdx < startIdxOfBuffer)
  {
    writeIdx = startIdxOfBuffer;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETEVCWRITEIDX, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif
/*@ assert (writeIdx  < PduR_GetSizeOfDeferredEventCacheArrayRam(memIdx)); */
  return writeIdx;
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_IsFull
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_EvC_IsFull(PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheIdx, PduR_MemIdxType memIdx)
{
  boolean retVal;

  SchM_Enter_PduR_PDUR_EXCLUSIVE_AREA_0();
  {
    PduR_DeferredEventCacheArrayRamReadIdxOfDeferredEventCacheRamType readIdx = PduR_EvC_GetDeferredEventCacheArrayRamReadIdxSafe(deferredEventCacheIdx, memIdx);
    PduR_DeferredEventCacheArrayRamWriteIdxOfDeferredEventCacheRamType writeIdx = PduR_EvC_GetDeferredEventCacheArrayRamWriteIdxSafe(deferredEventCacheIdx, memIdx);

    PduR_DeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRomType startIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamStartIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx);
    PduR_DeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRomType endIdxOfBuffer = PduR_GetDeferredEventCacheArrayRamEndIdxOfDeferredEventCacheRom(deferredEventCacheIdx, memIdx) - 1u;      /* ComStackLibs EndIdx is actually the first out of bounds index */

    /* Full condition is, if the readIdx is one smaller than the writeIdx. This is also valid if a wrap-around occured. */
    if(((writeIdx + 1u) == readIdx) || ((readIdx == startIdxOfBuffer) && (writeIdx == endIdxOfBuffer)))
    {
      /* Buffer is full */
      retVal = TRUE;
    }
    else
    {
      retVal = FALSE;
    }
  }
  SchM_Exit_PduR_PDUR_EXCLUSIVE_AREA_0();

  return retVal;
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_IsAnyEvCOverflowed
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_EvC_IsAnyEvCOverflowed(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx)
{
  PduR_SrcApplicationRomEndIdxOfDestApplicationManagerRomType srcApplicationRomIdx;
  boolean anyEventQueueIsOverflowed = FALSE;

  /* Check if any event queue has overflowed */
  for(srcApplicationRomIdx = PduR_GetSrcApplicationRomStartIdxOfDestApplicationManagerRom(destApplicationManagerRomIdx); srcApplicationRomIdx < PduR_GetSrcApplicationRomEndIdxOfDestApplicationManagerRom(destApplicationManagerRomIdx); srcApplicationRomIdx++)  /* FETA_PDUR_01 */
  {
    PduR_MemIdxType memIdx = PduR_GetDeferredEventCacheRomPartitionIdxOfSrcApplicationRom(srcApplicationRomIdx);
    boolean isEvCFull = PduR_EvC_IsFull(PduR_GetDeferredEventCacheRomIdxOfSrcApplicationRom(srcApplicationRomIdx), memIdx);

    if (isEvCFull)
    {
      anyEventQueueIsOverflowed = TRUE;
      break;
    }
  }

  return anyEventQueueIsOverflowed;
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_FlushAllEvCOfDestApplication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_EvC_FlushAllEvCOfDestApplication(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx)
{
  PduR_SrcApplicationRomEndIdxOfDestApplicationManagerRomType srcApplicationRomIdx;

  /* Flush Queues */
  for(srcApplicationRomIdx = PduR_GetSrcApplicationRomStartIdxOfDestApplicationManagerRom(destApplicationManagerRomIdx); srcApplicationRomIdx < PduR_GetSrcApplicationRomEndIdxOfDestApplicationManagerRom(destApplicationManagerRomIdx); srcApplicationRomIdx++)  /* FETA_PDUR_01 */
  {
    PduR_MemIdxType memIdx = PduR_GetDeferredEventCacheRomPartitionIdxOfSrcApplicationRom(srcApplicationRomIdx);

    PduR_EvC_ReadFlush(PduR_GetDeferredEventCacheRomIdxOfSrcApplicationRom(srcApplicationRomIdx), memIdx);
  }
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_ReadAndProcessEvCOfDestApplication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_EvC_ReadAndProcessEvCOfDestApplication(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx)
{
  PduR_SrcApplicationRomEndIdxOfDestApplicationManagerRomType srcApplicationRomIdx;

  for(srcApplicationRomIdx = PduR_GetSrcApplicationRomStartIdxOfDestApplicationManagerRom(destApplicationManagerRomIdx); srcApplicationRomIdx < PduR_GetSrcApplicationRomEndIdxOfDestApplicationManagerRom(destApplicationManagerRomIdx); srcApplicationRomIdx++)  /* FETA_PDUR_01 */
  {
    PduR_RmGDestRomIterType rmGDestRomIdx = 0u;
    PduR_DeferredEventCacheRomIdxOfSrcApplicationRomType deferredEventCacheRomIdx = PduR_GetDeferredEventCacheRomIdxOfSrcApplicationRom(srcApplicationRomIdx);
    PduR_MemIdxType memIdx = PduR_GetDeferredEventCacheRomPartitionIdxOfSrcApplicationRom(srcApplicationRomIdx);
    PduR_DeferredEventCacheArrayRamLengthOfDeferredEventCacheRomType readLimit = PduR_GetDeferredEventCacheArrayRamLengthOfDeferredEventCacheRom(deferredEventCacheRomIdx, memIdx);

    while(PduR_EvC_Get(deferredEventCacheRomIdx, &rmGDestRomIdx, memIdx) == E_OK)       /* FETA_PDUR_02 */
    {
      /* @ assert rmGDestRomIdx < PduR_GetSizeOfRmGDestRom(); */
      if(PduR_EvC_IsRmGDestIdxValidOnApplication(rmGDestRomIdx, destApplicationManagerRomIdx) == TRUE)
      {
        PduR_DispatchMainfuntion(rmGDestRomIdx);
      }
      /* Restrict read iterations if events are written to the cache simultaneously from a different core/partition. */
      readLimit--;
      if(readLimit == 0u)
      {
        break;
      }
    }
  }
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_IsRmGDestIdxValidOnApplication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(boolean, PDUR_CODE) PduR_EvC_IsRmGDestIdxValidOnApplication(PduR_RmGDestRomIterType rmGDestRomIdx, PduR_DestApplicationManagerRomIterType applicationId)
{
  boolean retVal;

  PduR_DestApplicationManagerRomIterType supposedApplicationId = PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(rmGDestRomIdx);

  if(applicationId == supposedApplicationId)
  {
    retVal = TRUE;
  }
  else
  {
    retVal = FALSE;
  }

  return retVal;
}
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_EvC_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_EvC_MainFunction(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx)
{
  /* In case of an overflowed event queue, flush all queues and apply fallback strategy to process all destinations */
  if(PduR_EvC_IsAnyEvCOverflowed(destApplicationManagerRomIdx) == TRUE)
  {
    PduR_EvC_FlushAllEvCOfDestApplication(destApplicationManagerRomIdx);
    PduR_ProcessAllDestinationsOfDestApplication(destApplicationManagerRomIdx);
  }
  /* Otherwise, process deferred event cache and call the corresponding Mainfunction */
  else
  {
    PduR_EvC_ReadAndProcessEvCOfDestApplication(destApplicationManagerRomIdx);
  }
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_EvC.c
 *********************************************************************************************************************/
