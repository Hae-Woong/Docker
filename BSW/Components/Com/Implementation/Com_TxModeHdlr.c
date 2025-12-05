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
/*!        \file  Com_TxModeHdlr.c
 *         \unit  TxModeHdlr
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_TXMODEHDLR_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_TxModeHdlr.h"
#include "Com_Cfg_TxModeHdlr.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_Repetition.h"

#include "Com_TxSignalFiltering.h"
#include "Com_TxDlMon.h"
#include "Com_Repetition.h"
#include "Com_TxMinDelay.h"
#include "Com_TxCyclic.h"
#include "Com_TxTransmit.h"

#include "SchM_Com.h"

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
  Com_TxModeHdlr_InitTxMode
**********************************************************************************************************************/
/*! \brief        This method initializes the transmission mode parameters (e.g. cycle counter). It is called each time
                  the active tx mode changes.
    \details      -
    \pre          -
    \param[in]    idxTxPduInfo    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \trace        SPEC-2736812, SPEC-2736813
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxModeHdlr_InitTxMode(Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxModeHdlr_InitTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxModeHdlr_InitTxMode(Com_TxPduInfoIterType idxTxPduInfo)
{
  /* If the current tx mode of the passed Tx ComIPdu is periodic */
  if(Com_TxModeHdlr_IsPeriodicTxMode(idxTxPduInfo))
  {
    /* Enable the cyclic send request and trigger the first transmission within the next Com_MainfunctionTx call */
    Com_TxCyclic_InitCyclicTxMode(idxTxPduInfo);
    Com_TxTransmit_SetTransmitRequest(idxTxPduInfo);
  }
  /* Otherwise disable the cyclic send request and remove the transmit request flag */
  else
  {
    Com_TxCyclic_StopCyclicSendRequest(idxTxPduInfo);
    Com_TxTransmit_ClearTransmitRequest(idxTxPduInfo);
  }

  /* Stop repetitions, if configured */
  if(Com_IsRepetitionInfoUsedOfTxPduInfo(idxTxPduInfo))
  {
    Com_RepetitionInfoIterType repetitionInfoIdx = Com_GetRepetitionInfoIdxOfTxPduInfo(idxTxPduInfo);
    /*@ assert repetitionInfoIdx < Com_GetSizeOfRepetitionInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_Repetition_Stop(repetitionInfoIdx);
  }

  /* Reset the tx timeout counter */
  Com_TxDlMon_InitTxMode(idxTxPduInfo);
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxModeHdlr_InitTxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxModeHdlr_InitTxIpdu(Com_TxPduInfoIterType ComPduId)
{
  /* Init Tx Filter parameters */
  Com_TxSignalFiltering_InitTxFilter(ComPduId);

  /* Set the initial Tx Mode of the passed Tx ComIPdu, initialize that mode and remove the transmit request */
  Com_SetCurrentTxMode(ComPduId, Com_IsInitModeOfTxPduInfo(ComPduId));
  Com_TxModeHdlr_InitTxMode(ComPduId);
  Com_TxTransmit_ClearTransmitRequest(ComPduId);

  /* If the current tx mode is a cyclic tx mode, set the cycle counter to the start delay time */
  if(Com_TxCyclic_IsCyclicSendRequest(ComPduId))
  {
    Com_TxCyclic_SetCycleTimeToCurrentTimeOffset(ComPduId);
  }

  /* Set the minimum delay counter to its init value */
  Com_TxMinDelay_ResetCounter(ComPduId);

  /* Clear event based trigger Flag if configured */
  Com_TxTransmit_ClearDirectTrigger(ComPduId);
}

/**********************************************************************************************************************
  Com_TxModeHdlr_UpdateTMS
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxModeHdlr_UpdateTMS(Com_TxPduInfoIterType idxTxPduInfo)
{
  /* Evaluate the current filter state of the Pdu and set the Tx Mode to the filter state. */
  const boolean newTxMode = Com_TxSignalFiltering_IsFilterStateTrue(idxTxPduInfo);

  /* If the Tx Mode has changed */
  if(Com_IsCurrentTxMode(idxTxPduInfo) != newTxMode)
  {
    /* Update the current Tx Mode with the new Tx Mode, initialize the new Tx Mode and trigger a deferred transmission without repetitions */
    Com_SetCurrentTxMode(idxTxPduInfo, newTxMode);

    Com_TxModeHdlr_InitTxMode(idxTxPduInfo);
    /* COM582: If a change of the TMS causes a change of the transmission mode to the transmission mode DIRECT an immediate, respecting the MDT, direct/ n-times transmission to the underlying layer shall be initiated. */
    Com_TxTransmit_TriggerDeferredTransmitAndSetTimer(idxTxPduInfo, FALSE);
  }
}

/**********************************************************************************************************************
  Com_TxModeHdlr_StopTxIpdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxModeHdlr_StopTxIpdus(Com_TxPduInfoIterType ComTxPduId)
{
  /* Reset transmit request, set repetition counter and repetition cycle counter to 0 */
  Com_TxTransmit_ClearTransmitRequest(ComTxPduId);
  if(Com_IsRepetitionInfoUsedOfTxPduInfo(ComTxPduId))
  {
    Com_RepetitionInfoIterType repetitionInfoIdx = Com_GetRepetitionInfoIdxOfTxPduInfo(ComTxPduId);
    /*@ assert repetitionInfoIdx < Com_GetSizeOfRepetitionInfo();*/ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_Repetition_Stop(repetitionInfoIdx);
  }
  /* reload the time offset counter and set minimum delay time to 0. If the I-PDU is started again, the time offset is already set */
  Com_TxCyclic_SetCycleTimeToCurrentTimeOffset(ComTxPduId);

  Com_TxMinDelay_ResetCounter(ComTxPduId);
}

/**********************************************************************************************************************
  Com_TxModeHdlr_SwitchIpduTxMode
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
FUNC(void, COM_CODE) Com_TxModeHdlr_SwitchIpduTxMode(Com_TxPduInfoIterType ComTxPduId, boolean Mode)
{
  /* If the passed tx mode differs form the current tx mode, update the current tx mode value and initialize the new tx mode */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  {
    if(Com_IsCurrentTxMode(ComTxPduId) != Mode)
    {
      Com_SetCurrentTxMode(ComTxPduId, Mode);
      Com_TxModeHdlr_InitTxMode(ComTxPduId);

      /* If new mode is cyclic (MIXED or DIRECT) */
      if(Com_TxCyclic_IsCyclicSendRequest(ComTxPduId))
      {
        const Com_CycleTimeCntType currentTimeOffset = Com_TxModeHdlr_GetCurrentTimeOffset(ComTxPduId);
        /* If TimeOffset is configured, set periodic counter to FirstTimeOffset */
        if(currentTimeOffset > 1U)
        {
          /* If MDT remaining time is smaller than FirstTimeOffset */
          if(Com_TxMinDelay_GetCurrentMDTValue(ComTxPduId) < currentTimeOffset)
          {
            /* Clear TransmitRequest, as elapsing MDT shall not trigger transmission */
            Com_TxTransmit_ClearTransmitRequest(ComTxPduId);
            /* Set period to FirstTimeOffset */
            Com_TxCyclic_SetCycleTimeToCurrentTimeOffset(ComTxPduId);
          }
        }
      }
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
}

/**********************************************************************************************************************
  Com_TxModeHdlr_IsDirectTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxModeHdlr_IsDirectTxMode(Com_TxPduInfoIterType TxPduId)
{
  return ((Com_IsCurrentTxMode(TxPduId)) ? (Com_IsDirectOfTxModeTrue(Com_GetTxModeTrueIdxOfTxPduInfo(TxPduId))) : (Com_IsDirectOfTxModeFalse(Com_GetTxModeFalseIdxOfTxPduInfo(TxPduId))));
}

/**********************************************************************************************************************
  Com_TxModeHdlr_IsPeriodicTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxModeHdlr_IsPeriodicTxMode(Com_TxPduInfoIterType TxPduId)
{
  return ((Com_IsCurrentTxMode(TxPduId)) ? (Com_IsPeriodicOfTxModeTrue(Com_GetTxModeTrueIdxOfTxPduInfo(TxPduId))) : (Com_IsPeriodicOfTxModeFalse(Com_GetTxModeFalseIdxOfTxPduInfo(TxPduId))));
}

/**********************************************************************************************************************
  Com_TxModeHdlr_GetCurrentTimePeriod
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_CycleTimeCntType, COM_CODE) Com_TxModeHdlr_GetCurrentTimePeriod(Com_TxPduInfoIterType TxPduId)
{
  return ((Com_IsCurrentTxMode(TxPduId)) ? (Com_GetTimePeriodOfTxModeTrue(Com_GetTxModeTrueIdxOfTxPduInfo(TxPduId))) : (Com_GetTimePeriodOfTxModeFalse(Com_GetTxModeFalseIdxOfTxPduInfo(TxPduId))));
}

/**********************************************************************************************************************
  Com_TxModeHdlr_GetCurrentTimeOffset
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_CycleTimeCntType, COM_CODE) Com_TxModeHdlr_GetCurrentTimeOffset(Com_TxPduInfoIterType TxPduId)
{
  return ((Com_IsCurrentTxMode(TxPduId)) ? (Com_GetTimeOffsetOfTxModeTrue(Com_GetTxModeTrueIdxOfTxPduInfo(TxPduId))) : (Com_GetTimeOffsetOfTxModeFalse(Com_GetTxModeFalseIdxOfTxPduInfo(TxPduId))));
}

/**********************************************************************************************************************
  Com_TxModeHdlr_IsCurrentTxModeTrue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxModeHdlr_IsCurrentTxModeTrue(Com_TxPduInfoIterType TxPduId)
{
  return Com_IsCurrentTxMode(TxPduId);
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
 *  END OF FILE: Com_TxModeHdlr.c
 *********************************************************************************************************************/
