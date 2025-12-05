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
/*!        \file  EthTSyn_SyncSendTx_Int.h
 *         \unit  SyncSendTx
 *        \brief  EthTSyn internal header file of the SyncSendTx unit.
 *      \details  Interface and type definitions of the SyncSendTx unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SYNC_SEND_TX_INT_H)
# define ETHTSYN_SYNC_SEND_TX_INT_H

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
 *  EthTSyn_SyncSendTx_VerifyCfg
 *********************************************************************************************************************/
/*! \brief      Verifies the generated configuration data with respect to the SyncSendSm.
 *  \details    -
 *  \param[out] ErrorIdPtr   The corresponding error code in case the configuration data are invalid
 *  \return     TRUE - Configuration is valid.
 *  \return     FALSE - Configuration is invalid. The corresponding error code is returned via ErrorIdPtr.
 *  \pre        EthTSyn_ConfigDataPtr is already initialized.
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_SyncSendTx_VerifyCfg(
  ETHTSYN_P2VAR(uint8) ErrorIdPtr);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_TxSync
 *********************************************************************************************************************/
/*! \brief      Transmits Sync message.
 *  \details    Retrieves and returns the current global and virtual local time tuple from StbM before transmitting.
 *  \param[in]  SyncSendSmIdx   SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \param[in]  SequenceId      Sequence Id of Sync message.
 *  \param[out] SyncTxInfoPtr   Sync transmission time information.
 *  \return     E_OK - Sync message transmission successful
 *  \return     E_NOT_OK - Sync message transmission failed
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \note       Function is called with handle of SyncSendSm because SyncSendTx unit has no own generated data. Handle
 *              is only used to access ROM data of SyncSendSm.
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncSendTx_TxSync(
                EthTSyn_SyncSendSmIterType SyncSendSmIdx,
                uint16                     SequenceId,
  ETHTSYN_P2VAR(EthTSyn_SyncTxInfoType)    SyncTxInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_TxFup
 *********************************************************************************************************************/
/*! \brief      Serializes and transmits FollowUp message.
 *  \details    -
 *  \param[in]  SyncSendSmIdx     SyncSend state machine.
 *                                [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \param[in]  SyncTxInfoPtr     Sync transmission time information
 *  \param[in]  TypeSpecHeaderPtr Object containing all message type specific fields of the PTP header.
 *  \param[in]  PotPtr            Precise origin timestamp.
 *  \return     E_OK - FollowUp message transmission successful
 *  \return     E_NOT_OK - FollowUp message transmission failed
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \note       Function is called with handle of SyncSendSm because SyncSendTx unit has no own generated data. Handle
 *              is only used to access ROM data of SyncSendSm.
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncSendTx_TxFup(
                  EthTSyn_SyncSendSmIterType   SyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)      SyncTxInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)  TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PotPtr);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_CalcPot
 *********************************************************************************************************************/
/*! \brief      Calculates the precise origin timestamp from input data.
 *  \details    If hardware timestamping is enabled and StbM and EthIf use different time sources, the current Ethernet
 *              timestamp and StbM internal virtual local time must additionally be retrieved for the calculation.
 *  \param[in]  ComCtrlPortIdx   Index of the ComCtrl port.
 *                               [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]  SyncEgrIntVltPtr Sync egress timestamp as internal virtual local time.
 *  \param[in]  SyncTxInfoPtr    Sync transmission time information.
 *  \param[out] PotPtr           Precise origin timestamp.
 *  \return     E_OK - Precise origin timestamp was successfully calculated
 *  \return     E_NOT_OK - Precise origin timestamp could not be calculated
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *    requires SyncTxInfoPtr != NULL_PTR;
 *    requires PotPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncSendTx_CalcPot(
                  EthTSyn_ComCtrl_PortIdxType  ComCtrlPortIdx,
                  EthTSyn_IntVltType           SyncEgrIntVlt,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)      SyncTxInfoPtr,
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) PotPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */
#endif /* ETHTSYN_SYNC_SEND_TX_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SyncSendSm_Int.h
 *********************************************************************************************************************/
