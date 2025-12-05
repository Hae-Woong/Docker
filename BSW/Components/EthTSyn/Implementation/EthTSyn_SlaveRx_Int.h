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
/*!        \file  EthTSyn_SlaveRx_Int.h
 *         \unit  SlaveRx
 *        \brief  EthTSyn internal header file of the SlaveRx unit.
 *      \details  Interface and type definitions of the SlaveRx unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SLAVE_RX_INT_H)
# define ETHTSYN_SLAVE_RX_INT_H

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
 *  EthTSyn_SlaveRx_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the SlaveRx unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_Reset
 *********************************************************************************************************************/
/*! \brief      Resets all linked slave rx data.
 *  \details    -
 *  \param[in]  SlaveRxDataHandlerIdx  Index of the SlaveRxDataHandler
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_Reset(
  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdSync
 *********************************************************************************************************************/
/*! \brief      Processing of a received Sync message.
 *  \details    Checks if passed port is ready to receive a Sync message and optionally processes the message.
 *  \param[in]  SlaveRxDataHandlerIdx  Index of the SlaveRxDataHandler mapped to the ComCtrlPort the message was
 *                                     received on
 *  \param[in]  PtpHdrPtr              Pointer to the de-serialized PTP header
 *  \param[in]  RxTsInfoPtr            Info about the reception timestamp(s) as well as optional Ethernet and StbM time
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdSync(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)              RxTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdFup
 *********************************************************************************************************************/
/*! \brief      Processing of a received FollowUp message.
 *  \details    Checks if passed port is ready to receive a FollowUp message and optionally processes the message.
 *  \param[in]  SlaveRxDataHandlerIdx  Index of the SlaveRxDataHandler mapped to the ComCtrlPort the message was
 *                                     received on
 *  \param[in]  PtpHdrPtr              Pointer to the de-serialized PTP header
 *  \param[in]  FupPayloadPtr          Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdFup(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                             FupPayloadPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdAnnounce
 *********************************************************************************************************************/
/*! \brief      Processing of a received Announce message.
 *  \details    Checks if passed port is ready to receive a Announce message and optionally processes the message.
 *  \param[in]  SlaveRxDataHandlerIdx  Index of the SlaveRxDataHandler mapped to the ComCtrlPort the message was
 *                                     received on
 *  \param[in]  PtpHdrPtr              Pointer to the de-serialized PTP header
 *  \param[in]  AnnouncePayloadPtr     Pointer to the Announce payload (still serialized in Network-Byte-Order)
 *  \pre        Must be called in context of EthTSyn_RxIndication() or EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdAnnounce(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                             AnnouncePayloadPtr);

/**********************************************************************************************************************
 * EthTSyn_SlaveRx_ResetValidMaster
 *********************************************************************************************************************/
/*! \brief          Resets the valid master of the SlaveRxData if valid master information is used.
 *  \details        -
 *  \param[in]      SlaveRxDataIdx  Index of the slave rx data
 *                                  [range: SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ResetValidMaster(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx);

/**********************************************************************************************************************
 * EthTSyn_SlaveRx_SetValidMaster
 *********************************************************************************************************************/
/*! \brief          Sets the valid master of the SlaveRxData if valid master information is used.
 *  \details        -
 *  \param[in]      SlaveRxDataIdx      Index of the slave rx data
 *                                      [range: SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData()]
 *  \param[in]      ValidMasterClockId  Clock id of the new valid master
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_SetValidMaster(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx,
  uint64                      ValidMasterClockId);

/**********************************************************************************************************************
 * EthTSyn_SlaveRx_CheckValidMasterIfSet
 *********************************************************************************************************************/
/*! \brief          Checks if a valid master was set before and if it matches to the passed master clock id.
 *  \details        -
 *  \param[in]      SlaveRxDataIdx      Index of the slave rx data
 *                                      [range: SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData()]
 *  \param[in]      MasterClockId       Master clock id which is supposed to be checked
 *  \return         TRUE - Passed master clock id is identical with valid master
 *  \return         FALSE - No valid master was set yet or passed master clock id is different
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckValidMasterIfSet(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx,
  uint64                      MasterClockId);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */
#endif /* ETHTSYN_SLAVE_RX_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SlaveRx_Int.h
 *********************************************************************************************************************/
