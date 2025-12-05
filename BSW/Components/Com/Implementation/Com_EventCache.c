/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Com_EventCache.c
 *         \unit  EventCache
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_EVENTCACHE_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_EventCache.h"
#include "Com_Cfg_EventCache.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#include "Com.h"
#include "Com_Reporting.h"
/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Com_EventCache_GetEventCacheArrayReadIdxSafe
 *********************************************************************************************************************/
/*! \brief       Get the current read index for the queue with additional optional error checks
 *  \details     -
 *  \param[in]   eventCacheIdx    Index of the event cache
 *  \pre         -
 *  \return      Com_EventCacheArrayReadIdxOfEventCacheType    Read index of the event cache array
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_EventCacheArrayReadIdxOfEventCacheType, COM_CODE) Com_EventCache_GetEventCacheArrayReadIdxSafe(Com_CEventCacheIterType eventCacheIdx);

/**********************************************************************************************************************
 * Com_EventCache_GetEventCacheArrayWriteIdxSafe
 *********************************************************************************************************************/
/*! \brief       Get the current write index for the queue with additional optional error checks
 *  \details     -
 *  \param[in]   eventCacheIdx    Index of the event cache
 *  \pre         -
 *  \return      Com_EventCacheArrayWriteIdxOfEventCacheType    Write index of the event cache array
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_EventCacheArrayWriteIdxOfEventCacheType, COM_CODE) Com_EventCache_GetEventCacheArrayWriteIdxSafe(Com_CEventCacheIterType eventCacheIdx);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
 * Com_EventCache_GetEventCacheArrayReadIdxSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_EventCacheArrayReadIdxOfEventCacheType, COM_CODE) Com_EventCache_GetEventCacheArrayReadIdxSafe(Com_CEventCacheIterType eventCacheIdx)
{
  /* Get the read index from the RAM array */
  Com_EventCacheArrayReadIdxOfEventCacheType readIdx = Com_GetEventCacheArrayReadIdxOfEventCache(eventCacheIdx);

  /* Get the appropriate start and end index of the buffer from the ROM array */
  const Com_EventCacheArrayStartIdxOfEventCacheType startIdxOfBuffer = Com_GetEventCacheArrayStartIdxOfEventCache(eventCacheIdx);
  const Com_EventCacheArrayEndIdxOfEventCacheType endOutOfBoundsIdxOfBuffer = Com_GetEventCacheArrayEndIdxOfEventCache(eventCacheIdx);

  if(readIdx >= endOutOfBoundsIdxOfBuffer) /* check includes the endIdxOfBuffer (out-of-bounds index)*/
  {
    readIdx = endOutOfBoundsIdxOfBuffer - 1u; /* set to actual endIdx */
    Com_Reporting_CallDetReportError(COMServiceId_EventCache_ReadIdx, COM_E_IDXOUTOFBOUNDS); /* COV_COM_REPORT_ERROR */
  }
  if(readIdx < startIdxOfBuffer)
  {
    readIdx = startIdxOfBuffer;
    Com_Reporting_CallDetReportError(COMServiceId_EventCache_ReadIdx, COM_E_IDXOUTOFBOUNDS); /* COV_COM_REPORT_ERROR */
  }

  return readIdx;
}

/**********************************************************************************************************************
 * Com_EventCache_GetEventCacheArrayWriteIdxSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_EventCacheArrayWriteIdxOfEventCacheType, COM_CODE) Com_EventCache_GetEventCacheArrayWriteIdxSafe(Com_CEventCacheIterType eventCacheIdx)
{
  /* Get write index from the RAM array */
  Com_EventCacheArrayWriteIdxOfEventCacheType writeIdx = Com_GetEventCacheArrayWriteIdxOfEventCache(eventCacheIdx);

  /* Get the appropriate start and end index of the buffer from the ROM array */
  const Com_EventCacheArrayStartIdxOfEventCacheType startIdxOfBuffer = Com_GetEventCacheArrayStartIdxOfEventCache(eventCacheIdx);
  const Com_EventCacheArrayEndIdxOfEventCacheType endOutOfBoundsIdxOfBuffer = Com_GetEventCacheArrayEndIdxOfEventCache(eventCacheIdx);

  if(writeIdx >= endOutOfBoundsIdxOfBuffer) /* check includes the endIdxOfBuffer (out-of-bounds index)*/
  {
    writeIdx = endOutOfBoundsIdxOfBuffer - 1u; /* set to actual endIdx */
    Com_Reporting_CallDetReportError(COMServiceId_EventCache_WriteIdx, COM_E_IDXOUTOFBOUNDS); /* COV_COM_REPORT_ERROR */
  }
  if(writeIdx < startIdxOfBuffer)
  {
    writeIdx = startIdxOfBuffer;
    Com_Reporting_CallDetReportError(COMServiceId_EventCache_WriteIdx, COM_E_IDXOUTOFBOUNDS); /* COV_COM_REPORT_ERROR */
  }

  return writeIdx;
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_EventCache_Init
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_EventCache_Init(void)
{
  for (Com_CEventCacheIterType cEventCacheIdx = 0u; cEventCacheIdx < Com_GetSizeOfCEventCache();  cEventCacheIdx++) /* FETA_COM_01 */
  {
    const Com_EventCacheArrayStartIdxOfEventCacheType startIdxOfBuffer = Com_GetEventCacheArrayStartIdxOfEventCache(cEventCacheIdx);

    Com_SetEventCacheArrayReadIdxOfEventCache(cEventCacheIdx, startIdxOfBuffer);
    Com_SetEventCacheArrayWriteIdxOfEventCache(cEventCacheIdx, startIdxOfBuffer);
  }
}

/**********************************************************************************************************************
Com_EventCache_Put
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_EventCache_Put(Com_CEventCacheIterType eventCacheIdx, Com_RxPduInfoIterType rxPduInfoIdx)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(Com_EventCache_IsFull(eventCacheIdx)) /* Full */
  {
    retVal = E_NOT_OK;
  }
  else /* writeIdx < readIdx (write is possible until read index is reached) OR readIdx < writeIdx OR readIdx == writeIdx */
  {
    const Com_EventCacheArrayWriteIdxOfEventCacheType writeIdx = Com_EventCache_GetEventCacheArrayWriteIdxSafe(eventCacheIdx);
    const Com_EventCacheArrayStartIdxOfEventCacheType startIdxOfBuffer = Com_GetEventCacheArrayStartIdxOfEventCache(eventCacheIdx);
    const Com_EventCacheArrayEndIdxOfEventCacheType endIdxOfBuffer = Com_GetEventCacheArrayEndIdxOfEventCache(eventCacheIdx) - 1u; /* Actual end index (not ComStackLib out-of-bounds index) */

    Com_SetRxPduInfoIdxOfEventCacheArray(writeIdx, (Com_RxPduInfoIdxOfEventCacheArrayType) rxPduInfoIdx);
    if(writeIdx == endIdxOfBuffer)
    {
      Com_SetEventCacheArrayWriteIdxOfEventCache(eventCacheIdx, startIdxOfBuffer); /* wrap-around */
    }
    else
    {
      Com_SetEventCacheArrayWriteIdxOfEventCache(eventCacheIdx, writeIdx + 1u);
    }
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
  Com_EventCache_Get
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_EventCache_Get(Com_CEventCacheIterType eventCacheIdx, CONSTP2VAR(Com_RxPduInfoIterType, AUTOMATIC, AUTOMATIC) rxPduInfoIdxPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

  const Com_EventCacheArrayReadIdxOfEventCacheType readIdx = Com_EventCache_GetEventCacheArrayReadIdxSafe(eventCacheIdx);
  const Com_EventCacheArrayWriteIdxOfEventCacheType writeIdx = Com_EventCache_GetEventCacheArrayWriteIdxSafe(eventCacheIdx);

  if(readIdx == writeIdx) /* empty */
  {
    retVal = E_NOT_OK;
  }
  else
  {
    /* read rxPduInfoIdx from Cache */
    const Com_RxPduInfoIterType localRxPduInfoIdx = Com_GetRxPduInfoIdxOfEventCacheArray(readIdx);

    if( (localRxPduInfoIdx < Com_GetSizeOfRxPduInfo()) && (!Com_IsInvalidHndOfRxPduInfo(localRxPduInfoIdx)) ) /* check if RxPduInfo is within bounds and handle is valid */
    {
      *rxPduInfoIdxPtr = localRxPduInfoIdx;
      retVal = E_OK;
    }

    /* Increase read index to next element.*/
    const Com_EventCacheArrayStartIdxOfEventCacheType startIdxOfBuffer = Com_GetEventCacheArrayStartIdxOfEventCache(eventCacheIdx);
    const Com_EventCacheArrayEndIdxOfEventCacheType endIdxOfBuffer = Com_GetEventCacheArrayEndIdxOfEventCache(eventCacheIdx) - 1u; /* Actual end index (not ComStackLib out-of-bounds index) */

    if(readIdx == endIdxOfBuffer) /* wrap-around */
    {
      Com_SetEventCacheArrayReadIdxOfEventCache(eventCacheIdx, startIdxOfBuffer);
    }
    else
    {
      Com_SetEventCacheArrayReadIdxOfEventCache(eventCacheIdx, readIdx + 1u);
    }
  }

  return retVal;
}

/**********************************************************************************************************************
  Com_EventCache_IsFull
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_EventCache_IsFull(Com_CEventCacheIterType eventCacheIdx)
{
  boolean retVal = TRUE; /* always assume full as standard */

  const Com_EventCacheArrayReadIdxOfEventCacheType readIdx = Com_EventCache_GetEventCacheArrayReadIdxSafe(eventCacheIdx);
  const Com_EventCacheArrayWriteIdxOfEventCacheType writeIdx = Com_EventCache_GetEventCacheArrayWriteIdxSafe(eventCacheIdx);

  const Com_EventCacheArrayStartIdxOfEventCacheType startIdxOfBuffer = Com_GetEventCacheArrayStartIdxOfEventCache(eventCacheIdx);
  const Com_EventCacheArrayEndIdxOfEventCacheType endIdxOfBuffer = Com_GetEventCacheArrayEndIdxOfEventCache(eventCacheIdx) - 1u; /* Actual end index (not ComStackLib out-of-bounds index) */ /* PRQA S 2983 */ /* MD_Com_2982_2983 */

  /* Full condition: writeIdx directly in front of readIdx (also valid for wrap-around case) */
  if( ((writeIdx + 1u) == readIdx) || ((readIdx == startIdxOfBuffer) && (writeIdx == endIdxOfBuffer)) )
  {
    retVal = TRUE; /* Full */
  }
  else
  {
    retVal = FALSE;
  }

  return retVal;
}

/**********************************************************************************************************************
  Com_EventCache_Flush
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_EventCache_Flush(Com_CEventCacheIterType eventCacheIdx)
{
  Com_SetEventCacheArrayReadIdxOfEventCache(eventCacheIdx, Com_EventCache_GetEventCacheArrayWriteIdxSafe(eventCacheIdx));
} /* COV_COM_EVENT_CACHE_FLUSH */

/**********************************************************************************************************************
  Com_EventCache_GetReadLimit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_EventCacheArrayLengthOfEventCacheType, COM_CODE) Com_EventCache_GetReadLimit(Com_CEventCacheIterType eventCacheIdx)
{
  return Com_GetEventCacheArrayLengthOfEventCache(eventCacheIdx);
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_Caching.c
 *********************************************************************************************************************/
