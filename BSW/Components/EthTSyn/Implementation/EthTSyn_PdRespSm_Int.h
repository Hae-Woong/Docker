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
/*!        \file  EthTSyn_PdRespSm_Int.h
 *         \unit  PdRespSm
 *        \brief  EthTSyn internal header file of the PdRespSm unit.
 *      \details  Interface and type definitions of the PdRespSm unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_PD_RESP_SM_INT_H)
# define ETHTSYN_PD_RESP_SM_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
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
 *  EthTSyn_PdRespSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the PdRespSm unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_MainFunction
 *********************************************************************************************************************/
/*! \brief      Processes all active Pdelay response state machines.
 *  \details    -
 *  \pre        Has to be called cyclically in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_Reset
 *********************************************************************************************************************/
/*! \brief      Triggers reset of the passed PdRespSm.
 *  \details    -
 *  \param[in]  PdRespSmIdx   Index of the PdRespSm
 *  \pre        Must be called in context of EthTSyn_TrcvLinkStateChg()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_Reset(
  EthTSyn_PdRespSm_SmIdxType PdRespSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_ProcRcvdPdReq
 *********************************************************************************************************************/
/*! \brief      Processes a received Pdelay_Req message.
 *  \details    Checks if passed port is ready to receive a Pdelay_Req message and optionally processes the message.
 *  \param[in]  PdRespIdx    Index of the PdRespSm
 *  \param[in]  PtpHdrPtr    Pointer to the de-serialized PTP header
 *  \param[in]  RxTsInfoPtr  Info about the reception timestamp(s)
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcRcvdPdReq(
                  EthTSyn_PdRespSm_SmIdxType  PdRespSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr,
  /* Note: Pdelay_Req does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)         RxTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_ProvidePdRespEgrTs
 *********************************************************************************************************************/
/*! \brief      Provide egress timestamp of the Pdelay_Resp to the passed PdRespSm.
 *  \details    -
 *  \param[in]  PdRespSmIdx   Index of the PdRespSm.
 *                            [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \param[in]  EgrTsInfoPtr  Info about the transmission timestamp(s) of the Pdelay_Resp
 *  \pre        Must be called in context of EthTSyn_TxConfirmation() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProvidePdRespEgrTs(
                  EthTSyn_PdRespSm_SmIdxType PdRespSmIdx,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)        EgrTsInfoPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */
#endif /* ETHTSYN_PD_RESP_SM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PdRespSm_Int.h
 *********************************************************************************************************************/
