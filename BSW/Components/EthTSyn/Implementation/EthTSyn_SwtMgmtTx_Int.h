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
/*!        \file  EthTSyn_SwtMgmtTx_Int.h
 *         \unit  SwtMgmtTx
 *        \brief  EthTSyn internal header file of the SwtMgmtTx unit.
 *      \details  Interface and type definitions of the SwtMgmtTx unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SWT_MGMT_TX_INT_H)
# define ETHTSYN_SWT_MGMT_TX_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
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
 * EthTSyn_SwtMgmtTx_Init
 *********************************************************************************************************************/
/*! \brief          Initializes the SwtMgmtTx unit.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_ReleasePendingMgmtObjsOfCtrl
 *********************************************************************************************************************/
/*! \brief      Releases all pending switch management objects of the passed EthIf controller.
 *  \details    -
 *  \param[in]  ComCtrlEthIfCtrlIdx   Index of the ComCtrlEthIfCtrl.
 *                                    [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_ReleasePendingMgmtObjsOfCtrl(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_PrepTx
 *********************************************************************************************************************/
/*! \brief      Prepares frame transmission in a bridge configuration.
 *  \details    Handles additional preparation steps (setting switch management info and enabling switch egress
 *              timestamping) in a bridge configuration.
 *  \param[in]  ComCtrlPortIdx  Index of ComCtrl port.
 *                              [ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]  BufIdx          Index of the buffer resource
 *  \param[in]  IsEgrTsRequired Indication if a switch egress timestamp is required
 *  \return     E_OK - Transmission preparation succeeded
 *  \return     E_NOT_OK - Transmission preparation failed
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_PrepTx(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  uint8                       BufIdx,
  boolean                     IsEgrTsRequired);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_CalcPotAndCorrField
 *********************************************************************************************************************/
/*! \brief      Calculates precise origin timestamp and correction field.
 *  \details    Calculates the POT and correction field depending on consideration of host and switch as one
 *              time-aware system.
 *  \param[in]  ComCtrlPortIdx Index of ComCtrl port.
 *                             [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]  EgrTsInfoPtr   Information about the transmission egress timestamp(s).
 *  \param[in]  SyncTxInfoPtr  Sync transmission time information.
 *  \param[out] PotPtr         Precise origin timestamp.
 *  \param[out] CorrFieldNs    Correction field in nanoseconds.
 *  \return     E_OK - Calculation succeeded
 *  \return     E_NOT_OK - Calculation failed
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_CalcPotAndCorrField(
                  EthTSyn_ComCtrl_PortIdxType  ComCtrlPortIdx,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)          EgrTsInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)      SyncTxInfoPtr,
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) PotPtr,
    ETHTSYN_P2VAR(uint64)                      CorrFieldNs);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_ReleasePendingMgmtObj
 *********************************************************************************************************************/
/*! \brief      Releases switch management object.
 *  \details    -
 *  \param[in]  SwtMgmtTxIdx Index of the SwtMgmtTx instance.
 *                           [range: SwtMgmtTxIdx < EthTSyn_GetSizeOfSwtMgmtTxDyn()]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires SwtMgmtTxIdx < EthTSyn_GetSizeOfSwtMgmtTxDyn();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_ReleasePendingMgmtObj(
  EthTSyn_SwtMgmtTx_IdxType SwtMgmtTxIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_HndlTxConf
 *********************************************************************************************************************/
/*! \brief      Handles switch egress timestamping upon TxConfirmation.
 *  \details    -
 *  \param[in]  ComCtrlEthIfCtrlIdx Index of the ComCtrlEthIfCtrl.
 *                                  [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \param[in]  BufIdx              Index of the buffer resource
 *  \param[in]  TxConfContextIdx    Index of TxConf context
 *                                  [range: TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext()]
 *  \pre        Must be called in context of the EthTSyn_TxConfirmation()
 *  \return     EthTSyn_GetSizeOfSwtMgmtTxDyn() - No free SwtMgmtTx instance available. No switch timestamps will be
 *                                                provided.
 *  \return     other values - Index of the used SwtMgmtTx instance. Switch timestamps will be provided later.
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()
 *    requires TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext()
 *  \endspec
 *********************************************************************************************************************/
FUNC(EthTSyn_SwtMgmtTx_IdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_HndlTxConf(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx,
  uint8                            BufIdx,
  EthTSyn_SizeOfTxConfContextType  TxConfContextIdx);

/**********************************************************************************************************************
 * EthTSyn_SwtMgmtTx_ProcTs
 *********************************************************************************************************************/
/*! \brief      Triggers processing of the switch timestamps.
 *  \details    -
 *  \param[in]  SwtMgmtTxIdx      Index of the SwtMgmtTx instance.
 *                                [range: SwtMgmtTxIdx < EthTSyn_GetSizeOfSwtMgmtTxDyn()]
 *  \param[in]  SwtIngrTsMgmtPtr  Switch ingress timestamp (at host port) with validity information
 *  \param[in]  SwtEgrTsMgmtPtr   Switch egress timestamp with validity information
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SwtMgmtTxIdx < EthTSyn_GetSizeOfSwtMgmtTxDyn();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_ProcTs(
                  EthTSyn_SwtMgmtTx_IdxType SwtMgmtTxIdx,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)       SwtIngrTsMgmtPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)       SwtEgrTsMgmtPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
#endif /* ETHTSYN_SWT_MGMT_TX_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SwtMgmtTx_Int.h
 *********************************************************************************************************************/
