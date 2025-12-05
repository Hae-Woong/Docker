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
 *         \file  IpduM_ContainerUtil.h
 *         \unit  ContainerUtil
 *        \brief  Microsar IpduM Container Util header file
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
#ifndef IPDUM_CONTAINERUTIL_H
# define IPDUM_CONTAINERUTIL_H

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
 *  IpduM_ContainerUtil_TransmitEmptyPdu()
*******************************************************************************************/
/*! \brief        Sends an empty PDU with DLC=0 with the given Container PDU ID.
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       E_OK            Transmission was successful
 *  \return       E_NOT_OK        Transmission was unsuccessful
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerUtil_TransmitEmptyPdu(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerUtil_CalculateSizeOfContainer()
*******************************************************************************************/
/*! \brief        Calculates and returns the size of the container PDU in the passed buffer.
 *  \details      -
 *  \param[in]    PduInfoPtr        Pointer to the buffer where the container is stored and its max length.
 *  \param[in]    headerSize
 *  \return       containerSize     Size of the container in bytes, including the closing zero-header.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        SPEC-2068547
 *  \spec
 *    requires PduInfoPtr != NULL_PTR;
 *    requires PduInfoPtr->SduDataPtr != NULL_PTR;
 *  \endspec
*******************************************************************************************/
FUNC(uint32, IPDUM_CODE) IpduM_ContainerUtil_CalculateSizeOfContainer(PduLengthType headerSize, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
*  IpduM_ContainerUtil_WriteHeader()
*******************************************************************************************/
/*! \brief        Writes the header to the container PDU buffer.
 *  \details      The caller has to ensure there is enough free space in the buffer!
 *  \param[in]    pContainerBuf   Pointer to buffer containing container PDU
 *  \param[in]    headerId          Header information: ID
 *  \param[in]    dlc               Header information: DLC
 *  \param[in]    headerSize
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different buffers
 *  \synchronous  TRUE
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerUtil_WriteHeader(P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) pContainerBuf, uint32 headerId, uint32 dlc, PduLengthType headerSize);

/*******************************************************************************************
*  IpduM_ContainerUtil_IncreaseDlcByMetaDataSize()
*******************************************************************************************/
/*! \brief        Changes length in last PDU's header in case of failed transmission
 *  \details      -
 *  \param[in]    txContainerId
 *  \param[in,out]pduInfoPtr
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerUtil_IncreaseDlcByMetaDataSize(IpduM_CTxContainerPduIterType txContainerId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
*  IpduM_ContainerUtil_DecreaseDlcByMetaDataSize()
*******************************************************************************************/
/*! \brief        Changes length in last PDU's header to match CanIf processing
 *  \details      -
 *  \param[in]    txContainerId
 *  \param[in,out]pduInfoPtr
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerUtil_DecreaseDlcByMetaDataSize(IpduM_CTxContainerPduIterType txContainerId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr);

/*******************************************************************************************
*  IpduM_ContainerUtil_ReadHeader()
*******************************************************************************************/
/*! \brief        Reads the header from the start position of the provided buffer
 *  \details      -
 *  \param[in]    headerSize
 *  \param[in]    pContainerBuf   Pointer to buffer containing container PDU, must be at least headerSize bytes large!
 *  \return       header          Header at the beginning of the provided container PDU buffer
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different buffers
 *  \synchronous  TRUE
 *  \spec
 *    requires pContainerBuf != NULL_PTR;
 *  \endspec
*******************************************************************************************/
FUNC(IpduM_ContainedPduHeaderType, IPDUM_CODE) IpduM_ContainerUtil_ReadHeader(P2CONST(uint8, AUTOMATIC, IPDUM_APPL_DATA) pContainerBuf, uint32 headerSize);

/*******************************************************************************************
 *  IpduM_ContainerUtil_SearchContainedPduIdByHeaderId()
*******************************************************************************************/
/*! \brief        Searches for the received Container PDU header ID in the list of Rx Contained PDUs.
 *  \details      -
 *  \param[in]    rxContainerPduIdx
 *  \param[in]    headerId               Header ID to be searched for.
 *  \param[out]   foundRxContainedPduId  ID of the found RxContainedPdu.
 *  \return       E_OK      Found a matching RxContainedPdu (returned via @foundRxContainedPduId)
 *                E_NOT_OK  Did not find any matching RxContainedPdu. @foundRxContainedPduId must not be used.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires rxContainerPduIdx < IpduM_GetSizeOfRxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerUtil_SearchContainedPduIdByHeaderId(IpduM_RxContainerPduIterType rxContainerPduIdx, uint32 headerId, P2VAR(IpduM_RxContainedPduIterType, AUTOMATIC, IPDUM_APPL_DATA) foundRxContainedPduId);


# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERUTIL_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerUtil.h
 *********************************************************************************************************************/
