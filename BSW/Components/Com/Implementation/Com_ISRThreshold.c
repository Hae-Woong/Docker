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
/*!        \file  Com_ISRThreshold.c
 *         \unit  ISRThreshold
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_ISRTHRESHOLD_SOURCE

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
#include "Com_ISRThreshold.h"
#include "Com_Cfg_ISRThreshold.h"

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
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_ISRThreshold_Init
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ISRThreshold_Init(void)
{
  for(Com_ThresholdCounterIterType isrThresholdInfoIdx = 0; isrThresholdInfoIdx < Com_GetSizeOfThresholdCounter(); isrThresholdInfoIdx++)  /* FETA_COM_01 */
  {
    Com_SetThresholdCounter(isrThresholdInfoIdx, Com_GetThresholdValueOfISRThresholdInfo(isrThresholdInfoIdx));
  }
}

/**********************************************************************************************************************
  Com_ISRThreshold_EnterExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ISRThreshold_EnterExclusiveArea(Com_ISRThresholdInfoIterType isrThresholdInfoIdx)
{
  switch (Com_GetISRLockTypeOfISRThresholdInfo(isrThresholdInfoIdx))
  {
    case COM_RX_ISRLOCKTYPEOFISRTHRESHOLDINFO:
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
      break;
    case COM_TX_ISRLOCKTYPEOFISRTHRESHOLDINFO:
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
      break;
    case COM_BOTH_ISRLOCKTYPEOFISRTHRESHOLDINFO:
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_BOTH();
      break;

    default:   /* COV_COM_MISRA */
    /* Intentionally Empty */
      break;
  }
}

/**********************************************************************************************************************
  Com_ISRThreshold_ExitExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ISRThreshold_ExitExclusiveArea(Com_ISRThresholdInfoIterType isrThresholdInfoIdx)
{
  Com_SetThresholdCounter(isrThresholdInfoIdx, Com_GetThresholdValueOfISRThresholdInfo(isrThresholdInfoIdx));
  switch (Com_GetISRLockTypeOfISRThresholdInfo(isrThresholdInfoIdx))
  {
    case COM_RX_ISRLOCKTYPEOFISRTHRESHOLDINFO:
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
      break;
    case COM_TX_ISRLOCKTYPEOFISRTHRESHOLDINFO:
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
      break;
    case COM_BOTH_ISRLOCKTYPEOFISRTHRESHOLDINFO:
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_BOTH();
      break;

    default:   /* COV_COM_MISRA */
    /* Intentionally Empty */
      break;
  }
}

/**********************************************************************************************************************
  Com_ISRThreshold_ThresholdCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ISRThreshold_ThresholdCheck(Com_ISRThresholdInfoIterType isrThresholdInfoIdx)
{
  if(Com_ISRThreshold_DecThresholdCounter(isrThresholdInfoIdx) == FALSE)
  {
    Com_ISRThreshold_ExitExclusiveArea(isrThresholdInfoIdx);
    Com_ISRThreshold_EnterExclusiveArea(isrThresholdInfoIdx);
  }
}

/**********************************************************************************************************************
  Com_ISRThreshold_DecThresholdCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_ISRThreshold_DecThresholdCounter(Com_ISRThresholdInfoIterType isrThresholdInfoIdx)
{
  boolean ret = FALSE;
  if(Com_GetThresholdCounter(isrThresholdInfoIdx) > 0u)
  {
    ret = TRUE;
    Com_DecThresholdCounter(isrThresholdInfoIdx);
  }
  return ret;
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
 *  END OF FILE: Com_ISRThreshold.c
 *********************************************************************************************************************/
