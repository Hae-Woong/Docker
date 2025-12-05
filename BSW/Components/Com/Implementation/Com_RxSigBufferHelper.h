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
/*!        \file  Com_RxSigBufferHelper.h
 *         \unit  RxSigBufferHelper
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_RXSIGBUFFERHELPER_H)
# define COM_RXSIGBUFFERHELPER_H

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
  Com_RxSigBufferHelper_CopyToSigBuffer
**********************************************************************************************************************/
/*! \brief        This function copies the value from the given newValuePtr to the datatype specific signal buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo   Index of RxAccessInfo
    \param[in]    idxRxBufferInfo   Index of RxBufferInfo
    \param[in]    comDataPtr        Pointer to a pointer to the memory where the tmp signal value is located and the length of the signal for dynamic length signals
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires idxRxBufferInfo < Com_GetSizeOfRxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBufferHelper_CopyToSigBuffer(Com_RxAccessInfoIterType idxRxAccessInfo, Com_RxBufferInfoIterType idxRxBufferInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_GetSigBufferPtr
**********************************************************************************************************************/
/*! \brief        This method returns a pointer to Com_RxSigBuffer according to the datatype of the signal.
    \details      -
    \pre          -
    \param[in]    idxRxBufferInfo                     Index of RxBufferInfo
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC) Pointer to the memory where the signal value is located
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxBufferInfo < Com_GetSizeOfRxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(P2VAR(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBufferHelper_GetSigBufferPtr(Com_RxBufferInfoIterType idxRxBufferInfo);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_GetTmpSigBufferPtr
**********************************************************************************************************************/
/*! \brief        This method returns a pointer to Com_RxTmpSigBuffer according to the datatype of the signal.
    \details      -
    \pre          -
    \param[in]    idxRxBufferInfo                     Index of RxBufferInfo
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC) Pointer to the memory where the tmp signal value is located
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxBufferInfo < Com_GetSizeOfRxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(P2VAR(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBufferHelper_GetTmpSigBufferPtr(Com_RxBufferInfoIterType idxRxBufferInfo);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_ReadSigOrGrpSignal
**********************************************************************************************************************/
/*! \brief        This function extracts the signal or group signal data from the passed Sdu data and copies it to the related tmp buffer.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo Index of RxAccessInfo.
    \param[in]    PduInfoPtr        contains the SDU buffer and the SDU length
    \param[in]    idxRxBufferInfo   Index of RxBufferInfo
    \param[in]    dynLength         Length of a dynamic length signal (zero if it isn't a dynamic length signal)
    \return       P2CONST(void, AUTOMATIC, AUTOMATIC) Pointer to the memory where the tmp signal value is located
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
      requires PduInfoPtr != NULL_PTR;
      requires idxRxBufferInfo < Com_GetSizeOfRxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(P2VAR(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBufferHelper_ReadSigOrGrpSignal(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxBufferInfoIterType idxRxBufferInfo, PduLengthType dynLength);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_SetRxInitValue
**********************************************************************************************************************/
/*! \brief        This function copies the ApplType dependent Init value to the RxSigBuffer.
    \details      -
    \pre          IsUsed check required: Com_IsInitValueUsedOfRxBufferInfo()
    \param[in]    idxRxAccessInfo Index of Com Signal or Group Signal
    \param[in]    idxBufferInfo   Index of RxSigBufferInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxAccessInfoIdx < Com_GetSizeOfRxAccessInfo();
      requires idxBufferInfo < Com_GetSizeOfRxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBufferHelper_SetRxInitValue(Com_RxAccessInfoIterType rxAccessInfoIdx, Com_RxBufferInfoIterType idxBufferInfo);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_SetRxDataTimeoutSubValue
**********************************************************************************************************************/
/*! \brief        This function copies the ApplType dependent RxDataTimeoutSubstitution value to the RxSigBuffer.
    \details      -
    \pre          IsUsed check required: Com_IsRxDataTimeoutSubstitutionValueUsedOfRxBufferInfo()        -
    \param[in]    idxRxAccessInfo Index of Com Signal or Group Signal
    \param[in]    idxBufferInfo   Index of RxSigBufferInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxAccessInfoIdx < Com_GetSizeOfRxAccessInfo();
      requires idxBufferInfo < Com_GetSizeOfRxBufferInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBufferHelper_SetRxDataTimeoutSubValue(Com_RxAccessInfoIterType rxAccessInfoIdx, Com_RxBufferInfoIterType idxBufferInfo);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_SetRxDynSignalLength
**********************************************************************************************************************/
/*! \brief        This function sets the length of the dyn signal to a new value.
    \details      -
    \pre          Com_IsRxDynSignalLengthUsedOfRxAccessInfo(rxAccessInfoIdx) is checked before calling this function
    \param[in]    idxRxAccessInfo   ID of RxAccessInfo
    \param[in]    dynLength   new dyn length value to be set
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBufferHelper_SetRxDynSignalLength(Com_RxAccessInfoIterType idxRxAccessInfo, PduLengthType dynLength);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_GetRxDynSignalLength
**********************************************************************************************************************/
/*! \brief        This function returns the length of the dyn signal.
    \details      -
    \pre          Com_IsRxDynSignalLengthUsedOfRxAccessInfo(rxAccessInfoIdx) is checked before calling this function
    \param[in]    idxRxAccessInfo   ID of RxAccessInfo
    \return       PduLengthType length of dyn signal
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxSigBufferHelper_GetRxDynSignalLength(Com_RxAccessInfoIterType idxRxAccessInfo);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_SetRxDynSignalTmpLengthForSignalGroups
**********************************************************************************************************************/
/*! \brief        This function sets the length of the dyn signal group to a new value.
    \details      -
    \pre          Com_IsRxDynSignalTmpLengthForSignalGroupsUsedOfRxAccessInfo(idxRxAccessInfo) is checked before calling this function
    \param[in]    idxRxAccessInfo   ID of RxAccessInfo
    \param[in]    dynLength         new dyn length value to be set
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBufferHelper_SetRxDynSignalTmpLengthForSignalGroups(Com_RxAccessInfoIterType idxRxAccessInfo, PduLengthType dynLength);

/**********************************************************************************************************************
  Com_RxSigBufferHelper_GetRxDynSignalTmpLengthForSignalGroups
**********************************************************************************************************************/
/*! \brief        This function returns the length of the dyn signal group.
    \details      -
    \pre          Com_IsRxDynSignalTmpLengthForSignalGroupsUsedOfRxAccessInfo(idxRxAccessInfo) is checked before calling this function
    \param[in]    idxRxAccessInfo      ID of RxAccessInfo
    \return       PduLengthType        length of dyn signal
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxSigBufferHelper_GetRxDynSignalTmpLengthForSignalGroups(Com_RxAccessInfoIterType idxRxAccessInfo);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXSIGBUFFERHELPER_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxSigBufferHelper.h
**********************************************************************************************************************/
