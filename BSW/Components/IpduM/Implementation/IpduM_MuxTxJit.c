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
 *         \file  IpduM_MuxTxJit.c
 *         \unit  MuxTxJit
 *        \brief  Microsar IpduM Multiplexer Tx Just in Time update source file
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

#define IPDUM_MUXTXJIT_SOURCE
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
#include "IpduM_MuxTxJit.h"
#include "IpduM_MuxUtil.h"
#include "PduR_IpduM.h"
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
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_MuxTx_Jit_UpdatePart
*******************************************************************************************/
/*! \brief          Updates the data of the txPartId via trigger transmit
 *  \details        -
 *  \param[in]      txPartId          id of the part
 *  \param[in]      txPathwayId       tx pathway handle
 *  \param[in,out]  pduInfoPtr        pointer and length of the provided buffer and length of the copied data on return
 *  \return         E_OK              the trigger transmit call has been successful and data has been updated.
 *  \return         E_NOT_OK          the trigger transmit call has been rejected by the upper layer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different tx Pathway IDs
 *  \synchronous    TRUE
 *  \spec
 *    requires txPathwayId < IpduM_GetSizeOfCTxPathway();
 *    requires txPartId < IpduM_GetSizeOfTxPart();
 *  \endspec
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Jit_UpdatePart(IpduM_TxPartIterType txPartId, IpduM_CTxPathwayIterType txPathwayId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) pduInfoPtr);

/*******************************************************************************************
 *  IpduM_MuxTx_Jit_TriggerTransmit()
*******************************************************************************************/
/*! \brief          Updates parts via Trigger Transmit.
 *  \details        -
 *  \param[in]      PdumTxPduId     upper layer trigger transmit handle
 *  \param[in]      txPathwayId     tx pathway handle
 *  \param[in]      txPartId        tx part handle
 *  \param[in,out]  pduInfoPtr      pointer and length of the provided buffer and length of the copied data on return
 *  \return         E_OK            the trigger transmit call has been successful and data has been updated.
 *  \return         E_NOT_OK        the trigger transmit call has been rejected by the upper layer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different tx Pathway IDs
 *  \synchronous    TRUE
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Jit_TriggerTransmit(PduIdType PdumTxPduId, IpduM_CTxPathwayIterType txPathwayId, IpduM_TxPartIterType txPartId,
                                                                       P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) pduInfoPtr);

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_MuxTx_Jit_UpdatePart
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
IPDUM_LOCAL_INLINE FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Jit_UpdatePart(IpduM_TxPartIterType txPartId, IpduM_CTxPathwayIterType txPathwayId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) pduInfoPtr)
{
  Std_ReturnType retVal = E_OK;

  if(IpduM_IsJitUpdateOfTxPart(txPartId))
  {
    retVal = IpduM_MuxTx_Jit_TriggerTransmit(IpduM_GetUlPduRefOfTxPart(txPartId), txPathwayId, txPartId, pduInfoPtr);
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_MuxTx_Jit_TriggerTransmit()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
IPDUM_LOCAL_INLINE FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Jit_TriggerTransmit(PduIdType PdumTxPduId, IpduM_CTxPathwayIterType txPathwayId, IpduM_TxPartIterType txPartId,
                                                                       P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) pduInfoPtr)
{
  Std_ReturnType retVal = E_OK;
  if(PduR_IpduMTriggerTransmit(PdumTxPduId, pduInfoPtr) == E_OK)
  {
    IpduM_SegmentIterType segmentIdx = IpduM_GetSegmentStartIdxOfTxPart(txPartId);
    PduInfoType segmentDestPduInfo;
    segmentDestPduInfo = IpduM_MuxTx_GetTxBufferOfTxPathway(txPathwayId);

    SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
    for(; segmentIdx < IpduM_GetSegmentEndIdxOfTxPart(txPartId); segmentIdx++)  /* FETA_IPDUM_01 */
    {
      IpduM_MuxUtil_CopySegment(segmentIdx, &segmentDestPduInfo, pduInfoPtr->SduDataPtr);
    }
    SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_MuxTx_Jit_UpdateCounterPart
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Jit_UpdateCounterPart(IpduM_CTxPathwayIterType txPathwayId, IpduM_TxUpIndIdxOfTxUpInfoType txPartId)
{
  Std_ReturnType retVal = E_OK;
  PduInfoType jitUpdatePduInfo;
  jitUpdatePduInfo.SduDataPtr = IpduM_GetAddrJitUpdateBuffer(IpduM_GetJitUpdateBufferStartIdxOfTxPathway(txPathwayId));
  jitUpdatePduInfo.SduLength = IpduM_GetJitUpdateBufferLengthOfTxPathway(txPathwayId);

  if(IpduM_IsTxPartStaticPartUsedOfTxPathway(txPathwayId))
  {
    if(txPartId == IpduM_GetTxPartStaticPartIdxOfTxPathway(txPathwayId))
    {
      IpduM_TxPartIterType txDynamicPartId = IpduM_GetTxPartDynamicPartIdxOfTxPathway(txPathwayId);
      /*@ assert (txDynamicPartId  < IpduM_GetSizeOfTxPart()); */  /* VCA_IPDUM_DYNAMICPARTIDX */
      retVal = IpduM_MuxTx_Jit_UpdatePart(txDynamicPartId, txPathwayId, &jitUpdatePduInfo);
    }
    else
    {
      retVal = IpduM_MuxTx_Jit_UpdatePart(IpduM_GetTxPartStaticPartIdxOfTxPathway(txPathwayId), txPathwayId, &jitUpdatePduInfo);
    }
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_MuxTx_Jit_UpdateAll
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_MuxTx_Jit_UpdateAll(IpduM_CTxPathwayIterType txPathwayId)
{
  Std_ReturnType retVal = E_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduInfoType jitUpdatePduInfo;
  jitUpdatePduInfo.SduDataPtr = IpduM_GetAddrTriggerTransmitBuffer(IpduM_GetTriggerTransmitBufferStartIdxOfTxPathway(txPathwayId));
  jitUpdatePduInfo.SduLength = IpduM_GetTriggerTransmitBufferLengthOfTxPathway(txPathwayId);

  if(IpduM_IsTxPartStaticPartUsedOfTxPathway(txPathwayId))
  {
    retVal = IpduM_MuxTx_Jit_UpdatePart(IpduM_GetTxPartStaticPartIdxOfTxPathway(txPathwayId), txPathwayId, &jitUpdatePduInfo);
  }

  if(retVal == E_OK)
  {
    IpduM_TxPartIterType txDynamicPartId = IpduM_GetTxPartDynamicPartIdxOfTxPathway(txPathwayId);
    /*@ assert (txDynamicPartId  < IpduM_GetSizeOfTxPart()); */  /* VCA_IPDUM_DYNAMICPARTIDX */
    retVal = IpduM_MuxTx_Jit_UpdatePart(txDynamicPartId, txPathwayId, &jitUpdatePduInfo);
  }

  return retVal;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_MuxTxJit.c
**********************************************************************************************************************/
