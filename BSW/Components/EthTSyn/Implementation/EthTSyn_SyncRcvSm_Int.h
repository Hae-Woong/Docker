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
/*!        \file  EthTSyn_SyncRcvSm_Int.h
 *         \unit  SyncRcvSm
 *        \brief  EthTSyn internal header file of the SyncRcvSm unit.
 *      \details  Interface and type definitions of the SyncRcvSm unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SYNC_RCV_SM_INT_H)
# define ETHTSYN_SYNC_RCV_SM_INT_H

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
 *  EthTSyn_SyncRcvSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the SyncRcvSm unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncRcvSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_MainFunction
 *********************************************************************************************************************/
/*! \brief      Processes all active Sync receive state machines.
 *  \details    -
 *  \pre        Must be called cyclically in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncRcvSm_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_Reset
 *********************************************************************************************************************/
/*! \brief      Triggers reset of the SyncRcvSm.
 *  \details    -
 *  \param[in]  SyncRcvSmIdx  Index of the sync receive state machine.
 *                            [range: SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm()]
 *  \pre        Must be called in context of EthTSyn_TrcvLinkStateChg()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncRcvSm_Reset(
  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_ProcRcvdSync
 *********************************************************************************************************************/
/*! \brief      Processing of a received Sync message.
 *  \details    Checks if the SyncRcvSm can receive a Sync message. Performs further validity checks of the received
 *              Sync message. Optionally processes the message.
 *  \param[in]  SyncRcvSmIdx      Index of the sync receive state machine.
 *                                [range: SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm()]
 *  \param[in]  PtpHdrPtr         Pointer to the de-serialized PTP header
 *  \param[in]  SyncRxTimeInfoPtr Time information of Sync reception
 *  \return     E_OK - Sync message processing by the SyncRcvSm succeeded
 *  \return     E_NOT_OK - Sync message processing by the SyncRcvSm failed
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncRcvSm_ProcRcvdSync(
                  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_SyncRxTimeInfoType) SyncRxTimeInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_ProcRcvdFup
 *********************************************************************************************************************/
/*! \brief      Processing of a received FollowUp message.
 *  \details    Checks if the SyncRcvSm can receive the FollowUp message (i.e. matching Sync was received before and
 *              no FollowUp reception timeout occurred). Optionally triggers processing of the message.
 *  \param[in]  SyncRcvSmIdx   Index of the sync receive state machine.
 *                             [range: SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm()]
 *  \param[in]  PtpHdrPtr      Pointer to the de-serialized PTP header
 *  \param[in]  FupPayloadPtr  Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \return     E_OK - FollowUp message processing by the SyncRcvSm succeeded
 *  \return     E_NOT_OK - FollowUp message processing by the SyncRcvSm failed
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncRcvSm_ProcRcvdFup(
                  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                      FupPayloadPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */
#endif /* ETHTSYN_SYNC_RCV_SM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SyncRcvSm_Int.h
 *********************************************************************************************************************/
