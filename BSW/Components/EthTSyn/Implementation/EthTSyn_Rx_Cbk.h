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
/*!        \file  EthTSyn_Rx_Cbk.h
 *         \unit  Rx
 *        \brief  EthTSyn callback header file of the Rx unit.
 *      \details  Callback definitions of the Rx unit.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_RX_CBK_H)
# define ETHTSYN_RX_CBK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types.h"
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
 *  EthTSyn_RxIndication
 *********************************************************************************************************************/
/*! \brief        Notification about a received EthTSyn frames.
 *  \details      By this API service the EthTSyn gets an indication and the data of a received frame.
 *  \param[in]    CtrlIdx      EthIf controller index
 *  \param[in]    FrameType    Frame type of received Ethernet frame
 *  \param[in]    IsBroadcast  Broadcast indication:
 *                             FALSE - frame is not a broadcast frame
 *                             TRUE - frame is a broadcast frame
 *  \param[in]    PhysAddrPtr  Pointer to the Physical source address (MAC address in network byte order) of received
 *                             Ethernet frame
 *  \param[in]    DataPtr      Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \param[in]    LenByte      Length of received data
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_RxIndication(
                uint8         CtrlIdx,
                Eth_FrameType FrameType,
                boolean       IsBroadcast,
  ETHTSYN_P2VAR(uint8)        PhysAddrPtr,
  ETHTSYN_P2VAR(uint8)        DataPtr,
                uint16        LenByte);

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_RX_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Rx_Cbk.h
 *********************************************************************************************************************/
