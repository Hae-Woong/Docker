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
/*!        \file  EthTSyn_PdReqSm_Int.h
 *         \unit  PdReqSm
 *        \brief  EthTSyn internal header file of the PdReqSm unit.
 *      \details  Interface and type definitions of the PdReqSm unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_PD_REQ_SM_INT_H)
# define ETHTSYN_PD_REQ_SM_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
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
 *  EthTSyn_PdReqSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes all Pdelay request state machines.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_MainFunction
 *********************************************************************************************************************/
/*! \brief      Processes all Pdelay request state machines.
 *  \details    -
 *  \pre        Must be called cyclically in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_Reset
 *********************************************************************************************************************/
/*! \brief      Triggers reset of the Pdelay request state machine.
 *  \details    -
 *  \param[in]  PdReqSmIdx   Index of the pdelay request state machine.
 *  \pre        Must be called in context of EthTSyn_TrcvLinkStateChg()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_Reset(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_LostResponse
 *********************************************************************************************************************/
/*! \brief      Informs Pdelay request state machine about a lost response.
 *  \details    AsCapable state is reset if the configured number of tolerated lost responses is reached.
 *  \param[in]  PdReqSmIdx   Index of the pdelay request state machine.
 *                           [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_LostResponse(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_RcvdPdResp
 *********************************************************************************************************************/
/*! \brief      Informs Pdelay request state machine about reception of Pdelay_Resp.
 *  \details    Resets Pdelay_Resp/Pdelay_Resp_Fup timeout counter.
 *  \param[in]  PdReqSmIdx   Index of the pdelay request state machine.
 *                           [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_RcvdPdResp(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_RcvdPdRespFup
 *********************************************************************************************************************/
/*! \brief      Informs Pdelay request state machine about reception of Pdelay_Resp_Fup.
 *  \details    -
 *  \param[in]  PdReqSmIdx   Index of the pdelay request state machine.
 *                           [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_RcvdPdRespFup(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_RespRespFupDataReady
 *********************************************************************************************************************/
/*! \brief      Informs Pdelay request state machine that all required data are available.
 *  \details    -
 *  \param[in]  PdReqSmIdx   Index of the pdelay request state machine.
 *                           [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_RespRespFupDataReady(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_GetSequenceId
 *********************************************************************************************************************/
/*! \brief      Returns current sequence Id.
 *  \details    -
 *  \param[in]  PdReqSmIdx   Index of the pdelay request state machine.
 *                           [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \return     Current sequence Id
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint16, ETHTSYN_CODE) EthTSyn_PdReqSm_GetSequenceId(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */
#endif /* ETHTSYN_PD_REQ_SM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PdReqSm_Int.h
 *********************************************************************************************************************/
