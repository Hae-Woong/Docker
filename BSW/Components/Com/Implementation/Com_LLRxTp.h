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
/*!        \file  Com_LLRxTp.h
 *         \unit  LLRxTp
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_LLRXTP_H)
# define COM_LLRXTP_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

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
  Com_LLRxTp_StartOfReceptionProcessing
**********************************************************************************************************************/
/*! \brief        This function is called by Com_StartOfReception() and indicates the start of the reception of a segmented TP Pdu.
    \details      -
    \pre          -
    \param[in]    rxPduId            ID of AUTOSAR COM I-PDU that has been received.
    \param[in]    TpSduInfoPtr       Payload information of the received TP segment (pointer to data and data + metadata length).
    \param[in]    TpSduLength        Length of the data
    \param[out]   RxBufferSizePtr    The Com returns in this value the remaining TP buffer size to the lower layer.
    \return       BufReq_ReturnType  BUFREQ_OK:       Connection has been accepted.
                                                      RxBufferSizePtr indicates the available receive buffer.
                                     BUFREQ_E_NOT_OK: Connection has been rejected.
                                                      RxBufferSizePtr remains unchanged.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      $external(TpSduInfoPtr);
      $external(RxBufferSizePtr);
      requires rxPduId < Com_GetSizeOfRxPduInfo();
      requires TpSduInfoPtr != NULL_PTR;
      requires RxBufferSizePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, COM_CODE) Com_LLRxTp_StartOfReceptionProcessing(Com_RxPduInfoIterType rxPduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) TpSduInfoPtr,
                                                                        PduLengthType TpSduLength, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) RxBufferSizePtr);

/**********************************************************************************************************************
  Com_LLRxTp_CopyRxDataProcessing
**********************************************************************************************************************/
/*! \brief        This function is called by Com_CopyRxData() to hand a received TP segment to Com.
                  The Com copies the received segment in its internal tp buffer.
    \details      -
    \pre          -
    \param[in]    rxPduId            ID of AUTOSAR COM I-PDU that has been received.
    \param[in]    PduInfoPointer     Payload information of the received TP segment (pointer to data and data length).
    \param[out]   RxBufferSizePtr    The Com returns in this value the remaining TP buffer size to the lower layer.
    \return       BufReq_ReturnType  BUFREQ_OK:       Connection has been accepted.
                                                      RxBufferSizePtr indicates the available receive buffer.
                                     BUFREQ_E_NOT_OK: Connection has been rejected.
                                                      RxBufferSizePtr remains unchanged.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      $external(PduInfoPointer);
      $external(RxBufferSizePtr);
      requires rxPduId < Com_GetSizeOfRxPduInfo();
      requires PduInfoPointer != NULL_PTR;
      requires RxBufferSizePtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, COM_CODE) Com_LLRxTp_CopyRxDataProcessing(Com_RxPduInfoIterType rxPduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPointer,
                                                                  CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) RxBufferSizePtr);

/**********************************************************************************************************************
  Com_LLRxTp_TpRxIndicationProcessing
**********************************************************************************************************************/
/*! \brief        This function is called by Com_TpRxIndication() after a TP I-PDU has been received to indicate the end of the reception.
    \details      -
    \pre          -
    \param[in]    rxPduId    ID of I-PDU that has been received.
                             Range: 0..(maximum number of I-PDU IDs received by AUTOSAR COM) - 1
    \param[in]    Result     Indicates whether the message was received successfully.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLRxTp_TpRxIndicationProcessing(Com_RxPduInfoIterType rxPduId, Std_ReturnType Result);

/**********************************************************************************************************************
  Com_LLRxTp_ResetRxTpConnection
**********************************************************************************************************************/
/*! \brief        This function is called to reset the Rx TP connection.
    \details      -
    \pre          -
    \param[in]    rxPduId      ID of I-PDU whose TP connection shall be reset.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLRxTp_ResetRxTpConnection(Com_RxPduInfoIterType rxPduId);

/**********************************************************************************************************************
  Com_LLRxTp_GetAddrRxTpBuffer
**********************************************************************************************************************/
/*! \brief        This function returns a pointer to the RxTpBuffer at the current index.
    \details      -
    \pre          -
    \param[in]    rxPduId                ID of I-PDU
    \return       Com_RxTpBufferPtrType  pointer to the RxTpBuffer
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_RxTpBufferPtrType, COM_CODE) Com_LLRxTp_GetAddrRxTpBuffer(Com_RxPduInfoIterType rxPduId);

#if (COM_DEV_ERROR_DETECT == STD_ON )
/**********************************************************************************************************************
  Com_LLRxTp_IsRxTpBufferMetaDataUsed
**********************************************************************************************************************/
/*! \brief        This function returns whether RxTpBufferMetaData is used or not.
    \details      -
    \pre          -
    \param[in]    rxPduId     ID of I-PDU
    \return       boolean     TRUE  RxTpBufferMetaData is used
                              FALSE RxTpBufferMetaData isn't used
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_LLRxTp_IsRxTpBufferMetaDataUsed(Com_RxPduInfoIterType rxPduId);

/**********************************************************************************************************************
  Com_LLRxTp_GetRxTpBufferMetaDataLength
**********************************************************************************************************************/
/*! \brief        This function returns the length of RxTpBufferMetaData.
    \details      -
    \pre          -
    \param[in]    rxPduId           ID of I-PDU
    \return       PduLengthType     length of RxTpBufferMetaData
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_LLRxTp_GetRxTpBufferMetaDataLength(Com_RxPduInfoIterType rxPduId);
#endif
#ifdef __PRQA__
#pragma PRQA_NO_SIDE_EFFECTS Com_LLRxTp_GetRxTpBufferMetaDataLength
#endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_LLRXTP_H */

/**********************************************************************************************************************
  END OF FILE: Com_LLRxTp.h
**********************************************************************************************************************/
