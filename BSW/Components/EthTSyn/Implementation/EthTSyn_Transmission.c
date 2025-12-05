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
/*!        \file  EthTSyn_Transmission.c
 *        \brief  EthTSyn Transmission source file
 *      \details  Implementation of all units included in the logical unit Transmission (Tx, TxConf and SwtMgmtTx).
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

#define ETHTSYN_TRANSMISSION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_Tx_Int.h"
#include "EthTSyn_TxConf_Int.h"
#include "EthTSyn_TxConf_Cbk.h"

#include "EthTSyn.h"
#include "EthTSyn_Lcfg.h"
#include "EthTSyn_MsgDefs_Int.h"
#include "EthTSyn_Memory_Int.h"
#include "EthTSyn_GeneratedCfgAccess.h"

#include "EthTSyn_ComCtrl_Int.h"
#include "EthTSyn_Init_Int.h"
#include "EthTSyn_Timestamp_Int.h"

#include "EthTSyn_PortSyncSendSm_Int.h"
#include "EthTSyn_PdReqTrcv_Int.h"
#include "EthTSyn_PdRespSm_Int.h"
#include "EthTSyn_SyncSendTx_Int.h"
#include "EthTSyn_SwtMgmtTx_Int.h"
#include "EthTSyn_SwtMgmtQ_Int.h"

#include "StbM.h"
#include "EthIf.h"
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

#include "vstdlib.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define ETHTSYN_INVALID_TX_BUFFER_IDX                                 0xFFu

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
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (ETHTSYN_TX_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * Internal function prototypes of Tx
 *!
 * \unit Tx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_TX)
#  if ((ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  EthTSyn_Tx_PrepareTx
 *********************************************************************************************************************/
/*! \brief        Prepares message transmission.
 *  \details      Enables hardware timestamping (if required and enabled) and sets SwtMgmtInfo (if SwtMgmt is
 *                supported). Releases the Tx buffer in case of failure.
 *  \param[in]    ComCtrlPortIdx  Index of the port
 *                                [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]    BufIdx          Index of the buffer resource
 *  \param[in]    IsEgrTsRequired Indicates if egress timestamp is required
 *  \return       E_OK - Preparation was successful
 *  \return       E_NOT_OK - Preparation failed
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Tx_PrepareTx(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  uint8                       BufIdx,
  boolean                     IsEgrTsRequired);
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) && (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_Tx_SerializeHeader
 *********************************************************************************************************************/
/*! \brief        Serializes and copies (in Network-Byte-Order) the PTP header to the Ethernet Tx buffer.
 *  \details      -
 *  \param[in]    ComCtrlPortIdx    Index of the port
 *                                  [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]    TypeSpecHeaderPtr Object containing all message type specific fields of the PTP header.
 *  \param[in]    PortInfoHeaderPtr Object containing all port information of the PTP header. If it is NULL_PTR,
 *                                  port information must be retrieved from ComCtrl unit.
 *                                  [range: can be NULL_PTR]
 *  \param[out]   TxBufferPtr       Tx buffer retrieved from EthIf
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Tx_SerializeHeader(
                  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType) TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType) PortInfoHeaderPtr,
    ETHTSYN_P2VAR(uint8)                      TxBufferPtr);

# endif /* ETHTSYN_NOUNIT_TX */

/**********************************************************************************************************************
 * Internal function prototypes of TxConf
 *!
 * \unit TxConf
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_TXCONF)
/**********************************************************************************************************************
 *  EthTSyn_TxConf_IntTxConfirmation
 *********************************************************************************************************************/
/*! \brief        Processes an invoked Tx confirmation.
 *  \details      Find active Tx confirmation context, retrieve and dispatch egress timestamp and deactivate context.
 *  \param[in]    CtrlIdx EthIf controller index
 *  \param[in]    BufIdx  Index of the buffer resource
 *  \pre          Must be called in context of the EthTSyn_TxConfirmation()
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different Controller and/or Buffer indices
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_IntTxConfirmation(
  uint8 CtrlIdx,
  uint8 BufIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_GetEgrTsAsIntVlt
 *********************************************************************************************************************/
/*! \brief        Gets the egress timestamp (as internal virtual local time) of a transmitted EthTSyn frame.
 *  \details      -
 *  \param[in]    CtrlIdx        EthIf controller index
 *  \param[in]    BufIdx         Index of the buffer resource
 *  \param[in]    ComCtrlPortIdx Index of the port
 *                               [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \return       The egress timestamp expressed as internal virtual local time together with its validity information
 *  \pre          Must be called in context of the EthTSyn_TxConfirmation()
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different Controller and/or Buffer indices
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_IntVltMgmtType, ETHTSYN_CODE) EthTSyn_TxConf_GetEgrTsAsIntVlt(
  uint8                         CtrlIdx,
  uint8                         BufIdx,
  EthTSyn_SizeOfComCtrlPortType ComCtrlPortIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_DispatchEgrTs
 *********************************************************************************************************************/
/*! \brief        Forwards information about egress and switch timestamps to transmitting unit.
 *  \details      -
 *  \param[in]    TxConfContextIdx Index of TxConf context
 *                                 [range: TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext()]
 *  \param[in]    TxTsInfoPtr      Info about the transmission timestamp(s)
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different Tx confirmation context indices
 *  \spec
 *    requires TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_DispatchEgrTs(
                EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
ETHTSYN_P2CONST(EthTSyn_TsInfoType)             TxTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_IntReleaseTxContext
 *********************************************************************************************************************/
/*! \brief        Releases pending SwtMgmtTx object and deactivates Tx confirmation context.
 *  \details      -
 *  \param[in]    TxConfContextIdx Index of TxConf context
 *                                 [range: TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext()]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_IntReleaseTxContext(
  EthTSyn_SizeOfTxConfContextType     TxConfContextIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_FindContext
 *********************************************************************************************************************/
/*! \brief        Finds active Tx confirmation context matching to given EthIf controller and buffer index.
 *  \details      -
 *  \param[in]    CtrlIdx  EthIf controller index
 *  \param[in]    BufIdx   Index of the buffer resource
 *  \return       EthTSyn_GetSizeOfTxConfContext() - No matching active Tx confirmation context found.
 *  \return       Any other value - Index of matching Tx confirmation context.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different Controller and/or Buffer indices
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SizeOfTxConfContextType, ETHTSYN_CODE) EthTSyn_TxConf_FindContext(
  uint8 CtrlIdx,
  uint8 BufIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ActivateContext
 *********************************************************************************************************************/
/*! \brief        Adds Tx confirmation context at the end of the list of active contexts.
 *  \details      -
 *  \param[in]    TxConfContextIdx Index of TxConf context
 *                                 [range: TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext()]
 *  \pre          Must be called in the context of ETHTSYN_EXCLUSIVE_AREA_TX_CONF.
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ActivateContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx);

/**********************************************************************************************************************
 *  EthTSyn_TxConf_DeactivateContext
 *********************************************************************************************************************/
/*! \brief        Removes Tx confirmation context from the list of active contexts.
 *  \details      -
 *  \param[in]    TxConfContextIdx Index of TxConf context
 *                                 [range: TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext()]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different Tx confirmation contexts
 *  \spec
 *    requires TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_DeactivateContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx);

# endif /* ETHTSYN_NOUNIT_TXCONF */

/**********************************************************************************************************************
 * Internal function prototypes of SwtMgmtTx
 *!
 * \unit SwtMgmtTx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SWTMGMTTX)
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
#   if (ETHTSYN_SWT_MGMT_ONE_TAS == STD_OFF)
/**********************************************************************************************************************
 * EthTSyn_SwtMgmtTx_CalcSyncSwtResTime
 *********************************************************************************************************************/
/*! \brief          Calculates the switch residence time of a transmitted Sync message.
 *  \details        When the host is acting as time master, every transmitted Sync message passes the switch which
 *                  causes an additional delay (switch residence time) not included in Pdelay. To compensate this
 *                  delay, the residence time is calculated and added as correction value to the FollowUp message.
 *                  This routine calculates the switch residence time (uplink to Tx). If calculation is not possible,
 *                  E_NOT_OK is returned.
 *  \param[in]      SyncSwtIngrTsMgmtPtr  Switch ingress timestamp (host port) and validity info of transmitted Sync
 *  \param[in]      SyncSwtEgrTsMgmtPtr   Switch egress timestamp and validity info of transmitted Sync
 *  \param[out]     SwtResTimePtr         The switch residence time of the Sync message
 *  \return         E_OK - Switch residence time is valid
 *  \return         E_NOT_OK - Switch residence time is invalid
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_CalcSyncSwtResTime(
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)    SyncSwtIngrTsMgmtPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)    SyncSwtEgrTsMgmtPtr,
    ETHTSYN_P2VAR(EthTSyn_UTimediffType) SwtResTimePtr);
#   endif /* (ETHTSYN_SWT_MGMT_ONE_TAS == STD_OFF) */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
# endif /* ETHTSYN_NOUNIT_SWTMGMTTX */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Internal functions of Tx
 *!
 * \unit Tx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_TX)
#  if ((ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  EthTSyn_Tx_PrepareTx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Tx_PrepareTx(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  uint8                       BufIdx,
  boolean                     IsEgrTsRequired)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 ethIfCtrlIdx = EthTSyn_ComCtrl_GetEthIfCtrlIdx(ComCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call EthIf to enable hardware egress timestamping if required and enabled. */
#   if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  if (IsEgrTsRequired)
  {
    retVal = EthIf_EnableEgressTimestamp(ethIfCtrlIdx, BufIdx);
  }
#   endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

  /* #20 Call SwtMgmtTx to prepare transmission if switch management is enabled. */
#   if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  retVal |= EthTSyn_SwtMgmtTx_PrepTx(ComCtrlPortIdx, BufIdx, IsEgrTsRequired); /* PRQA S 2986 */ /* MD_EthTSyn_2986_PP */
#   endif /* ETHTSYN_SWT_MGMT_SUPPORT == STD_ON */

  /* #30 If message preparation failed, release Tx buffer by calling EthIf_Transmit with LenByte of 0. */
  if (retVal != E_OK)
  {
    /* Since the Tx buffer is released, the destination MAC does not matter. Just use the first generated address for
     * simplified access. */
    EthTSyn_PhysAddrType destAddr = {0u};

    destAddr[0u] = EthTSyn_GetByte0OfDestinationMac(0u);
    destAddr[1u] = EthTSyn_GetByte1OfDestinationMac(0u);
    destAddr[2u] = EthTSyn_GetByte2OfDestinationMac(0u);
    destAddr[3u] = EthTSyn_GetByte3OfDestinationMac(0u);
    destAddr[4u] = EthTSyn_GetByte4OfDestinationMac(0u);
    destAddr[5u] = EthTSyn_GetByte5OfDestinationMac(0u);

    (void)EthIf_Transmit(ethIfCtrlIdx, BufIdx, ETHTSYN_FRAME_TYPE, FALSE, 0u, destAddr);
  }

  return retVal;
} /* EthTSyn_Tx_PrepareTx */
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) && (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

/**********************************************************************************************************************
 *  EthTSyn_Tx_SerializeHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Tx_SerializeHeader(
                  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType) TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType) PortInfoHeaderPtr,
    ETHTSYN_P2VAR(uint8)                      TxBufferPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_MsgHdrPortInfoType locPortInfoHeader = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear the Ethernet Tx buffer (only the relevant part). */
  VStdLib_MemClr(TxBufferPtr, TypeSpecHeaderPtr->MessageLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

  /* #20 Retrieve values of general header fields from ComCtrl unit if not provided. */
  if (PortInfoHeaderPtr == NULL_PTR)
  {
    locPortInfoHeader.SourceClockId = EthTSyn_ComCtrl_GetClockId(ComCtrlPortIdx);
    locPortInfoHeader.SourcePortNumber = EthTSyn_GetNumberOfComCtrlPort(ComCtrlPortIdx);
    locPortInfoHeader.DomainNumber = EthTSyn_ComCtrl_GetTimeDomainId(ComCtrlPortIdx);
  }
  /* #30 Otherwise, use the provided port info. */
  else
  {
    locPortInfoHeader = (*PortInfoHeaderPtr);
  }

  /* #40 Serialize entire header. */
  EthTSyn_Mem_PutUint8(TxBufferPtr, ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_OFS,
    ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC | TypeSpecHeaderPtr->MessageType);
  EthTSyn_Mem_PutUint8(TxBufferPtr, ETHTSYN_MSG_HDR_RESERVED_0_OFS, ETHTSYN_PTP_VERSION);
  EthTSyn_Mem_PutUint16(TxBufferPtr, ETHTSYN_MSG_HDR_MSG_LENGTH_OFS, TypeSpecHeaderPtr->MessageLength);
  EthTSyn_Mem_PutUint8(TxBufferPtr, ETHTSYN_MSG_HDR_DOMAIN_NUM_OFS, locPortInfoHeader.DomainNumber);
  EthTSyn_Mem_PutUint8(TxBufferPtr, ETHTSYN_MSG_HDR_FLAGS_0_OFS, TypeSpecHeaderPtr->Flags0);
  EthTSyn_Mem_PutUint8(TxBufferPtr, ETHTSYN_MSG_HDR_FLAGS_1_OFS, TypeSpecHeaderPtr->Flags1);
  EthTSyn_Mem_PutUint48(TxBufferPtr, ETHTSYN_MSG_HDR_CORR_FIELD_OFS, TypeSpecHeaderPtr->CorrectionFieldNs);
  EthTSyn_Mem_PutUint16(TxBufferPtr, ETHTSYN_MSG_HDR_CORR_FIELD_SUB_NS_OFS, TypeSpecHeaderPtr->CorrectionFieldSubNs);
  EthTSyn_Mem_PutUint64(TxBufferPtr, ETHTSYN_MSG_HDR_SRC_PORT_IDENT_CLOCK_IDENT_OFS, locPortInfoHeader.SourceClockId);
  EthTSyn_Mem_PutUint16(TxBufferPtr, ETHTSYN_MSG_HDR_SRC_PORT_IDENT_PORT_NUM_OFS, locPortInfoHeader.SourcePortNumber);
  EthTSyn_Mem_PutUint16(TxBufferPtr, ETHTSYN_MSG_HDR_SEQUENCE_ID_OFS, TypeSpecHeaderPtr->SequenceId);
  EthTSyn_Mem_PutUint8(TxBufferPtr, ETHTSYN_MSG_HDR_CONTROL_OFS, TypeSpecHeaderPtr->Control);
  EthTSyn_Mem_PutUint8(TxBufferPtr, ETHTSYN_MSG_HDR_LOG_MSG_INTERVAL_OFS,
    (uint8)TypeSpecHeaderPtr->LogMessageInterval);
} /* EthTSyn_Tx_SerializeMsg */
# endif /* ETHTSYN_NOUNIT_TX */

/**********************************************************************************************************************
 * Internal functions of TxConf
 *!
 * \unit TxConf
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_TXCONF)
/**********************************************************************************************************************
 *  EthTSyn_TxConf_IntTxConfirmation
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
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_IntTxConfirmation(
  uint8 CtrlIdx,
  uint8 BufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SizeOfTxConfContextType txConfContIdx = EthTSyn_GetSizeOfTxConfContext();

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF)
  /* #10 Enter critical section to ensure that time between invocation of TxConfirmation and getting the software
   *     egress timestamp is minimized (if SW-Timestamping is used). */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) */

  /* #20 Find active Tx confirmation context. */
  txConfContIdx = EthTSyn_TxConf_FindContext(CtrlIdx, BufIdx);

  if (txConfContIdx < EthTSyn_GetSizeOfTxConfContext())
  {
    /* #200 If active context was found, retrieve egress timestamp. */
    EthTSyn_IntVltMgmtType egrTsAsIntVltMgmt = {0};

    egrTsAsIntVltMgmt = EthTSyn_TxConf_GetEgrTsAsIntVlt(CtrlIdx, BufIdx,
      EthTSyn_GetComCtrlPortIdxOfTxConfContext(txConfContIdx));

#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF)
    /* #201 Leave critical section (if SW-Timestamping is used). */
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) */
    {
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
      /* #202 Try to get SwtMgmtTxMgmt object if switch management is supported. */
      const EthTSyn_ComCtrl_EthIfCtrlIdxType comCtrlEthIfCtrlIdx = EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(
        EthTSyn_GetComCtrlPortIdxOfTxConfContext(txConfContIdx));
      const EthTSyn_SwtMgmtTx_IdxType swtMgmtTxIdx = EthTSyn_SwtMgmtTx_HndlTxConf(comCtrlEthIfCtrlIdx, BufIdx,
        txConfContIdx);

      if (swtMgmtTxIdx < EthTSyn_GetSizeOfSwtMgmtTxDyn())
      {
        /* #2020 If successful,
         *   store SwtMgmtTxMgmtObjIdx and egress timestamp for later processing.
         *   Unmap the TxConfContext from the Ethernet TxBufferIndex because the TxConfirmation was invoked. */
        EthTSyn_SetSwtMgmtTxDynIdxOfTxConfContextDyn(txConfContIdx, swtMgmtTxIdx);
        EthTSyn_SetEgrTsIntVltOfTxConfContextDyn(txConfContIdx, egrTsAsIntVltMgmt.IntVlt);
        EthTSyn_SetEgrTsIntVltValidOfTxConfContextDyn(txConfContIdx, egrTsAsIntVltMgmt.IntVltValid);
        EthTSyn_SetBufferIdxOfTxConfContextDyn(txConfContIdx, ETHTSYN_INVALID_TX_BUFFER_IDX);
      }
      else
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
      {
        /* #2021 If no SwtMgmtTxMgmt object is available or switch management is not supported, dispatch timestamp(s)
         *       to transmitting unit. */
        EthTSyn_TsInfoType txTsInfo = {0};
        txTsInfo.IntVltMgmt = egrTsAsIntVltMgmt;
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
        txTsInfo.SwtIngrTsMgmt.TsValid = FALSE;
        txTsInfo.SwtIngrTsMgmt.Ts.secondsHi = 0u;
        txTsInfo.SwtIngrTsMgmt.Ts.seconds = 0u;
        txTsInfo.SwtIngrTsMgmt.Ts.nanoseconds = 0u;
        txTsInfo.SwtEgrTsMgmt.TsValid = FALSE;
        txTsInfo.SwtEgrTsMgmt.Ts.secondsHi = 0u;
        txTsInfo.SwtEgrTsMgmt.Ts.seconds = 0u;
        txTsInfo.SwtEgrTsMgmt.Ts.nanoseconds = 0u;
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

        EthTSyn_TxConf_DispatchEgrTs(txConfContIdx, &txTsInfo);

        /* #2022 Deactivate Tx confirmation context. */
        EthTSyn_TxConf_DeactivateContext(txConfContIdx);
      }
    }
  }
#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF)
  else
  {
    /* #203 No active Tx confirmation context found, leave critical section (if SW-Timestamping is used). */
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();
  }
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) */
} /* EthTSyn_TxConf_IntTxConfirmation */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_GetEgrTsAsIntVlt
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_IntVltMgmtType, ETHTSYN_CODE) EthTSyn_TxConf_GetEgrTsAsIntVlt(
  uint8                         CtrlIdx,
  uint8                         BufIdx,
  EthTSyn_SizeOfComCtrlPortType ComCtrlPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_IntVltMgmtType egrTsAsIntVltMgmt = {0u /* IntVal */, FALSE /* IntVltValid */};

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  /* #10 Check if HW-Timestamping is used. */
  {
    Eth_TimeStampType ethEgrTs = {0u, 0u, 0u};
    Eth_TimeStampQualType ethEgrTsQual = ETH_INVALID;

    /* #11 Try to get the Ethernet/HW egress timestamp via EthIf_GetEgressTimestamp() and check if it is valid. */
    if (EthIf_GetEgressTimestamp(CtrlIdx, BufIdx, &ethEgrTs, &ethEgrTsQual) == E_OK)
    {
      if (ethEgrTsQual == ETH_VALID)
      {
        /* #12 Try to convert the egress timestamp into an internal virtual local time. */
        EthTSyn_IntVltType egrIntVlt = 0u;
        if (EthTSyn_Ts_TsToIntVltChecked(&ethEgrTs, &egrIntVlt) == E_OK)
        {
          egrTsAsIntVltMgmt.IntVlt = egrIntVlt;
          egrTsAsIntVltMgmt.IntVltValid = TRUE;
        }
      }
    }

    ETHTSYN_DUMMY_STATEMENT(ComCtrlPortIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  }
#  else
  /* #20 Otherwise, SW-Timestamping is used. */
  {
    StbM_VirtualLocalTimeType stbmVlt = {0u, 0u};
    StbM_SynchronizedTimeBaseType tbId = EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(
      EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(ComCtrlPortIdx));

    /* #21 Get the current virtual local time from the StbM via StbM_GetCurrentVirtualLocalTime() and convert
     *     it into internal representation in case the time was retrieved successfully. */
    if (StbM_GetCurrentVirtualLocalTime(tbId, &stbmVlt) == E_OK)
    {
      egrTsAsIntVltMgmt.IntVlt = EthTSyn_Ts_VltToIntVlt(&stbmVlt);
      egrTsAsIntVltMgmt.IntVltValid = TRUE;
    }

    ETHTSYN_DUMMY_STATEMENT(BufIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    ETHTSYN_DUMMY_STATEMENT(CtrlIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  }
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  return egrTsAsIntVltMgmt;
} /* EthTSyn_TxConf_GetEgrTsAsIntVlt */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_DispatchEgrTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_DispatchEgrTs(
                EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
ETHTSYN_P2CONST(EthTSyn_TsInfoType)             TxTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check to which unit the timestamp(s) must be forwarded. */
  switch (EthTSyn_GetTxSmOfTxConfContext(TxConfContextIdx))
  {
#  if (ETHTSYN_MASTER_SUPPORT == STD_ON)
  case ETHTSYN_PORTSYNCSENDSM_TXSMOFTXCONFCONTEXT:
    {
      /* #20 Forward timestamp(s) to PortSyncSm if master support is enabled. */
      EthTSyn_PortSyncSendSm_ProvideSyncEgrTs(EthTSyn_GetTxSmIdxOfTxConfContext(TxConfContextIdx), TxTsInfoPtr);
      break;
    }
#  endif /* ETHTSYN_MASTER_SUPPORT == STD_ON */
#  if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
  case ETHTSYN_PDREQSM_TXSMOFTXCONFCONTEXT:
    {
      /* #30 Forward timestamp(s) to PdReqTrcv unit if pdelay initiator support is enabled. */
      EthTSyn_PdReqTrcv_ProvidePdReqEgrTs(EthTSyn_GetTxSmIdxOfTxConfContext(TxConfContextIdx), TxTsInfoPtr);
      break;
    }
#  endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */
#  if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON)
  case ETHTSYN_PDRESPSM_TXSMOFTXCONFCONTEXT:
    {
      /* #40 Forward timestamp(s) to PdRespSm unit if pdelay responder support is enabled. */
      EthTSyn_PdRespSm_ProvidePdRespEgrTs(EthTSyn_GetTxSmIdxOfTxConfContext(TxConfContextIdx), TxTsInfoPtr);
      break;
    }
#  endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */
  default: /* COV_ETHTSYN_EMPTY_BRANCH */
    {
      /* Invalid indirection type: should never be hit */
      break;
    }
  }
} /* EthTSyn_TxConf_DispatchEgrTs */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_IntReleaseTxContext
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_IntReleaseTxContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to ensure data consistency. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  /* #20 If switch management is supported and a switch management object is attached to the Tx confirmation context,
   *     release switch management object. */
  {
    const EthTSyn_SwtMgmtTx_IdxType swtMgmtTxIdx = EthTSyn_GetSwtMgmtTxDynIdxOfTxConfContextDyn(TxConfContextIdx);
    if (swtMgmtTxIdx < EthTSyn_GetSizeOfSwtMgmtTxDyn())
    {
      EthTSyn_SwtMgmtTx_ReleasePendingMgmtObj(swtMgmtTxIdx);
    }
  }
#  endif /* ETHTSYN_SWT_MGMT_SUPPORT == STD_ON */

  /* #30 Deactivate Tx confirmation context. */
  EthTSyn_TxConf_DeactivateContext(TxConfContextIdx);

  /* #40 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
} /* EthTSyn_TxConf_IntReleaseTxContext */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_FindContext
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SizeOfTxConfContextType, ETHTSYN_CODE) EthTSyn_TxConf_FindContext(
  uint8 CtrlIdx,
  uint8 BufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SizeOfTxConfContextType txConfContIdx = EthTSyn_GetSizeOfTxConfContext();
  EthTSyn_TxConfContextDynIterType txConfContIter = EthTSyn_GetSizeOfTxConfContext();
  EthTSyn_TxConfContextDynIterType loopGuard = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to ensure data consistency. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  /* #20 Find Tx confirmation context with matching buffer index in the list of active contexts. */
  for (txConfContIter = EthTSyn_GetTxConfContextDynActiveFirstIdx();
       txConfContIter < EthTSyn_GetSizeOfTxConfContext();
       txConfContIter = EthTSyn_GetTxConfContextNextActiveIdxOfTxConfContextDyn(txConfContIter))
  {
    if ((EthTSyn_ComCtrl_GetEthIfCtrlIdx(EthTSyn_GetComCtrlPortIdxOfTxConfContext(txConfContIter)) == CtrlIdx) &&
        (EthTSyn_GetBufferIdxOfTxConfContextDyn(txConfContIter) == BufIdx))
    {
      txConfContIdx = (EthTSyn_SizeOfTxConfContextType)txConfContIter;
    }

    loopGuard++;
    if ((txConfContIdx < EthTSyn_GetSizeOfTxConfContext()) || (loopGuard >= EthTSyn_GetSizeOfTxConfContext())) /* COV_ETHTSYN_LOOP_GUARD */
    {
      break;
    }
  }

  /* #30 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  return txConfContIdx;
} /* EthTSyn_TxConf_FindContext */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ActivateContext
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ActivateContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Release the Tx confirmation context in case of re-activation. */
  if (EthTSyn_IsActiveOfTxConfContextDyn(TxConfContextIdx))
  {
    EthTSyn_TxConf_IntReleaseTxContext(TxConfContextIdx);
  }

  {
    /* #20 Add Tx confirmation context at the end of the list of active contexts and activate it. */
    const EthTSyn_SizeOfTxConfContextType lastIdx = EthTSyn_GetTxConfContextDynActiveLastIdx();

    if (lastIdx < EthTSyn_GetSizeOfTxConfContext())
    {
      /* List is not empty. */
      EthTSyn_SetTxConfContextDynActiveLastIdx(TxConfContextIdx);
      /* Link new context to previous context. */
      EthTSyn_SetTxConfContextNextActiveIdxOfTxConfContextDyn(lastIdx, TxConfContextIdx);
      EthTSyn_SetTxConfContextPrevActiveIdxOfTxConfContextDyn(TxConfContextIdx, lastIdx);
    }
    else
    {
      /* List is empty. */
      EthTSyn_SetTxConfContextDynActiveFirstIdx(TxConfContextIdx);
      EthTSyn_SetTxConfContextDynActiveLastIdx(TxConfContextIdx);
      EthTSyn_SetTxConfContextPrevActiveIdxOfTxConfContextDyn(TxConfContextIdx, EthTSyn_GetSizeOfTxConfContext());
    }

    /* Next index is always invalid for last context in the list. */
    EthTSyn_SetTxConfContextNextActiveIdxOfTxConfContextDyn(TxConfContextIdx, EthTSyn_GetSizeOfTxConfContext());

    EthTSyn_SetActiveOfTxConfContextDyn(TxConfContextIdx, TRUE);
  }
} /* EthTSyn_TxConf_ActivateContext */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_DeactivateContext
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_DeactivateContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SizeOfTxConfContextType previousContext = EthTSyn_GetSizeOfTxConfContext();
  EthTSyn_SizeOfTxConfContextType nextContext = EthTSyn_GetSizeOfTxConfContext();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to ensure data consistency. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
  previousContext = EthTSyn_GetTxConfContextPrevActiveIdxOfTxConfContextDyn(TxConfContextIdx);
  nextContext = EthTSyn_GetTxConfContextNextActiveIdxOfTxConfContextDyn(TxConfContextIdx);

  /* #20 Check that Tx confirmation context is active. */
  if (EthTSyn_IsActiveOfTxConfContextDyn(TxConfContextIdx) == TRUE)
  {
    /* #30 Remove Tx confirmation context from the list of active contexts. */
    if (previousContext < EthTSyn_GetSizeOfTxConfContext())
    {
      /* Link the next to the previous context in the list (next index can be invalid). */
      EthTSyn_SetTxConfContextNextActiveIdxOfTxConfContextDyn(previousContext, nextContext);
    }
    else
    {
      /* Remove from start of the list (next index can be invalid). */
      EthTSyn_SetTxConfContextDynActiveFirstIdx(nextContext);
    }

    if (nextContext < EthTSyn_GetSizeOfTxConfContext())
    {
      /* Link the previous to the next context in the list (previous index can be invalid). */
      EthTSyn_SetTxConfContextPrevActiveIdxOfTxConfContextDyn(nextContext, previousContext);
    }
    else
    {
      /* Remove from end of the list (previous index can be invalid). */
      EthTSyn_SetTxConfContextDynActiveLastIdx(previousContext);
    }

    /* #40 Invalidate fields of removed context. */
    EthTSyn_SetBufferIdxOfTxConfContextDyn(TxConfContextIdx, ETHTSYN_INVALID_TX_BUFFER_IDX);
    EthTSyn_SetActiveOfTxConfContextDyn(TxConfContextIdx, FALSE);
    EthTSyn_SetTxConfContextNextActiveIdxOfTxConfContextDyn(TxConfContextIdx, EthTSyn_GetSizeOfTxConfContextDyn());
    EthTSyn_SetTxConfContextPrevActiveIdxOfTxConfContextDyn(TxConfContextIdx, EthTSyn_GetSizeOfTxConfContextDyn());

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    EthTSyn_SetEgrTsIntVltOfTxConfContextDyn(TxConfContextIdx, 0u);
    EthTSyn_SetEgrTsIntVltValidOfTxConfContextDyn(TxConfContextIdx, FALSE);
    EthTSyn_SetSwtMgmtTxDynIdxOfTxConfContextDyn(TxConfContextIdx, EthTSyn_GetSizeOfSwtMgmtTxDyn());
#  endif /* ETHTSYN_SWT_MGMT_SUPPORT == STD_ON */
  }

  /* #50 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
} /* EthTSyn_TxConf_DeactivateContext */

# endif /* ETHTSYN_NOUNIT_TXCONF */

/**********************************************************************************************************************
 * Internal functions of SwtMgmtTx
 *!
 * \unit SwtMgmtTx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SWTMGMTTX)
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
#   if (ETHTSYN_SWT_MGMT_ONE_TAS == STD_OFF)
/**********************************************************************************************************************
 * EthTSyn_SwtMgmtTx_CalcSyncSwtResTime
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_CalcSyncSwtResTime(
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)    SyncSwtIngrTsMgmtPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)    SyncSwtEgrTsMgmtPtr,
    ETHTSYN_P2VAR(EthTSyn_UTimediffType) SwtResTimePtr)
{
    /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_UTimediffType swtResTimeLoc = 0u;

  /* ----- Implementation ----------------------------------------------- */
#    if ( ETHTSYN_SWT_UPLINK_TO_TX_RESIDENCE_TIME_NS > 0 )
  {
    swtResTimeLoc = (EthTSyn_UTimediffType)ETHTSYN_SWT_UPLINK_TO_TX_RESIDENCE_TIME_NS;
    retVal = E_OK;
    ETHTSYN_DUMMY_STATEMENT(SyncSwtIngrTsMgmtPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    ETHTSYN_DUMMY_STATEMENT(SyncSwtEgrTsMgmtPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  }
#    else
  {
    if ( (SyncSwtIngrTsMgmtPtr->TsValid == TRUE) && (SyncSwtEgrTsMgmtPtr->TsValid == TRUE) )
    {
      EthTSyn_TimediffType syncSwtResTimeCalc = 0;

      if (EthTSyn_Ts_TsMinusTs(&SyncSwtEgrTsMgmtPtr->Ts, &SyncSwtIngrTsMgmtPtr->Ts, &syncSwtResTimeCalc)  == E_OK)
      {
        if (syncSwtResTimeCalc >= 0)
        {
          swtResTimeLoc = (EthTSyn_UTimediffType)syncSwtResTimeCalc;
          retVal = E_OK;
        }
      }
    }
  }
#    endif /* ETHTSYN_SWT_UPLINK_TO_TX_RESIDENCE_TIME_NS > 0 */

  /* Set out-parameter */
  (*SwtResTimePtr) = swtResTimeLoc;

  return retVal;
} /* EthTSyn_SwtMgmtTx_CalcSyncSwtResTime() */
#   endif /* (ETHTSYN_SWT_MGMT_ONE_TAS == STD_OFF) */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
# endif /* ETHTSYN_NOUNIT_SWTMGMTTX */
#endif /* ETHTSYN_TX_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Public interfaces of Tx
 *!
 * \unit Tx
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_TX)
# if (ETHTSYN_TX_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  EthTSyn_Tx_StartTxSequence
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Tx_StartTxSequence(
                  EthTSyn_ComCtrl_PortIdxType     ComCtrlPortIdx,
                  EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)     TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType)     PortInfoHeaderPtr,
    ETHTSYN_P2VAR(EthTSyn_EthTxBufferStructType)  TxBufMgmtPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TxBufMgmtPtr->Length = 0u;
  TxBufMgmtPtr->BufferIdx = 0u;
  TxBufMgmtPtr->BufferPtr = NULL_PTR;

  /* #10 Check if transmission is allowed. */
  if (EthTSyn_ComCtrl_IsTxAllowed(ComCtrlPortIdx) == TRUE)
  {
    EthTSyn_EthTxBufferStructType locTxBufMgmt = {0};
    const uint8 ethIfCtrlIdx = EthTSyn_ComCtrl_GetEthIfCtrlIdx(ComCtrlPortIdx);
    const EthTSyn_FramePrioOfComCtrlPortType framePrio = EthTSyn_GetFramePrioOfComCtrlPort(ComCtrlPortIdx);

    /* #20 Try to allocate an Ethernet TxBuffer via EthIf_ProvideTxBuffer. */
    locTxBufMgmt.BufferPtr = NULL_PTR;
    locTxBufMgmt.Length = TypeSpecHeaderPtr->MessageLength;
    if (EthIf_ProvideTxBuffer(ethIfCtrlIdx, ETHTSYN_FRAME_TYPE, framePrio, &locTxBufMgmt.BufferIdx,
                              (ETHTSYN_P2VAR(ETHTSYN_P2VAR(Eth_DataType)))&locTxBufMgmt.BufferPtr, /* PRQA S 0310 */ /* MD_EthTSyn_PtrCast_EthBuffer */
                              &locTxBufMgmt.Length) == BUFREQ_OK)
    {
      /*@ assert locTxBufMgmt.BufferPtr != NULL_PTR; */
      /* #30 Try to prepare transmission (if HW-timestamping or switch management is enabled). */
#  if ((ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON))
      const boolean isEgrTsRequired = (boolean)(TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext());
      if (EthTSyn_Tx_PrepareTx(ComCtrlPortIdx, locTxBufMgmt.BufferIdx, isEgrTsRequired) == E_OK)
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) && (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */
      {
        /* #40 Serialize the PTP message header and copy it to the Ethernet Tx buffer. */
        EthTSyn_Tx_SerializeHeader(ComCtrlPortIdx, TypeSpecHeaderPtr, PortInfoHeaderPtr, locTxBufMgmt.BufferPtr);

        /* #50 Set up TxConf context for retrieved Tx buffer. */
        EthTSyn_TxConf_SetupTxContext(TxConfContextIdx, ethIfCtrlIdx, locTxBufMgmt.BufferIdx);

        /* #60 Set Tx buffer information and return E_OK. */
        TxBufMgmtPtr->BufferIdx = locTxBufMgmt.BufferIdx;
        TxBufMgmtPtr->BufferPtr = locTxBufMgmt.BufferPtr;
        TxBufMgmtPtr->Length = locTxBufMgmt.Length;

        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_Tx_StartTxSequence() */

/**********************************************************************************************************************
 *  EthTSyn_Tx_FinishTxSequence
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Tx_FinishTxSequence(
  EthTSyn_ComCtrl_PortIdxType     ComCtrlPortIdx,
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
  uint8                           EthTxBufIdx,
  uint16                          TxLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const uint8 ethIfCtrlIdx = EthTSyn_ComCtrl_GetEthIfCtrlIdx(ComCtrlPortIdx);
  const boolean isEgrTsRequired = (boolean)(TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext());
  const EthTSyn_ComCtrlTimeDomainIdxOfComCtrlPortType tdIdx =
    EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(ComCtrlPortIdx);
  const EthTSyn_DestinationMacIdxOfComCtrlTimeDomainType destAddrIdx =
    EthTSyn_GetDestinationMacIdxOfComCtrlTimeDomain(tdIdx);
  EthTSyn_PhysAddrType destAddr = {0u};

  /* ----- Implementation ----------------------------------------------- */
  destAddr[0u] = EthTSyn_GetByte0OfDestinationMac(destAddrIdx);
  destAddr[1u] = EthTSyn_GetByte1OfDestinationMac(destAddrIdx);
  destAddr[2u] = EthTSyn_GetByte2OfDestinationMac(destAddrIdx);
  destAddr[3u] = EthTSyn_GetByte3OfDestinationMac(destAddrIdx);
  destAddr[4u] = EthTSyn_GetByte4OfDestinationMac(destAddrIdx);
  destAddr[5u] = EthTSyn_GetByte5OfDestinationMac(destAddrIdx);

  /* #10 Trigger transmission by calling EthIf_Transmit and check if transmission succeeds. */
  if (EthIf_Transmit(ethIfCtrlIdx, EthTxBufIdx, ETHTSYN_FRAME_TYPE, isEgrTsRequired, TxLenByte, destAddr) == E_OK)
  {
    /* #100 Restart Debounce timer. */
    EthTSyn_ComCtrl_RestartDebounceTimer(ComCtrlPortIdx);
    retVal = E_OK;
  }
  /* #20 Otherwise. */
  else
  {
    /* #200 Release Tx buffer by calling EthIf_Transmit with LenByte of 0. */
    (void)EthIf_Transmit(ethIfCtrlIdx, EthTxBufIdx, ETHTSYN_FRAME_TYPE, FALSE, 0u, destAddr);

    /* #201 Release TxConf context. */
    EthTSyn_TxConf_ReleaseTxContext(TxConfContextIdx);
  }

  return retVal;
} /* EthTSyn_Tx_FinishTxSequence() */

# endif /* ETHTSYN_TX_SUPPORT == STD_ON */
#endif /* ETHTSYN_NOUNIT_TX */

/**********************************************************************************************************************
 * Public interfaces of TxConf
 *!
 * \unit TxConf
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_TXCONF)
# if (ETHTSYN_TX_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_TxConf_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TxConfContextDynIterType txConfContIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize linked list of active Tx confirmation contexts. */
  EthTSyn_SetTxConfContextDynActiveFirstIdx(EthTSyn_GetSizeOfTxConfContextDyn());
  EthTSyn_SetTxConfContextDynActiveLastIdx(EthTSyn_GetSizeOfTxConfContextDyn());

  /* #20 Iterate over all Tx confirmation contexts and initialize variables. */
  for (txConfContIter = 0; txConfContIter < EthTSyn_GetSizeOfTxConfContextDyn(); txConfContIter++)
  {
    EthTSyn_SetBufferIdxOfTxConfContextDyn(txConfContIter, ETHTSYN_INVALID_TX_BUFFER_IDX);
    EthTSyn_SetActiveOfTxConfContextDyn(txConfContIter, FALSE);
    EthTSyn_SetTxConfContextNextActiveIdxOfTxConfContextDyn(txConfContIter, EthTSyn_GetSizeOfTxConfContextDyn());
    EthTSyn_SetTxConfContextPrevActiveIdxOfTxConfContextDyn(txConfContIter, EthTSyn_GetSizeOfTxConfContextDyn());

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    EthTSyn_SetEgrTsIntVltOfTxConfContextDyn(txConfContIter, 0u);
    EthTSyn_SetEgrTsIntVltValidOfTxConfContextDyn(txConfContIter, FALSE);
    EthTSyn_SetSwtMgmtTxDynIdxOfTxConfContextDyn(txConfContIter, EthTSyn_GetSizeOfSwtMgmtTxDyn());
#  endif /* ETHTSYN_SWT_MGMT_SUPPORT == STD_ON */
  }

} /* EthTSyn_TxConf_Init() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_SetupTxContext
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
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_SetupTxContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
  uint8                           CtrlIdx,
  uint8                           BufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SizeOfTxConfContextType bufTxConfContIdx = EthTSyn_GetSizeOfTxConfContext();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Tx confirmation context is valid. */
  if (TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext())
  {
    /* #20 Enter critical section to ensure data consistency. */
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

    /* #30 If buffer is used by any active Tx confirmation context, release that context. */
    bufTxConfContIdx = EthTSyn_TxConf_FindContext(CtrlIdx, BufIdx);
    if (bufTxConfContIdx < EthTSyn_GetSizeOfTxConfContext())
    {
      EthTSyn_TxConf_IntReleaseTxContext(bufTxConfContIdx);
    }

    /* #40 (Re-)activate Tx confirmation context. */
    EthTSyn_TxConf_ActivateContext(TxConfContextIdx);

    /* #50 Set buffer index of Tx confirmation context. */
    EthTSyn_SetBufferIdxOfTxConfContextDyn(TxConfContextIdx, BufIdx);

    /* #60 Leave critical section. */
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
  }
} /* EthTSyn_TxConf_SetupTxContext() */

/**********************************************************************************************************************
 *  EthTSyn_TxConf_ReleaseTxContext
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ReleaseTxContext(
  EthTSyn_SizeOfTxConfContextType TxConfContextIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Tx confirmation context is valid. */
  if (TxConfContextIdx < EthTSyn_GetSizeOfTxConfContext())
  {
    /* #20 Release Tx confirmation context. */
    EthTSyn_TxConf_IntReleaseTxContext(TxConfContextIdx);
  }
} /* EthTSyn_TxConf_ReleaseTxContext() */

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_TxConf_ProvideSwtTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConf_ProvideSwtTs(
                  EthTSyn_SizeOfTxConfContextType TxConfContextIdx,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)             SwtIngrTsPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)             SwtEgrTsPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process provided timestamps if Tx confirmation context is active. */
  if (EthTSyn_IsActiveOfTxConfContextDyn(TxConfContextIdx) == TRUE)
  {
    /* #20 Dispatch timestamps to transmitting unit. */
    EthTSyn_TsInfoType txTsInfo = {0};
    txTsInfo.IntVltMgmt.IntVlt = EthTSyn_GetEgrTsIntVltOfTxConfContextDyn(TxConfContextIdx);
    txTsInfo.IntVltMgmt.IntVltValid = EthTSyn_IsEgrTsIntVltValidOfTxConfContextDyn(TxConfContextIdx);
    txTsInfo.SwtIngrTsMgmt = *SwtIngrTsPtr;
    txTsInfo.SwtEgrTsMgmt = *SwtEgrTsPtr;

    EthTSyn_TxConf_DispatchEgrTs(TxConfContextIdx, &txTsInfo);

    /* #30 Release Tx confirmation context. */
    EthTSyn_TxConf_DeactivateContext(TxConfContextIdx);
  }
} /* EthTSyn_TxConf_ProvideSwtTs() */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
# endif /* ETHTSYN_TX_SUPPORT == STD_ON */

/**********************************************************************************************************************
 * Service interfaces of TxConf
 *!
 * \unit TxConf
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthTSyn_TxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_TxConfirmation(
  uint8 CtrlIdx,
  uint8 BufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if (EthTSyn_Init_IsInitialized() == FALSE)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if (EthTSyn_ComCtrl_FindComCtrlEthIfCtrl(CtrlIdx) >= EthTSyn_GetSizeOfComCtrlEthIfCtrl())
  {
    errorId = ETHTSYN_E_CTRL_IDX;
  }
  else
# endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
  /* ----- Implementation ----------------------------------------------- */
  /* #20 Process Tx confirmation. */
# if (ETHTSYN_TX_SUPPORT == STD_ON)
    EthTSyn_TxConf_IntTxConfirmation(CtrlIdx, BufIdx);
# else
  ETHTSYN_DUMMY_STATEMENT(BufIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(CtrlIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* ETHTSYN_TX_SUPPORT == STD_ON */
  }

  /* ----- Development Error Report ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_TX_CONFIRMATION, errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
} /* EthTSyn_TxConfirmation() */

#endif /* ETHTSYN_NOUNIT_TXCONF */

/**********************************************************************************************************************
 * Public interfaces of SwtMgmtTx
 *!
 * \unit SwtMgmtTx
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_SWTMGMTTX)
# if (ETHTSYN_TX_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtTxDynIterType swtMgmtTxIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (swtMgmtTxIter = 0u; swtMgmtTxIter < EthTSyn_GetSizeOfSwtMgmtTxDyn(); swtMgmtTxIter++)
  {
    EthTSyn_SetComCtrlEthIfCtrlIdxOfSwtMgmtTxDyn(swtMgmtTxIter, EthTSyn_GetSizeOfComCtrlEthIfCtrl());
    EthTSyn_SetTxConfContextIdxOfSwtMgmtTxDyn(swtMgmtTxIter, EthTSyn_GetSizeOfTxConfContext());
    EthTSyn_SetStateOfSwtMgmtTxDyn(swtMgmtTxIter, ETHTSYN_UNUSED_STATEOFSWTMGMTTXDYN);
  }
} /* EthTSyn_SwtMgmtTx_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_ReleasePendingMgmtObjsOfCtrl
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_ReleasePendingMgmtObjsOfCtrl(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtTxDynIterType swtMgmtTxIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
  for (swtMgmtTxIter = 0u; swtMgmtTxIter < EthTSyn_GetSizeOfSwtMgmtTxDyn(); swtMgmtTxIter++)
  {
    if ((EthTSyn_GetStateOfSwtMgmtTxDyn(swtMgmtTxIter) == ETHTSYN_PENDING_STATEOFSWTMGMTTXDYN) &&
        (EthTSyn_GetComCtrlEthIfCtrlIdxOfSwtMgmtTxDyn(swtMgmtTxIter) == ComCtrlEthIfCtrlIdx))
    {
      EthTSyn_SetStateOfSwtMgmtTxDyn(swtMgmtTxIter, ETHTSYN_RELEASED_STATEOFSWTMGMTTXDYN);
    }
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
} /* EthTSyn_SwtMgmtTx_ReleasePendingMgmtObjsOfCtrl() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_PrepTx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_PrepTx(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  uint8                       BufIdx,
  boolean                     IsEgrTsRequired)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const EthTSyn_ComCtrlEthIfCtrlIdxOfComCtrlPortType comCtrlEthIfCtrlIdx =
    EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(ComCtrlPortIdx);
  const EthTSyn_EthIfCtrlIdxOfComCtrlEthIfCtrlType ethIfCtrlIdx =
    EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(comCtrlEthIfCtrlIdx);
  EthSwt_MgmtInfoType swtMgmtInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  swtMgmtInfo.SwitchIdx = EthTSyn_GetSwitchIdxOfComCtrlPort(ComCtrlPortIdx);
  swtMgmtInfo.PortIdx = EthTSyn_GetSwitchPortIdxOfComCtrlPort(ComCtrlPortIdx);

  if (EthIf_SetSwitchMgmtInfo(ethIfCtrlIdx, BufIdx, &swtMgmtInfo) == E_OK)                                             /* VCA_ETHTSYN_CALL_EXTERNAL_FUNCTION_VAR_POINTER_ETHIF_SWT_MGMT_INFO */
  {
    if (IsEgrTsRequired == TRUE)
    {
      retVal = EthIf_SwitchEnableTimeStamping(ethIfCtrlIdx, BufIdx, &swtMgmtInfo);                                     /* VCA_ETHTSYN_CALL_EXTERNAL_FUNCTION_VAR_POINTER_ETHIF_SWT_MGMT_INFO */
    }
    else
    {
      retVal = E_OK;
    }
  }

  return retVal;
} /* EthTSyn_SwtMgmtTx_PrepTx() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_CalcPotAndCorrField
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
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_CalcPotAndCorrField(
                  EthTSyn_ComCtrl_PortIdxType  ComCtrlPortIdx,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)          EgrTsInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)      SyncTxInfoPtr,
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) PotPtr,
    ETHTSYN_P2VAR(uint64)                      CorrFieldNs)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint64 syncSwtResTimeNs = 0u;
  EthTSyn_IntVltType syncEgrTsIntVlt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize out-parameters. */
  PotPtr->secondsHi   = 0u;
  PotPtr->seconds     = 0u;
  PotPtr->nanoseconds = 0u;

  (*CorrFieldNs) = 0u;

#   if (ETHTSYN_SWT_MGMT_ONE_TAS == STD_ON)
  /* #10 If switch and host are considered as one time-aware system:
   *   Check if switch egress timestamp is valid.
   *     Try to convert the switch egress timestamp into a virtual local time in internal representation for later
   *     calculation of the Pot. */
  if (EgrTsInfoPtr->SwtEgrTsMgmt.TsValid == TRUE)
  {
    retVal = EthTSyn_Ts_TsToIntVltChecked(&EgrTsInfoPtr->SwtEgrTsMgmt.Ts, &syncEgrTsIntVlt);
  }
#   else
  /* #20 If switch and host are considered as different time-aware systems:
   *   Check if host egress timestamp is valid.
   *     Use the host egress timestamp for later calculation of the Pot.
   *     Calculate the switch residence time. */
  if (EgrTsInfoPtr->IntVltMgmt.IntVltValid == TRUE)
  {
    syncEgrTsIntVlt = EgrTsInfoPtr->IntVltMgmt.IntVlt;
    retVal = EthTSyn_SwtMgmtTx_CalcSyncSwtResTime(&EgrTsInfoPtr->SwtIngrTsMgmt, &EgrTsInfoPtr->SwtEgrTsMgmt,
      &syncSwtResTimeNs);
  }
#   endif /* (ETHTSYN_SWT_MGMT_ONE_TAS == STD_ON) */

  /* #30 Check if former calculations succeeded.
   *   Trigger calculation of the Pot via EthTSyn_SyncSendTx_CalcPot() using
   *   the former determined Sync egress timestamp.
   *   Add the switch residence time to the POT in case of a boundary clock.
   *   Otherwise, return the switch residence time as correction field value. */
  if (retVal == E_OK)
  {
    retVal = EthTSyn_SyncSendTx_CalcPot(ComCtrlPortIdx, syncEgrTsIntVlt, SyncTxInfoPtr, PotPtr);
#   if (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_BOUNDARY)
    retVal |= EthTSyn_Ts_TsPlusUTimediff(PotPtr, syncSwtResTimeNs);
#   else
    (*CorrFieldNs) = syncSwtResTimeNs;
#   endif /* (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_BOUNDARY) */
  }

  return retVal;
} /* EthTSyn_SwtMgmtTx_CalcPotAndCorrField() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_ReleasePendingMgmtObj
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_ReleasePendingMgmtObj(
  EthTSyn_SwtMgmtTx_IdxType SwtMgmtTxIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  EthTSyn_SetStateOfSwtMgmtTxDyn(SwtMgmtTxIdx, ETHTSYN_RELEASED_STATEOFSWTMGMTTXDYN);
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_SwtMgmtTx_ReleasePendingMgmtObj() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_HndlTxConf
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
FUNC(EthTSyn_SwtMgmtTx_IdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_HndlTxConf(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx,
  uint8                            BufIdx,
  EthTSyn_SizeOfTxConfContextType  TxConfContextIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtTx_IdxType swtMgmtTxIdx = EthTSyn_GetSizeOfSwtMgmtTxDyn();
  const EthTSyn_SwtMgmtQTxInstIdxOfSwtMgmtQIndType swtMgmtQTxIdx = EthTSyn_GetSwtMgmtQTxInstIdxOfSwtMgmtQInd(0u);

  /* ----- Implementation ----------------------------------------------- */
  swtMgmtTxIdx = EthTSyn_SwtMgmtQ_PreparePush(swtMgmtQTxIdx, ETHTSYN_SWTMGMTTXDYN_SWTMGMTOBJUSEROFSWTMGMTQBUF,
    EthTSyn_GetSizeOfSwtMgmtTxDyn());
  if (swtMgmtTxIdx < EthTSyn_GetSizeOfSwtMgmtTxDyn())
  {
    EthTSyn_SwtMgmt_EthSwtMgmtObjPtrType txMgmtObjPtr = NULL_PTR;

    if (EthIf_GetTxMgmtObject(EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx), BufIdx,
          &txMgmtObjPtr) == E_OK)
    {
      EthTSyn_SwtMgmtQ_Push(swtMgmtQTxIdx, txMgmtObjPtr);

      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
      /* Critical section required to ensure that no release of the SwtMgmtTx elements is triggered during
       * modification. */

      EthTSyn_SetTxConfContextIdxOfSwtMgmtTxDyn(swtMgmtTxIdx, TxConfContextIdx);
      EthTSyn_SetComCtrlEthIfCtrlIdxOfSwtMgmtTxDyn(swtMgmtTxIdx, ComCtrlEthIfCtrlIdx);

      EthTSyn_SetStateOfSwtMgmtTxDyn(swtMgmtTxIdx, ETHTSYN_PENDING_STATEOFSWTMGMTTXDYN);

      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
    }
    else
    {
      EthTSyn_SwtMgmtQ_CancelPush(swtMgmtQTxIdx);
      swtMgmtTxIdx = EthTSyn_GetSizeOfSwtMgmtTxDyn();
    }
  }

  return swtMgmtTxIdx;
} /* EthTSyn_SwtMgmtTx_HndlTxConf() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtTx_ProcTs
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtTx_ProcTs(
                  EthTSyn_SwtMgmtTx_IdxType SwtMgmtTxIdx,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)       SwtIngrTsMgmtPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)       SwtEgrTsMgmtPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  if (EthTSyn_GetStateOfSwtMgmtTxDyn(SwtMgmtTxIdx) == ETHTSYN_PENDING_STATEOFSWTMGMTTXDYN)
  {
    const EthTSyn_TxConfContextIdxOfSwtMgmtTxDynType txConfContextIdx =
      EthTSyn_GetTxConfContextIdxOfSwtMgmtTxDyn(SwtMgmtTxIdx);

    EthTSyn_SetStateOfSwtMgmtTxDyn(SwtMgmtTxIdx, ETHTSYN_UNUSED_STATEOFSWTMGMTTXDYN);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

    /*@ assert txConfContextIdx < EthTSyn_GetSizeOfTxConfContext(); */                                                 /* VCA_ETHTSYN_SWT_MGMT_TX_TX_CONF_IDX */
    EthTSyn_TxConf_ProvideSwtTs(txConfContextIdx, SwtIngrTsMgmtPtr, SwtEgrTsMgmtPtr);
  }
  else
  {
    EthTSyn_SetStateOfSwtMgmtTxDyn(SwtMgmtTxIdx, ETHTSYN_UNUSED_STATEOFSWTMGMTTXDYN);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }
} /* EthTSyn_SwtMgmtTx_ProcTs() */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
# endif /* (ETHTSYN_TX_SUPPORT == STD_ON) */
#endif /* ETHTSYN_NOUNIT_SWTMGMTTX */

#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Transmission.c
 *********************************************************************************************************************/
