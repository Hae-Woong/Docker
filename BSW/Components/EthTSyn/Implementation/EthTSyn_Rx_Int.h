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
/*!        \file  EthTSyn_Rx_Int.h
 *         \unit  Rx
 *        \brief  EthTSyn internal header file of the Rx unit.
 *      \details  Interface and type definitions of the Rx unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_RX_INT_H)
# define ETHTSYN_RX_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

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
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Rx_ProcRcvdMsg
 *********************************************************************************************************************/
/*! \brief        Triggers processing of a received EthTSyn message.
 *  \details      -
 *  \param[in]    ComCtrlPortIdx   Index of the ComCtrlPort
 *                                 [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]    DataPtr          Pointer to payload of the received Ethernet frame
 *                                 (i.e. Ethernet header is not provided)
 *  \param[in]    RxTsInfoPtr      Info about the reception timestamp(s) as well as optional Ethernet and StbM time
 *  \pre          Message type of the received message is valid, i.e. Sync, FollowUp, Announce, Pdelay_Req,
 *                Pdelay_Resp or Pdelay_Resp_Fup
 *  \pre          The RxBuffer is big enough to hold the complete message (length indicated in the message header)
 *  \pre          Must be called in context of EthTSyn_SwtMgmt_MainFunction()
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \config       ETHTSYN_SWT_MGMT_SUPPORT
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *    requires RxTsInfoPtr != NULL_PTR;
 *    requires DataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Rx_ProcRcvdMsg(
                  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  ETHTSYN_P2CONST(uint8)                      DataPtr,
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)       RxTsInfoPtr);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_RX_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Rx_Int.h
 *********************************************************************************************************************/
