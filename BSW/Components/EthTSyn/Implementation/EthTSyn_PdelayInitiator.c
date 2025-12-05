/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  EthTSyn_PdelayInitiator.c
 *        \brief  EthTSyn PdelayInitiator source file
 *      \details  Implementation of all units included in the logical unit PdelayInitiator (PdReqSm and PdReqTrcv).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define ETHTSYN_PD_INITIATOR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_GeneratedCfgAccess.h"
#if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
# include "EthTSyn.h"
# include "EthTSyn_MsgDefs_Int.h"
# include "EthTSyn_Memory_Int.h"
# include "EthTSyn_PdReqSm_Int.h"
# include "EthTSyn_PdReqTrcv_Int.h"
# include "EthTSyn_ComCtrl_Int.h"
# include "EthTSyn_Timestamp_Int.h"
# include "EthTSyn_Tx_Int.h"
# include "EthTSyn_PortParamStats_Int.h"

# include "StbM.h"
# include "SchM_EthTSyn.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 * Internal function prototypes of PdReqSm
 *!
 * \unit PdReqSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PDREQSM)
/**********************************************************************************************************************
 * EthTSyn_PdReqSm_IntInit
 *********************************************************************************************************************/
/*! \brief          Initialize dynamic data of pdelay request state machine.
 *  \details        -
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_IntInit(
  EthTSyn_PdReqSmIterType PdReqSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_SetState
 *********************************************************************************************************************/
/*! \brief          Sets the state of a pdelay request state machine within a critical section that protects against
 *                  Tx and Rx interrupts.
 *  \details        -
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \param[in]      State         New state of the pdelay request state machine.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_SetState(
  EthTSyn_PdReqSmIterType PdReqSmIdx,
  EthTSyn_StateOfPdReqSmDynType State);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_IntLostResponse
 *********************************************************************************************************************/
/*! \brief          Increments lost response counter and sets AsCapable state to FALSE, if counter exceeds configured
 *                  threshold.
 *  \details        -
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre            Function call must be protected against Rx interrupts
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_IntLostResponse(
  EthTSyn_PdReqSmIterType PdReqSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcSm
 *********************************************************************************************************************/
/*! \brief          Processes a pdelay request state machine.
 *  \details        -
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre            Must be called cyclically in context of the EthTSyn_MainFunction()
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcSm(
  EthTSyn_PdReqSmIterType PdReqSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcState
 *********************************************************************************************************************/
/*! \brief          Processes the current state of the pdelay request state machine.
 *  \details        -
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre            Must be called cyclically by EthTSyn_PdReqSm_ProcSm()
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcState(
  EthTSyn_PdReqSmIterType PdReqSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcStateTxPdReq
 *********************************************************************************************************************/
/*! \brief          Processes the TX_PD_REQ state of the pdelay request state machine.
 *  \details        -
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre            Must be called by EthTSyn_PdReqSm_ProcState().
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcStateTxPdReq(
  EthTSyn_PdReqSmIterType PdReqSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcStateWaitRespRespFup
 *********************************************************************************************************************/
/*! \brief          Processes the WAIT_RESP_RESP_FUP state of the pdelay request state machine.
 *  \details        -
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre            Must be called cyclically by EthTSyn_PdReqSm_ProcState() if state machine is in state
 *                  WAIT_RESP_RESP_FUP.
 *  \pre            Must be called within the same critical section (with protection against Rx interrupts) in which
 *                  the state was checked by EthTSyn_PdReqSm_ProcState().
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcStateWaitRespRespFup(
  EthTSyn_PdReqSmIterType PdReqSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcStateComputePdelay
 *********************************************************************************************************************/
/*! \brief          Processes the COMPUTE_PDELAY state of the pdelay request state machine.
 *  \details        -
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \pre            Must be called by EthTSyn_PdReqSm_ProcState().
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcStateComputePdelay(
  EthTSyn_PdReqSmIterType PdReqSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ComputePdelay
 *********************************************************************************************************************/
/*! \brief          Tries to calculate new pdelay value.
 *  \details        -
 *  \param[in]      PdReqSmIdx      Index of the pdelay request state machine.
 *                                  [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \param[in]      RespRespFupData Timestamps required for pdelay calculation.
 *  \param[out]     Pdelay          New pdelay value.
 *  \return         E_OK - Pdelay calculation successful
 *  \return         E_NOT_OK - Pdelay calculation failed: do not use value of out-parameter
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdReqSm_ComputePdelay(
                  EthTSyn_PdReqSmIterType        PdReqSmIdx,
  ETHTSYN_P2CONST(EthTSyn_PdRespRespFupDataType) RespRespFupData,
    ETHTSYN_P2VAR(EthTSyn_PdelayType)            Pdelay);

#  if (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_PdReqSm_GetCurrentTime
 *********************************************************************************************************************/
/*! \brief          Tries to retrieve the current local and global time from StbM.
 *  \details        Retrieved timestamps are stored by the state machine.
 *  \param[in]      PdReqSmIdx    Index of the pdelay request state machine.
 *                                [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \return         E_OK - Time retrieval was successful
 *  \return         E_NOT_OK - Time retrieval failed: do not transmit Pdelay_Req message
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdReqSm_GetCurrentTime(
  EthTSyn_PdReqSmIterType PdReqSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ReportTimeValidData
 *********************************************************************************************************************/
/*! \brief          Reports time validation data to StbM
 *  \details        -
 *  \param[in]      PdReqSmIdx      Index of the pdelay request state machine.
 *                                  [range: PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm()]
 *  \param[in]      Pdelay          Current pdelay value.
 *  \param[in]      RespRespFupData Timestamps and responder source port Id to be reported.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ReportTimeValidData(
                  EthTSyn_PdReqSmIterType        PdReqSmIdx,
                  EthTSyn_PdelayType             Pdelay,
  ETHTSYN_P2CONST(EthTSyn_PdRespRespFupDataType) RespRespFupData);
#  endif /* (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON) */

# endif /* ETHTSYN_NOUNIT_PDREQSM */

/**********************************************************************************************************************
 * Internal function prototypes of PdReqTrcv
 *!
 * \unit PdReqTrcv
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PDREQTRCV)
/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_IntInit
 *********************************************************************************************************************/
/*! \brief      Initializes the PdReqTrcv instance.
 *  \param[in]  PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                            [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \details    -
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_IntInit(
  EthTSyn_PdReqTrcvDynIterType PdReqTrcvIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_Reset
 *********************************************************************************************************************/
/*! \brief          Resets the PdReqTrcv.
 *  \details        Resets all dynamic data of the passed PdReqTrcv and changes the state to IDLE. Takes care about
 *                  interrupt protection.
 *  \param[in]      PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                                [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_Reset(
  EthTSyn_PdReqTrcvDynIterType PdReqTrcvIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_SetStateTxIntrProtected
 *********************************************************************************************************************/
/*! \brief          Sets the PdReqTrcv state while protected against Ethernet transmission interrupts.
 *  \details        -
 *  \param[in]      PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                                [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \param[in]      NewState      The new state
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_SetStateTxIntrProtected(
  EthTSyn_PdReqTrcv_IdxType       PdReqTrcvIdx,
  EthTSyn_StateOfPdReqTrcvDynType NewState);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_SetStateEthIntrProtected
 *********************************************************************************************************************/
/*! \brief          Sets the PdReqTrcv state while protected against Ethernet interrupts.
 *  \details        -
 *  \param[in]      PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                                [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \param[in]      NewState      The new state
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_SetStateEthIntrProtected(
  EthTSyn_PdReqTrcvDynIterType    PdReqTrcvIdx,
  EthTSyn_StateOfPdReqTrcvDynType NewState);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_CheckStateTxIntrProtected
 *********************************************************************************************************************/
/*! \brief          Checks if the PdReqTrcv is in the desired state
 *  \details        Reads and checks the state of the PdReqTrcv while protected against Ethernet transmission
 *                  interrupts.
 *  \param[in]      PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                                [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \param[in]      DesiredState  The desired state
 *  \return         TRUE - PdReqTrcv is in the desired state
 *  \return         FALSE - PdReqTrcv is not in the desired state
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdReqTrcv_CheckStateTxIntrProtected(
  EthTSyn_PdReqTrcv_IdxType       PdReqTrcvIdx,
  EthTSyn_StateOfPdReqTrcvDynType DesiredState);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_GetPdRxTxTsMgmt
 *********************************************************************************************************************/
/*! \brief          Gets the timestamp and validity information relevant for Pdelay from the passed TsInfo.
 *  \details        Pdelay uses the internal timestamps, when EthTSyn is configured as end station.
 *                  When configured as bridge, the timestamps of the external switch ports are used.
 *  \param[in]      Dir        Indication if timestamp is required for RX or TX direction.
 *  \param[in]      TsInfoPtr  The whole timestamp information
 *  \return         The timestamp relevant for Pdelay and its validity information
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires TsInfoPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_TsMgmtType, ETHTSYN_CODE) EthTSyn_PdReqTrcv_GetPdRxTxTsMgmt(
                  EthTSyn_TsDirType   Dir,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType) TsInfoPtr);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_CheckRequestingPortIdentity
 *********************************************************************************************************************/
/*! \brief          Checks if a received Pdelay_Resp/Pdelay_Resp_Fup belongs to a Pdelay_Req of the own system.
 *  \details        The Pdelay_Resp and Pdelay_Resp_Fup both contain the requesting port identity at the same location
 *                  within the payload. This information is extracted and compared to the local port identity.
 *  \param[in]      PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                                [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \return         TRUE - Pdelay_Resp/Pdelay_Resp_Fup belongs to Pdelay_Req of own system
 *  \return         FALSE - Pdelay_Resp/Pdelay_Resp_Fup does not belong to Pdelay_Req of own system
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdReqTrcv_CheckRequestingPortIdentity(
                  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  ETHTSYN_P2CONST(uint8)                    PdPayloadPtr);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_GetLocalPortId
 *********************************************************************************************************************/
/*! \brief          Gets the local port identity (i.e. port identity of the own system).
 *  \details        -
 *  \param[in]      PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                                [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \return         The local port identity
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_PortIdentityType, ETHTSYN_CODE) EthTSyn_PdReqTrcv_GetLocalPortId(
  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_GetPdTsFromMsg
 *********************************************************************************************************************/
/*! \brief          Gets the request received/response origin timestamp from a received Pdelay_Resp/Pdelay_Resp_Fup.
 *  \details        The Pdelay_Resp and Pdelay_Resp_Fup both contain a Pdelay timestamp at the some location within
 *                  the payload. For a Pdelay_Resp this is the request received timestamp and for the Pdelay_Resp_Fup
 *                  it is the response origin timestamp.
 *  \param[in]      PdPayloadPtr  Pointer to the still serialized payload of the Pdelay_Resp/Pdelay_Resp_Fup
 *  \return         The extracted Pdelay timestamp
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_GlobalTimestampType, ETHTSYN_CODE) EthTSyn_PdReqTrcv_GetPdTsFromMsg(
  ETHTSYN_P2CONST(uint8) PdPayloadPtr);

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_CheckAndHndlDuplicatedResponse
 *********************************************************************************************************************/
/*! \brief          Checks for a duplicated Pdelay response and informs the PdReqSm about a lost response in case of
 *                  duplicated response.
 *  \details        -
 *  \param[in]      PdReqTrcvIdx  Index of the PdReqTrcv instance.
 *                                [range: PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn()]
 *  \param[in]      PdRespSeqId   Sequence id of the received response
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_CheckAndHndlDuplicatedResponse(
  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  uint16                    PdRespSeqId);
# endif /* ETHTSYN_NOUNIT_PDREQTRCV */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Internal functions of PdReqSm
 *!
 * \unit PdReqSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PDREQSM)
/**********************************************************************************************************************
 * EthTSyn_PdReqSm_IntInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_IntInit(
  EthTSyn_PdReqSmIterType PdReqSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_VirtualLocalTimeType vlt = {0u};
  StbM_TimeStampShortType ts = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize dynamic data of pdelay request state machine. */
  EthTSyn_SetStateOfPdReqSmDyn(PdReqSmIdx, ETHTSYN_INITIALIZING_STATEOFPDREQSMDYN);
  EthTSyn_SetSmResetRequestedOfPdReqSmDyn(PdReqSmIdx, FALSE);
  EthTSyn_SetFirstPdelayOfPdReqSmDyn(PdReqSmIdx, TRUE);
  EthTSyn_SetSequenceIdOfPdReqSmDyn(PdReqSmIdx, 0u);
  EthTSyn_SetTxIntervalCntOfPdReqSmDyn(PdReqSmIdx, 0u);
  EthTSyn_SetRespTimeoutCntOfPdReqSmDyn(PdReqSmIdx, 0u);
  EthTSyn_SetLostResponseCntOfPdReqSmDyn(PdReqSmIdx, 0u);
  EthTSyn_SetRefLocalTimeOfPdReqSmDyn(PdReqSmIdx, vlt);
  EthTSyn_SetRefGlobalTimeOfPdReqSmDyn(PdReqSmIdx, ts);
} /* EthTSyn_PdReqSm_IntInit() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_SetState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_SetState(
  EthTSyn_PdReqSmIterType PdReqSmIdx,
  EthTSyn_StateOfPdReqSmDynType State)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to protect against Rx and Tx interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 Set state of pdelay request state machine. */
  EthTSyn_SetStateOfPdReqSmDyn(PdReqSmIdx, State);

  /* #30 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_PdReqSm_SetState() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_IntLostResponse
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_IntLostResponse(
  EthTSyn_PdReqSmIterType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetLostResponseCntOfPdReqSmDyn(PdReqSmIdx) <= EthTSyn_GetMaxLostResponsesOfPdReqSm(PdReqSmIdx))
  {
    EthTSyn_IncLostResponseCntOfPdReqSmDyn(PdReqSmIdx);

    if (EthTSyn_GetLostResponseCntOfPdReqSmDyn(PdReqSmIdx) > EthTSyn_GetMaxLostResponsesOfPdReqSm(PdReqSmIdx))
    {
      const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx);

      EthTSyn_ComCtrl_SetAsCapable(comCtrlPortIdx, FALSE);

      EthTSyn_PortParamStats_IncPdLostRespExceededCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(comCtrlPortIdx));
    }
  }
} /* EthTSyn_PdReqSm_IntLostResponse() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcSm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_EthTSyn_6050_TrivialFunctions */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcSm(
  EthTSyn_PdReqSmIterType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if state machine reset is requested. Check and reset within critical section as protection against
   *     all Ethernet interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  if (EthTSyn_IsSmResetRequestedOfPdReqSmDyn(PdReqSmIdx) == TRUE)
  {
    const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx);

    /* #100 Reset state machine. */
    EthTSyn_PdReqSm_IntInit(PdReqSmIdx);

    /* #101 Reset transceiver, pdelay value and AsCapable state. */
    /* Using PdReqSmIdx to access PdReqTrcv unit is fine due to size relation. */
    EthTSyn_PdReqTrcv_CancelCycle(PdReqSmIdx);
    EthTSyn_ComCtrl_ResetPdelay(comCtrlPortIdx);
    EthTSyn_ComCtrl_SetAsCapable(comCtrlPortIdx, FALSE);
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 Decrement transmission interval counter if possible. */
  if (EthTSyn_GetTxIntervalCntOfPdReqSmDyn(PdReqSmIdx) > 0u)
  {
    EthTSyn_DecTxIntervalCntOfPdReqSmDyn(PdReqSmIdx);

    /* #30 Check if transmission interval counter is expired. */
    if (EthTSyn_GetTxIntervalCntOfPdReqSmDyn(PdReqSmIdx) == 0u)
    {
      /* #300 Enter critical section to protect against Rx and Tx interrupts. */
      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

      /* #301 Check if state is WAIT_RESP_RESP_FUP.
       *        Inform PortParamStats unit about missing reception of Pdelay_Resp or Pdelay_Resp_Fup.
       *        Increment and handle lost response counter. */
      if (EthTSyn_GetStateOfPdReqSmDyn(PdReqSmIdx) == ETHTSYN_WAIT_RESP_RESP_FUP_STATEOFPDREQSMDYN)
      {
        EthTSyn_PortParamStats_IncRxDiscardCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
          EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx)));

        EthTSyn_PdReqSm_IntLostResponse(PdReqSmIdx);
      }

      /* #302 Set state to TX_PD_REQ. */
      EthTSyn_SetStateOfPdReqSmDyn(PdReqSmIdx, ETHTSYN_TX_PD_REQ_STATEOFPDREQSMDYN);

      /* #303 Increment sequence Id. */
      EthTSyn_IncSequenceIdOfPdReqSmDyn(PdReqSmIdx);

      /* #304 Leave critical section. */
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

      /* #305 Cancel cycle of pdelay request transceiver. */
      /* Using PdReqSmIdx to access PdReqTrcv unit is fine due to size relation. */
      EthTSyn_PdReqTrcv_CancelCycle(PdReqSmIdx);
    }
  }

  /* #40 Process current state. */
  EthTSyn_PdReqSm_ProcState(PdReqSmIdx);
} /* EthTSyn_PdReqSm_ProcSm() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcState(
  EthTSyn_PdReqSmIterType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check current state within critical section to protect against Rx and Tx interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  switch (EthTSyn_GetStateOfPdReqSmDyn(PdReqSmIdx))
  {
    /* #20 State is INITIALIZING. */
  case ETHTSYN_INITIALIZING_STATEOFPDREQSMDYN:
    {
      const EthTSyn_ComCtrlPortIdxOfPdReqSmType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx);

      /* #200 Check if link state is active. */
      if (EthTSyn_ComCtrl_IsLinkStateActive(comCtrlPortIdx) == TRUE)
      {
        /* #2000 Set state to TX_PD_REQ. */
        EthTSyn_SetStateOfPdReqSmDyn(PdReqSmIdx, ETHTSYN_TX_PD_REQ_STATEOFPDREQSMDYN);
      }

      /* #201 Leave critical section. */
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
      break;
    }
    /* #30 State is TX_PD_REQ. */
  case ETHTSYN_TX_PD_REQ_STATEOFPDREQSMDYN:
    {
      /* #300 Leave critical section. */
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

      /* #301 Process state. */
      EthTSyn_PdReqSm_ProcStateTxPdReq(PdReqSmIdx);
      break;
    }
    /* #40 State is WAIT_RESP_RESP_FUP. */
  case ETHTSYN_WAIT_RESP_RESP_FUP_STATEOFPDREQSMDYN:
    {
      /* 400 Process state. */
      EthTSyn_PdReqSm_ProcStateWaitRespRespFup(PdReqSmIdx);

      /* #401 Leave critical section. */
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
      break;
    }
    /* #50 State is COMPUTE_PDELAY. */
  case ETHTSYN_COMPUTE_PDELAY_STATEOFPDREQSMDYN:
    {
      /* #500 Leave critical section. */
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

      /* #501 Process state. */
      EthTSyn_PdReqSm_ProcStateComputePdelay(PdReqSmIdx);
      break;
    }
    /* #60 State is WAIT_TX_PD_REQ or WAIT_REQ_EGR_TS. */
  default:
    {
      /* #600 Leave critical section. */
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
      break;
    }
  }
} /* EthTSyn_PdReqSm_ProcState() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcStateTxPdReq
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcStateTxPdReq(
  EthTSyn_PdReqSmIterType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to retrieve current global and local time from StbM (if time validation is enabled). */
#  if (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON)
  Std_ReturnType timeValidRetVal = E_OK;

  if (EthTSyn_IsTimeValidationEnabledOfPdReqSm(PdReqSmIdx))
  {
    timeValidRetVal = EthTSyn_PdReqSm_GetCurrentTime(PdReqSmIdx);
  }

  if (timeValidRetVal == E_OK)
#  endif /* (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON) */
  {
    /* #20 Reset response timeout counter and set state to WAIT_RESP_RESP_FUP before transmission of Pdelay_Req
     *     message. This is necessary because RxIndication and TxConfirmation interrupts can occur at any time directly
     *     after transmission. */
    EthTSyn_SetRespTimeoutCntOfPdReqSmDyn(PdReqSmIdx, EthTSyn_GetPdRespTimeoutOfPdReqSm(PdReqSmIdx));
    EthTSyn_PdReqSm_SetState(PdReqSmIdx, ETHTSYN_WAIT_RESP_RESP_FUP_STATEOFPDREQSMDYN);

    /* #30 Try to transmit Pdelay_Req message.
     *       Reset transmission interval counter.
     *       Report transmission of Pdelay_Req message to PortParamStats unit. */
    /* Using PdReqSmIdx to access PdReqTrcv unit is fine due to size relation. */
    if (EthTSyn_PdReqTrcv_TxPdReq(PdReqSmIdx, EthTSyn_GetSequenceIdOfPdReqSmDyn(PdReqSmIdx)) == E_OK)
    {
      EthTSyn_SetTxIntervalCntOfPdReqSmDyn(PdReqSmIdx, EthTSyn_GetPdReqPeriodOfPdReqSm(PdReqSmIdx));

      EthTSyn_PortParamStats_IncTxPdReqCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
        EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx)));
    }
    /* #31 Otherwise, reset state to TX_PD_REQ. */
    else
    {
      EthTSyn_PdReqSm_SetState(PdReqSmIdx, ETHTSYN_TX_PD_REQ_STATEOFPDREQSMDYN);
    }
  }
} /* EthTSyn_PdReqSm_ProcStateTxPdReq() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcStateWaitRespRespFup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcStateWaitRespRespFup(
  EthTSyn_PdReqSmIterType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Decrement response timeout counter, if possible, and check if counter expired. */
  if (EthTSyn_GetRespTimeoutCntOfPdReqSmDyn(PdReqSmIdx) > 0u)
  {
    EthTSyn_DecRespTimeoutCntOfPdReqSmDyn(PdReqSmIdx);

    if (EthTSyn_GetRespTimeoutCntOfPdReqSmDyn(PdReqSmIdx) == 0u)
    {
      /* #20 Report to PortParamStats unit that response timeout expired. */
      EthTSyn_PortParamStats_IncRxDiscardCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
        EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx)));

      /* #30 Set state to WAIT_TX_PD_REQ. */
      EthTSyn_PdReqSm_SetState(PdReqSmIdx, ETHTSYN_WAIT_TX_PD_REQ_STATEOFPDREQSMDYN);

      /* #40 Reset PdReqTrcv unit. */
      /* Using PdReqSmIdx to access PdReqTrcv unit is fine due to size relation. */
      EthTSyn_PdReqTrcv_CancelCycle(PdReqSmIdx);

      /* #50 Increment and handle lost response counter. */
      EthTSyn_PdReqSm_IntLostResponse(PdReqSmIdx);
    }
  }
} /* EthTSyn_PdReqSm_ProcStateWaitRespRespFup() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ProcStateComputePdelay
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ProcStateComputePdelay(
  EthTSyn_PdReqSmIterType PdReqSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx);
  EthTSyn_PdRespRespFupDataType respRespFupData = {0};
  EthTSyn_PdelayType pdelay = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Retrieve required data from transceiver unit. */
  /* Using PdReqSmIdx to access PdReqTrcv unit is fine due to size relation. */
  EthTSyn_PdReqTrcv_GetRespRespFupData(PdReqSmIdx, &respRespFupData);

  /* #20 Try to compute new pdelay value. */
  if (EthTSyn_PdReqSm_ComputePdelay(PdReqSmIdx, &respRespFupData, &pdelay) == E_OK)
  {
    /* #200 Store new pdelay value and set AsCapable state to TRUE. */
    EthTSyn_ComCtrl_SetAsCapable(comCtrlPortIdx, TRUE);
    EthTSyn_ComCtrl_SetPdelay(comCtrlPortIdx, pdelay);

    /* #201 Reset lost response counter. */
    EthTSyn_SetLostResponseCntOfPdReqSmDyn(PdReqSmIdx, 0u);

    /* #202 Report time validation data to StbM (if time validation is enabled). */
#  if (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON)
    if (EthTSyn_IsTimeValidationEnabledOfPdReqSm(PdReqSmIdx))
    {
      EthTSyn_PdReqSm_ReportTimeValidData(PdReqSmIdx, pdelay, &respRespFupData);
    }
#  endif /* (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON) */
  }
  /* #21 Otherwise, set AsCapable state to FALSE. */
  else
  {
    EthTSyn_ComCtrl_SetAsCapable(comCtrlPortIdx, FALSE);
  }

  /* #30 Set state to WAIT_TX_PD_REQ. */
  EthTSyn_PdReqSm_SetState(PdReqSmIdx, ETHTSYN_WAIT_TX_PD_REQ_STATEOFPDREQSMDYN);
} /* EthTSyn_PdReqSm_ProcStateComputePdelay() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ComputePdelay
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdReqSm_ComputePdelay(
                  EthTSyn_PdReqSmIterType        PdReqSmIdx,
  ETHTSYN_P2CONST(EthTSyn_PdRespRespFupDataType) RespRespFupData,
    ETHTSYN_P2VAR(EthTSyn_PdelayType)            Pdelay)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_UTimediffType tmpPdelay = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* computes the mean propagation delay on the link (see also IEEE802.1AS-2011 Figure 11-1) */
  /* t1: Pdelay_Req egress timestamp (initiator),  t2: Pdelay_Req ingress timestamp (responder) */
  /* t3: Pdelay_Resp egress timestamp (responder), t4: Pdelay_Resp ingress timestamp (initiator) */
  /* t4 - t1 : local residence time, t3 - t2 : remote residence time */
  /*      (t4 - t1) - (t3 - t2)      */
  /* D = ___________________________ */
  /*              2                  */

  /* #10 Check if retrieved timestamps are valid. */
  if ((RespRespFupData->PdReqEgrTsMgmt.TsValid == TRUE) && (RespRespFupData->PdRespIngrTsMgmt.TsValid == TRUE))
  {
    /* #20 Try to calculate local residence time. */
    EthTSyn_TimediffType localResTime = 0;
    if (EthTSyn_Ts_TsMinusTs(&RespRespFupData->PdRespIngrTsMgmt.Ts, &RespRespFupData->PdReqEgrTsMgmt.Ts,
         &localResTime) == E_OK)
    {
      /* #30 Try to calculate remote residence time. */
      EthTSyn_TimediffType remoteResTime = 0;
      if (EthTSyn_Ts_TsMinusTs(&RespRespFupData->ResponseOriginTs, &RespRespFupData->RequestReceiptTs,
           &remoteResTime) == E_OK)
      {
        /* #40 Check if calculated values are physically possible. */
        if ((localResTime >= 0) && (remoteResTime >= 0) && (localResTime >= remoteResTime))
        {
          /* #50 Calculate pdelay value and check that it does not exceed the configured threshold. */
          tmpPdelay = ((EthTSyn_UTimediffType)localResTime - (EthTSyn_UTimediffType)remoteResTime);
          tmpPdelay /= 2u;
          if (tmpPdelay < EthTSyn_GetPdThresholdOfPdReqSm(PdReqSmIdx))
          {
            /* #60 Apply average weight correction if required. */
            if (EthTSyn_IsFirstPdelayOfPdReqSmDyn(PdReqSmIdx))
            {
              EthTSyn_SetFirstPdelayOfPdReqSmDyn(PdReqSmIdx, FALSE);
            }
            else
            {
              const EthTSyn_UTimediffType averageWeight = EthTSyn_GetAverageWeightOfPdReqSm(PdReqSmIdx);
              if (averageWeight > 1u)
              {
                EthTSyn_UTimediffType currPdelay = (EthTSyn_UTimediffType)EthTSyn_ComCtrl_GetPdelay(
                  EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx));
                tmpPdelay = (EthTSyn_UTimediffType)(((averageWeight - 1u) * currPdelay) + tmpPdelay) / averageWeight;
              }
            }

            retVal = E_OK;
          }
        }
      }
    }
  }

  (*Pdelay) = (EthTSyn_PdelayType)tmpPdelay;

  return retVal;
} /* EthTSyn_PdReqSm_ComputePdelay() */

#  if (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_PdReqSm_GetCurrentTime
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdReqSm_GetCurrentTime(
  EthTSyn_PdReqSmIterType PdReqSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const StbM_SynchronizedTimeBaseType tbId = EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(
                                             EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(
                                             EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx)));
  StbM_TimeStampType globalTime = {0u};
  StbM_VirtualLocalTimeType localTime = {0u};
  StbM_UserDataType userData = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to retrieve the current global and local time from StbM. */
  if (StbM_BusGetCurrentTime(tbId, &globalTime, &localTime, &userData) == E_OK)
  {
    /* #20 Check if the timestamp format of the global time is valid. */
    if (EthTSyn_Ts_IsStbmTsValid(&globalTime) == TRUE)
    {
      StbM_TimeStampShortType globalTimeShort = {0};

      globalTimeShort.nanoseconds = globalTime.nanoseconds;
      globalTimeShort.seconds = globalTime.seconds;

      /* #30 Store current global and local time for later use. */
      EthTSyn_SetRefGlobalTimeOfPdReqSmDyn(PdReqSmIdx, globalTimeShort);
      EthTSyn_SetRefLocalTimeOfPdReqSmDyn(PdReqSmIdx, localTime);

      retVal = E_OK;
    }
  }

  return retVal;
} /* EthTSyn_PdReqSm_GetCurrentTime() */

/**********************************************************************************************************************
 * EthTSyn_PdReqSm_ReportTimeValidData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_ReportTimeValidData(
                  EthTSyn_PdReqSmIterType        PdReqSmIdx,
                  EthTSyn_PdelayType             Pdelay,
  ETHTSYN_P2CONST(EthTSyn_PdRespRespFupDataType) RespRespFupData)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_VirtualLocalTimeType requestOriginVlt = {0};
  StbM_VirtualLocalTimeType responseReceiptVlt = {0};
  Std_ReturnType tmpRetVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to convert request origin and response receipt timestamps into virtual local time. */
  tmpRetVal = EthTSyn_Ts_TsToVltChecked(&RespRespFupData->PdReqEgrTsMgmt.Ts, &requestOriginVlt);
  tmpRetVal |= EthTSyn_Ts_TsToVltChecked(&RespRespFupData->PdRespIngrTsMgmt.Ts, &responseReceiptVlt);

  if (tmpRetVal == E_OK)
  {
    const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx);
    const StbM_SynchronizedTimeBaseType tbId = EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(
                                              EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(comCtrlPortIdx));
    StbM_PdelayInitiatorMeasurementType timeValidData = {0};

    timeValidData.sequenceId = EthTSyn_GetSequenceIdOfPdReqSmDyn(PdReqSmIdx);
    timeValidData.pdelay = Pdelay;
    timeValidData.referenceGlobalTimestamp = EthTSyn_GetRefGlobalTimeOfPdReqSmDyn(PdReqSmIdx);
    timeValidData.referenceLocalTimestamp = EthTSyn_GetRefLocalTimeOfPdReqSmDyn(PdReqSmIdx);
    timeValidData.requestOriginTimestamp = requestOriginVlt;
    timeValidData.responseReceiptTimestamp = responseReceiptVlt;
    timeValidData.requestReceiptTimestamp.seconds = RespRespFupData->RequestReceiptTs.seconds;
    timeValidData.requestReceiptTimestamp.nanoseconds = RespRespFupData->RequestReceiptTs.nanoseconds;
    timeValidData.responseOriginTimestamp.seconds = RespRespFupData->ResponseOriginTs.seconds;
    timeValidData.responseOriginTimestamp.nanoseconds = RespRespFupData->ResponseOriginTs.nanoseconds;
    timeValidData.responsePortId.portNumber = RespRespFupData->ResponderSrcPortNum;
    timeValidData.responsePortId.clockIdentity = RespRespFupData->ResponderSrcClockId;
    timeValidData.requestPortId.portNumber = EthTSyn_GetNumberOfComCtrlPort(comCtrlPortIdx);
    timeValidData.requestPortId.clockIdentity = EthTSyn_ComCtrl_GetClockId(comCtrlPortIdx);

    /* #20 Report time validation data to StbM. */
    (void)StbM_EthSetPdelayInitiatorData(tbId, &timeValidData);
  }
} /* EthTSyn_PdReqSm_ReportTimeValidData() */
#  endif /* (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON) */

# endif /* ETHTSYN_NOUNIT_PDREQSM */

/**********************************************************************************************************************
 * Internal functions of PdReqTrcv
 *!
 * \unit PdReqTrcv
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PDREQTRCV)
/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_IntInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_IntInit(
  EthTSyn_PdReqTrcvDynIterType PdReqTrcvIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_GlobalTimestampType zeroTs = {0u};
  const EthTSyn_TsMgmtType zeroTsMgmt = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize PdReqTrcv data and set state to IDLE. */
  EthTSyn_SetPdReqEgrTsMgmtOfPdReqTrcvDyn(PdReqTrcvIdx, zeroTsMgmt);
  EthTSyn_SetPdReqEgrTsProvidedOfPdReqTrcvDyn(PdReqTrcvIdx, FALSE);
  EthTSyn_SetPdRespIngrTsMgmtOfPdReqTrcvDyn(PdReqTrcvIdx, zeroTsMgmt);
  EthTSyn_SetReqReceivedTsOfPdReqTrcvDyn(PdReqTrcvIdx, zeroTs);
  EthTSyn_SetRespOriginTsOfPdReqTrcvDyn(PdReqTrcvIdx, zeroTs);
  EthTSyn_SetRespPortNumOfPdReqTrcvDyn(PdReqTrcvIdx, 0u);
  EthTSyn_SetRespClockIdOfPdReqTrcvDyn(PdReqTrcvIdx, 0u);

  EthTSyn_SetStateOfPdReqTrcvDyn(PdReqTrcvIdx, ETHTSYN_IDLE_STATEOFPDREQTRCVDYN);
} /* EthTSyn_PdReqTrcv_IntInit() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_Reset(
  EthTSyn_PdReqTrcvDynIterType PdReqTrcvIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to protect against Ethernet interrupts to ensure data consistency. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 Reset PdReqTrcv data and set state to IDLE. */
  EthTSyn_PdReqTrcv_IntInit(PdReqTrcvIdx);

  /* #30 Leave the critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_PdReqTrcv_Reset() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_SetStateTxIntrProtected
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_SetStateTxIntrProtected(
  EthTSyn_PdReqTrcv_IdxType       PdReqTrcvIdx,
  EthTSyn_StateOfPdReqTrcvDynType NewState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to exclude Ethernet transmission interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  /* #20 Set the state of the PdReqTrcv. */
  EthTSyn_SetStateOfPdReqTrcvDyn(PdReqTrcvIdx, NewState);

  /* #30 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
} /* EthTSyn_PdReqTrcv_SetStateTxIntrProtected() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_SetStateEthIntrProtected
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_SetStateEthIntrProtected(
  EthTSyn_PdReqTrcvDynIterType    PdReqTrcvIdx,
  EthTSyn_StateOfPdReqTrcvDynType NewState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to exclude Ethernet interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 Set the state of the PdReqTrcv. */
  EthTSyn_SetStateOfPdReqTrcvDyn(PdReqTrcvIdx, NewState);

  /* #30 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_PdReqTrcv_SetStateEthIntrProtected() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_CheckStateTxIntrProtected
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdReqTrcv_CheckStateTxIntrProtected(
  EthTSyn_PdReqTrcv_IdxType       PdReqTrcvIdx,
  EthTSyn_StateOfPdReqTrcvDynType DesiredState)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isDesiredState = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to exclude Ethernet transmission interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  /* #20 Read and check the state of the PdReqTrcv. */
  isDesiredState = (boolean)(EthTSyn_GetStateOfPdReqTrcvDyn(PdReqTrcvIdx) == DesiredState);

  /* #30 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  return isDesiredState;
} /* EthTSyn_PdReqTrcv_CheckStateTxIntrProtected() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_GetPdRxTxTsMgmt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_TsMgmtType, ETHTSYN_CODE) EthTSyn_PdReqTrcv_GetPdRxTxTsMgmt(
                  EthTSyn_TsDirType   Dir,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType) TsInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TsMgmtType pdTsMgmt = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If switch management support is used. */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  {
    /* #100 Use the SwtEgrTs if Dir is TX and the SwtIngrTs otherwise. */
    pdTsMgmt = ((Dir == ETHTSYN_TS_DIR_TX) ? TsInfoPtr->SwtEgrTsMgmt : TsInfoPtr->SwtIngrTsMgmt);
  }
  /* #11 Otherwise. */
#  else
  {
    /* #110 Transform the end station timestamp (provided as IntVltMgmt) into the TsMgmt representation and use it. */
    pdTsMgmt = EthTSyn_Ts_IntVltMgmtToTsMgmt(&TsInfoPtr->IntVltMgmt);
    ETHTSYN_DUMMY_STATEMENT(Dir); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  }
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  return pdTsMgmt;
} /* EthTSyn_PdReqTrcv_GetPdRxTxTsMgmt() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_CheckRequestingPortIdentity
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdReqTrcv_CheckRequestingPortIdentity(
                  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  ETHTSYN_P2CONST(uint8)                    PdPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint64 reqClockId =
    EthTSyn_Mem_GetUint64(PdPayloadPtr, ETHTSYN_MSG_PD_RESP_RESP_FUP_REQ_CLOCK_IDENTITY_PL_REL_OFS);
  const uint16 reqPortNum = EthTSyn_Mem_GetUint16(PdPayloadPtr, ETHTSYN_MSG_PD_RESP_RESP_FUP_REQ_PORT_NUM_PL_REL_OFS);
  EthTSyn_PortIdentityType localPortId = {0};

  /* ----- Implementation ----------------------------------------------- */
  localPortId = EthTSyn_PdReqTrcv_GetLocalPortId(PdReqTrcvIdx);

  /* #10 Return TRUE when local clock identity and port number are equal to the requesting clock id and port number
   *     from the Pdelay_Resp/Pdelay_Resp_Fup message and FALSE otherwise. */
  return (boolean)((localPortId.PortNumber == reqPortNum) && (localPortId.ClockIdentity == reqClockId));
} /* EthTSyn_PdReqTrcv_CheckRequestingPortIdentity() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_GetLocalPortId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_PortIdentityType, ETHTSYN_CODE) EthTSyn_PdReqTrcv_GetLocalPortId(
  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortIdentityType localPortId = {0u};
  const EthTSyn_ComCtrlPortIdxOfPdReqSmType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqTrcvIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the clock identity and port number of the underlying EthTSyn port. */
  localPortId.ClockIdentity = EthTSyn_ComCtrl_GetClockId(comCtrlPortIdx);
  localPortId.PortNumber = EthTSyn_GetNumberOfComCtrlPort(comCtrlPortIdx);

  return localPortId;
} /* EthTSyn_PdReqTrcv_GetLocalPortId() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_GetPdTsFromMsg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_GlobalTimestampType, ETHTSYN_CODE) EthTSyn_PdReqTrcv_GetPdTsFromMsg(
  ETHTSYN_P2CONST(uint8) PdPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_GlobalTimestampType pdTs = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 De-serialize the Pdelay timestamp from the Pdelay_Resp/Pdelay_Resp_Fup message and return it. */
  pdTs.secondsHi = EthTSyn_Mem_GetUint16(PdPayloadPtr, ETHTSYN_MSG_PD_RESP_RESP_FUP_TS_SEC_HI_PL_REL_OFS);
  pdTs.seconds = EthTSyn_Mem_GetUint32(PdPayloadPtr, ETHTSYN_MSG_PD_RESP_RESP_FUP_TS_SEC_LOW_PL_REL_OFS);
  pdTs.nanoseconds = EthTSyn_Mem_GetUint32(PdPayloadPtr, ETHTSYN_MSG_PD_RESP_RESP_FUP_TS_NSEC_PL_REL_OFS);

  return pdTs;
} /* EthTSyn_PdReqTrcv_GetPdTsFromMsg() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_CheckAndHndlDuplicatedResponse
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_CheckAndHndlDuplicatedResponse(
  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  uint16                    PdRespSeqId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if state is WAIT_PD_RESP_FUP protected against Ethernet transmission interrupts. */
  if (EthTSyn_PdReqTrcv_CheckStateTxIntrProtected(PdReqTrcvIdx, ETHTSYN_WAIT_PD_RESP_FUP_STATEOFPDREQTRCVDYN) == TRUE)
  {
    /* #20 Check if sequence id of the received response matches the current cycle. */
    if (PdRespSeqId == EthTSyn_PdReqSm_GetSequenceId(PdReqTrcvIdx))
    {
      /* #30 Change the state of the PdReqTrcv to IDLE protected against Ethernet transmission interrupts. */
      EthTSyn_PdReqTrcv_SetStateTxIntrProtected(PdReqTrcvIdx, ETHTSYN_IDLE_STATEOFPDREQTRCVDYN);

      /* #40 Report lost response to the PdReqSm due to duplicated response. */
      EthTSyn_PdReqSm_LostResponse(PdReqTrcvIdx);
    }
  }

} /* EthTSyn_PdReqTrcv_CheckAndHndlDuplicatedResponse() */
# endif /* ETHTSYN_NOUNIT_PDREQTRCV */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Public interfaces of PdReqSm
 *!
 * \unit PdReqSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PDREQSM)

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdReqSmIterType pdReqSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize dynamic data of all pdelay request state machines. */
  for (pdReqSmIter = 0; pdReqSmIter < EthTSyn_GetSizeOfPdReqSm(); pdReqSmIter++)
  {
    EthTSyn_PdReqSm_IntInit(pdReqSmIter);
  }
} /* EthTSyn_PdReqSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdReqSmIterType pdReqSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process all pdelay request state machines. */
  for (pdReqSmIter = 0; pdReqSmIter < EthTSyn_GetSizeOfPdReqSm(); pdReqSmIter++)
  {
    EthTSyn_PdReqSm_ProcSm(pdReqSmIter);
  }
} /* EthTSyn_PdReqSm_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_Reset(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if pdelay request state machine exists. */
  if (PdReqSmIdx < EthTSyn_GetSizeOfPdReqSm())
  {
    /* #20 Trigger reset of state machine. */
    EthTSyn_SetSmResetRequestedOfPdReqSmDyn(PdReqSmIdx, TRUE);
  }
} /* EthTSyn_PdReqSm_Reset() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_LostResponse
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_LostResponse(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to protect against interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 Check if state is WAIT_RESP_RESP_FUP. */
  if (EthTSyn_GetStateOfPdReqSmDyn(PdReqSmIdx) == ETHTSYN_WAIT_RESP_RESP_FUP_STATEOFPDREQSMDYN)
  {
    /* #30 Increment and handle lost response counter. */
    EthTSyn_PdReqSm_IntLostResponse(PdReqSmIdx);

    /* #40 Set state to TX_PD_REQ. */
    EthTSyn_SetStateOfPdReqSmDyn(PdReqSmIdx, ETHTSYN_TX_PD_REQ_STATEOFPDREQSMDYN);

    /* #50 Increment sequence Id. */
    EthTSyn_IncSequenceIdOfPdReqSmDyn(PdReqSmIdx);
  }

  /* #60 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_PdReqSm_LostResponse() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_RcvdPdResp
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_RcvdPdResp(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to protect against Tx interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  /* #20 Check if state is WAIT_RESP_RESP_FUP. */
  if (EthTSyn_GetStateOfPdReqSmDyn(PdReqSmIdx) == ETHTSYN_WAIT_RESP_RESP_FUP_STATEOFPDREQSMDYN)
  {
    /* #30 Report reception of Pdelay_Resp message to PortParamStats unit. */
    EthTSyn_PortParamStats_IncRxPdRespCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
      EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx)));

    /* #40 Reset pdelay response timeout counter. */
    EthTSyn_SetRespTimeoutCntOfPdReqSmDyn(PdReqSmIdx, EthTSyn_GetPdRespTimeoutOfPdReqSm(PdReqSmIdx));
  }

  /* #50 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
} /* EthTSyn_PdReqSm_RcvdPdResp() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_RcvdPdRespFup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_RcvdPdRespFup(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to protect against Tx interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  /* #20 Check if state is WAIT_RESP_RESP_FUP. */
  if (EthTSyn_GetStateOfPdReqSmDyn(PdReqSmIdx) == ETHTSYN_WAIT_RESP_RESP_FUP_STATEOFPDREQSMDYN)
  {
    /* #30 Report reception of Pdelay_Resp_Fup message to PortParamStats unit. */
    EthTSyn_PortParamStats_IncRxPdRespFupCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
      EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx)));

    /* #40 Set state to WAIT_REQ_EGR_TS. */
    EthTSyn_SetStateOfPdReqSmDyn(PdReqSmIdx, ETHTSYN_WAIT_REQ_EGR_TS_STATEOFPDREQSMDYN);
  }

  /* #50 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
} /* EthTSyn_PdReqSm_RcvdPdRespFup() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_RespRespFupDataReady
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqSm_RespRespFupDataReady(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to protect against Rx and Tx interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 If state is WAIT_RESP_RESP_FUP, report reception of Pdelay_Resp_Fup message to PortParamStats unit. */
  if (EthTSyn_GetStateOfPdReqSmDyn(PdReqSmIdx) == ETHTSYN_WAIT_RESP_RESP_FUP_STATEOFPDREQSMDYN)
  {
    EthTSyn_PortParamStats_IncRxPdRespFupCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
      EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqSmIdx)));
  }

  /* #30 If state is WAIT_RESP_RESP_FUP or WAIT_REQ_EGR_TS, set state to COMPUTE_PDELAY. */
  if ((EthTSyn_GetStateOfPdReqSmDyn(PdReqSmIdx) == ETHTSYN_WAIT_RESP_RESP_FUP_STATEOFPDREQSMDYN) ||
     (EthTSyn_GetStateOfPdReqSmDyn(PdReqSmIdx) == ETHTSYN_WAIT_REQ_EGR_TS_STATEOFPDREQSMDYN))
  {
    EthTSyn_SetStateOfPdReqSmDyn(PdReqSmIdx, ETHTSYN_COMPUTE_PDELAY_STATEOFPDREQSMDYN);
  }

  /* #40 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_PdReqSm_RespRespFupDataReady() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqSm_GetSequenceId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, ETHTSYN_CODE) EthTSyn_PdReqSm_GetSequenceId(
  EthTSyn_PdReqSm_SmIdxType PdReqSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return current sequence Id. */
  return EthTSyn_GetSequenceIdOfPdReqSmDyn(PdReqSmIdx);
} /* EthTSyn_PdReqSm_GetSequenceId() */

# endif /* ETHTSYN_NOUNIT_PDREQSM */

/**********************************************************************************************************************
 * Public interfaces of PdReqTrcv
 *!
 * \unit PdReqTrcv
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PDREQTRCV)

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdReqTrcvDynIterType pdReqTrcvIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all global dynamic data of the PdReqTrcv unit. */
  for (pdReqTrcvIter = 0u; pdReqTrcvIter < EthTSyn_GetSizeOfPdReqTrcvDyn(); pdReqTrcvIter++)
  {
    EthTSyn_PdReqTrcv_IntInit(pdReqTrcvIter);
  }

} /* EthTSyn_PdReqTrcv_Init() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_CancelCycle
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_CancelCycle(
  EthTSyn_PdReqTrcvDynIterType PdReqTrcvIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset PdReqTrcv data and set state to IDLE. */
  EthTSyn_PdReqTrcv_Reset(PdReqTrcvIdx);
} /* EthTSyn_PdReqTrcv_CancelCycle() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_TxPdReq
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdReqTrcv_TxPdReq(
  EthTSyn_PdReqTrcvDynIterType PdReqTrcvIdx,
  uint16                       SequenceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  /* Using PdReqTrcvIdx to access ROM data of PdReqSm is fine due to size relation. */
  const EthTSyn_ComCtrlPortIdxOfPdReqSmType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdReqSm(PdReqTrcvIdx);
  const EthTSyn_TxConfContextIdxOfPdReqSmType txConfIdx = EthTSyn_GetTxConfContextIdxOfPdReqSm(PdReqTrcvIdx);
  EthTSyn_MsgHdrTypeSpecType pdReqMsgHdr = {0u};
  EthTSyn_EthTxBufferStructType txBufMgmt = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset data of potential former Pdelay cycle and change state of PdReqTrcv to IDLE. */
  EthTSyn_PdReqTrcv_Reset(PdReqTrcvIdx);

  /* #20 Prepare type specific message header for the Pdelay request message. */
  pdReqMsgHdr.MessageType = ETHTSYN_MSG_TYPE_PD_REQ;
  pdReqMsgHdr.MessageLength = ETHTSYN_MSG_PD_REQ_LEN;
  pdReqMsgHdr.SequenceId = SequenceId;
  pdReqMsgHdr.Control = ETHTSYN_MSG_HDR_CONTROL_PDELAY_REQ;
  pdReqMsgHdr.Flags0 = ETHTSYN_MSG_HDR_FLAGS_0_DEFAULT;
  pdReqMsgHdr.Flags1 = ETHTSYN_MSG_HDR_FLAGS_1_DEFAULT;
  pdReqMsgHdr.LogMessageInterval = EthTSyn_GetLogMsgIntervalOfPdReqSm(PdReqTrcvIdx);
  pdReqMsgHdr.CorrectionFieldNs = ETHTSYN_MSG_HDR_COR_FIELD_DEFAULT;
  pdReqMsgHdr.CorrectionFieldSubNs = ETHTSYN_MSG_HDR_COR_FIELD_SUB_NS_DEFAULT;

  /* #30 Try to start the transmission sequence. */
  txBufMgmt.BufferPtr = NULL_PTR; /* Set BufferPtr to known value for VCA analysis */
  if (EthTSyn_Tx_StartTxSequence(comCtrlPortIdx, txConfIdx, &pdReqMsgHdr, NULL_PTR, &txBufMgmt) == E_OK)
  {
    /*@ assert txBufMgmt.BufferPtr != NULL_PTR; */                                                                     /* VCA_ETHTSYN_TX_BUF_VALID */
    /* Pdelay_Req message does not contain any payload (just reserved). The whole transmission buffer is initialized
     * by the Tx unit. Therefore, it is not required to copy the 'payload' to the buffer here. */

    /* #40 Change state of the PdReqTrcv to WAIT_PD_RESP protected against Ethernet interrupts. */
    EthTSyn_PdReqTrcv_SetStateEthIntrProtected(PdReqTrcvIdx, ETHTSYN_WAIT_PD_RESP_STATEOFPDREQTRCVDYN);

    /* #50 Try to finalize the transmission sequence. */
    if (EthTSyn_Tx_FinishTxSequence(comCtrlPortIdx, txConfIdx, txBufMgmt.BufferIdx, ETHTSYN_MSG_PD_REQ_LEN) == E_OK)
    {
      /* #500 Inform caller about successful transmission. */
      retVal = E_OK;
    }
    /* #51 Otherwise, change state of the PdReqTrcv to IDLE protected against Ethernet interrupts and return an
     *     error. */
    else
    {
      EthTSyn_PdReqTrcv_SetStateEthIntrProtected(PdReqTrcvIdx, ETHTSYN_IDLE_STATEOFPDREQTRCVDYN);
    }
  }

  return retVal;
} /* EthTSyn_PdReqTrcv_TxPdReq() */

/**********************************************************************************************************************
 * EthTSyn_PdReqTrcv_ProvidePdReqEgrTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_ProvidePdReqEgrTs(
                  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)       EgrTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to exclude Ethernet reception interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
  {
    /* #20 Check if PdReqTrcv is in the state to receive a Pdelay_Req egress timestamp (i.e. WAIT_PD_RESP,
     *     WAIT_PD_RESP_FUP or WAIT_PD_REQ_EGR_TS). */
    const EthTSyn_StateOfPdReqTrcvDynType pdReqTrcvState = EthTSyn_GetStateOfPdReqTrcvDyn(PdReqTrcvIdx);

    if ((pdReqTrcvState == ETHTSYN_WAIT_PD_RESP_STATEOFPDREQTRCVDYN) ||
        (pdReqTrcvState == ETHTSYN_WAIT_PD_RESP_FUP_STATEOFPDREQTRCVDYN) ||
        (pdReqTrcvState == ETHTSYN_WAIT_PD_REQ_EGR_TS_STATEOFPDREQTRCVDYN))
    {
      /* #30 Check if timestamp was not provided already. */
      if (!EthTSyn_IsPdReqEgrTsProvidedOfPdReqTrcvDyn(PdReqTrcvIdx))
      {
        /* #40 Store the timestamp and validity information for later processing. */
        EthTSyn_SetPdReqEgrTsMgmtOfPdReqTrcvDyn(PdReqTrcvIdx,
          EthTSyn_PdReqTrcv_GetPdRxTxTsMgmt(ETHTSYN_TS_DIR_TX, EgrTsInfoPtr));

        /* #50 Mark timestamp as provided. */
        EthTSyn_SetPdReqEgrTsProvidedOfPdReqTrcvDyn(PdReqTrcvIdx, TRUE);

        /* #60 Change state of the PdReqTrcv to ALL_DATA_AVAILABLE when state is WAIT_PD_REQ_EGR_TS and inform the
         *     PdReqSm about the availability of the data. */
        if (pdReqTrcvState == ETHTSYN_WAIT_PD_REQ_EGR_TS_STATEOFPDREQTRCVDYN)
        {
          EthTSyn_SetStateOfPdReqTrcvDyn(PdReqTrcvIdx, ETHTSYN_ALL_DATA_AVAILABLE_STATEOFPDREQTRCVDYN);
          EthTSyn_PdReqSm_RespRespFupDataReady(PdReqTrcvIdx);
        }
      }
    }
  }
  /* #11 Leave the critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
} /* EthTSyn_PdReqTrcv_ProvidePdReqEgrTs() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_ProcRcvdPdResp
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_EthTSyn_6050_TrivialFunctions */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_ProcRcvdPdResp(
                  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)       PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                    PdRespPayloadPtr,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)       RxTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if interface is called for a valid PdReqTrcv. */
  if (PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn())
  {
    /* #20 Verify message length in header. */
    if (PtpHdrPtr->MessageLength == ETHTSYN_MSG_PD_RESP_LEN)
    {
      /* #30 Check if PdReqTrcv is in state WAIT_PD_RESP protected against Ethernet transmission interrupts. */
      if (EthTSyn_PdReqTrcv_CheckStateTxIntrProtected(PdReqTrcvIdx, ETHTSYN_WAIT_PD_RESP_STATEOFPDREQTRCVDYN) == TRUE)
      {
        const EthTSyn_GlobalTimestampType reqReceivedTs = EthTSyn_PdReqTrcv_GetPdTsFromMsg(PdRespPayloadPtr);

        /* #40 Check if:
         *       sequence id of the Pdelay_Resp matches to the transmitted Pdelay_Req.
         *       the requesting source port identity of the Pdelay_Resp matches to the local port.
         *       the received request receipt timestamp is valid. */
        if ((EthTSyn_PdReqSm_GetSequenceId(PdReqTrcvIdx) == PtpHdrPtr->SequenceId) &&
            (EthTSyn_PdReqTrcv_CheckRequestingPortIdentity(PdReqTrcvIdx, PdRespPayloadPtr) == TRUE) && /* PRQA S 3415 */ /* MD_EthTSyn_3415_ReadOnly */
            (EthTSyn_Ts_IsTsValid(&reqReceivedTs) == TRUE)) /* PRQA S 3415 */ /* MD_EthTSyn_3415_ReadOnly */
        {
          /* #50 Store Pdelay_Resp ingress timestamp, request received timestamp and responder source port identity
           *     for later processing. */
          EthTSyn_SetPdRespIngrTsMgmtOfPdReqTrcvDyn(PdReqTrcvIdx,
            EthTSyn_PdReqTrcv_GetPdRxTxTsMgmt(ETHTSYN_TS_DIR_RX, RxTsInfoPtr));

          EthTSyn_SetReqReceivedTsOfPdReqTrcvDyn(PdReqTrcvIdx, reqReceivedTs);

          EthTSyn_SetRespPortNumOfPdReqTrcvDyn(PdReqTrcvIdx, PtpHdrPtr->SourcePortNumber);
          EthTSyn_SetRespClockIdOfPdReqTrcvDyn(PdReqTrcvIdx, PtpHdrPtr->SourceClockId);

          /* #60 Change state of the PdReqTrcv to WAIT_PD_RESP_FUP protected against Ethernet transmission interrupts. */
          EthTSyn_PdReqTrcv_SetStateTxIntrProtected(PdReqTrcvIdx, ETHTSYN_WAIT_PD_RESP_FUP_STATEOFPDREQTRCVDYN);

          /* #70 Inform the PdReqSm about a received Pdelay_Resp. */
          EthTSyn_PdReqSm_RcvdPdResp(PdReqTrcvIdx);
        }
        /* #41 Otherwise, received invalid Pdelay_Resp. Drop the Pdelay_Resp, change state of the PdReqTrcv to IDLE (
         *     protected against Ethernet transmission interrupts) and inform the PdReqSm about a lost response. */
        else
        {
          EthTSyn_PdReqTrcv_SetStateTxIntrProtected(PdReqTrcvIdx, ETHTSYN_IDLE_STATEOFPDREQTRCVDYN);

          EthTSyn_PdReqSm_LostResponse(PdReqTrcvIdx);
        }
      }
      /* #31 Otherwise, check and handle potential duplicated response. */
      else
      {
        EthTSyn_PdReqTrcv_CheckAndHndlDuplicatedResponse(PdReqTrcvIdx, PtpHdrPtr->SequenceId);
      }
    }
  }
} /* EthTSyn_PdReqTrcv_ProcRcvdPdResp() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_ProcRcvdPdRespFup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_EthTSyn_6050_TrivialFunctions, MD_EthTSyn_6080_RobustnessChecks */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_ProcRcvdPdRespFup(
                  EthTSyn_PdReqTrcv_IdxType PdReqTrcvIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)       PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                    PdRespFupPayloadPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if interface is called for a valid PdReqTrcv. */
  if (PdReqTrcvIdx < EthTSyn_GetSizeOfPdReqTrcvDyn())
  {
    /* #20 Verify message length in header. */
    if (PtpHdrPtr->MessageLength == ETHTSYN_MSG_PD_RESP_FUP_LEN)
    {
      /* #30 Check if PdReqTrcv is in state WAIT_PD_RESP_FUP protected against Ethernet transmission interrupts. */
      if (EthTSyn_PdReqTrcv_CheckStateTxIntrProtected(PdReqTrcvIdx,
            ETHTSYN_WAIT_PD_RESP_FUP_STATEOFPDREQTRCVDYN) == TRUE)
      {
        const EthTSyn_GlobalTimestampType respOriginTs = EthTSyn_PdReqTrcv_GetPdTsFromMsg(PdRespFupPayloadPtr);
        /* #40 Check if:
         *       sequence id of the Pdelay_Resp_Fup matches to the transmitted Pdelay_Req.
         *       the requesting source port identity of the Pdelay_Resp_Fup matches to the local port.
         *       the Pdelay_Resp_Fup was transmitted from the same node as the Pdelay_Resp.
         *       the received response origin timestamp is valid. */
        if ((EthTSyn_PdReqSm_GetSequenceId(PdReqTrcvIdx) == PtpHdrPtr->SequenceId) &&
            (EthTSyn_PdReqTrcv_CheckRequestingPortIdentity(PdReqTrcvIdx, PdRespFupPayloadPtr) == TRUE) && /* PRQA S 3415 */ /* MD_EthTSyn_3415_ReadOnly */
            (EthTSyn_GetRespPortNumOfPdReqTrcvDyn(PdReqTrcvIdx) == PtpHdrPtr->SourcePortNumber) &&
            (EthTSyn_GetRespClockIdOfPdReqTrcvDyn(PdReqTrcvIdx) == PtpHdrPtr->SourceClockId) &&
            (EthTSyn_Ts_IsTsValid(&respOriginTs) == TRUE)) /* PRQA S 3415 */ /* MD_EthTSyn_3415_ReadOnly */
        {
          /* #50 Copy the response origin timestamp for later processing. */
          EthTSyn_SetRespOriginTsOfPdReqTrcvDyn(PdReqTrcvIdx, respOriginTs);

          /* #60 Enter critical section to exclude Ethernet transmission interrupts and check if Pdelay_Req egress
           *     timestamps was already provided. */
          SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
          if (EthTSyn_IsPdReqEgrTsProvidedOfPdReqTrcvDyn(PdReqTrcvIdx))
          {
            /* #600 Change state of the PdReqTrcv to ALL_DATA_AVAILABLE. */
            EthTSyn_SetStateOfPdReqTrcvDyn(PdReqTrcvIdx, ETHTSYN_ALL_DATA_AVAILABLE_STATEOFPDREQTRCVDYN);

            /* #601 Leave critical section. */
            SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

            /* #602 Inform the PdReqSm about availability of all Pdelay data. */
            EthTSyn_PdReqSm_RespRespFupDataReady(PdReqTrcvIdx);
          }
          /* #61 Otherwise. */
          else
          {
            /* #610 Change state of the PdReqTrcv to WAIT_PPD_REQ_EGR_TS. */
            EthTSyn_SetStateOfPdReqTrcvDyn(PdReqTrcvIdx, ETHTSYN_WAIT_PD_REQ_EGR_TS_STATEOFPDREQTRCVDYN);

            /* #611 Leave critical section. */
            SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

            /* #612 Inform the PdReqSm about received Pdelay_Resp_Fup. */
            EthTSyn_PdReqSm_RcvdPdRespFup(PdReqTrcvIdx);
          }
        }
        /* #51 Otherwise, received invalid Pdelay_Resp_Fup. Drop the Pdelay_Resp_Fup, change state of the PdReqTrcv to
         *     IDLE and inform PdReqSm about a lost response. */
        else
        {
          EthTSyn_PdReqTrcv_SetStateTxIntrProtected(PdReqTrcvIdx, ETHTSYN_IDLE_STATEOFPDREQTRCVDYN);

          EthTSyn_PdReqSm_LostResponse(PdReqTrcvIdx);
        }
      }
    }
  }
} /* EthTSyn_PdReqTrcv_ProcRcvdPdRespFup() */

/**********************************************************************************************************************
 *  EthTSyn_PdReqTrcv_GetRespRespFupData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdReqTrcv_GetRespRespFupData(
                EthTSyn_PdReqTrcvDynIterType   PdReqTrcvIdx,
  ETHTSYN_P2VAR(EthTSyn_PdRespRespFupDataType) PdRespRespFupDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdRespRespFupDataType pdRespRespFupData = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to exclude Ethernet interrupts. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 Check if PdReqTrcv is in state ALL_DATA_AVAILABLE. */
  if (EthTSyn_GetStateOfPdReqTrcvDyn(PdReqTrcvIdx) == ETHTSYN_ALL_DATA_AVAILABLE_STATEOFPDREQTRCVDYN)
  {
    /* #30 Copy all relevant Pdelay data. */
    pdRespRespFupData.PdReqEgrTsMgmt = EthTSyn_GetPdReqEgrTsMgmtOfPdReqTrcvDyn(PdReqTrcvIdx);
    pdRespRespFupData.PdRespIngrTsMgmt = EthTSyn_GetPdRespIngrTsMgmtOfPdReqTrcvDyn(PdReqTrcvIdx);
    pdRespRespFupData.RequestReceiptTs = EthTSyn_GetReqReceivedTsOfPdReqTrcvDyn(PdReqTrcvIdx);
    pdRespRespFupData.ResponseOriginTs = EthTSyn_GetRespOriginTsOfPdReqTrcvDyn(PdReqTrcvIdx);
#  if (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON)
    pdRespRespFupData.ResponderSrcClockId = EthTSyn_GetRespClockIdOfPdReqTrcvDyn(PdReqTrcvIdx);
    pdRespRespFupData.ResponderSrcPortNum = EthTSyn_GetRespPortNumOfPdReqTrcvDyn(PdReqTrcvIdx);
#  endif /* ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON */
  }

  /* #11 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  (*PdRespRespFupDataPtr) = pdRespRespFupData;
} /* EthTSyn_PdReqTrcv_GetRespRespFupData() */

# endif /* ETHTSYN_NOUNIT_PDREQTRCV */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PdelayInitiator.c
 *********************************************************************************************************************/
