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
/*!        \file  EthTSyn_PdelayResponder.c
 *         \unit  PdRespSm
 *        \brief  EthTSyn PdRespSm source file
 *      \details  Implementation of the PdRespSm unit.
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

#define ETHTSYN_PD_RESPONDER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_GeneratedCfgAccess.h"

#if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
# include "EthTSyn_PdRespSm_Int.h"
# include "SchM_EthTSyn.h"
# include "EthTSyn_ComCtrl_Int.h"
# include "EthTSyn_Timestamp_Int.h"
# include "StbM.h"
# include "EthTSyn_Tx_Int.h"
# include "EthTSyn_MsgDefs_Int.h"
# include "EthTSyn_PortParamStats_Int.h"
# include "EthTSyn_Memory_Int.h"

# if ( (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) )
#  include "EthIf.h"
# endif /* ( (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) ) */

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
 * EthTSyn_PdRespSm_IntInit
 *********************************************************************************************************************/
/*! \brief          Initializes the passed PdRespSm
 *  \details        -
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                  [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \pre            Must be called within critical section 0 when not called in Init context.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_IntInit(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_Proc
 *********************************************************************************************************************/
/*! \brief          Processes the passed PdRespSm
 *  \details        -
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                  [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \pre            Has to be called cyclically in context of the EthTSyn_MainFunction()
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_Proc(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ProcState
 *********************************************************************************************************************/
/*! \brief          Processes the current state of the passed PdRespSm
 *  \details        -
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                               [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \pre            Has to be called cyclically in context of the EthTSyn_MainFunction()
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcState(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ProcStateInitializing
 *********************************************************************************************************************/
/*! \brief          Processes the INITIALIZING state of the passed PdRespSm
 *  \details        -
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                               [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \pre            Has to be called cyclically in context of the EthTSyn_MainFunction() when PdRespSm is in state
 *                  INITIALIZING
 *  \pre            Must be called within critical section 0
 *  \context        TASK
 *  \reentrant      FALSE
 *  \note           Critical section 0 is closed within this function
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcStateInitializing(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ProcStateTxPdResp
 *********************************************************************************************************************/
/*! \brief          Processes the TX_PD_RESP state of the passed PdRespSm
 *  \details        -
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                               [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \pre            Has to be called cyclically in context of the EthTSyn_MainFunction() when PdRespSm is in state
 *                  TX_PD_RESP
 *  \pre            Must be called within critical section 0
 *  \context        TASK
 *  \reentrant      FALSE
 *  \note           Critical section 0 is closed within this function
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcStateTxPdResp(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ProcStateTxPdRespFup
 *********************************************************************************************************************/
/*! \brief          Processes the TX_PD_RESP_FUP state of the passed PdRespSm
 *  \details        -
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                               [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \pre            Has to be called cyclically in context of the EthTSyn_MainFunction() when PdRespSm is in state
 *                  TX_PD_RESP_FUP
 *  \pre            Must be called within critical section 0
 *  \context        TASK
 *  \reentrant      FALSE
 *  \note           Critical section 0 is closed within this function
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcStateTxPdRespFup(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_TxPdResp
 *********************************************************************************************************************/
/*! \brief          Transmits a Pdelay_Resp message for the passed PdRespSm.
 *  \details        -
 *  \param[in]      PdRespSmIdx       Index of the PdRespSm
 *                                    [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \param[in]      SequenceId        Sequence id of the Pdelay cycle (received with corresponding Pdelay_Req)
 *  \param[in]      RequestorPortNum  Port number of the Pdelay requestor
 *  \param[in]      RequestorClockId  Clock identity of the Pdelay requestor
 *  \param[in]      PdReqRcvdTsPtr    Ingress timestamp of the corresponding Pdelay_Req
 *  \return         E_OK - Pdelay_Resp message transmission succeeded
 *  \return         E_NOT_OK - Pdelay_Resp message transmission failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_TxPdResp(
                  EthTSyn_PdRespSmDynIterType  PdRespSmIdx,
                  uint16                       SequenceId,
                  uint16                       RequestorPortNum,
                  uint64                       RequestorClockId,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PdReqRcvdTsPtr);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_TxPdRespFup
 *********************************************************************************************************************/
/*! \brief          Transmits a Pdelay_Resp_Fup message for the passed PdRespSm.
 *  \details        -
 *  \param[in]      PdRespSmIdx       Index of the PdRespSm
 *                                    [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \param[in]      SequenceId        Sequence id of the Pdelay cycle (received with corresponding Pdelay_Req)
 *  \param[in]      RequestorPortNum  Port number of the Pdelay requestor
 *  \param[in]      RequestorClockId  Clock identity of the Pdelay requestor
 *  \param[in]      PdRespOrgTsPtr    Egress timestamp of the corresponding Pdelay_Resp
 *  \return         E_OK - Pdelay_Resp_Fup message transmission succeeded
 *  \return         E_NOT_OK - Pdelay_Resp_Fup message transmission failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_TxPdRespFup(
                  EthTSyn_PdRespSmDynIterType  PdRespSmIdx,
                  uint16                       SequenceId,
                  uint16                       RequestorPortNum,
                  uint64                       RequestorClockId,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PdRespOrgTsPtr);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_GetPdRxTxTsMgmt
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_TsMgmtType, ETHTSYN_CODE) EthTSyn_PdRespSm_GetPdRxTxTsMgmt(
                  EthTSyn_TsDirType   Dir,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType) TsInfoPtr);

# if ( (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) || \
       (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) )
/**********************************************************************************************************************
 * EthTSyn_PdRespSm_TakeRefTime
 *********************************************************************************************************************/
/*! \brief          Takes reference times if required.
 *  \details        Fetches reference times (i.e. current virtual local time and global time) from StbM.
 *                  Optionally: Fetches the current HW time from EthIf together with a matching virtual local time
 *                              from StbM.
 *                  Retrieved timestamps are stored by the state machine.
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                               [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \return         E_OK - No reference times required or time retrieval was successful
 *  \return         E_NOT_OK - Time retrieval failed: do not transmit Pdelay_Resp message
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_TakeRefTime(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_IsRefTimeRequired
 *********************************************************************************************************************/
/*! \brief          Checks if reference times are required.
 *  \details        -
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                               [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \return         TRUE - Reference times required
 *  \return         FALSE - No reference times required
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdRespSm_IsRefTimeRequired(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_TakeAddRefTime
 *********************************************************************************************************************/
/*! \brief          Takes additional reference times if required.
 *  \details        Optionally: Fetches the current HW time from EthIf together with a matching virtual local time
 *                              from StbM.
 *                  Retrieved timestamps are stored by the state machine.
 *  \param[in]      PdRespSmIdx  Index of the PdRespSm
 *                               [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \return         E_OK - No reference times required or time retrieval was successful
 *  \return         E_NOT_OK - Time retrieval failed: do not transmit Pdelay_Resp message
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_TakeAddRefTime(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx);
# endif /* ( (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) || \
             (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) ) */

# if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_PdRespSm_GetTimeValidData
 *********************************************************************************************************************/
/*! \brief          Gets the time validation data.
 *  \details        -
 *  \param[in]      PdRespSmIdx       Index of the PdRespSm
 *                                    [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \param[out]     TimeValidDataPtr  The time validation data
 *  \return         E_OK - Getting time validation data succeeded
 *  \return         E_NOT_OK - Getting time validation data failed
 *  \pre            Has to be called upon processing of the TX_PD_RESP_FUP state within the same critical section
 *                  as state transition to WAIT_RX_PD_REQ is done
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_GetTimeValidData(
                EthTSyn_PdRespSmDynIterType          PdRespSmIdx,
  ETHTSYN_P2VAR(StbM_PdelayResponderMeasurementType) TimeValidDataPtr);
# endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

# if (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ConvertIngrTs
 *********************************************************************************************************************/
/*! \brief          Converts an ingress timestamp into a global timestamp if required.
 *  \details        -
 *  \param[in]      PdRespSmIdx Index of the PdRespSm
 *                              [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \param[in,out]  TsPtr       In: Timestamp which shall be converted
 *                             Out: Converted timestamp
 *  \return         E_OK - No timestamp conversion required or timestamp successfully converted
 *  \return         E_NOT_OK - Timestamp conversion failed
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_ConvertIngrTs(
                EthTSyn_PdRespSmDynIterType  PdRespSmIdx,
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr);

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ConvertEgrTs
 *********************************************************************************************************************/
/*! \brief          Converts an egress timestamp into a global timestamp if required.
 *  \details        -
 *  \param[in]      PdRespSmIdx Index of the PdRespSm
 *                              [range: PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn()]
 *  \param[in,out]  TsPtr       In: Timestamp which shall be converted
 *                             Out: Converted timestamp
 *  \return         E_OK - No timestamp conversion required or timestamp successfully converted
 *  \return         E_NOT_OK - Timestamp conversion failed
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_ConvertEgrTs(
                EthTSyn_PdRespSmDynIterType  PdRespSmIdx,
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr);
# endif /* (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * EthTSyn_PdRespSm_IntInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_IntInit(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_GlobalTimestampType zeroTs = {0u};

  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SetSmResetRequestedOfPdRespSmDyn(PdRespSmIdx, FALSE);
  EthTSyn_SetSequenceIdOfPdRespSmDyn(PdRespSmIdx, 0u);
  EthTSyn_SetRequestorClockIdOfPdRespSmDyn(PdRespSmIdx, 0u);
  EthTSyn_SetRequestorPortNumOfPdRespSmDyn(PdRespSmIdx, 0u);
  EthTSyn_SetPdReqRcvdTsOfPdRespSmDyn(PdRespSmIdx, zeroTs);
  EthTSyn_SetPdRespOrgTsOfPdRespSmDyn(PdRespSmIdx, zeroTs);
  EthTSyn_SetStateOfPdRespSmDyn(PdRespSmIdx, ETHTSYN_INITIALIZING_STATEOFPDRESPSMDYN);
} /* EthTSyn_PdRespSm_IntInit() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_Proc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_Proc(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 With disabled interrupts:
   *       Re-initialize the PdRespSm if a reset is requested. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  if (EthTSyn_IsSmResetRequestedOfPdRespSmDyn(PdRespSmIdx))
  {
    EthTSyn_PdRespSm_IntInit(PdRespSmIdx);
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 Process the current state of the PdRespSm. */
  EthTSyn_PdRespSm_ProcState(PdRespSmIdx);
} /* EthTSyn_PdRespSm_Proc() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ProcState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcState(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to ensure data consistency. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #20 Check the current state of the PdRespSm.
   *       Do the specific handling for the states INITIALIZING, TX_PD_RESP and TX_PD_RESP_FUP.
   *       Leave critical section only for any other state. */
  switch (EthTSyn_GetStateOfPdRespSmDyn(PdRespSmIdx))
  {
  case ETHTSYN_INITIALIZING_STATEOFPDRESPSMDYN:
    {
      EthTSyn_PdRespSm_ProcStateInitializing(PdRespSmIdx);
      break;
    }

  case ETHTSYN_TX_PD_RESP_STATEOFPDRESPSMDYN:
    {
      EthTSyn_PdRespSm_ProcStateTxPdResp(PdRespSmIdx);
      break;
    }

  case ETHTSYN_TX_PD_RESP_FUP_STATEOFPDRESPSMDYN:
    {
      EthTSyn_PdRespSm_ProcStateTxPdRespFup(PdRespSmIdx);
      break;
    }

  default:
    {
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
      break;
    }
  }
} /* EthTSyn_PdRespSm_ProcState() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ProcStateInitializing
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcStateInitializing(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_ComCtrlPortIdxOfPdRespSmType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx);

  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_ComCtrl_IsLinkStateActive(comCtrlPortIdx) == TRUE)
  {
    EthTSyn_SetStateOfPdRespSmDyn(PdRespSmIdx, ETHTSYN_WAIT_RX_PD_REQ_STATEOFPDRESPSMDYN);
  }

  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_PdRespSm_ProcStateInitializing() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ProcStateTxPdResp
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcStateTxPdResp(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint16 seqId = EthTSyn_GetSequenceIdOfPdRespSmDyn(PdRespSmIdx);
  const uint16 reqPortNum = EthTSyn_GetRequestorPortNumOfPdRespSmDyn(PdRespSmIdx);
  const uint64 reqClockId = EthTSyn_GetRequestorClockIdOfPdRespSmDyn(PdRespSmIdx);
  EthTSyn_GlobalTimestampType pdReqRcvdTs = {0u};

  /* ----- Implementation ----------------------------------------------- */
  pdReqRcvdTs = EthTSyn_GetPdReqRcvdTsOfPdRespSmDyn(PdRespSmIdx);

  EthTSyn_SetStateOfPdRespSmDyn(PdRespSmIdx, ETHTSYN_WAIT_PD_RESP_EGR_TS_STATEOFPDRESPSMDYN);
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* REDUX comment removed. */
  if (EthTSyn_PdRespSm_TxPdResp(PdRespSmIdx, seqId, reqPortNum, reqClockId, &pdReqRcvdTs) != E_OK)
  {
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

    if (EthTSyn_GetStateOfPdRespSmDyn(PdRespSmIdx) == ETHTSYN_WAIT_PD_RESP_EGR_TS_STATEOFPDRESPSMDYN)
    {
      EthTSyn_SetStateOfPdRespSmDyn(PdRespSmIdx, ETHTSYN_TX_PD_RESP_STATEOFPDRESPSMDYN);
    }
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }
} /* EthTSyn_PdRespSm_ProcStateTxPdResp() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ProcStateTxPdRespFup
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
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcStateTxPdRespFup(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint16 seqId = EthTSyn_GetSequenceIdOfPdRespSmDyn(PdRespSmIdx);
  const uint16 reqPortNum = EthTSyn_GetRequestorPortNumOfPdRespSmDyn(PdRespSmIdx);
  const uint64 reqClockId = EthTSyn_GetRequestorClockIdOfPdRespSmDyn(PdRespSmIdx);
  EthTSyn_GlobalTimestampType pdRespOrgTs = {0u};
# if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
    Std_ReturnType getTimeValidDataRetVal = E_NOT_OK;
    StbM_PdelayResponderMeasurementType timeValidData = {0};
# endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  pdRespOrgTs = EthTSyn_GetPdRespOrgTsOfPdRespSmDyn(PdRespSmIdx);

# if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
  if (EthTSyn_IsTimeValidationEnabledOfPdRespSm(PdRespSmIdx))
  {
    getTimeValidDataRetVal = EthTSyn_PdRespSm_GetTimeValidData(PdRespSmIdx, &timeValidData);
  }
# endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  /* #10 Trigger transmission of the Pdelay_Resp_Fup message.
   *       Check if Pdelay_Resp_Fup message transmission succeeded.
   *       Protected against Ethernet reception interrupts:
   *         Check if state of the PdRespSm is still TX_PD_RESP_FUP to ensure that no new Pdelay cycle was started
   *         during an Pdelay_Req Rx interrupt.
   *           Change state to WAIT_RX_PD_REQ. */
  /* REDUX comment removed. */
  if (EthTSyn_PdRespSm_TxPdRespFup(PdRespSmIdx, seqId, reqPortNum, reqClockId, &pdRespOrgTs) == E_OK)
  {
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();

    if (EthTSyn_GetStateOfPdRespSmDyn(PdRespSmIdx) == ETHTSYN_TX_PD_RESP_FUP_STATEOFPDRESPSMDYN)
    {
      EthTSyn_SetStateOfPdRespSmDyn(PdRespSmIdx, ETHTSYN_WAIT_RX_PD_REQ_STATEOFPDRESPSMDYN);
    }
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();

# if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
    /* #20 Report time validation data if getting the data was successful (if enabled). */
    if (getTimeValidDataRetVal == E_OK)
    {
      const StbM_SynchronizedTimeBaseType tbId = EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(
        EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx)));

      (void)StbM_EthSetPdelayResponderData(tbId, &timeValidData);
    }
# endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */
  }
} /* EthTSyn_PdRespSm_ProcStateTxPdRespFup() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_TxPdResp
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
/* PRQA S 6050 1 */ /* MD_EthTSyn_6050_TrivialFunctions */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_TxPdResp(
                  EthTSyn_PdRespSmDynIterType  PdRespSmIdx,
                  uint16                       SequenceId,
                  uint16                       RequestorPortNum,
                  uint64                       RequestorClockId,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PdReqRcvdTsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
# if ( (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) || \
       (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) )
  /* #10 Try to take reference times (if required). */
  if (EthTSyn_PdRespSm_TakeRefTime(PdRespSmIdx) == E_OK)
# endif /* ( (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) || \
             (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) ) */
  {
    EthTSyn_GlobalTimestampType convertedPdReqRcvdTs = {0};

    convertedPdReqRcvdTs = (*PdReqRcvdTsPtr);
# if (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON)
    /* #20 Try to convert the Pdelay request received timestamp into a global timestamp (if required). */
    if (EthTSyn_PdRespSm_ConvertIngrTs(PdRespSmIdx, &convertedPdReqRcvdTs) == E_OK)
# endif /* (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) */
    {
      const EthTSyn_ComCtrlPortIdxOfPdRespSmType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx);
      const EthTSyn_TxConfContextIdxOfPdRespSmType txConfCtxIdx = EthTSyn_GetTxConfContextIdxOfPdRespSm(PdRespSmIdx);
      EthTSyn_MsgHdrTypeSpecType typeSpecHdr = {0};
      EthTSyn_EthTxBufferStructType txBufMgmt = {0};

      /* #30 Prepare the type specific message header.
       *       Try to start transmission sequence via the Tx unit.
       *         Copy the Pdelay_Resp specific payload to the Ethernet Tx buffer.
       *           Try to finalize the transmission sequence via the Tx unit.
       *             Report transmission of Pdelay_Resp message to PortParamStats unit. */

      typeSpecHdr.Control              = ETHTSYN_MSG_HDR_CONTROL_PDELAY_RESP;
      typeSpecHdr.CorrectionFieldNs    = ETHTSYN_MSG_HDR_COR_FIELD_DEFAULT;
      typeSpecHdr.CorrectionFieldSubNs = ETHTSYN_MSG_HDR_COR_FIELD_SUB_NS_DEFAULT;
      typeSpecHdr.Flags0               = ETHTSYN_MSG_HDR_FLAGS_0_SYNC_PDELAY_RESP;
      typeSpecHdr.Flags1               = ETHTSYN_MSG_HDR_FLAGS_1_DEFAULT;
      typeSpecHdr.LogMessageInterval   = ETHTSYN_MSG_HDR_LOG_MSG_INTERVAL_DEFAULT;
      typeSpecHdr.MessageLength        = ETHTSYN_MSG_PD_RESP_LEN;
      typeSpecHdr.MessageType          = ETHTSYN_MSG_TYPE_PD_RESP;
      typeSpecHdr.SequenceId           = SequenceId;

      if (EthTSyn_Tx_StartTxSequence(comCtrlPortIdx, txConfCtxIdx, &typeSpecHdr, NULL_PTR, &txBufMgmt) == E_OK)
      {
        /*@ assert txBufMgmt.BufferPtr != NULL_PTR; */                                                                 /* VCA_ETHTSYN_TX_BUF_VALID */
        /*@ assert $lengthOf(txBufMgmt.BufferPtr) >= typeSpecHdr.MessageLength; */                                     /* VCA_ETHTSYN_TX_BUF_VALID */
        EthTSyn_Mem_PutUint16(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_SEC_HI_OFS,
          convertedPdReqRcvdTs.secondsHi);
        EthTSyn_Mem_PutUint32(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_SEC_LOW_OFS,
          convertedPdReqRcvdTs.seconds);
        EthTSyn_Mem_PutUint32(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_NSEC_OFS,
          convertedPdReqRcvdTs.nanoseconds);
        EthTSyn_Mem_PutUint64(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_REQ_CLOCK_IDENTITY_OFS, RequestorClockId);
        EthTSyn_Mem_PutUint16(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_REQ_PORT_NUM_OFS, RequestorPortNum);

        if (EthTSyn_Tx_FinishTxSequence(comCtrlPortIdx, txConfCtxIdx, txBufMgmt.BufferIdx,
          ETHTSYN_MSG_PD_RESP_LEN) == E_OK)
        {
          EthTSyn_PortParamStats_IncTxPdRespCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(comCtrlPortIdx));
          retVal = E_OK;
        }
      }
    }
  }

  return retVal;
} /* EthTSyn_PdRespSm_TxPdResp() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_TxPdRespFup
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_TxPdRespFup(
                  EthTSyn_PdRespSmDynIterType  PdRespSmIdx,
                  uint16                       SequenceId,
                  uint16                       RequestorPortNum,
                  uint64                       RequestorClockId,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PdRespOrgTsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_GlobalTimestampType convertedPdRespOrgTs = {0};

  /* ----- Implementation ----------------------------------------------- */
  convertedPdRespOrgTs = (*PdRespOrgTsPtr);
# if (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON)
  /* #10 Try to convert the Pdelay response origin timestamp into a global timestamp (if required). */
  if (EthTSyn_PdRespSm_ConvertEgrTs(PdRespSmIdx, &convertedPdRespOrgTs) == E_OK)
# endif /* (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) */
  {
    const EthTSyn_ComCtrlPortIdxOfPdRespSmType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx);
    const EthTSyn_TxConfContextIdxOfPdRespSmType txConfCtxIdx = EthTSyn_GetTxConfContextIdxOfPdRespSm(PdRespSmIdx);
    EthTSyn_MsgHdrTypeSpecType typeSpecHdr = {0};
    EthTSyn_EthTxBufferStructType txBufMgmt = {0};

    /* #20 Prepare the type specific message header.
     *       Try to start transmission sequence via the Tx unit.
     *         Copy the Pdelay_Resp_Fup specific payload to the Ethernet Tx buffer.
     *           Try to finalize the transmission sequence via the Tx unit.
     *             Report transmission of Pdelay_Resp_Fup message to PortParamStats unit. */

    typeSpecHdr.Control              = ETHTSYN_MSG_HDR_CONTROL_PDELAY_RESP_FUP;
    typeSpecHdr.CorrectionFieldNs    = ETHTSYN_MSG_HDR_COR_FIELD_DEFAULT;
    typeSpecHdr.CorrectionFieldSubNs = ETHTSYN_MSG_HDR_COR_FIELD_SUB_NS_DEFAULT;
    typeSpecHdr.Flags0               = ETHTSYN_MSG_HDR_FLAGS_0_DEFAULT;
    typeSpecHdr.Flags1               = ETHTSYN_MSG_HDR_FLAGS_1_DEFAULT;
    typeSpecHdr.LogMessageInterval   = ETHTSYN_MSG_HDR_LOG_MSG_INTERVAL_DEFAULT;
    typeSpecHdr.MessageLength        = ETHTSYN_MSG_PD_RESP_FUP_LEN;
    typeSpecHdr.MessageType          = ETHTSYN_MSG_TYPE_PD_RESP_FUP;
    typeSpecHdr.SequenceId           = SequenceId;

    if (EthTSyn_Tx_StartTxSequence(comCtrlPortIdx, txConfCtxIdx, &typeSpecHdr, NULL_PTR, &txBufMgmt) == E_OK)
    {
      /*@ assert txBufMgmt.BufferPtr != NULL_PTR; */                                                                   /* VCA_ETHTSYN_TX_BUF_VALID */
      /*@ assert $lengthOf(txBufMgmt.BufferPtr) >= typeSpecHdr.MessageLength; */                                       /* VCA_ETHTSYN_TX_BUF_VALID */
      EthTSyn_Mem_PutUint16(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_SEC_HI_OFS,
        convertedPdRespOrgTs.secondsHi);
      EthTSyn_Mem_PutUint32(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_SEC_LOW_OFS,
        convertedPdRespOrgTs.seconds);
      EthTSyn_Mem_PutUint32(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_NSEC_OFS,
        convertedPdRespOrgTs.nanoseconds);
      EthTSyn_Mem_PutUint64(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_FUP_REQ_CLOCK_IDENTITY_OFS, RequestorClockId);
      EthTSyn_Mem_PutUint16(txBufMgmt.BufferPtr, ETHTSYN_MSG_PD_RESP_FUP_REQ_PORT_NUM_OFS, RequestorPortNum);

      if (EthTSyn_Tx_FinishTxSequence(comCtrlPortIdx, txConfCtxIdx, txBufMgmt.BufferIdx,
        ETHTSYN_MSG_PD_RESP_FUP_LEN) == E_OK)
      {
        EthTSyn_PortParamStats_IncTxPdRespFupCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(comCtrlPortIdx));
        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_PdRespSm_TxPdRespFup() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_GetPdRxTxTsMgmt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_TsMgmtType, ETHTSYN_CODE) EthTSyn_PdRespSm_GetPdRxTxTsMgmt(
                  EthTSyn_TsDirType   Dir,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType) TsInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TsMgmtType pdTsMgmt = {0u};

  /* ----- Implementation ----------------------------------------------- */
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  pdTsMgmt = ((Dir == ETHTSYN_TS_DIR_TX) ? TsInfoPtr->SwtEgrTsMgmt : TsInfoPtr->SwtIngrTsMgmt);
# else
  pdTsMgmt = EthTSyn_Ts_IntVltMgmtToTsMgmt(&TsInfoPtr->IntVltMgmt);
  ETHTSYN_DUMMY_STATEMENT(Dir); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  return pdTsMgmt;
} /* EthTSyn_PdRespSm_GetPdRxTxTsMgmt() */

# if ( (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) || \
       (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) )
/**********************************************************************************************************************
 * EthTSyn_PdRespSm_TakeRefTime
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_TakeRefTime(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if reference time is required. */
  if (EthTSyn_PdRespSm_IsRefTimeRequired(PdRespSmIdx))
  {
    const EthTSyn_ComCtrlPortIdxOfPdRespSmType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx);
    const StbM_SynchronizedTimeBaseType tbId = EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(
      EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(comCtrlPortIdx));
    StbM_TimeStampType stbmGlobalTime = {0u};
    StbM_VirtualLocalTimeType localTime = {0u};
    StbM_UserDataType userData = {0u};

    /* #20 Try to retrieve the current global and local time from StbM.
     *       Check if the timestamp format of the global time is valid.
     *         Store current global and local time for later use. */
    if (StbM_BusGetCurrentTime(tbId, &stbmGlobalTime, &localTime, &userData) == E_OK)
    {
      if (EthTSyn_Ts_IsStbmTsValid(&stbmGlobalTime) == TRUE)
      {
        EthTSyn_GlobalTimestampType globalTime = {0u};

        globalTime.secondsHi = stbmGlobalTime.secondsHi;
        globalTime.seconds = stbmGlobalTime.seconds;
        globalTime.nanoseconds = stbmGlobalTime.nanoseconds;

        EthTSyn_SetRefLocalTimeOfPdRespSmDyn(PdRespSmIdx, localTime);
        EthTSyn_SetRefGlobalTimeOfPdRespSmDyn(PdRespSmIdx, globalTime);

        /* #30 Get additional reference times. */
        retVal = EthTSyn_PdRespSm_TakeAddRefTime(PdRespSmIdx);
      }
    }
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_PdRespSm_TakeRefTime() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_IsRefTimeRequired
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PdRespSm_IsRefTimeRequired(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isRefTimeRequired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
  if (EthTSyn_IsTimeValidationEnabledOfPdRespSm(PdRespSmIdx))
  {
    isRefTimeRequired = TRUE;
  }
#  endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

#  if (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON)
  if (EthTSyn_IsUseGlobalTimeOfPdRespSm(PdRespSmIdx))
  {
    isRefTimeRequired = TRUE;
  }
#  endif /* (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) */

  return isRefTimeRequired;
} /* EthTSyn_PdRespSm_IsRefTimeRequired() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_TakeAddRefTime
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_TakeAddRefTime(
  EthTSyn_PdRespSmDynIterType PdRespSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
#  if ( (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) )
  /* #10 Try to get additional reference times from StbM and EthIf for later time conversion (if
   *     UseGlobalTimeSupport and HW-Timestamping are enabled). */
  {
    const EthTSyn_ComCtrlPortIdxOfPdRespSmType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx);

    if ((EthTSyn_IsUseGlobalTimeOfPdRespSm(PdRespSmIdx)) &&
      (!EthTSyn_IsStbMTimeSrcEqualOfComCtrlPort(comCtrlPortIdx)))
    {
      const uint8 ethIfCtrlIdx = EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(
        EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(comCtrlPortIdx));
      const StbM_SynchronizedTimeBaseType tbId = EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(
        EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(comCtrlPortIdx));
      Std_ReturnType stbmRetVal = E_NOT_OK;
      Std_ReturnType ethIfRetVal = E_NOT_OK;
      StbM_VirtualLocalTimeType currVlt = {0u};
      EthTSyn_GlobalTimestampType currEthTime = {0u};
      Eth_TimeStampQualType currEthTimeQual = ETH_INVALID;

      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();
      stbmRetVal = StbM_GetCurrentVirtualLocalTime(tbId, &currVlt);
      ethIfRetVal = EthIf_GetCurrentTime(ethIfCtrlIdx, &currEthTimeQual, &currEthTime);
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();

      if ((stbmRetVal == E_OK) && (ethIfRetVal == E_OK) && (currEthTimeQual == ETH_VALID))
      {
        const EthTSyn_IntVltType currIntVlt = EthTSyn_Ts_VltToIntVlt(&currVlt);
        EthTSyn_IntVltType currEthTimeAsVlt = 0u;

        retVal = EthTSyn_Ts_TsToIntVltChecked(&currEthTime, &currEthTimeAsVlt);
        EthTSyn_SetRefStbmVltOfPdRespSmDyn(PdRespSmIdx, currIntVlt);
        EthTSyn_SetRefHwTimeOfPdRespSmDyn(PdRespSmIdx, currEthTimeAsVlt);
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }
#  else
  ETHTSYN_DUMMY_STATEMENT(PdRespSmIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* ( (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) ) */

  return retVal;
} /* EthTSyn_PdRespSm_TakeAddRefTime() */
# endif /* ( (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) || \
             (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) ) */

# if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_PdRespSm_GetTimeValidData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_GetTimeValidData(
                EthTSyn_PdRespSmDynIterType          PdRespSmIdx,
  ETHTSYN_P2VAR(StbM_PdelayResponderMeasurementType) TimeValidDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  StbM_VirtualLocalTimeType requestReceiptVlt = {0u};
  StbM_VirtualLocalTimeType responseOriginVlt = {0u};
  StbM_PdelayResponderMeasurementType timeValidData = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to convert request receipt and response origin timestamps into virtual local time.
   *       Get all time validation data. */
  retVal = EthTSyn_Ts_TsToVltChecked(EthTSyn_GetAddrPdReqRcvdTsOfPdRespSmDyn(PdRespSmIdx), &requestReceiptVlt);
  retVal |= EthTSyn_Ts_TsToVltChecked(EthTSyn_GetAddrPdRespOrgTsOfPdRespSmDyn(PdRespSmIdx), &responseOriginVlt);

  if (retVal == E_OK)
  {
    const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx);
    EthTSyn_GlobalTimestampType refGlobalTime = {0u};

    refGlobalTime = EthTSyn_GetRefGlobalTimeOfPdRespSmDyn(PdRespSmIdx);

    timeValidData.sequenceId                           = EthTSyn_GetSequenceIdOfPdRespSmDyn(PdRespSmIdx);
    timeValidData.requestPortId.portNumber             = EthTSyn_GetRequestorPortNumOfPdRespSmDyn(PdRespSmIdx);
    timeValidData.requestPortId.clockIdentity          = EthTSyn_GetRequestorClockIdOfPdRespSmDyn(PdRespSmIdx);
    timeValidData.responsePortId.portNumber            = EthTSyn_GetNumberOfComCtrlPort(comCtrlPortIdx);
    timeValidData.responsePortId.clockIdentity         = EthTSyn_ComCtrl_GetClockId(comCtrlPortIdx);
    timeValidData.requestReceiptTimestamp              = requestReceiptVlt;
    timeValidData.responseOriginTimestamp              = responseOriginVlt;
    timeValidData.referenceLocalTimestamp              = EthTSyn_GetRefLocalTimeOfPdRespSmDyn(PdRespSmIdx);
    timeValidData.referenceGlobalTimestamp.seconds     = refGlobalTime.seconds;
    timeValidData.referenceGlobalTimestamp.nanoseconds = refGlobalTime.nanoseconds;
  }

  /* #20 Set out-parameter. */
  (*TimeValidDataPtr) = timeValidData;

  return retVal;
} /* EthTSyn_PdRespSm_GetTimeValidData() */
# endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

# if (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ConvertIngrTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_ConvertIngrTs(
                EthTSyn_PdRespSmDynIterType  PdRespSmIdx,
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if timestamp conversion is required. */
  if (EthTSyn_IsUseGlobalTimeOfPdRespSm(PdRespSmIdx))
  {
    /* #20 Convert the timestamp depending on which time source is used by StbM and EthTSyn and the timestamp
     *     direction. */
#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    if (!EthTSyn_IsStbMTimeSrcEqualOfComCtrlPort(EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx)))
    {
      retVal = EthTSyn_Ts_IngrTsToGlobalTsTimeSrcNeq(TsPtr,
        EthTSyn_GetAddrRefLocalTimeOfPdRespSmDyn(PdRespSmIdx),
        EthTSyn_GetAddrRefGlobalTimeOfPdRespSmDyn(PdRespSmIdx),
        EthTSyn_GetRefStbmVltOfPdRespSmDyn(PdRespSmIdx), EthTSyn_GetRefHwTimeOfPdRespSmDyn(PdRespSmIdx));
    }
    else
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
    {
      retVal = EthTSyn_Ts_IngrTsToGlobalTsTimeSrcEq(TsPtr,
        EthTSyn_GetAddrRefLocalTimeOfPdRespSmDyn(PdRespSmIdx),
        EthTSyn_GetAddrRefGlobalTimeOfPdRespSmDyn(PdRespSmIdx));
    }
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_PdRespSm_ConvertIngrTs() */

/**********************************************************************************************************************
 * EthTSyn_PdRespSm_ConvertEgrTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PdRespSm_ConvertEgrTs(
                EthTSyn_PdRespSmDynIterType  PdRespSmIdx,
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if timestamp conversion is required. */
  if (EthTSyn_IsUseGlobalTimeOfPdRespSm(PdRespSmIdx))
  {
    /* #20 Convert the timestamp depending on which time source is used by StbM and EthTSyn and the timestamp
     *     direction. */
#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    if (!EthTSyn_IsStbMTimeSrcEqualOfComCtrlPort(EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx)))
    {
       retVal = EthTSyn_Ts_EgrTsToGlobalTsTimeSrcNeq(TsPtr,
         EthTSyn_GetAddrRefLocalTimeOfPdRespSmDyn(PdRespSmIdx),
         EthTSyn_GetAddrRefGlobalTimeOfPdRespSmDyn(PdRespSmIdx),
         EthTSyn_GetRefStbmVltOfPdRespSmDyn(PdRespSmIdx), EthTSyn_GetRefHwTimeOfPdRespSmDyn(PdRespSmIdx));
    }
    else
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
    {
      retVal = EthTSyn_Ts_EgrTsToGlobalTsTimeSrcEq(TsPtr,
        EthTSyn_GetAddrRefLocalTimeOfPdRespSmDyn(PdRespSmIdx),
        EthTSyn_GetAddrRefGlobalTimeOfPdRespSmDyn(PdRespSmIdx));
    }
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_PdRespSm_ConvertEgrTs() */
# endif /* (ETHTSYN_PD_RESP_USE_GLOBAL_TIME_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdRespSmDynIterType pdRespSmDynIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (pdRespSmDynIter = 0u; pdRespSmDynIter < EthTSyn_GetSizeOfPdRespSmDyn(); pdRespSmDynIter++)
  {
    EthTSyn_PdRespSm_IntInit(pdRespSmDynIter);
  }
} /* EthTSyn_PdRespSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdRespSmDynIterType pdRespSmDynIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (pdRespSmDynIter = 0u; pdRespSmDynIter < EthTSyn_GetSizeOfPdRespSmDyn(); pdRespSmDynIter++)
  {
    EthTSyn_PdRespSm_Proc(pdRespSmDynIter);
  }
} /* EthTSyn_PdRespSm_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_Reset(
  EthTSyn_PdRespSm_SmIdxType PdRespSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if (PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn())
  {
    EthTSyn_SetSmResetRequestedOfPdRespSmDyn(PdRespSmIdx, TRUE);
  }
} /* EthTSyn_PdRespSm_Reset() */

/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_ProcRcvdPdReq
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
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProcRcvdPdReq(
                  EthTSyn_PdRespSm_SmIdxType  PdRespSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr,
  /* Note: Pdelay_Req does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)         RxTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if passed PdRespSm handle is valid. */
  if (PdRespSmIdx < EthTSyn_GetSizeOfPdRespSmDyn())
  {
    /* #20 Verify message length in header. */
    if (PtpHdrPtr->MessageLength == ETHTSYN_MSG_PD_REQ_LEN)
    {
      /* #30 Enter critical section to exclude Ethernet transmission interrupts. */
      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
      {
        const EthTSyn_StateOfPdRespSmDynType state = EthTSyn_GetStateOfPdRespSmDyn(PdRespSmIdx);
        boolean acceptMsg = FALSE;

        /* #40 Accept message if:
         *       PdRespSm is in state WAIT_RX_PD_REQ or
         *       PdRespSm is in a state != INITIALIZING and the Pdelay_Req belongs to another Pdelay cycle. */
        if (state == ETHTSYN_WAIT_RX_PD_REQ_STATEOFPDRESPSMDYN)
        {
          acceptMsg = TRUE;
        }
        else if ((state != ETHTSYN_INITIALIZING_STATEOFPDRESPSMDYN) &&
          (PtpHdrPtr->SequenceId != EthTSyn_GetSequenceIdOfPdRespSmDyn(PdRespSmIdx)))
        {
          acceptMsg = TRUE;
        }
        else
        {
          acceptMsg = FALSE;
        }

        if (acceptMsg == TRUE)
        {
          EthTSyn_TsMgmtType pdReqIngrTsMgmt = {0u};
          /* #50 Check if the Pdelay_Req ingress timestamp is valid.
           *       Store the sequence id, port identity of the requestor and the Pdelay_Req ingress timestamp for later
           *       transmission of the corresponding Pdelay_Resp and Pdelay_Resp_Fup.
           *       Report reception of Pdelay_Req message to PortParamStats unit.
           *       Change state of the PdRespSm to TX_PD_RESP. */

          pdReqIngrTsMgmt = EthTSyn_PdRespSm_GetPdRxTxTsMgmt(ETHTSYN_TS_DIR_RX, RxTsInfoPtr);

          if (pdReqIngrTsMgmt.TsValid == TRUE)
          {
            EthTSyn_SetSequenceIdOfPdRespSmDyn(PdRespSmIdx, PtpHdrPtr->SequenceId);
            EthTSyn_SetRequestorPortNumOfPdRespSmDyn(PdRespSmIdx, PtpHdrPtr->SourcePortNumber);
            EthTSyn_SetRequestorClockIdOfPdRespSmDyn(PdRespSmIdx, PtpHdrPtr->SourceClockId);

            EthTSyn_SetPdReqRcvdTsOfPdRespSmDyn(PdRespSmIdx, pdReqIngrTsMgmt.Ts);

            EthTSyn_PortParamStats_IncRxPdReqCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
              EthTSyn_GetComCtrlPortIdxOfPdRespSm(PdRespSmIdx)));

            EthTSyn_SetStateOfPdRespSmDyn(PdRespSmIdx, ETHTSYN_TX_PD_RESP_STATEOFPDRESPSMDYN);
          }
        }
      }
      /* #60 Leave critical section. */
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
    }
  }
} /* EthTSyn_PdRespSm_ProcRcvdPdReq() */

/**********************************************************************************************************************
 *  EthTSyn_PdRespSm_ProvidePdRespEgrTs
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
FUNC(void, ETHTSYN_CODE) EthTSyn_PdRespSm_ProvidePdRespEgrTs(
                  EthTSyn_PdRespSm_SmIdxType PdRespSmIdx,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)        EgrTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
  if (EthTSyn_GetStateOfPdRespSmDyn(PdRespSmIdx) == ETHTSYN_WAIT_PD_RESP_EGR_TS_STATEOFPDRESPSMDYN)
  {
    EthTSyn_TsMgmtType egrTsMgmt = {0u};

    egrTsMgmt = EthTSyn_PdRespSm_GetPdRxTxTsMgmt(ETHTSYN_TS_DIR_TX, EgrTsInfoPtr);

    if (egrTsMgmt.TsValid == TRUE)
    {
      EthTSyn_SetPdRespOrgTsOfPdRespSmDyn(PdRespSmIdx, egrTsMgmt.Ts);
      EthTSyn_SetStateOfPdRespSmDyn(PdRespSmIdx, ETHTSYN_TX_PD_RESP_FUP_STATEOFPDRESPSMDYN);
    }
    else
    {
      EthTSyn_SetStateOfPdRespSmDyn(PdRespSmIdx, ETHTSYN_WAIT_RX_PD_REQ_STATEOFPDRESPSMDYN);
    }
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
} /* EthTSyn_PdRespSm_ProvidePdRespEgrTs() */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PdelayResponder.c
 *********************************************************************************************************************/
