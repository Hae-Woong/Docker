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
 *         \file  IpduM_ContainerRxProcessing.c
 *         \unit  ContainerRxProcessing
 *        \brief  Microsar IpduM ContainerRxProcessing source file
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

#define IPDUM_CONTAINERRXPROCESSING_SOURCE
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
#include "IpduM_ContainerRxProcessing.h"
#include "IpduM_ContainerUtil.h"
#include "IpduM_Reporting.h"
#include "PduR_IpduM.h"
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
 *  IpduM_ContainerRxProcessing_UnpackContainerPdu_DynamicLayout()
*******************************************************************************************/
/*! \brief        Retrieves all contained PDUs from a dynamic layout container PDU and sends RxIndications where configured.
 *  \details      -
 *  \param[in]    rxContainerId
 *  \param[in]    dataPtr        Pointer to the received PDU data.
 *  \param[in]    pduLength      Length of received rx container
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(void, IPDUM_CODE) IpduM_ContainerRxProcessing_UnpackContainerPdu_DynamicLayout(IpduM_RxContainerPduIterType rxContainerId, P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, PduLengthType pduLength);

/*******************************************************************************************
 *  IpduM_ContainerRxProcessing_UnpackContainerPdu_StaticLayout()
*******************************************************************************************/
/*! \brief        Retrieves all contained PDUs from a static layout container PDU and sends RxIndications where configured.
 *  \details      -
 *  \param[in]    rxContainerId
 *  \param[in]    dataPtr        Pointer to the received PDU data.
 *  \param[in]    pduLength      Length of received rx container
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(void, IPDUM_CODE) IpduM_ContainerRxProcessing_UnpackContainerPdu_StaticLayout(IpduM_RxContainerPduIterType rxContainerId, P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, PduLengthType pduLength);

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerRxProcessing_UnpackContainerPdu_DynamicLayout()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
IPDUM_LOCAL_INLINE FUNC(void, IPDUM_CODE) IpduM_ContainerRxProcessing_UnpackContainerPdu_DynamicLayout(IpduM_RxContainerPduIterType rxContainerId, P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, PduLengthType pduLength) /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
  uint32 nextContainedPduByteOffset = 0u; /* start of the next containedPdu (header + data) in the container */
  uint8 headerSize = IpduM_GetHeaderSizeOfRxContainerPdu(rxContainerId);
  uint8 metaDataSize = IpduM_GetMetaDataSizeOfRxContainerPdu(rxContainerId);
  uint8 metaData[IPDUM_MAX_METADATA_SIZE] = {0, 0, 0, 0}; /* local buffer for meta data */
  IpduM_RxContainerBufferPtrType unpackBuffer = IpduM_GetAddrRxContainerBuffer(rxContainerId); /* buffer for unpacking PDU */

  if(metaDataSize > 0u)
  {
    /* copy from dataPtr to metaData */
    VStdLib_MemCpy_s(metaData, IPDUM_MAX_METADATA_SIZE, &(dataPtr[(pduLength - metaDataSize)]), metaDataSize); /* VCA_IPDUM_MEMCPY_8 */
    /* overwrite metaData in dataPtr with 0, so normal unpacking works properly */
    VStdLib_MemSet(&(dataPtr[(pduLength - metaDataSize)]), 0u, metaDataSize); /* VCA_IPDUM_MEMSET_1 */
  }

  boolean ableToReadNextHeader = /* nextContainedPduByteOffset (0) + */ headerSize <= pduLength;

  while(ableToReadNextHeader) /* FETA_IPDUM_05 */ /* PRQA S 0771 */ /* MD_IpduM_0771 */
  {
    /* get header id and dlc of the contained PDU */
    IpduM_ContainedPduHeaderType header = IpduM_ContainerUtil_ReadHeader(&(dataPtr[nextContainedPduByteOffset]), headerSize);

    /* zero header marks the end of the container Pdu */
    if(header.headerId == 0u)
    {
      break;
    }

    /* check if the read DLC does even fit in into the whole Pdu. */
    boolean readContainedPduFitsIntoBuffer = header.dlc <= (pduLength - nextContainedPduByteOffset - headerSize);
    if(readContainedPduFitsIntoBuffer)
    {
      /* look up the header id in IpduM_RxContainedPdu */
      IpduM_RxContainedPduIterType rxContainedPduId = 0u;

      if(IpduM_ContainerUtil_SearchContainedPduIdByHeaderId(rxContainerId, header.headerId, &rxContainedPduId) == E_OK)
      {
        /* check if either "accept all" is configured or PDU is configured for this container */
        if((IpduM_IsRxContainerPduUsedOfRxContainedPdu(rxContainedPduId) && (IpduM_GetRxContainerPduIdxOfRxContainedPdu(rxContainedPduId) == rxContainerId))
          || IpduM_IsAcceptAllPdusOfRxContainerPdu(rxContainerId)
        )
        {
          PduInfoType dynamicPduInfo;
          dynamicPduInfo.SduDataPtr = &(dataPtr[nextContainedPduByteOffset + headerSize]);
          dynamicPduInfo.SduLength = (PduLengthType) header.dlc;
          /* for meta data PDUs, use generated buffer for assembling of PDU with meta data before transmission */
          if(metaDataSize > 0u)
          {
            /* copy data to buffer */
            VStdLib_MemCpy_s(&(unpackBuffer[0]), (PduLengthType)(pduLength - nextContainedPduByteOffset - headerSize), &(dataPtr[nextContainedPduByteOffset + headerSize]), header.dlc); /* VCA_IPDUM_MEMCPY_9 */
            /* copy meta data to buffer */
            VStdLib_MemCpy_s(&(unpackBuffer[header.dlc]), IPDUM_MAX_METADATA_SIZE, metaData, metaDataSize); /* VCA_IPDUM_MEMCPY_10 */
            dynamicPduInfo.SduDataPtr = unpackBuffer;
            dynamicPduInfo.SduLength = (PduLengthType) (header.dlc + metaDataSize);
          }

          PduR_IpduMRxIndication(IpduM_GetContainedPduRefOfRxContainedPdu(rxContainedPduId), &dynamicPduInfo);
        }
      }

      nextContainedPduByteOffset += headerSize + header.dlc;
      ableToReadNextHeader = (nextContainedPduByteOffset + headerSize) <= pduLength;
    }
    else
    {
      IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERRX_UNPACKCONTAINERPDU_DYNAMICLAYOUT, IPDUM_E_HEADER);
      break;
    }
  }
}

/*******************************************************************************************
 *  IpduM_ContainerRxProcessing_UnpackContainerPdu_StaticLayout()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
IPDUM_LOCAL_INLINE FUNC(void, IPDUM_CODE) IpduM_ContainerRxProcessing_UnpackContainerPdu_StaticLayout(IpduM_RxContainerPduIterType rxContainerId, P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, PduLengthType pduLength)
{
  IpduM_RxContainedPduIndEndIdxOfRxContainerPduType containedPduIndIdx;

  /* PDUs with static layout and no header */
  for(containedPduIndIdx = IpduM_GetRxContainedPduIndStartIdxOfRxContainerPdu(rxContainerId); containedPduIndIdx < IpduM_GetRxContainedPduIndEndIdxOfRxContainerPdu(rxContainerId); containedPduIndIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_RxContainedPduIterType rxContainedPduIdx = IpduM_GetRxContainedPduInd(containedPduIndIdx);

    /* Check Update Bit */
    PduLengthType updateBitBytePos;
    uint8 updateBitMask;
    updateBitBytePos = IpduM_GetUpdateIndicationBytePositionOfRxContainedPdu(rxContainedPduIdx);
    updateBitMask = (uint8) (0x01u << IpduM_GetUpdateIndicationBitInBytePositionOfRxContainedPdu(rxContainedPduIdx));
    /* if (update bits used AND update bit is set) OR (update bits not used) */
    if((IpduM_IsUpdateBitUsedOfRxContainedPdu(rxContainedPduIdx) && ((dataPtr[updateBitBytePos] & updateBitMask) != 0x00u)) || !IpduM_IsUpdateBitUsedOfRxContainedPdu(rxContainedPduIdx))
    {
      PduInfoType staticPduInfo;
      staticPduInfo.SduDataPtr = &(dataPtr[IpduM_GetOffsetOfRxContainedPdu(rxContainedPduIdx)]);
      staticPduInfo.SduLength = IpduM_GetLengthOfRxContainedPdu(rxContainedPduIdx);

      /* if contained pdus completely fits into received container */
      if(IpduM_GetEndBytePositionOfRxContainedPdu(rxContainedPduIdx) <= pduLength)
      {
        /* send RxIndication for the referenced PduR-PDU to PduR module */
        PduR_IpduMRxIndication(IpduM_GetContainedPduRefOfRxContainedPdu(rxContainedPduIdx), &staticPduInfo);
      }
    }
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerRxProcessing_UnpackContainerPdu()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerRxProcessing_UnpackContainerPdu(IpduM_RxContainerPduIterType rxContainerId, P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, PduLengthType pduLength)
{
  /*@ assert (dataPtr != NULL_PTR); */  /* VCA_IPDUM_API_PATTERN_CHECK */
  if(IpduM_GetHeaderSizeOfRxContainerPdu(rxContainerId) > 0u)
  {/* Header-Size Short or Long */
    IpduM_ContainerRxProcessing_UnpackContainerPdu_DynamicLayout(rxContainerId, dataPtr, pduLength);
  }
  else
  {/* Header-Size None */
    IpduM_ContainerRxProcessing_UnpackContainerPdu_StaticLayout(rxContainerId, dataPtr, pduLength);
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerRxProcessing.c
**********************************************************************************************************************/
