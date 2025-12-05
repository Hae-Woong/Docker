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
 *         \file  PduR_RmIf_DeferredProcessing.c
 *         \unit  RmIf_DeferredProcessing
 *        \brief  Pdu Router Routing Manager Interface Deferred Processing source file
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

#define PDUR_RMIF_DEFERREDPROCESSING_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Lock.h"
#include "PduR_RmIf_DeferredProcessing.h"
#include "PduR_RmIf_QueueProcessing.h"
#include "PduR_EvC.h"

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
#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_DeferredProcessing_Init
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_DeferredProcessing_Init(PduR_MemIdxType memIdx)
{
  PduR_RmBufferedIfPropertiesRomIterType rmBufferedIfPropertiesRamIdx;
  for(rmBufferedIfPropertiesRamIdx = 0u; rmBufferedIfPropertiesRamIdx < PduR_GetSizeOfRmBufferedIfPropertiesRam(memIdx); rmBufferedIfPropertiesRamIdx++)  /* FETA_PDUR_01 */
  {
    PduR_SetIfSmStateOfRmBufferedIfPropertiesRam(rmBufferedIfPropertiesRamIdx, PDUR_RM_TX_IDLE_IFSMSTATEOFRMBUFFEREDIFPROPERTIESRAM, memIdx);
  }
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_DeferredProcessing_IsDeferredTransmissionTriggered
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmIf_DeferredProcessing_IsDeferredTransmissionTriggered(PduR_RmGDestRomIterType rmGDestRomIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
  boolean retVal = FALSE;
  PduR_RmBufferedIfPropertiesRomIdxOfRmGDestRomType rmBufferedIfPropertiesRomIdx = PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx);
  PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

  if(PduR_GetIfSmStateOfRmBufferedIfPropertiesRam(rmBufferedIfPropertiesRomIdx, memIdx) == PDUR_RM_TX_IDLE_IFSMSTATEOFRMBUFFEREDIFPROPERTIESRAM)
  {
    retVal = FALSE;
  }
  else
  {
    retVal = TRUE;
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_DeferredProcessing_Transmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_DeferredProcessing_Transmit(PduR_RmDestRomIterType rmDestRomIdx, uint8 apiId)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_RmGDestRomIterType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);

  switch (PduR_GetPduRDestPduProcessingOfRmGDestRom(rmGDestRomIdx))
  {
    case PDUR_IMMEDIATE_PDURDESTPDUPROCESSINGOFRMGDESTROM:
    {
      if(PduR_GetDirectionOfRmGDestRom(rmGDestRomIdx) == PDUR_TX_DIRECTIONOFRMGDESTROM)
      {
        retVal = PduR_RmIf_QueueProcessing_TransmitImmediately(rmGDestRomIdx, apiId);
      }
      else
      {
        /* This situation can never occur. Queued Rx routing paths can only be processed deferred. */
        retVal = E_NOT_OK;
      }
      break;
    }
    case PDUR_DEFERRED_PDURDESTPDUPROCESSINGOFRMGDESTROM:
    {
      PduR_SetIfSmStateOfRmBufferedIfPropertiesRam(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx), PDUR_RM_TX_WAITING_IFSMSTATEOFRMBUFFEREDIFPROPERTIESRAM, PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx));
# if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)
      (void) PduR_EvC_WriteDeferredEventCache(rmGDestRomIdx, PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)));
# endif
      retVal = E_OK;
      break;
    }
    default:       /* COV_PDUR_MISRA */
      /* Nothing to do: This should never be reached */
      break;
  }

  return retVal;
}
#endif

#if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_DeferredProcessing_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_DeferredProcessing_MainFunction(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_Lock_LockRoutingPathByGDestPdu(rmGDestRomIdx);
  {
    {
      PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

      if(PduR_GetIfSmStateOfRmBufferedIfPropertiesRam(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx), memIdx) == PDUR_RM_TX_WAITING_IFSMSTATEOFRMBUFFEREDIFPROPERTIESRAM)
      {
        PduR_SetIfSmStateOfRmBufferedIfPropertiesRam(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx), PDUR_RM_TX_IDLE_IFSMSTATEOFRMBUFFEREDIFPROPERTIESRAM, memIdx);

        if(PduR_GetDirectionOfRmGDestRom(rmGDestRomIdx) == PDUR_TX_DIRECTIONOFRMGDESTROM)
        {
          (void) PduR_RmIf_QueueProcessing_TransmitImmediately(rmGDestRomIdx, PDUR_FCT_MAINFUNCTION_TX);
        }
        else
        {
          PduR_RmIf_QueueProcessing_MainFunctionRx(rmGDestRomIdx);
        }
      }
    }
  }
  PduR_Lock_UnlockRoutingPathByGDestPdu(rmGDestRomIdx);
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RmIf_DeferredProcessing.c
 *********************************************************************************************************************/
