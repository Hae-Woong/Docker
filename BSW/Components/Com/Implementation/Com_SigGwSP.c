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
/*!        \file  Com_SigGwSP.c
 *         \unit  SigGwSP
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_SIGGWSP_SOURCE

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
#include "Com.h"
#include "Com_SigGwSP.h"
#include "Com_Cfg_SignalGatewayData.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_GwTout.h"
#include "Com_RxSigBuffer.h"
#include "Com_Transmission.h"

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
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
  Com_SigGwSP_SignalGroupRouting
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwSP_SignalGroupRouting(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping)
{
  const Com_TxSigGrpInfoIterType txSigGrpId = Com_GetTxSigGrpInfoIdxOfGwSigGrpMapping(idxGwSigGrpMapping);

  /* Handle Gw Timeout if configured */
  Com_GwTout_Event(Com_GetTxPduInfoIdxOfTxSigGrpInfo(txSigGrpId));

  /* Iterate over all related group signal mappings */
  for(Com_GwGrpSigMappingIterType idxGwGrpSigMapping = Com_GetGwGrpSigMappingStartIdxOfGwSigGrpMapping(idxGwSigGrpMapping);
      idxGwGrpSigMapping < Com_GetGwGrpSigMappingEndIdxOfGwSigGrpMapping(idxGwSigGrpMapping); idxGwGrpSigMapping++)  /* FETA_COM_01 */
  {
    /* Get the pointer to the source buffer and length of signal */
    const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
    CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetGatewaySourcePtr(idxRxAccessInfo);
    const PduLengthType constDataLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
    const ComDataTypeConst data = { constDataPtr, constDataLength };

    /* Process GroupSignal */
    const Com_TxGrpSigInfoIterType txSigId = Com_GetTxGrpSigInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
    const Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxGrpSigInfo(txSigId);
    (void) Com_Transmission_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
  }
  /* Call Com_SendSignalGroup for the related signal group with the respective shadowBuffer */
  P2CONST(uint8, AUTOMATIC, AUTOMATIC) shdBuffer = NULL_PTR;
  if(Com_IsTxBufferInfoUsedOfTxSigGrpInfo(txSigGrpId))
  {
    shdBuffer = Com_Transmission_GetAddrTxBufferFirstElement(Com_GetTxBufferInfoIdxOfTxSigGrpInfo(txSigGrpId));
  }
  (void) Com_Transmission_SendSignalGroup_Processing(txSigGrpId, shdBuffer);
}

/**********************************************************************************************************************
  Com_SigGwSP_SignalRouting
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwSP_SignalRouting(Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping)
{
  const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfGwSigMapping(idxGwSigMapping);
  const Com_TxSigInfoIterType txSigId = Com_GetTxSigInfoIdxOfGwSigMapping(idxGwSigMapping);
  const Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxSigInfo(txSigId);

  /* Get the pointer to the source buffer and length of signal */
  CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetGatewaySourcePtr(idxRxAccessInfo);
  const PduLengthType constDataLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
  const ComDataTypeConst data = { constDataPtr, constDataLength };

  /* Handle Gw Timeout if configured */
  Com_GwTout_Event(Com_GetTxPduInfoIdxOfTxSigInfo(txSigId));

  /* Process Signal */
  (void) Com_Transmission_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
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
 *  END OF FILE: Com_SigGwSP.c
 *********************************************************************************************************************/
