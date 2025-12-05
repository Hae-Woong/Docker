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
/*!        \file  EthTSyn_PdReqTrcv_Int.h
 *         \unit  PdReqTrcv
 *        \brief  EthTSyn internal header file of the PdReqTrcv unit.
 *      \details  Interface and type definitions of the PdReqTrcv unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_PD_REQ_RX_INT_H)
# define ETHTSYN_PD_REQ_RX_INT_H

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
 *  EthTSyn_PdReqTrcv_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the PdReqTrcv unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_CancelCycle
 *********************************************************************************************************************/
/*! \brief      Cancels an ongoing Pdelay cycle and and resets the internal state of the PdReqTrcv.
 *  \details    -
 *  \param[in]  PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                            [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_CancelCycle(
  EthTSyn_PdReqTrcvDynIterType PdReqTrcvIdx);

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_TxPdReq
 *********************************************************************************************************************/
/*! \brief      Triggers transmission of a Pdelay_Req message.
 *  \details    -
 *  \param[in]  PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                            [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \param[in]  SequenceId    The sequence id of the Pdelay cycle
 *  \return     E_OK - Transmission of Pdelay_Req message succeeded
 *  \return     E_NOT_OK - Transmission of Pdelay_Req message failed
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdReqTrcv_TxPdReq(
  EthTSyn_PdReqTrcvDynIterType PdReqTrcvIdx,
  uint16                       SequenceId);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_ProvidePdReqEgrTs
 *********************************************************************************************************************/
/*! \brief          Provides the egress timestamp of a transmitted Pdelay_Req message to the PdReqTrcv.
 *  \details        -
 *  \param[in]      PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                                [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \param[in]      EgrTsInfoPtr  The Pdelay_Req egress timestamps
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_ProvidePdReqEgrTs(
                  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)       EgrTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_ProcRcvdPdResp
 *********************************************************************************************************************/
/*! \brief      Processes a received Pdelay_Resp message.
 *  \details    -
 *  \param[in]  PdReqTrcvIdx       Index of the PdReqTrcv instance.
 *  \param[in]  PtpHdrPtr          Pointer to the de-serialized PTP header
 *  \param[in]  PdRespPayloadPtr   Pointer to the Pdelay_Resp payload (still serialized in Network-Byte-Order)
 *  \param[in]  RxTsInfoPtr        Info about the ingress timestamp(s)
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *   requires PtpHdrPtr != NULL_PTR;
 *   requires PdRespPayloadPtr != NULL_PTR;
 *   requires RxTsInfoPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_ProcRcvdPdResp(
                  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)       PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                    PdRespPayloadPtr,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)       RxTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_ProcRcvdPdRespFup
 *********************************************************************************************************************/
/*! \brief      Processes a received Pdelay_Resp_Fup message.
 *  \details    -
 *  \param[in]  PdReqTrcvIdx          Index of the PdReqTrcv instance.
 *  \param[in]  PtpHdrPtr             Pointer to the de-serialized PTP header
 *  \param[in]  PdRespFupPayloadPtr   Pointer to the Pdelay_Resp_Fup payload (still serialized in Network-Byte-Order)
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_ProcRcvdPdRespFup(
                  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)       PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                    PdRespFupPayloadPtr);

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_GetRespRespFupData
 *********************************************************************************************************************/
/*! \brief      Retrieve all Pdelay_Resp/Pdelay_Resp_Fup data required for pdelay calculation and optional time
 *              validation reporting.
 *  \details    -
 *  \param[in]  PdReqTrcvIdx          Index of the PdReqTrcv instance.
 *                                    [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \param[out] PdRespRespFupDataPtr  Pdelay_Resp/Pdelay_Resp_Fup data
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_GetRespRespFupData(
                EthTSyn_PdReqTrcvDynIterType   PdReqTrcvIdx,
  ETHTSYN_P2VAR(EthTSyn_PdRespRespFupDataType) PdRespRespFupDataPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */
#endif /* ETHTSYN_PD_REQ_RX_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PdReqRx_Int.h
 *********************************************************************************************************************/
