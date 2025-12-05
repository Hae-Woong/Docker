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
/*!        \file  Com_TxBuffer.h
 *         \unit  TxBuffer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXBUFFER_H)
# define COM_TXBUFFER_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

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
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_TxBuffer_InitTxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes a tx I-PDU in Com_TxBuffer.
    \details      -
    \pre          -
    \param[in]    ComPduId   Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_InitTxIpdu(Com_TxPduInfoIterType ComPduId);

/**********************************************************************************************************************
  Com_TxBuffer_InitShdwBuffer
**********************************************************************************************************************/
/*! \brief        This function initializes the ShadowBuffer of a SignalGroup.
    \details      -
    \pre          -
    \param[in]    sigGrpHnd   Handle of the SignalGroup.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_InitShdwBuffer(Com_TxSigGrpInfoIterType sigGrpHnd);

/**********************************************************************************************************************
  Com_TxBuffer_CpyInitMetaDataToBffr
**********************************************************************************************************************/
/*! \brief        This function copies the initial meta data to the  TxBuffer.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of I-PDU to be transmitted.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_CpyInitMetaDataToBffr(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxBuffer_ClearUpdateBits
**********************************************************************************************************************/
/*! \brief        This method sets the update bits to 0.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
***********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_ClearUpdateBits(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxBuffer_SetUpdateBitSignalGroup
**********************************************************************************************************************/
/*! \brief        This method sets the update bits of the Signal Group, if an UpdateBit is configured.
    \details      -
    \pre          -
    \param[in]    idxTxSigGrpInfo    ID of TxSigGrpInfo.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxSigGrpInfo < Com_GetSizeOfTxSigGrpInfo();
    \endspec
***********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_SetUpdateBitSignalGroup(Com_TxSigGrpInfoIterType idxTxSigGrpInfo);

/**********************************************************************************************************************
  Com_TxBuffer_SetUpdateBitSignal
**********************************************************************************************************************/
/*! \brief        This method sets the update bits of the Signal, if an UpdateBit is configured.
    \details      -
    \pre          -
    \param[in]    idxTxSigInfo    ID of TxSigInfo.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxSigInfo < Com_GetSizeOfTxSigInfo();
    \endspec
***********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_SetUpdateBitSignal(Com_TxSigInfoIterType idxTxSigInfo);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_TxBuffer_SetUpdateBitGwDescriptionAccess
**********************************************************************************************************************/
/*! \brief        This method sets the update bits of the Gateway Description Access, if an UpdateBit is configured.
    \details      -
    \pre          -
    \param[in]    idxGwDescriptionAccessInfo    ID of GwDescriptionAccessInfo.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxGwDescriptionAccessInfo < Com_GetSizeOfGwDescriptionAccessInfo();
    \endspec
***********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_SetUpdateBitGwDescriptionAccess(Com_GwDescriptionAccessInfoIterType idxGwDescriptionAccessInfo);
#endif

/**********************************************************************************************************************
  Com_TxBuffer_WriteSignalGroup
**********************************************************************************************************************/
/*! \brief        This method copies the data from the signal group shadow buffer to the related Tx ComIPdu buffer.
    \details      -
    \pre          -
    \param[in]    SignalGroupId    ID of Tx signal group.
    \param[in]    ShadowBufferPtr  Reference to the signal group buffer.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires SignalGroupId < Com_GetSizeOfTxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxBuffer_WriteSignalGroup(Com_TxSigGrpInfoIterType SignalGroupId, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) ShadowBufferPtr);

/**********************************************************************************************************************
  Com_TxBuffer_GetAddrTxBufferAtIdx
**********************************************************************************************************************/
/*! \brief        This method returns the Address of the TxBuffer at the given TxBufferInfoIdx + offset.
    \details      -
    \pre          Before calling this function it must be checked first, that the offset is < than Com_TxBuffer_GetTxBufferLength()!
    \param[in]    txBufferInfoIdx     ID of TxBufferInfo
    \param[in]    offset              offset in TxBuffer to the start index referenced by the given txBufferInfoIdx
    \return       Com_TxBufferPtrType
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires txBufferInfoIdx < Com_GetSizeOfTxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_TxBufferPtrType, COM_CODE) Com_TxBuffer_GetAddrTxBufferAtIdx(Com_TxBufferInfoIterType txBufferInfoIdx, PduLengthType offset);

/**********************************************************************************************************************
  Com_TxBuffer_GetAddrTxBufferFirstElement
**********************************************************************************************************************/
/*! \brief        This method returns the Address of the TxBuffer at the start index referred by the given TxBufferInfoIdx.
    \details      -
    \pre          -
    \param[in]    txBufferInfoIdx     ID of TxBufferInfo
    \return       Com_TxBufferPtrType
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires txBufferInfoIdx < Com_GetSizeOfTxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_TxBufferPtrType, COM_CODE) Com_TxBuffer_GetAddrTxBufferFirstElement(Com_TxBufferInfoIterType txBufferInfoIdx);

/**********************************************************************************************************************
  Com_TxBuffer_GetTxBufferLength
**********************************************************************************************************************/
/*! \brief        This method returns the Length of the TxBuffer referred by the given TxBufferInfoIdx.
    \details      -
    \pre          -
    \param[in]    txBufferInfoIdx     ID of TxBufferInfo
    \return       Com_TxBufferLengthOfTxBufferInfoType
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires txBufferInfoIdx < Com_GetSizeOfTxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_TxBufferLengthOfTxBufferInfoType, COM_CODE) Com_TxBuffer_GetTxBufferLength(Com_TxBufferInfoIterType txBufferInfoIdx);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXBUFFER_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxBuffer.h
**********************************************************************************************************************/
