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
/*!        \file  EthTSyn_ComCtrl.c
 *         \unit  ComCtrl
 *        \brief  EthTSyn ComCtrl source file
 *      \details  Implementation of the ComCtrl unit.
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

#define ETHTSYN_COM_CTRL_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_ComCtrl.h"
#include "EthTSyn_ComCtrl_Cbk.h"
#include "EthTSyn_ComCtrl_Int.h"

#include "EthTSyn_GeneratedCfgAccess.h"
#include "EthTSyn_Init_Int.h"
#include "EthTSyn.h"

#include "EthTSyn_SyncSendSm_Int.h"
#include "EthTSyn_AnnounceSendSm_Int.h"

#include "EthTSyn_SlaveRx_Int.h"

#include "EthTSyn_PdReqSm_Int.h"

#include "EthTSyn_PdRespSm_Int.h"

#include "EthTSyn_SwtMgmtRx_Int.h"
#include "EthTSyn_SwtMgmtTx_Int.h"
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
#include "EthIf.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define ETHTSYN_IEEE_802_1_AS_CLOCK_IDENTITY_BYTE_3                   0xFFu
#define ETHTSYN_IEEE_802_1_AS_CLOCK_IDENTITY_BYTE_4                   0xFEu

/* Multicast bit mask for the MSB of a MAC address */
#define ETHTSYN_MAC_ADDR_MSB_MULTICAST_BIT_MASK                       0x01u

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
 *  EthTSyn_ComCtrl_HandleTrcvLinkStateDown
 *********************************************************************************************************************/
/*! \brief          Handles a reported transceiver link state down.
 *  \details        -
 *  \param[in]      ComCtrlEthIfCtrlIdx   Index of the ComCtrlEthIfCtrl.
 *                                        [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \pre            Must be called in context of EthTSyn_TrcvLinkStateChg()
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_HandleTrcvLinkStateDown(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_HandleTrcvLinkStateActive
 *********************************************************************************************************************/
/*! \brief          Handles a reported transceiver link state active.
 *  \details        -
 *  \param[in]      ComCtrlEthIfCtrlIdx   Index of the ComCtrlEthIfCtrl.
 *                                        [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \pre            Must be called in context of EthTSyn_TrcvLinkStateChg()
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_HandleTrcvLinkStateActive(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_SetupCtrl
 *********************************************************************************************************************/
/*! \brief          Setup the passed ComCtrlEthIfCtrl.
 *  \details        Resets the transmission mode and the clock identity.
 *  \param[in]      ComCtrlEthIfCtrlIdx   Index of the ComCtrlEthIfCtrl.
 *                                        [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_SetupCtrl(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_UpdatePhysAddrFilter
 *********************************************************************************************************************/
/*! \brief          Adds/removes all relevant multicast addresses to/from the physical address filter.
 *  \details        -
 *  \param[in]      ComCtrlEthIfCtrlIdx   Index of the ComCtrlEthIfCtrl.
 *                                        [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \param[in]      Action                Action that shall be applied on the MAC address filter:
 *                                        ETH_ADD_TO_FILTER - adapt filter to be able to receive frames with the linked
 *                                                            MAC addresses as destination MAC address
 *                                        ETH_REMOVE_FROM_FILTER - adapt filter to prevent reception of frames with the
 *                                                                 linked MAC addresses as destination MAC address
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_UpdatePhysAddrFilter(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx,
  Eth_FilterActionType             Action);

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_ResetAllStateMachines
 *********************************************************************************************************************/
/*! \brief          Resets all state machines operating on the passed ComCtrlEthIfCtrl.
 *  \details        -
 *  \param[in]      ComCtrlEthIfCtrlIdx   Index of the ComCtrlEthIfCtrl.
 *                                        [range: ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires ComCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_ResetAllStateMachines(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_HandleTrcvLinkStateDown
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_HandleTrcvLinkStateDown(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if link state changed from active to down. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  if (EthTSyn_IsLinkStateActiveOfComCtrlEthIfCtrlDyn(ComCtrlEthIfCtrlIdx))
  {
    /* #20 Store the new transceiver link state. */
    EthTSyn_SetLinkStateActiveOfComCtrlEthIfCtrlDyn(ComCtrlEthIfCtrlIdx, FALSE);

    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();

    /* #30 Remove multicast frame reception. */
    (void)EthTSyn_ComCtrl_UpdatePhysAddrFilter(ComCtrlEthIfCtrlIdx, ETH_REMOVE_FROM_FILTER);

    /* #40 Reset all ports of the controller. */
    EthTSyn_ComCtrl_ResetAllStateMachines(ComCtrlEthIfCtrlIdx);

    /* #50 Release all pending switch management objects mapped to the controller (if ETHTSYN_SWT_MGMT_SUPPORT). */
#if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    EthTSyn_SwtMgmtRx_ReleasePendingMgmtObjsOfCtrl(ComCtrlEthIfCtrlIdx);
    EthTSyn_SwtMgmtTx_ReleasePendingMgmtObjsOfCtrl(ComCtrlEthIfCtrlIdx);
#endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  }
  else
  {
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  }
} /* EthTSyn_ComCtrl_HandleTrcvLinkStateDown() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_HandleTrcvLinkStateActive
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_HandleTrcvLinkStateActive(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if link state changed from down to active. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  if (!EthTSyn_IsLinkStateActiveOfComCtrlEthIfCtrlDyn(ComCtrlEthIfCtrlIdx))
  {
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
    /* #20 Setup the controller. */
    EthTSyn_ComCtrl_SetupCtrl(ComCtrlEthIfCtrlIdx);

    /* #30 Add multicast frame reception. */
    (void)EthTSyn_ComCtrl_UpdatePhysAddrFilter(ComCtrlEthIfCtrlIdx, ETH_ADD_TO_FILTER);

    /* #40 Store the new transceiver link state. */
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
    EthTSyn_SetLinkStateActiveOfComCtrlEthIfCtrlDyn(ComCtrlEthIfCtrlIdx, TRUE);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  }
  else
  {
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  }
} /* EthTSyn_ComCtrl_HandleTrcvLinkStateActive() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_SetupCtrl
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_SetupCtrl(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Eth_PhysAddrType physAddr = {0u};
  uint64 clockId = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the physical address for the EthIf controller via EthIf_GetPhysAddr(). */
  EthIf_GetPhysAddr(EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx), physAddr);

  /* #20 Derive the clock identity from the physical address according to IEEE802.1As and store it. */
  /* According to IEEE802.1As the clock identity is an EUI-64 derived by the EUI-48 MAC address in the following way:
   * 1. Assign higher three bytes of MAC address (OUI) to higher three of the clock identity
   * 2. Assign 0xFF and 0xFE to bytes 3 respectively 4 to clock identity
   * 3. Assign lower three bytes of MAC address to lower three bytes of the clock identity */
  clockId = (uint64)(
    ((uint64)physAddr[0u] << 56u) | ((uint64)physAddr[1u] << 48u) | ((uint64)physAddr[2u] << 40u) |
    ((uint64)ETHTSYN_IEEE_802_1_AS_CLOCK_IDENTITY_BYTE_3 << 32u) |
    ((uint64)ETHTSYN_IEEE_802_1_AS_CLOCK_IDENTITY_BYTE_4 << 24u) |
    ((uint64)physAddr[3u] << 16u) | ((uint64)physAddr[4u] << 8u) | ((uint64)physAddr[5u])
    );

  /* #30 Set the clock identity and reset the transmission mode to ETHTSYN_TX_ON. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  EthTSyn_SetTxActiveOfComCtrlEthIfCtrlDyn(ComCtrlEthIfCtrlIdx, TRUE);
  EthTSyn_SetClockIdOfComCtrlEthIfCtrlDyn(ComCtrlEthIfCtrlIdx, clockId);
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_ComCtrl_SetupCtrl() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_UpdatePhysAddrFilter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_UpdatePhysAddrFilter(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx,
  Eth_FilterActionType             Action)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint8 ethIfCtrlIdx = EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx);
  EthTSyn_DestinationMacIndIterType destMacIndIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all linked destination addresses of the controller. */
  for(destMacIndIter = EthTSyn_GetDestinationMacIndStartIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx);
      destMacIndIter < EthTSyn_GetDestinationMacIndEndIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx);
      destMacIndIter++)
  {
    const EthTSyn_DestinationMacIndType destAddrIdx = EthTSyn_GetDestinationMacInd(destMacIndIter);

    /* #20 Check if the current address is a multicast address. */
    if ((EthTSyn_GetByte0OfDestinationMac(destAddrIdx) & ETHTSYN_MAC_ADDR_MSB_MULTICAST_BIT_MASK) != 0u)
    {
      EthTSyn_PhysAddrType destAddr = {0u};

      destAddr[0u] = EthTSyn_GetByte0OfDestinationMac(destAddrIdx);
      destAddr[1u] = EthTSyn_GetByte1OfDestinationMac(destAddrIdx);
      destAddr[2u] = EthTSyn_GetByte2OfDestinationMac(destAddrIdx);
      destAddr[3u] = EthTSyn_GetByte3OfDestinationMac(destAddrIdx);
      destAddr[4u] = EthTSyn_GetByte4OfDestinationMac(destAddrIdx);
      destAddr[5u] = EthTSyn_GetByte5OfDestinationMac(destAddrIdx);

      /* #30 Update the physical address filter of the underlying EthIf controller accordingly. */
      (void)EthIf_UpdatePhysAddrFilter(ethIfCtrlIdx, destAddr, Action);
    }
  }
} /* EthTSyn_ComCtrl_UpdatePhysAddrFilter() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_ResetAllStateMachines
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_ResetAllStateMachines(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_ComCtrlPortIndIterType comCtrlPortIndIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all ComCtrlPorts of the passed ComCtrlEthIfCtrl. */
  for (comCtrlPortIndIter = EthTSyn_GetComCtrlPortIndStartIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx);
       comCtrlPortIndIter < EthTSyn_GetComCtrlPortIndEndIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx);
       comCtrlPortIndIter++)
  {
    const EthTSyn_ComCtrlPortIndType comCtrlPortIdxTmp = EthTSyn_GetComCtrlPortInd(comCtrlPortIndIter);

    /* #20 Reset the SyncSendSm and the optional AnnounceSendSm (if ETHTSYN_MASTER_SUPPORT). */
#if (ETHTSYN_MASTER_SUPPORT == STD_ON)
    EthTSyn_SyncSendSm_Reset(EthTSyn_GetSyncSendSmIdxOfComCtrlPort(comCtrlPortIdxTmp));
    EthTSyn_AnnounceSendSm_Reset(EthTSyn_GetAnnounceSendSmIdxOfComCtrlPort(comCtrlPortIdxTmp));
#endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */

    /* #30 Reset the SlaveRx data (if ETHTSYN_SLAVE_SUPPORT). */
#if (ETHTSYN_SLAVE_SUPPORT == STD_ON)
    EthTSyn_SlaveRx_Reset(EthTSyn_GetSlaveRxDataHandlerIdxOfComCtrlPort(comCtrlPortIdxTmp));
#endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */

    /* #40 Reset the PdReqSm (if ETHTSYN_PDELAY_INITIATOR_SUPPORT). */
#if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
    EthTSyn_PdReqSm_Reset(EthTSyn_GetPdReqSmIdxOfComCtrlPort(comCtrlPortIdxTmp));
#endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */

    /* #50 Reset the PdRespSm (if ETHTSYN_PDELAY_RESPONDER_SUPPORT). */
#if (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON)
    EthTSyn_PdRespSm_Reset(EthTSyn_GetPdRespSmIdxOfComCtrlPort(comCtrlPortIdxTmp));
#endif /* (ETHTSYN_PDELAY_RESPONDER_SUPPORT == STD_ON) */
  }
} /* EthTSyn_ComCtrl_ResetAllStateMachines() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/* ----- Public interfaces of ComCtrl ----------------------------------- */
/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_ComCtrlPhysPortIterType comCtrlPhysPortIter = 0u;
  EthTSyn_ComCtrlEthIfCtrlIterType comCtrlEthIfCtrlIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all ComCtrlPhysPort. */
  for (comCtrlPhysPortIter = 0u;
       comCtrlPhysPortIter < EthTSyn_GetSizeOfComCtrlPhysPort();
       comCtrlPhysPortIter++)
  {
#if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
    EthTSyn_SetAsCapableOfComCtrlPhysPortDyn(comCtrlPhysPortIter,
      EthTSyn_IsAlwaysAsCapableOfComCtrlPhysPort(comCtrlPhysPortIter));
#endif /* (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON) */
    EthTSyn_SetPdelayValueNsOfComCtrlPhysPortDyn(comCtrlPhysPortIter,
      EthTSyn_GetInitialPdelayNsOfComCtrlPhysPort(comCtrlPhysPortIter));
    EthTSyn_SetDebounceTimerOfComCtrlPhysPortDyn(comCtrlPhysPortIter, 0u);
  }

  /* #20 Iterate all ComCtrlEthIfCtrl. */
  for (comCtrlEthIfCtrlIter = 0u;
       comCtrlEthIfCtrlIter < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
       comCtrlEthIfCtrlIter++)
  {
    /* #200 Initialize the link state. */
    EthTSyn_SetLinkStateActiveOfComCtrlEthIfCtrlDyn(comCtrlEthIfCtrlIter, FALSE);

    /* #201 Initialize other ComCtrlEthIfCtrl related RAM data. */
    EthTSyn_SetClockIdOfComCtrlEthIfCtrlDyn(comCtrlEthIfCtrlIter, 0u);
    EthTSyn_SetTxActiveOfComCtrlEthIfCtrlDyn(comCtrlEthIfCtrlIter, TRUE);
  }
} /* EthTSyn_ComCtrl_Init() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_ComCtrlPhysPortIterType comCtrlPhysPortIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Decrement all debounce timers. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  for (comCtrlPhysPortIter = 0u;
       comCtrlPhysPortIter < EthTSyn_GetSizeOfComCtrlPhysPort();
       comCtrlPhysPortIter++)
  {
    if (EthTSyn_GetDebounceTimerOfComCtrlPhysPortDyn(comCtrlPhysPortIter) > 0u)
    {
      EthTSyn_DecDebounceTimerOfComCtrlPhysPortDyn(comCtrlPhysPortIter);
    }
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

} /* EthTSyn_ComCtrl_MainFunction)= */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetClockId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint64, ETHTSYN_CODE) EthTSyn_ComCtrl_GetClockId(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint64 clockId = 0u;
  const EthTSyn_ComCtrlEthIfCtrlIdxOfComCtrlPortType comCtrlEthIfCtrlIdx =
    EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(ComCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the stored ClockId in case the link state is active and '0' otherwise. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  if (EthTSyn_IsLinkStateActiveOfComCtrlEthIfCtrlDyn(comCtrlEthIfCtrlIdx))
  {
    clockId = EthTSyn_GetClockIdOfComCtrlEthIfCtrlDyn(EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(ComCtrlPortIdx));
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();

  return clockId;
} /* EthTSyn_ComCtrl_GetClockId() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_IsLinkStateActive
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_ComCtrl_IsLinkStateActive(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean linkStateActive = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return TRUE if link state is active and FALSE otherwise. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  linkStateActive = (boolean)EthTSyn_IsLinkStateActiveOfComCtrlEthIfCtrlDyn(
    EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(ComCtrlPortIdx));
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();

  return linkStateActive;
} /* EthTSyn_ComCtrl_IsLinkStateActive() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_IsTxAllowed
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_ComCtrl_IsTxAllowed(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean txAllowed = FALSE;
  const EthTSyn_ComCtrlEthIfCtrlIdxOfComCtrlPortType comCtrlEthIfCtrlIdx =
    EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(ComCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if link state is active. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  if (EthTSyn_IsLinkStateActiveOfComCtrlEthIfCtrlDyn(comCtrlEthIfCtrlIdx))
  {
    /* #20 Check if transmission mode is enabled. */
    if (EthTSyn_IsTxActiveOfComCtrlEthIfCtrlDyn(comCtrlEthIfCtrlIdx))
    {
      const EthTSyn_ComCtrlPhysPortIdxOfComCtrlPortType comCtrlPhysPortIdx =
        EthTSyn_GetComCtrlPhysPortIdxOfComCtrlPort(ComCtrlPortIdx);

      /* #30 Check if debounce time expired. */
      if (EthTSyn_GetDebounceTimerOfComCtrlPhysPortDyn(comCtrlPhysPortIdx) == 0u)
      {
        /* #40 Mark Tx as allowed. */
        txAllowed = TRUE;
      }
    }
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  return txAllowed;
} /* EthTSyn_ComCtrl_IsTxAllowed() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_RestartDebounceTimer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_RestartDebounceTimer(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_ComCtrlPhysPortIdxOfComCtrlPortType comCtrlPhysPortIdx =
    EthTSyn_GetComCtrlPhysPortIdxOfComCtrlPort(ComCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Re-initialize the debounce timer to the configured start value. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  EthTSyn_SetDebounceTimerOfComCtrlPhysPortDyn(comCtrlPhysPortIdx,
    EthTSyn_GetDebounceTimerInitOfComCtrlPhysPort(comCtrlPhysPortIdx));
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_ComCtrl_RestartDebounceTimer() */

#if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_SetAsCapable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_SetAsCapable(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  boolean                     IsAsCapable)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_ComCtrlPhysPortIdxOfComCtrlPortType comCtrlPhysPortIdx =
    EthTSyn_GetComCtrlPhysPortIdxOfComCtrlPort(ComCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set value of AsCapable-flag if AlwaysAsCapable is disabled. */
  if (!EthTSyn_IsAlwaysAsCapableOfComCtrlPhysPort(comCtrlPhysPortIdx))
  {
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    EthTSyn_SetAsCapableOfComCtrlPhysPortDyn(comCtrlPhysPortIdx, IsAsCapable);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }
} /* EthTSyn_ComCtrl_SetAsCapable() */
#endif /* ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_IsAsCapable
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_ComCtrl_IsAsCapable(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isAsCapable = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return value of AsCapable-flag (if pdelay initiator support is enabled, return TRUE otherwise). */
#if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  isAsCapable = EthTSyn_IsAsCapableOfComCtrlPhysPortDyn(EthTSyn_GetComCtrlPhysPortIdxOfComCtrlPort(ComCtrlPortIdx));
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
#else
  ETHTSYN_DUMMY_STATEMENT(ComCtrlPortIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON */

  return isAsCapable;
} /* EthTSyn_ComCtrl_IsAsCapable() */

#if (ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_SetPdelay
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_SetPdelay(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx,
  EthTSyn_PdelayType          Pdelay)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_ComCtrlPhysPortIdxOfComCtrlPortType comCtrlPhysPortIdx =
    EthTSyn_GetComCtrlPhysPortIdxOfComCtrlPort(ComCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set Pdelay value if static Pdelay is disabled. */
  if (!EthTSyn_IsUseStaticPdelayOfComCtrlPhysPort(comCtrlPhysPortIdx))
  {
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    EthTSyn_SetPdelayValueNsOfComCtrlPhysPortDyn(ComCtrlPortIdx, Pdelay);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }
} /* EthTSyn_ComCtrl_SetPdelay() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_ResetPdelay
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_ComCtrl_ResetPdelay(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_ComCtrlPhysPortIdxOfComCtrlPortType comCtrlPhysPortIdx =
    EthTSyn_GetComCtrlPhysPortIdxOfComCtrlPort(ComCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set Pdelay value to the configured initial value. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  EthTSyn_SetPdelayValueNsOfComCtrlPhysPortDyn(comCtrlPhysPortIdx,
    EthTSyn_GetInitialPdelayNsOfComCtrlPhysPort(comCtrlPhysPortIdx));
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_ComCtrl_ResetPdelay() */
#endif /* ETHTSYN_PDELAY_INITIATOR_SUPPORT == STD_ON */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetPdelay
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(EthTSyn_PdelayType, ETHTSYN_CODE) EthTSyn_ComCtrl_GetPdelay(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PdelayType pdelay = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return Pdelay value. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  pdelay = EthTSyn_GetPdelayValueNsOfComCtrlPhysPortDyn(EthTSyn_GetComCtrlPhysPortIdxOfComCtrlPort(ComCtrlPortIdx));
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

  return pdelay;
} /* EthTSyn_ComCtrl_GetPdelay() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetEthIfCtrlIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, ETHTSYN_CODE) EthTSyn_ComCtrl_GetEthIfCtrlIdx(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* #10 Return the index of the underlying EthIfCtrl. */
  return EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(ComCtrlPortIdx));
} /* EthTSyn_ComCtrl_GetEthIfCtrlIdx() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_FindComCtrlEthIfCtrl
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(EthTSyn_ComCtrl_EthIfCtrlIdxType, ETHTSYN_CODE) EthTSyn_ComCtrl_FindComCtrlEthIfCtrl(
  uint8 EthIfCtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_ComCtrl_EthIfCtrlIdxType comCtrlEthIfCtrlIdx = EthTSyn_GetSizeOfComCtrlEthIfCtrl();
  EthTSyn_ComCtrlEthIfCtrlIterType comCtrlEthIfCtrlIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all ComCtrlEthIfCtrl. */
  for (comCtrlEthIfCtrlIter = 0u;
       comCtrlEthIfCtrlIter < EthTSyn_GetSizeOfComCtrlEthIfCtrl();
       comCtrlEthIfCtrlIter++)
  {
    /* #20 Check if EthIfCtrlIdx of the current ComCtrlEthIfCtrl matches the requested one. */
    if (EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(comCtrlEthIfCtrlIter) == EthIfCtrlIdx)
    {
      /* #30 Stop iteration and return the current ComcTrlEthIfCtrl index. */
      comCtrlEthIfCtrlIdx = (EthTSyn_ComCtrl_EthIfCtrlIdxType)comCtrlEthIfCtrlIter;
      break;
    }
  }

  return comCtrlEthIfCtrlIdx;
} /* EthTSyn_ComCtrl_FindComCtrlEthIfCtrl() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_FindPort
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(EthTSyn_ComCtrl_PortIdxType, ETHTSYN_CODE) EthTSyn_ComCtrl_FindPort(
  EthTSyn_ComCtrl_EthIfCtrlIdxType ComCtrlEthIfCtrlIdx,
  uint8                            TimeDomainId)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetSizeOfComCtrlPort();
  EthTSyn_ComCtrlPortIndIterType comCtrlPortIndIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all ComCtrlPorts of the passed ComCtrlEthIfCtrl. */
  for (comCtrlPortIndIter = EthTSyn_GetComCtrlPortIndStartIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx);
       comCtrlPortIndIter < EthTSyn_GetComCtrlPortIndEndIdxOfComCtrlEthIfCtrl(ComCtrlEthIfCtrlIdx);
       comCtrlPortIndIter++)
  {
    EthTSyn_ComCtrlPortIndType comCtrlPortIdxTmp = EthTSyn_GetComCtrlPortInd(comCtrlPortIndIter);

    /* #20 Check if time domain id of the current ComCtrlPort matches the passed one. */
    if (EthTSyn_GetIdOfComCtrlTimeDomain(
      EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(comCtrlPortIdxTmp)) == TimeDomainId)
    {
      /* #30 Stop iteration and return the current ComCtrlPort index. */
      comCtrlPortIdx = (EthTSyn_ComCtrl_PortIdxType)comCtrlPortIdxTmp;
      break;
    }
  }

  return comCtrlPortIdx;
} /* EthTSyn_ComCtrl_FindPort() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_FindTimeDomain
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(EthTSyn_ComCtrl_TimeDomainIdxType, ETHTSYN_CODE) EthTSyn_ComCtrl_FindTimeDomain(
  uint8 TimeDomainId)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_ComCtrlTimeDomainIterType comCtrlTdIter = 0u;
  EthTSyn_ComCtrl_TimeDomainIdxType comCtrlTdIdx = EthTSyn_GetSizeOfComCtrlTimeDomain();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all ComCtrlTimeDomains. */
  for (comCtrlTdIter = 0u;
       comCtrlTdIter < EthTSyn_GetSizeOfComCtrlTimeDomain();
       comCtrlTdIter++)
  {
    /* #20 Check if time domain id of the current ComCtrlTimeDomain matches the passed one. */
    if (EthTSyn_GetIdOfComCtrlTimeDomain(comCtrlTdIter) == TimeDomainId)
    {
      /* #30 Stop iteration and return the current ComCtrlTimeDomain index. */
      comCtrlTdIdx = (EthTSyn_ComCtrl_TimeDomainIdxType)comCtrlTdIter;
      break;
    }
  }

  return comCtrlTdIdx;
} /* EthTSyn_ComCtrl_FindTimeDomain() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetTimeDomainId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, ETHTSYN_CODE) EthTSyn_ComCtrl_GetTimeDomainId(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the time domain id of the passed port. */
  return EthTSyn_GetIdOfComCtrlTimeDomain(EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(ComCtrlPortIdx));
} /* EthTSyn_ComCtrl_GetTimeDomainId() */

/**********************************************************************************************************************
 *  EthTSyn_ComCtrl_GetSynchronizedTimeBaseId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(StbM_SynchronizedTimeBaseType, ETHTSYN_CODE) EthTSyn_ComCtrl_GetSynchronizedTimeBaseId(
  EthTSyn_ComCtrl_PortIdxType ComCtrlPortIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the synchronized time base id of the passed port. */
  return EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(
    EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(ComCtrlPortIdx));
} /* EthTSyn_ComCtrl_GetSynchronizedTimeBaseId() */

/* ----- Service interfaces of ComCtrl ---------------------------------- */
/**********************************************************************************************************************
 *  EthTSyn_SetTransmissionMode
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SetTransmissionMode(
  uint8                        CtrlIdx,
  EthTSyn_TransmissionModeType Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if (EthTSyn_Init_IsInitialized() == FALSE)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if ((Mode != ETHTSYN_TX_ON) && (Mode != ETHTSYN_TX_OFF))
  {
    errorId = ETHTSYN_E_PARAM;
  }
  else
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Check if a matching ComCtrlEthIfCtrl exists. */
    EthTSyn_ComCtrl_EthIfCtrlIdxType comCtrlEthIfCtrlIdx = EthTSyn_ComCtrl_FindComCtrlEthIfCtrl(CtrlIdx);

    if (comCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl())
    {
      /* #200 Set the transmission mode. */
      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
      EthTSyn_SetTxActiveOfComCtrlEthIfCtrlDyn(comCtrlEthIfCtrlIdx, (boolean)(Mode == ETHTSYN_TX_ON));
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    }
    /* #30 Otherwise, set errorId to ETHTSYN_E_CTRL_IDX (if DevErrorReport is enabled). */
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
    else
    {
      errorId = ETHTSYN_E_CTRL_IDX;
    }
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_SET_TRANSMISSION_MODE, errorId);
  }
#else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
} /* EthTSyn_SetTransmissionMode() */

/* ----- Public callbacks of ComCtrl ------------------------------------ */
/**********************************************************************************************************************
 *  EthTSyn_TrcvLinkStateChg
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
FUNC(void, ETHTSYN_CODE) EthTSyn_TrcvLinkStateChg(
  uint8                 CtrlIdx,
  EthTrcv_LinkStateType TrcvLinkState)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of component. */
  if (EthTSyn_Init_IsInitialized() == FALSE)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Check if a matching ComCtrlEthIfCtrl exists. */
    EthTSyn_ComCtrl_EthIfCtrlIdxType comCtrlEthIfCtrlIdx = EthTSyn_ComCtrl_FindComCtrlEthIfCtrl(CtrlIdx);

    if (comCtrlEthIfCtrlIdx < EthTSyn_GetSizeOfComCtrlEthIfCtrl())
    {
      /* #30 Check the passed transceiver link state. */
      switch (TrcvLinkState)
      {
        /* #40 Handle transceiver link state down. */
      case ETHTRCV_LINK_STATE_DOWN:
        {
          EthTSyn_ComCtrl_HandleTrcvLinkStateDown(comCtrlEthIfCtrlIdx);
          break;
        }

        /* #50 Handle transceiver link state active. */
      case ETHTRCV_LINK_STATE_ACTIVE:
        {
          EthTSyn_ComCtrl_HandleTrcvLinkStateActive(comCtrlEthIfCtrlIdx);
          break;
        }

        /* #60 Otherwise, handle invalid transceiver link state. */
      default:
        {
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
          errorId = ETHTSYN_E_PARAM;
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
          break;
        }
      }
    }
    /* No DET for invalid Ctrl Idx, because EthIf calls TrcvLinkStateChg functions for every registered UpperLayer
       and each configured EthIf controller. */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #70 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_TRCV_LINK_STATE_CHG, errorId);
  }
#else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */
} /* EthTSyn_TrcvLinkStateChg() */

#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_ComCtrl.c
 *********************************************************************************************************************/
