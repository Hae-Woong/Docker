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
/*!        \file  Com_TxGroupSignalProcessing.h
 *         \unit  TxGroupSignalProcessing
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_TXGROUPSIGNALPROCESSING_H)
# define COM_TXGROUPSIGNALPROCESSING_H

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
  Com_TxGroupSignalProcessing_ProcessGroupSignal
**********************************************************************************************************************/
/*! \brief        This function updates the group signal object with the group signal data referenced by the
                  SignalDataPtr parameter. The Length parameter is evaluated for dynamic length signals.
    \details      -
    \pre          -
    \param[in]    txGrpSigId    Index of group signal to be sent.
    \param[in]    comDataPtr    Pointer to Reference to the group signal data to be transmitted and length of the dynamic length group signal.
    \return       uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                                (or service failed due to development error)
                    COM_BUSY                    in case the TP-Buffer is locked for large data types handling
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \trace        SPEC-2736898, SPEC-2736899
    \spec
      requires comDataPtr != NULL_PTR;
      requires txGrpSigId < Com_GetSizeOfTxGrpSigInfo();
    \endspec
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxGroupSignalProcessing_ProcessGroupSignal(Com_TxGrpSigInfoIterType txGrpSigId, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_TxGroupSignalProcessing_GetTxTmpPduLength
**********************************************************************************************************************/
/*! \brief        This function gets the tx tmp pdu length.
    \details      -
    \pre          -
    \param[in]    txAccessInfoIdx        Id of Tx Signal
    \return       Com_TxSduLengthType length of tx tmp pdu
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires txAccessInfoIdx < Com_GetSizeOfTxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_TxSduLengthType, COM_CODE) Com_TxGroupSignalProcessing_GetTxTmpPduLength(Com_TxAccessInfoIterType txAccessInfoIdx);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_TXGROUPSIGNALPROCESSING_H */
/**********************************************************************************************************************
  END OF FILE: Com_TxGroupSignalProcessing.h
**********************************************************************************************************************/
