/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  EthTSyn_Tx_Int.h
 *         \unit  Tx
 *        \brief  EthTSyn internal header file of the Tx unit.
 *      \details  Interface and type definitions of the Tx unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_TX_INT_H)
# define ETHTSYN_TX_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_TX_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define ETHTSYN_START_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_Tx_StartTxSequence
 *********************************************************************************************************************/
/*! \brief        Starts the transmission sequence for a PTP message.
 *  \details      Retrieves a Tx buffer from EthIf, serializes the message header and copies it to the
 *                Ethernet Tx buffer. Prepares subsequent TxConfirmation if required.
 *  \param[in]    ComCtrlPortIdx    Index of the port
 *                                  [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]    TxConfContextIdx  Index of the associated TxConf context. If < EthTSyn_GetSizeOfTxConfContext(),
 *                                  an egress timestamp and therefore a Tx confirmation is expected
 *  \param[in]    TypeSpecHeaderPtr Object containing all message type specific fields of the PTP header.
 *  \param[in]    PortInfoHeaderPtr Object containing all port information of the PTP header. If it is NULL_PTR,
 *                                  port information must be retrieved from ComCtrl unit.
 *                                  [range: can be NULL_PTR]
 *  \param[out]   TxBufMgmtPtr      Information about the Ethernet Tx buffer for later processing. Only valid
 *                                  when E_OK is returned. The following information is included:
 *                                  - Pointer to the Ethernet Tx buffer (start of PTP message, i.e. header)
 *                                  - Total length of the granted Ethernet Tx buffer
 *                                  - Index of the buffer required for later transmission
 *  \return       E_OK - Successfully started transmission sequence
 *  \return       E_NOT_OK - Failed to start transmission sequence
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Tx_StartTxSequence(
                  EthTSyn_ComCtrl_PortIdxType     ComCtrlPortIdx,
                  EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)     TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType)     PortInfoHeaderPtr,
    ETHTSYN_P2VAR(EthTSyn_EthTxBufferStructType)  TxBufMgmtPtr);

/**********************************************************************************************************************
 *  EthTSyn_Tx_FinishTxSequence
 *********************************************************************************************************************/
/*! \brief        Finishes the transmission sequence for a PTP message.
 *  \details      Triggers message transmission and releases the Ethernet Tx buffer in case of any failure.
 *  \param[in]    ComCtrlPortIdx    Index of the port
 *                                  [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]    TxConfContextIdx  Index of the associated TxConf context. If < EthTSyn_GetSizeOfTxConfContext(),
 *                                  an egress timestamp and therefore a Tx confirmation is expected
 *  \param[in]    EthTxBufIdx       Index of the Ethernet Tx buffer which was alloced for the Tx sequence
 *  \param[in]    TxLenByte         Length of the PTP message in byte
 *  \return       E_OK - Successfully finalized transmission sequence
 *  \return       E_NOT_OK - Failed to finalize transmission sequence. Transmission sequence was terminated. For a
 *                           retry, a new sequence needs to be started via EthTSyn_Tx_StartTxSequence()
 *  \pre          Transmission sequence was successfully started via EthTSyn_Tx_StartTxSequence()
 *  \pre          Message was copied to the Ethernet Tx buffer (in Network-Byte-Order) provided by
 *                EthTSyn_Tx_StartTxSequence()
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Tx_FinishTxSequence(
  EthTSyn_ComCtrl_PortIdxType     ComCtrlPortIdx,
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
  uint8                           EthTxBufIdx,
  uint16                          TxLenByte);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* ETHTSYN_TX_SUPPORT == STD_ON */
#endif /* ETHTSYN_TX_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Tx_Int.h
 *********************************************************************************************************************/
