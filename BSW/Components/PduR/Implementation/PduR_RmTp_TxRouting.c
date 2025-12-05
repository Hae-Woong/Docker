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
 *         \file  PduR_RmTp_TxRouting.c
 *         \unit  RmTp_TxRouting
 *        \brief  Pdu Router Routing Manager Transport Protocol Tx Routing source file
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
/* PRQA  S 3673 EOF */ /* MD_MSR_Rule8.13 */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_RMTP_TXROUTING_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Mm.h"
#include "PduR_RmTp_TxRouting.h"
#include "PduR_RmTp_TxInst.h"
#include "PduR_RPG.h"

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
 * PduR_RmTp_TxRouting_DispatchCopyTxData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_TxRouting_DispatchCopyTxData(PduR_RmDestRomIterType rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                              P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(PduR_RPG_IsDestPduEnabled(rmDestRomIdx) == TRUE)
  {
# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
    if(PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
    {
      if((retry == NULL_PTR) || (retry->TpDataState != TP_DATARETRY))
      {
        retVal = PduR_RmTp_TxInst_CopyTxData(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx), info, retry, availableDataPtr);
      }
    }
    else
# endif
    {
      PduR_RmSrcRomIdxOfRmDestRomType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);
      retVal = PduR_Mm_CallTpCopyTxData(PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx), info, retry, availableDataPtr);
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_RmTp_TxRouting_DispatchTpTxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_TxRouting_DispatchTpTxConfirmation(PduR_RmDestRomIterType rmDestRomIdx, Std_ReturnType result)
{
# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  if(PduR_IsQueuedOfRmDestRom(rmDestRomIdx) && PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)))  /* COV_PDUR_LAST_COND_ALWAYS_TRUE */
  {
    PduR_RmTp_TxInst_TxConfirmation(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx), result);
  }
  else
# endif
  {
    PduR_Mm_CallTpTxConfirmation(PduR_GetMmRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)), PduR_GetSrcHndOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)), result);
  }
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp_TxRouting.c
 *********************************************************************************************************************/
