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
 *         \file  PduR_RPG.c
 *         \unit  RPG
 *        \brief  Pdu Router Routing Path Group source file
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

#define PDUR_RPG_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_MultipleSourceHandler.h"
#include "PduR_RPG.h"
#include "PduR_RmIf_QueueProcessing.h"
#include "PduR_RmTp_TxInst.h"
#include "PduR_Lock.h"

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
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * PduR_RPG_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RPG_Init(PduR_MemIdxType memIdx)
{
  {
    PduR_RmDestRpgRomIterType rmDestRpgRomIdx;
    for(rmDestRpgRomIdx = 0u; rmDestRpgRomIdx < PduR_GetSizeOfRmDestRpgRom(memIdx); rmDestRpgRomIdx++)  /* FETA_PDUR_01 */
    {
      PduR_SetEnabledCntOfRmDestRpgRam(rmDestRpgRomIdx, PduR_GetInitialEnabledCntOfRmDestRpgRom(rmDestRpgRomIdx, memIdx), memIdx);
    }
  }
  {
    PduR_RpgRomIterType rpgRomIdx;
    for(rpgRomIdx = 0u; rpgRomIdx < PduR_GetSizeOfRpgRom(memIdx); rpgRomIdx++)  /* FETA_PDUR_01 */
    {
      PduR_SetEnabledOfRpgRam(rpgRomIdx, PduR_IsEnabledAtInitOfRpgRom(rpgRomIdx, memIdx), memIdx);
    }
  }
}

/**********************************************************************************************************************
 * PduR_RPG_EnableRouting
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RPG_EnableRouting(PduR_RpgRomIterType rpgRomIdx, PduR_MemIdxType memIdx)
{
  if(!PduR_IsEnabledOfRpgRam(rpgRomIdx, memIdx))
  {
    PduR_RmDestRpgRomIndIterType rmDestRpgRomIndIdx;
    for(rmDestRpgRomIndIdx = PduR_GetRmDestRpgRomIndStartIdxOfRpgRom(rpgRomIdx, memIdx); rmDestRpgRomIndIdx < PduR_GetRmDestRpgRomIndEndIdxOfRpgRom(rpgRomIdx, memIdx); rmDestRpgRomIndIdx++)  /* FETA_PDUR_01 */
    {
      PduR_RmDestRpgRomIterType rmDestRpgRomIdx = PduR_GetRmDestRpgRomInd(rmDestRpgRomIndIdx, memIdx);
      PduR_RmDestRomIterType rmDestRomIdx = PduR_GetRmDestRomIdxOfRmDestRpgRom(rmDestRpgRomIdx, memIdx);
      PduR_Lock_LockRoutingPathByDestPdu(rmDestRomIdx);
      {
        PduR_SetEnabledCntOfRmDestRpgRam(rmDestRpgRomIdx, PduR_GetEnabledCntOfRmDestRpgRam(rmDestRpgRomIdx, memIdx) + 1u, memIdx);
      }
      PduR_Lock_UnlockRoutingPathByDestPdu(rmDestRomIdx);
    }
    PduR_SetEnabledOfRpgRam(rpgRomIdx, TRUE, memIdx);
  }
}

/**********************************************************************************************************************
 * PduR_RPG_DisableRouting
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
FUNC(void, PDUR_CODE) PduR_RPG_DisableRouting(PduR_RpgRomIterType rpgRomIdx, PduR_MemIdxType memIdx)
{
  if(PduR_IsEnabledOfRpgRam(rpgRomIdx, memIdx))
  {
    PduR_RmDestRpgRomIndIterType rmDestRpgRomIndIdx;
    for(rmDestRpgRomIndIdx = PduR_GetRmDestRpgRomIndStartIdxOfRpgRom(rpgRomIdx, memIdx); rmDestRpgRomIndIdx < PduR_GetRmDestRpgRomIndEndIdxOfRpgRom(rpgRomIdx, memIdx); rmDestRpgRomIndIdx++)  /* FETA_PDUR_01 */
    {
      PduR_RmDestRpgRomIterType rmDestRpgRomIdx = PduR_GetRmDestRpgRomInd(rmDestRpgRomIndIdx, memIdx);
      PduR_RmDestRomIterType rmDestRomIdx = PduR_GetRmDestRomIdxOfRmDestRpgRom(rmDestRpgRomIdx, memIdx);

      PduR_Lock_LockRoutingPathByDestPdu(rmDestRomIdx);
      {
#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
        if(PduR_IsIfOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)) && PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
        {
          PduR_RmIf_QueueProcessing_DisableRoutingAction(rmDestRomIdx);
        }
# endif
        if(PduR_IsIfOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)) && !PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && (PduR_GetDirectionOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)) == PDUR_TX_DIRECTIONOFRMGDESTROM))  /* COV_PDUR_UNQUEUED_ROUTING_ONLY_STATIC */
        {/* For unqueued If Tx destinations: */
          PduR_MultipleSourceHandler_Reset(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx));
        }
#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
        if(PduR_IsTpOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)) && PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
        {
          PduR_RmTp_TxInst_CancelTransmit(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx), PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)));
        }
#endif
        PduR_SetEnabledCntOfRmDestRpgRam(rmDestRpgRomIdx, (PduR_GetEnabledCntOfRmDestRpgRam(rmDestRpgRomIdx, memIdx) - 1u), memIdx);
      }
      PduR_Lock_UnlockRoutingPathByDestPdu(rmDestRomIdx);
    }
    PduR_SetEnabledOfRpgRam(rpgRomIdx, FALSE, memIdx);
  }
}

/**********************************************************************************************************************
 * PduR_RPG_IsDestPduEnabled
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RPG_IsDestPduEnabled(PduR_RmDestRomIterType rmDestRomIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  return (!PduR_IsRmDestRpgRomUsedOfRmDestRom(rmDestRomIdx)) || (0u != PduR_GetEnabledCntOfRmDestRpgRam(PduR_GetRmDestRpgRomIdxOfRmDestRom(rmDestRomIdx), PduR_GetRmDestRpgRomPartitionIdxOfRmDestRom(rmDestRomIdx)));
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RPG.c
 *********************************************************************************************************************/
