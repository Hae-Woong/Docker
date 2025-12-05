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
 *         \file  IpduM_ContainerTxTrigger.c
 *         \unit  ContainerTxTrigger
 *        \brief  Microsar IpduM ContainerTxTrigger source file
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

#define IPDUM_CONTAINERTXTRIGGER_SOURCE
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
#include "IpduM_ContainerTxTrigger.h"

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
*  IpduM_ContainerTxTrigger_Init()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTxTrigger_Init(void)
{
  IpduM_VTxContainerPduIterType txContainerPduIdx;

  for(txContainerPduIdx = 0u; txContainerPduIdx < IpduM_GetSizeOfVTxContainerPdu(); txContainerPduIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_SetFirstContainedPduOfTxContainerPdu(txContainerPduIdx, FALSE);
  }
}

/*******************************************************************************************
*  IpduM_ContainerTxTrigger_IsTriggered()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTxTrigger_IsTriggered(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId, uint32 containerLength)
{
  IpduM_CTxContainerPduIterType txContainerId = IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId);
  boolean isTriggered;

  if(containerLength > IpduM_GetTxSizeThresholdOfTxContainerPdu(txContainerId))
  {
    isTriggered = TRUE;
  }
  else if(IpduM_IsTxPduTriggerOfTxContainedPdu(txContainedId))
  {
    isTriggered = TRUE;
  }
  else if(IpduM_IsFirstContainedPduTriggerOfTxContainerPdu(txContainerId) && IpduM_IsFirstContainedPduOfTxContainerPdu(txContainerId))
  {
    isTriggered = TRUE;
  }
  else
  {
    /* else path because of MISRA. */
    isTriggered = FALSE;
  }

  return isTriggered;
}

/*******************************************************************************************
*  IpduM_ContainerTxTrigger_SetFirstContainedPduFlag()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTxTrigger_SetFirstContainedPduFlag(IpduM_CTxContainerPduIterType txContainerIdx, boolean value)
{
  IpduM_SetFirstContainedPduOfTxContainerPdu(txContainerIdx, value);
}

/*******************************************************************************************
*  IpduM_ContainerTxTrigger_IsFirstContainedPdu()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTxTrigger_IsFirstContainedPdu(IpduM_CTxContainerPduIterType txContainerIdx)
{
  return IpduM_IsFirstContainedPduOfTxContainerPdu(txContainerIdx);
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxTrigger.c
**********************************************************************************************************************/
