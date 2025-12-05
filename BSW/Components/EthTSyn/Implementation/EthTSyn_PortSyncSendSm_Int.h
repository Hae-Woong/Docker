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
/*!        \file  EthTSyn_PortSyncSendSm_Int.h
 *         \unit  PortSyncSendSm
 *        \brief  EthTSyn internal header file of the PortSyncSendSm unit.
 *      \details  Interface and type definitions of the PortSyncSendSm unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_PORT_SYNC_SEND_SM_INT_H)
# define ETHTSYN_PORT_SYNC_SEND_SM_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_MASTER_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
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
 *  EthTSyn_PortSyncSendSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes all variable data of the PortSyncSendSm unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_TxSync
 *********************************************************************************************************************/
/*! \brief      Transmits a Sync message and optionally invokes Sync sent callback.
 *  \details    -
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \param[in]  TypeSpecHeaderPtr  Object containing all message type specific fields of the PTP header.
 *  \param[in]  PortInfoHeaderPtr  Object containing all port information of the PTP header.
 *                                 [range: can be NULL_PTR]
 *  \return     E_OK - Sync message transmitted successfully
 *  \return     E_NOT_OK - Transmission of Sync message failed
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_TxSync(
  EthTSyn_PortSyncSendSm_SmIdxType            PortSyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType) TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType) PortInfoHeaderPtr);

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_ProvideSyncEgrTs
 *********************************************************************************************************************/
/*! \brief      Stores the egress timestamp(s) of the transmitted sync message and their state(s).
 *  \details    -
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \param[in]  EgrTsInfoPtr       Info about the transmission egress timestamp(s).
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_ProvideSyncEgrTs(
  EthTSyn_PortSyncSendSm_SmIdxType    PortSyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType) EgrTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_GetSyncEgrTs
 *********************************************************************************************************************/
/*! \brief      Gets the egress timestamp(s) of the transmitted sync message and their state(s).
 *  \details    -
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \param[out] EgrTsInfoPtr       Info about the transmission egress timestamp(s). Only valid if E_OK returned.
 *  \return     E_OK - Egress timestamps are available
 *  \return     E_NOT_OK - Egress timestamps were not provided to PortSyncSendSm yet: EgrTsInfoPtr is invalid
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_GetSyncEgrTs(
  EthTSyn_PortSyncSendSm_SmIdxType  PortSyncSendSmIdx,
  ETHTSYN_P2VAR(EthTSyn_TsInfoType) EgrTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_StartTxFupSequence
 *********************************************************************************************************************/
/*! \brief      Starts the transmission sequence of a FollowUp message.
 *  \details    Retrieves Ethernet Tx buffer from Tx unit containing the serialized FollowUp message header.
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \param[in]  TypeSpecHeaderPtr  Object containing all message type specific fields of the PTP header.
 *  \param[in]  PortInfoHeaderPtr  Object containing all port information of the PTP header.
 *                                 [range: can be NULL_PTR]
 *  \param[out] TxBufMgmtPtr       Information about the Ethernet Tx buffer for later processing. Only valid if E_OK
 *                                 returned. The following information is included:
 *                                 - Pointer to the Ethernet Tx buffer (start of PTP message, i.e. header)
 *                                 - Total length of the granted Ethernet Tx buffer
 *                                 - Index of the buffer required for later transmission
 *  \return     E_OK - FollowUp message transmission sequence was successfully started
 *  \return     E_NOT_OK - FollowUp message transmission sequence was not started: TxBufMgmtPtr is invalid
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_StartTxFupSequence(
  EthTSyn_PortSyncSendSm_SmIdxType               PortSyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)    TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType)    PortInfoHeaderPtr,
    ETHTSYN_P2VAR(EthTSyn_EthTxBufferStructType) TxBufMgmtPtr);

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_FinishTxFupSequence
 *********************************************************************************************************************/
/*! \brief      Finishes the transmission sequence of a FollowUp message.
 *  \details    Transmits FollowUp message and invokes optional FollowUp sent callback.
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \param[in]  TxBufMgmtPtr       Information about the Ethernet Tx buffer. The following information is included:
 *                                 - Pointer to the Ethernet Tx buffer (start of PTP message, i.e. header)
 *                                 - Total length of the granted Ethernet Tx buffer
 *                                 - Index of the buffer required for later transmission
 *  \param[in]  FupLenByte         Length of the FollowUp message in byte.
 *  \return     E_OK - FollowUp message transmitted successfully
 *  \return     E_NOT_OK - Transmission of FollowUp message failed
 *  \pre        Transmission sequence was successfully started via EthTSyn_PortSyncSendSm_StartTxFupSequence()
 *  \pre        Message was copied to the Ethernet Tx buffer (in Network-Byte-Order) provided by
 *              EthTSyn_PortSyncSendSm_StartTxFupSequence()
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_FinishTxFupSequence(
  EthTSyn_PortSyncSendSm_SmIdxType               PortSyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_EthTxBufferStructType) TxBufMgmtPtr,
                  uint16                         FupLenByte);

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_CancelCycle
 *********************************************************************************************************************/
/*! \brief      Cancels an active Sync/FollowUp cycle.
 *  \details    -
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \pre        Must not be called while transmission sequence is ongoing (i.e. between calls to
 *              EthTSyn_PortSyncSendSm_StartTxFupSequence and EthTSyn_PortSyncSendSm_FinishTxFupSequence). Otherwise,
 *              retrieved Ethernet Tx buffer will never be released again.
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_CancelCycle(
  EthTSyn_PortSyncSendSm_SmIdxType PortSyncSendSmIdx);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */
#endif /* ETHTSYN_PORT_SYNC_SEND_SM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PortSyncSendSm_Int.h
 *********************************************************************************************************************/
