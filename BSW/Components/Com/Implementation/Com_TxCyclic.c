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
/*!        \file  Com_TxCyclic.c
 *         \unit  TxCyclic
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TXCYCLIC_SOURCE

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
#include "Com_TxCyclic.h"
#include "Com_Cfg_TxCyclic.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_TxModeHdlr.h"
#include "Com_TxMinDelay.h"
#include "Com_TxDlMon.h"
#include "Com_TxTransmit.h"

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
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_TxCyclic_StopCyclicTransmission
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_StopCyclicTransmission(Com_TxPduInfoIterType ComTxPduId)
{
  Com_TxModeHdlr_StopTxIpdus(ComTxPduId);
  Com_TxCyclic_StopCyclicSendRequest(ComTxPduId);
}
#endif

/**********************************************************************************************************************
  Com_TxCyclic_StopCyclicSendRequest
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_StopCyclicSendRequest(Com_TxPduInfoIterType ComTxPduId)
{
  Com_SetCyclicSendRequest(ComTxPduId, FALSE);
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_TxCyclic_StartCyclicTransmission
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_StartCyclicTransmission(Com_TxPduInfoIterType ComTxPduId)
{
  if(!Com_IsCyclicSendRequest(ComTxPduId))
  {
    Com_SetCyclicSendRequest(ComTxPduId, TRUE);
    Com_SetCycleTimeCnt(ComTxPduId, 1u);
  }
}
#endif

/**********************************************************************************************************************
  Com_TxCyclic_SetCycleTimeToCurrentTimeOffset
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_SetCycleTimeToCurrentTimeOffset(Com_TxPduInfoIterType ComTxPduId)
{
  Com_SetCycleTimeCnt(ComTxPduId, Com_TxModeHdlr_GetCurrentTimeOffset(ComTxPduId));
}

/**********************************************************************************************************************
  Com_TxCyclic_InitCyclicTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_InitCyclicTxMode(Com_TxPduInfoIterType ComTxPduId)
{
  Com_SetCycleTimeCnt(ComTxPduId, (Com_CycleTimeCntType) (Com_TxModeHdlr_GetCurrentTimePeriod(ComTxPduId) + Com_TxMinDelay_GetCurrentMDTValue(ComTxPduId) + 1u));
  Com_SetCyclicSendRequest(ComTxPduId, TRUE);
}

/**********************************************************************************************************************
  Com_TxCyclic_ProcessCyclicPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxCyclic_ProcessCyclicPdu(Com_TxPduInfoIterType ComTxPduId)
{
  /* If the passed Tx ComIPdu is in a cyclic Pdu, decrement the cycle counter */
  if(Com_IsCyclicSendRequest(ComTxPduId))
  {
    Com_DecCycleTimeCnt(ComTxPduId);
    /* If the cycle counter is 0, set the transmit flag and reload the cycle counter */
    if(Com_GetCycleTimeCnt(ComTxPduId) == 0u)
    {
      Com_TxTransmit_SetTransmitRequest(ComTxPduId);

      Com_SetCycleTimeCnt(ComTxPduId, Com_TxModeHdlr_GetCurrentTimePeriod(ComTxPduId));

      /* Reload Tx Deadline monitoring timer if configured */
      Com_TxDlMon_StartTxDlMonCounter(ComTxPduId);
    }
  }
}

/**********************************************************************************************************************
  Com_TxCyclic_IsCyclicSendRequest
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxCyclic_IsCyclicSendRequest(Com_TxPduInfoIterType ComTxPduId)
{
  return Com_IsCyclicSendRequest(ComTxPduId);
}


#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */


/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/


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
 *  END OF FILE: Com_TxCyclic.c
 *********************************************************************************************************************/
