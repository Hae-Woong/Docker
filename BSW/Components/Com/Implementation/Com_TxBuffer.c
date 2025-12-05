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
/*!        \file  Com_TxBuffer.c
 *         \unit  TxBuffer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_TXBUFFER_SOURCE

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
#include "Com_TxBuffer.h"
#include "Com_Cfg_TxBuffer.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_DescriptionGatewayData.h"

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
  Com_TxBuffer_WriteInitValuesToTxBuffer
**********************************************************************************************************************/
/*! \brief        This function initializes the TxBuffer referred by the given index with init values.
    \details      -
    \pre          -
    \param[in]    txBufferInfoIdx Index of TxBufferInfo
    \param[in]    offset          offset to init value start idx
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxBuffer_WriteInitValuesToTxBuffer(Com_TxBufferInfoIterType txBufferInfoIdx);

/**********************************************************************************************************************
  Com_TxBuffer_SetUpdateBit
**********************************************************************************************************************/
/*! \brief        This method sets the update bit in the TxBuffer at the position referred by the given TxBufferInfoIdx..
    \details      -
    \pre          -
    \param[in]    txBufferInfoIdx    ID of TxBufferInfo.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
***********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxBuffer_SetUpdateBit(Com_TxBufferInfoIterType txBufferInfoIdx);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxBuffer_WriteInitValuesToTxBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxBuffer_WriteInitValuesToTxBuffer(Com_TxBufferInfoIterType txBufferInfoIdx)
{
  if(Com_IsTxPduInitValueUsedOfTxBufferInfo(txBufferInfoIdx)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    /*@ assert txBufferInfoIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_CSL03 */
    CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) pTxPduBuffer = Com_TxBuffer_GetAddrTxBufferFirstElement(txBufferInfoIdx);
    CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) pInitValues = Com_GetAddrTxPduInitValue(Com_GetTxPduInitValueStartIdxOfTxBufferInfo(txBufferInfoIdx));
    VStdLib_MemCpy_s(pTxPduBuffer, Com_GetTxBufferLengthOfTxBufferInfo(txBufferInfoIdx), pInitValues, Com_GetTxPduInitValueLengthOfTxBufferInfo(txBufferInfoIdx)); /* VCA_COM_MEMCPY_CSL_DEST */    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
  }
}

/**********************************************************************************************************************
  Com_TxBuffer_SetUpdateBit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxBuffer_SetUpdateBit(Com_TxBufferInfoIterType txBufferInfoIdx)
{
  if(Com_IsUbMaskUsedOfTxBufferInfo(txBufferInfoIdx))  /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    const Com_UbMaskIterType ubMaskIdx = Com_GetUbMaskStartIdxOfTxBufferInfo(txBufferInfoIdx);
    const Com_TxBufferIterType txBufferIdx = Com_GetTxBufferStartIdxOfTxBufferInfo(txBufferInfoIdx);

    /*@ assert txBufferIdx <  Com_GetSizeOfTxBuffer(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_SetTxBuffer(txBufferIdx, (Com_GetTxBuffer(txBufferIdx) | Com_GetUbMask(ubMaskIdx)));
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxBuffer_InitTxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_InitTxIpdu(Com_TxPduInfoIterType ComPduId)
{
  if(Com_IsTxBufferInfoUsedOfTxPduInfo(ComPduId))
  {
    const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfTxPduInfo(ComPduId);
    Com_TxBuffer_WriteInitValuesToTxBuffer(txBufferInfoIdx);
  }

  Com_TxBuffer_CpyInitMetaDataToBffr(ComPduId);
}

/**********************************************************************************************************************
  Com_TxBuffer_InitShdwBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_InitShdwBuffer(Com_TxSigGrpInfoIterType sigGrpHnd)
{
  if(Com_IsTxBufferInfoUsedOfTxSigGrpInfo(sigGrpHnd))
  {
    const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfTxSigGrpInfo(sigGrpHnd);
    Com_TxBuffer_WriteInitValuesToTxBuffer(txBufferInfoIdx);
  }
}

/**********************************************************************************************************************
Com_TxBuffer_CpyInitMetaDataToBffr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *

**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_CpyInitMetaDataToBffr(Com_TxPduInfoIterType ComTxPduId)
{
  if(Com_IsTxBufferInfoMetaDataUsedOfTxPduInfo(ComTxPduId))
  {
    const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoMetaDataIdxOfTxPduInfo(ComTxPduId);
    Com_TxBuffer_WriteInitValuesToTxBuffer(txBufferInfoIdx); /*@ assert txBufferInfoIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_CSL03 */
  }
}

/**********************************************************************************************************************
  Com_TxBuffer_ClearUpdateBits
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_ClearUpdateBits(Com_TxPduInfoIterType ComTxPduId)
{
  if(Com_IsTxBufferInfoUsedOfTxPduInfo(ComTxPduId))
  {
    const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfTxPduInfo(ComTxPduId);
    Com_TxBufferIterType txBufferIdx = Com_GetTxBufferStartIdxOfTxBufferInfo(txBufferInfoIdx);  /* PRQA S 2983 */ /* MD_Com_2982_2983 */
    for(Com_UbMaskIterType ubMaskIdx = Com_GetUbMaskStartIdxOfTxBufferInfo(txBufferInfoIdx);
        ubMaskIdx < Com_GetUbMaskEndIdxOfTxBufferInfo(txBufferInfoIdx); ubMaskIdx++)  /* FETA_COM_01 */
    {
      if(txBufferIdx < Com_GetTxBufferEndIdxOfTxBufferInfo(txBufferInfoIdx))  /* COV_COM_UBMASK_CHECK */
      {
        /*@ assert txBufferIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        Com_SetTxBuffer(txBufferIdx, Com_GetTxBuffer(txBufferIdx) & Com_GetUbMask(ubMaskIdx));
        txBufferIdx++;
      }
    }
  }
}

/**********************************************************************************************************************
  Com_TxBuffer_SetUpdateBitSignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_SetUpdateBitSignalGroup(Com_TxSigGrpInfoIterType idxTxSigGrpInfo)
{
  if(Com_IsTxBufferInfoUbIdxAndMaskUsedOfTxSigGrpInfo(idxTxSigGrpInfo))
  {
    Com_TxBuffer_SetUpdateBit(Com_GetTxBufferInfoUbIdxAndMaskIdxOfTxSigGrpInfo(idxTxSigGrpInfo));
  }
}

/**********************************************************************************************************************
  Com_TxBuffer_SetUpdateBitSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_SetUpdateBitSignal(Com_TxSigInfoIterType idxTxSigInfo)
{
  if(Com_IsTxBufferInfoUbIdxAndMaskUsedOfTxSigInfo(idxTxSigInfo))
  {
    Com_TxBuffer_SetUpdateBit(Com_GetTxBufferInfoUbIdxAndMaskIdxOfTxSigInfo(idxTxSigInfo));
  }
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_TxBuffer_SetUpdateBitGwDescriptionAccess
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_SetUpdateBitGwDescriptionAccess(Com_GwDescriptionAccessInfoIterType idxGwDescriptionAccessInfo)
{
  if(Com_IsTxBufferInfoUbIdxAndMaskUsedOfGwDescriptionAccessInfo(idxGwDescriptionAccessInfo))
  {
    Com_TxBuffer_SetUpdateBit(Com_GetTxBufferInfoUbIdxAndMaskIdxOfGwDescriptionAccessInfo(idxGwDescriptionAccessInfo));
  }
}
#endif

/**********************************************************************************************************************
  Com_TxBuffer_WriteSignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_WriteSignalGroup(Com_TxSigGrpInfoIterType SignalGroupId, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) ShadowBufferPtr)
{
  if(ShadowBufferPtr != NULL_PTR)
  {
    if(Com_IsTxBufferInfoSigGrpInTxIPDUUsedOfTxSigGrpInfo(SignalGroupId))
    {
      const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoSigGrpInTxIPDUIdxOfTxSigGrpInfo(SignalGroupId);
      if(Com_IsTxSigGrpMaskUsedOfTxBufferInfo(txBufferInfoIdx)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
      {
        Com_TxBufferIterType i = 0u;
        CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) maskPtr = Com_GetAddrTxSigGrpMask(Com_GetTxSigGrpMaskStartIdxOfTxBufferInfo(txBufferInfoIdx));
        for(Com_TxBufferIterType txBufferIdx = Com_GetTxBufferStartIdxOfTxBufferInfo(txBufferInfoIdx);
            txBufferIdx < Com_GetTxBufferEndIdxOfTxBufferInfo(txBufferInfoIdx); txBufferIdx++)  /* FETA_COM_01 */
        {
          /*@ assert txBufferIdx <  Com_GetSizeOfTxBuffer(); */ /* VCA_COM_CSL03 */
          Com_SetTxBuffer(txBufferIdx, (Com_GetTxBuffer(txBufferIdx) & COM_CAST2UINT8(~maskPtr[i])) | (ShadowBufferPtr[i] & maskPtr[i]));
          i++;
        }
      }
    }
  }
}

/**********************************************************************************************************************
  Com_TxBuffer_GetAddrTxBufferAtIdx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_TxBufferPtrType, COM_CODE) Com_TxBuffer_GetAddrTxBufferAtIdx(Com_TxBufferInfoIterType txBufferInfoIdx, PduLengthType offset)
{
  return Com_GetAddrTxBuffer((Com_TxBufferIterType) (Com_GetTxBufferStartIdxOfTxBufferInfo(txBufferInfoIdx) + (Com_TxBufferIterType) offset));
}

/**********************************************************************************************************************
  Com_TxBuffer_GetAddrTxBufferFirstElement
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_TxBufferPtrType, COM_CODE) Com_TxBuffer_GetAddrTxBufferFirstElement(Com_TxBufferInfoIterType txBufferInfoIdx)
{
  return Com_GetAddrTxBuffer(Com_GetTxBufferStartIdxOfTxBufferInfo(txBufferInfoIdx));
}

/**********************************************************************************************************************
  Com_TxBuffer_GetTxBufferLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_TxBufferLengthOfTxBufferInfoType, COM_CODE) Com_TxBuffer_GetTxBufferLength(Com_TxBufferInfoIterType txBufferInfoIdx)
{
  return Com_GetTxBufferLengthOfTxBufferInfo(txBufferInfoIdx);
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
 *  END OF FILE: Com_TxBuffer.c
 *********************************************************************************************************************/
