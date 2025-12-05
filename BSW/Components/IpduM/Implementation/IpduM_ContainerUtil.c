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
 *         \file  IpduM_ContainerUtil.c
 *         \unit  ContainerUtil
 *        \brief  Microsar IpduM Container Util source file
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

#define IPDUM_CONTAINERUTIL_SOURCE
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
#include "IpduM_Reporting.h"
#include "IpduM_ContainerUtil.h"
#include "SchM_IpduM.h"
#include "PduR_IpduM.h"

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
 *  IpduM_ContainerUtil_TransmitEmptyPdu()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerUtil_TransmitEmptyPdu(IpduM_CTxContainerPduIterType txContainerId)
{
  PduInfoType pduInfo;
  uint8 data = 0u;
  /* The actual memory this pointer is pointing at doesn't matter, because the SduLength is zero. Must not be a NULL_PTR though. */
  pduInfo.SduDataPtr = &data;
  pduInfo.SduLength = 0u;

  return PduR_IpduMTransmit(IpduM_GetTxContainerPduRefOfTxContainerPdu(txContainerId), &pduInfo);
}

/*******************************************************************************************
 *  IpduM_ContainerUtil_CalculateSizeOfContainer()
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
 */
FUNC(uint32, IPDUM_CODE) IpduM_ContainerUtil_CalculateSizeOfContainer(PduLengthType headerSize, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)   /* PRQA S 1505 */ /* MD_MSR_Rule8.7 */
{
  uint32 nextContainedPduByteOffset = 0u; /* start of the next containedPdu (header + data) in the container */
  uint32 pduLength = (uint32) PduInfoPtr->SduLength;

  boolean ableToReadNextHeader = /* nextContainedPduByteOffset (0) + */ headerSize <= pduLength;

  while(ableToReadNextHeader) /* FETA_IPDUM_05 */ /* PRQA S 0771 */ /* MD_IpduM_0771 */
  {
    /* get header id and dlc of the contained PDU */
    IpduM_ContainedPduHeaderType header = IpduM_ContainerUtil_ReadHeader(&(PduInfoPtr->SduDataPtr[nextContainedPduByteOffset]), headerSize);  /* VCA_IPDUM_API_PATTERN_CHECK */

    /* zero header marks the end of the container Pdu */
    if(header.headerId == 0u)
    {
      break;
    }

    /* check if the read DLC does even fit in into the whole Pdu. */
    boolean readContainedPduFitsIntoBuffer = header.dlc <= (pduLength - nextContainedPduByteOffset - headerSize);
    if(readContainedPduFitsIntoBuffer)
    {
      nextContainedPduByteOffset += headerSize + header.dlc;
      ableToReadNextHeader = (nextContainedPduByteOffset + headerSize) <= pduLength;
    }
    else
    {
      break;
    }
  }
  
  return nextContainedPduByteOffset;
}

/*******************************************************************************************
 *  IpduM_ContainerUtil_WriteHeader()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerUtil_WriteHeader(P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) pContainerBuf,  uint32 headerId, uint32 dlc, PduLengthType headerSize)
{
  if(headerSize == IPDUM_HEADERSIZE_SHORT)
  {
#  if (IPDUM_HEADER_BYTE_ORDER == IPDUM_LITTLE_ENDIAN)
    pContainerBuf[0] = ((uint8) (headerId >> 0));       /* PRQA S 2985 */ /* MD_IpduM_2985 */ /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[1] = ((uint8) (headerId >> 8));       /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[2] = ((uint8) (headerId >> 16));      /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[3] = ((uint8) (dlc >> 0));    /* PRQA S 2985 */ /* MD_IpduM_2985 */ /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
#  else /* (IPDUM_HEADER_BYTE_ORDER == IPDUM_BIG_ENDIAN) */
    pContainerBuf[0] = ((uint8) (headerId >> 16));      /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[1] = ((uint8) (headerId >> 8));       /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[2] = ((uint8) (headerId >> 0));       /* PRQA S 2985 */ /* MD_IpduM_2985 */ /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[3] = ((uint8) (dlc >> 0));    /* PRQA S 2985 */ /* MD_IpduM_2985 */ /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
#  endif
  }
  if(headerSize == IPDUM_HEADERSIZE_LONG)
  {
#  if (IPDUM_HEADER_BYTE_ORDER == IPDUM_LITTLE_ENDIAN)
    pContainerBuf[0] = ((uint8) (headerId >> 0));       /* PRQA S 2985 */ /* MD_IpduM_2985 */ /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[1] = ((uint8) (headerId >> 8));       /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[2] = ((uint8) (headerId >> 16));      /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[3] = ((uint8) (headerId >> 24));      /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[4] = ((uint8) (dlc >> 0));    /* PRQA S 2985 */ /* MD_IpduM_2985 */ /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[5] = ((uint8) (dlc >> 8));    /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[6] = ((uint8) (dlc >> 16));   /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[7] = ((uint8) (dlc >> 24));   /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
#  else /* (IPDUM_HEADER_BYTE_ORDER == IPDUM_BIG_ENDIAN) */
    pContainerBuf[0] = ((uint8) (headerId >> 24));      /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[1] = ((uint8) (headerId >> 16));      /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[2] = ((uint8) (headerId >> 8));       /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[3] = ((uint8) (headerId >> 0));       /* PRQA S 2985 */ /* MD_IpduM_2985 */ /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[4] = ((uint8) (dlc >> 24));   /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[5] = ((uint8) (dlc >> 16));   /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[6] = ((uint8) (dlc >> 8));    /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
    pContainerBuf[7] = ((uint8) (dlc >> 0));    /* PRQA S 2985 */ /* MD_IpduM_2985 */ /* VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER */
#  endif
  }
}

/*******************************************************************************************
 *  IpduM_ContainerUtil_IncreaseDlcByMetaDataSize()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerUtil_IncreaseDlcByMetaDataSize(IpduM_CTxContainerPduIterType txContainerId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  /* J1939 only uses short headers */
  if(IpduM_GetHeaderSizeOfTxContainerPdu(txContainerId) == IPDUM_HEADERSIZE_SHORT)
  {
    if(IpduM_GetMetaDataSizeOfTxContainerPdu(txContainerId) > 0u)
    {
        /* increase DLC of last header by meta data size */
        PduInfoPtr->SduDataPtr[IPDUM_METADATA_DLC_POSITION] += IpduM_GetMetaDataSizeOfTxContainerPdu(txContainerId);
    }
  }
}

/*******************************************************************************************
 *  IpduM_ContainerUtil_DecreaseDlcByMetaDataSize()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerUtil_DecreaseDlcByMetaDataSize(IpduM_CTxContainerPduIterType txContainerId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  /* J1939 only uses short headers */
  if(IpduM_GetHeaderSizeOfTxContainerPdu(txContainerId) == IPDUM_HEADERSIZE_SHORT)
  {
    if(IpduM_GetMetaDataSizeOfTxContainerPdu(txContainerId) > 0u)
    {
        /* decrease DLC of last header by meta data size */
        PduInfoPtr->SduDataPtr[IPDUM_METADATA_DLC_POSITION] -= IpduM_GetMetaDataSizeOfTxContainerPdu(txContainerId);
    }
  }
}

/*******************************************************************************************
*  IpduM_ContainerUtil_ReadHeader()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(IpduM_ContainedPduHeaderType, IPDUM_CODE) IpduM_ContainerUtil_ReadHeader(P2CONST(uint8, AUTOMATIC, IPDUM_APPL_DATA) pContainerBuf, uint32 headerSize)
{
  IpduM_ContainedPduHeaderType header;
  
  header.headerId = 0;
  header.dlc = 0;

  if(headerSize == IPDUM_HEADERSIZE_SHORT)
  {
    /* short header */
    SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
#  if (IPDUM_HEADER_BYTE_ORDER == IPDUM_LITTLE_ENDIAN)
    /* *INDENT-OFF* */
    header.headerId =
      (((uint32) pContainerBuf[0]) << 0) |  /* PRQA S 2985 */ /* MD_IpduM_2985 */
      (((uint32) pContainerBuf[1]) << 8) |
      (((uint32) pContainerBuf[2]) << 16);
    header.dlc = ((uint32) pContainerBuf[3]);
    /* *INDENT-ON* */
#  else /* (IPDUM_HEADER_BYTE_ORDER == IPDUM_BIG_ENDIAN) */
    /* *INDENT-OFF* */
    header.headerId =
      (((uint32) pContainerBuf[0]) << 16) |
      (((uint32) pContainerBuf[1]) << 8) |
      (((uint32) pContainerBuf[2]) << 0);  /* PRQA S 2985 */ /* MD_IpduM_2985 */
    header.dlc = (((uint32) pContainerBuf[3]) << 0);  /* PRQA S 2985 */ /* MD_IpduM_2985 */
    /* *INDENT-ON* */
#  endif
    SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  }
  if(headerSize == IPDUM_HEADERSIZE_LONG)
  {
    /* long header */
    SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
#  if (IPDUM_HEADER_BYTE_ORDER == IPDUM_LITTLE_ENDIAN)
    /* *INDENT-OFF* */
    header.headerId =
      (((uint32) pContainerBuf[0]) << 0) |  /* PRQA S 2985 */ /* MD_IpduM_2985 */
      (((uint32) pContainerBuf[1]) << 8) |
      (((uint32) pContainerBuf[2]) << 16) |
      (((uint32) pContainerBuf[3]) << 24);
    header.dlc =
      (((uint32) pContainerBuf[4]) << 0) |  /* PRQA S 2985 */ /* MD_IpduM_2985 */
      (((uint32) pContainerBuf[5]) << 8) |
      (((uint32) pContainerBuf[6]) << 16) |
      (((uint32) pContainerBuf[7]) << 24);
    /* *INDENT-ON* */
#  else /* (IPDUM_HEADER_BYTE_ORDER == IPDUM_BIG_ENDIAN) */
    /* *INDENT-OFF* */
    header.headerId =
      (((uint32) pContainerBuf[0]) << 24) |
      (((uint32) pContainerBuf[1]) << 16) |
      (((uint32) pContainerBuf[2]) << 8) |
      (((uint32) pContainerBuf[3]) << 0);  /* PRQA S 2985 */ /* MD_IpduM_2985 */
    header.dlc =
      (((uint32) pContainerBuf[4]) << 24) |
      (((uint32) pContainerBuf[5]) << 16) |
      (((uint32) pContainerBuf[6]) << 8) |
      (((uint32) pContainerBuf[7]) << 0);  /* PRQA S 2985 */ /* MD_IpduM_2985 */
    /* *INDENT-ON* */
#  endif
    SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  }
  return header;
}

/*******************************************************************************************
 *  IpduM_ContainerUtil_SearchContainedPduIdByHeaderId()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerUtil_SearchContainedPduIdByHeaderId(IpduM_RxContainerPduIterType rxContainerPduIdx, uint32 headerId, P2VAR(IpduM_RxContainedPduIterType, AUTOMATIC, IPDUM_APPL_DATA) foundRxContainedPduId)
{
  Std_ReturnType retVal = E_NOT_OK;
  IpduM_RxContainedPduIndEndIdxOfRxContainerPduType containedPduIndIdx;

  for(containedPduIndIdx = IpduM_GetRxContainedPduIndStartIdxOfRxContainerPdu(rxContainerPduIdx); containedPduIndIdx < IpduM_GetRxContainedPduIndEndIdxOfRxContainerPdu(rxContainerPduIdx); containedPduIndIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_RxContainedPduIterType rxContainedPduId = IpduM_GetRxContainedPduInd(containedPduIndIdx);

    if(IpduM_GetHeaderIdOfRxContainedPdu(rxContainedPduId) == (IpduM_HeaderIdOfRxContainedPduType)headerId)
    {
      *foundRxContainedPduId = rxContainedPduId;
      retVal = E_OK;
      break;
    }
  }

  return retVal;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerUtil.c
**********************************************************************************************************************/
