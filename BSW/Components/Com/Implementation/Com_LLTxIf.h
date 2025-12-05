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
/*!        \file  Com_LLTxIf.h
 *         \unit  LLTxIf
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_LLTXIF_H)
# define COM_LLTXIF_H

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
  Com_LLTxIf_Transmit
**********************************************************************************************************************/
/*! \brief        This function wraps the function PduR_ComTransmit. The I-PDU is transmitted immediate.
    \details      -
    \pre          -
    \param[in]    ComTxPduId      ID of I-PDU to be transmitted.
    \return       Std_ReturnType
                    E_OK          Transmit request has been accepted
                    E_NOT_OK      E_NOT_OK Transmit request has not been accepted
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \trace        SPEC-2736903, SPEC-2736898, SPEC-2736899, SPEC-2737062
    \context      TASK
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_LLTxIf_Transmit(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_LLTxIf_CancelTransmit
**********************************************************************************************************************/
/*! \brief        This function wraps the call of PduR_ComCancelTransmit(), if configured, for the given Tx I-PDU.
                  If required, the ID is converted to the PduR handle space.
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
FUNC(void, COM_CODE) Com_LLTxIf_CancelTransmit(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_LLTxIf_TxConfirmationProcessing
**********************************************************************************************************************/
/*! \brief        The lower layer communication interface module confirms the transmission of an I-PDU.
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
FUNC(void, COM_CODE) Com_LLTxIf_TxConfirmationProcessing(Com_TxPduInfoIterType TxPduId);

/**********************************************************************************************************************
  Com_LLTxIf_TriggerTransmitProcessing
**********************************************************************************************************************/
/*! \brief            Within this function, the upper layer module (called module) shall copy its data into the buffer
                      provided by PduInfoPtr->SduDataPtr and update the length of the actual copied data in PduInfoPtr->SduLength.
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
FUNC(Std_ReturnType, COM_CODE) Com_LLTxIf_TriggerTransmitProcessing(Com_TxPduInfoIterType TxPduId, CONSTP2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_LLTXIF_H */
/**********************************************************************************************************************
  END OF FILE: Com_LLTxIf.h
**********************************************************************************************************************/
