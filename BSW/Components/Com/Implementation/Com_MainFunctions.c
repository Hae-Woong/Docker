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
/*!        \file  Com_MainFunctions.c
 *         \unit  MainFunctions
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_MAINFUNCTIONS_SOURCE

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
#include "Com_MainFunctions.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_Repetition.h"

#include "Com_Initialization.h"
#include "Com_RxDlMon.h"
#include "Com_Notifications.h"
#include "Com_RxPduProcessing.h"
#include "Com_DesGw.h"
#include "Com_SigGw.h"
#include "Com_TxDlMon.h"
#include "Com_GwTout.h"
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
  GLOBAL DATA PROTOTYPES
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

/**********************************************************************************************************************
  Com_MainFunctionRxInternal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_MainFunctionRxInternal(Com_MainFunctionRxStructIterType mainFunctionId)
{
  /* Check if COM is initialized */
  if(Com_Initialization_GetInitialized())
  {
    if(mainFunctionId < Com_GetSizeOfMainFunctionRxStruct())
    {
      /* Do deferred processing */
      Com_RxPduProcessing_MainFunctionRx(mainFunctionId);

      /* Do Rx deadline monitoring */
      Com_RxDlMon_MainFunctionRx(mainFunctionId);
    }
  }
}

/**********************************************************************************************************************
  Com_MainFunctionTxInternal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
***********************************************************************************************************************/
FUNC(void, COM_CODE) Com_MainFunctionTxInternal(Com_MainFunctionTxStructIterType mainFunctionId)
{
  /* Check if COM is initialized */
  if(Com_Initialization_GetInitialized())
  {
    if(mainFunctionId < Com_GetSizeOfMainFunctionTxStruct())
    {
      /* Do TxMode processing */
      if(Com_HasTxProcessingPdus()) /* COV_COM_TXPROCESS_ALWAYS_TRUE_FOR_CFG_WITH_CYCLIC_TXIPDUS */
      {
        Com_Transmission_MainFunctionProcessing(mainFunctionId);
      }

      /* Do Tx deadline monitoring, if configured */
      Com_TxDlMon_MainFunctionTx(mainFunctionId);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
      /* Handle GwTimeout, if configured */
      Com_GwTout_MainFunctionTx(mainFunctionId);
#endif

      /* Process transmit request and initiate transition */
      Com_Transmission_MainFunctionTxTransmit(mainFunctionId);

      /* Process deferred Tx Confirmations */
      Com_Notifications_MainFunctionTx(mainFunctionId);
    }
  }
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_MainFunctionRouteSignalsInternal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_MainFunctionRouteSignalsInternal(Com_MainFunctionRouteSignalsStructIterType mainFunctionId)
{
  /* Check if COM is initialized */
  if(Com_Initialization_GetInitialized())
  {
    if(mainFunctionId < Com_GetSizeOfMainFunctionRouteSignalsStruct())
    {
      /* Do description routing */
      if(Com_IsEventCacheGatewayEventCacheUsedOfMainFunctionRouteSignalsStruct(mainFunctionId) ||
          (Com_IsPartitionsTxStructUsedOfMainFunctionRouteSignalsStruct(mainFunctionId) &&
          Com_IsCprQueueDescriptionRoutingIndUsedOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId))))
      {
        Com_DesGw_MainFunction(mainFunctionId);
      }

      /* Do signal routing */
      Com_SigGw_MainFunction(mainFunctionId);
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
 *  END OF FILE: Com_MainFunctions.c
 *********************************************************************************************************************/
