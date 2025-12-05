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
/*!        \file  EthTSyn_AnnounceSendSm_Int.h
 *         \unit  AnnounceSendSm
 *        \brief  EthTSyn internal header file of the AnnounceSendSm unit.
 *      \details  Interface and type definitions of the AnnounceSendSm unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_ANNOUNCE_SEND_SM_INT_H)
# define ETHTSYN_ANNOUNCE_SEND_SM_INT_H

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
 *  EthTSyn_AnnounceSendSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the AnnounceSendSm unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_AnnounceSendSm_MainFunction
 *********************************************************************************************************************/
/*! \brief      Processes all active Announce send state machines.
 *  \details    -
 *  \pre        Must be called cyclically in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_AnnounceSendSm_Reset
 *********************************************************************************************************************/
/*! \brief      Resets the passed Announce send state machine.
 *  \details    -
 *  \param[in]  AnnounceSendSmIdx   AnnounceSend state machine. Invalid handle might be passed.
 *  \pre        Must be called in context of EthTSyn_TrcvLinkStateChg(
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_Reset(
  EthTSyn_AnnounceSendSm_SmIdxType AnnounceSendSmIdx);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */
#endif /* ETHTSYN_ANNOUNCE_SEND_SM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_AnnounceSendSm_Int.h
 *********************************************************************************************************************/
