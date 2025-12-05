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
/*!        \file  Com_GwTout.c
 *         \unit  GwTout
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_GWTOUT_SOURCE

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
#include "Com_GwTout.h"
#include "Com_Cfg_GwTout.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_Timer.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_Transmission.h"

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
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_GwTout_MainFunctionTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GwTout_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId)
{
  if(Com_HasGwTimeoutInfo())
  {
    SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
    {
      /* Iterate over all gateway timeout infos */
      for(Com_GwTimeoutInfoOfMainFunctionTxIndIterType gwTimeoutInfoIndIdx = Com_GetGwTimeoutInfoOfMainFunctionTxIndStartIdxOfMainFunctionTxStruct(mainFunctionId);
          gwTimeoutInfoIndIdx < Com_GetGwTimeoutInfoOfMainFunctionTxIndEndIdxOfMainFunctionTxStruct(mainFunctionId); gwTimeoutInfoIndIdx++)  /* FETA_COM_01 */
      {
        const Com_GwTimeoutInfoIterType gwToutHndl = Com_GetGwTimeoutInfoOfMainFunctionTxInd(gwTimeoutInfoIndIdx);

        /* If the tx pdu is active and the timeout is active, decrement the gateway timeout counter */
        if(Com_IPduGroupHdlr_IsTxPduActive((Com_GetTxPduInfoIdxOfGwTimeoutInfo(gwToutHndl))))
        {
          Com_Timer_DecCounter(Com_GetComTimerInfoIdxOfGwTimeoutInfo(gwToutHndl)); /* VCA_COM_CSL03 */
        }
      }
    }
    SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
  }
}

/**********************************************************************************************************************
  Com_GwTout_InitCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GwTout_InitCounter(Com_TxPduInfoIterType ComTxPduId)
{
  /* If the passed ComIPdu has a configured gateway timeout */
  if(Com_IsGwTimeoutInfoUsedOfTxPduInfo(ComTxPduId))
  {
    /* Initialize the timeout counter */
    Com_Timer_InitCounter(Com_GetComTimerInfoIdxOfGwTimeoutInfo(Com_GetGwTimeoutInfoIdxOfTxPduInfo(ComTxPduId)));

    /* Stop cyclic transmission */
    Com_Transmission_StopCyclicTransmission(ComTxPduId);
  }
}

/**********************************************************************************************************************
  Com_GwTout_Event
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GwTout_Event(Com_TxPduInfoIterType ComTxPduId)
{
  /* If the passed Tx ComIPdu is active and the ComIPdu has a configured gateway timeout */
  if(Com_IPduGroupHdlr_IsTxPduActive(ComTxPduId))
  {
    if(Com_IsGwTimeoutInfoUsedOfTxPduInfo(ComTxPduId))
    {
      /* If the current tx mode is periodic */
      const Com_GwTimeoutInfoIterType gwToutId = Com_GetGwTimeoutInfoIdxOfTxPduInfo(ComTxPduId);
      if(Com_Transmission_IsPeriodicTxMode(ComTxPduId))
      {
        /* Start the cyclic transmission of the passed Tx ComIPdu */
        Com_Transmission_StartCyclicTransmission(ComTxPduId);
        /* Reload the gateway timeout counter with the gateway timeout factor */
        Com_Timer_StartCounter(Com_GetComTimerInfoIdxOfGwTimeoutInfo(gwToutId));
      }
      /* Otherwise set the gateway routing timeout to 0 */
      else
      {
        Com_Timer_StopCounter(Com_GetComTimerInfoIdxOfGwTimeoutInfo(gwToutId));
      }
    }
  }
}
#endif


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
 *  END OF FILE: Com_GwTout.c
 *********************************************************************************************************************/
