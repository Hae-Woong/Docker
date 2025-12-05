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
/*!        \file  Com_RxSigBuffer.h
 *         \unit  RxSigBuffer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_RXSIGBUFFER_H)
# define COM_RXSIGBUFFER_H

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
  Com_RxSigBuffer_InitRxSignalBuffer
**********************************************************************************************************************/
/*! \brief        This function initializes the buffer of a Signal or GroupSignal.
    \details      -
    \pre          -
    \param[in]    rxAccessInfoIdx    Index of RxAccessInfo
    \context      TASK
    \trace        SPEC-2736775
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxAccessInfoIdx < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_InitRxSignalBuffer(Com_RxAccessInfoIterType rxAccessInfoIdx);

/**********************************************************************************************************************
  Com_RxSigBuffer_InitRxSignalGroupBuffer
**********************************************************************************************************************/
/*! \brief        This function initializes the buffer of a SignalGroup.
    \details      -
    \pre          -
    \param[in]    idxRxSigGrpInfo   Index of RxSigGrpInfo
    \context      TASK
    \trace        SPEC-2736775
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_InitRxSignalGroupBuffer(Com_RxSigGrpInfoIterType idxRxSigGrpInfo);

/**********************************************************************************************************************
  Com_RxSigBuffer_ReadSignal
**********************************************************************************************************************/
/*! \brief        This function extracts the ComSignal data from the passed PDU and copies it to the related tmp buffer.
    \details      -
    \pre          -
    \param[in]    PduInfoPtr          contains the SDU buffer and the SDU length.
    \param[in]    idxRxAccessInfo     Index of ComSignal.
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC) pointer to the tmp buffer
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_ReadSignal(CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxAccessInfoIterType idxRxAccessInfo);

/**********************************************************************************************************************
  Com_RxSigBuffer_ReadSignalGroup
**********************************************************************************************************************/
/*! \brief        This function extracts the signal group data from the given PDU and copies it to the related tmp buffer.
    \details      -
    \pre          -
    \param[in]    PduInfoPtr          contains the SDU buffer and the SDU length.
    \param[in]    idxRxSigGrpInfo     Index of RxSigGrpInfo.
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_ReadSignalGroup(CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxSigGrpInfoIterType idxRxSigGrpInfo);

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtr()
**********************************************************************************************************************/
/*! \brief         This function copies the value of a Signal from the RxSigBuffer to the given SignalDataPtr.
    \details       -
    \pre           -
    \param[in]     idxRxAccessInfo Index of RxAccessInfo.
    \param[out]    SignalDataPtr   Reference to the signal data in which to store the received data.
    \context       TASK|ISR2
    \reentrant     TRUE, for different handles
    \synchronous   TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalDataPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtr(Com_RxAccessInfoIterType idxRxAccessInfo, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtrDynSig()
**********************************************************************************************************************/
/*! \brief         This function copies the value of a dynamic length Signal from the RxSigBuffer to the given SignalDataPtr.
    \details       -
    \pre           -
    \param[in]     idxRxAccessInfo Index of RxAccessInfo.
    \param[out]    comDataPtr      Pointer to reference to the signal data in which to store the received data and length of the signal for dynamic signals.
    \context       TASK|ISR2
    \reentrant     TRUE, for different handles
    \synchronous   TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires SignalDataPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtrDynSig(Com_RxAccessInfoIterType idxRxAccessInfo, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalDataPtr, PduLengthType dynSignalLength);

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyFromRxSigBufferToSignalGroupArrayPtr()
**********************************************************************************************************************/
/*! \brief         This function copies the value of a signal group with array access enabled from the RxSigBuffer to the given SignalGroupArrayPtr.
    \details       -
    \pre           -
    \param[in]     idxRxSigGrpInfo       Index of RxSigGrpInfo.
    \param[out]    SignalGroupArrayPtr   Index of RxAccessInfo.
    \context       TASK|ISR2
    \reentrant     TRUE, for different handles
    \synchronous   TRUE
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
      requires SignalGroupArrayPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyFromRxSigBufferToSignalGroupArrayPtr(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr);

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyFromShadowBufferToRxSigBuffer()
**********************************************************************************************************************/
/*! \brief        This function copies the value of a GroupSignal from the ShadowBuffer to the RxSigBuffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo Index of RxAccessInfo.
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyFromShadowBufferToRxSigBuffer(Com_RxAccessInfoIterType idxRxAccessInfo);

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyToSigBuffer
**********************************************************************************************************************/
/*! \brief        This function copies the value from the newValuePtr (tmp buffer of the passed ComSignal) to the signal buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo   Handle Id of ComSignal
    \param[in]    comDataPtr        Pointer to a pointer to the memory where the tmp signal value is located and the length of the signal for dynamic length signals
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyToSigBuffer(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyToShdwBuffer
**********************************************************************************************************************/
/*! \brief        This function copies the value from the tmp buffer of the passed ComSignalGroup to the shadow buffer.
    \details      -
    \pre          -
    \param[in]    idxRxSigGrpInfo   Handle Id of ComSignalGroup
    \param[in]    PduInfoPtr        contains the SDU buffer and the SDU length.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyToShdwBuffer(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/**********************************************************************************************************************
  Com_RxSigBuffer_GetSigBufferPtr
**********************************************************************************************************************/
/*! \brief        This method returns a pointer to the datatype dependent signal buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo                     Index of Com Signal
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC) Pointer to the datatype dependent signal buffer
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_GetSigBufferPtr(Com_RxAccessInfoIterType idxRxAccessInfo);

/**********************************************************************************************************************
  Com_RxSigBuffer_GetShdBufferPtr
**********************************************************************************************************************/
/*! \brief        This method returns a pointer to the datatype dependent shadow buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo                      Index of Com Group Signal
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC)  Pointer to the datatype dependent shadow buffer
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_GetShdBufferPtr(Com_RxAccessInfoIterType idxRxAccessInfo);

/**********************************************************************************************************************
  Com_RxSigBuffer_GetTmpGrpSigBufferPtr
**********************************************************************************************************************/
/*! \brief        This method returns a pointer to the datatype dependent tmp buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo                     Index of Com Group Signal
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC) Pointer to the datatype dependent tmp buffer
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_GetTmpGrpSigBufferPtr(Com_RxAccessInfoIterType idxRxAccessInfo);

/**********************************************************************************************************************
  Com_RxSigBuffer_SetRxInitValue
**********************************************************************************************************************/
/*! \brief        This function copies the ApplType dependent Init value to the signal or shadow buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo Index of Com Signal or Group Signal
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxAccessInfoIdx < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_SetRxInitValue(Com_RxAccessInfoIterType rxAccessInfoIdx);

/**********************************************************************************************************************
  Com_RxSigBuffer_SetRxDataTimeoutSubValue
**********************************************************************************************************************/
/*! \brief        This function copies the ApplType dependent RxDataTimeoutSubstitution value to the signal or shadow buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo Index of Com Signal or Group Signal
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxAccessInfoIdx < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_SetRxDataTimeoutSubValue(Com_RxAccessInfoIterType rxAccessInfoIdx);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_RxSigBuffer_CopySigToMinProcGwBuffer
**********************************************************************************************************************/
/*! \brief        This function copies the value of the tmp buffer of the passed ComSignal to the MinProcGwBuffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo   Handle Id of ComSignal
    \param[in]    newValuePtr       Pointer to the memory where the tmp signal value is located
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopySigToMinProcGwBuffer(Com_RxAccessInfoIterType idxRxAccessInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) newValuePtr);
#endif

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_RxSigBuffer_CopySigGrpToMinProcGwBuffer
**********************************************************************************************************************/
/*! \brief        This function copies the value of the tmp buffer of the passed ComSignalGroup to the MinProcGwBuffer.
    \details      -
    \pre          -
    \param[in]    idxRxSigGrpInfo   Handle Id of ComSignalGroup
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopySigGrpToMinProcGwBuffer(Com_RxSigGrpInfoIterType idxRxSigGrpInfo);
#endif

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_RxSigBuffer_GetGatewaySourcePtr
**********************************************************************************************************************/
/*! \brief        This function returns the pointer to the source buffer of a gateway routing.
    \details      The source buffer can be the signal buffer, shadow buffer or MinProcGwBuffer.
    \param[in]    idxRxAccessInfo   Index of RxAccessInfo.
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC) Pointer to the datatype dependent buffer
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_GetGatewaySourcePtr(Com_RxAccessInfoIterType idxRxAccessInfo);
#endif

/**********************************************************************************************************************
  Com_RxSigBuffer_GetRxDynSignalLength
**********************************************************************************************************************/
/*! \brief        This function returns the length of the dyn signal.
    \details      -
    \pre          -
    \param[in]    rxAccessInfoIdx     RxAccessInfo index
    \return       PduLengthType length of dyn signal
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxAccessInfoIdx < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxSigBuffer_GetRxDynSignalLength(Com_RxAccessInfoIterType rxAccessInfoIdx);

/**********************************************************************************************************************
  Com_RxSigBuffer_GetRxDynamicLength
**********************************************************************************************************************/
/*! \brief        This function returns the current dynamic signal length, dependent on the passed SDU length.
    \details      -
    \pre          -
    \param[in]    sduLength           SDU length
    \param[in]    rxAccessInfoIdx     RxAccessInfo index
    \return       PduLengthType       current dynamic signal length.
    \trace        SPEC-2736904
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxAccessInfoIdx < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxSigBuffer_GetRxDynamicLength(PduLengthType sduLength, Com_RxAccessInfoIterType rxAccessInfoIdx);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXSIGBUFFER_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxSigBuffer.h
**********************************************************************************************************************/
