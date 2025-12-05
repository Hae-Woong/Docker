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
/*!        \file  Com_Repetition.c
 *         \unit  Repetition
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_REPETITION_SOURCE

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
#include "Com_Repetition.h"
#include "Com_Cfg_Repetition.h"

#include "Com_TxTransmit.h"
#include "Com_TxModeHdlr.h"

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
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_Repetition_GetCurRepCnt
**********************************************************************************************************************/
/*! \brief             This function returns the current repetition count.
 *  \details           -
 *  \pre               -
 *  \param[in]         repetitionInfoIdx Repetition Info index.
 *  \return            current repetition count.
 *  \synchronous       TRUE
    \reentrant         TRUE, for different handles
    \context           TASK|ISR2
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_RepCntType, COM_CODE) Com_Repetition_GetCurRepCnt(Com_RepetitionInfoIterType repetitionInfoIdx);

/**********************************************************************************************************************
  Com_Repetition_GetCurrentRepetitionPeriod
**********************************************************************************************************************/
/*! \brief             This function returns the current repetition period.
 *  \details           -
 *  \pre               -
 *  \param[in]         repetitionInfoIdx Repetition Info index.
 *  \return            current repetition period.
 *  \synchronous       TRUE
    \reentrant         TRUE, for different handles
    \context           TASK|ISR2
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_RepCycleCntType, COM_CODE) Com_Repetition_GetCurrentRepetitionPeriod(Com_RepetitionInfoIterType repetitionInfoIdx);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Repetition_GetCurRepCnt
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_RepCntType, COM_CODE) Com_Repetition_GetCurRepCnt(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  Com_RepCntType retVal = 0u;
  const Com_TxPduInfoIterType TxPduId = Com_GetTxPduInfoIdxOfRepetitionInfo(repetitionInfoIdx);
  if (Com_TxModeHdlr_IsCurrentTxModeTrue(TxPduId))
  {
    retVal = (Com_RepCntType)Com_GetRepCntTrueOfRepetitionInfo(repetitionInfoIdx);
  }
  else
  {
    retVal = (Com_RepCntType)Com_GetRepCntFalseOfRepetitionInfo(repetitionInfoIdx);
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_Repetition_GetCurrentRepetitionPeriod
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_RepCycleCntType, COM_CODE) Com_Repetition_GetCurrentRepetitionPeriod(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  Com_RepCycleCntType retVal = 0u;
  const Com_TxPduInfoIterType TxPduId = Com_GetTxPduInfoIdxOfRepetitionInfo(repetitionInfoIdx);
  if (Com_TxModeHdlr_IsCurrentTxModeTrue(TxPduId))
  {
    retVal = (Com_RepCycleCntType)Com_GetRepPeriodTrueOfRepetitionInfo(repetitionInfoIdx);
  }
  else
  {
    retVal = (Com_RepCycleCntType)Com_GetRepPeriodFalseOfRepetitionInfo(repetitionInfoIdx);
  }
  return retVal;
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Repetition_Init
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_Init(void)
{
  for(Com_RepetitionInfoIterType repetitionInfoIndex = 0u; repetitionInfoIndex < Com_GetSizeOfRepetitionInfo(); repetitionInfoIndex++)  /* FETA_COM_01 */
  {
    Com_SetRepCnt(repetitionInfoIndex, 0u);
    Com_SetRepCycleCnt(repetitionInfoIndex, 0u);
  }
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_Repetition_Start
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_Start(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  Com_SetRepCnt(repetitionInfoIdx, Com_Repetition_GetCurRepCnt(repetitionInfoIdx));
  Com_SetRepCycleCnt(repetitionInfoIdx, Com_Repetition_GetCurrentRepetitionPeriod(repetitionInfoIdx));
}
#endif

/**********************************************************************************************************************
  Com_Repetition_StartFirstTransmission
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_StartFirstTransmission(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  Com_SetRepCnt(repetitionInfoIdx, Com_Repetition_GetCurRepCnt(repetitionInfoIdx));
  if (Com_Repetition_IsInProgress(repetitionInfoIdx))
  {
    Com_SetRepCycleCnt(repetitionInfoIdx, 1u);
  }
}

/**********************************************************************************************************************
  Com_Repetition_Stop
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_Stop(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  Com_SetRepCnt(repetitionInfoIdx, 0u);
  Com_SetRepCycleCnt(repetitionInfoIdx, 0u);
}

#if (COM_STRICT_REPETITION_PERIOD == STD_ON)
/**********************************************************************************************************************
  Com_Repetition_ResetRepCycle
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_ResetRepCycle(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  Com_SetRepCycleCnt(repetitionInfoIdx, Com_Repetition_GetCurrentRepetitionPeriod(repetitionInfoIdx));
}
#endif

/**********************************************************************************************************************
  Com_Repetition_IsInProgress
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Repetition_IsInProgress(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  boolean retVal = FALSE;
  if(Com_GetRepCnt(repetitionInfoIdx) > 0u)
  {
    retVal = TRUE;
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_Repetition_ProcessRepetitions
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Repetition_ProcessRepetitions(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  const Com_TxPduInfoIterType TxPduId = Com_GetTxPduInfoIdxOfRepetitionInfo(repetitionInfoIdx);  /* PRQA S 2983 */ /* MD_Com_2982_2983 */
  /* If the repetition count is greater 0, decrement the repetition cycle counter */
  if(Com_GetRepCnt(repetitionInfoIdx) > 0u)
  {
    if(!Com_TxTransmit_IsInitialTransmitFlag(TxPduId))
    {
      Com_TxTransmit_ClearTransmitRequest(TxPduId);
    }

    Com_DecRepCycleCnt(repetitionInfoIdx);
    /* If the repetition cycle counter is equal 0, set a transmit request and reload the repetition cycle counter */
    if(Com_GetRepCycleCnt(repetitionInfoIdx) == 0u)
    {
      Com_TxTransmit_SetTransmitRequest(TxPduId);
      Com_SetRepCycleCnt(repetitionInfoIdx, Com_Repetition_GetCurrentRepetitionPeriod(repetitionInfoIdx));
    }
  }
}

/**********************************************************************************************************************
  Com_Repetition_Confirmation
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Repetition_Confirmation(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  boolean retVal = TRUE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* Decrement the Rep Counter if greater 0 */
  if(0u < Com_GetRepCnt(repetitionInfoIdx))
  {
    Com_DecRepCnt(repetitionInfoIdx);

    /* Check if the all requested transmits have been performed and this is the final Tx Confirmation */
    if(0u < Com_GetRepCnt(repetitionInfoIdx))
    {
      /* this is the first or an intermediate confirmation for an N-Times transmission return false to avoid notification of the upper layer */
      retVal = FALSE;
    }
  }
  return retVal;
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
 *  END OF FILE: Com_Repetition.c
 *********************************************************************************************************************/
