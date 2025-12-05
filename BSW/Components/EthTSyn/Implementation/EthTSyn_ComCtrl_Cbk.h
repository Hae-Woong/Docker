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
/*!        \file  EthTSyn_ComCtrl_Cbk.h
 *         \unit  ComCtrl
 *        \brief  EthTSyn callback header file of the ComCtrl unit.
 *      \details  Callback definitions of the ComCtrl unit.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_COM_CTRL_CBK_H)
# define ETHTSYN_COM_CTRL_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthIf_Types.h"

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

/**********************************************************************************************************************
 *  EthTSyn_TrcvLinkStateChg
 *********************************************************************************************************************/
/*! \brief        Callback function that notifies a changed state of the transceiver link
 *  \details      Allows resetting state machine in case of unexpected Link loss to avoid inconsistent
                  Sync and Follow_Up sequences
 *  \param[in]    CtrlIdx        Index of the Ethernet controller
 *  \param[in]    TrcvLinkState  New link state of the transceiver
 *                               ETHTRCV_LINK_STATE_DOWN - Link Down
 *                               ETHTRCV_LINK_STATE_ACTIVE - Link Up
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_TrcvLinkStateChg(
  uint8                 CtrlIdx,
  EthTrcv_LinkStateType TrcvLinkState);

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_COM_CTRL_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_ComCtrl_Cbk.h
 *********************************************************************************************************************/
