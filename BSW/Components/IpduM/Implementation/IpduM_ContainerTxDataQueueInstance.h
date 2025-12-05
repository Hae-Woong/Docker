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
 *         \file  IpduM_ContainerTxDataQueueInstance.h
 *         \unit  ContainerTxDataQueueInstance
 *        \brief  Microsar IpduM Container Tx Data Queue Instance header file
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
#ifndef IPDUM_CONTAINERTXDATAQUEUEINSTANCE_H
# define IPDUM_CONTAINERTXDATAQUEUEINSTANCE_H

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
 *  IpduM_ContainerTx_DataQueueInstance_GetAsPointer()
*******************************************************************************************/
/*! \brief        Returns the pointer and length of this queue instance via pduInfoPtr.
 *  \details      -
 *  \param[in]    dataQueueInstanceIdx
 *  \return       pointer and length to this instance buffer
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(PduInfoType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_GetAsPointer(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_Get()
*******************************************************************************************/
/*! \brief        Copies the data from the queue instance to the destination buffer.
 *  \details      -
 *  \param[in]    dataQueueInstanceIdx
 *  \param[out]   destBuffer pointer to the destination buffer
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_Get(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) destBuffer);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_ClearBuffer()
*******************************************************************************************/
/*! \brief        Clear the queue instance buffer with the clearPattern.
 *  \details      -
 *  \param[in]    dataQueueInstanceIdx Idx to the instance which shall be reset
 *  \param[in]    clearPattern
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_ClearBuffer(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, uint8 clearPattern);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_ResetUpdateBits()
*******************************************************************************************/
/*! \brief        Resets the update bits used in a static layout.
 *  \details      -
 *  \param[in]    dataQueueInstanceIdx Idx to the instance which shall be reset
 *  \param[in]    txContainerId Id to get the update bit positions
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_ResetUpdateBits(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_GetFillCount
*******************************************************************************************/
/*! \brief        Return how much data is in the data queue instance
 *  \details      -
 *  \param[in]    dataQueueInstanceIdx
 *  \return       amount of bytes
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(PduLengthType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_GetFillCount(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_Put
*******************************************************************************************/
/*! \brief        Put the provided Pdu into this queue Instance.
 *  \details      Pdu is written at the writeOffset position. This offset is zero based and only describes the position inside this queue instance.
 *  \param[in]    dataQueueInstanceIdx
 *  \param[in]    PduInfoPtr pointer to the Pdu which shall be put into the queue
 *  \param[in]    writeOffset
 *  \return       E_OK      put was successful
 *  \return       E_NOT_OK  put was not successful
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_Put(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr, PduLengthType writeOffset);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_PutHeader
*******************************************************************************************/
/*! \brief        Put the provided header into this queue instance.
 *  \details      The header is written at the writeOffset position. This offset is zero based and only describes the position inside this queue instance.
 *  \param[in]    dataQueueInstanceIdx
 *  \param[in]    writeOffset
 *  \param[in]    headerId
 *  \param[in]    dlc
 *  \param[in]    headerSize
 *  \return       E_OK     put header was successful
 *  \return       E_NOT_OK put header was not successful
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_PutHeader(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, PduLengthType writeOffset, uint32 headerId, uint32 dlc, uint8 headerSize);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_SetBit
*******************************************************************************************/
/*! \brief        Set the bit @bitPosition.
 *  \details      The bitPosition is zero based. It indicates the bitPosition relative to the beginning of this queue instance.
 *  \param[in]    dataQueueInstanceIdx
 *  \param[in]    bytePosition
 *  \param[in]    bitInBytePosition
 *  \return       E_OK     the bit was set
 *  \return       E_NOT_OK the bit was not set
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_SetBit(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, PduLengthType bytePosition, uint8 bitInBytePosition);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_ClearBit
*******************************************************************************************/
/*! \brief        Clear the bit @bitPosition.
 *  \details      The bitPosition is zero based. It indicates the bitPosition relative to the beginning of this queue instance.
 *  \param[in]    dataQueueInstanceIdx
 *  \param[in]    bytePosition
 *  \param[in]    bitInBytePosition
 *  \return       E_OK     the bit was cleared
 *  \return       E_NOT_OK the bit was not cleared
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_ClearBit(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, PduLengthType bytePosition, uint8 bitInBytePosition);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_GetFreeBytesCount
*******************************************************************************************/
/*! \brief        Return how much bytes are available in the queue instance
 *  \details      -
 *  \param[in]    dataQueueInstanceIdx
 *  \return       amount of bytes
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *  \endspec
*******************************************************************************************/
FUNC(PduLengthType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_GetFreeBytesCount(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_Init
*******************************************************************************************/
/*! \brief        Clear the instance data buffer
 *  \details      -
 *  \param[in]    dataQueueInstanceIdx instance to be cleared
 *  \param[in]    txContainerIdx id to find the optional update bit positions
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *    requires txContainerIdx < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_Init(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, IpduM_CTxContainerPduIterType txContainerIdx);

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_CopyContainedPduIDs2TxConfBuffer()
*******************************************************************************************/
/*! \brief        Copies the IDs of the to be confirmed contained Pdus of the queued container instance to the tx confirmation buffer.
 *  \details      -
 *  \param[in]    dataQueueInstanceIdx the IDs are copied from this queue instance idx
 *  \param[in]    txContainerId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs
 *  \synchronous  TRUE
 *  \spec
 *    requires dataQueueInstanceIdx < IpduM_GetSizeOfCDataQueueInstance();
 *    requires txContainerId < IpduM_GetSizeOfCTxContainerPdu();
 *  \endspec
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_CopyContainedPduIDs2TxConfBuffer(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, IpduM_CTxContainerPduIterType txContainerId);

# define IPDUM_STOP_SEC_CODE
# include "IpduM_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* IPDUM_CONTAINERTXDATAQUEUEINSTANCE_H */

/**********************************************************************************************************************
 *  END OF FILE: IpduM_ContainerTxDataQueueInstance.h
 *********************************************************************************************************************/
