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
/*!        \file  EthTSyn_FupRxHandler_Int.h
 *         \unit  FupRxHandler
 *        \brief  EthTSyn internal header file of the FupRxHandler unit.
 *      \details  Interface and type definitions of the FupRxHandler unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_FUP_RX_HANDLER_INT_H)
# define ETHTSYN_FUP_RX_HANDLER_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_SLAVE_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
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
 *  EthTSyn_FupRxHandler_IsRcvdFupValid
 *********************************************************************************************************************/
/*! \brief      Checks if a received FollowUp message is valid with respect to its message structure.
 *  \details    Checks if the structure of the FollowUp message is valid (included CRC setting and value, if required).
 *  \param[in]  FupRxHandlerIdx    Index of the FupRxHandler
 *                                 [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]  PtpHdrPtr          Pointer to the de-serialized PTP header
 *  \param[in]  FupPayloadPtr      Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \return     TRUE - FollowUp message is valid and can be processed
 *  \return     FALSE - FollowUp message is invalid and most not be processed
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_IsRcvdFupValid(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)          PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                       FupPayloadPtr);

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_ProcRcvdFup
 *********************************************************************************************************************/
/*! \brief      Processes a received FollowUp message.
 *  \details    Processes the information to calculate the new global time and forward it to the StbM (if required).
 *              Provides the FollowUp data for time validation to the StbM (if required).
 *  \param[in]  FupRxHandlerIdx    Index of the FupRxHandler
 *                                 [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]  SyncRxTimeInfoPtr  Time information of Sync reception
 *  \param[in]  PtpHdrPtr          Pointer to the de-serialized PTP header
 *  \param[in]  FupPayloadPtr      Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \pre        Validity of the FollowUp message was verified via EthTSyn_FupRxHandler_IsRcvdFupValid()
 *  \pre        Has to be called in the same context as EthTSyn_FupRxHandler_IsRcvdFupValid()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ProcRcvdFup(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncRxTimeInfoType)  SyncRxTimeInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)          PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                       FupPayloadPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */
#endif /* ETHTSYN_FUP_RX_HANDLER_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_FupRxHandler_Int.h
 *********************************************************************************************************************/
