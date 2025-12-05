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
 *         \file  PduR_Lock.c
 *         \unit  Lock
 *        \brief  Pdu Router Lock source file
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
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_LOCK_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Lock.h"
#include "SchM_PduR.h"
#include "PduR_Reporting.h"
#if (PDUR_SPINLOCKRAM == STD_ON)
# include "Os.h"
#endif

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

/**********************************************************************************************************************
 * PduR_Lock_Lock
 *********************************************************************************************************************/
/*! \brief      This API locks the corresponding configured lock mechanism.
 *  \details    The lock can either be an exclusive area or a spinlock.
 *              The lock can be "locked" multiple times as long as it happens on the same core. This can be used in recursive calls.
 *  \param[in]  lockIdx  Lock ID
 *  \pre        -
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Lock_Lock(PduR_LockRomIterType lockIdx);

/**********************************************************************************************************************
 * PduR_Lock_Unlock
 *********************************************************************************************************************/
/*! \brief      This API unlocks the corresponding configured lock mechanism.
 *  \details    The lock can either be an exclusive area or a spinlock.
 *              The lock is only "unlocked" when this is the last call to the API in a sequence of nested "Lock" calls.
 *  \param[in]  lockIdx  Lock ID
 *  \pre        -
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Lock_Unlock(PduR_LockRomIterType lockIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (PDUR_SPINLOCKRAM == STD_ON)
/**********************************************************************************************************************
 * PduR_Lock_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_Init(PduR_MemIdxType memIdx)
{
  PduR_SpinlockRamIterType spinlockRamIdx;
  uint32 spinLockInitValue = Appl_GetSpinlockInitVal();

  for(spinlockRamIdx = 0u; spinlockRamIdx < PduR_GetSizeOfSpinlockRam(memIdx); spinlockRamIdx++)  /* FETA_PDUR_01 */
  {
    PduR_SpinlockCounterRamIterType spinlockCounterRamIdx = PduR_GetSpinlockCounterRamStartIdxOfSpinlockRom(spinlockRamIdx, memIdx);

    for(; spinlockCounterRamIdx < PduR_GetSpinlockCounterRamEndIdxOfSpinlockRom(spinlockRamIdx, memIdx); spinlockCounterRamIdx++)  /* FETA_PDUR_01 */
    {
      PduR_SetSpinlockCounterRam(spinlockCounterRamIdx, 0u, memIdx);
    }

    PduR_SetLockVariableOfSpinlockRam(spinlockRamIdx, spinLockInitValue, memIdx);
  }
}
#endif
/**********************************************************************************************************************
 * PduR_Lock_Lock
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Lock_Lock(PduR_LockRomIterType lockIdx)
{
  {
#if (PDUR_SPINLOCKRAM == STD_ON)
    if(!PduR_IsExclusiveAreaRomUsedOfLockRom(lockIdx))
    {
      SchM_Enter_PduR_PDUR_EXCLUSIVE_AREA_0();
      {
        PduR_SpinlockRamIterType spinlockIdx = PduR_GetSpinlockRamIdxOfLockRom(lockIdx);
        PduR_SpinlockRamPartitionIdxOfLockRomType memIdx = PduR_GetSpinlockRamPartitionIdxOfLockRom(lockIdx);

        uint8 core_id = (uint8) GetCoreID();

        if(core_id < PduR_GetSpinlockCounterRamLengthOfSpinlockRom(spinlockIdx, memIdx))
        {
          PduR_SpinlockCounterRamIterType spinlockCounterRamIdx = (PduR_SpinlockCounterRamIterType) PduR_GetSpinlockCounterRamStartIdxOfSpinlockRom(spinlockIdx, memIdx) + core_id;

          if(0u == PduR_GetSpinlockCounterRam(spinlockCounterRamIdx, memIdx))
          {
            uint32 retryCounter = 0u;
            uint32 retryCounterValue = PduR_GetSpinlockRetryCounterOfGeneralPropertiesRom(0u);

            while((Appl_TryToGetSpinlock(PduR_GetAddrLockVariableOfSpinlockRam(spinlockIdx, memIdx)) != E_OK) && (retryCounter < retryCounterValue)) /* FETA_PDUR_02 */
            {
              retryCounter++;
            }
            /* Report DET error if timeout elapsed */
            if(retryCounter >= retryCounterValue)
            {
              PduR_Reporting_ReportRuntimeError(PDUR_LOCK_LOCK, PDUR_E_SPINLOCKTIMEOUT);
            }
          }
          PduR_IncSpinlockCounterRam(spinlockCounterRamIdx, memIdx);
        }
        else
        {
          PduR_Reporting_ReportError(PDUR_LOCK_LOCK, PDUR_E_FATAL);
        }
      }
    }
    else
#endif
    {
      PduR_GetLockOfExclusiveAreaRom(PduR_GetExclusiveAreaRomIdxOfLockRom(lockIdx)) ();
    }
  }
}

/**********************************************************************************************************************
 * PduR_Lock_Unlock
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_Lock_Unlock(PduR_LockRomIterType lockIdx)
{
  {
#if (PDUR_SPINLOCKRAM == STD_ON)
    if(!PduR_IsExclusiveAreaRomUsedOfLockRom(lockIdx))
    {
      {
        PduR_SpinlockRamIterType spinlockIdx = PduR_GetSpinlockRamIdxOfLockRom(lockIdx);
        PduR_SpinlockRamPartitionIdxOfLockRomType memIdx = PduR_GetSpinlockRamPartitionIdxOfLockRom(lockIdx);

        uint8 core_id = (uint8) GetCoreID();

        if(core_id < PduR_GetSpinlockCounterRamLengthOfSpinlockRom(spinlockIdx, memIdx))
        {
          PduR_SpinlockCounterRamIterType spinlockCounterRamIdx = (PduR_SpinlockCounterRamIterType) PduR_GetSpinlockCounterRamStartIdxOfSpinlockRom(spinlockIdx, memIdx) + core_id;
          /* @ assert spinlockCounterRamIdx < PduR_GetSizeOfSpinlockCounterRam(memIdx); */  /* VCA_PDUR_SPINLOCK_COUNTER_VARIABLES */
          if(1u == PduR_GetSpinlockCounterRam(spinlockCounterRamIdx, memIdx))
          {
            if(E_OK != Appl_ReleaseSpinlock(PduR_GetAddrLockVariableOfSpinlockRam(spinlockIdx, memIdx)))
            {
              PduR_Reporting_ReportError(PDUR_LOCK_UNLOCK, PDUR_E_FATAL);
            }
          }
          PduR_DecSpinlockCounterRam(spinlockCounterRamIdx, memIdx);
        }
        else
        {
          PduR_Reporting_ReportError(PDUR_LOCK_UNLOCK, PDUR_E_FATAL);
        }
      }
      SchM_Exit_PduR_PDUR_EXCLUSIVE_AREA_0();
    }
    else
#endif
    {
      PduR_GetUnlockOfExclusiveAreaRom(PduR_GetExclusiveAreaRomIdxOfLockRom(lockIdx)) ();
    }
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * PduR_Lock_LockRoutingPathBySrcPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_LockRoutingPathBySrcPdu(PduR_RmSrcRomIterType rmSrcRomIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */  /* COV_PDUR_UNCOVERED_FUNCTION_LOCKBYSRCPDU */
{
  PduR_Lock_Lock(PduR_GetLockRomIdxOfRmSrcRom(rmSrcRomIdx));
}

/**********************************************************************************************************************
 * PduR_Lock_UnlockRoutingPathBySrcPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_UnlockRoutingPathBySrcPdu(PduR_RmSrcRomIterType rmSrcRomIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */  /* COV_PDUR_UNCOVERED_FUNCTION_LOCKBYSRCPDU */
{
  PduR_Lock_Unlock(PduR_GetLockRomIdxOfRmSrcRom(rmSrcRomIdx));
}

/**********************************************************************************************************************
 * PduR_Lock_LockRoutingPathByDestPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_LockRoutingPathByDestPdu(PduR_RmDestRomIterType rmDestRomIdx)
{
  PduR_Lock_Lock(PduR_GetLockRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)));
}

/**********************************************************************************************************************
 * PduR_Lock_UnlockRoutingPathByDestPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_UnlockRoutingPathByDestPdu(PduR_RmDestRomIterType rmDestRomIdx)
{
  PduR_Lock_Unlock(PduR_GetLockRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)));
}

/**********************************************************************************************************************
 * PduR_Lock_LockRoutingPathByGDestPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_LockRoutingPathByGDestPdu(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_Lock_Lock(PduR_GetLockRomIdxOfRmGDestRom(rmGDestRomIdx));
}

/**********************************************************************************************************************
 * PduR_Lock_UnlockRoutingPathByGDestPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_UnlockRoutingPathByGDestPdu(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_Lock_Unlock(PduR_GetLockRomIdxOfRmGDestRom(rmGDestRomIdx));
}

#if (PDUR_FMFIFORAM == STD_ON)
/**********************************************************************************************************************
 * PduR_Lock_LockBuffersByFmFifo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_LockBuffersByFmFifo(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx)
{
  if(PduR_IsLockRomUsedOfFmFifoRom(fmFifoRomIdx, memIdx))  /* COV_PDUR_MULTICORE_ONLY */
  {
    PduR_Lock_Lock(PduR_GetLockRomIdxOfFmFifoRom(fmFifoRomIdx, memIdx));
  }
}
#endif

#if (PDUR_FMFIFORAM == STD_ON)
/**********************************************************************************************************************
 * PduR_Lock_UnlockBuffersByFmFifo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Lock_UnlockBuffersByFmFifo(PduR_FmFifoRomIterType fmFifoRomIdx, PduR_MemIdxType memIdx)
{
  if(PduR_IsLockRomUsedOfFmFifoRom(fmFifoRomIdx, memIdx))  /* COV_PDUR_MULTICORE_ONLY */
  {
    PduR_Lock_Unlock(PduR_GetLockRomIdxOfFmFifoRom(fmFifoRomIdx, memIdx));
  }
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_Lock.c
 *********************************************************************************************************************/
