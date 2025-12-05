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
/*!        \file  Com_TxSignalIf.h
 *         \unit  TxSignalIf
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXSIGNALIF_H)
# define COM_TXSIGNALIF_H

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
  Com_TxSignalIf_SendSignalGroup_Processing
**********************************************************************************************************************/
/*! \brief        The service Com_TxSignalIf_SendSignalGroup_Processing copies the content of the associated shadow buffer to
                  the associated I-PDU buffer.
    \details      -
    \pre          Prior to this call, all group signals should be updated in the shadow buffer by the call of Com_SendSignal.
                  Interrupts must have been locked by the calling function.
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
FUNC(uint8, COM_CODE) Com_TxSignalIf_SendSignalGroup_Processing(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) ShadowBufferPtr);

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignal_Processing
**********************************************************************************************************************/
/*! \brief        The function Com_TxSignalIf_SendSignal_Processing forwards the signal or groupSignal to the TxSignalProcessing or TxGroupSignalProcessing unit.
    \details      -
    \pre          Interrupts must have been locked by the calling function.
    \note         This function does not provide a sign conversion to be compatible with gateway use case.
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
FUNC(uint8, COM_CODE) Com_TxSignalIf_SendSignal_Processing(Com_TxAccessInfoIterType txAccessInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroupArray_Processing
**********************************************************************************************************************/
/*! \brief        The service Com_SendSignalGroupArray copies the content of the provided SignalGroupArrayPtr to the
 *                associated I-PDU. The provided data shall correspond to the array representation of the signal group.
    \details      -
    \pre          The configuration switch ComEnableSignalGroupArrayApi has to be enabled.
                  Interrupts must have been locked by the calling function.
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
FUNC(uint8, COM_CODE) Com_TxSignalIf_SendSignalGroupArray_Processing(Com_TxSigGrpInfoIterType SignalGroupId, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXSIGNALIF_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxSignalIf.h
**********************************************************************************************************************/
