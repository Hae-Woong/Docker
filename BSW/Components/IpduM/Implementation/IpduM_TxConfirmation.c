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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  IpduM_TxConfirmation.c
 *         \unit  TxConfirmation
 *        \brief  Microsar IpduM TxConfirmation source file
 *      \details  This is the implementation of the MICROSAR IpduM module.
 *                The basic software module is based on the AUTOSAR IpduM specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define IPDUM_TXCONFIRMATION_SOURCE
/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_IpduM_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "IpduM.h"
#include "IpduM_MuxTx.h"
#include "IpduM_ContainerTx.h"
#include "IpduM_TxConfirmation.h"
#include "SchM_IpduM.h"

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
  LOCAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_TxConfirmation_Init()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_Init(void)
{
  IpduM_CTxLoInfoIterType txConfTimeoutCntIdx;
  for(txConfTimeoutCntIdx = 0u; txConfTimeoutCntIdx < IpduM_GetSizeOfVTxLoInfo(); txConfTimeoutCntIdx++) /* FETA_IPDUM_01 */
  {
    /* Reset timeout counter for given multiplex handle in this loop iteration */
    IpduM_SetTxConfTimeoutCntOfVTxLoInfo(txConfTimeoutCntIdx, 0u);
  }
}

/*******************************************************************************************
 *  IpduM_TxConfirmation_StartTimeout()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_StartTimeout(IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx)
{
  /* Load timeout counter */
  IpduM_SetTxConfTimeoutCntOfVTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx, IpduM_GetTxConfirmationTimeoutOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx)); /* PRQA S 2842 */ /* MD_IpduM_2842 */
}

/*******************************************************************************************
 *  IpduM_TxConfirmation_StopTimeout()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_StopTimeout(IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx)
{
  /* Reset timeout counter */
  IpduM_SetTxConfTimeoutCntOfVTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx, 0u); /* PRQA S 2842 */ /* MD_IpduM_2842 */
}

/*******************************************************************************************
 *  IpduM_TxConfirmation_IsTransmissionAllowed
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_TxConfirmation_IsTransmissionAllowed(IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx)
{
  return (IpduM_GetTxConfirmationTimeoutOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx) == 0u) || (IpduM_GetTxConfTimeoutCntOfVTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx) == 0u);
}

/*******************************************************************************************
 *  IpduM_TxConfirmation_TxConfirmation()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_TxConfirmation(IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx)
{
  if((IpduM_GetTxConfirmationTimeoutOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx) == 0u)  /* No TxConfirmationTimeout is configured. */
      || (IpduM_GetTxConfTimeoutCntOfVTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx) != 0u)  /* TxConfirmationTimeout is configured and NOT elapsed. */
      )
  {
    if(IpduM_IsTxPathwayUsedOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx))
    {
      IpduM_MuxTx_TxConfirmation(IpduM_GetTxPathwayIdxOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx));
    }
    if(IpduM_IsTxContainerPduUsedOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx))
    {
      IpduM_ContainerTx_TxConfirmation(IpduM_GetTxContainerPduIdxOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx), E_OK);
    }
  }

  IpduM_TxConfirmation_StopTimeout(txLoInfoWithInvalidIndexesIdx);
}

/*******************************************************************************************
 *  IpduM_TxConfirmation_MainFunction()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation_MainFunction(IpduM_MainFunctionTxIndirectionIterType mainFunctionTxIndirectionIdx)
{
  IpduM_TxLoInfoIndStartIdxOfMainFunctionTxIndirectionType txLoInfoIndIdx = IpduM_GetTxLoInfoIndStartIdxOfMainFunctionTxIndirection(mainFunctionTxIndirectionIdx);
  IpduM_TxLoInfoIndEndIdxOfMainFunctionTxIndirectionType txLoInfoIndEndIdx = IpduM_GetTxLoInfoIndEndIdxOfMainFunctionTxIndirection(mainFunctionTxIndirectionIdx);

  for(; txLoInfoIndIdx < txLoInfoIndEndIdx; txLoInfoIndIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_CTxLoInfoIdxOfTxLoInfoWithInvalidIndexesType txLoInfoWithInvalidIndexesIdx = IpduM_GetTxLoInfoInd(txLoInfoIndIdx);

    /* TxConfirmation Timeout is about to elapse. Call negative TxConfirmation! */
    if(IpduM_GetTxConfTimeoutCntOfVTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx) == 1u)
    {
      /* Call negative TxConfirmation for Container Pdus. */
      if(IpduM_IsTxContainerPduUsedOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx))
      {
        IpduM_ContainerTx_TxConfirmation(IpduM_GetTxContainerPduIdxOfCTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx), E_NOT_OK);
      }
      /* No negative TxConfirmation for Multiplex Pdus implemented. */
    }

    SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
    if(IpduM_GetTxConfTimeoutCntOfVTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx) != 0u)
    {
      IpduM_DecTxConfTimeoutCntOfVTxLoInfoOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx);
    }
    SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_TxConfirmation.c
**********************************************************************************************************************/
