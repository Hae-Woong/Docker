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
/*!        \file  Com_Transmission.h
 *         \unit  Transmission
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TRANSMISSION_H)
# define COM_TRANSMISSION_H

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
  Com_Transmission_SendSignalGroup_Processing
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxSignalIf_SendSignalGroup_Processing function.
    \details      -
    \pre          -
    \param[in]    idxTxSigGrpInfo    ID of signal group to be send.
    \param[in]    ShadowBufferPtr    Pointer to Shadow Buffer
    \return       uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                                (or service failed due to development error)
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxSigGrpInfo < Com_GetSizeOfTxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_SendSignalGroup_Processing(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) ShadowBufferPtr);

/**********************************************************************************************************************
  Com_Transmission_SendSignal_Processing
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxSignalIf_SendSignal_Processing
    \details      -
    \pre          -
    \param[in]    txAccessInfo  Index of signal or group signal to be sent.
    \param[in]    comDataPtr    Pointer to reference to the signal data to be transmitted and length of the dynamic signal.
    \return       uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                                (or service failed due to development error)
                    COM_BUSY                    in case the TP-Buffer is locked for large data types handling
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires comDataPtr != NULL_PTR;
      requires txAccessInfo < Com_GetSizeOfTxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_SendSignal_Processing(Com_TxAccessInfoIterType txAccessInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_Transmission_SendSignalGroupArray_Processing
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxSignalIf_SendSignalGroupArray_Processing function
    \details      -
    \pre          -
    \param[in]    SignalGroupId               Id of signal group to be sent.
    \param[in]    SignalGroupArrayPtr         Reference to the signal group array.
    \return       uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped (or service failed due to development
                                                error)
                    COM_BUSY                    in case the TP-Buffer is locked for large data types handling
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires $external(SignalGroupArrayPtr);
      requires SignalGroupId < Com_GetSizeOfTxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_SendSignalGroupArray_Processing(Com_TxSigGrpInfoIterType SignalGroupId, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr);

/**********************************************************************************************************************
  Com_Transmission_InvalidateSignal
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxInv_InvalidateSignal function.
    \details      -
    \pre          -
    \param[in]    txAccessInfo ID of the access info table.
    \return       uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                                (or service failed due to development error)
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires txAccessInfo < Com_GetSizeOfTxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_InvalidateSignal(Com_TxAccessInfoIterType txAccessInfo);

/**********************************************************************************************************************
  Com_Transmission_InvalidateSignalGroup
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxInv_InvalidateSignalGroup function.
    \details      -
    \pre          -
    \param[in]    SignalGroupId ID of the access info table.
    \return       uint8
                  E_OK                        service has been accepted
                  COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                              (or service failed due to development error)
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires SignalGroupId < Com_GetSizeOfTxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_InvalidateSignalGroup(Com_TxSigGrpInfoIterType SignalGroupId);

/**********************************************************************************************************************
  Com_Transmission_TriggerIpduSend
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxTransmit_TriggerIpduSend function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_TriggerIpduSend(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_Transmission_TriggerIpduSendWithMetaData
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxTransmit_TriggerIpduSendWithMetaData function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \param[in]    MetaData    Pointer to the MetaData.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
      requires MetaData != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_TriggerIpduSendWithMetaData(Com_TxPduInfoIterType ComTxPduId, P2CONST(uint8, AUTOMATIC, AUTOMATIC) MetaData);

/**********************************************************************************************************************
  Com_Transmission_TxConfirmationProcessing
**********************************************************************************************************************/
/*! \brief        This function calls the Com_LLTxIf_TxConfirmationProcessing function.
    \details      -
    \pre          -
    \param[in]    TxPduId       ID of the I-PDU that has been transmitted.
    \trace        SPEC-2736844
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_TxConfirmationProcessing(Com_TxPduInfoIterType TxPduId);

/**********************************************************************************************************************
  Com_Transmission_TriggerTransmitProcessing
**********************************************************************************************************************/
/*! \brief            This function calls the Com_LLTxIf_TriggerTransmitProcessing function.
    \details          -
    \pre              -
    \param[in]        TxPduId      ID of the SDU that is requested to be transmitted.
    \param[in,out]    PduInfoPtr   Contains a pointer to a buffer (SduDataPtr) to where the SDU
                                   data shall be copied, and the available buffer size in SduLength.
                                   On return, the service will indicate the length of the copied SDU
                                   data in SduLength.
    \return           E_OK         SDU has been copied and SduLength indicates the number of copied bytes.
    \return           E_NOT_OK     No data has been copied, because
                                   Com is not initialized
                                   or TxPduId is not valid
                                   or PduInfoPtr is NULL_PTR
                                   or SduDataPtr is NULL_PTR
                                   or SduLength is to small.
    \trace            DSGN-Com23021
    \trace            SPEC-2737060
    \context          TASK|ISR2
    \synchronous      TRUE
    \reentrant        TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_Transmission_TriggerTransmitProcessing(Com_TxPduInfoIterType TxPduId, CONSTP2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/**********************************************************************************************************************
  Com_Transmission_CopyTxDataProcessing
**********************************************************************************************************************/
/*! \brief        This function calls the Com_LLTxTp_CopyTxDataProcessing function.
    \details      -
    \pre          -
    \param[in]    PduId              ID of AUTOSAR COM I-PDU that is transmitted.
    \param[in]    PduInfoPtr         Pointer to a PduInfoType, which indicates the number of bytes to be copied (SduLength)
                                     and the location where the data have to be copied to (SduDataPtr).
    \param[in]    RetryInfoPtr       Information if the last transmit was successful or if a retransmit of the data is needed.
    \param[out]   TxDataCntPtr       Remaining Tx data after completion of this call.
    \param[out]   BufReq_ReturnType  BUFREQ_OK:       Connection has been accepted.
                                                      RxBufferSizePtr indicates the available receive buffer.
                                     BUFREQ_E_NOT_OK: Connection has been rejected.
                                                      RxBufferSizePtr remains unchanged.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      $external(PduInfoPtr);
      $external(RetryInfoPtr);
      $external(TxDataCntPtr);
      requires PduId < Com_GetSizeOfTxPduInfo();
      requires PduInfoPtr != NULL_PTR;
      requires RetryInfoPtr != NULL_PTR;
      requires TxDataCntPtr != NULL_PTR;
      requires $lengthOf(PduInfoPtr->SduDataPtr) >= PduInfoPtr->SduLength;
    \endspec
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, COM_CODE) Com_Transmission_CopyTxDataProcessing(Com_TxPduInfoIterType PduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                                  CONSTP2CONST(RetryInfoType, AUTOMATIC, AUTOMATIC) RetryInfoPtr, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) TxDataCntPtr);

/**********************************************************************************************************************
  Com_Transmission_TpTxConfirmationProcessing
**********************************************************************************************************************/
/*! \brief        This function calls the Com_LLTxTp_TpTxConfirmationProcessing function.
    \details      -
    \pre          -
    \param[in]    PduId     ID of AUTOSAR COM I-PDU that is transmitted.
    \param[in]    Result    Indicates whether the Pdu was transmitted successfully.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires PduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_TpTxConfirmationProcessing(Com_TxPduInfoIterType PduId, Std_ReturnType Result);

/**********************************************************************************************************************
  Com_Transmission_MainFunctionProcessing
**********************************************************************************************************************/
/*! \brief        This function iterates over all Tx ComIPdus and processes the cyclic transmission, the repetition counter and the MDT counter.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling tx main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_MainFunctionProcessing(Com_MainFunctionTxStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_Transmission_MainFunctionTxTransmit
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxTransmit_MainFunctionTx function.
    \details      -
    \pre          -
    \param[in]    mainFunctionId    ID of the calling tx main function.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires mainFunctionId < Com_GetSizeOfMainFunctionTxStruct();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_MainFunctionTxTransmit(Com_MainFunctionTxStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_Transmission_InitTx
**********************************************************************************************************************/
/*! \brief        This function initializes Tx IPdus and related parameters.
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_InitTx(void);

/**********************************************************************************************************************
  Com_Transmission_InitTxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes a tx I-PDU.
    \details      -
    \pre          -
    \param[in]    ComPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComPduId <  Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_InitTxIpdu(Com_TxPduInfoIterType ComPduId);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
Com_Transmission_StartCyclicTransmission
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxCyclic_StartCyclicTransmission function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_StartCyclicTransmission(Com_TxPduInfoIterType ComTxPduId);
#endif

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
Com_Transmission_StopCyclicTransmission
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxCyclic_StopCyclicTransmission function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different Handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_StopCyclicTransmission(Com_TxPduInfoIterType ComTxPduId);
#endif

/**********************************************************************************************************************
  Com_Transmission_StopRepetitions
**********************************************************************************************************************/
/*! \brief        This function calls the Com_Repetition_Stop function.
    \details      -
    \pre          -
    \param[in]    repetitionInfoIdx ID of RepetitionInfo.
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \context      TASK|ISR2
    \spec
      requires repetitionInfoIdx < Com_GetSizeOfRepetitionInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_StopRepetitions(Com_RepetitionInfoIterType repetitionInfoIdx);

/**********************************************************************************************************************
  Com_Transmission_ClearTransmitRequest
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxTransmit_ClearTransmitRequest function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_ClearTransmitRequest(Com_TxPduInfoIterType ComTxPduId);

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_Transmission_DesGwTransmit
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxTransmit_DesGwTransmit function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId              Valid ID of TxPdu that should be sent
    \param[in]    isImmediate             true, if TxPdu should be sent immediately, otherwise transmission will be triggered
                                          in next main function cycle
    \param[in]    withoutRepetitionFlag   true, if no repetitions are required
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec    
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_DesGwTransmit(Com_TxPduInfoIterType ComTxPduId, boolean isImmediate, boolean withoutRepetitionFlag);
# endif

/**********************************************************************************************************************
  Com_Transmission_ClearUpdateBits
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxBuffer_ClearUpdateBits function.
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
FUNC(void, COM_CODE) Com_Transmission_ClearUpdateBits(Com_TxPduInfoIterType ComTxPduId);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_Transmission_SetUpdateBitGwDescriptionAccess
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxBuffer_SetUpdateBitGwDescriptionAccess function.
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
FUNC(void, COM_CODE) Com_Transmission_SetUpdateBitGwDescriptionAccess(Com_GwDescriptionAccessInfoIterType idxGwDescriptionAccessInfo);
#endif

/**********************************************************************************************************************
  Com_Transmission_GetAddrTxBufferFirstElement
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxBuffer_GetAddrTxBufferFirstElement function.
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
FUNC(Com_TxBufferPtrType, COM_CODE) Com_Transmission_GetAddrTxBufferFirstElement(Com_TxBufferInfoIterType txBufferInfoIdx);

/**********************************************************************************************************************
  Com_Transmission_GetTxBufferLength
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxBuffer_GetTxBufferLength function.
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
FUNC(Com_TxBufferLengthOfTxBufferInfoType, COM_CODE) Com_Transmission_GetTxBufferLength(Com_TxBufferInfoIterType txBufferInfoIdx);

/**********************************************************************************************************************
  Com_Transmission_CancelTransmit
**********************************************************************************************************************/
/*! \brief        This function calls the Com_LLTxIf_CancelTransmit function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of I-PDU to be transmitted.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_CancelTransmit(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_Transmission_ResetTxTpConnection
**********************************************************************************************************************/
/*! \brief        This function calls the Com_LLTxTp_ResetTxTpConnection function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of I-PDU whose TP connection shall be reset.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_ResetTxTpConnection(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_Transmission_StopTxIpdus
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxModeHdlr_StopTxIpdus function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \trace        SPEC-2736842
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_StopTxIpdus(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_Transmission_SwitchIpduTxMode
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxModeHdlr_SwitchIpduTxMode function.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \param[in]    Mode        Tx Mode to be set.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_SwitchIpduTxMode(Com_TxPduInfoIterType ComTxPduId, boolean Mode);

/**********************************************************************************************************************
Com_Transmission_IsDirectTxMode
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxModeHdlr_IsDirectTxMode function.
    \details      -
    \pre          -
    \param[in]    TxPduId               Id of Tx ComIPdu.
    \return       boolean
                    TRUE if current tx mode is DIRECT
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Transmission_IsDirectTxMode(Com_TxPduInfoIterType TxPduId);
#ifdef __PRQA__
#pragma PRQA_NO_SIDE_EFFECTS Com_Transmission_IsDirectTxMode
#endif

/**********************************************************************************************************************
  Com_Transmission_IsPeriodicTxMode
**********************************************************************************************************************/
/*! \brief        This function calls the Com_TxModeHdlr_IsPeriodicTxMode function.
    \details      -
    \pre          -
    \return       boolean
                    TRUE if current tx mode is periodic
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires TxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Transmission_IsPeriodicTxMode(Com_TxPduInfoIterType TxPduId);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TRANSMISSION_H */

/**********************************************************************************************************************
  END OF FILE: Com_Transmission.h
**********************************************************************************************************************/
