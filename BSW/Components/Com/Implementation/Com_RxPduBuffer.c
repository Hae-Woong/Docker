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
/*!        \file  Com_RxPduBuffer.c
 *         \unit  RxPduBuffer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_RXPDUBUFFER_SOURCE

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
#include "Com_RxPduBuffer.h"
#include "Com_Cfg_RxPduBuffer.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#include "vstdlib.h"

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
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxPduBuffer_CopyToRxDefPduBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduBuffer_CopyToRxDefPduBuffer(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  /* If the passed Rx ComIPdu is a NORMAL Pdu and no NULL_PTR, copy the passed Sdu data to the deferred Pdu buffer */
  if(Com_GetTypeOfRxPduInfo(comRxPduInfoIdx) == COM_NORMAL_TYPEOFRXPDUINFO)
  {
    if(PduInfoPtr->SduDataPtr != NULL_PTR)
    {
      if(Com_IsRxDefPduBufferUsedOfRxPduInfo(comRxPduInfoIdx))
      {
        const Com_RxDefPduBufferLengthOfRxPduInfoType numberOfBytes = ((Com_GetRxDefPduBufferLengthOfRxPduInfo(comRxPduInfoIdx)) > PduInfoPtr->SduLength) ? (Com_RxDefPduBufferLengthOfRxPduInfoType) (PduInfoPtr->SduLength) : Com_GetRxDefPduBufferLengthOfRxPduInfo(comRxPduInfoIdx);
        VStdLib_MemCpy_s(Com_GetAddrRxDefPduBuffer(Com_GetRxDefPduBufferStartIdxOfRxPduInfo(comRxPduInfoIdx)), Com_GetRxDefPduBufferLengthOfRxPduInfo(comRxPduInfoIdx), PduInfoPtr->SduDataPtr, numberOfBytes); /* VCA_COM_MEMCPY_CSL_DEST */    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
      }
    }
  }

  /* Store the passed Sdu length in the handle deferred buffer */
  /* mark the Pdu as stored by storing the SduLength + 1, because of ZeroBit Pdus */
  Com_SetHandleRxPduDeferred(Com_GetHandleRxPduDeferredIdxOfRxPduInfo(comRxPduInfoIdx), (PduInfoPtr->SduLength + 1u));
}

/**********************************************************************************************************************
  Com_RxPduBuffer_ResetLengthOfDeferredRxPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduBuffer_ResetLengthOfDeferredRxPdu(Com_RxPduInfoIterType comRxPduInfoIdx)
{
  if(Com_IsHandleRxPduDeferredUsedOfRxPduInfo(comRxPduInfoIdx))
  {
    const Com_HandleRxPduDeferredIterType handleDeferredIdx = Com_GetHandleRxPduDeferredIdxOfRxPduInfo(comRxPduInfoIdx);
    /*@ assert handleDeferredIdx < Com_GetSizeOfHandleRxPduDeferred(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_SetHandleRxPduDeferred(handleDeferredIdx, 0u);
  }
}

/**********************************************************************************************************************
  Com_RxPduBuffer_GetLengthOfDeferredRxPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxPduBuffer_GetLengthOfDeferredRxPdu(Com_RxPduInfoIterType comRxPduInfoIdx)
{
  PduLengthType retVal = 0u;
  if(Com_IsHandleRxPduDeferredUsedOfRxPduInfo(comRxPduInfoIdx))
  {
    retVal = Com_GetHandleRxPduDeferred(Com_GetHandleRxPduDeferredIdxOfRxPduInfo(comRxPduInfoIdx));
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_RxPduBuffer_GetAddrOfRxDefPduBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_RxDefPduBufferPtrType, COM_CODE) Com_RxPduBuffer_GetAddrOfRxDefPduBuffer(Com_RxPduInfoIterType comRxPduInfoIdx)
{
  Com_RxDefPduBufferPtrType rxDefPduBufferPtr = NULL_PTR;

  if(Com_IsRxDefPduBufferUsedOfRxPduInfo(comRxPduInfoIdx))
  {
    rxDefPduBufferPtr = Com_GetAddrRxDefPduBuffer(Com_GetRxDefPduBufferStartIdxOfRxPduInfo(comRxPduInfoIdx));
  }

  return rxDefPduBufferPtr;
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
 *  END OF FILE: Com_RxPduBuffer.c
 *********************************************************************************************************************/
