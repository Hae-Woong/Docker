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
/*!        \file  Com_Timer.c
 *         \unit  Timer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TIMER_SOURCE

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
#include "Com_Timer.h"
#include "Com_Cfg_Timer.h"

#include "Com_TxDlMon.h"
#include "Com_RxDlMon.h"
#include "Com_Transmission.h"

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
  Com_Timer_EventDispatcher
**********************************************************************************************************************/
/*! \brief        This function calls the dependent timeout event callout.
    \details      -
    \pre          -
    \param[in]    idxTimerInfo timer info handle
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Timer_EventDispatcher(Com_ComTimerInfoIterType idxTimerInfo);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Timer_EventDispatcher
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Timer_EventDispatcher(Com_ComTimerInfoIterType idxTimerInfo)
{
  switch (Com_GetEventCalloutDestHandleOfComTimerInfo(idxTimerInfo))
  {
    case COM_TXTOUTINFO_EVENTCALLOUTDESTHANDLEOFCOMTIMERINFO:
      Com_TxDlMon_Event((Com_TxTOutInfoIterType) Com_GetEventCalloutDestHandleIdxOfComTimerInfo(idxTimerInfo)); /* VCA_COM_ENUM_INDEX_CSL04 */
      break;

    case COM_RXTOUTINFO_EVENTCALLOUTDESTHANDLEOFCOMTIMERINFO:
      Com_RxDlMon_TimeOutEvent((Com_RxTOutInfoIterType) Com_GetEventCalloutDestHandleIdxOfComTimerInfo(idxTimerInfo)); /* VCA_COM_ENUM_INDEX_CSL04 */
      break;

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
    case COM_TXPDUINFO_EVENTCALLOUTDESTHANDLEOFCOMTIMERINFO:
      Com_Transmission_StopCyclicTransmission((Com_TxPduInfoIterType) Com_GetEventCalloutDestHandleIdxOfComTimerInfo(idxTimerInfo)); /* VCA_COM_ENUM_INDEX_CSL04 */
      break;
#endif
    default:
      /* Intentionally Empty */
      break;
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Timer_DecCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_DecCounter(Com_ComTimerInfoIterType idxTimerInfo)
{
  if(Com_GetTimerCounterValue(idxTimerInfo) > 0u)
  {
    Com_DecTimerCounterValue(idxTimerInfo);
    if(Com_GetTimerCounterValue(idxTimerInfo) == 0u)
    {
      if(Com_IsAutoReloadOfComTimerInfo(idxTimerInfo))
      {
        Com_SetTimerCounterValue(idxTimerInfo, Com_GetStartValueOfComTimerInfo(idxTimerInfo) - (Com_TimerCounterValueType)1u);
      }
      Com_Timer_EventDispatcher(idxTimerInfo);
    }
  }
}

/**********************************************************************************************************************
  Com_Timer_Init
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_Init(void)
{
  for(Com_ComTimerInfoIterType idxTimerInfo = 0u; idxTimerInfo < Com_GetSizeOfComTimerInfo(); idxTimerInfo++)  /* FETA_COM_01 */
  {
    Com_SetTimerCounterValue(idxTimerInfo, Com_GetInitValueOfComTimerInfo(idxTimerInfo));
  }
}

/**********************************************************************************************************************
  Com_Timer_StartCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_StartCounter(Com_ComTimerInfoIterType idxTimerInfo)
{
  Com_SetTimerCounterValue(idxTimerInfo, Com_GetStartValueOfComTimerInfo(idxTimerInfo));
}

/**********************************************************************************************************************
  Com_Timer_InitCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_InitCounter(Com_ComTimerInfoIterType idxTimerInfo)
{
  Com_SetTimerCounterValue(idxTimerInfo, Com_GetInitValueOfComTimerInfo(idxTimerInfo));
}

/**********************************************************************************************************************
  Com_Timer_StopCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Timer_StopCounter(Com_ComTimerInfoIterType idxTimerInfo)
{
  Com_SetTimerCounterValue(idxTimerInfo, 0u);
}

/**********************************************************************************************************************
  Com_Timer_IsTimerRunning
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Timer_IsTimerRunning(Com_ComTimerInfoIterType idxTimerInfo)
{
  boolean ret = FALSE;
  if(Com_GetTimerCounterValue(idxTimerInfo) != 0u)
  {
    ret = TRUE;
  }
  return ret;
}

/**********************************************************************************************************************
  Com_Timer_GetCurrentTimerValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_TimerCounterValueType, COM_CODE) Com_Timer_GetCurrentTimerValue(Com_TimerCounterValueIterType idxTimerInfo)
{
  return Com_GetTimerCounterValue(idxTimerInfo);
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
 *  END OF FILE: Com_Timer.c
 *********************************************************************************************************************/
