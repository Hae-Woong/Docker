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
 *         \file  IpduM_MuxRx.c
 *         \unit  MuxRx
 *        \brief  Microsar IpduM Multiplexer Rx source file
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

#define IPDUM_MUXRX_SOURCE
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
#include "IpduM_MuxRx.h"
#include "IpduM_MuxUtil.h"
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
 *  IpduM_MuxRx_RxIndication()
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
FUNC(void, IPDUM_CODE) IpduM_MuxRx_RxIndication(IpduM_RxPathwayIndIdxOfRxInfoType rxMuxPduId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  PduInfoType pduInfo;
  /* Point to copy target buffer */
  pduInfo.SduDataPtr = IpduM_GetAddrRxBuffer(IpduM_GetRxBufferStartIdxOfRxMuxPdu(rxMuxPduId));
  pduInfo.SduLength = PduInfoPtr->SduLength;    /* Set length of pdu */

  /* Ensure consistency of pduInfo pointer. */
  if(PduInfoPtr->SduLength > ((PduLengthType)IpduM_GetRxBufferLengthOfRxMuxPdu(rxMuxPduId))) /* COV_IPDUM_DEFENSIVE_PROGRAMMING */
  {
    pduInfo.SduLength = (PduLengthType) IpduM_GetRxBufferLengthOfRxMuxPdu(rxMuxPduId);

    IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_MUXRX_RXINDICATION, IPDUM_E_PARAM);
  }

  {/* block for encapsulation */
    /* loop over dynamic parts */
    IpduM_RxDynPduIterType ipduMDynPduIdx = IpduM_GetRxDynPduStartIdxOfRxMuxPdu(rxMuxPduId);
    for(; ipduMDynPduIdx < IpduM_GetRxDynPduEndIdxOfRxMuxPdu(rxMuxPduId); ipduMDynPduIdx++) /* FETA_IPDUM_01 */
    {
      if(IpduM_MuxUtil_IsDynPartLongEnough(ipduMDynPduIdx, pduInfo.SduLength))
      {
        if(IpduM_MuxUtil_RxDynPduMatchesSelectorPattern(rxMuxPduId, ipduMDynPduIdx, PduInfoPtr))
        {
          IpduM_SegmentIterType segmentIdx = IpduM_GetSegmentStartIdxOfRxDynPdu(ipduMDynPduIdx);
          for(; segmentIdx < IpduM_GetSegmentEndIdxOfRxDynPdu(ipduMDynPduIdx); segmentIdx++)  /* FETA_IPDUM_01 */
          {
            /* copy dynamic part to rx buffer */
            IpduM_MuxUtil_CopySegment(segmentIdx, &pduInfo, PduInfoPtr->SduDataPtr);
          }
          /* Indicate reception of dynamic part to PduR, hand over the buffer containing received data */
          PduR_IpduMRxIndication(IpduM_GetUlPduRefOfRxDynPdu(ipduMDynPduIdx), &pduInfo);
          break;
        }
      }
    }
  }

  if(IpduM_IsRxStaticPduUsedOfRxMuxPdu(rxMuxPduId))  
  {
    IpduM_RxStaticPduIterType ipduMStaticPduIdx = IpduM_GetRxStaticPduIdxOfRxMuxPdu(rxMuxPduId);

    if(IpduM_MuxUtil_IsStaticPartLongEnough(ipduMStaticPduIdx, pduInfo.SduLength))
    {
      IpduM_SegmentIterType segmentIdx = IpduM_GetSegmentStartIdxOfRxStaticPdu(ipduMStaticPduIdx);
      for(; segmentIdx < IpduM_GetSegmentEndIdxOfRxStaticPdu(ipduMStaticPduIdx); segmentIdx++)  /* FETA_IPDUM_01 */
      {
        /* Copy received data to rx buffer */
        IpduM_MuxUtil_CopySegment(segmentIdx, &pduInfo, PduInfoPtr->SduDataPtr);
      }
      /* Indicate reception of static part to PduR, hand over the buffer containing received data */
      PduR_IpduMRxIndication(IpduM_GetUlPduRefOfRxStaticPdu(ipduMStaticPduIdx), &pduInfo);
    }
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_MuxRx.c
**********************************************************************************************************************/
