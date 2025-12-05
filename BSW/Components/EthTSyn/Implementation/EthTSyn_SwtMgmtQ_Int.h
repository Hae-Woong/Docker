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
/*!        \file  EthTSyn_SwtMgmtQ_Int.h
 *         \unit  SwtMgmtQ
 *        \brief  EthTSyn internal header file of the SwtMgmtQ unit.
 *      \details  Interface and type definitions of the SwtMgmtQ unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SWT_MGMT_Q_INT_H)
# define ETHTSYN_SWT_MGMT_Q_INT_H

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
 *  EthTSyn_SwtMgmtQ_Init
 *********************************************************************************************************************/
/*! \brief        Initializes the SwtMgmtQ unit.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_MainFunction
 *********************************************************************************************************************/
/*! \brief        Processes the SwtMgmtQ unit.
 *  \details      -
 *  \pre          Must be called in the context of EthTSyn_SwtMgmt_MainFunction().
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_PreparePush
 *********************************************************************************************************************/
/*! \brief        Checks if SwtMgmtQ has a free buffer available. Blocks queue for other users if available.
 *  \details      -
 *  \param[in]    SwtMgmtQIdx        SwtMgmtQ instance index
 *                                   [range: SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ()]
 *  \param[in]    SwtMgmtQUserType   User type of the SwtMgmtQ instance (SwtMgmtRx or SwtMgmtTx)
 *                                   [range: ETHTSYN_SWTMGMTRX_SWTMGMTOBJUSEROFSWTMGMTQBUF ||
 *                                           ETHTSYN_SWTMGMTTXDYN_SWTMGMTOBJUSEROFSWTMGMTQBUF]
 *  \param[in]    InvSwtMgmtQUserIdx Invalid index of SwtMgmtQ user
 *  \return       InvSwtMgmtQUserIdx - No free buffer found for SwtMgmtQ user
 *  \return       Other values - Index of the SwtMgmtQ user instance associated to the free SwtMgmtQ buffer
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \note         After this function, always call either EthTSyn_SwtMgmtQ_Push() or EthTSyn_SwtMgmtQ_CancelPush()
 *                for the same SwtMgmtQ instance to unblock the queue.
 *  \spec
 *    requires SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ();
 *    requires (SwtMgmtQUserType == ETHTSYN_SWTMGMTRX_SWTMGMTOBJUSEROFSWTMGMTQBUF) ||
 *             (SwtMgmtQUserType == ETHTSYN_SWTMGMTTXDYN_SWTMGMTOBJUSEROFSWTMGMTQBUF);
 *  \endspec
 *********************************************************************************************************************/
FUNC(EthTSyn_SwtMgmtQ_BufUserIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_PreparePush(
  EthTSyn_SwtMgmtQ_IdxType        SwtMgmtQIdx,
  EthTSyn_SwtMgmtQ_BufUserType    SwtMgmtQUserType,
  EthTSyn_SwtMgmtQ_BufUserIdxType InvSwtMgmtQUserIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_Push
 *********************************************************************************************************************/
/*! \brief        Stores switch management object pointer in queue and unblocks queue for other users.
 *  \details      -
 *  \param[in]    SwtMgmtQIdx      SwtMgmtQ instance index
 *                                 [range: SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ()]
 *  \param[in]    EthSwtMgmtObjPtr Ethernet switch management object
 *                                 [range: EthSwtMgmtObjPtr != NULL_PTR]
 *  \pre          EthTSyn_SwtMgmtQ_PreparePush() must be called successfully for the same SwtMgmtQ instance before
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ();
 *    requires EthSwtMgmtObjPtr != NULL_PTR
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_Push(
  EthTSyn_SwtMgmtQ_IdxType             SwtMgmtQIdx,
  EthTSyn_SwtMgmt_EthSwtMgmtObjPtrType EthSwtMgmtObjPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtQ_CancelPush
 *********************************************************************************************************************/
/*! \brief        Unblocks queue for other users.
 *  \details      -
 *  \param[in]    SwtMgmtQIdx SwtMgmtQ instance index
 *                            [range: SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ()]
 *  \pre          EthTSyn_SwtMgmtQ_PreparePush() must be called successfully for the same SwtMgmtQ instance before
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires SwtMgmtQIdx < EthTSyn_GetSizeOfSwtMgmtQ();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtQ_CancelPush(
  EthTSyn_SwtMgmtQ_IdxType SwtMgmtQIdx);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
#endif /* ETHTSYN_SWT_MGMT_Q_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SwtMgmtRxMsgHndl_Int.h
 *********************************************************************************************************************/
