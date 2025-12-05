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
/*!        \file  Com_LLTxTp.h
 *         \unit  LLTxTp
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_LLTXTP_H)
# define COM_LLTXTP_H

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
  Com_LLTxTp_CopyTxDataProcessing
**********************************************************************************************************************/
/*! \brief        This function is called by Com_CopyTxData() to hand a to be transmitted TP segment to the PduR.
                  The Com copies the to be transmitted segment to the given buffer.
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
FUNC(BufReq_ReturnType, COM_CODE) Com_LLTxTp_CopyTxDataProcessing(Com_TxPduInfoIterType PduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                                  CONSTP2CONST(RetryInfoType, AUTOMATIC, AUTOMATIC) RetryInfoPtr, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) TxDataCntPtr);

/**********************************************************************************************************************
  Com_LLTxTp_TpTxConfirmationProcessing
**********************************************************************************************************************/
/*! \brief        This function is called by Com_TpTxCOnfirmation() after a TP I-PDU has been transmitted to indicate
                  the end of the transmission.
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
FUNC(void, COM_CODE) Com_LLTxTp_TpTxConfirmationProcessing(Com_TxPduInfoIterType PduId, Std_ReturnType Result);

/**********************************************************************************************************************
  Com_LLTxTp_ResetTxTpConnection
**********************************************************************************************************************/
/*! \brief        This function resets the TxTP state machine
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of I-PDU whose TP connection shall be reset.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLTxTp_ResetTxTpConnection(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_LLTxTp_IsTpUsed
**********************************************************************************************************************/
/*! \brief        This function returns whether TxTp is used or not for the current IPdu ID.
    \details      -
    \pre          -
    \param[in]    ComTxPduId      ID of Tx I-PDU .
    \return       boolean  TRUE   TxTpInfo is used for the current IPdu Id
                           FALSE  TxTpInfo isn't used for the current IPdu Id
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_LLTxTp_IsTpUsed(Com_TxPduInfoIterType ComTxPduId);
#ifdef __PRQA__
#pragma PRQA_NO_SIDE_EFFECTS Com_LLTxTp_IsTpUsed
#endif

/**********************************************************************************************************************
  Com_LLTxTp_SetTxTpConnectionState
**********************************************************************************************************************/
/*! \brief        This function sets the TxTpConnectionState to a new value.
    \details      -
    \pre          IsUsed check required: Com_IsTxTpInfoUsedOfTxPduInfo()
    \param[in]    ComTxPduId      ID of Tx I-PDU .
    \param[in]    state           The state to be set.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLTxTp_SetTxTpConnectionState(Com_TxPduInfoIterType ComTxPduId, Com_TxTpConnectionStateType state);

/**********************************************************************************************************************
  Com_LLTxTp_IsTpStateReady
**********************************************************************************************************************/
/*! \brief        This function returns if the TP state machine is Ready or not.
    \details      -
    \pre          IsUsed check required: Com_IsTxTpInfoUsedOfTxPduInfo()
    \param[in]    ComTxPduId          Tx PDU index
    \return       boolean             TRUE: Tp is ready, otherwise FALSE
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_LLTxTp_IsTpStateReady(Com_TxPduInfoIterType ComTxPduId);
#ifdef __PRQA__
#pragma PRQA_NO_SIDE_EFFECTS Com_LLTxTp_IsTpStateReady
#endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_LLTXTP_H */

/**********************************************************************************************************************
  END OF FILE: Com_LLTxTp.h
**********************************************************************************************************************/
