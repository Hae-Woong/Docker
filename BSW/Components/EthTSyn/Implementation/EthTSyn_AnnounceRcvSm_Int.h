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
/*!        \file  EthTSyn_AnnounceRcvSm_Int.h
 *         \unit  AnnounceRcvSm
 *        \brief  EthTSyn internal header file of the AnnounceRcvSm unit.
 *      \details  Interface and type definitions of the AnnounceRcvSm unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_ANNOUNCE_RCV_SM_INT_H)
# define ETHTSYN_ANNOUNCE_RCV_SM_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Cfg.h"
# if (ETHTSYN_SLAVE_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
#  include "EthTSyn_Types_Int.h"
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
 *  EthTSyn_AnnounceRcvSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes variable data of all Announce receive state machines.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_MainFunction
 *********************************************************************************************************************/
/*! \brief      Processes all Announce receive state machines.
 *  \details    -
 *  \pre        Must be called cyclically in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_Reset
 *********************************************************************************************************************/
/*! \brief      Resets an Announce receive state machine.
 *  \details    -
 *  \param[in]  AnnounceRcvSmIdx   Index of the Announce receive state machine.
 *  \pre        Must be called in context of EthTSyn_TrcvLinkStateChg()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_Reset(
  EthTSyn_AnnounceRcvSm_SmIdxType AnnounceRcvSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_ProcRcvdAnnounce
 *********************************************************************************************************************/
/*! \brief      Processes a received announce message.
 *  \details    -
 *  \param[in]  AnnounceRcvSmIdx    Index of the Announce receive state machine.
 *  \param[in]  PtpHdrPtr           Pointer to the de-serialized PTP header.
 *  \param[in]  AnnouncePayloadPtr  Pointer to the Announce payload (still serialized in Network-Byte-Order).
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different AnnounceRcvSmIdx
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_ProcRcvdAnnounce(
                  EthTSyn_AnnounceRcvSm_SmIdxType AnnounceRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)             PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                          AnnouncePayloadPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */
#endif /* ETHTSYN_ANNOUNCE_RCV_SM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_AnnounceRcvSm_Int.h
 *********************************************************************************************************************/
