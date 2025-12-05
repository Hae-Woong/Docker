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
/*!        \file  EthTSyn_TxConf_Int.h
 *         \unit  TxConf
 *        \brief  EthTSyn internal header file of the TxConf unit.
 *      \details  Interface and type definitions of the TxConf unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_TX_CONF_INT_H)
# define ETHTSYN_TX_CONF_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_TX_SUPPORT == STD_ON)
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
 *  EthTSyn_TxConf_Init
 *********************************************************************************************************************/
/*! \brief        Initializes all variable data of the Tx confirmation unit.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_SetupTxContext
 *********************************************************************************************************************/
/*! \brief        Assign passed buffer index to Tx confirmation context and activate context.
 *  \details      Prepares Tx confirmation context for later processing of the TxConfirmation callback.
 *  \param[in]    CtrlIdx          EthIf controller index
 *  \param[in]    TxConfContextIdx Index of the associated TxConf context. Invalid values are allowed.
 *  \param[in]    BufIdx           Index of the buffer resource
 *  \pre          BufIdx was retrieved from EthIf via EthIf_ProvideTxBuffer.
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_SetupTxContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
  uint8                           CtrlIdx,
  uint8                           BufIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ReleaseTxContext
 *********************************************************************************************************************/
/*! \brief        Inactivate Tx confirmation context.
 *  \details      -
 *  \param[in]    TxConfContextIdx Index of the associated TxConf context. Invalid values are allowed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ReleaseTxContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx);

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_TxConf_ProvideSwtTs
 *********************************************************************************************************************/
/*! \brief        Provides switch ingress and egress timestamps for the Tx confirmation context.
 *  \details      Dispatches timestamps to transmitting unit and releases Tx confirmation context.
 *  \param[in]    TxConfContextIdx Index of the associated TxConf context
 *                                 [range: TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext()]
 *  \param[in]    SwtIngrTsPtr     Switch ingress timestamp
 *  \param[in]    SwtEgrTsPtr      Switch egress timestamp
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ProvideSwtTs(
                  EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)             SwtIngrTsPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)             SwtEgrTsPtr);

#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* ETHTSYN_TX_SUPPORT == STD_ON */
#endif /* ETHTSYN_TX_CONF_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_TxConf_Int.h
 *********************************************************************************************************************/
