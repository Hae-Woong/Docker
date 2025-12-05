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
 *         \file  IpduM_MuxUtil.h
 *         \unit  MuxUtil
 *        \brief  Microsar IpduM Multiplexer util header file
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
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#ifndef IPDUM_MUXUTIL_H
# define IPDUM_MUXUTIL_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "IpduM_Cfg.h"
# include "IpduM_Lcfg.h"
# include "IpduM_PBcfg.h"
# include "ComStack_Types.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define IPDUM_START_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/*******************************************************************************************
*  IpduM_MuxUtil_CopySegment()
*******************************************************************************************/
/*! \brief        Copies segments in PDUs
 *  \details      -
 *  \param[in]    segmentIdx
 *  \param[out]   destPduInfoPtr    PduInfoPtr to target buffer
 *  \param[in]    srcPtr            Pointer to source buffer
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different segments
 *  \synchronous  TRUE
 *  \spec
 *    requires segmentIdx < IpduM_GetSizeOfSegment();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_MuxUtil_CopySegment(IpduM_SegmentIterType segmentIdx, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) destPduInfoPtr, P2CONST(uint8, AUTOMATIC, IPDUM_APPL_DATA) srcPtr);

/*******************************************************************************************
*  IpduM_MuxUtil_WriteMuxFillerSegment()
*******************************************************************************************/
/*! \brief        Writes filler segments for imperfectly overlapping dynamic parts in PDUs
 *  \details      -
 *  \param[in]    restSegmentIdx
 *  \param[in]    txPathwayIdx
 *  \param[out]   destPduInfoPtr    PduInfoPtr to target buffer
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different segments
 *  \synchronous  TRUE
 *  \spec
 *    requires restSegmentIdx < IpduM_GetSizeOfRestSegment();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_MuxUtil_WriteMuxFillerSegment(IpduM_RestSegmentIterType restSegmentIdx, IpduM_CTxPathwayIterType txPathwayIdx, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) destPduInfoPtr);

/*******************************************************************************************
*  IpduM_MuxUtil_RxDynPduMatchesSelectorPattern()
*******************************************************************************************/
/*! \brief        Does this rxDynPdu match the selector pattern in PduInfoPtr?
 *  \details      -
 *  \param[in]    rxMuxPduIdx
 *  \param[in]    rxDynPduIdx
 *  \param[in]    PduInfoPtr
 *  \return       rxDynPdu matches the selector pattern in PduInfoPtr
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires rxMuxPduIdx < IpduM_GetSizeOfRxMuxPdu();
 *    requires rxDynPduIdx < IpduM_GetSizeOfRxDynPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_MuxUtil_RxDynPduMatchesSelectorPattern(IpduM_RxMuxPduIterType rxMuxPduIdx, IpduM_RxDynPduIterType rxDynPduIdx, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
*  IpduM_MuxUtil_IsDynPartLongEnough()
*******************************************************************************************/
/*! \brief        Is the actual received Pdu length large enough so that all of this dynamic part segments can be read?
 *  \details      -
 *  \param[in]    rxDynPduIdx internal ID of the dynamic part
 *  \param[in]    length Actual received Pdu length
 *  \return       The Pdu is large enough to read all dynamic part segments.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires rxDynPduIdx < IpduM_GetSizeOfRxDynPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_MuxUtil_IsDynPartLongEnough(IpduM_RxDynPduIterType rxDynPduIdx, PduLengthType length);

/*******************************************************************************************
*  IpduM_MuxUtil_IsStaticPartLongEnough()
*******************************************************************************************/
/*! \brief        Is the actual received Pdu length large enough so that the static part segments can be read?
 *  \details      -
 *  \param[in]    rxStaticPduIdx internal ID of the static part
 *  \param[in]    length Actual received Pdu length
 *  \return       The Pdu is large enough to read all static part segments.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires rxStaticPduIdx < IpduM_GetSizeOfRxStaticPdu();
 *  \endspec
*******************************************************************************************/
FUNC(boolean, IPDUM_CODE) IpduM_MuxUtil_IsStaticPartLongEnough(IpduM_RxStaticPduIterType rxStaticPduIdx, PduLengthType length);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_MUXUTIL_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_MuxUtil.h
 *********************************************************************************************************************/
