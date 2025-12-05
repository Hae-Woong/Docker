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
/*!        \file  EthTSyn_Reception.c
 *        \brief  EthTSyn Reception source file
 *      \details  Implementation of all units included in the logical unit Reception (Rx and SwtMgmtRx).
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

#define ETHTSYN_RX_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn.h"
#include "EthTSyn_Rx_Cbk.h"
#include "EthTSyn_Rx_Int.h"
#include "EthTSyn_Init_Int.h"
#include "EthTSyn_GeneratedCfgAccess.h"
#include "EthTSyn_ComCtrl_Int.h"
#include "EthTSyn_MsgDefs_Int.h"
#include "EthTSyn_Memory_Int.h"
#include "EthTSyn_Timestamp_Int.h"
#include "EthTSyn_SwtMgmtRx_Int.h"
#include "EthTSyn_SwtMgmtQ_Int.h"

#include "EthTSyn_SlaveRx_Int.h"
#include "EthTSyn_PdReqTrcv_Int.h"
#include "EthTSyn_PdRespSm_Int.h"

#include "StbM.h"
#include "EthIf.h"
#include "vstdlib.h"
#if ( (ETHTSYN_DEV_ERROR_REPORT == STD_ON) || (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) )
# include "Det.h"
#endif /* ( (ETHTSYN_DEV_ERROR_REPORT == STD_ON) || (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) ) */

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
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 * Internal function prototypes of Rx
 *!
 * \unit Rx
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_RX)
/**********************************************************************************************************************
 *  EthTSyn_Rx_IntRxIndication
 *********************************************************************************************************************/
/*! \brief        Notification about a received EthTSyn frames.
 *  \details      Internal implementation of the EthTSyn_RxIndication.
 *  \param[in]    CtrlIdx      EthIf controller index
 *  \param[in]    DataPtr      Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \param[in]    LenByte      Length of received data
 *  \return       ETHTSYN_E_NO_ERROR - No DET error detected
 *  \return       other values - DET error code in case an DET error was detected
 *  \pre          Must be called in context of the EthTSyn_RxIndication()
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_IntRxIndication(
                  uint8  CtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr,
                  uint16 LenByte);

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetIngrTimingInfo
 *********************************************************************************************************************/
/*! \brief        Gets the ingress timestamp of a received EthTSyn frame.
 *  \details      Also retrieves the current Ethernet and StbM times if message is a Sync and Slave support as well as
 *                hardware timestamping is enabled.
 *  \param[in]    CtrlIdx      EthIf controller index
 *  \param[in]    DataPtr      Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \return       Ingress timestamp and optional Ethernet and StbM times expressed as internal virtual local time with
 *                validity information
 *  \pre          Must be called in context of the EthTSyn_RxIndication()
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_IngrTimingInfoType, ETHTSYN_CODE) EthTSyn_Rx_GetIngrTimingInfo(
                  uint8  CtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr);

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetIngrTsAsIntVlt
 *********************************************************************************************************************/
/*! \brief        Gets the ingress timestamp of a received EthTSyn frame as internal virtual local time with validity
 *                information.
 *  \details      -
 *  \param[in]    CtrlIdx      EthIf controller index
 *  \param[in]    DataPtr      Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \return       The ingress timestamp expressed as internal virtual local time together with its validity information
 *  \pre          Must be called in context of the EthTSyn_RxIndication()
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_IntVltMgmtType, ETHTSYN_CODE) EthTSyn_Rx_GetIngrTsAsIntVlt(
                  uint8  CtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr);

# if ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  EthTSyn_Rx_GetSyncRxEthAndStbmTime
 *********************************************************************************************************************/
/*! \brief        Gets the current Ethernet and StbM times.
 *  \details      -
 *  \param[in]    CtrlIdx           EthIf controller index
 *  \param[in]    DataPtr           Pointer to payload of the received Ethernet frame
 *  \param[out]   SyncRxEthTimePtr  Current Ethernet time (only valid if E_OK is returned)
 *  \param[out]   SyncRxStbmTimeptr Current StbM virtual local time (only valid if E_OK is returned)
 *  \return       E_OK - Retrieving current Ethernet and StbM times was successful and timestamps are valid
 *  \return       E_NOT_OK - Retrieving current Ethernet and StbM times failed or timestamps are invalid
 *  \pre          Must be called in context of the EthTSyn_RxIndication()
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Rx_GetSyncRxEthAndStbmTime(
                  uint8               CtrlIdx,
  ETHTSYN_P2CONST(uint8)              DataPtr,
    ETHTSYN_P2VAR(EthTSyn_IntVltType) SyncRxEthTimePtr,
    ETHTSYN_P2VAR(EthTSyn_IntVltType) SyncRxStbmTimeptr);
# endif /* ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)) */

/**********************************************************************************************************************
 *  EthTSyn_Rx_CheckMsgGeneral
 *********************************************************************************************************************/
/*! \brief        Checks if the received EthTSyn frames is valid with respect to general aspects.
 *  \details      -
 *  \param[in]    DataPtr               Pointer to payload of the received Ethernet frame
 *                                      (i.e. Ethernet header is not provided)
 *  \param[in]    LenByte               Length of received data
 *  \return       ETHTSYN_E_NO_ERROR - No DET error detected
 *  \return       other values - DET error code in case an DET error was detected
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \note         The following aspects are checked here:
 *                - Length of the received message can hold the general PTP header and fits the length provided in the
 *                  message header
 *                - The message type is known
 *                - PTP protocol version is supported
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_CheckMsgGeneral(
  ETHTSYN_P2CONST(uint8) DataPtr,
                  uint16 LenByte);

/**********************************************************************************************************************
 *  EthTSyn_Rx_IsMsgTypeValid
 *********************************************************************************************************************/
/*! \brief        Checks if the message type of the received PTP message is valid.
 *  \details      -
 *  \param[in]    DataPtr   Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \return       TRUE - Message type is valid
 *  \return       FALSE - Message type is invalid
 *  \pre          Ethernet frame (DataPtr) is big enough to hold the PTP message header
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \note         Valid message types are Sync, FollowUp, Announce, Pdelay_Req, Pdelay_Resp and Pdelay_Resp_Fup.
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Rx_IsMsgTypeValid(
  ETHTSYN_P2CONST(uint8) DataPtr);

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetMsgTypeFromHdr
 *********************************************************************************************************************/
/*! \brief        Gets the PTP message type included in the message header.
 *  \details      -
 *  \param[in]    DataPtr   Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \return       The message type from the message header. This might be an invalid type!
 *  \pre          Ethernet frame (DataPtr) is big enough to hold the PTP message header
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_GetMsgTypeFromHdr(
  ETHTSYN_P2CONST(uint8) DataPtr);

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetMsgLenFromHdr
 *********************************************************************************************************************/
/*! \brief        Gets the PTP message length included in the message header.
 *  \details      -
 *  \param[in]    DataPtr   Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \return       The message length from the message header
 *  \pre          Ethernet frame (DataPtr) is big enough to hold the PTP message header
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_Rx_GetMsgLenFromHdr(
  ETHTSYN_P2CONST(uint8) DataPtr);

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetTdIdFromHdr
 *********************************************************************************************************************/
/*! \brief        Gets the time domain id included in the message header.
 *  \details      -
 *  \param[in]    DataPtr   Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \return       The time domain id from the message header
 *  \pre          Ethernet frame (DataPtr) is big enough to hold the PTP message header
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_GetTdIdFromHdr(
  ETHTSYN_P2CONST(uint8) DataPtr);

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetPtpProtocolVersionFromHdr
 *********************************************************************************************************************/
/*! \brief        Gets the PTP protocol version included in the message header.
 *  \details      -
 *  \param[in]    DataPtr   Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \return       The PTP protocol version from the message header
 *  \pre          Ethernet frame (DataPtr) is big enough to hold the PTP message header
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_GetPtpProtocolVersionFromHdr(
  ETHTSYN_P2CONST(uint8) DataPtr);

/**********************************************************************************************************************
 *  EthTSyn_Rx_IntProcRcvdMsg
 *********************************************************************************************************************/
/*! \brief        Triggers processing of a received EthTSyn message.
 *  \details      -
 *  \param[in]    ComCtrlPortIdx   Index of the ComCtrlPort
 *                                 [range: ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort()]
 *  \param[in]    DataPtr          Pointer to payload of the received Ethernet frame
 *                                 (i.e. Ethernet header is not provided)
 *  \param[in]    RxTsInfoPtr      Info about the reception timestamp(s) as well as optional Ethernet and StbM time
 *  \pre          Message type of the received message is valid, i.e. Sync, FollowUp, Announce, Pdelay_Req,
 *                Pdelay_Resp or Pdelay_Resp_Fup
 *  \pre          The RxBuffer is big enough to hold the complete message (length indicated in the message header)
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires ComCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort();
 *    requires DataPtr != NULL_PTR;
 *    requires RxTsInfoPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Rx_IntProcRcvdMsg(
                  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  ETHTSYN_P2CONST(uint8)                      DataPtr,
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)       RxTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_Rx_DeSerializeHdr
 *********************************************************************************************************************/
/*! \brief        De-serializes the PTP header of a received EthTSyn message.
 *  \details      -
 *  \param[in]    DataPtr   Pointer to payload of the received Ethernet frame (i.e. Ethernet header is not provided)
 *  \pre          The RxBuffer is big enough to hold the PTP message header
 *  \return       The de-serialized PTP message header
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_MsgHdrType, ETHTSYN_CODE) EthTSyn_Rx_DeSerializeHdr(
  ETHTSYN_P2CONST(uint8) DataPtr);
#endif /* ETHTSYN_NOUNIT_RX */

/**********************************************************************************************************************
 * Internal function prototypes of SwtMgmtRx
 *!
 * \unit SwtMgmtRx
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_SWTMGMTRX)
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_GetComCtrlPort
 *********************************************************************************************************************/
/*! \brief        Tries to find a ComCtrlPort that matches to the switch management information and PTP message header.
 *  \details      -
 *  \param[in]    SwtMgmtRxIdx       SwtMgmtRx instance index
 *                                   [range: SwtMgmtRxIdx < EthTSyn_GetSizeOfSwtMgmtRx()]
 *  \param[in]    DataPtr            Pointer to payload of the received Ethernet frame
 *                                   (i.e. Ethernet header is not provided)
 *  \param[in]    SwtMgmtInfoMgmtPtr Switch management information and validity information
 *  \pre          -
 *  \return       EthTSyn_GetSizeOfComCtrlPort() - no matching port was found
 *  \return       Other values - Index of the matching ComCtrlPort
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires SwtMgmtRxIdx < EthTSyn_GetSizeOfSwtMgmtRx();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_ComCtrl_PortIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_GetComCtrlPort(
                  EthTSyn_SwtMgmtRx_IdxType               SwtMgmtRxIdx,
  ETHTSYN_P2CONST(uint8)                                  DataPtr,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmt_MgmtInfoMgmtStructType) SwtMgmtInfoMgmtPtr);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
#endif /* ETHTSYN_NOUNIT_SWTMGMTRX */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Internal functions of Rx
 *!
 * \unit Rx
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_RX)
/**********************************************************************************************************************
 *  EthTSyn_Rx_IntRxIndication
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
/* PRQA S 6050, 6080 1 */ /* MD_EthTSyn_6050_TrivialFunctions, MD_EthTSyn_6080_RobustnessChecks */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_IntRxIndication(
                  uint8  CtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr,
                  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;
  EthTSyn_ComCtrl_EthIfCtrlIdxType comCtrlEthIfCtrlIdx = EthTSyn_GetSizeOfComCtrlEthIfCtrl();
  EthTSyn_IngrTimingInfoType ingrTimingInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the received message is big enough to hold the general PTP header. */
  if (LenByte >= ETHTSYN_MSG_HDR_LEN)
  {
    /* #20 Get ingress timing information. */
    ingrTimingInfo = EthTSyn_Rx_GetIngrTimingInfo(CtrlIdx, DataPtr);

    /* #30 Find a matching ComCtrlEthIfCtrl. */
    comCtrlEthIfCtrlIdx = EthTSyn_ComCtrl_FindComCtrlEthIfCtrl(CtrlIdx);
    if (comCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl())
    {
      /* #40 Check if received message is valid with respect to general aspects. */
      errorId = EthTSyn_Rx_CheckMsgGeneral(DataPtr, LenByte);
      if (errorId == ETHTSYN_E_NO_ERROR)
      {
        /* #50 Check if a matching ComCtrlPort exists. */
        const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx =
          EthTSyn_ComCtrl_FindPort(comCtrlEthIfCtrlIdx, EthTSyn_Rx_GetTdIdFromHdr(DataPtr));

        if (comCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort())
        {
          /* #60 Check if link state is active. */
          if (EthTSyn_ComCtrl_IsLinkStateActive(comCtrlPortIdx) == TRUE)
          {
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
            /* #70 Trigger processing of the received message (if switch management is disabled). */
            EthTSyn_RxTsInfoType rxTsInfo = {0};
            rxTsInfo.TsInfo.IntVltMgmt = ingrTimingInfo.IntVltMgmt;
#  if ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON))
            rxTsInfo.SyncRxEthIntVltMgmt = ingrTimingInfo.SyncRxEthIntVltMgmt;
            rxTsInfo.SyncRxStbmIntVltMgmt = ingrTimingInfo.SyncRxStbmIntVltMgmt;
#  endif /* ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)) */

            EthTSyn_Rx_IntProcRcvdMsg(comCtrlPortIdx, DataPtr, &rxTsInfo);
# else
            /* #71 Forward the received frame to the SwitchManagementRx unit for message buffering
             *     (if switch management is enabled). */
            errorId = EthTSyn_SwtMgmtRx_Receive(comCtrlEthIfCtrlIdx, DataPtr, &ingrTimingInfo);
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */
          }
          /* #61 Otherwise, return errorId ETHTSYN_E_TRCV_DOWN. */
          else
          {
            errorId = ETHTSYN_E_TRCV_DOWN;
          }
        }
        /* #51 Otherwise, return errorId ETHTSYN_E_DOMAIN_NUMBER. */
        else
        {
          errorId = ETHTSYN_E_DOMAIN_NUMBER;
        }
      }
    }
    /* #31 Otherwise, return errorId ETHTSYN_E_CTRL_IDX. */
    else
    {
      errorId = ETHTSYN_E_CTRL_IDX;
    }
  }
  /* #11 Otherwise, return ETHTSYN_E_MSG_LENGTH. */
  else
  {
    errorId = ETHTSYN_E_MSG_LENGTH;
  }

  return errorId;
} /* EthTSyn_Rx_IntRxIndication() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetIngrTimingInfo
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_IngrTimingInfoType, ETHTSYN_CODE) EthTSyn_Rx_GetIngrTimingInfo(
                  uint8  CtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_IngrTimingInfoType ingrTimingInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF)
  /* #10 Enter critical section to ensure that time between invocation of RxIndication and getting the software
   *     ingress timestamp is minimized (if SW-Timestamping is used). */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) */

  /* #20 Get the ingress timestamp. */
  ingrTimingInfo.IntVltMgmt = EthTSyn_Rx_GetIngrTsAsIntVlt(CtrlIdx, DataPtr);

# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF)
  /* #30 Leave critical section (if SW-Timestamping is used). */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_OFF) */

# if ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON))
  {
    const uint8 transSpecAndMsgTypeField = EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_OFS);
    const uint8 msgType = (uint8)(transSpecAndMsgTypeField & ETHTSYN_MSG_HDR_MSG_TYPE_MASK);

    ingrTimingInfo.SyncRxEthIntVltMgmt.IntVltValid = FALSE;
    ingrTimingInfo.SyncRxStbmIntVltMgmt.IntVltValid = FALSE;

    /* #40 Check if message is a Sync (if HW-Timestamping is used and slave support is enabled). */
    if (msgType == ETHTSYN_MSG_TYPE_SYNC)
    {
      /* #50 Try to retrieve the current Ethernet and StbM times. */
      if (EthTSyn_Rx_GetSyncRxEthAndStbmTime(CtrlIdx, DataPtr, &ingrTimingInfo.SyncRxEthIntVltMgmt.IntVlt,
        &ingrTimingInfo.SyncRxStbmIntVltMgmt.IntVlt) == E_OK)
      {
        ingrTimingInfo.SyncRxEthIntVltMgmt.IntVltValid = TRUE;
        ingrTimingInfo.SyncRxStbmIntVltMgmt.IntVltValid = TRUE;
      }
    }
  }
# endif /* ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)) */

  return ingrTimingInfo;
} /* EthTSyn_Rx_GetIngrTimingInfo() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetIngrTsAsIntVlt
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_IntVltMgmtType, ETHTSYN_CODE) EthTSyn_Rx_GetIngrTsAsIntVlt(
                  uint8  CtrlIdx,
  ETHTSYN_P2CONST(uint8) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_IntVltMgmtType ingrTsAsIntVltMgmt = {0u /* IntVal */, FALSE /* IntVltValid */};

  /* ----- Implementation ----------------------------------------------- */
# if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  /* #10 Check if HW-Timestamping is used. */
  {
    Eth_TimeStampType ethIngrTs = {0u, 0u, 0u};
    Eth_TimeStampQualType ethIngrTsQual = ETH_INVALID;

    /* #11 Try to get the Ethernet/HW ingress timestamp via EthIf_GetIngressTimestamp() and check if it is valid. */
    if (EthIf_GetIngressTimestamp(CtrlIdx, (ETHTSYN_P2VAR(Eth_DataType))DataPtr, &ethIngrTs, &ethIngrTsQual) == E_OK) /* PRQA S 0310, 0311, 3305 */ /* MD_EthTSyn_PtrCast_EthBuffer */
    {
      if (ethIngrTsQual == ETH_VALID)
      {
        /* #12 Try to convert the ingress timestamp into an internal virtual local time. */
        EthTSyn_IntVltType ingrIntVlt = 0u;
        if (EthTSyn_Ts_TsToIntVltChecked(&ethIngrTs, &ingrIntVlt) == E_OK)
        {
          ingrTsAsIntVltMgmt.IntVlt = ingrIntVlt;
          ingrTsAsIntVltMgmt.IntVltValid = TRUE;
        }
      }
    }
  }
# else
  /* #20 Otherwise, SW-Timestamping is used. */
  {
    /* #21 Check if the time domain from the received message is known by the EthTSyn. */
    const EthTSyn_ComCtrl_TimeDomainIdxType comCtrlTdIdx =
      EthTSyn_ComCtrl_FindTimeDomain(EthTSyn_Rx_GetTdIdFromHdr(DataPtr));

     if (comCtrlTdIdx < EthTSyn_GetSizeOfComCtrlTimeDomain())
     {
       /* #22 Get the current virtual local time from the StbM via StbM_GetCurrentVirtualLocalTime() and convert
        *     it into internal representation in case the time was retrieved successfully. */
       StbM_VirtualLocalTimeType stbmVlt = {0u, 0u};
       StbM_SynchronizedTimeBaseType tbId = EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(comCtrlTdIdx);

       if (StbM_GetCurrentVirtualLocalTime(tbId, &stbmVlt) == E_OK)
       {
         ingrTsAsIntVltMgmt.IntVlt = EthTSyn_Ts_VltToIntVlt(&stbmVlt);
         ingrTsAsIntVltMgmt.IntVltValid = TRUE;
       }
     }

     ETHTSYN_DUMMY_STATEMENT(CtrlIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  }
# endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */

  return ingrTsAsIntVltMgmt;
} /* EthTSyn_Rx_GetIngrTsAsIntVlt() */

# if ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON))
/**********************************************************************************************************************
 *  EthTSyn_Rx_GetCurrEthAndStbmTime
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
/* PRQA S 6050 1 */ /* MD_EthTSyn_6050_TrivialFunctions */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Rx_GetSyncRxEthAndStbmTime(
                uint8                 CtrlIdx,
  ETHTSYN_P2CONST(uint8)              DataPtr,
    ETHTSYN_P2VAR(EthTSyn_IntVltType) SyncRxEthTimePtr,
    ETHTSYN_P2VAR(EthTSyn_IntVltType) SyncRxStbmTimeptr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const EthTSyn_ComCtrl_TimeDomainIdxType comCtrlTdIdx = EthTSyn_ComCtrl_FindTimeDomain(
    EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_DOMAIN_NUM_OFS));

  /* ----- Implementation ----------------------------------------------- */
  (*SyncRxEthTimePtr) = 0u;
  (*SyncRxStbmTimeptr) = 0u;

  /* #10 Check if timedomain of message is valid. */
  if (comCtrlTdIdx < EthTSyn_GetSizeOfComCtrlTimeDomain())
  {
    /* #20 Check if StbM uses a different time source as the Ethernet controller. */
    if (EthTSyn_IsStbMTimeSrcEqualOfSlavePortOfComCtrlTimeDomain(comCtrlTdIdx) == FALSE)
    {
      const StbM_SynchronizedTimeBaseType timeBaseId =
        EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(comCtrlTdIdx);
      Eth_TimeStampType currEthTime = {0u};
      Eth_TimeStampQualType currEthTimeQual = ETH_INVALID;
      Std_ReturnType ethGetCurrTimeRetVal = E_NOT_OK;
      Std_ReturnType stbmGetCurrVltRetVal = E_NOT_OK;
      StbM_VirtualLocalTimeType stbmCurrVlt = {0u};

      /* #30 Get the current Ethernet time and current virtual local time. Protect both calls from interruptions to
      *      minimize the delay between the two function calls. */
      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();
      ethGetCurrTimeRetVal = EthIf_GetCurrentTime(CtrlIdx, &currEthTimeQual, &currEthTime);
      stbmGetCurrVltRetVal = StbM_GetCurrentVirtualLocalTime(timeBaseId, &stbmCurrVlt);
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();

      /* #40 Check if Ethernet and StbM times were retrieved successfully and are valid. */
      if ((ethGetCurrTimeRetVal == E_OK) && (currEthTimeQual == ETH_VALID) && (stbmGetCurrVltRetVal == E_OK))
      {
        if (EthTSyn_Ts_TsToIntVltChecked(&currEthTime, SyncRxEthTimePtr) == E_OK)
        {
          (*SyncRxStbmTimeptr) = EthTSyn_Ts_VltToIntVlt(&stbmCurrVlt);
          retVal = E_OK;
        }
      }
    }
  }

  return retVal;
} /* EthTSyn_Rx_GetCurrEthAndStbmTime() */
# endif /* ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)) */

/**********************************************************************************************************************
 *  EthTSyn_Rx_CheckMsgGeneral
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
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_CheckMsgGeneral(
  ETHTSYN_P2CONST(uint8) DataPtr,
                  uint16 LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the message type is valid. */
  if (EthTSyn_Rx_IsMsgTypeValid(DataPtr) == TRUE)
  {
    /* #20 Check if the received message is at least as big as the message length in the PTP header. */
    if (LenByte >= EthTSyn_Rx_GetMsgLenFromHdr(DataPtr))
    {
      /* #30 Check if the PTP protocol version is supported. */
      if (EthTSyn_Rx_GetPtpProtocolVersionFromHdr(DataPtr) == ETHTSYN_PTP_VERSION)
      {
        errorId = ETHTSYN_E_NO_ERROR;
      }
      /* #31 Otherwise, return ETHTSYN_E_PROTOCOL_VERSION. */
      else
      {
        errorId = ETHTSYN_E_PROTOCOL_VERSION;
      }
    }
    /* #21 Otherwise, return ETHTSYN_E_MSG_LENGTH. */
    else
    {
      errorId = ETHTSYN_E_MSG_LENGTH;
    }
  }
  /* #11 Otherwise, return ETHTSYN_E_MSG_TYPE. */
  else
  {
    errorId = ETHTSYN_E_MSG_TYPE;
  }

  return errorId;
} /* EthTSyn_Rx_CheckMsgGeneral() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_IsMsgTypeValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_Rx_IsMsgTypeValid(
  ETHTSYN_P2CONST(uint8) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isMsgTypeValid = FALSE;
  uint8 msgType = EthTSyn_Rx_GetMsgTypeFromHdr(DataPtr);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return TRUE when message type is Sync, FollowUp, Announce, Pdelay_Req, Pdelay_Resp or Pdelay_Resp_Fup.
         Otherwise, return FALSE. */
  if (
    (msgType == ETHTSYN_MSG_TYPE_SYNC)     ||
    (msgType == ETHTSYN_MSG_TYPE_FUP)      ||
    (msgType == ETHTSYN_MSG_TYPE_ANNOUNCE) ||
    (msgType == ETHTSYN_MSG_TYPE_PD_REQ)   ||
    (msgType == ETHTSYN_MSG_TYPE_PD_RESP)  ||
    (msgType == ETHTSYN_MSG_TYPE_PD_RESP_FUP)
    )
  {
    isMsgTypeValid = TRUE;
  }

  return isMsgTypeValid;
} /* EthTSyn_Rx_IsMsgTypeValid() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetMsgTypeFromHdr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_GetMsgTypeFromHdr(
  ETHTSYN_P2CONST(uint8) DataPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the message type from the message header and return it. */
  /* Note: Shift of ETHTSYN_MSG_HDR_MSG_TYPE_SHIFT is not required because it is 0. */
  return (uint8)(EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_MSG_TYPE_OFS) & ETHTSYN_MSG_HDR_MSG_TYPE_MASK);
} /* EthTSyn_Rx_GetMsgTypeFromHdr() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetMsgLenFromHdr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_Rx_GetMsgLenFromHdr(
  ETHTSYN_P2CONST(uint8) DataPtr)
{
  /* #10 Read the message length from the message header and return it. */
  return EthTSyn_Mem_GetUint16(DataPtr, ETHTSYN_MSG_HDR_MSG_LENGTH_OFS);
} /* EthTSyn_Rx_GetMsgLenFromHdr() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetTdIdFromHdr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_GetTdIdFromHdr(
  ETHTSYN_P2CONST(uint8) DataPtr)
{
  /* #10 Read the time domain id from the message header and return it. */
  return EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_DOMAIN_NUM_OFS);
} /* EthTSyn_Rx_GetTdIdFromHdr() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_GetPtpProtocolVersionFromHdr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_Rx_GetPtpProtocolVersionFromHdr(
  ETHTSYN_P2CONST(uint8) DataPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the PTP protocol version from the message header and return it. */
  /* Note: Shift of ETHTSYN_MSG_HDR_VERSION_PTP_SHIFT is not required because it is 0. */
  return (uint8)(EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_VERSION_PTP_OFS) & ETHTSYN_MSG_HDR_VERSION_PTP_MASK);
} /* EthTSyn_Rx_GetPtpProtocolVersionFromHdr() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_IntProcRcvdMsg
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_Rx_IntProcRcvdMsg(
                  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  ETHTSYN_P2CONST(uint8)                      DataPtr,
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)       RxTsInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_MsgHdrType msgHdr = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 De-serialize the PTP message header. */
  msgHdr = EthTSyn_Rx_DeSerializeHdr(DataPtr);

  /* #20 Check the message type. */
  switch(msgHdr.MessageType)
  {
    /* #30 Forward a received Sync message to the SlaveRx unit (if slave support is enabled). */
    case ETHTSYN_MSG_TYPE_SYNC:
      {
# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
        EthTSyn_SlaveRx_ProcRcvdSync(EthTSyn_GetSlaveRxDataHandlerIdxOfComCtrlPort(ComCtrlPortIdx), &msgHdr,
          RxTsInfoPtr);
# else
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
        /* #31 Report DET runtime error ETHTSYN_E_TMCONFLICT
         *     (if slave support is disabled and master-slave conflict detection is enabled). */
        (void)Det_ReportRuntimeError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_RX_INT_PROC_RCVD_MSG,
          ETHTSYN_E_TMCONFLICT);
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */
        break;
      }

    /* #40 Forward a received FollowUp message to the SlaveRx unit (if slave support is enabled). */
    case ETHTSYN_MSG_TYPE_FUP:
      {
# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
        ETHTSYN_CONSTP2CONST(uint8) ptpPayloadPtr = &DataPtr[ETHTSYN_MSG_HDR_LEN];
        EthTSyn_SlaveRx_ProcRcvdFup(EthTSyn_GetSlaveRxDataHandlerIdxOfComCtrlPort(ComCtrlPortIdx), &msgHdr,
          ptpPayloadPtr);
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */
        break;
      }

    /* #50 Forward a received Announce message to the SlaveRx unit (if slave support is enabled). */
    case ETHTSYN_MSG_TYPE_ANNOUNCE:
      {
# if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
        ETHTSYN_CONSTP2CONST(uint8) ptpPayloadPtr = &DataPtr[ETHTSYN_MSG_HDR_LEN];
        EthTSyn_SlaveRx_ProcRcvdAnnounce(EthTSyn_GetSlaveRxDataHandlerIdxOfComCtrlPort(ComCtrlPortIdx), &msgHdr,
          ptpPayloadPtr);
# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */
        break;
      }

    /* #60 Forward a received Pdelay_Req message to the PdRespSm unit (if pdelay responder is enabled). */
    case ETHTSYN_MSG_TYPE_PD_REQ:
      {
# if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON)
        EthTSyn_PdRespSm_ProcRcvdPdReq(EthTSyn_GetPdRespSmIdxOfComCtrlPort(ComCtrlPortIdx), &msgHdr,
          &RxTsInfoPtr->TsInfo);
# endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */
        break;
      }

    /* #70 Forward a received Pdelay_Resp message to the PdReqTrcv unit (if pdelay initiator is enabled). */
    case ETHTSYN_MSG_TYPE_PD_RESP:
      {
# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
        ETHTSYN_CONSTP2CONST(uint8) ptpPayloadPtr = &DataPtr[ETHTSYN_MSG_HDR_LEN];
        EthTSyn_PdReqTrcv_ProcRcvdPdResp(EthTSyn_GetPdReqSmIdxOfComCtrlPort(ComCtrlPortIdx), &msgHdr, ptpPayloadPtr,
          &RxTsInfoPtr->TsInfo);
# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */
        break;
      }

    /* #80 Forward a received Pdelay_Resp_Fup message to the PdReqTrcv unit (if pdelay initiator is enabled). */
    default: /* ETHTSYN_MSG_TYPE_PD_RESP_FUP */
      {
# if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
        ETHTSYN_CONSTP2CONST(uint8) ptpPayloadPtr = &DataPtr[ETHTSYN_MSG_HDR_LEN];
        EthTSyn_PdReqTrcv_ProcRcvdPdRespFup(EthTSyn_GetPdReqSmIdxOfComCtrlPort(ComCtrlPortIdx), &msgHdr,
          ptpPayloadPtr);
# endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */
        break;
      }
  }

  ETHTSYN_DUMMY_STATEMENT(ComCtrlPortIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(RxTsInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
} /* EthTSyn_Rx_IntProcRcvdMsg() */

/**********************************************************************************************************************
 *  EthTSyn_Rx_DeSerializeHdr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_MsgHdrType, ETHTSYN_CODE) EthTSyn_Rx_DeSerializeHdr(
  ETHTSYN_P2CONST(uint8) DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_MsgHdrType msgHdr = {0};
  uint8 transSpecAndMsgTypeField = EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_OFS);
  uint8 resAndVersionField = EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_RESERVED_0_OFS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read all PTP message header fields from the RxBuffer and return the header as structure. */
  msgHdr.TransportSpecific = (uint8)(
    (transSpecAndMsgTypeField & ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_MASK) >> ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_SHIFT);
  /* Note: Shift of ETHTSYN_MSG_HDR_MSG_TYPE_SHIFT is not required because it is 0. */
  msgHdr.MessageType = (uint8)(transSpecAndMsgTypeField & ETHTSYN_MSG_HDR_MSG_TYPE_MASK);

  /* Note: Shift of ETHTSYN_MSG_HDR_VERSION_PTP_SHIFT is not required because it is 0. */
  msgHdr.PtpVersion = (uint8)(resAndVersionField & ETHTSYN_MSG_HDR_VERSION_PTP_MASK);

  msgHdr.MessageLength = EthTSyn_Mem_GetUint16(DataPtr, ETHTSYN_MSG_HDR_MSG_LENGTH_OFS);
  msgHdr.DomainNumber = EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_DOMAIN_NUM_OFS);

  msgHdr.Flags0 = EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_FLAGS_0_OFS);
  msgHdr.Flags1 = EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_FLAGS_1_OFS);

  msgHdr.CorrectionFieldNs = (uint64)EthTSyn_Mem_GetUint48(DataPtr, ETHTSYN_MSG_HDR_CORR_FIELD_OFS);
  msgHdr.CorrectionFieldSubNs = EthTSyn_Mem_GetUint16(DataPtr, ETHTSYN_MSG_HDR_CORR_FIELD_SUB_NS_OFS);

  msgHdr.SourceClockId = EthTSyn_Mem_GetUint64(DataPtr, ETHTSYN_MSG_HDR_SRC_PORT_IDENT_CLOCK_IDENT_OFS);
  msgHdr.SourcePortNumber = EthTSyn_Mem_GetUint16(DataPtr, ETHTSYN_MSG_HDR_SRC_PORT_IDENT_PORT_NUM_OFS);

  msgHdr.SequenceId = EthTSyn_Mem_GetUint16(DataPtr, ETHTSYN_MSG_HDR_SEQUENCE_ID_OFS);

  msgHdr.Control = EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_CONTROL_OFS);

  msgHdr.LogMessageInterval = (sint8)EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_LOG_MSG_INTERVAL_OFS);

  return msgHdr;
} /* EthTSyn_Rx_DeSerializeHdr() */

#endif /* ETHTSYN_NOUNIT_RX */

/**********************************************************************************************************************
 * Internal functions of SwtMgmtRx
 *!
 * \unit SwtMgmtRx
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_SWTMGMTRX)
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_GetComCtrlPort
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_ComCtrl_PortIdxType, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_GetComCtrlPort(
                  EthTSyn_SwtMgmtRx_IdxType               SwtMgmtRxIdx,
  ETHTSYN_P2CONST(uint8)                                  DataPtr,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmt_MgmtInfoMgmtStructType) SwtMgmtInfoMgmtPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetSizeOfComCtrlPort();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if SwtMgmtInfo object is valid. */
  if (SwtMgmtInfoMgmtPtr->IsValid == TRUE)
  {
    const EthTSyn_ComCtrl_EthIfCtrlIdxType comCtrlEthIfCtrlIdx =
      EthTSyn_GetComCtrlEthIfCtrlIdxOfSwtMgmtRxDyn(SwtMgmtRxIdx);
    EthTSyn_ComCtrlPortIndIterType comCtrlPortIndIter = 0;

    /*@ assert comCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl(); */                                           /* VCA_ETHTSYN_SWT_MGMT_RX_BUF_VALID_CTRL_IDX */
    /* #20 Check for every port associated to the stored EthIf controller: */
    for (comCtrlPortIndIter = EthTSyn_GetComCtrlPortIndStartIdxOfComCtrlEthIfCtrl(comCtrlEthIfCtrlIdx);
         comCtrlPortIndIter < EthTSyn_GetComCtrlPortIndEndIdxOfComCtrlEthIfCtrl(comCtrlEthIfCtrlIdx);
         comCtrlPortIndIter++)
    {
      const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdxTmp = EthTSyn_GetComCtrlPortInd(comCtrlPortIndIter);

      /* #30 The switch port Idx matches.
       *     The switch Idx matches.
       *     The timedomain Id is equal to the one of the PTP header. */
      if ((EthTSyn_GetSwitchPortIdxOfComCtrlPort(comCtrlPortIdxTmp) == SwtMgmtInfoMgmtPtr->MgmtInfo.PortIdx) &&
          (EthTSyn_GetSwitchIdxOfComCtrlPort(comCtrlPortIdxTmp) == SwtMgmtInfoMgmtPtr->MgmtInfo.SwitchIdx))
      {
        const uint8 tdIdOfMsg = EthTSyn_Mem_GetUint8(DataPtr, ETHTSYN_MSG_HDR_DOMAIN_NUM_OFS);
        const uint8 tdIdOfPort = EthTSyn_GetIdOfComCtrlTimeDomain(
          EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(comCtrlPortIdxTmp));
        if (tdIdOfMsg == tdIdOfPort)
        {
          /* #40 Return matching port. */
          comCtrlPortIdx = comCtrlPortIdxTmp;
          break;
        }
      }
    }
  }

  return comCtrlPortIdx;
} /* EthTSyn_SwtMgmtRx_GetComCtrlPort() */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
#endif /* ETHTSYN_NOUNIT_SWTMGMTRX */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Public interfaces of Rx
 *!
 * \unit Rx
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_RX)
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_Rx_ProcRcvdMsg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_Rx_ProcRcvdMsg(
                  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  ETHTSYN_P2CONST(uint8)                      DataPtr,
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)       RxTsInfoPtr)
{
  /* #10 Process the received message. */
  EthTSyn_Rx_IntProcRcvdMsg(ComCtrlPortIdx, DataPtr, RxTsInfoPtr);
} /* EthTSyn_Rx_ProcRcvdMsg() */
# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/* ----- Service interfaces of Rx --------------------------------------- */
/**********************************************************************************************************************
 *  EthTSyn_RxIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
FUNC(void, ETHTSYN_CODE) EthTSyn_RxIndication(
                uint8         CtrlIdx,
                Eth_FrameType FrameType,
                boolean       IsBroadcast,
  ETHTSYN_P2VAR(uint8)        PhysAddrPtr, /* PRQA S 3673 */ /* MD_EthTSyn_3673_EthIfCompatibility */
  ETHTSYN_P2VAR(uint8)        DataPtr, /* PRQA S 3673 */ /* MD_EthTSyn_3673_EthIfCompatibility */
                uint16        LenByte)
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
  else if ((DataPtr == NULL_PTR) || (PhysAddrPtr == NULL_PTR))
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else if (FrameType != ETHTSYN_FRAME_TYPE)
  {
    errorId = ETHTSYN_E_FRAME_TYPE;
  }
  else
# endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Process the RxIndication. */
    errorId = EthTSyn_Rx_IntRxIndication(CtrlIdx, DataPtr, LenByte);
  }

# if (ETHTSYN_DEV_ERROR_DETECT == STD_OFF)
  ETHTSYN_DUMMY_STATEMENT(FrameType); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(PhysAddrPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_OFF) */
  ETHTSYN_DUMMY_STATEMENT(IsBroadcast); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  /* ----- Development Error Report ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_RX_INDICATION, errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
} /* EthTSyn_RxIndication() */
#endif /* ETHTSYN_NOUNIT_RX */

/**********************************************************************************************************************
 * Public interfaces of SwtMgmtRx
 *!
 * \unit SwtMgmtRx
 *********************************************************************************************************************/
#if !defined(ETHTSYN_NOUNIT_SWTMGMTRX)
# if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtRxIterType swtMgmtRxIter = 0u;
  EthTSyn_IngrTimingInfoType invIngrTimingInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  invIngrTimingInfo.IntVltMgmt.IntVltValid = FALSE;
#  if ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON))
  invIngrTimingInfo.SyncRxEthIntVltMgmt.IntVltValid = FALSE;
  invIngrTimingInfo.SyncRxStbmIntVltMgmt.IntVltValid = FALSE;
#  endif /* ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)) */

  for (swtMgmtRxIter = 0; swtMgmtRxIter < EthTSyn_GetSizeOfSwtMgmtRx(); swtMgmtRxIter++)
  {
    EthTSyn_SetIngrTimingInfoOfSwtMgmtRxDyn(swtMgmtRxIter, invIngrTimingInfo);
    EthTSyn_SetStateOfSwtMgmtRxDyn(swtMgmtRxIter, ETHTSYN_UNUSED_STATEOFSWTMGMTRXDYN);
    EthTSyn_SetComCtrlEthIfCtrlIdxOfSwtMgmtRxDyn(swtMgmtRxIter, EthTSyn_GetSizeOfComCtrlEthIfCtrl());
  }

  VStdLib_MemClr(EthTSyn_GetAddrSwtMgmtRxMsgBuf(0u), EthTSyn_GetSizeOfSwtMgmtRxMsgBuf()); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
} /* EthTSyn_SwtMgmtRx_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_Receive
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
/* PRQA S 6050 1 */ /* MD_EthTSyn_6050_TrivialFunctions */
FUNC(uint8, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_Receive(
                  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx,
  ETHTSYN_P2CONST(uint8)                           DataPtr,
  ETHTSYN_P2CONST(EthTSyn_IngrTimingInfoType)      IngrTimingInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;
  const EthTSyn_SwtMgmtQ_IdxType swtMgmtQIdx = EthTSyn_GetSwtMgmtQRxInstIdxOfSwtMgmtQInd(0u);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reserve free SwtMgmtQ buffer via EthTSyn_SwtMgmtQ_PreparePush() and check if returned SwtMgmtRxIdx is valid. */
  const EthTSyn_SwtMgmtRx_IdxType swtMgmtRxIdx = EthTSyn_SwtMgmtQ_PreparePush(swtMgmtQIdx,
    ETHTSYN_SWTMGMTRX_SWTMGMTOBJUSEROFSWTMGMTQBUF, EthTSyn_GetSizeOfSwtMgmtRx());

  if (swtMgmtRxIdx < EthTSyn_GetSizeOfSwtMgmtRx())
  {
    /* #20 Check if message fits into returned SwtMgmtRx buffer. */
    const uint16 msgLength = EthTSyn_Mem_GetUint16(DataPtr, ETHTSYN_MSG_HDR_MSG_LENGTH_OFS);
    if (msgLength <= EthTSyn_GetSwtMgmtRxMsgBufLengthOfSwtMgmtRx(swtMgmtRxIdx))
    {
      /* #30 Try to retrieve RxMgmt object from EthIf via EthIf_GetRxMgmtObject(). */
      EthTSyn_SwtMgmt_EthSwtMgmtObjPtrType ethSwtMgmtObjPtr = NULL_PTR;
      if (EthIf_GetRxMgmtObject(EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx),
            (Eth_DataType*)DataPtr, &ethSwtMgmtObjPtr) == E_OK) /* PRQA S 0310, 0311, 3305 */ /* MD_EthTSyn_PtrCast_EthBuffer */
      {
        /* #40 Store message, EthIf controller index, ingress timestamp and optional Ethernet and StbM times in
         *     internal buffer.
         *     Push RxMgmt object to SwtMgmtQ.
         *     Set buffer state to PENDING while protected against link state interrupts. */
        /* PRQA S 0315 2 */ /* MD_MSR_VStdLibCopy */
        VStdLib_MemCpy_s(EthTSyn_GetAddrSwtMgmtRxMsgBuf(EthTSyn_GetSwtMgmtRxMsgBufStartIdxOfSwtMgmtRx(swtMgmtRxIdx)),
          EthTSyn_GetSwtMgmtRxMsgBufLengthOfSwtMgmtRx(swtMgmtRxIdx), DataPtr, msgLength);

        EthTSyn_SetComCtrlEthIfCtrlIdxOfSwtMgmtRxDyn(swtMgmtRxIdx, ComCtrlEthIfCtrlIdx);
        EthTSyn_SetIngrTimingInfoOfSwtMgmtRxDyn(swtMgmtRxIdx, (*IngrTimingInfoPtr));

        EthTSyn_SwtMgmtQ_Push(swtMgmtQIdx, ethSwtMgmtObjPtr);

        SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
        EthTSyn_SetStateOfSwtMgmtRxDyn(swtMgmtRxIdx, ETHTSYN_PENDING_STATEOFSWTMGMTRXDYN);
        SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
      }
      /* #31 Otherwise release SwtMgmtQ buffer via EthTSyn_SwtMgmtQ_CancelPush().
      *      Return error Id ETHTSYN_SWT_MGMT_E_GET_RX_MGMT_OBJ_FAILED. */
      else
      {
        EthTSyn_SwtMgmtQ_CancelPush(swtMgmtQIdx);
        errorId = ETHTSYN_SWT_MGMT_E_GET_RX_MGMT_OBJ_FAILED;
      }
    }
    /* #21 Otherwise release SwtMgmtQ buffer via EthTSyn_SwtMgmtQ_CancelPush().
     *     Return error Id ETHTSYN_SWT_MGMT_E_MSG_BUFFER_PAYLOAD_OVERFLOW. */
    else
    {
      EthTSyn_SwtMgmtQ_CancelPush(swtMgmtQIdx);
      errorId = ETHTSYN_SWT_MGMT_E_MSG_BUFFER_PAYLOAD_OVERFLOW;
    }
  }
  /* #11 Otherwise, return error Id ETHTSYN_SWT_MGMT_E_MSG_BUFFER_OVERFLOW. */
  else
  {
    errorId = ETHTSYN_SWT_MGMT_E_MSG_BUFFER_OVERFLOW;
  }

  return errorId;
} /* EthTSyn_SwtMgmtRx_Receive() */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ProcMsg
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ProcMsg(
                  EthTSyn_SwtMgmtRx_IdxType               SwtMgmtRxIdx,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmt_MgmtInfoMgmtStructType) SwtMgmtInfoMgmtPtr,
  ETHTSYN_P2CONST(EthTSyn_SwtMgmtRxTsInfoType)            SwtRxTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  if (EthTSyn_GetStateOfSwtMgmtRxDyn(SwtMgmtRxIdx) == ETHTSYN_PENDING_STATEOFSWTMGMTRXDYN)
  {
    EthTSyn_SetStateOfSwtMgmtRxDyn(SwtMgmtRxIdx, ETHTSYN_UNUSED_STATEOFSWTMGMTRXDYN);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    {
      ETHTSYN_P2CONST(uint8) dataPtr = EthTSyn_GetAddrSwtMgmtRxMsgBuf(
        EthTSyn_GetSwtMgmtRxMsgBufStartIdxOfSwtMgmtRx(SwtMgmtRxIdx));
      const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx =
        EthTSyn_SwtMgmtRx_GetComCtrlPort(SwtMgmtRxIdx, dataPtr, SwtMgmtInfoMgmtPtr);

      if (comCtrlPortIdx < EthTSyn_GetSizeOfComCtrlPort())
      {
        EthTSyn_IngrTimingInfoType ingrTimingInfo = {0};
        EthTSyn_RxTsInfoType rxTsInfo = {0u};

        ingrTimingInfo = EthTSyn_GetIngrTimingInfoOfSwtMgmtRxDyn(SwtMgmtRxIdx);

        rxTsInfo.TsInfo.IntVltMgmt = ingrTimingInfo.IntVltMgmt;
        rxTsInfo.TsInfo.SwtIngrTsMgmt = SwtRxTsInfoPtr->SwtIngrTsMgmt;
        rxTsInfo.TsInfo.SwtEgrTsMgmt = SwtRxTsInfoPtr->SwtEgrTsMgmt;
#  if ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON))
        rxTsInfo.SyncRxEthIntVltMgmt = ingrTimingInfo.SyncRxEthIntVltMgmt;
        rxTsInfo.SyncRxStbmIntVltMgmt = ingrTimingInfo.SyncRxStbmIntVltMgmt;
#  endif /* ((ETHTSYN_SLAVE_SUPPORT == STD_ON) && (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)) */
#  if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
        rxTsInfo.AddInfo = SwtRxTsInfoPtr->AddInfo;
#  endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */
        EthTSyn_Rx_ProcRcvdMsg(comCtrlPortIdx, dataPtr, &rxTsInfo);
      }
    }
  }
  else
  {
    EthTSyn_SetStateOfSwtMgmtRxDyn(SwtMgmtRxIdx, ETHTSYN_UNUSED_STATEOFSWTMGMTRXDYN);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }
} /* EthTSyn_SwtMgmtRx_ProcMsg() */

#  if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_GetSyncIngrTsAndResTime
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_GetSyncIngrTsAndResTime(
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)    SyncIngrTsInfoPtr,
    ETHTSYN_P2VAR(EthTSyn_IntVltType)    SyncIngrTsPtr,
    ETHTSYN_P2VAR(EthTSyn_UTimediffType) SwtResTimePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize out-parameter */
  (*SyncIngrTsPtr) = 0u;
  (*SwtResTimePtr) = 0u;

#   if (ETHTSYN_SWT_MGMT_ONE_TAS == STD_ON)
  {
    /* #10 If host and switch are considered as one time aware system, check if switch ingress timestamp is valid.
     *       Switch ingress timestamp is considered as Sync ingress timestamp.
     *       Residence time is zero. */
    if (SyncIngrTsInfoPtr->SwtIngrTsMgmt.TsValid == TRUE)
    {
      EthTSyn_IntVltType ingrIntVlt = 0u;
      if (EthTSyn_Ts_TsToIntVltChecked(&SyncIngrTsInfoPtr->SwtIngrTsMgmt.Ts, &ingrIntVlt) == E_OK)
      {
        (*SyncIngrTsPtr) = ingrIntVlt;
        (*SwtResTimePtr) = 0;
        retVal = E_OK;
      }
    }
  }
#   else
  {
    /* #20 Otherwise check if host ingress timestamp is valid.
     *       Host ingress timestamp is considered as Sync ingress timestamp. */
    if (SyncIngrTsInfoPtr->IntVltMgmt.IntVltValid == TRUE)
    {
      (*SyncIngrTsPtr) = SyncIngrTsInfoPtr->IntVltMgmt.IntVlt;

      /* #30 If configured rx residence time is > 0, use this static value as residence time. */
#    if ( ETHTSYN_SWT_RX_TO_UPLINK_RESIDENCE_TIME_NS > 0 )
      {
        (*SwtResTimePtr) = (EthTSyn_TimediffType)ETHTSYN_SWT_RX_TO_UPLINK_RESIDENCE_TIME_NS;
        retVal = E_OK;
      }
#    else
      {
        /* #31 Try to calculate residence time from switch egress and ingress timestamps, otherwise.
         *       If calculation succeeded, return E_OK. */
        if ((SyncIngrTsInfoPtr->SwtIngrTsMgmt.TsValid == TRUE) && (SyncIngrTsInfoPtr->SwtEgrTsMgmt.TsValid == TRUE))
        {
          EthTSyn_TimediffType signedResTime = 0;
          if (EthTSyn_Ts_TsMinusTs(&SyncIngrTsInfoPtr->SwtEgrTsMgmt.Ts, &SyncIngrTsInfoPtr->SwtIngrTsMgmt.Ts,
               &signedResTime) == E_OK)
          {
            if (signedResTime >= 0)
            {
              (*SwtResTimePtr) = (EthTSyn_UTimediffType)signedResTime;
              retVal = E_OK;
            }
          }
        }
      }
#    endif /* ETHTSYN_SWT_RX_TO_UPLINK_RESIDENCE_TIME_NS > 0 */
    }
  }
#   endif /* (ETHTSYN_SWT_MGMT_ONE_TAS == STD_ON) */

  return retVal;
} /* EthTSyn_SwtMgmtRx_GetSyncIngrTsAndResTime() */
#  endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmtRx_ReleasePendingMgmtObjsOfCtrl
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtMgmtRx_ReleasePendingMgmtObjsOfCtrl(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtMgmtRxIterType swtMgmtRxIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
  for (swtMgmtRxIter = 0; swtMgmtRxIter < EthTSyn_GetSizeOfSwtMgmtRx(); swtMgmtRxIter++)
  {
    if ((EthTSyn_GetStateOfSwtMgmtRxDyn(swtMgmtRxIter) == ETHTSYN_PENDING_STATEOFSWTMGMTRXDYN) &&
       (EthTSyn_GetComCtrlEthIfCtrlIdxOfSwtMgmtRxDyn(swtMgmtRxIter) == ComCtrlEthIfCtrlIdx))
    {
      EthTSyn_SetStateOfSwtMgmtRxDyn(swtMgmtRxIter, ETHTSYN_RELEASED_STATEOFSWTMGMTRXDYN);
    }
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
} /* EthTSyn_SwtMgmtRx_ReleasePendingMgmtObjsOfCtrl() */

# endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
#endif /* ETHTSYN_NOUNIT_SWTMGMTRX */

#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Reception.c
 *********************************************************************************************************************/
